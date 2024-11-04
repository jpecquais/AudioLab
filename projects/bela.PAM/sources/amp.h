#include "models.h"
#include <libraries/RTNeural-main/RTNeural/RTNeural.h>

template <class T, int hiddenSize>
class Amp{
    public:
        Amp() = default;
        ~Amp() = default;

        void setup(){
            setupWeights();	
            auto& gru = (model).template get<0>();
            auto& dense = (model).template get<1>();
            gru.setWVals(mainModelData.rec_weight_ih_l0);
            gru.setUVals(mainModelData.rec_weight_hh_l0);
            gru.setBVals(mainModelData.rec_bias);
            dense.setWeights(mainModelData.lin_weight);
            dense.setBias(mainModelData.lin_bias.data());
            model.reset();
        }

        T process(T* pInput){
            return model.forward(pInput);
        }

    private:
        RTNeural::ModelT<T, 1, 1, RTNeural::GRULayerT<T, 1, hiddenSize>, RTNeural::DenseT<T, hiddenSize, 1>> model;
};