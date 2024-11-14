from dataclasses import dataclass
from Parameter import Parameter
from typing import Dict

class State:
    def __init__(self, name: str, parameters: Dict[str,Parameter]):
        self._name = name
        self._properties: Dict[float]
        for name, properties in parameters.items():
            self._properties[name] = properties['value']

    def get_name(self):
        return self._name
    
    def get_properties(self):
        return self._properties