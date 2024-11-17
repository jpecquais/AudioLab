from Parameter import Parameter, create_parameter
from State import State
import numpy as np
from dataclasses import dataclass
from typing import List, Optional, Dict, Any


class Gear:
    def __init__(self, name: str, parameters: Dict[Parameter]): 
        # Gears are constructed by create_gear()
        self._name: str = name # as in parameter, name may not belong here.
        self._parameters: Dict[Parameter] = parameters
        self._states: Dict[State]

    def save_state(self, state_name: str):
        self._states[state_name] = State(self._parameters)

    def load_state(self, state_name: str):
        state_to_load = self._states[state_name]
        for name, value in state_to_load.properties.items():
            self._parameters[name].value = value

    def update_state(self, state_name: str):
        state_to_update = self._states[state_name]
        for name, value in state_to_update.properties.items():
            value = self._parameters[name].value

    @property
    def parameters(self):
        return self._parameters



def create_gear(gear_data: Dict[str, Any]) -> Gear:
    name = gear_data['name']
    parameters_data = gear_data['parameters']
    dict_parameter = {}
    for i, (name, parameter_data) in enumerate(parameters_data.items()):
        parameter_name = parameter_data['name']
        parameter = create_parameter(name=parameter_data['name'],
                                     type=parameter_data['type'],
                                     default=parameter_data['default'],
                                     minimum=parameter_data['minimum'] or None,
                                     maximum=parameter_data['maximum'] or None)
        dict_parameter[parameter_name] = parameter
    return Gear(name=name, parameters=dict_parameter)