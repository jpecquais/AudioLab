# Bela.PAM Project management

## Backlogs:

- [ ] - Create an AudioBuffer class.
  - [ ] - FIXME: Double ressources release with current C pointer implementation.
- [ ] - Implement a FIR to parallel biquad converter (Prony algorithm)

## Working on:

- [ ] - Add MIDI support

## Done:

- [x] - Remove samplerate template from InputSection class.
- [x] - Optimized impulse response length to limit CPU usage.
  - [!] Solution: integrate the speaker sound inside the neural network modal. DID NOT WORK
  - [x] Currently use a shorter impulse response (256 samples). **Conversion to IIR could improve fidelity to full length IR**.
- [x] - Test input section implementation
  - Seems to work: there's signal at the output
- [x] - Rotary effect emulation
- [x] - Add python code for deconvolution
- [x] - Python script for pushing code to Bela
  - [x] - Push cpp audiolab librarie to `root@bela.local/Bela/libraries/audiolab`
  - [x] - Create mecanism to update project code.