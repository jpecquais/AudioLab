#ifndef ZERO_LATENCY_CONVOLUTION_TPP
#define ZERO_LATENCY_CONVOLUTION_TPP

#include "zero_latency_convolution.hpp"

ZeroLatencyConvolution::~ZeroLatencyConvolution()
{
    if (convolution_)
    {
        conv_destroy(convolution_);
    }
}

bool ZeroLatencyConvolution::init(const float* impulseResponse, int ir_length, int block_size)
{
    if (ir_length < head_ir_length_) return false;

    block_size_ = block_size;
    ir_length_ = ir_length;

    tail_ir_length_ = ir_length_ - head_ir_length_;
    head_convolution_buffer_length_ = block_size_ + head_ir_length_ - 1;

    head_ir_buffer_.resize(head_ir_length_);
    tail_ir_buffer_.resize(tail_ir_length_);

    head_convolution_buffer.resize(head_convolution_buffer_length_);

    in_buffer_.resize(block_size_);

    for (size_t i = 0; i < head_ir_length_; i++)
    {
        head_ir_buffer_[i] = impulseResponse[i];
    }

    for (size_t i = 0; i < tail_ir_length_; i++)
    {
        tail_ir_buffer_[i] = impulseResponse[head_ir_length_+i];
    }
    std::fill(head_convolution_buffer.begin(), head_convolution_buffer.end(), 0.0f);
    convolution_ = conv_new_uniform_partitioning(tail_ir_buffer_.data(), tail_ir_length_, head_ir_length_, output_callback, this);
    if (!convolution_) return false;

    return true;
}

void ZeroLatencyConvolution::process(const float* input, float* output)
{
    current_output_target_ = output;
    for (size_t i = 0; i < block_size_; i++)
    {
        in_buffer_[i] = input[i];
    }
    conv_process(convolution_, in_buffer_.data(), block_size_);

    for (size_t i = 0; i < head_ir_length_-1; ++i)
    {
        output[i] += head_convolution_buffer[block_size_+i];
    }

    convolve_time_domain_section();
    for (size_t i = 0; i < block_size_; ++i)
    {
        output[i] += head_convolution_buffer[i];
    }
}

void ZeroLatencyConvolution::reset() {}

void ZeroLatencyConvolution::output_callback(void* SELF, dft_sample_t* output, int num_samples)
{
    ZeroLatencyConvolution* self = static_cast<ZeroLatencyConvolution*>(SELF);
    for (int i = 0; i < num_samples-1; ++i)
    {
        self->current_output_target_[i] += output[i];
    }
}

void ZeroLatencyConvolution::convolve_time_domain_section()
{
    std::fill(head_convolution_buffer.begin(), head_convolution_buffer.end(), 0.0f);

    for (int j = 0; j < head_ir_length_; ++j)
    {
        auto current_ir_sample = head_ir_buffer_[j];
        for (int i = 0; i < block_size_; ++i)
        {
            head_convolution_buffer[i+j] += in_buffer_[i]*current_ir_sample;
        }
    }
}

#endif // ZERO_LATENCY_CONVOLUTION_TPP