#ifndef ZERO_LATENCY_CONVOLUTION_HPP
#define ZERO_LATENCY_CONVOLUTION_HPP

#include <cstddef>
#include <algorithm>
#include <vector>
#include "../lib/Partitioned-Convolution/lib/Convolution.h"

class ZeroLatencyConvolution
{
public:
    using AudioBuffer = std::vector<float>;

    ZeroLatencyConvolution() = default;
    ~ZeroLatencyConvolution();

    ZeroLatencyConvolution(const ZeroLatencyConvolution&) = delete;
    ZeroLatencyConvolution(ZeroLatencyConvolution&&) = delete;
    ZeroLatencyConvolution& operator=(const ZeroLatencyConvolution&) = delete;
    ZeroLatencyConvolution& operator=(ZeroLatencyConvolution&&) = delete;

    bool init(const float* impulseResponse, int ir_length, int block_size);
    void process(const float* input, float* output);
    void reset();

private:
    Convolution* convolution_ = nullptr;
    float* current_output_target_ = nullptr;

    static constexpr size_t head_ir_length_ = 128;
    size_t block_size_;
    size_t ir_length_;
    size_t tail_ir_length_;
    size_t head_convolution_buffer_length_;

    AudioBuffer head_ir_buffer_;
    AudioBuffer tail_ir_buffer_;

    AudioBuffer head_convolution_buffer;

    AudioBuffer in_buffer_;

    static void output_callback(void* SELF, dft_sample_t* output, int num_samples);
    void convolve_time_domain_section();
};


#endif // ZERO_LATENCY_CONVOLUTION_HPP