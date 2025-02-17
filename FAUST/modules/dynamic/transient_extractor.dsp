declare name "Transient Extractor";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";

import("stdfaust.lib");
import("../../lib/filters.lib");

envelop_extractor = hilbert : (pow(2),pow(2)) : + : sqrt ;
peak_follower(att,rel,sig) = sig : envelop_extractor : env_peak_follower(att,rel);

// Time parameter are expected in secondes
env_peak_follower(att,rel,sig) = sig : peakholder : si.onePoleSwitching(att,rel) with {

    //Constant declaration
    attack_time = att*ma.SR; //smpls
    release_time = rel*ma.SR; // smpls
    envelop_time = attack_time + release_time; //smpls
    attack_filter_freq = 1/att; // Hz
    release_filter_freq = 1/rel; // Hz

    peakholder(sig) = loop ~ si.bus(2) : ! , _ with {
        loop(timer_state, out_state) = timer, sample_signal with {
            is_new_peak = sig >= out_state;
            is_attack_time_over = timer_state >= attack_time;

            resample_input = is_new_peak | is_attack_time_over;

            timer = ba.if(resample_input, 0, timer_state + 1);
            sample_signal = ba.if(resample_input, sig, out_state);
        };
    };
};
    

// };

rms_follower(rms_size,sig) = sig : envelop_extractor : ba.slidingRMSp(rms_size_spl,3*48000+1) with {
    rms_size_spl = rms_size*ma.SR;
};

classic_env(att,rel,sig) = sig : envelop_extractor : si.onePoleSwitching(att,rel);

process = _ <: (peak_follower(attack_time,release_time), classic_env(attack_time,release_time)) with {
    attack_time = hslider("attack_time",0.1,0,100,0.1)/1000;
    release_time = hslider("release_time",0.1,0,1000,0.1)/1000;
};