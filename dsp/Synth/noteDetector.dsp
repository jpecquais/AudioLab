declare name "Note Detector";
declare author "Jean-Loup Pecquais";
declare version "1.00";
declare license "GPL3"; 

import("stdfaust.lib");

sigVariation = fi.conv((1,-1)); //Allows to compute de derivation of the input signal

process = sigVariation : abs : *(10);