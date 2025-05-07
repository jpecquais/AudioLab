#include <vector>

template <class T, int num_input_output_channels>
class Gain
{
public:
    Gain():block_size(32),m_value(1),m_interpolation_factor(0.7),target_value_(nullptr){}
    ~Gain() = default;

    void setup(T* input_param, int new_block_size, T new_interpolation_factor)
    {
        target_value_ = input_param;
        block_size = new_block_size;
        m_interpolation_factor = 1-new_interpolation_factor;
    }

    T process(T input_sample)
    {
        m_value += (*target_value_-m_value)*m_interpolation_factor;
        return input_sample*m_value;
    }

    void process(T** input_buffer, T** output_buffer)
    {
        for (int ch = 0; ch < num_input_output_channels; ch++)
        {
            for (int i = 0; i < block_size; i++)
            {
                output_buffer[ch][i] = process(input_buffer[ch][i]);
            }
        }
    }

private:
    int block_size;
    int m_num_input_output_channels = num_input_output_channels;
    T m_value, m_interpolation_factor;
    T* target_value_;
    std::vector<T> m_env;

};