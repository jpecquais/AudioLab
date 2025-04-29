// audio_buffer.tpp
// Template implementations for AudioBuffer
// Included by audio_buffer.hpp - Do NOT include directly

// Note: We don't strictly need includes here if they are already in the .hpp,
// but it can be good practice for clarity, especially if the .hpp might change.
#include "audio_buffer.hpp"
#include <cstddef>   // For size_t
#include <stdexcept> // For std::invalid_argument
#include <new>       // For potential exceptions from new

// --- Private Helper Method Definitions ---

template<class T, int number_of_channel, int buffer_length>
auto AudioBuffer<T, number_of_channel, buffer_length>::allocate_buffer() -> Error
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

template<class T, int number_of_channel, int buffer_length>
auto AudioBuffer<T, number_of_channel, buffer_length>::deallocate_buffer() -> Error
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

template<class T, int number_of_channel, int buffer_length>
AudioBuffer<T, number_of_channel, buffer_length>::AudioBuffer()
    : buffer_length_(buffer_length), number_channels_(number_of_channel), buffer_(nullptr)
{
    Error err = allocate_buffer();
    if (err != Error::ok)
    {
         throw std::runtime_error("Failed to allocate audio buffer");
    }
}

template<class T, int number_of_channel, int buffer_length>
AudioBuffer<T, number_of_channel, buffer_length>::AudioBuffer(const AudioBuffer<T, number_of_channel, buffer_length>& buffer) // Changed to const&
    : buffer_length_(buffer.buffer_length_), // Access members directly
      number_channels_(buffer.number_channels_),
      buffer_(nullptr) // Start with null buffer
{
    Error err = allocate_buffer();
    if (err != Error::ok)
    {
         throw std::runtime_error("Failed to allocate audio buffer");
    }

    for (size_t ch = 0; ch < number_channels_; ++ch)
    {
        for (size_t i = 0; i < buffer_length_; ++i)
        {
            buffer_[ch][i] = buffer.buffer_[ch][i];
        }
    }
}

template<class T, int number_of_channel, int buffer_length>
AudioBuffer<T, number_of_channel, buffer_length>::AudioBuffer(AudioBuffer<T, number_of_channel, buffer_length>&& buffer) noexcept // Added noexcept
    : buffer_length_(buffer.buffer_length_),
      number_channels_(buffer.number_channels_),
      buffer_(buffer.buffer_) // Steal the pointer
{
    buffer.buffer_ = nullptr;
}

// --- Destructor Definition ---

template<class T, int number_of_channel, int buffer_length>
AudioBuffer<T, number_of_channel, buffer_length>::~AudioBuffer()
{
    deallocate_buffer();
}

// --- Assignment Operator Definitions ---

template<class T, int number_of_channel, int buffer_length>
AudioBuffer<T, number_of_channel, buffer_length>& AudioBuffer<T, number_of_channel, buffer_length>::operator=(const AudioBuffer<T, number_of_channel, buffer_length>& buffer)
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

template<class T, int number_of_channel, int buffer_length>
AudioBuffer<T, number_of_channel, buffer_length>& AudioBuffer<T, number_of_channel, buffer_length>::operator=(AudioBuffer<T, number_of_channel, buffer_length>&& buffer) noexcept // Added noexcept
{
    if (this != &buffer)
    {
        deallocate_buffer();
        buffer_ = buffer.buffer_;
        buffer.buffer_ = nullptr;
    }
    return *this;
}