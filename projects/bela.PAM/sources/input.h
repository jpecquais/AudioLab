#pragma once
#include <libraries/AudioLab/filters/biquad.h>
#include <cmath>

static constexpr int NUM_2O_ALLPASSES = 5;
static constexpr int NUM_BIQUAD_1 = 2;
static constexpr int NUM_BIQUAD_2 = 3;

template <class T>
class InputSection{
public:
    InputSection() : m_numAllpasses_1{NUM_BIQUAD_1},m_numAllpasses_2{NUM_BIQUAD_1}{}
    ~InputSection() = default;

    struct StateChannel
    {
        T gain_in_1;
        T gain_in_2;
    };

    StateChannel clean_channel, drive_channel;

    void setup(T fs, int new_block_size)
    {
        float blackbird_input_gain_clean = db2linear<float>(-12.f);
        float constable_input_gain_clean = -1 * db2linear<float>(0.f);
        
        float blackbird_input_gain_drive = db2linear<float>(-12.f);
        float constable_input_gain_drive = db2linear<float>(0.f);

        clean_channel.gain_in_1 = blackbird_input_gain_clean;
        clean_channel.gain_in_2 = constable_input_gain_clean;
        
        drive_channel.gain_in_1 = blackbird_input_gain_drive;
        drive_channel.gain_in_2 = constable_input_gain_drive;

        m_gainIn1 = clean_channel.gain_in_1;
        m_gainIn2 = clean_channel.gain_in_2;

        m_fs = fs;
        block_size = new_block_size;
        m_ts = 1./fs;

        initAllpassesParameters();
        computeBiquadCoefficients();
    }

    T process(const T t_in1, const T t_in2){
        T in1 = t_in1;
        T in2 = t_in2;

        for (int i=0;i<m_numAllpasses_1;i++){
            in1 = biquads[i].process(in1);
            in2 = biquads[i+m_numAllpasses_1].process(in2);
        }

        for (int i=m_numAllpasses_1;i<m_numAllpasses_2;i++){
            in2 = biquads[i].process(in2);
        }

        return in1*m_gainIn1+in2*m_gainIn2;
    }

    void process(T** input_buffer, T** output_buffer)
    {
        for (int i = 0; i < block_size; i++)
        {
            output_buffer[0][i] = process(input_buffer[0][i],input_buffer[1][i]);
            output_buffer[1][i] = 0;
        }
    }

    void toggle_channels(float drive_state)
    {
        if (drive_state > 0)
        {
            m_gainIn1 = drive_channel.gain_in_1;
            m_gainIn2 = drive_channel.gain_in_2;
        }
        else
        {
            m_gainIn1 = clean_channel.gain_in_1;
            m_gainIn2 = clean_channel.gain_in_2;
        }
    }

private:
    static constexpr int NUM_INPUT_CHANNEL = 2;
    static constexpr int NUM_OUTPUT_CHANNEL = 1;

    int block_size;
    const int m_numAllpasses_1;
    const int m_numAllpasses_2;
    T m_fs, m_ts;
    T m_gainIn1;
    T m_gainIn2;
    struct AllpassParameters{
        T fc;
        T bw;
    };
    std::array<AllpassParameters,NUM_2O_ALLPASSES> allpassesParameters;
    std::array<audiolab::Biquad<T,audiolab::TF2>,NUM_2O_ALLPASSES> biquads;

    void initAllpassesParameters(){
        allpassesParameters[0] = {42, 1.5}; // Hz, Octave
        allpassesParameters[1] = {595, 2};
        allpassesParameters[2] = {50, 1.5};
        allpassesParameters[3] = {732, 2};
        allpassesParameters[4] = {21150, 3.7};
    }

    void computeBiquadCoefficients(){
        for (int i=0; i<NUM_2O_ALLPASSES; i++){
            T fc = allpassesParameters[i].fc; 
            T bw = allpassesParameters[i].bw;
            bw = std::abs(fc*std::pow<T>(2,bw)-fc);

            T r = std::exp(-PI*bw*m_ts);
            T theta = 2*PI*fc*m_ts;

            T alpha = r*r;
            T beta = -2*r*std::cos(theta);

            biquads[i].set(alpha,beta,1,beta,alpha);
        }
    }
};