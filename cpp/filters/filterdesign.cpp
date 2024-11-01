#include "filterdesign.h"

namespace audiolab
{
    template<>
    Sos<2> HilbertReal<4>()
    {
        Sos<2> result;

        result[0][0] = 0.02569f;
        result[0][1] = -0.260502f;
        result[0][2] = -0.260502f;
        result[0][3] = 0.02569f;
        result[0][4] = 1;

        result[0][0] = -1.8685f;
        result[0][1] = 0.870686f;
        result[0][2] = 0.870686f;
        result[0][3] = -1.8685f;
        result[0][4] = 1;

        return result;
    }

    template<>
    Sos<2> HilbertImag<4>()
    {
        Sos<2> result;

        result[0][0] = -1.94632f;
        result[0][1] = 0.94657f;
        result[0][2] = 0.94657f;
        result[0][3] = -1.94632f;
        result[0][4] = 1;

        result[0][0] = -0.83774f;
        result[0][1] = 0.06338f;
        result[0][2] = 0.06338f;
        result[0][3] = -0.83774f;
        result[0][4] = 1;

        return result;
    }
}