import("stdfaust.lib");

speaker_power = 100; //V
speaker_output_impedance = 8; //Ohm
V_speaker_input = sqrt(speaker_power/speaker_output_impedance); //V

aa_half_tanh(x) = aa.ADAA1(EPS,f,F1,0.005*x)*500 with {
    // Using the polynomial approx often cited (closer to atan than tanh)
    neg_f(x_f) = x_f * (27.0 + x_f*x_f) / (27.0 + 9.0 * x_f*x_f); // From Fig 9 in paper
    neg_F1(x_F1) = (x_F1*x_F1)/18 + 4/3*log(x_F1*x_F1+3);

    f(x_f) = ba.if(x_f<0,neg_f(0.33*x_f),x_f);
    F1(x_F1) = ba.if(x_F1<0,neg_F1(0.33*x_F1),0.5*x_F1*x_F1);

    EPS = 1/ma.SR;
};

// --- Speaker Nonlinearity Model Parameters (Yeh et al. DAFx-08) ---
fs = ma.SR;
speaker_fo = 71.2;         // Speaker resonance frequency (Hz)
speaker_qts = 0.64;        // Speaker total Q factor
speaker_k = 0.000136;      // Displacement sensitivity (m/V) - check scaling!

// --- Derived Speaker Parameters ---
wo = 2.0 * ma.PI * speaker_fo;
vo = wo / fs; // Discrete resonance freq (radians/sample)

// --- Hx(z) Filter Coefficients ---
hx_den_a0 = 1.0 + vo/speaker_qts + vo*vo;
hx_den_a1 = -vo/speaker_qts - 2.0*vo*vo;
hx_den_a2 = vo*vo;
// Assuming numerator represents DC gain K
hx_num_b0 = speaker_k;
hx_num_b1 = 0.0;
hx_num_b2 = 0.0;

// --- Hx(z)^-1 Filter Coefficients ---
h_inv_b0 = (1.0/speaker_k) * hx_den_a0; // (1/K)*(1 + vo/QTS + vo^2)
h_inv_b1 = (1.0/speaker_k) * hx_den_a1; // (1/K)*(-vo/QTS - 2*vo^2)
h_inv_b2 = (1.0/speaker_k) * hx_den_a2; // (1/K)*(vo^2)

// --- Filter Functions ---
voltage_to_displacement(e) = e : fi.tf2(hx_num_b0, hx_num_b1, hx_num_b2, hx_den_a1, hx_den_a2) : *(hx_den_a0);

polynomial_nonlinearity(x) = waveshaper with {
    waveshaper = aa_half_tanh(x); //Magic number gotten from curve of tanh(gain*x)
};

displacement_to_voltage(x) = x : fi.fir((h_inv_b0, h_inv_b1, h_inv_b2));

// --- Complete Nonlinear Speaker Model ---
nonlinear_speaker(signal_in) = signal_in : voltage_to_displacement : polynomial_nonlinearity : displacement_to_voltage ;

// --- Example Usage in Amp Chain ---
// Assuming power_amp_output is the signal from your transformer model
process =  nonlinear_speaker;