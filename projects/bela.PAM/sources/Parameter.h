#pragma once
#include <memory>
#include "PamRotaryEffect.h"

template <class T>
class Parameter{
    public:
    Parameter(std::string t_name, T t_default, T t_min, T t_max):m_name(t_name),m_value(t_default),m_min(t_min),m_max(t_max),m_default(t_default){};
    ~Parameter() = default;

    virtual void setValue(T t_newVal){
        m_value = std::max<T>(std::min<T>(t_newVal,m_max),m_min);
    }

    virtual T getValue(){
        return m_value;
    }

    T getMaximum(){
        return m_max;
    }

    T getMinimum(){
        return m_min;
    }

    T getDefault(){
        return m_default;
    }

    void setValueFromMidi(int t_value){
        setValue(((t_value/127.)*(m_max-m_min))+m_min);
    }

    protected:
    std::string m_name;
    T m_value, m_min, m_max, m_default;
};

template <class T>
class FAUSTParameter : public Parameter<T>{
    public:
    FAUSTParameter(std::shared_ptr<MapUI> t_faustUI, std::string t_name, T t_default, T t_min, T t_max) : Parameter<T>(t_name, t_default, t_min, t_max),m_faustUI(t_faustUI){}
    ~FAUSTParameter() = default;

    void setValue(T t_newVal){
        Parameter<T>::setValue(t_newVal);
        m_faustUI->setParamValue(this->m_name,this->m_value);
    }

    T getValue(){
        return m_faustUI->getParamValue(this->m_name);
    }

    private:
    std::shared_ptr<MapUI> m_faustUI;
};