#include "OnePoleSwitcher.h"

namespace audiolab
{

    void OnePoleSwitcher::Init(float samplerate)
    {
        samplerate_ = samplerate;
        sampleperiod_ = 1 / samplerate_;
        lastY_ = 0;
        SetAttack(0.005);
        SetRelease(0.5);
    }

    float OnePoleSwitcher::Process(float input)
    {
        float a = lastY_>input ? release_ : attack_;
        float output = (1-a)*input+a*lastY_;
        lastY_ = output;
        return output;
    }

}