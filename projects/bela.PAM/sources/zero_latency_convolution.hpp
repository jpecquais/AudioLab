#ifndef ZERO_LATENCY_CONVOLUTION_HPP
#define ZERO_LATENCY_CONVOLUTION_HPP

#include <cstddef>
#include <algorithm>
#include "../lib/Partitioned-Convolution/lib/Convolution.h"

template <int block_size, int ir_length>
class ZeroLatencyConvolution
{
public:
    ZeroLatencyConvolution() = default;
    ~ZeroLatencyConvolution();

    ZeroLatencyConvolution(const ZeroLatencyConvolution&) = delete;
    ZeroLatencyConvolution(ZeroLatencyConvolution&&) = delete;
    ZeroLatencyConvolution& operator=(const ZeroLatencyConvolution&) = delete;
    ZeroLatencyConvolution& operator=(ZeroLatencyConvolution&&) = delete;

    bool init(const float* impulseResponse);
    void process(const float* input, float* output);
    void reset();

private:
    Convolution* convolution_ = nullptr;
    float* current_output_target_ = nullptr;

    static constexpr size_t head_ir_length_ = 64;
    size_t tail_ir_length_ = ir_length - head_ir_length_;
    size_t head_convolution_buffer_length_ = block_size + head_ir_length_ - 1;

    float head_ir_buffer_[head_ir_length_];
    float tail_ir_buffer_[tail_ir_length_];

    float head_convolution_buffer[head_convolution_buffer_length_];

    float in_buffer_[block_size];

    static void output_callback(void* SELF, dft_sample_t* output, int num_samples);
    void convolve_time_domain_section();
};

#include "ZeroLatencyConvolution.tpp"

#endif // ZERO_LATENCY_CONVOLUTION_HPP