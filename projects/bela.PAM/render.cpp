#include <memory>
#include <Bela.h>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Convolver/Convolver.h>
#include "sources/math.h"
#include "sources/amp.h"
#include "sources/input.h"
#include "sources/PamRotaryEffect.h"
#include "sources/Parameter.h"
#include "sources/MidiController.h"

//Debug mode
// #define DEBUG
#ifdef DEBUG
	#include <libraries/Oscillator/Oscillator.h>
	#include <libraries/Scope/Scope.h>
#endif

//Constant Declaration
const int 			NEURAL_NETWORK_HIDDEN_SIZE  = 8;
const std::string 	IMPULSE_RESPONSE_PATH 		= "ressources/impulses_responses/final_IR_1024.wav";
const int 			BLACKBIRD_POLARITY 			= 1;
const int 			CONSTABLE_POLARITY 			= -1;
const float 		BLACKBIRD_INPUT_GAIN 		= db2linear<float>(-12)*BLACKBIRD_POLARITY;
const float 		CONSTABLE_INPUT_GAIN 		= db2linear<float>(0)*CONSTABLE_POLARITY;
const float 		OUTPUT_GAIN 				= db2linear<float>(-12);

enum CHANNEL{
	LEFT = 0,
	RIGHT = 1,
	STEREO = 2
};

float **theBuffer; //TODO: should be refactorized with smart pointer.

//Instanciation of main dsp objects
#ifdef DEBUG
	Oscillator osc;
	Scope scope;
#endif
InputSection<float, 44100> theInputSection;
Amp<float,NEURAL_NETWORK_HIDDEN_SIZE> theAmp;
Convolver theCabinet;
PamRotaryEffect theRotary;

//Define "UI"
std::shared_ptr<MapUI> theUI;
MidiController<float> theMidiController;
Parameter<float> outputGain("OutputGain",1.,0.,1.);
FAUSTParameter<float> slowRotationSpeed(theUI,"slow_rotation_speed",1.,0.1,4.);
FAUSTParameter<float> fastRotationSpeed(theUI,"fast_rotation_speed",7.,4.,10.);
FAUSTParameter<float> mix(theUI,"mix",50.,0.,100.);
FAUSTParameter<float> slowFastMode(theUI,"slow_fast",0,1,0);
FAUSTParameter<float> breakMode(theUI,"break",0,1,0);


bool setup(BelaContext *context, void *userData)
{
	#ifdef DEBUG
		osc.setup(context->audioSampleRate, Oscillator::sine);
		osc.setFrequency(440);
		osc.setPhase(0);
		scope.setup(2, context->audioSampleRate);
	#endif

	theMidiController.setup("hw:1,0,0");
	//Attach parameter to MidiControler
	theMidiController.attachParameterToCC(slowRotationSpeed,0);
	theMidiController.attachParameterToCC(fastRotationSpeed,1);
	theMidiController.attachParameterToCC(mix,2);
	theMidiController.attachParameterToCC(slowFastMode,3);
	theMidiController.attachParameterToCC(breakMode,4);
	theMidiController.attachParameterToCC(outputGain,7);

	//Init dsp blocks
	theInputSection.setup(BLACKBIRD_INPUT_GAIN,CONSTABLE_INPUT_GAIN);
	theCabinet.setup(IMPULSE_RESPONSE_PATH, context->audioFrames, 1024);
	theAmp.setup();
	theRotary.init(context->audioSampleRate);
	theBuffer = new float*[CHANNEL::STEREO];
	for (int i = 0; i < CHANNEL::STEREO; i++) {
		theBuffer[i] = new float[context->audioFrames];
	}
	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		// Sum both input
		#ifndef DEBUG
			theBuffer[CHANNEL::LEFT][n] = theInputSection.process(audioRead(context, n, CHANNEL::LEFT),
				  												  audioRead(context, n, CHANNEL::RIGHT));
		#else
			theBuffer[CHANNEL::LEFT][n] = theInputSection.process(osc.process()*0.25,0); //TODO: sine generator
		#endif
		// Power Amp Simulation
		theBuffer[CHANNEL::LEFT][n] = theAmp.process(&theBuffer[CHANNEL::LEFT][n])*OUTPUT_GAIN; // Rest of signal chain is linear, so output gain can be applied here.
	}
	// Linear Speaker Simulation
	theCabinet.process(theBuffer[0], theBuffer[0], context->audioFrames);
	theRotary.compute(context->audioFrames,theBuffer,theBuffer);
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		#ifdef DEBUG
			scope.log(theBuffer[CHANNEL::LEFT][n], theBuffer[CHANNEL::LEFT][n]);
		#endif
		audioWrite(context, n, CHANNEL::LEFT, theBuffer[CHANNEL::LEFT][n]);
		audioWrite(context, n, CHANNEL::RIGHT, theBuffer[CHANNEL::LEFT][n]); // Should be the right channel in theBuffer.
	}
}

void cleanup(BelaContext *context, void *userData)
{
	for (int i=0; i<CHANNEL::STEREO;i++){
		delete theBuffer[i];
	}
	delete theBuffer;
}
