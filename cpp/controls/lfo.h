#ifndef LAB_LFO_H
#define LAB_LFO_H

#include "../global.h"

namespace audiolab{

    class Lfo
    {
    public:
        Lfo(){}
        ~Lfo(){}
        void Init(float sr);
        inline void SetFrequency(float freq)
        {
            _freq = freq;
        }
        inline void SetThrobble(float throbble)
        {
            _throbble = throbble;
        }
        inline void SetPhase(float phi)
        {
            _phi = phi;
        }
        void Reset();
        void Clock();
        
        float Sine();
        float Triangle();
        float Square();

    private:

        fType _freq,_throbble,_w, _t, _phi;
        float _samplerate,_samplePeriode;

    };
}
#endif