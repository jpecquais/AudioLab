#ifndef LAB_ONEPOLESWITCHER_H
#define LAB_ONEPOLESWITCHER_H

#include "../global.h"

namespace audiolab
{

    class OnePoleSwitcher
    {
    public:
        void Init(float samplerate);
        inline void SetAttack(float attack){attack_ = exp(-sampleperiod_/attack);}
        inline void SetRelease(float release){release_ = exp(-sampleperiod_/release);}
        float Process(float input);
    private:
        fType samplerate_,sampleperiod_;
        fType attack_,release_;
        fType lastY_;
    };

}

#endif