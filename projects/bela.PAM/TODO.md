# Bela.PAM Project management

## 📥 Backlogs:

- [ ] - Create an AudioBuffer class.
- [ ] - FIXME: Default value of parameters are not applyied to FAUSTParameters.

## 🧠 Working on:

- [ ] - Implement a FIR to parallel biquad converter (Prony algorithm)
  - At this point Prony and FDLS have been roughly tested, without clear improvement over trunc impulse response. FDLS tests may have bugs. 

## 💪 Done:

- [x] - Bump frequency response of rotary sound to 4500 Hz.
- [x] - FAUSTParameter do not respond to MIDI CC messages.
- [x] - Add MIDI support
  - [x] CC Support
- [x] - Double ressources release with current C pointer implementation.
  - The issue was not comming from the float** theBuffer but from smart point pointing to address of already instanciated object.
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

## 🤯 Stuck: