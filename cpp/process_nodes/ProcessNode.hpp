
#ifndef PROCESS_NODE_HPP
#define PROCESS_NODE_HPP

#include "../buffering/AudioBuffer.hpp"
#include <functional>
#include <memory>

template<class T>
class ProcessNode
{
public:
    using ProcessCallback = std::function<void(T**,T**)>;
    using PtrProcessNode = std::unique_ptr<ProcessNode<T>>;

    virtual ~ProcessNode() = default;
    
    virtual void bypass(int bypass_state) = 0;
    virtual void activate(int activate_state) = 0;
    virtual void process(AudioBuffer<T>& input_buffer, AudioBuffer<T>& output_buffer) = 0;

    template<int block_size, int num_io_channels>
    static PtrProcessNode make_processnode(ProcessCallback process_callback);

    template<int block_size, int num_io_channels>
    static PtrProcessNode make_faust_processnode(std::function<void(int,T**,T**)> faust_process_callback, int block_size, int num_io_channels);
};

template<class T, int block_size, int num_io_channels>
class ConcreteNode : public ProcessNode<T>
{
public:
    using NodeAudioBuffer = FixedSizedAudioBuffer<T,num_io_channels,block_size>;
    ConcreteNode(ProcessCallback process_callback):process_callback_(std::function<ProcessCallback>(process_callback))
    {
        block_size_ = block_size;
        num_io_channel_ = num_io_channels;
        is_bypassed = false;
        is_activated = true;
    }
    ~ConcreteNode() = default;
    
    void bypass(int bypass_state) final;
    void activate(int activate_state) final;
    void process(AudioBuffer<T>& input_buffer, AudioBuffer<T>& output_buffer) final;
    
private:
    NodeAudioBuffer internal_buffer_;
    ProcessCallback process_callback_;
    int block_size_;
    int num_io_channel_;
    bool is_bypassed, is_activated;
}

#include "ProcessNode.tpp"

#endif //PROCESS_NODE_HPP