#ifndef LAB_GLOBAL_H
#define LAB_GLOBAL_H

#include <array>

#define PI 3.1415927410125732421875f

namespace audiolab
{
    using fType = float;
    template<size_t size>
    using fArray = std::array<fType,size>;
}


#endif