declare name "Transient Extractor";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";

import("stdfaust.lib");
import("../../lib/filters.lib");

/*
TODO :
- [ ] - Refactor peakholder and release_filter to extract of timer conditionnal dsp function.
*/
envelop_extractor = hilbert : (pow(2),pow(2)) : + : sqrt ;
peak_follower(att,rel,sig) = sig : envelop_extractor : env_peak_follower(att,rel);

//Time parameter are expected in secondes
env_peak_follower(att,rel,sig) = sig : peakholder : attack_filter : release_filter with {
    attack_time = att*ma.SR; //smpls
    release_time = rel*ma.SR; // smpls
    envelop_time = attack_time + release_time; //smpls
    attack_filter_freq = 1/att; // Hz

    peakholder(x) = loop ~ si.bus(2) : ! , _ with {
        loop(timerState, outState) = timer , output with {
                is_new_peak = x >= outState;
                is_time_out = timerState >= attack_time;
                bypass = is_new_peak | is_time_out;
                timer = ba.if(bypass, 0, timerState + 1);
                output = ba.if(bypass, x, outState);
        };
    };  

    attack_filter = fi.lowpass(1,attack_filter_freq);
    release_filter(x) = loop~_ : !, _ with {
        loop(timeState) = timer, output with {
            is_time_out = timeState >= envelop_time;
            is_not_time_to_release = timeState < attack_time & is_time_out;
            bypass = is_not_time_to_release;
            timer = ba.if(is_time_out, 0, timeState + 1);
            output = ba.if(bypass, x, si.onePoleSwitching(0,rel,x));
        };
    };
};

process = peak_follower(attack_time,release_time) with {
    attack_time = hslider("attack_time",0.1,0,100,0.1)/1000;
    release_time = hslider("release_time",0.1,0,1000,0.1)/1000;
};