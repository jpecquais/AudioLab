import("stdfaust.lib");

declare name "Compressor";
declare author "Jean-Loup Pecquais";
declare version "1.00";
declare license "GPL3"; 

import("stdfaust.lib");

RMS_compression_gain_mono2(strength,thresh,att,rel,rms,knee,prePost) =
  RMS(rms): ba.bypass1(prePost,si.lag_ud(att,rel)) : ba.linear2db : gain_computer(strength,thresh,knee) : ba.bypass1((prePost*-1)+1,si.lag_ud(rel,att)) : ba.db2linear
with {
  gain_computer(strength,thresh,knee,level) =
    select3((level>(thresh-(knee/2)))+(level>(thresh+(knee/2))),
            0,
            ((level-thresh+(knee/2)):pow(2)/(2*knee)),
            (level-thresh)
           ) : max(0)*-strength;
  	RMS(time) = ba.slidingRMS(s) with { s = ba.sec2samp(time):int:max(1); };
};

RMS_compression_gain_N_chan2(strength,thresh,att,rel,rms,knee,prePost,link,1) =
  RMS_compression_gain_mono2(strength,thresh,att,rel,rms,knee,prePost);

RMS_compression_gain_N_chan2(strength,thresh,att,rel,rms,knee,prePost,link,N) =
  par(i, N, RMS_compression_gain_mono2(strength,thresh,att,rel,knee,prePost))
  <:(si.bus(N),(ba.parallelMin(N)<:si.bus(N))):ro.interleave(N,2):par(i,N,(it.interpolate_linear(link)));

RMS_FBFFcompressor_N_chan2(strength,thresh,att,rel,rms,knee,prePost,link,FBFF,meter,N) =
  si.bus(N) <: si.bus(N*2):
  (
    (
      (
        (ro.interleave(N,2):par(i, N*2, abs) :par(i, N, it.interpolate_linear(FBFF)) : RMS_compression_gain_N_chan2(strength*(1+(((FBFF*-1)+1)*1)),thresh,att,rel,rms,knee,prePost,link,N))
       ,si.bus(N)
      )
    :(ro.interleave(N,2):par(i,N,meter*_)) //Should implement lookahead like this meter*(_ : @(max(0,floor(0.5+ma.SR*lad))))
    )~si.bus(N)
  );

process = RMS_FBFFcompressor_N_chan2(strength,thresh,att,rel,rms,knee,prePost,0,1,meter,1)
with{
    strength = hslider("Strenght", 0, 0, 1, 0.01);
    thresh = hslider("Threshold", 0, -96, 0, 0.1);
    att = hslider("Attack", 0, 0, 120, 0.1)/1000;
    rel = hslider("Release", 0, 0, 3000, 0.1)/1000;
    rms = hslider("RMS Size", 0, 0, 3000, 0.1)/1000;
    knee = hslider("Knee", 0, 0, 24, 0.1);
    prePost = hslider("prePost", 0, 0, 1 ,1);
    meter = _<:(_, (ba.linear2db:max(maxGR):(hbargraph("[1][unit:dB][tooltip: gain reduction in dB]", maxGR, 0)))):attach;
    maxGR = 0;
};