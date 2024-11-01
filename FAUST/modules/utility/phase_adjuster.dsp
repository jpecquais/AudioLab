import("stdfaust.lib");

NUM_CHANNELS = 2;

hilbert(sig) = real(sig),imag(sig) with {
    real(x) = x : fi.tf22t(-0.260502,0.02569,1.,0.02569,-0.260502) : fi.tf22t(0.870686,-1.8685,1.,-1.8685,0.870686);
    imag(x) = x : fi.tf22t(0.94657,-1.94632,1.,-1.94632,0.94657) : fi.tf22t(0.06338,-0.83774,1.,-0.83774,0.06338);
};

blend = environment {
    angle(phase,in1,in2) = in1*(cos(phase))+in2*(sin(phase));
    power(mix,in1,in2) = in1*sqrt(clamp_mix)+in2*sqrt(1-clamp_mix) with {
        clamp_mix = mix : max(0) : min(1);
    };
};

mono_chain(i) = _ : @(del_len) : hilbert : blend.angle(phase_rot) : ba.bypass1(ap_bypass,fi.svf.ap(ap_freq,ap_q)) with {
    del_len = vslider("delay_length_%i[unit:samples][style:knob]",0,0,480,1);
    phase_rot = vslider("phase_rotation_%i[unit:deg][style:knob]",0,-180,180,0.1)/180*ma.PI;

    ap_bypass = 1-checkbox("allpass_section_%i[style:knob]");
    ap_freq = vslider("allpass_freq_%i[unit:Hz][style:knob][scale:log]",100,20,20000,1);
    ap_q = vslider("allpass_q_%i[style:knob]",1,0.1,4,0.1);
};

process = si.bus(NUM_CHANNELS) : par(i,NUM_CHANNELS,mono_chain(i));