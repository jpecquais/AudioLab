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

    void setup(T fs, T t_gainIn1, T t_gainIn2){
        m_fs = fs;
        m_ts = 1./fs;
        m_gainIn1 = t_gainIn1;
        m_gainIn2 = t_gainIn2;
        initAllpassesParameters();
        computeBiquadCoefficients();
    }

    T process(const T t_in1, const T t_in2){
        T in1 = t_in1;
        T in2 = t_in2;

        //TODO: Filtering is broken
        for (int i=0;i<m_numAllpasses_1;i++){
            in1 = biquads[i].process(in1);
            in2 = biquads[i+m_numAllpasses_1].process(in2);
        }

        for (int i=m_numAllpasses_1;i<m_numAllpasses_2;i++){
            in2 = biquads[i].process(in2);
        }

        return in1*m_gainIn1+in2*m_gainIn2;
    }
    private:
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