declare name "Transient Extractor";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";

import("stdfaust.lib");
import("../../lib/filters.lib");

envelop_extractor = hilbert : (pow(2),pow(2)) : + : sqrt ;

// Time parameter are expected in secondes
peak_follower(att,rel,sig) = sig : envelop_extractor : peakholder : si.onePoleSwitching(att,rel) with {
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

rms_follower(rms_size,sig) = sig : envelop_extractor : ba.slidingRMS(rms_size_spl,3*48000+1) with {
    rms_size_spl = rms_size*ma.SR;
};

classic_env(att,rel,sig) = sig : envelop_extractor : si.onePoleSwitching(att,rel);

power_mean_and_variance(window_size,sig) = dc_power,fluctuation_power with {
    sig_power = mean(window_size,pow(2,sig));
    dc_power = pow(2,mean(window_size,sig));
    fluctuation_power = sig_power-dc_power;
    mean(time,x) = ba.slidingRMS(time*ma.SR,x);
};

crest_factor(window_size,sig) = (crest / rms) with {
    attack_time = hslider("attack_time",0,0,100,0.1)/1000;
    release_time = hslider("release_time",100,0,1000,0.1)/1000;
    crest = sig : peak_follower(attack_time,release_time);
    rms = rms_follower(window_size,abs(sig)) + ma.EPSILON;
};

power_crest_factor(release_time,sig) = sig : pow(2) : crest_factor(release_time) : sqrt;

spectral_power_ratio(window_len,sig) = high_end_power/rms*low_end_power/rms : sqrt with {
    FC = 500; //Hz
    window_len_smpl = window_len*ma.SR;
    low_end_power = sig : fi.lowpass(1,FC) : envelop_extractor : pow(2) : fi.lowpass(1,1/window_len);
    high_end_power = sig : fi.highpass(1,FC) : envelop_extractor : pow(2) : fi.lowpass(1,1/window_len);
    rms = ba.slidingRMS(3*ma.SR,abs(sig):pow(2)) + ma.EPSILON;
};

transient_designer(strength,sig) = _ with {
    /*
    How it should works:
    Define a threshold on the derivative of the crest factor.
    when the threshold is crossed, start to compress the signal.
    The compression enveloppe is obtain by a peak follower algorithm.
    Time constant should be short, lookahead is ideal to reduce harmonic distortion.
    */
};

transient(window_len,sig) = sig : power_crest_factor(window_len);

process = peak_follower(attack_time,release_time) with {
    attack_time = hslider("attack_time",0,0,100,0.1)/1000;
    release_time = hslider("release_time",100,0,1000,0.1)/1000;
    strength = hslider("strength",0,-100,100,0.1)/100;
    rms_size = hslider("rms_size",100,0,3000,0.1)/1000;
    pre_comp = hslider("pre_comp",100,0,3000,0.1)/1000*ma.SR;
};