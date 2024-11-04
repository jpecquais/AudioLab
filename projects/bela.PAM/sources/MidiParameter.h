#pragma once
#include <memory>

enum MidiMessages{
    CC = 0,
    PC = 1
};

template <class T>
class TMidiParameter{
    public:
    TMidiParameter() = default;
    ~TMidiParameter() = default;

    void setDirty(){
        m_isDirty = true;
    }

    void setClean(){
        m_isDirty = false;
    }

    void boundParameter(T *t_pParam){
        m_pParam.reset(t_pParam); //TODO: Need a param class with pointer to variable, min, max and default.
    }

    virtual void setParameter(int inputValue) = 0;

    protected:
    std::shared_ptr<T> m_pParam;
    bool m_isDirty;
};

template <class T, MidiMessages>
class MidiControler{
    public:
    MidiControler() = default;
    ~MidiControler() = default;
};

template <class T>
class MidiControler<T, MidiMessages::CC>{

    public:
    MidiControler() = default;
    ~MidiControler() = default;

    void setPcNumber(int t_pcNumber){
        m_pcNumber = t_pcNumber;
    }

    void setParam(int t_ccValue){
        *(m_pParam.get()) = t_ccValue; // TODO: Need scaling;
    }

    private:
    int m_ccNumber;
};