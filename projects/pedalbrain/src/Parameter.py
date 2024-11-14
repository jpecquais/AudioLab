from abc import ABC, abstractmethod
from typing import Union, TypeAlias

ParameterType: TypeAlias = Union[float,int,bool]
Number : TypeAlias = Union[float,int]

class Parameter(ABC):
    def __init__(self, name: str, default: ParameterType):
        self.name = name
        self._default = default
        self._value = default

    def reset(self):
        self._value = self._default

    def getValue(self):
        return self._value
    
    @abstractmethod
    def setValue(self):
        pass

class IntParameter(Parameter):
    def __init__(self, name: str, default: int, min: int, max: int):
        super().__init__(name, default)
        self._value = default
        self._min = min
        self._max = max

    def setValue(self, new_value: float):
        minimum = self._min
        maximum = self._max
        clamped_value = min(max(minimum, new_value), maximum)
        self._value = clamped_value

class FloatParameter(Parameter):
    def __init__(self, name: str, default: float, min: float, max: float):
        super().__init__(name, default)
        self._value = default
        self._min = min
        self._max = max

    def setValue(self, new_value: float):
        minimum = self._min
        maximum = self._max
        clamped_value = min(max(minimum, new_value), maximum)
        self._value = clamped_value
    
class BooleanParameter(Parameter):
    def __init__(self, name: str, default: bool):
        super().__init__(name, default)
        self._value = default

    def setValue(self, new_value: bool):
        self._value = new_value

def create_parameter(name: str, type: ParameterType, default: ParameterType, min: Number = None, max: Number = None) -> Parameter:
    if type == float: return FloatParameter(name,default,min,max)
    if type == int: return IntParameter(name,default,min,max)
    if type == bool: return BooleanParameter(name,default)