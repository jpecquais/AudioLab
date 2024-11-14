from Parameter import Parameter
from State import State
import numpy as np
from dataclasses import dataclass
from typing import List, Optional, Dict, Any


class Gear:
    def __init__(self, name: str, parameters: List[Parameter]): #How do we want to construct Gear ?
        self._name: str = name
        self._parameters: Dict[Parameter]
        self._states: Dict[State]

    def save_state(self, state_name: str):
        new_state = State(state_name,self._parameters)
        self._states[new_state.get_name()] = new_state.get_properties()

    def load_state(self, state_name: str):
        state_to_load: Dict = self._states[state_name]
        for name, value in state_to_load.items():
            self._parameters[name]['value'] = value
    
    def setParameterValue(self, parameter_name: str, new_value: float):
        self._parameters[parameter_name]['value'] = new_value

    def getParameterValue(self, parameter_name: str):
        return self._parameters[parameter_name]['value']



def create_gear(gear_data: Dict[str, Any]) -> Gear:
    name = gear_data['name']
    parameters = gear_data['parameters']
    for i, (name, parameter_data) in enumerate(parameters.items()):
        parameter = Parameter(name=parameter_data['name'],
                              value=parameter_data['value'],
                              min_value=parameter_data['min_value'],
                              max_value=parameter_data['max_value'],
                              default_value=parameter_data['default_value'])
        parameters[i] = parameter
    return Gear(name=name, parameters=parameters)