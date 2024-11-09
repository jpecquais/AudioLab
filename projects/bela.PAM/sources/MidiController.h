#pragma once
#include <libraries/Midi/Midi.h>
#include <memory>
#include "Parameter.h"

struct CC{
    midi_byte_t number;
    midi_byte_t value;
};

CC getCC(const MidiChannelMessage t_message){
    CC new_CC;
    new_CC.number = t_message.getDataByte(1);
    new_CC.value = t_message.getDataByte(2);
    return new_CC;
}

template <class T>
class MidiController{
    public:
    MidiController() = default;
    ~MidiController() = default;

    void setup(const std::string t_midiPort, const int t_midiChannel){
        m_midi.readFrom(t_midiPort.c_str());
        m_midi.writeTo(t_midiPort.c_str());
        m_midi.enableParser(true);
        m_midi.setParserCallback(callback, (void *)t_midiPort.c_str());
        m_midiChannel = t_midiChannel;
    }

    void attachParameterToCC(Parameter<T>& t_parameter, int t_cc){
        m_ccMessages[t_cc].reset(&t_parameter);
    }
    
    private:
    void callback(){
        while(m_midi.getParser()->numAvailableMessages()>0){
            MidiChannelMessage message = m_midi.getParser()->getNextChannelMessage();
            if (message.getChannel() != m_midiChannel) return;
            if (message.getType() == kmmControlChange){
                &m_ccMessages[message.getDataByte(0)]->setValueFromMidi(message.getDataByte(1));
            }
        }
    }

    Midi m_midi;
    int m_midiChannel;
    std::array<std::unique_ptr<Parameter<T>>,128> m_ccMessages;
};