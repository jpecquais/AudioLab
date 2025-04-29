#ifndef ZERO_LATENCY_CONVOLUTION_TPP
#define ZERO_LATENCY_CONVOLUTION_TPP

#include "ZeroLatencyConvolution.hpp"
#include <algorithm> // For std::copy, std::fill
#include <cstddef>   // For size_t if needed standalone (though hpp includes it)
#include "../lib/Partitioned-Convolution/lib/Convolution.h" // For Convolution types/functions

template <int block_size, int ir_length>
ZeroLatencyConvolution<block_size, ir_length>::~ZeroLatencyConvolution()
{
    if (convolution_)
    {
        conv_destroy(convolution_);
    }
}

template <int block_size, int ir_length>
bool ZeroLatencyConvolution<block_size, ir_length>::init(const float* impulseResponse)
{
    std::copy(impulseResponse, impulseResponse + head_ir_length_, head_ir_buffer_);
    std::copy(impulseResponse+head_ir_length_, impulseResponse + ir_length, tail_ir_buffer_);
    std::fill(head_convolution_buffer, head_convolution_buffer + head_convolution_buffer_length_, 0.0f);
    convolution_ = conv_new_optimum_partitioning(tail_ir_buffer_, tail_ir_length_, head_ir_length_, output_callback, this);
    if (!convolution_) return false;

    return true;
}

template <int block_size, int ir_length>
void ZeroLatencyConvolution<block_size, ir_length>::process(const float* input, float* output)
{
    current_output_target_ = output;
    std::copy(input, input + block_size, in_buffer_);
    conv_process(convolution_, in_buffer_, block_size);
    for (int i = 0; i < head_ir_length_-1; ++i)
    {
        output[i] += head_convolution_buffer[block_size+i];
    }
    convolve_time_domain_section();
    for (int i = 0; i < block_size; ++i)
    {
        output[i] += head_convolution_buffer[i];
    }
}

template <int block_size, int ir_length>
void ZeroLatencyConvolution<block_size, ir_length>::reset() {}

template <int block_size, int ir_length>
void ZeroLatencyConvolution<block_size, ir_length>::output_callback(void* SELF, dft_sample_t* output, int num_samples)
{
    ZeroLatencyConvolution* self = static_cast<ZeroLatencyConvolution*>(SELF);
    std::copy(output, output + num_samples, self->current_output_target_);
}

template <int block_size, int ir_length>
void ZeroLatencyConvolution<block_size, ir_length>::convolve_time_domain_section()
{
    std::fill(head_convolution_buffer, head_convolution_buffer + head_convolution_buffer_length_, 0.0f);

    for (int j = 0; j < head_ir_length_; ++j)
    {
        auto current_ir_sample = head_ir_buffer_[j];
        for (int i = 0; i < block_size; ++i)
        {
            head_convolution_buffer[i+j] += in_buffer_[i]*current_ir_sample;
        }
    }
}

#endif // ZERO_LATENCY_CONVOLUTION_TPP