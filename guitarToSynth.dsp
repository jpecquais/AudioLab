declare name "Guitar To Synth";
declare author "Jean-Loup Pecquais";
declare version "1.00";
declare license "GPL3"; 


// Try guitar signal to control an oscillator.
// First version : direct magnetic pickup, monophonic detection

// Current state :
// Envelope is "ok". There is also a constant return to 0 Hz, producing a glide effect which should be solved : sample&hold should solve this.

import("stdfaust.lib");

favorFundamentalFqc(fqc) = fi.notchw(fqc/2, fqc*2);

pitchToOsc(trig) = fi.lowpass(4, 1500) : fi.highpass(4, 40) : (favorFundamentalFqc <: *(an.amp_follower(0.02)) : an.pitchTracker(2,0.01))~(*(2)) : os.square;

ampToEnv = co.compressor_mono(20, -40, 0.001, 0.05) : *(2) : an.amp_follower(0.01) : *(10);

process = _ <: pitchToOsc(trig), (trig : en.adsr(0.01,0.1,0.7,0.1) : si.smoo ) : si.dot(1)
with {
    trig = ampToEnv>0.05;
};