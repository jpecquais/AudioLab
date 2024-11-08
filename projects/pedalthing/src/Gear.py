import MIDI
from dataclasses import dataclass
from typing import List, Optional

@dataclass
class GearData:
    name: str
    channel: int
    pc: MIDI.PC
    cc: List[MIDI.CC]
    tempo: Optional[float] = None

class Gear:
    def __init__(self, data: GearData):
        self._midi_channel = min(max(GearData.channel,1),16)
        self._name = GearData.name
        self._pc   = GearData.pc
        self._cc   = GearData.cc

    def get_channel(self) -> int:
        return self._midi_channel

    def get_name(self):
        return self._name

    def push(self):
        # Send MIDI data to device
        pass