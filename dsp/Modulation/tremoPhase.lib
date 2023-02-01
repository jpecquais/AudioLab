import("stdfaust.lib");

lfo = os.osc(8);

bpc = checkbox("bypass");

process = ba.bypass1(bpc,(_ <: (fi.crossover2LR4(500*(lfo+1.5)*0.5) : si.bus(2) : ( *(lfo+1)*.5,*(1-lfo)*.5 )  ),_ :> _ ))<: _,_;