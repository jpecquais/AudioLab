#pragma once
#include <libraries/Midi/Midi.h>
#include <memory>
#include "Parameter.h"

template <class T>
class MidiController{
    public:
    MidiController() = default;
    ~MidiController() = default;

    void setup(const std::string midiPort){
        m_midi.readFrom(midiPort.c_str());
        m_midi.writeTo(midiPort.c_str());
        m_midi.enableParser(true);
    }

    void attachParameterToCC(Parameter<T>& t_parameter, int t_cc){
        m_ccMessages[t_cc].reset(&t_parameter);
    }
    
    void parse(){
        while(m_midi.getParser()->numAvailableMessages()>0){
            MidiChannelMessage message = m_midi.getParser()->getNextChannelMessage();
            if (message.getType() == kmmControlChange){
                //Do stuff
            }
        }
    }

    private:
    Midi m_midi;
    std::array<std::unique_ptr<Parameter<T>>,128> m_ccMessages;
};