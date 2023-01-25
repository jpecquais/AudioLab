# FAUST Lab Repository

This repository host generic FAUST .lib and .dsp file, mainly for plugin building purpose.

## Example of terminal command to build a .dsp file

    faust -a tools/faustMinimal.h -O build -svg dsp/Dynamic/jlpCompressor.dsp -o jlpCompressor.h

This command create a .h file and the SVG diagram (very useful to debug) in the build folder.