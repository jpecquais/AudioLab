// audio_buffer.tpp
// Template implementations for FixedSizedAudioBuffer
// Included by audio_buffer.hpp - Do NOT include directly

// Note: We don't strictly need includes here if they are already in the .hpp,
// but it can be good practice for clarity, especially if the .hpp might change.
#include "AudioBuffer.hpp"
#include <cstddef>   // For size_t
#include <stdexcept> // For std::invalid_argument
#include <new>       // For potential exceptions from new

template<class T>
void AudioBuffer<T> :: copy_from(T** input_buffer, int num_channels, int buffer_length)
{
    for (int ch = 0; ch < num_channels; ch++)
    {
        for (int smpl = 0; smpl < buffer_length; smpl++)
        {
            buffer_[ch][smpl] = input_buffer[ch][smpl];
        }
    }
}

template<class T>
void AudioBuffer<T> :: copy_from(const AudioBuffer<T>& input_buffer)
{
    if (*this!=input_buffer) return;
    for (int ch = 0; ch < number_channels_; ch++)
    {
        for (int smpl = 0; smpl < buffer_length_; smpl++)
        {
            buffer_[ch][smpl] = input_buffer[ch][smpl];
        }
    }
}


template<class T>
bool AudioBuffer<T> :: operator==(AudioBuffer<T>& other) const
{
    return ((number_channels_ == other.get_number_of_channels()) and (buffer_length_ == other.get_buffer_length()));
}


// --- Private Helper Method Definitions ---

template<class T>
auto AudioBuffer<T> :: allocate_buffer() -> Error
{
    // Check if sizes are valid (already checked in constructor throwing exception,
    // but good defense here too, though maybe redundant)
    if (buffer_length_ == 0 || number_channels_ == 0) return Error::empty_size;

    // Check if buffer already exists
    if (buffer_ != nullptr)
    {
        return Error::buffer_already_allocated;
    }

    // Allocate channel pointers
    try
    {
        buffer_ = new T*[number_channels_];

        // Allocate buffer for each channel
        size_t allocated_channels = 0;
        try
        {
            for (allocated_channels = 0; allocated_channels < number_channels_; ++allocated_channels)
            {
                // Use () for value initialization (zeros for primitives)
                buffer_[allocated_channels] = new T[buffer_length_]();
                for (size_t smpl = 0; smpl < buffer_length_; smpl++)
                {
                    buffer[allocated_channels][smpl] = 0;
                }
            }
        }
        catch (...) // Catch allocation failure for individual channels
        {
            // Clean up successfully allocated channels
            for (size_t ch = 0; ch < allocated_channels; ++ch)
            {
                delete[] buffer_[ch];
            }
            // Clean up the array of pointers
            delete[] buffer_;
            buffer_ = nullptr; // Ensure state is consistent
            return Error::bad_data; // Or a more specific memory error
        }
    }
    catch (...) // Catch allocation failure for the array of pointers
    {
        buffer_ = nullptr; // Ensure state is consistent
        return Error::bad_data; // Or a more specific memory error
    }

    return Error::ok;
}

template<class T>
auto AudioBuffer<T> :: deallocate_buffer() -> Error
{
    if (buffer_ != nullptr) {
        for (size_t ch = 0; ch < number_channels_; ++ch)
        {
            // Check if channel pointer is valid before deleting (optional defense)
            if (buffer_[ch] != nullptr)
            {
                delete[] buffer_[ch];
                buffer_[ch] = nullptr; // Optional: null out after delete
            }
        }
        delete[] buffer_;
        buffer_ = nullptr;
        return Error::ok;
    }
    else
    {
        return Error::buffer_not_allocated;
    }
}


// --- Constructor Definitions ---

template<class T>
AudioBuffer<T> :: AudioBuffer(size_t number_channels, size_t buffer_length)
{
    buffer_length_ = buffer_length;
    number_channels_ = number_channels;
    buffer_ = nullptr;

    Error err = allocate_buffer();
    if (err != Error::ok)
    {
         throw std::runtime_error("Failed to allocate audio buffer");
    }
}

template<class T>
AudioBuffer<T> :: AudioBuffer(const AudioBuffer<T>& buffer) // Changed to const&
    : buffer_length_(buffer.buffer_length_), // Access members directly
      number_channels_(buffer.number_channels_),
      buffer_(nullptr) // Start with null buffer
{
    Error err = allocate_buffer();
    if (err != Error::ok)
    {
         throw std::runtime_error("Failed to allocate audio buffer");
    }

}

template<class T>
AudioBuffer<T> :: AudioBuffer(AudioBuffer<T>&& buffer) noexcept // Added noexcept
    : buffer_length_(buffer.buffer_length_),
      number_channels_(buffer.number_channels_),
      buffer_(buffer.buffer_) // Steal the pointer
{
    buffer.buffer_ = nullptr;
}

template<class T>
AudioBuffer<T> :: ~AudioBuffer()
{
    deallocate_buffer();
}

// --- Assignment Operator Definitions ---

template<class T>
AudioBuffer<T>& AudioBuffer<T>::operator=(const AudioBuffer<T>& buffer)
{
    //FIXME: Potentially dangerous if this or buffer have been moved before.
    if (this == &buffer)
    {
        return *this;
    }

    for (int ch = 0; ch < number_of_channel; ch++)
    {
        for (int smpl = 0; smpl < buffer_length; smpl++)
        {
            buffer_[ch][smpl] = buffer.buffer_[ch][smpl];
        }
    }

    return *this;
}

template<class T>
AudioBuffer<T>& AudioBuffer<T>::operator=(AudioBuffer<T>&& buffer) noexcept // Added noexcept
{
    if (this != &buffer)
    {
        deallocate_buffer();
        buffer_ = buffer.buffer_;
        buffer.buffer_ = nullptr;
    }
    return *this;
}