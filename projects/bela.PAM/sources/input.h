#include <audiolab/filters/biquad.h>


template <class T, int SamplingRate>
class InputSection{
    public:
        InputSection() = default;
        ~InputSection() = default;

        void init(T t_gainIn1, T t_gainIn2){
            m_gainIn1 = t_gainIn1;
            m_gainIn2 = t_gainIn2;
            initAllpassesParameters();
            computeBiquadCoefficients();
        }

        T process(T& in1, T& in2){
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
        const int m_num2oAllpass = 5;
        const T m_fs = static_cast<T>(SamplingRate);
        const T m_ts = 1./m_fs;
        const T m_gainIn1;
        const T m_gainIn2;
        struct AllpassParameters{
            T fc;
            T bw;
        };
        std::array<AllpassParameters,num2oAllpass> allpassesParameters;
        std::array<audiolab::Biquad<T,audiolab::TF2>,num2oAllpass> biquads;

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
            for (int i=0; i<m_num2oAllpass; i++){
                T fc = allpassesParameters[i].fc; 
                T bw = allpassesParameters[i].bw;

                T r = std::exp<T>(-PI*bw*m_ts);
                T theta = 2*PI*fc*m_ts;

                T alpha = r*r;
                T beta = -2*r*std::cos<T>(theta);

                biquads[i].set(alpha,beta,1,beta,alpha);
            }
        }

};