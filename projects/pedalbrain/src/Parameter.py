from abc import ABC, abstractmethod
from typing import Union, TypeAlias, Dict

ParameterType: TypeAlias = Union[float,int,bool]
Number : TypeAlias = Union[float,int]

class Parameter(ABC):
    def __init__(self, default: ParameterType):
        self._default = default
        self._value = default

    def reset(self):
        self._value = self._default

    @property
    def value(self):
        return self._value
    
    @abstractmethod
    @value.setter
    def value(self, new_value):
        pass

class IntParameter(Parameter):
    def __init__(self, default: int, min: int, max: int):
        super().__init__(default)
        self._min = min
        self._max = max

    @Parameter.value.setter
    def value(self, new_value):
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

    @Parameter.value.setter
    def value(self, new_value):
        minimum = self._min
        maximum = self._max
        clamped_value = min(max(minimum, new_value), maximum)
        self._value = clamped_value
    
class BooleanParameter(Parameter):
    def __init__(self, name: str, default: bool):
        super().__init__(name, default)
        self._value = default

    @Parameter.value.setter
    def value(self, new_value):
        self._value = new_value

Parameters: TypeAlias = Dict[Parameter]

def create_parameter(name: str, type: ParameterType, default: ParameterType, minimum: Number = None, maximum: Number = None) -> Parameter:
    if type == float: return FloatParameter(name,default,minimum,maximum)
    if type == int: return IntParameter(name,default,minimum,maximum)
    if type == bool: return BooleanParameter(name,default)