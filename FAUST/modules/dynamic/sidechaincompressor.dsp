import("stdfaust.lib");
process = hgroup("Sidechain Compressor", _,_,(_<:_,_) : ro.interleave(2,2) : *(g),*(g) : outputGain(gain)) with {
    g = co.peak_compression_gain_mono(strength,thresh,att,rel,knee,prePost);
    outputGain(gain) = *(gain),*(gain);
    gain = vslider("[6]Output Gain",0,-24,24,0.1) :ba.db2linear;
    strength = co.ratio2strength(vslider("[2]Ratio",1,1,10,0.1));
    thresh = vslider("[1]Threshold",0,-80,0,0.1);
    att = vslider("[3]Attack",5,0,120,0.1)/1000;
    rel = vslider("[4]Release",80,20,500,0.1)/1000;
    knee = vslider("[5]Knee",0,0,12,0.1);
    prePost = 0;
};