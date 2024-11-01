import("stdfaust.lib");

tanh = aa.tanh1;

xlim(x) = (4*x/(coefficient*(abs(x)+1)))~_ with {
    coefficient(y) = 1+abs(y/(x+1e-14));
};

xcube(gain,x) = x*(1-gain)+pow(x,3)*gain;

env(x) = abs(x) : si.smoo : tanh;
phase_splitter(gain,x) = _ <: (xlim , (*(-1) : (xcube(gain) : xlim : +(gain))));

powertube(bias,x) = +(x,bias) <: ba.if(_>0.1,_,0) : xlim;
transformer(inphase,outphase) = inphase - outphase : xlim;

push_pull(bias,x) = phase_splitter(env(x)*0.5,x) : (powertube(bias),powertube(bias)) : transformer;

negative_feedback_loop(x,presence) = *(x,-2) : fi.peak_eq_cq(presence,5000,0.5);

process = (+ <: push_pull(bias))~(negative_feedback_loop(presence)) : fi.dcblocker with{
    bias = hslider("bias",0.2,0,1,0.01) : si.smoo;
    presence = hslider("presence",1,0.5,2,0.01) : si.smoo;
};
