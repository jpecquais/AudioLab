
def in_midi_range(value):
    return max(min(value,127),0)

class PC:
    def __init__(self):
        self._value: int

    def set(self, value) -> int:
        self._value = in_midi_range(value)

    def get(self, value):
        return self._value

class CC:
    def __init__(self, number: int, value: int = 0):
        self._number = in_midi_range(number)
        self._value = in_midi_range(value)
    
    def set_number(self, number: int):
        self._number = in_midi_range(number)
    
    def get_number(self) -> int:
        return self._number

    def set_value(self, value: int):
        self._value = in_midi_range(value)
    
    def get_value(self) -> int:
        return self._value
    
