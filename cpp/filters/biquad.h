#ifndef LAB_BIQUAD_H
#define LAB_BIQUAD_H

#include <algorithm>
#include <iterator>
#include <array>

#include "../global.h"

namespace audiolab
{

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
template <class T>
class TBiquad
{
    public:
    TBiquad() = default;
    ~TBiquad() = default;

    /** Initializes the biquad module.
    */
    void Init()
    {
        m_a[0] = 1;
        m_a[1] = m_a[2] = m_b[0] = m_b[1] = m_b[2] = 0;
    }
    /** Set coefficients of the Biquad filter.
     * \param a1, a2, b0, b1, b2 - The coefficients of the filter
     * OR
     * \param std::array<float> coefficients - Array of five coefficients (a1,a2,b0,b1,b2)
    */
    inline void SetCoefficients(T b0, T b1, T b2, T a1, T a2)
    {
        m_a[1]=a1;
        m_a[2]=a2;
        m_b[0]=b0;
        m_b[1]=b1;
        m_b[2]=b2;
    }
    inline void SetCoefficients(std::array<T,5> coefficients)
    {
        std::copy(std::begin(coefficients),std::begin(coefficients)+1,std::begin(m_a)+1);
        std::copy(std::begin(coefficients)+2,std::end(coefficients),std::begin(m_b));
    }

    protected:
    std::array<T,3> m_a,m_b;
};

template <class T, BiquadForm form>
class Biquad : public TBiquad
{
    Biquad() = default;
    ~Biquad() = default;
};

template <class T>
class Biquad<T, TF2> : public TBiquad<T>
{
    /** Transposed Form 2 implementation of the Biquad. Should be used on floating point architecture.
    */
    public:
    T process(T& x)
    {
        T w0 = m_b[0] * x + m_w1;
        m_w1 = m_b[1] * x - m_a[1] * w0 + m_w2;
        m_w2 = m_b[2] * x - m_a[2] * w0;

        m_w2 = m_w1;
        m_w1 = w0;

        return w0;
    }

    private:
    fType m_w1{0},m_w2{0};
};

template <class T>
class Biquad<T,DF1> : public TBiquad
{
    /** Direct Form 1 implementation of the Biquad. Should be used on fixed point architecture
    */
    public:
    T process(T& x)
    {
        T y = (m_b[0] * x + m_b[1] * x1_ + m_b[2] * x2_ - m_a[1] * y1_ - m_a[2] * y2_);
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