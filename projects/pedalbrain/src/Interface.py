from abc import ABC, abstractmethod
from Parameter import Parameter, Parameters
from typing import TypeAlias, Dict, List

class Controller(ABC):
    def __init__(self):
        pass

    @abstractmethod
    def bind(self, parameters: Parameters):
        pass

    @abstractmethod
    def dump(self):
        pass

    @abstractmethod
    def listen(self):
        pass

class MIDIController(Controller):
    def __init__(self, midi_channel, bind_to_pc: Parameter = None):
        super().__init__()
        self._midi_channel
        self._midi_pc
        self._midi_cc_list = List[Parameter]

    def bind(self, parameters: Parameters, cc_list: List[int] = None):
        for i, (parameter_name, parameter) in enumerate(Parameters.items()):
            self._midi_cc_list[i] = parameter

    def dump(self):
        pass

    def listen(self):
        pass

class OSCController(Controller):
    def __init__(self):
        super().__init__()