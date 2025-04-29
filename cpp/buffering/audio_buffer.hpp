// audio_buffer.hpp
#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <cstddef> // For size_t
#include <stdexcept> // For std::invalid_argument (used in constructor declaration/contract)

template<class T, int number_of_channel, int buffer_length>
class AudioBuffer
{
public:
    // Public Enum for Error Codes
    enum class Error {
        ok = 0,
        bad_data,
        empty_size,
        buffer_already_allocated,
        buffer_not_allocated
    };

    // --- Constructors ---
    // Constructor with size
    AudioBuffer();

    // Copy Constructor
    AudioBuffer(const AudioBuffer& buffer); // Note: Changed to const& (good practice)

    // Move Constructor
    AudioBuffer(AudioBuffer&& buffer) noexcept; // Added noexcept (good practice for moves)

    // --- Destructor ---
    ~AudioBuffer();

    // --- Assignment Operators ---
    // Copy Assignment Operator
    AudioBuffer& operator=(const AudioBuffer& buffer);

    // Move Assignment Operator
    AudioBuffer& operator=(AudioBuffer&& buffer) noexcept; // Added noexcept

    // --- Accessors ---
    // Get raw buffer pointer
    T** get() const {return buffer_;}

    // Get buffer length (number of samples per channel)
    inline size_t get_buffer_length() const {return buffer_length_;}// Kept size_t, original had int

    // Get number of channels
    inline size_t get_number_of_channels() const {return number_channels_;} // Kept size_t, original had int

    // Access a specific channel
    T* operator[](size_t channel_index) const {return buffer_[channel_index];}

private:
    // --- Private Members ---
    const size_t buffer_length_ = buffer_length;
    const size_t number_channels_ = number_of_channel;
    T** buffer_;

    // --- Private Helper Methods ---
    // Allocate internal buffer
    auto allocate_buffer() -> Error;

    // Deallocate internal buffer
    auto deallocate_buffer() -> Error;
};

// --- Include the Implementation File ---
// This line brings in the template method definitions
#include "audio_buffer.tpp"

#endif // AUDIO_BUFFER_HPP