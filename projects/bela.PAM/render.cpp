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

//Debug mode
//#define DEBUG
#ifdef DEBUG
	#include <libraries/Oscillator/Oscillator.h>
	#include <libraries/Scope/Scope.h>
#endif

//Constant Declaration
static constexpr unsigned int	MAX_IMPULSE_LENGTH			= 256;
static constexpr unsigned int 	NEURAL_NETWORK_HIDDEN_SIZE  = 8;
static constexpr int 			BLACKBIRD_POLARITY 			= 1;
static constexpr int 			CONSTABLE_POLARITY 			= -1;
static constexpr int 			MIDI_CH						= 5;
static constexpr int 			BELA_MIDI_CH 				= MIDI_CH-1; //Bela count midi channel from zero !!!
static const	 float	 		OUTPUT_GAIN 				= db2linear<float>(-12.f);
static const 	 float	 		BLACKBIRD_INPUT_GAIN 		= db2linear<float>(-12.f)*BLACKBIRD_POLARITY;
static const	 float	 		CONSTABLE_INPUT_GAIN 		= db2linear<float>(0.f)*CONSTABLE_POLARITY;
static const	 std::string	MIDI_PORT					= "hw:0,0,0";
static const	 std::string 	IMPULSE_RESPONSE_PATH 		= "ressources/impulses_responses/final_IR_1024.wav";

enum CHANNEL{
	LEFT = 0,
	RIGHT = 1,
	STEREO = 2
};

static float **theBuffer = nullptr; //TODO: should be refactorized with smart pointer.

//Instanciation of main dsp objects
#ifdef DEBUG
	Oscillator osc;
	Scope scope;
#endif
static InputSection<float> theInputSection;
static Amp<float,NEURAL_NETWORK_HIDDEN_SIZE> theAmp;
static Convolver theCabinet;
static PamRotaryEffect theRotary;
static float totalOutputGain = OUTPUT_GAIN;

//Define "UI"
static MapUI theUI;
static Parameter<float> outputGain("OutputGain",1.f,0.f,1.f);
static Parameter<float> drive("drive",1.f,0.f,10.f);
static FAUSTParameter<float> mix(&theUI,"mix",25.f,0.f,100.f);
static FAUSTParameter<float> slowFastMode(&theUI,"slow_fast",0.f,0.f,1.f);
static FAUSTParameter<float> breakMode(&theUI,"break",0.f,0.f,1.f);

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
	#ifdef DEBUG
		osc.setup(context->audioSampleRate, Oscillator::sine);
		osc.setFrequency(440);
		osc.setPhase(0);
		scope.setup(2, context->audioSampleRate);
	#endif
	//Attach callback
	drive.setCallback(&computeOutputGain);
	outputGain.setCallback(&computeOutputGain);

	//MIDI setup
	theMidi.readFrom(MIDI_PORT.c_str());
	theMidi.writeTo(MIDI_PORT.c_str());
	theMidi.enableParser(true);
	theMidi.setParserCallback(&midiCallback, (void *)MIDI_PORT.c_str());	

	//Bind parameters to midi CC number.
	Parameters[0] = &mix;
	Parameters[1] = &slowFastMode;
	Parameters[2] = &breakMode;
	Parameters[3] = &drive;
	Parameters[7] = &outputGain;

	//Init dsp blocks
	theInputSection.setup(context->audioSampleRate,BLACKBIRD_INPUT_GAIN,CONSTABLE_INPUT_GAIN);
	theAmp.setup();
	theCabinet.setup(IMPULSE_RESPONSE_PATH, context->audioFrames, MAX_IMPULSE_LENGTH);
	theRotary.init(context->audioSampleRate);
	theRotary.buildUserInterface(&theUI);
	theBuffer = new float*[CHANNEL::STEREO];
	for (int i = 0; i < CHANNEL::STEREO; i++) {
		theBuffer[i] = new float[context->audioFrames];
	}

	for (auto parameter : Parameters){
		if (parameter != nullptr) parameter->reset();
	}

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
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		// Sum both input
		#ifndef DEBUG
			theBuffer[CHANNEL::LEFT][n] = theInputSection.process(audioRead(context, n, CHANNEL::LEFT),
				  												  audioRead(context, n, CHANNEL::RIGHT));
		#else
			theBuffer[CHANNEL::LEFT][n] = theInputSection.process(osc.process()*0.25,0); //sine generator
		#endif
		// Power Amp & Non Linear Speaker Simulation
		theBuffer[CHANNEL::LEFT][n] *= drive.getValue();
		theBuffer[CHANNEL::LEFT][n] = theAmp.process(&theBuffer[CHANNEL::LEFT][n]);// Rest of signal chain is linear, so output gain can be applied here.
		theBuffer[CHANNEL::LEFT][n] *= totalOutputGain;
	}

	// Linear Speaker Simulation
	theCabinet.process(theBuffer[0],theBuffer[0],context->audioFrames);
	// Rotary speaker simulation
	theRotary.compute(context->audioFrames,theBuffer,theBuffer);

	for(unsigned int n = 0; n < context->audioFrames; n++) {
		#ifdef DEBUG
			scope.log(theBuffer[CHANNEL::LEFT][n], theBuffer[CHANNEL::RIGHT][n]);
		#endif
		audioWrite(context, n, CHANNEL::LEFT, theBuffer[CHANNEL::LEFT][n]);
		audioWrite(context, n, CHANNEL::RIGHT, theBuffer[CHANNEL::RIGHT][n]); // Should be the right channel in theBuffer.
	}
}

/**
 * @brief Cleans up resources allocated during setup.
 *
 * This function deallocates the memory used for the audio buffer.
 *
 * @param context Pointer to the BelaContext containing audio settings.
 * @param userData Pointer to user-defined data.
 */
void cleanup(BelaContext *context, void *userData)
{
	for (int i=0; i<CHANNEL::STEREO;i++){
		delete[] theBuffer[i];
		theBuffer[i] = nullptr;
	}
	delete[] theBuffer;
	theBuffer = nullptr;
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
	#ifdef DEBUG
		rt_printf("MIDI Channel: %i \n",message.getChannel());
	#endif
	if (message.getChannel() != BELA_MIDI_CH) return;
	if (message.getType() == kmmControlChange){
		#ifdef DEBUG
			rt_printf("MIDI CC Message: %i \n",message.getDataByte(0));
		#endif
		auto& currParam = Parameters[message.getDataByte(0)];
		if (currParam == nullptr) return;
		#ifdef DEBUG
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