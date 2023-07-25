declare name "Phase Alignment";
declare author "Jean-Loup Pecquais";
declare version "1.00";
declare license "GPL3"; 

import("stdfaust.lib");
jlpFi = library("lib/jlpFilters.lib");


//PHASE ROTATOR

phaseRotator(phi) = _ <: jlpFi.hilbert(14) : *(cos(phi)), *(sin(phi)) :> _;

//POLARITY SWITCH
/*
Simple phase inversion witch
*/

polarity(sw) = *(2*sw-1);

/////////////////////////
//PANELS & PROPERTIES///
///////////////////////

inputPanel = vgroup("[1]Input Section", polarity(sw) : jlpFi.highpassN(N,4,fc)) with{
    sw = hslider("[0]polaritySwitch", 0, 0, 1, 1);
    fc = hslider("[1]hpFrequency", 0., 0., 280., 1.):si.smoo;
    N = hslider("[2]order", 1, 1, 4, 1);
};

delayPanel = vgroup("[2]Sample Delay",ba.bypass1(1-bpd,de.sdelay(0.25*ma.SR,1024,delay))) with{
    bpd = hslider("[0]activateDelay[style:menu{'Off':0;'On':1}]",0,0,1,1);
    delay = hslider("[1]delay[unit:smpl]", 0, 0, 1100, 1):si.smoo;
};

phasePanel = vgroup("[3]Phase Rotation",ba.bypass1(1-bpp,phaseRotator(phi))) with{
bpp = hslider("[0]activatePhase[style:menu{'Off':0;'On':1}]",0,0,1,1);
phi = hslider("[1]phaseOffset[unit:°]", 0, -90, 90, 0.1)*3.141592653589793/180:si.smoo;
};

allpassPanel = vgroup("[4]Allpass Filter", ba.bypass1(1-bpap,fi.svf.ap(freq,q))) with{
    bpap = hslider("[0]activateAllpass[style:menu{'Off':0;'On':1}]",0,0,1,1);
    freq = hslider("[1]frequency[unit:Hz][scale:log]", 1000, 10, 10000, 1):si.smoo;
    q = hslider("[2]q", 1, 0.1, 10, 0.1):si.smoo;
};

///////////////////
//MAIN FUNCTION///
/////////////////

process = par(i,2,tgroup("[0]Phase Alignment Tool", inputPanel : phasePanel : allpassPanel : delayPanel));