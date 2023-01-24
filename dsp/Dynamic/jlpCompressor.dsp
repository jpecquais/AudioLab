import("stdfaust.lib");

declare name "Compressor";
declare author "Jean-Loup Pecquais";
declare version "1.00";
declare license "GPL3"; 


//TODO : TO BE IMPLEMENTED IN A PROPER LIB
/*
Attack Hold Release Envelop.
Create an envelop based on an input signal compared to a threshold.

The attack determines how long the envelop take to be fully open, once the signal is superior to the threshold.
The release determines how long the envelop take to be fully closed, once the signal is inferior to the threshold.
The hold determines how long the envelop hold its value before going to the release phase, once the signal is inferior to the threshold.

Based on FAUST function : en.adsr()

*/
declare ahr author "Jean-Loup Pecquais";
declare ahr licence "GPL3";
ahr(at,rt,ht,thresh,x) = A : *(1-R) : max(0)
with {

    // Durations in samples
    an = max(1, at*ma.SR);
    hn = max(1, ht*ma.SR);
    rn = max(1, rt*ma.SR);

    // Deltas per samples
    adelta = 1/an;
    ddelta = (1-sl)/dn;

    // Attack time 
    atime =  +(x>=thresh) ~ *(x>=thresh);

    // Attack curve
    A = min(atime * adelta, 1);

    // Release time starts when gate is 0
    rtime =  +(x<=thresh) ~ *(x<=thresh);

    // Release curve starts when gate is 0 with the current value of the envelope
    R = ba.if(rtime<=hn, 1, (rtime+hn)/rn);

};


// TODO : TO BE IMPLEMENTED IN A PROPER LIB
// TODO : RENAMING FUNCTIONS NAME
RMS_compression_gain_mono2(strength,thresh,att,hld,rel,rms,knee,prePost) =
  fi.highpass(4,10) : RMS(rms): ba.linear2db : gain_computer(strength,thresh,knee) : ba.db2linear <: *(ahr(att,hld,rel,loThresh)) 
with {
    gate = 1;
    loThresh = thresh-(knee/2);
    gain_computer(strength,thresh,knee,level) =
        select3((level>loThresh)+(level>(thresh+(knee/2))),
                0,
                ((level-thresh+(knee/2)):pow(2)/(2*knee)),
                (level-thresh)
                ) : max(0)*-strength;
  	RMS(time) = ba.slidingRMS(s) with { s = ba.sec2samp(time):int:max(1); };
};

RMS_compression_gain_N_chan2(strength,thresh,att,rel,hld,rms,knee,prePost,link,1) =
  RMS_compression_gain_mono2(strength,thresh,att,rel,hld,rms,knee,prePost);

RMS_compression_gain_N_chan2(strength,thresh,att,rel,hld,rms,knee,prePost,link,N) =
  par(i, N, RMS_compression_gain_mono2(strength,thresh,att,rel,knee,prePost))
  <:(si.bus(N),(ba.parallelMin(N)<:si.bus(N))):ro.interleave(N,2):par(i,N,(it.interpolate_linear(link)));

//TODO : FORCE FEEDFORWARD WHEN LAH IS ON
RMS_FBFFcompressor_N_chan2(strength,thresh,att,rel,hld,rms,knee,lad,prePost,link,FBFF,meter,N) =
  si.bus(N) <: si.bus(N*2):
  (
    (
      (
        (ro.interleave(N,2):par(i, N*2, abs) :par(i, N, it.interpolate_linear(FBFF)) : RMS_compression_gain_N_chan2(strength*(1+(((FBFF*-1)+1)*1)),thresh,att,rel,hld,rms,knee,prePost,link,N))
       ,si.bus(N)
      )
    :(ro.interleave(N,2):par(i,N,meter*@(max(0,floor(0.5+ma.SR*lad)))))
    )~si.bus(N)
  );

process = RMS_FBFFcompressor_N_chan2(strength,thresh,att,rel,hld,rms,knee,lad,prePost,0,1,meter,1)
with{
    strength = hslider("Strenght", 0, 0, 1, 0.01);
    thresh = hslider("Threshold", 0, -96, 0, 0.1);
    att = hslider("Attack", 0, 0, 120, 0.1)/1000;
    rel = hslider("Release", 0, 0, 3000, 0.1)/1000;
    hld = hslider("Hold", 0, 0, 3000, 0.1)/1000;
    rms = hslider("RMS Size", 0, 0, 3000, 0.1)/1000;
    knee = hslider("Knee", 0, 0, 24, 0.1);
    lad = ba.if(checkbox("Perfect attack"),att, 0);//hslider("Look Ahead", 0, 0, 120, 0.1);
    prePost = hslider("prePost", 0, 0, 1 ,1);
    meter = _<:(_, (ba.linear2db:max(maxGR):(hbargraph("GainReduction[1][unit:dB][tooltip: gain reduction in dB]", maxGR, 0)))):attach;
    maxGR = 0;
};