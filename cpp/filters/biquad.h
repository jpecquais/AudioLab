#ifndef LAB_BIQUAD_H
#define LAB_BIQUAD_H

#include <algorithm>
#include <iterator>
#include <array>

#include "../global.h"

namespace audiolab
{

using fType = float;
template<size_t size>
using fArray = std::array<fType,size>;

/** Biquad cell filter
 * takes 5 coefficient to be set. (a0 is always 1)
 * 
 * Author : Jean-Loup Pecquais
*/

typedef enum{
    DF1,
    DF2, // Not implemented
    TF1, // Not implemented
    TF2,
} BiquadForm;

/** TBiquad class implement generic behavior for subsequent Biquad form implementation
*/
class TBiquad
{
    public:
    TBiquad() = default;
    ~TBiquad() = default;

    /** Initializes the biquad module.
    */
    void Init()
    {
        a_[0] = 1;
        a_[1] = a_[2] = b_[0] = b_[1] = b_[2] = 0;
    }
    /** Set coefficients of the Biquad filter.
     * \param a1, a2, b0, b1, b2 - The coefficients of the filter
     * OR
     * \param std::array<float> coefficients - Array of five coefficients (a1,a2,b0,b1,b2)
    */
    inline void SetCoefficients(fType a1, fType a2, fType b0, fType b1, fType b2)
    {
        a_[1]=a1;
        a_[2]=a2;
        b_[0]=b0;
        b_[1]=b1;
        b_[2]=b2;
    }
    inline void SetCoefficients(fArray<5> coefficients)
    {
        std::copy(std::begin(coefficients),std::begin(coefficients)+1,std::begin(a_)+1);
        std::copy(std::begin(coefficients)+2,std::end(coefficients),std::begin(b_));
    }

    protected:
    fArray<3> a_,b_;
};

template <BiquadForm form>
class Biquad : public TBiquad
{
    Biquad() = default;
    ~Biquad() = default;
};

template <>
class Biquad<TF2> : public TBiquad
{
    /** Transposed Form 2 implementation of the Biquad. Should be used on floating point architecture.
    */
    public:
    float Process(float x)
    {
        float w0 = b_[0] * x + w1_;
        w1_ = b_[1] * x - a_[1] * w0 + w2_;
        w2_ = b_[2] * x - a_[2] * w0;

        w2_ = w1_;
        w1_ = w0;

        return w0;
    }

    private:
    fType w1_{0},w2_{0};
};

template <>
class Biquad<DF1> : public TBiquad
{
    /** Direct Form 1 implementation of the Biquad. Should be used on fixed point architecture
    */
    public:
    float Process(float x)
    {
        float y = (b_[0] * x + b_[1] * x1_ + b_[2] * x2_ - a_[1] * y1_ - a_[2] * y2_);
        x2_ = x1_;
        x1_ = x;
        y2_ = y1_;
        y1_ = y;

        return y;
    }

    private:
    fType x1_{0},x2_{0},y1_{0},y2_{0};
};

}// End of audiolab namespace

#endif