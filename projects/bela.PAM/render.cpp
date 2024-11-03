#include <Bela.h>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Convolver/Convolver.h>
#include "sources/math.h"
#include "sources/amp.h"

//Constant Declaration
const int 			NEURAL_NETWORK_HIDDEN_SIZE  = 8;
const std::string 	IMPULSE_RESPONSE_PATH 		= "ressources/impulses_responses/final_IR_1024.wav";
const int 			BLACKBIRD_POLARITY 			= 1;
const int 			CONSTABLE_POLARITY 			= -1;
const float 		BLACKBIRD_INPUT_GAIN 		= db2linear<float>(-12)*BLACKBIRD_POLARITY;
const float 		CONSTABLE_INPUT_GAIN 		= db2linear<float>(0)*CONSTABLE_POLARITY;
const float 		OUTPUT_GAIN 				= db2linear<float>(-12);

//Instanciation of main dsp objects
Amp<float,NEURAL_NETWORK_HIDDEN_SIZE> theAmp;
Convolver theCabinet;

bool setup(BelaContext *context, void *userData)
{
	theCabinet.setup(IMPULSE_RESPONSE_PATH, context->audioFrames, 1024);
	theAmp.setup();
	return true;
}

void render(BelaContext *context, void *userData)
{
	float buffer[context->audioFrames];
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		// Sum both input
		buffer[n] = audioRead(context, n, 0)*BLACKBIRD_INPUT_GAIN
				  + audioRead(context, n, 1)*CONSTABLE_INPUT_GAIN;
		// Power Amp Simulation
		buffer[n] = theAmp.process(&buffer[n])*OUTPUT_GAIN;
	}
	// Linear Speaker Simulation
	theCabinet.process(buffer, buffer, context->audioFrames);
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		audioWrite(context, n, 0, buffer[n]);
		audioWrite(context, n, 1, buffer[n]);
	}
}

void cleanup(BelaContext *context, void *userData)
{

}
