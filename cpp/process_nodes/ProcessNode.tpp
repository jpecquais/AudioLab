#include "ProcessNode.hpp"
#include "../buffering/AudioBuffer.hpp"
#include <memory>





template<class T>
auto ProcessNode<T> :: make_processnode(ProcessCallback process_callback, int block_size, int num_io_channels) -> PtrProcessNode
{
    return std::make_unique<ConcreteNode<T,block_size,num_io_channels>>(new ConcreteNode<T,block_size,num_io_channels>(process_callback));
}

template<class T>
template<int block_size, int num_io_channels>
auto ProcessNode<T> :: make_faust_processnode(std::function<void(int,T**,T**)> faust_process_callback) -> PtrProcessNode
{
    return std::make_unique<ProcessNode<T>>(new ConcreteNode<T,block_size,num_io_channels>(
        [](T** input_buffer, T** output_buffer){ faust_process_callback(block_size_,input_buffer,output_buffer) })
    );
}






template<class T, int block_size, int num_io_channels>
void ConcreteNode<T, block_size, num_io_channels> :: bypass(int bypass_state)
{
    is_bypassed = bypass_state;
}

template<class T, int block_size, int num_io_channels>
void ConcreteNode<T, block_size, num_io_channels> :: activate(int activate_state)
{
    is_activated = activate_state;
}

template<class T, int block_size, int num_io_channels>
void ConcreteNode<T, block_size, num_io_channels> :: process(AudioBuffer<T>& input_buffer, AudioBuffer<T>& output_buffer)
{
    if (is_activated) process_callback_(input_buffer.get(),internal_buffer_);
    output_buffer.copy_from((is_bypassed or (!is_activated)) ? input_buffer : internal_buffer_);
}