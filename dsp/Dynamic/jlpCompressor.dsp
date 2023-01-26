import("lib/jlpLibs.lib");
import("stdfaust.lib");

declare name "Compressor";
declare author "Jean-Loup Pecquais";
declare version "1.00";
declare license "GPL3"; 

// TODO : TO BE IMPLEMENTED IN A PROPER LIB

//TODO : FORCE FEEDFORWARD WHEN LAH IS ON
comp_BigBrother_nCh(strength,thresh,att,rel,hld,rms,knee,lad,link,FBFF,meter,N) =
  si.bus(N) <: si.bus(N*2):
  (
    (
      (
        (ro.interleave(N,2):par(i, N*2, abs) :par(i, N, it.interpolate_linear(FBFF)) : jlpDyn.comp_genericGainComputer_nCh(strength*(1+(((FBFF*-1)+1)*1)),thresh,att,rel,hld,rms,knee,link,N))
       ,si.bus(N)
      )
    :(ro.interleave(N,2):par(i,N,meter*@(max(0,floor(0.5+ma.SR*lad)))))
    )~si.bus(N)
  );







comp_HybridComp_nCh(strength,thresh,att,rel,hld,rms,knee,lad,link,FBFF,meter,N) =
  si.bus(N) <: si.bus(N*2) :(jlpDyn.comp_genericGainComputer_nCh(strength,thresh,att,rel,hld,0.005,knee,0,N),_) : 
  (
    (
      (( ro.interleave(N,2) :  par(i, N, it.interpolate_linear(FBFF))),si.bus(N))
      : (ro.interleave(N,2) : par(i,N,meter*@(max(0,floor(0.5+ma.SR*lad)))))
    )~(si.bus(N) <: jlpDyn.comp_genericGainComputer_nCh(strength,thresh,att,rel,hld,rms,knee,link,N))
  );
  





comp_HybridComp_nCh2(strength,thresh,att,rel,hld,rms,knee,lad,link,FBFF,meter,N) =
  si.bus(N) <: si.bus(N*2) :
  (
    (
      ((jlpDyn.comp_genericGainComputer_nCh(strength,thresh,att,rel,hld,0.005,knee,0,N)),si.bus(N))
      : (ro.interleave(N,2) : par(i,N,meter*@(max(0,floor(0.5+ma.SR*lad)))))
    )
  );




comp_HybridComp_nCh3(strength,thresh,att,rel,hld,rms,knee,lad,link,FBFF,meter,N) =
  si.bus(N) : 
  (
    (
      (si.bus(N),si.bus(N))
      : (ro.interleave(N,2) : par(i,N,meter*@(max(0,floor(0.5+ma.SR*lad)))))
    )~(si.bus(N) <: jlpDyn.comp_genericGainComputer_nCh(strength,thresh,att,rel,hld,rms,knee,link,N))
  );
  





FBFFcompressor(strength,thresh,att,rel,knee,prePost,link,FBFF,meter,N) =
  si.bus(N) <: si.bus(N*2) :
  (
    ((par(i,2,co.peak_compression_gain_N_chan_db(strength,thresh,att,rel,knee,prePost,link,N)) : ro.interleave(N,2) : par(i,N,it.interpolate_linear(FBFF))),si.bus(N))
    : (ro.interleave(N,2) : par(i,N,(meter : ba.db2linear)*_))
  )
  ~ si.bus(N);









//process = jlpDyn.comp_genericGainComputer_nCh(strength,thresh,att,rel,hld,0.005,knee,0,2)
//process = co.peak_compression_gain_N_chan_db(strength,thresh,att,rel,knee,0,0,2)
//process = jlpDyn.comp_genericGainComputer_nCh(strength,thresh,att,rel,hld,rms,knee,0,2)
process = comp_HybridComp_nCh2(strength,thresh,att,rel,hld,rms,knee,lad,0,1,meter,2)
//process = FBFFcompressor(strength,thresh,att,rel,knee,0,1,1,meter,2)
with{
    strength = hslider("Strenght", 0, 0, 1, 0.01);
    thresh = hslider("Threshold", 0, -96, 0, 0.1);
    att = hslider("Attack", 0, 0, 120, 0.1)/1000;
    rel = hslider("Release", 0, 0, 3000, 0.1)/1000;
    hld = hslider("Hold", 0, 0, 3000, 0.1)/1000;
    rms = hslider("RMS Size", 0, 0, 3000, 0.1)/1000;
    knee = hslider("Knee", 0, 0, 24, 0.1);
    lad = ba.if(checkbox("Perfect attack"),att, 0);//hslider("Look Ahead", 0, 0, 120, 0.1);
    meter = _<:(_, (ba.linear2db:max(maxGR):(hbargraph("GainReduction[1][unit:dB][tooltip: gain reduction in dB]", maxGR, 0)))):attach;
    maxGR = 0;
};