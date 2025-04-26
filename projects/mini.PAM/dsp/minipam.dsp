import("stdfaust.lib");

global_amp_power = 100; //W
global_amp_output_impedance = 8; //Ohm
global_speaker_power = 200; //W
global_speaker_impedance = 8; //Ohm

power_amp = component("../../../faust/modules/saturations/poweramp-pushpull.dsp")[
    amp_power = global_amp_power;
    amp_output_impedance = global_amp_output_impedance;
];

non_linear_speaker = component("../../../faust/modules/saturations/non-linear-speaker.dsp")[
    speaker_power = global_speaker_power;
    speaker_impedance = global_speaker_impedance;
];

speaker = component("iir-speaker.dsp");

process = *(preamp_scalling) : power_amp : pre_speaker_scaling : non_linear_speaker : speaker with {
    V_peak_daisy = 1.2; //V
    preamp_scalling = V_peak_daisy*preamp_gain;
    preamp_gain = hslider("preamp gain",0,0,40,0.01) : ba.db2linear : si.smoo;

    V_peak_power_amp = sqrt(global_amp_output_impedance*global_amp_power);
    V_peak_speaker = sqrt(global_speaker_impedance*global_speaker_power);
    pre_speaker_scaling = *(V_peak_power_amp/V_peak_speaker);
};