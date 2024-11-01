#ifndef LAB_FILTER_DESIGN_H
#define LAB_FILTER_DESIGN_H

#include "../global.h"

namespace audiolab
{

    using Coefficients = fArray<5>;
    template<size_t size>
    using Sos = std::array<Coefficients,size>;

    template<int order>
    Sos<(order+1)/2> HilbertReal()
    {
        Sos<(order+1)/2> result;
        for (auto &r : result)
        {
            for (auto &c : r){c=0;} // TODO : DO A REAL GENERIC IMPLEMENTATION !!!!
        }
        return result;
    }

    template<>
    Sos<2> HilbertReal<4>();

    template<int order>
    Sos<(order+1)/2> HilbertImag()
    {
        Sos<(order+1)/2> result;
        for (auto &r : result)
        {
            for (auto &c : r){c=0;} // TODO : DO A REAL GENERIC IMPLEMENTATION !!!!
        }
        return result;
    }

    template<>
    Sos<2> HilbertImag<4>();

}

#endif