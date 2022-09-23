// Try guitar signal to control an oscillator.
// First version : direct magnetic pickup, monophonic detection

// Current state :
// Envelope is "ok". Pitch tracking often catch the first harmonic. There is also a constant return to 0 Hz, producing a glide effect which should be solved.

import("stdfaust.lib");

pitchToOsc = fi.lowpass(4, 1500) : fi.highpass(4, 40) <: *(an.amp_follower(0.01)) : an.pitchTracker(2,0.01) : os.square;
ampToEnv = an.amp_follower(0.01) : *(10);

process = _ : co.compressor_mono(20, -40, 0.001, 0.05) : *(2) <: pitchToOsc, ((ampToEnv>0.05) : en.adsr(0.01,0.1,0.7,0.1) : si.smoo ) : si.dot(1) ;//*(_ : ampToEnv) ;