declare name "Wobbles";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";
// declare license "BSD"; 

import("stdfaust.lib");

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

allpass(Q,x) = +(x'-Q*x)~(*(Q));
allpass_section(n,Q,x) = x : seq(i,4,ba.bypass1(n<=i,allpass(Q)));

wobbles_mono(mix,stages,freq,depth,manual,phi,sig) = sig : hilbert <: (hilbert_path,allpass_path) : blend.power(mix) with {
    rotation = os.oscp(freq,phi)*ma.PI*depth+manual;//*num_revolutions;
    hilbert_path(real,imag) = (real,imag) : blend.angle(rotation);
    allpass_path = (_,!) : allpass_section(stages,0.96);
};

wobbles_stereo(mix,stages,freq,depth,manual,phi) = par(i,2,wobbles_mono(mix,stages,freq,depth,manual,i*phi));

process = wobbles_stereo(mix,stages,freq,depth,0,spread) with {

    // env(rel) = si.bus(2) : + : (an.amp_follower(rel)*env_sens) ;

    engine_panel(x) = hgroup("[0]engine",x);
    freq = engine_panel(vslider("[0]freq",0.5,0,8,0.1));
    spread = engine_panel(vslider("[2]spread",0.,0.,100,1)*0.005*ma.PI);

    color_panel(x) = hgroup("[1]color",x);
    mix = color_panel(1-(vslider("[0]mix",0,0,1,0.1))*0.5);
    stages = color_panel(vslider("[1]stages",0,0,4,1));

    depth = 1-(stages%2)/2;//engine_panel(vslider("[1]depth",0.,0.,1.,0.1));
};