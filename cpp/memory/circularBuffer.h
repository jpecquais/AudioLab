#ifndef LAB_CIRCULAR_BUFFER_H
#define LAB_CIRCULAR_BUFFER_H

#include <memory>
#include <array>

namespace audiolab
{

template <typename T>
class ICircularBuffer
{
    public:
    virtual ~ICircularBuffer() = 0;
    virtual void Push(T data) = 0;
    virtual void Write(T data) = 0;
    virtual T &operator[](size_t idx) = 0;
    virtual size_t GetCapacity() = 0;
};

template <typename T, size_t maxSize>
class CircularBuffer : public ICircularBuffer<T>
{

    public:
    CircularBuffer() = default;
    ~CircularBuffer() = default;


    void Push(T data) // Push data in front of the buffer
    {
        m_data[m_iBegin]=data;
        m_iBegin = logical_to_real_idx(m_iCapacity-1);
    }

    void Write(T data)
    {
        m_data[m_iBegin]=data;
        m_iBegin = logical_to_real_idx(1);
    }

    T &operator[](size_t idx)
    {
        idx = std::clamp<size_t>(idx,-m_iCapacity,m_iCapacity);
        if (idx<0) idx+=m_iCapacity;
        return m_data[logical_to_real_idx(idx)];
    }

    inline size_t GetCapacity(){return m_iCapacity;}

    private:
    inline size_t logical_to_real_idx(size_t idx){return (idx+m_iBegin)%m_iCapacity;}
    size_t m_iBegin, m_iCapacity;
    T m_data[maxSize];
    // std::array<T,maxSize> m_data;

};

}

#endif
