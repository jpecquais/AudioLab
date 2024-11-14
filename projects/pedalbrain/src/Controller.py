from abc import ABC, abstractmethod
from typing import List,Optional

class IController(ABC):
    def __init__(self):
        pass
    
    @abstractmethod
    def dump(self):
        pass

    @abstractmethod
    def binds_to(self):
        pass

class MIDIController(IController):
    def __init__(self):
        super().__init__()
        self.midi_channel: int
        self.midi_pc: int
        self.midi_cc: List[Optional[int]] = [None]*128

class OSCController(IController):
    def __init__(self):
        super().__init__()

