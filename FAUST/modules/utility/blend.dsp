import("stdfaust.lib");

blend = hslider("Blend",0.5,0,1,0.1);
polarity = (-1)*(checkbox("Polarity"))*2+1;

process = +(*(blend),*((1-blend)*polarity));