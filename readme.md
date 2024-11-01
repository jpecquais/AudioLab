# AudioLab - DSP things

This repository stores many of the little projects and things I've investigated. It also serves as my own DSP library (for the best and the worst!)

## File structure

+ The **cpp** folder contains C++ codes, classes and functions.
+ The **faust** folder contains Faust codes and modules and tools.
+ The **python** folder contains research scripts and filter design stuff.

## Using faustbuild.py

`faustbuild.py` is a little utility to help compile, test and deploy Faust code. You can invoke it like this:
`python3 path/to/faustbuild.py path/to/faust_code.dsp -to some_supported_langage`. You can make the call shorter by creating an alias in your bash or zsh file: `alias faustbuild="path/to/faustbuild.py"`. All builds are located in a `.build` folder.

At this point, only JSFX is supported.

### JSFX generation

Command:
`python3 path/to/faustbuild.py path/to/faust_code.dsp -to jsfx`

Files are generated in the `.build/jsfx/Effects` directory, along with an empty REAPER session in the `.build/jsfx`. This allows us to use REAPER’s effect project feature. If you store jsfx plugins in a folder called ‘Effect` alongside a Reaper session, Reaper will scan these plugins and hide jsfx of the same name in the global path.