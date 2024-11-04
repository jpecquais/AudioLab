#include <memory>
#include <Bela.h>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Convolver/Convolver.h>
#include "sources/math.h"
#include "sources/amp.h"
#include "sources/input.h"
#include "sources/PamRotaryEffect.h"

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
InputSection<float, 44100> theInputSection;
Amp<float,NEURAL_NETWORK_HIDDEN_SIZE> theAmp;
Convolver theCabinet;
PamRotaryEffect theRotary;

bool setup(BelaContext *context, void *userData)
{
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
		theBuffer[CHANNEL::LEFT][n] = theInputSection.process(audioRead(context, n, CHANNEL::LEFT),
				  											  audioRead(context, n, CHANNEL::RIGHT));
		// Power Amp Simulation
		theBuffer[CHANNEL::LEFT][n] = theAmp.process(&theBuffer[CHANNEL::LEFT][n])*OUTPUT_GAIN; // Rest of signal chain is linear, so output gain can be applied here.
	}
	// Linear Speaker Simulation
	theCabinet.process(theBuffer[0], theBuffer[0], context->audioFrames);
	theRotary.compute(context->audioFrames,theBuffer,theBuffer);
	for(unsigned int n = 0; n < context->audioFrames; n++) {
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
