#include "daisy_seed.h"
#include "sources/miniPAM.h"
// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hardware
static constexpr int NUM_CHANNELS = 2;

static DaisySeed hw;
static MiniPAM theAmp;
static MapUI theFaustUI;

// DSP STATES
struct DspState{
    const float sample_rate;
    const int block_size;
};

static constexpr DspState dsp_state = {48000.f,64};
static float** internal_buffer;

static void uninterleave_buffer(const float* source_buffer, float** target_buffer, int num_ch, int block_size)
{
    for (int ch = 0; ch < num_ch; ch++)
    {
        for (int smpl = 0; smpl < block_size; smpl++)
        {
            target_buffer[ch][smpl] = source_buffer[num_ch*smpl+ch];
        }
    }
}

static void interleave_buffer(float** source_buffer, float* target_buffer, int num_ch, int block_size)
{
    for (int ch = 0; ch < num_ch; ch++)
    {
        for (int smpl = 0; smpl < block_size; smpl++)
        {
            target_buffer[num_ch*smpl+ch] = source_buffer[ch][smpl];
        }
    }
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
    uninterleave_buffer(in,internal_buffer,dsp_state.block_size,NUM_CHANNELS);
    theAmp.compute(hw.AudioBlockSize(),internal_buffer,internal_buffer);
    interleave_buffer(internal_buffer,out,dsp_state.block_size,NUM_CHANNELS);
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hw.SetAudioBlockSize(dsp_state.block_size);
    hw.SetAudioSampleRate(daisy::SaiHandle::Config::SampleRate::SAI_48KHZ); // Quick but dirty
    hw.Configure();
    hw.Init();

    theAmp.init(dsp_state.sample_rate);
	theAmp.buildUserInterface(&theFaustUI);

    for (int ch = 0; ch < NUM_CHANNELS; ch++)
    {
        internal_buffer[ch] = new float[dsp_state.block_size];
        for (int smpl = 0; smpl < dsp_state.block_size; smpl++)
        {
            internal_buffer[ch][smpl] = 0;
        }
    } 
    // start callback
    hw.StartAudio(AudioCallback);

    while(1) {}
}
