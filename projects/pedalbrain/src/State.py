from dataclasses import dataclass
from Parameter import Parameter
from typing import Dict

class State:
    def __init__(self, name: str, parameters: Dict[str,Parameter]):
        self._properties: Dict[float]
        self.update(parameters)
    
    @property
    def properties(self):
        return self._properties
    
    def update(self, parameters: Dict[str,Parameter]):
        for name, properties in parameters.items():
            self._properties[name] = properties.value