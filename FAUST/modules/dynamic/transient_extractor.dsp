declare name "Transient Extractor";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";

import("stdfaust.lib");
import("../../lib/filters.lib");

ramp(period,reset) = (+(0):*(reset!=1))~(+(1/(period*ma.SR))):min(1);

peak_follower(attack_time,release_time,sig) = sig : envelop_extractor : peak_hold(attack_time+release_time) /*: attack_filter(attack_time) : si.onePoleSwitching(ba.tau2pole(0),ba.tau2pole(release_time))*/ with {
    envelop_extractor(sig) = sig : hilbert : (pow(2),pow(2)) : + : sqrt ;
    // peak_hold(hold_time,sig) = loop(sig)~(_) with {
    //     loop(sig) = (must_hold(sig), sig) : ba.sAndH;
    //     must_hold(sig,past_sig) = phasor(0.5,0);
    // };
    peak_hold(hold_time, sig) = main(sig)~(_) with {
        main(sig,past_sig) = /*ba.sAndH(must_release,sig)*/ must_release with {
            must_release = ramp(hold_time,>(sig,past_sig)); //TODO: Problem is here.
        } ;
    };
    attack_filter(t,sig) = sig : fi.lowpass(1,1/t) ;
    //peak_env(p,sig) = sig : (max:*(p))~(_) ;
};

process = peak_follower(attack_time,release_time) with {
    attack_time = hslider("attack_time",0.001,0,1,0.01);
    release_time = hslider("release_time",0.01,0,1,0.01);
};