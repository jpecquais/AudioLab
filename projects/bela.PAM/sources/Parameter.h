#pragma once
#include <memory>
#include "PamRotaryEffect.h"

template <class T>
class IParameter{
    public:
    IParameter(std::string t_name, T t_default, T t_min, T t_max, T t_alpha):m_name(t_name),m_value(t_default),m_new_value(t_default),m_min(t_min),m_max(t_max),m_default(t_default),m_alpha(t_alpha){};
    ~IParameter() = default;

    virtual void setValue(T t_newVal) = 0;
    virtual T getValue() = 0;

    T getMaximum(){
        return m_max;
    }

    T getMinimum(){
        return m_min;
    }

    T getDefault(){
        return m_default;
    }

    std::string& getName(){
        return m_name;
    }

    void updateValue(){
        m_value += (m_new_value-m_value)*m_alpha;
    }

    void setValueFromMidi(int t_value){
        setValue(((t_value/127.)*(m_max-m_min))+m_min);
    }

    protected:
    std::string m_name;
    T m_value, m_new_value, m_min, m_max, m_default, m_alpha;
};

template <class T>
class Parameter : public IParameter<T>{
    public:
    Parameter(std::string t_name, T t_default, T t_min, T t_max, T t_alpha):IParameter<T>(t_name,t_default,t_min,t_max,t_alpha){};
    ~Parameter() = default;

    void setValue(T t_newVal){
        this->m_value = std::max<T>(std::min<T>(t_newVal,this->m_max),this->m_min);
    }

    T getValue(){
        return this->m_value;
    }

};

template <class T>
class FAUSTParameter : public IParameter<T>{
    public:
    FAUSTParameter(MapUI* t_faustUI, std::string t_name, T t_default, T t_min, T t_max, T t_alpha) : IParameter<T>(t_name, t_default, t_min, t_max,t_alpha),m_faustUI(t_faustUI){}
    ~FAUSTParameter() = default;

    void setValue(T t_newVal){
        this->m_value = std::max<T>(std::min<T>(t_newVal,this->m_max),this->m_min);
        m_faustUI->setParamValue(this->m_name,this->m_value);
    }

    T getValue(){
        return m_faustUI->getParamValue(this->m_name);
    }

    private:
    MapUI* m_faustUI;
};