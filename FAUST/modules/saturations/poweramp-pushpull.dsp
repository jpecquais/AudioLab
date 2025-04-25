import("stdfaust.lib");
import("../../lib/filters.lib");

///////////////
//DSP OBJECTS
//////////////

aa_tanh(x) = aa.ADAA1(EPS,f,F1,x) with {
    // Using the polynomial approx often cited (closer to atan than tanh)
    f(x_f) = x_f * (27.0 + x_f*x_f) / (27.0 + 9.0 * x_f*x_f); // From Fig 9 in paper
    F1(x_F1) = (x_F1*x_F1)/18 + 4/3*log(x_F1*x_F1+3);
    EPS = 1/ma.SR;
};

transformer_waveshaper = aa_tanh;

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

waveshaper(x) = x : fi.tf1(alpha,1,alpha) with {
        alpha = x : aa.hyperbolic;
};

///////////////////
// CONCRET OBJECTS
///////////////////

phase_splitter(x) = (x, -x);

powertube(bias,x) = +(x,dynamic_bias) : waveshaper : sag with {
    threshold = 0.01;

    bias_shif_amount = 0.2;
    env = x : peak_follower(0.01,0.01); // between 0.002 and 0.02 s
    dynamic_bias = bias*(1 - env*bias_shif_amount);

    sag(x) = x*sag_factor with {
        env = x : peak_follower(0.1,0.1); // between 0.02 and 0.2 s
        sag_amount = 0.2;
        sag_factor = 1-env*sag_amount;
    };
};

transformer(inphase,outphase) = inphase - outphase : fi.dcblocker : fi.resonlp(6000, 0.707, 1.0) : transformer_waveshaper;

push_pull(bias,x) = x : phase_splitter : (powertube(bias),powertube(bias)) : transformer;

negative_feedback_loop(x,presence) = x : *(-0.1) : fi.peak_eq_cq(presence,5000,0.5);

/////////////////////
// AMP FUNCTION
/////////////////////

amp_power = 100; //V
amp_output_impedance = 8; //Ohm

power_amp(master,bias,presence) = input_scaling : (+ : push_pull(bias))~(negative_feedback_loop(presence)) : output_scaling with {
    push_pull_output_impedance = 4000; //Ohm
    impedance_ratio = sqrt(push_pull_output_impedance/amp_output_impedance);

    V_peak_amp_output = sqrt(amp_power*amp_output_impedance); //V
    V_peak_power_tube = V_peak_amp_output*impedance_ratio; //V
    V_peak_input = 20; //V - this is an order of magnetude

    Normalized_V_peak_input = V_peak_input / V_peak_power_tube;

    scale_factor = Normalized_V_peak_input;
    input_scaling = *(scale_factor);

    output_scaling = *(master*V_peak_amp_output/V_peak_amp_output);
};

process = power_amp(master,bias,presence) with{
    bias = hslider("bias",0.2,0,1,0.01) : si.smoo;
    presence = hslider("presence",1,0.5,2,0.01) : si.smoo;
    master = hslider("master",0,-20,20,0.01) : ba.db2linear : si.smoo;
};
