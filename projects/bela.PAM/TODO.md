# Bela.PAM Project management

## Backlogs:

- [ ] - Remove samplerate template from InputSection class.
- [ ] - Create an AudioBuffer class.

## Working on:

- [ ] - Add MIDI support

## Done:

- [x] - Optimized impulse response length to limit CPU usage.
  - [x] Solution: integrate the speaker sound inside the neural network modal
- [x] - Test input section implementation
  - Seems to work: there's signal at the output
- [x] - Rotary effect emulation
- [x] - Add python code for deconvolution
- [x] - Python script for pushing code to Bela
  - [x] - Push cpp audiolab librarie to `root@bela.local/Bela/libraries/audiolab`
  - [x] - Create mecanism to update project code.