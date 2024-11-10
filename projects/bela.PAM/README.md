# Bela.PAM

The Bela.PAM project is a DSP implementation of a guitar power amp, a guitar speaker, and the effect of a rotating speaker. PAM stands for "Power Amp Model".

## Inspiration

While there is a lot of literature on the infamous Leslie cabinet, there's not much around the more generic rotating speaker cabinets. Some iconic machines, such as the Fender Vibratone, the Yamaha RA100/200 or the Gibson Maestro Rover RO-1, have quite different designs and, therefore, give different tones.

Additionally, most Leslie simulations also simulate the cabinet’s frequency response. Applying it on top of a guitar speaker (whether real or simulated) can result in a less than ideal sound.

## Processing structure

This project is highly tailored, and not all features may be relevant in a different application scenario.

The Bela.PAM is made up of four distinct sections:

+ The input section
+ The power amp section
+ the cabinet section
+ the rotating effect simulation

### Input section

I have an unconventional configuration, with two tube preamps, the Effectrode Blackbird and Kingsley Constable, connected in parallel. To get the most out of them, it is therefore desirable to do some ‘phase-EQ’ to make sure they are perfectly summed together. The input section performs this function, maintaining the balance between the two preamps and adjusting their phase relationship.

### Power Amp Section

The power amp section uses the [RTNeural](https://github.com/jatinchowdhury18/RTNeural) framework and a custom power amp model. The
power amp model has been trained by following the guidelines for the [NeuralSeed](https://github.com/GuitarML/NeuralSeed) project.

The Laney LionHeart L20H-410 power amp serves as the model for this implementation. Due to the limited CPU resources on the Bela, a GRU layer with a hidden size of 8. Because this specific power amp is quite clean, the model is still very convincing.

The specificity of this model is that it was created by recording the speaker's output. The frequency response of the speaker was then removed through a deconvolution process.

### Cabinet Section

The cabinet section is a straightforward time-domain convolution with the impulse response of the cabinet, which has been carefully crafted using [Cabinetron](https://www.threebodytech.com/en/products/cabinetron?from=Cabinetron-Walkthrough) and IRs from [York Audio](https://www.yorkaudio.co/).

### Rotating Effect

The emulation of the speaker rotation effect is written in FAUST and tries to mimic the Yamaha RA100/200 or the Maestro Rover RO-1. This is by no means a physical model for any of these two units, since I do not own either. The effect is
purely based on recordings of these machines and on trying to reproduce their perceptual effect **by ear**.

While there are a few differences between a Leslie and a Yamaha RA200, the most notable is the fact that there is no rotating effect on the lower end of the spectrum. Additionally, the Yamaha rotating speaker features several spinning heads that contribute to a more complex effect. This implementation addresses these two distinctions.

## User Interface

There is no user interface for this project. The power amp has no settings, nor the cabinet simulation or the input section. The rotating effect has a slow/fast and a running/stop switch, as well as a dry/wet parameter. All these settings can only be accessed through MIDI. Additionally, users can enable a Bypass mode for both the power amp and cabinet, making it possible to connect directly to a traditional guitar amplifier’s effects loop.

## More Details

I have written many articles on my [blog](www.jeanlouppecquais.com/blog) about this project. Although they are all in French, there must be some great translation tools for websites in the age of AI. Diagrams are also written in both French and English, just in case.