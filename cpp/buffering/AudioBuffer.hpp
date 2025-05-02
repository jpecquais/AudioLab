// AUDIOBUFFER.hpp
#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <cstddef> // For size_t
#include <stdexcept> // For std::invalid_argument (used in constructor declaration/contract)

// AudioBuffer
// Goals -> manage a T**, where T is a float type.
// Why no C++ memory management ? Because it is designed for embeded devices.
// AudioBuffer buffer is dynamically allocated.
// AudioBuffer can be resize, but only explicitly.

template<class T>
class AudioBuffer
{
public: 

    enum class Error {
        ok = 0,
        bad_data,
        empty_size,
        buffer_already_allocated,
        buffer_not_allocated
    };

    AudioBuffer() = default;
    AudioBuffer(size_t number_channels, size_t buffer_length);
    AudioBuffer(const AudioBuffer& buffer); // Note: Changed to const& (good practice)
    AudioBuffer(AudioBuffer&& buffer) noexcept; // Added noexcept (good practice for moves)
    
    AudioBuffer& operator=(const AudioBuffer& buffer);
    AudioBuffer& operator=(AudioBuffer&& buffer) noexcept; // Added noexcept

    ~AudioBuffer() = default;

    void resize(size_t new_number_channels, size_t new_buffer_length);

    // Get raw buffer pointer (pointer to array of channel pointers)
    inline T** get(){ return buffer_; }
    inline const T* const* get() const { return buffer_; }
    
    inline T* operator[](size_t channel_index){ return buffer_[channel_index];} 
    inline const T* operator[](size_t channel_index) const { return buffer_[channel_index];} 

    // Accessor
    inline size_t get_buffer_length() const { return buffer_length_; }
    inline size_t get_number_of_channels() const { return number_channels_; }

    // Comparison operator.
    bool operator==(AudioBuffer<T>& other) const;

    // Helper function
    void copy_from(T** input_buffer, int num_channels, int buffer_length);
    void copy_from(const AudioBuffer<T>& input_buffer);

    // --- Private Members ---
    size_t buffer_length_;
    size_t number_channels_;
    T** buffer_;

protected:
        // --- Private Helper Methods ---
        // Allocate internal buffer
        auto allocate_buffer() -> Error;
    
        // Deallocate internal buffer
        auto deallocate_buffer() -> Error;

};

// --- Include the Implementation File ---
// This line brings in the template method definitions
#include "AudioBuffer.tpp"

#endif // AUDIO_BUFFER_HPP