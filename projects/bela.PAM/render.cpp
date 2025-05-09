#include <memory>
#include <Bela.h>
#include <libraries/Convolver/Convolver.h>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Midi/Midi.h>
#include <libraries/AudioFile/AudioFile.h>
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
static constexpr unsigned int	MAX_IMPULSE_LENGTH			= 256; //Currently, it is the safe spot
static constexpr unsigned int 	NEURAL_NETWORK_HIDDEN_SIZE  = 8;
static constexpr int 			MIDI_CH						= 5;
static constexpr int 			BELA_MIDI_CH				= MIDI_CH-1;  //Bela count midi channel from 0
static const	 float	 		OUTPUT_GAIN 				= db2linear<float>(-12.f);
static const	 std::string	MIDI_PORT					= "hw:1,0,0";
static const	 std::string 	IMPULSE_RESPONSE_PATH 		= "ressources/impulses_responses/final_IR_1024.wav";

enum CHANNEL{
	LEFT = 0,
	RIGHT = 1,
	STEREO = 2
};

static float** theBuffer = nullptr; //main buffer
static float** bypassBuffer = nullptr; //to bypass effects.

//Instanciation of main dsp objects
#ifdef DEBUG_AUDIO
Oscillator osc;
Scope scope;
#endif

static InputSection<float> theInputSection;
static Amp<float,NEURAL_NETWORK_HIDDEN_SIZE> theAmp;
static Convolver theCabinet;
static PamRotaryEffect theRotary;
static Gain<float,CHANNEL::STEREO> theOutputGain;

//Define DSP Controller
static MapUI theUI;


//Define MIDI
static Midi theMidi;
using pParameter = std::unique_ptr<IParameter<float>>;
static std::array<pParameter,128> Parameters;
static void midiCallback(MidiChannelMessage message, void *arg);

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

	Parameters[0] = std::make_unique<FAUSTParameter<float>>(&theUI,"mix",20.f,0.f,100.f);
	Parameters[1] = std::make_unique<FAUSTParameter<float>>(&theUI,"slow_fast",0.f,0.f,1.f);
	Parameters[2] = std::make_unique<FAUSTParameter<float>>(&theUI,"break",0.f,0.f,1.f);
	Parameters[3] = std::make_unique<Parameter<float>>("bypass_amp",0,0,1);
	Parameters[4] = std::make_unique<CallbackParameter<float, std::function<void(float)>>>([](float new_value){theInputSection.toggle_channels(new_value);},"toggle_channels",0,0,1);
	Parameters[7] = std::make_unique<Parameter<float>>("OutputGain",1.f,0.f,1.f);
	
	//Init dsp blocks
	theInputSection.setup(current_sample_rate,current_buffer_size);
	theAmp.setup(current_buffer_size);
	theCabinet.setup(IMPULSE_RESPONSE_PATH, current_buffer_size, MAX_IMPULSE_LENGTH);
	theRotary.init(current_sample_rate);
	theRotary.buildUserInterface(&theUI);
	theOutputGain.setup(Parameters[7]->get(),current_buffer_size,0.7);

	//allocate buffer
	theBuffer = new float*[CHANNEL::STEREO];
	for (size_t ch = 0; ch < CHANNEL::STEREO; ch++)
	{
		theBuffer[ch] = new float[current_buffer_size];
		for (size_t smpl = 0; smpl < current_buffer_size; smpl++)
		{
			theBuffer[ch][smpl] = 0;
		}

	}

	return true;
}

void render(BelaContext *context, void *userData)
{
	bela_uninterleaved_input_buffer<float>(context->audioIn,theBuffer,CHANNEL::STEREO,context->audioFrames);
	theInputSection.process(theBuffer,theBuffer); //1-2% BELA CPU
	if(Parameters[3]->getValue()==0)
	{
		theAmp.process(theBuffer,theBuffer); // 50-55% BELA CPU
		theCabinet.process(theBuffer[0],theBuffer[0],context->audioFrames); // 20% BELA CPU
	}
	theRotary.compute(context->audioFrames,theBuffer,theBuffer);
	theOutputGain.process(theBuffer,theBuffer); //Almost uncountable.
	
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		#ifdef DEBUG_AUDIO
			scope.log(theBuffer[CHANNEL::LEFT][n], theBuffer[CHANNEL::RIGHT][n]);
			audioWrite(context, n, CHANNEL::LEFT, osc.process()*OUTPUT_GAIN);
			audioWrite(context, n, CHANNEL::RIGHT, osc.process()*OUTPUT_GAIN);
		#else
			audioWrite(context, n, CHANNEL::LEFT, theBuffer[CHANNEL::LEFT][n]*OUTPUT_GAIN);
			audioWrite(context, n, CHANNEL::RIGHT, theBuffer[CHANNEL::RIGHT][n]*OUTPUT_GAIN);
		#endif
	}
}

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
	}
}