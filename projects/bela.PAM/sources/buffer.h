#include <cstddef>
#include <stdexcept>

template<class T>
class AudioBuffer
{
private:
    int m_block_size;
    int m_number_channels;
    T** m_buffer;

public:
    enum class Error {
        ok = 0,
        bad_data,
        empty_size,
        buffer_already_allocated,
        buffer_not_allocated
    };

    AudioBuffer(size_t block_size, size_t number_channel)
        : m_block_size(block_size), m_number_channels(number_channel), m_buffer(nullptr)
    {
        if (block_size == 0 || number_channel == 0) {
            throw std::invalid_argument("Block size and number of channels must be greater than 0");
        }
    }

    AudioBuffer(AudioBuffer& buffer)
        : m_block_size(buffer.get_block_size()), m_number_channels(buffer.get_number_of_channels()), m_buffer(nullptr)
    {
        allocate_buffer();
        auto& buffer_data = buffer.get();
        for (size_t ch = 0; ch < m_number_channels; ch++) {
            for (size_t i = 0; i < m_block_size; i++) {
                m_buffer[ch][i] = buffer_data[ch][i];
            }
        }
    }

    AudioBuffer(AudioBuffer&& buffer)
        : m_block_size(buffer.m_block_size), m_number_channels(buffer.m_number_channels), m_buffer(buffer.m_buffer)
    {
        buffer.m_buffer = nullptr;
        buffer.m_block_size = 0;
        buffer.m_number_channels = 0;
    }

    ~AudioBuffer() {
        deallocate_buffer();
    }

    T**& get() {
        return m_buffer;
    }

    inline int get_block_size() {
        return m_block_size;
    }

    inline int get_number_of_channels() {
        return m_number_channels;
    }

    auto allocate_buffer() -> Error {
        if (m_block_size == 0 || m_number_channels == 0) return Error::empty_size;
        if (!m_buffer) {
            try {
                m_buffer = new T*[m_number_channels];
                for (size_t ch = 0; ch < m_number_channels; ch++) {
                    m_buffer[ch] = new T[m_block_size]();
                }
            } catch (...) {
                for (size_t ch = 0; ch < m_number_channels; ch++) {
                    delete[] m_buffer[ch];
                }
                delete[] m_buffer;
                m_buffer = nullptr;
                return Error::bad_data;
            }
        } else {
            return Error::buffer_already_allocated;
        }
        return Error::ok;
    }

    auto deallocate_buffer() -> Error {
        if (m_buffer) {
            for (size_t ch = 0; ch < m_number_channels; ch++) {
                delete[] m_buffer[ch];
            }
            delete[] m_buffer;
            m_buffer = nullptr;
        } else {
            return Error::buffer_not_allocated;
        }
        return Error::ok;
    }

    T* operator[](size_t idx) {
        return m_buffer[idx];
    }
};