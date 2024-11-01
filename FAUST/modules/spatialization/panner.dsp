import("stdfaust.lib");

pan = hslider("Blend",0.5,0,1,0.1):si.smoo;
polarity = (-1)*(checkbox("Polarity"))*2+1;

process = _ <: (*(sqrt(1-pan)),*(sqrt(pan))*polarity);