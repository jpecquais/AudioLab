#include <memory>
#include <Bela.h>
#include <libraries/Convolver/Convolver.h>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Midi/Midi.h>
#include "sources/math.h"
#include "sources/amp.h"
#include "sources/input.h"
#include "sources/PamRotaryEffect.h"
#include "sources/Parameter.h"
#include "sources/buffer_helper.h"
#include "sources/gain.h"


//Debug mode
// #define DEBUG_AUDIO
// #define DEBUG_CTRL
#ifdef DEBUG_AUDIO
#include <libraries/Oscillator/Oscillator.h>
#include <libraries/Scope/Scope.h>
#endif

//Constant Declaration
static constexpr unsigned int	MAX_IMPULSE_LENGTH			= 256;
static constexpr unsigned int 	NEURAL_NETWORK_HIDDEN_SIZE  = 8;
static constexpr int 			BLACKBIRD_POLARITY 			= 1;
static constexpr int 			CONSTABLE_POLARITY 			= -1;
static constexpr int 			MIDI_CH						= 5;
static constexpr int 			BELA_MIDI_CH				= MIDI_CH-1;  //Bela count midi channel from 0
static const	 float	 		OUTPUT_GAIN 				= db2linear<float>(-12.f);
static const 	 float	 		BLACKBIRD_INPUT_GAIN 		= db2linear<float>(-12.f)*BLACKBIRD_POLARITY;
static const	 float	 		CONSTABLE_INPUT_GAIN 		= db2linear<float>(0.f)*CONSTABLE_POLARITY;
static const	 std::string	MIDI_PORT					= "hw:1,0,0";
static const	 std::string 	IMPULSE_RESPONSE_PATH 		= "ressources/impulses_responses/final_IR_1024.wav";

enum CHANNEL{
	LEFT = 0,
	RIGHT = 1,
	STEREO = 2
};

static float** theBuffer = nullptr; //main buffer
static float** bypassBuffer = nullptr; //to bypass effects.

// Additionnal pointers to handle dynamic routing
static float** amp_input_buffer = nullptr;
static float** amp_output_buffer = nullptr;
static float** cabinet_input_buffer = nullptr;
static float** cabinet_output_buffer = nullptr;

//Instanciation of main dsp objects
#ifdef DEBUG
Oscillator osc;
Scope scope;
#endif
static InputSection<float> theInputSection;
static Amp<float,NEURAL_NETWORK_HIDDEN_SIZE> theAmp;
static Convolver theCabinet;
static PamRotaryEffect theRotary;
static Gain<float,CHANNEL::STEREO> theOutputGain;

void BypassStateFunction(int state, float**& input_buffer, float**& output_buffer)
{
    if (state)
    {
        input_buffer = theBuffer;
        output_buffer = theBuffer;
    }
    else
    {
        input_buffer = bypassBuffer;
        output_buffer = bypassBuffer;
    }
}

using BypassCallback = std::function<void(int)>;
BypassCallback set_amp_bypass_state = [](int state)
{
    BypassStateFunction(state, amp_input_buffer, amp_output_buffer);
};
BypassCallback set_cab_bypass_state = [](int state)
{
	BypassStateFunction(state, cabinet_input_buffer, cabinet_output_buffer);
};

using GainCallback = std::function<void(float)>;
GainCallback set_output_gain = [](float new_val)
{
	theOutputGain.set(new_val);
};
static float totalOutputGain = OUTPUT_GAIN;

//Define DSP Controller
static MapUI theUI;
static FAUSTParameter<float> mix(&theUI,"mix",20.f,0.f,100.f);
static FAUSTParameter<float> slowFastMode(&theUI,"slow_fast",0.f,0.f,1.f);
static FAUSTParameter<float> breakMode(&theUI,"break",0.f,0.f,1.f);
static CallbackParameter<float,GainCallback> outputGain(set_output_gain,"OutputGain",1.f,0.f,1.f);
static CallbackParameter<float,BypassCallback> amp_bypass(set_amp_bypass_state,"bypass_amp",0,0,1);
static CallbackParameter<float,BypassCallback> cab_bypass(set_cab_bypass_state,"bypass_cab",0,0,1);

//Define MIDI
static Midi theMidi;
static std::array<IParameter<float>*,128> Parameters;
static void midiCallback(MidiChannelMessage message, void *arg);

//Forward declaration
void computeOutputGain(); // <-- Callback to compute the output gain.
						  // 	 Should optimize the CPU load in the render function

/**
 * @brief Initializes the audio processing setup.
 *
 * This function sets up the audio context, initializes the input section,
 * amplifier, convolver, and rotary effect. It also sets up MIDI communication
 * and binds parameters to MIDI CC numbers.
 * 
 * To limit the number of computation in the main render function, the output gain
 * is computed inside a callback. We need to attach the callback to the parameters
 * that have an effect on the output gain.
 *
 * @param context Pointer to the BelaContext containing audio settings.
 * @param userData Pointer to user-defined data.
 * @return true if the setup is successful, false otherwise.
 */
bool setup(BelaContext *context, void *userData)
{
	#ifdef DEBUG_AUDIO
	osc.setup(context->audioSampleRate, Oscillator::sine);
	osc.setFrequency(440);
	osc.setPhase(0);
	scope.setup(2, context->audioSampleRate);
	#endif

	auto current_sample_rate = context->audioSampleRate;
	auto current_buffer_size = context->audioFrames;

	//MIDI setup
	theMidi.readFrom(MIDI_PORT.c_str());
	theMidi.writeTo(MIDI_PORT.c_str());
	theMidi.enableParser(true);

	//Attach parameter to MidiControler
	theMidi.setParserCallback(&midiCallback, (void *)MIDI_PORT.c_str());	


	//Bind parameters to midi CC number.
	ccToParameters[0] = &mix;
	ccToParameters[1] = &slowFastMode;
	ccToParameters[2] = &breakMode;
	ccToParameters[3] = &amp_bypass;
	ccToParameters[4] = &cab_bypass;
	ccToParameters[7] = &outputGain;

	//Init dsp blocks
	theInputSection.setup(current_sample_rate,current_buffer_size,BLACKBIRD_INPUT_GAIN,CONSTABLE_INPUT_GAIN);
	theAmp.setup(current_buffer_size);
	theCabinet.setup(IMPULSE_RESPONSE_PATH, current_buffer_size, MAX_IMPULSE_LENGTH);
	theRotary.init(current_sample_rate);
	theRotary.buildUserInterface(&theUI);
	theOutputGain.setup(current_buffer_size,0.7);

	//allocate buffers
	theBuffer = new float*[CHANNEL::STEREO];
	bypassBuffer = new float*[CHANNEL::STEREO];
	for (int i = 0; i < CHANNEL::STEREO; i++) {
		theBuffer[i] = new float[current_buffer_size];
		bypassBuffer[i] = new float[current_buffer_size];
	}

	amp_input_buffer = theBuffer;
	amp_output_buffer = theBuffer;
	cabinet_input_buffer = theBuffer;
	cabinet_output_buffer = theBuffer;

	return true;
}

/**
 * @brief Processes the audio in real-time.
 *
 * This function handles the audio processing for each frame. It reads audio input,
 * processes it through the input section, amplifier, convolver, and rotary effect,
 * and writes the output to the audio buffer.
 *
 * @param context Pointer to the BelaContext containing audio settings.
 * @param userData Pointer to user-defined data.
 */
void render(BelaContext *context, void *userData)
{
	bela_uninterleaved_input_buffer<float>(context->audioIn,theBuffer,CHANNEL::STEREO,context->audioFrames);
	theInputSection.process(theBuffer,theBuffer);
	theAmp.process(amp_input_buffer,amp_output_buffer);
	theCabinet.process(cabinet_input_buffer[0],cabinet_output_buffer[0],context->audioFrames);
	theRotary.compute(context->audioFrames,theBuffer,theBuffer);
	theOutputGain.process(theBuffer,theBuffer);
	
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		#ifdef DEBUG_AUDIO
		scope.log(theBuffer[CHANNEL::LEFT][n], theBuffer[CHANNEL::RIGHT][n]);
		#endif

		audioWrite(context, n, CHANNEL::LEFT, theBuffer[CHANNEL::LEFT][n]*OUTPUT_GAIN);
		audioWrite(context, n, CHANNEL::RIGHT, theBuffer[CHANNEL::RIGHT][n]*OUTPUT_GAIN);
	}
}

/**
 * @brief Cleans up allocated resources for audio buffers.
 *
 * This function is called to release memory allocated for the audio buffers
 * used during the program's execution. It ensures that all dynamically
 * allocated memory is properly deallocated to prevent memory leaks.
 *
 * @param context A pointer to the BelaContext structure, which contains
 *                information about the audio and sensor context. This parameter
 *                is not used in this function.
 * @param userData A pointer to user-defined data. This parameter is not used
 *                 in this function.
 *
 * The function performs the following cleanup steps:
 * - Iterates over the stereo channels and deletes the memory allocated for
 *   `theBuffer` and `bypassBuffer` for each channel.
 * - Sets the pointers for each channel in `theBuffer` and `bypassBuffer` to `nullptr`.
 * - Deletes the arrays `theBuffer` and `bypassBuffer`.
 * - Sets the pointers `theBuffer` and `bypassBuffer` to `nullptr`.
 */
void cleanup(BelaContext *context, void *userData)
{
	for (int i=0; i<CHANNEL::STEREO;i++){
		delete[] theBuffer[i];
		delete[] bypassBuffer[i];
		theBuffer[i] = nullptr;
		bypassBuffer[i] = nullptr;
	}
	delete[] theBuffer;
	delete[] bypassBuffer;
	theBuffer = nullptr;
	bypassBuffer = nullptr;
}


/**
 * @brief Handles MIDI messages.
 *
 * This function processes MIDI messages and updates the corresponding parameters
 * based on the MIDI CC numbers.
 * 
 * The CC numbers correspond to the index of the parameter in the parameters list.
 *
 * @param message The MIDI message received.
 * @param arg Pointer to additional arguments (typically the MIDI port).
 */
void midiCallback(MidiChannelMessage message, void *arg){
	#ifdef DEBUG_CTRL
	rt_printf("MIDI Channel: %i \n",message.getChannel());
	#endif
	if (message.getChannel() != BELA_MIDI_CH) return;
	if (message.getChannel() != BELA_MIDI_CH) return;
	if (message.getType() == kmmControlChange){
		#ifdef DEBUG_CTRL
		rt_printf("MIDI CC Message: %i \n",message.getDataByte(0));
		#endif
		auto& currParam = Parameters[message.getDataByte(0)];
		if (currParam == nullptr) return;
		#ifdef DEBUG_CTRL
		rt_printf("Is a valid CC\n");
		#endif
		currParam->setValueFromMidi(message.getDataByte(1));
		if (currParam->hasCallback()) currParam->invokeCallback();
	}
}


/**
 * @brief Computes the global output gain.
 *
 * This function calculates the global output gain based on the current values
 * of the output gain and drive parameters.
 */
void computeOutputGain(){
	totalOutputGain = OUTPUT_GAIN*outputGain.getValue()/drive.getValue();
}