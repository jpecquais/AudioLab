#include <cstddef>
template<class T>
class AudioBuffer
{
private:

    int m_block_size;
    int m_number_channels;
    T** m_buffer;

public:
    
    enum class Error{
        ok = 0,
        bad_data,
        empty_size,
        buffer_already_allocated,
        buffer_not_allocated
    };
    
    AudioBuffer(size_t block_size, size_t number_channel);
    AudioBuffer(AudioBuffer& buffer);
    AudioBuffer(AudioBuffer&& buffer);
    ~AudioBuffer();
    
    T**& get();
    inline int get_block_size(){ return m_block_size; }
    inline int get_number_of_channels(){ return m_number_channels; }
    
    auto allocate_buffer() -> Error;
    auto deallocate_buffer() -> Error;
    
    T* operator[](size_t idx);

};

template<class T>
AudioBuffer<T>::AudioBuffer(size_t block_size, size_t number_channel):m_block_size(block_size),
                                                                      m_number_channels(number_channel),
                                                                      m_buffer(nullptr)
{
    //TODO: throw error if block_size of number_channel are == 0;
}

template<class T>
AudioBuffer<T>::AudioBuffer(AudioBuffer& buffer) // Copy constructor
{
    m_block_size = buffer.get_block_size();
    m_number_channels = buffer.get_number_of_channels();
    allocate_buffer();
    auto& buffer_data = buffer.get();
    for (size_t ch = 0; ch < m_number_channels; ch++)
    {
        for (size_t i = 0; i < m_block_size; i++)
        {
            m_buffer[ch][i] = buffer_data[ch][i];
        }
    }
}

template<class T>
AudioBuffer<T>::AudioBuffer(AudioBuffer&& buffer) // Move constructor
{
    //TODO
}

template<class T>
AudioBuffer<T>::~AudioBuffer()
{
    deallocate_buffer();
}

template<class T>
auto AudioBuffer<T>::allocate_buffer() -> Error
{
    if (m_block_size == 0 or m_number_channels == 0) return Error::empty_size;
    if (!m_buffer)
    {
        try
        {
            m_buffer = new float*[m_block_size];
            for (size_t ch = 0; ch < m_number_channels; ch++)
            {
                m_buffer[ch] = new float[m_block_size];
                for (size_t i = 0; i < m_block_size; i++)
                {
                    m_buffer[ch][i] = 0;
                }
            }
        }
        catch()
        {
            for (size_t _ch = 0; _ch < ch; _ch++)
            {
                delete[] m_buffer[_ch];
            }
            delete[] m_buffer;
            return Error::bad_data;
        }
    }
    else
    {
        return Error::buffer_already_allocated;
    }
    return Error::ok;
}

template<class T>
auto AudioBuffer<T>::deallocate_buffer() -> Error
{
    if (m_buffer)
    {
        for (size_t ch = 0; ch < m_number_channels; _ch++)
            {
                delete[] m_buffer[ch];
            }
        delete[] m_buffer;
    }
    else
    {
        return Error::buffer_not_allocated;
    }
    return Error::ok;
}

template<class T>
T* AudioBuffer<T>::operator[](size_t idx)
{
    return m_buffer[idx];
}

template<class T>
T**& AudioBuffer<T>::get()
{
    return m_buffer;
}