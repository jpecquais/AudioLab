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
    void init()
    {
        m_a[0] = 1;
        m_a[1] = m_a[2] = m_b[0] = m_b[1] = m_b[2] = 0;
    }
    /** Set coefficients of the Biquad filter.
     * \param a1, a2, b0, b1, b2 - The coefficients of the filter
     * OR
     * \param std::array<float> coefficients - Array of five coefficients (a1,a2,b0,b1,b2)
    */
    inline void set(T b0, T b1, T b2, T a1, T a2)
    {
        m_a[1]=a1;
        m_a[2]=a2;
        m_b[0]=b0;
        m_b[1]=b1;
        m_b[2]=b2;
    }
    inline void set(std::array<T,5> coefficients)
    {
        std::copy(std::begin(coefficients),std::begin(coefficients)+1,std::begin(m_a)+1);
        std::copy(std::begin(coefficients)+2,std::end(coefficients),std::begin(m_b));
    }

    protected:
    std::array<T,3> m_a,m_b;
};

template <class T, BiquadForm form>
class Biquad : public TBiquad<T>
{
    public:
    Biquad() = default;
    ~Biquad() = default;
};

template <class T>
class Biquad<T, TF2> : public TBiquad<T>
{
    /** Transposed Form 2 implementation of the Biquad. Should be used on floating point architecture.
    */
    public:
    Biquad() = default;
    ~Biquad() = default;

    T process(T& x)
    {
        T w0 = this->m_b[0] * x + this->m_w1;
        this->m_w1 = this->m_b[1] * x - this->m_a[1] * w0 + this->m_w2;
        this->m_w2 = this->m_b[2] * x - this->m_a[2] * w0;

        this->m_w2 = this->m_w1;
        this->m_w1 = w0;

        return w0;
    }

    private:
    T m_w1{0}, m_w2{0};
};

template <class T>
class Biquad<T, DF1> : public TBiquad<T>
{
    /** Direct Form 1 implementation of the Biquad. Should be used on fixed point architecture
    */
    public:
    Biquad() = default;
    ~Biquad() = default;

    T process(T& x)
    {
        T y = (this->m_b[0] * x + this->m_b[1] * this->x1_ + this->m_b[2] * this->x2_ - this->m_a[1] * this->y1_ - this->m_a[2] * this->y2_);
        this->x2_ = this->x1_;
        this->x1_ = x;
        this->y2_ = this->y1_;
        this->y1_ = y;

        return y;
    }

    private:
    T x1_{0}, x2_{0}, y1_{0}, y2_{0};
};

}// End of audiolab namespace

#endif
