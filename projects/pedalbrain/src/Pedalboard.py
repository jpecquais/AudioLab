import numpy as np
from Gear import Gear

class Pedalboard:
    def __init__(self):
        self._gear_list = np.array([None]*128,dtype=object)

    def add_gear(self, new_gear: Gear, midi_channel: int):
        if self._gear_list[midi_channel] != None:
            return # TODO: Handle error here
        else:
            self._gear_list[midi_channel] = new_gear

    def get_gears(self):
        return self._gear_list