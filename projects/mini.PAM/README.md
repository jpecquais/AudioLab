# mini.PAM

## Presentation

mini.PAM is a project that aimed at reproducing a guitar power amp attached to a loudspeaker. It uses traditionnal DSP techniques (as in not machine learning or AI of any sort). It is designed to run on a Daisy Seed microcontroller and is written in FAUST.

### Power amp model

The power amp is inspired by class AB design with a feedback loop (presence control). Tubes are modeled using an antialiased hyperbolic waveshaper. The output transformer is modeled using an antialiased hyperbolic tangent. Sag and dynamic bias are include in the algorithm to improve the response feel.

### Speaker model

The speaker are split in two part : linear and non-linear part. The non linear part mimics the displacement distortion of a speaker using a waveshaper. The linear part use a FIR to IIR algorithm using the prony method to provide an economical way to approach a 1024 tap FIR usually for linear speaker modelisation.

## Build

+ Clone the AudioLab mono-repo.
+ Init and pull all submodules.
+ Go to AudioLab/third-party/daisy
  + make the DaisySP lib (make command inside the folder)
  + make the libDaisy lib (make command inside the folder)
+ Go to AudioLab/project/mini.PAM
  + make the project (make command)
+ Use the web tool of Daisy to upload the build/miniPAM.bin to the Daisy Seed micro-controller
+ Have fun !