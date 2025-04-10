declare name "Wobbles";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";
// declare license "BSD"; 

/* 
TODOLIST:

- [x] : Remove mix parameter
- [x] : Remove manual parameter
- [ ] : implement internal frequency control of allpasses

*/

import("stdfaust.lib");


hilbert(sig) = real(sig),imag(sig) with {
    real(x) = x : fi.tf22t(-0.260502,0.02569,1.,0.02569,-0.260502) : fi.tf22t(0.870686,-1.8685,1.,-1.8685,0.870686);
    imag(x) = x : fi.tf22t(0.94657,-1.94632,1.,-1.94632,0.94657) : fi.tf22t(0.06338,-0.83774,1.,-0.83774,0.06338);
};

envelop = environment{

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

    rms_follower(rms_size,sig) = sig : envelop_extractor : ba.slidingRMS(rms_size_spl) with {
        rms_size_spl = rms_size*ma.SR;
    };

    crest_factor(window_size,sig) = (crest / rms) with {
        attack_time = 0.001;
        release_time = 0.2;
        crest = sig : peak_follower(attack_time,release_time);
        rms = rms_follower(window_size,sig) + ma.EPSILON;
    };

    power_crest_factor(release_time,sig) = sig : pow(2) : crest_factor(release_time) : sqrt;

    transient(window_len,sig) = sig : power_crest_factor(window_len);
};


lf_random(freq) = no.noise : peakholder : fi.lowpass(1,freq) with {
    hold_time = 1/freq*ma.SR;
    peakholder(sig) = loop ~ si.bus(2) : ! , _ with {
        loop(timer_state, out_state) = timer, sample_signal with {
            is_time_out = timer_state >= hold_time;

            resample_input = is_time_out;

            timer = ba.if(resample_input, 0, timer_state + 1);
            sample_signal = ba.if(resample_input, sig, out_state);
        };
    };
};

multi_lfo(freq,amplitude,offset,is_unipolar,shape) = selected_lfo with {

    NUM_OF_LFO = 2;
    actual_shape = abs(shape);

    unipolar_lfo(x) = x*(is_unipolar+(1-is_unipolar)*(0.5))+0.5*(1-is_unipolar);
    bipolar_lfo(x) = x*(1-is_unipolar+(is_unipolar)*(0.5))+0.5*(is_unipolar);

    sine = os.osc(freq)*amplitude+offset;
    triangle = os.lf_triangle(freq)*amplitude+offset;
    saw = os.lf_sawpos(freq)*amplitude*shape;

    selected_lfo = (sine,saw) : ba.selectn(NUM_OF_LFO,actual_shape);

};

blend = environment {
    angle(phase,in1,in2) = in1*(cos(phase))+in2*(sin(phase));
    power(mix,in1,in2) = in1*sqrt(clamp_mix)+in2*sqrt(1-clamp_mix) with {
        clamp_mix = mix : max(0) : min(1);
    };
};

allpass(Q,x) = +(x'-Q*x)~(*(Q));
allpass_section(n,Q,x) = x : seq(i,4,ba.bypass1(n<=i,allpass(Q)));

wobbles_mono(mode,freq,intensity,phase,sheperd_mode,sig) = sig : hilbert <: (hilbert_path,allpass_path) : blend.power(mix) with {

    NUM_MODES = 5;
    NUM_MODES_PROPERTIES = 5;

    is_barberpol = sheperd_mode != 0;

    vibrato_mode = lfo_amplitude,lfo_amplitude_offset,num_stages,mix,f_ratio with {
        lfo_amplitude = ba.if(is_barberpol,1,intensity)*2*ma.PI;
        lfo_amplitude_offset = 0;
        num_stages = 0;
        mix = 1;
        f_ratio = 1;
    };
    tremolo_mode = lfo_amplitude,lfo_amplitude_offset,num_stages,mix,f_ratio with {
        lfo_amplitude = ba.if(is_barberpol,2,1)*ma.PI;
        lfo_amplitude_offset = 0;
        num_stages = 0;
        mix = 0.5*intensity;
        f_ratio = 0.75;
    };
    harmtrem_mode = lfo_amplitude,lfo_amplitude_offset,num_stages,mix,f_ratio with {
        lfo_amplitude = ba.if(is_barberpol,2,0.5)*ma.PI;
        lfo_amplitude_offset = -0.5*ma.PI;
        num_stages = 1;
        mix = 0.5*intensity;
        f_ratio = 1.5;
    };
    two_phasor_mode = lfo_amplitude,lfo_amplitude_offset,num_stages,mix,f_ratio with {
        lfo_amplitude = ba.if(is_barberpol,2,1)*ma.PI;
        lfo_amplitude_offset = 0;
        num_stages = 2;
        mix = 0.5*intensity;
        f_ratio = 1;
    };
    four_phasor_mode = lfo_amplitude,lfo_amplitude_offset,num_stages,mix,f_ratio with {
        lfo_amplitude = ba.if(is_barberpol,2,1)*ma.PI;
        lfo_amplitude_offset = 0;
        num_stages = 4;
        mix = 0.5*intensity;
        f_ratio = 1;
    };

    current_mode = (vibrato_mode,tremolo_mode,harmtrem_mode,two_phasor_mode,four_phasor_mode) : ba.selectbus(NUM_MODES_PROPERTIES,NUM_MODES,mode);
    lfo_amplitude = ba.selectn(NUM_MODES_PROPERTIES,0,current_mode);
    lfo_amplitude_offset = ba.selectn(NUM_MODES_PROPERTIES,1,current_mode);
    num_stages = ba.selectn(NUM_MODES_PROPERTIES,2,current_mode);
    mix = ba.selectn(NUM_MODES_PROPERTIES,3,current_mode);

    f_ratio = ba.selectn(NUM_MODES_PROPERTIES,4,current_mode);
    env_depth = hslider("env_depth",0,0,20,0.01);
    env = (envelop.transient(1,sig)-1)*env_depth;

    actual_freq = freq*f_ratio+env;
    lfo = multi_lfo(actual_freq,lfo_amplitude,lfo_amplitude_offset,0,sheperd_mode);

    hilbert_path(real,imag) = (real,imag) : blend.angle(lfo);
    allpass_path(real,imag) = (real,(imag:!)) : allpass_section(num_stages,0.96);

};

// process = os.lf_sawpos(freq),os.lf_sawpos(freq)*(-1) with {
process = wobbles_mono(stages,freq,depth,0,sheperd_mode) <: _,_ with {

    freq = vslider("[0][scale:log]freq",1.2,0.1,100,0.01);
    depth = vslider("[2]depth",0.5,0.,1.,0.01);
    stages = vslider("[3]mode",2,0,4,1);
    sheperd_mode = vslider("[4]sheperd_mode",0,-1,1,1);

};