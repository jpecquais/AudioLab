#ifndef ZERO_LATENCY_CONVOLUTION_H
#define ZERO_LATENCY_CONVOLUTION_H

#include <cstddef>
#include <algorithm>
#include "../lib/Partitioned-Convolution/lib/Convolution.h"

template <int block_size, int ir_length>
class ZeroLatencyConvolution
{
public:
    ZeroLatencyConvolution() = default;
    ~ZeroLatencyConvolution()
    {
        if (convolution_) {
            conv_destroy(convolution_);
        }
    }

    ZeroLatencyConvolution(const ZeroLatencyConvolution&) = delete;
    ZeroLatencyConvolution(ZeroLatencyConvolution&&) = delete;
    ZeroLatencyConvolution& operator=(const ZeroLatencyConvolution&) = delete;
    ZeroLatencyConvolution& operator=(ZeroLatencyConvolution&&) = delete;

    // Initialize the convolution with necessary parameters
    bool init(const float* impulseResponse)
    {
        std::copy(impulseResponse, impulseResponse + head_ir_length_, head_ir_buffer_);
        std::copy(impulseResponse+head_ir_length_, impulseResponse + ir_length, tail_ir_buffer_);
        std::fill(head_convolution_buffer, head_convolution_buffer + head_convolution_buffer_length_, 0.0f);
        convolution_ = conv_new_optimum_partitioning(tail_ir_buffer_, tail_ir_length_, head_ir_length_, output_callback, this);
        if (!convolution_) return false;

        return true;
    }
    
    // Process a block of input samples and produce output samples
    void process(const float* input, float* output)
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

    // Reset the internal state of the convolution
    void reset(){}

private:
    // Internal data structures for managing the non-uniform partitioned convolution
    Convolution* convolution_ = nullptr;
    float* current_output_target_ = nullptr;

    static constexpr size_t head_ir_length_ = 64;
    size_t tail_ir_length_ = ir_length - head_ir_length_;
    size_t head_convolution_buffer_length_ = block_size + head_ir_length_ - 1;

    float head_ir_buffer_[head_ir_length_];
    float tail_ir_buffer_[tail_ir_length_];

    float head_convolution_buffer[head_convolution_buffer_length_];

    float in_buffer_[block_size];

    static void output_callback(void* SELF, dft_sample_t* output, int num_samples)
    {
        ZeroLatencyConvolution* self = static_cast<ZeroLatencyConvolution*>(SELF);
        std::copy(output, output + num_samples, self->current_output_target_);
    }

    void convolve_time_domain_section()
    {
        std::fill(head_convolution_buffer_, head_convolution_buffer_ + head_convolution_buffer_length_, 0.0f);

        for (int j = 0; j < head_ir_length_; ++j)
        {
            auto current_ir_sample = head_ir_buffer_[j];
            for (int i = 0; i < block_size; ++i)
            {
                head_convolution_buffer[i+j] += in_buffer_[i]*current_ir_sample;
            }
        }
    }
};

#endif // ZERO_LATENCY_CONVOLUTION_H