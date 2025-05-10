declare author "Jean-Loup Pecquais";
declare version "1.00";

import("../../lib/filters.lib");

/*
Bugs -
[ ] - when stop is engaged, discontinuity happend during the reset phase
[ ] - when the rotary is stop, a clear filtering is happening, which depend on stereo width...
*/

C_SOUND = 340;
INV_C_SOUND = 0.0029411764705882353;
GAIN_ATTENUATION = -20;
MAX_DELAY_LEN = 1024; //smpls
ONE_OVER_SQRT_2 = 0.707;
TWO_PI = 2*ma.PI;
PI_OVER_FOUR = ma.PI*0.25;
T_S = 1/ma.SR;

cm_to_samples(cm) = cm*0.01 : meter_to_samples : int;
meter_to_samples(m) = m : *(INV_C_SOUND*ma.SR) : int;
inch_to_meter(inch) = inch : *(0.0254);
inch_to_samples(inch) = inch : inch_to_meter : meter_to_samples : int;

main_lfo_shape(x) = abs(sin(x));
theta_to_front_back(x) = x-0.5*sin(2*x);

engine(freq,is_stop,inertia) = angular_position with {
    is_turning = 1-is_stop;
    internal_freq = freq*(is_turning) : fi.avg_tau(inertia);
    reset_threshold = internal_freq < 0.1;
    conditional_smoothing(sig) = select2(reset_threshold,sig,(sig:fi.avg_tau(inertia*0.1)));
    angular_position = os.lf_sawpos_reset(internal_freq,reset_threshold) : conditional_smoothing : *(TWO_PI);
};

speaker(theta,diameter,shaft_len,sig_real,sig_imag) = phase_model : directivity_model : doppler_effect with {

    directivity_model =  fi.lowpass(1,f_lowpass) : fi.svf.hs(fc_highshelf,0.707,gain_highshelf) : fi.ff_fcomb(MAX_DELAY_LEN,dist_smpls,0.5,0.5) with {
        
        static_f_lowpass = C_SOUND/(2*inch_to_meter(diameter)*sin(PI_OVER_FOUR));
        f_lowpass = 20000-(20000-static_f_lowpass)*(main_lfo_shape(theta));

        fc_highshelf = 2*static_f_lowpass;
        gain_highshelf = GAIN_ATTENUATION*(main_lfo_shape(0.5*theta_to_front_back(theta)));

        dist_smpls = inch_to_samples(diameter)*(main_lfo_shape(theta));

    };
    doppler_effect = de.fdelay(MAX_DELAY_LEN,dist) with {
        dist = 2*cm_to_samples(shaft_len)*(main_lfo_shape(0.5*theta));
    };
    phase_model = real_path, imag_path : + with {
        phase = theta_to_front_back(theta);
        real_path = *(sig_real,cos(phase));
        imag_path = *(sig_imag,sin(phase));
    };
};

rotative_speaker(freq,is_stop,inertia,diameter,shaft_len,sig_real,sig_imag) = speaker(theta,diameter,shaft_len,sig_real,sig_imag) : rotating_compensation_filter with {
    
    theta = engine(freq,is_stop,inertia);

    rotating_compensation_filter = fi.svf.hs(f_comp_filter,q_comp_filter,g_comp_filter);
    f_comp_filter = 2*C_SOUND/(2*inch_to_meter(diameter)*sin(PI_OVER_FOUR));
    q_comp_filter = 0.3;
    g_comp_filter = 15*(1-is_stop) : fi.avg_tau(inertia);
};

rotary_cabinet(sig) = effect_path, dry_sig : ro.cross2 : (+,+)  with {

    freq = hslider("freq (Hz)",1,0.1,10,0.1);
    mix = hslider("mix (%)",100,0.,100,0.1)*0.01 : si.smoo;
    inertia = hslider("inertia time (s)",.5,0,10,0.1);
    stop_state = hslider("stop (on/off)",0,0,1,1);
    stereo_width = hslider("stereo width (%)",100,0,100,0.1)*0.01 : sqrt : si.smoo;

    center_gain = (1-stereo_width);
    side_gain = stereo_width;

    diameter = 6; //inch
    shaft_len = 10; //cm
    f_1 = freq;
    f_2 = freq+0.17;
    f_3 = freq-0.11;
    f_x = 600;

    input_section = sig : hilbert : crossover;
    crossover = si.bus(2) <: si.bus(4) : ro.cross2 : par(i,2,fi.lowpass(1,f_x),fi.highpass(1,f_x)) : ro.cross2;
    effect_path = input_section : (low_speaker,head_speakers) : ro.cross2 : (+,+) : par(i,2,*(sqrt(mix*1.414)));
    dry_sig = input_section : (_,!,_,!) : + <: si.bus(2) : par(i,2,*(sqrt(1-mix)));

    stereo_field_control(g_m,g_s,l,r) = l_out,r_out with {
        mid = g_m*(l+r);
        side = g_s*(l-r);
        l_out = 0.5*(mid+side);
        r_out = 0.5*(mid-side);
    };

    low_speaker = speaker(0,diameter,shaft_len) : *(0.707) <:si.bus(2);
    head_speakers = (*(0.707),*(0.707)) <: (rotative_speaker(f_1,stop_state,inertia,diameter,shaft_len),
                                           (rotative_speaker(f_2,stop_state,inertia,diameter,shaft_len):*(0.707)<:_,_),
                                            rotative_speaker(f_3,stop_state,inertia,diameter,shaft_len))
                                         : (+,+) : stereo_field_control(center_gain,side_gain);

};

process = rotary_cabinet;