declare author "Jean-Loup Pecquais";
declare version "1.00";

import("../../lib/filters.lib");

/*
TODO:
Optimization
1. use ma.copysign when possible
2. do only ONE hilbert transform at the beginning of the process and carry the real and imag part inside functions.
3. precompute gain compensation
*/

// copysign(x,y) = pow(-1,y>0)*x; //because ma.copysign does not work in jsfx!!!
copysign(x,y) = sign_y*abs(x) with {
    sign_y = y/abs(y+ma.EPSILON);
}; //because ma.copysign does not work in jsfx!!!

MAX_DELAY_LEN = 1000; //smpls
C_SOUND = 340;

inch_to_meter(inch) = inch : *(0.0254);
inch_to_samples(inch) = inch : *(0.0254/340)*ma.SR : int;
cm_to_samples(cm) = cm : *(0.01/340)*ma.SR : int;

speaker(theta,diameter,shaft_len) = directivity_model : diffraction_model : doppler_effect : phase_model with {
    theta_to_useful(x) = cos(x)*0.5+0.5;
    directivity_model = fi.highshelf(3,gain1,fc_highshelf) : fi.highshelf(3,gain1,fc_highshelf/2) with {

        //Can be precomputed because it is extremely expensive
        gain_attenuation = -20;
        gain_att_lin = ba.db2linear(gain_attenuation);
        gain_compensation = ba.linear2db( 1/(sqrt(gain_att_lin + (1-gain_att_lin)*(3/8))) );
        //

        gain1 = gain_attenuation*(theta_to_useful(theta*2))+gain_compensation;
        gain2 = gain_attenuation*(sqrt(theta_to_useful(theta)))+gain_compensation;
        fc_highshelf = C_SOUND/(2*inch_to_meter(diameter)*sin(ma.PI/4));
    };
    diffraction_model = de.fdelay1(MAX_DELAY_LEN,dist) with {
        dist = diameter*(theta_to_useful(2*theta));
    };
    doppler_effect = de.fdelay1(MAX_DELAY_LEN,dist) with {
        dist = 2*cm_to_samples(shaft_len)*(theta_to_useful(theta));
    };
    phase_model = hilbert : real_path, imag_path : + with {
        scalable(val,func,scale) = copysign(pow(abs(computed_val),scale),computed_val) with {
            computed_val = func(val);
        };
        real_path = *(scalable(theta,cos,0.75));
        imag_path = *(scalable(theta,sin,0.75));
    };
};

process = _ : crossover : ((low_speaker),(head_speakers)) : ro.cross2 : (+,+) with {
    f_x = 800;
    crossover = _ <: fi.lowpass(1,f_x),fi.highpass(1,f_x);
    low_speaker = 0.707*speaker(0,diameter,shaft_len)<:si.bus(2);
    head_speakers = *(0.707) <: speaker(lfo_l,diameter,shaft_len),(0.707*speaker(lfo_c,diameter,shaft_len)<:_,_),speaker(lfo_r,diameter,shaft_len) : (+,+);

    diameter = 4; //inch
    shaft_len = 10; //cm
    freq = hslider("freq",1,0.1,10,0.1);
    lfo_l = os.lf_sawpos_phase(freq,0)*2*ma.PI;
    lfo_r = os.lf_sawpos_phase((freq+0.17),0.25)*2*ma.PI;
    lfo_c = os.lf_sawpos_phase((freq-0.11),0.65)*2*ma.PI;
};