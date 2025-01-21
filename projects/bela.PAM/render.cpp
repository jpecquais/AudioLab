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
// #define DEBUG
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

//Define "UI"
static MapUI theUI;
static Parameter<float> outputGain("OutputGain",1.f,0.f,1.f,.1f);
static FAUSTParameter<float> mix(&theUI,"mix",50.f,0.f,100.f,0.f);
static FAUSTParameter<float> slowFastMode(&theUI,"slow_fast",0.f,0.f,1.f,0.f);
static FAUSTParameter<float> breakMode(&theUI,"break",0.f,0.f,1.f,0.f);

//Define MIDI
static Midi theMidi;
static std::array<IParameter<float>*,128> ccToParameters;
static void midiCallback(MidiChannelMessage message, void *arg);

bool setup(BelaContext *context, void *userData)
{
	#ifdef DEBUG
		osc.setup(context->audioSampleRate, Oscillator::sine);
		osc.setFrequency(440);
		osc.setPhase(0);
		scope.setup(2, context->audioSampleRate);
	#endif

	theMidi.readFrom(MIDI_PORT.c_str());
	theMidi.writeTo(MIDI_PORT.c_str());
	theMidi.enableParser(true);
	//Attach parameter to MidiControler
	theMidi.setParserCallback(&midiCallback, (void *)MIDI_PORT.c_str());	
	//Bind parameters to midi CC number.
	ccToParameters[0] = &mix;
	ccToParameters[1] = &slowFastMode;
	ccToParameters[2] = &breakMode;
	ccToParameters[7] = &outputGain;

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
	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		mix.updateValue();
		slowFastMode.updateValue();
		breakMode.updateValue();
		outputGain.updateValue();
		// Sum both input
		#ifndef DEBUG
			theBuffer[CHANNEL::LEFT][n] = theInputSection.process(audioRead(context, n, CHANNEL::LEFT),
				  												  audioRead(context, n, CHANNEL::RIGHT));
		#else
			theBuffer[CHANNEL::LEFT][n] = theInputSection.process(osc.process()*0.25,0); //sine generator
		#endif
		// Power Amp & Speaker Simulation
		theBuffer[CHANNEL::LEFT][n] = theAmp.process(&theBuffer[CHANNEL::LEFT][n])*OUTPUT_GAIN*outputGain.getValue(); // Rest of signal chain is linear, so output gain can be applied here.
	}
	// Linear Speaker Simulation
	theCabinet.process(theBuffer[0],theBuffer[0],context->audioFrames);
	theRotary.compute(context->audioFrames,theBuffer,theBuffer);

	for(unsigned int n = 0; n < context->audioFrames; n++) {
		#ifdef DEBUG
			scope.log(theBuffer[CHANNEL::LEFT][n], theBuffer[CHANNEL::RIGHT][n]);
		#endif
		audioWrite(context, n, CHANNEL::LEFT, theBuffer[CHANNEL::LEFT][n]);
		audioWrite(context, n, CHANNEL::RIGHT, theBuffer[CHANNEL::RIGHT][n]); // Should be the right channel in theBuffer.
	}
}

void cleanup(BelaContext *context, void *userData)
{
	for (int i=0; i<CHANNEL::STEREO;i++){
		delete[] theBuffer[i];
		theBuffer[i] = nullptr;
	}
	delete[] theBuffer;
	theBuffer = nullptr;
}

void midiCallback(MidiChannelMessage message, void *arg){
	#ifdef DEBUG
		rt_printf("MIDI Channel: %i \n",message.getChannel());
	#endif
	if (message.getChannel() != MIDI_CH) return;
	if (message.getType() == kmmControlChange){
		#ifdef DEBUG
			rt_printf("MIDI CC Message: %i \n",message.getDataByte(0));
		#endif
		auto& currParam = ccToParameters[message.getDataByte(0)];
		if (currParam == nullptr) return;
		#ifdef DEBUG
			rt_printf("Is a valid CC\n");
		#endif
		currParam->setValueFromMidi(message.getDataByte(1));
	}
}