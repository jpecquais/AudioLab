#pragma once
#include <libraries/AudioLab/filters/biquad.h>
#include <cmath>

const int NUM_2O_ALLPASSES = 5;

template <class T, int SamplingRate>
class InputSection{
    public:
    InputSection() = default;
    ~InputSection() = default;

    void setup(T t_gainIn1, T t_gainIn2){
        m_gainIn1 = t_gainIn1;
        m_gainIn2 = t_gainIn2;
        initAllpassesParameters();
        computeBiquadCoefficients();
    }

    T process(T in1, T in2){
        const int num_allpasses_1 = 2;
        const int num_allpasses_2 = 3;

        for (int i=0;i<num_allpasses_1;i++){
            in1 = biquads[i].process(in1);
            in2 = biquads[i+num_allpasses_1].process(in2);
        }

        for (int i=num_allpasses_1;i<num_allpasses_2;i++){
            in2 = biquads[i].process(in2);
        }

        return in1*m_gainIn1+in2*m_gainIn2;
    }
    private:
    T m_gainIn1;
    T m_gainIn2;
    struct AllpassParameters{
        T fc;
        T bw;
    };
    std::array<AllpassParameters,NUM_2O_ALLPASSES> allpassesParameters;
    std::array<audiolab::Biquad<T,audiolab::TF2>,NUM_2O_ALLPASSES> biquads;

    void initAllpassesParameters(){
        allpassesParameters[0].fc = 42; //Hz
        allpassesParameters[0].bw = 1.5; //Octave
        allpassesParameters[1].fc = 595;
        allpassesParameters[1].bw = 2;
        allpassesParameters[2].fc = 50;
        allpassesParameters[2].bw = 1.5;
        allpassesParameters[3].fc = 732;
        allpassesParameters[3].bw = 2;
        allpassesParameters[4].fc = 21150;
        allpassesParameters[4].bw = 3.7;
    }

    void computeBiquadCoefficients(){
        T ts = 1./SamplingRate;
        for (int i=0; i<NUM_2O_ALLPASSES; i++){
            T fc = allpassesParameters[i].fc; 
            T bw = allpassesParameters[i].bw;
            bw = std::abs(fc*std::pow<T>(2,bw)-fc);

            T r = std::exp(-PI*bw*ts);
            T theta = 2*PI*fc*ts;

            T alpha = r*r;
            T beta = -2*r*std::cos(theta);

            biquads[i].set(alpha,beta,1,beta,alpha);
        }
    }

};