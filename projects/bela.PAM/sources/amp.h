#include "models.h"
#include <libraries/RTNeural-main/RTNeural/RTNeural.h>

template <class T, int hiddenSize>
class Amp
{
public:
    Amp() = default;
    ~Amp() = default;

    void setup(int new_block_size){
        setupWeights();	
        auto& gru = (model).template get<0>();
        auto& dense = (model).template get<1>();
        gru.setWVals(mainModelData.rec_weight_ih_l0);
        gru.setUVals(mainModelData.rec_weight_hh_l0);
        gru.setBVals(mainModelData.rec_bias);
        dense.setWeights(mainModelData.lin_weight);
        dense.setBias(mainModelData.lin_bias.data());
        model.reset();
        block_size = new_block_size;
    }

    T process(T* pInput){
        return model.forward(pInput);
    }

    void process(T** input_buffer, T** output_buffer)
    {
        for (int i = 0; i < block_size; i++)
        {
            output_buffer[0][i] = process(input_buffer[0]+i);
        }
    }

private:
    int block_size;

    static constexpr int NUM_INPUT_CHANNEL = 1;
    static constexpr int NUM_OUTPUT_CHANNEL = 1;
    bool is_bypass = false;
    
    RTNeural::ModelT<T, 1, 1, RTNeural::GRULayerT<T, 1, hiddenSize>, RTNeural::DenseT<T, hiddenSize, 1>> model;
};