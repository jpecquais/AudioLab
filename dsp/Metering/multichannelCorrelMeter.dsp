import("stdfaust.lib");

//process = _,_ <: ( * : fi.lowpass(2,10)) , ((^(2) : fi.lowpass(2,10) , ^(2) : fi.lowpass(2,10)) : * : sqrt) : /;
N = 10;
correlMeterTop = * : fi.lowpass(2,N);
correlMeterBottom  = ((^(2) : fi.lowpass(2,N)) , (^(2) : fi.lowpass(2,N))) : *  : sqrt;
correlMeter(i) = (correlMeterTop, correlMeterBottom) : / : hbargraph("CorrelMeter %i", -1, 1);
process = par(i,5,correlMeter(i));//no.pink_noise : fi.mth_octave_filterbank5(3,20000,12) : par(i, 12, (_ <: (_, @(1000)) <: correlMeter(i) :> !)) : no.noise;//correlMeter ;
// : par(i,N,_)