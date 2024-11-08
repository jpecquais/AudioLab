# Pedalthing

## Pitch

Pedalthing is a MIDI remote for pedalboard. The concept is based around the idea of having a simple software solution to manage pedalboard presets, songs and setlist. Of course, you must have MIDI equipments to be able to use pedalthing.


## Programmatic approach

### Class description

A **Gear** is an equipment that support midi. The class itself is an abstract class and you must implement your various equipment by make new classes that inherit from **Gear**. A **Gear** receive MIDI data from pedalpitch.

A **Controller** is an equipment that support midi. Like a Gear, specific equipment must be implemented by deriving from **Controller**, which is an abstract class. A **Controller** emit MIDI data to pedalpitch.

A **Preset** is a data structure that hold all MIDI data for all Gear. A **Preset** can be recall (send all data to all gears).

A **Song** is an ordered list of *Presets*.

A **Setlist** is an ordered list of *Setlist*.

Note that Presets can be shared by several songs, and a song can be shared by several setlist. Thus, Pedalthing encourage to design presets in a generic way.