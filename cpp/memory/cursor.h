#ifndef LAB_CURSOR_H
#define LAB_CURSOR_H

#include <memory>
#include "circularBuffer.h"

namespace audiolab
{

    template<typename T>
    class ReadCursor
    {
    public:

        ReadCursor() = default;
        ~ReadCursor() = default;

        void AttachTo(std::shared_ptr<ICircularBuffer<T>> buffer)
        {
            buffer_ = buffer;
        }

        T Read()
        {
            T a = buffer_[-pos_];
            T b = buffer_[-pos_-1];
            return a + (b - a) * frac_;
        }

        inline void SetOffset(size_t pos){pos_=pos;}
        void SetDelay(double delay)
        {
            frac_ = delay-static_cast<int>(delay);
            pos_ = static_cast<size_t>(delay);
        }

    private:

        size_t pos_;
        double frac_;
        std::shared_ptr<ICircularBuffer<T>> buffer_;

    };

}

#endif