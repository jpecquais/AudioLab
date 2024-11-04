#pragma once
#include <libraries/Midi/Midi.h>

class MidiControler{
    public:
    MidiControler() = default;
    ~MidiControler() = default;

    void setup(std::str midiPort){
        m_midi.readFrom(midiPort);
        m_midi.writeTo(midiPort);
        m_midi.enableParser(true);
    }
    
    void parse{
        while(m_midi->getParser()->numAvailableMessages()>0){
            MidiChannelMessage message = m_midi.getParser()->getNextChannelMessage();
            if (message.getType() == kmmControlChange){
                //Do stuff
            }
        }
    }

    private:
    Midi m_midi;
}