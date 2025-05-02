import numpy as np
import scipy.signal as signal
import soundfile as sf
import matplotlib.pyplot as plt
import audio_dspy as adsp
import pathlib
import argparse

# --- Core Functions ---

def iir_to_fir_zpk(order, lambda_, impulse):
    """
    Calculates IIR filter Zeros (z) and Poles (p) using Prony's method
    with frequency warping. Does NOT determine the final gain k.
    """
    a_order = order
    b_order = order
    h_warped = adsp.allpass_warp(lambda_, impulse)
    bwp, awp = adsp.prony(h_warped, b_order, a_order)
    zwp = np.roots(bwp)
    pwp = np.roots(awp)
    z = (zwp + lambda_) / (1 + lambda_ * zwp)
    p = (pwp + lambda_) / (1 + lambda_ * pwp)
    # Return only z and p. Gain will be calculated separately.
    return z, p

def calculate_energy_matching_gain(z, p, original_impulse, simulation_length=None):
    """
    Calculates the gain 'k' required for the ZPK filter (z, p, k)
    to have the same energy as the original_impulse.
    """
    # 1. Calculate original energy
    energy_original = np.sum(original_impulse**2)
    if energy_original < 1e-18: # Handle near-silent impulse
        print("Warning: Original impulse energy is close to zero.")
        return 0.0

    # 2. Generate normalized SOS (k=1)
    try:
        sos_norm = signal.zpk2sos(z, p, 1.0, pairing='nearest')
    except Exception as e:
        print(f"Error creating normalized SOS: {e}")
        # Fallback: maybe return 1.0 or re-raise? Let's return 1.0 as a guess.
        return 1.0

    # 3. Generate impulse response of normalized filter
    # Use a simulation length at least as long as the original IR,
    # potentially longer if poles are very close to the unit circle.
    if simulation_length is None:
        simulation_length = len(original_impulse) * 2 # Heuristic: simulate for twice the IR length
        # Ensure a minimum length for stability calculation
        simulation_length = max(simulation_length, 2048)

    delta_input = np.zeros(simulation_length)
    delta_input[0] = 1.0
    try:
        ir_norm = signal.sosfilt(sos_norm, delta_input)
    except Exception as e:
         print(f"Error simulating normalized filter response: {e}")
         # Fallback gain
         return 1.0

    # 4. Calculate energy of normalized IIR impulse response
    energy_norm = np.sum(ir_norm**2)
    if energy_norm < 1e-18:
        print("Warning: Normalized Prony filter energy is close to zero.")
        # This could happen if the filter is unstable and blows up then wraps,
        # or if the filter is effectively zero.
        # If original energy wasn't zero, this indicates a problem.
        # Returning 0.0 might be safest.
        return 0.0

    # 5. Calculate the energy-matching gain
    k = np.sqrt(energy_original / energy_norm)

    print(f"Original Energy: {energy_original:.4g}, Normalized IIR Energy: {energy_norm:.4g}, Calculated Gain k: {k:.4g}")

    return k

def sos_to_faust_tf22t(sos_array, order=None, lambda_val=None):
    """
    Converts a SciPy SOS array to a Faust DSP script using cascaded tf22t biquads.
    (Added order and lambda_val parameters for comments in Faust code)
    """
    if not isinstance(sos_array, np.ndarray):
        try:
            sos_array = np.array(sos_array, dtype=float)
        except Exception as e:
            raise ValueError(f"Input could not be converted to a NumPy array: {e}")

    if sos_array.ndim != 2 or sos_array.shape[1] != 6:
        raise ValueError(f"Input sos_array must be a 2D array with 6 columns, got shape {sos_array.shape}")

    n_sections = sos_array.shape[0]

    if n_sections == 0:
        faust_code = f'''
import("stdfaust.lib");
process = _; // Pass-through for empty filter
'''
        return faust_code.strip()

    biquad_strings = []
    for i, section in enumerate(sos_array):
        b0, b1, b2, a0, a1, a2 = section
        if not np.isclose(a0, 1.0):
            print(f"Warning: Section {i} has a0 = {a0:.8g} which is not close to 1. tf22t ignores a0.")
        biquad_str = f"fi.tf22t({b0:.16g}, {b1:.16g}, {b2:.16g}, {a1:.16g}, {a2:.16g})"
        biquad_strings.append(biquad_str)

    faust_filter_chain = " : ".join(biquad_strings)

    # Add comments about the generation parameters
    param_comments = "// Filter generated from Prony's method with energy matching\n"
    if order is not None:
        param_comments += f"// Original Prony Order: {order}\n"
        param_comments += f"// Effective Filter Order (approx): {n_sections * 2}\n" # Each SOS is order 2
    if lambda_val is not None:
        param_comments += f"// Warping Lambda: {lambda_val:.4g}\n"


    faust_code = f'''
import("stdfaust.lib");

{param_comments}
process = {faust_filter_chain};
'''
    return faust_code.strip()

# --- Main Execution Logic ---

def main():
    parser = argparse.ArgumentParser(
        description="Approximate an impulse response using Prony's method with energy matching and generate a Faust DSP file."
    )
    parser.add_argument("ir_path", type=pathlib.Path, help="Path to the input impulse response audio file (.wav).")
    parser.add_argument("dsp_path", type=pathlib.Path, help="Path to store the output Faust DSP file (.dsp).")
    parser.add_argument("--order", "-n", type=int, required=True, help="Order for the Prony method (determines number of poles/zeros).")
    parser.add_argument("--lambda", "-l", type=float, required=True, dest='lambda_val', help="Lambda value for frequency warping (0 to 1).")
    parser.add_argument("--plot", action='store_true', help="Show a plot comparing the original and approximated frequency responses.")
    parser.add_argument("--sim-length", type=int, default=None, help="Optional simulation length for normalized IR energy calculation (default: 2 * len(original_ir)).")


    args = parser.parse_args()

    if not args.ir_path.is_file():
        print(f"Error: Input impulse response file not found: {args.ir_path}")
        exit(1)
    if not (0 <= args.lambda_val <= 1):
         print(f"Warning: Lambda value ({args.lambda_val}) is outside the typical range [0, 1].")

    print(f"Processing Impulse Response: {args.ir_path}")
    print(f"Prony Order: {args.order}")
    print(f"Lambda: {args.lambda_val}")
    print(f"Output Faust Path: {args.dsp_path}")

    # --- Load Impulse Response ---
    try:
        impulse, sample_rate = sf.read(args.ir_path)
        print(f"Loaded IR with {len(impulse)} samples, Sample Rate: {sample_rate} Hz")
        if impulse.ndim > 1:
            print("Warning: Input IR is stereo, using only the first channel.")
            impulse = impulse[:, 0]
        impulse = impulse.astype(np.float64)
        # Normalize original impulse peak to 1.0? Often done, but let's match raw energy for now.
        # impulse = impulse / np.max(np.abs(impulse))
    except Exception as e:
        print(f"Error loading impulse response file: {e}")
        exit(1)

    # --- IIR Approximation (Zeros and Poles only first) ---
    print("Calculating Prony Zeros and Poles...")
    try:
        z, p = iir_to_fir_zpk(args.order, args.lambda_val, impulse)
    except Exception as e:
        print(f"Error during Prony ZPK calculation: {e}")
        exit(1)

    # --- Calculate Energy Matching Gain ---
    print("Calculating energy matching gain...")
    try:
        k = calculate_energy_matching_gain(z, p, impulse, simulation_length=args.sim_length)
    except Exception as e:
        print(f"Error calculating energy matching gain: {e}")
        exit(1)


    # --- Convert ZPK to Final SOS using calculated gain ---
    print("Converting ZPK to SOS with calculated gain...")
    try:
        # Ensure stability by using pairing='nearest' or similar
        prony_sos = signal.zpk2sos(z, p, k, pairing='nearest')
        print(f"Generated {prony_sos.shape[0]} second-order sections.")
    except Exception as e:
        print(f"Error during ZPK->SOS conversion: {e}")
        exit(1)

    # --- Generate and Save Faust Code ---
    print(f"Generating Faust code for {args.dsp_path}...")
    try:
        # Pass order/lambda to add comments in Faust code
        faust_code = sos_to_faust_tf22t(prony_sos,
                                       order=args.order,
                                       lambda_val=args.lambda_val)

        args.dsp_path.parent.mkdir(parents=True, exist_ok=True)
        with args.dsp_path.open('w') as file:
            file.write(faust_code)
        print(f"Faust DSP file successfully written to: {args.dsp_path}")

    except Exception as e:
        print(f"Error writing Faust file: {e}")
        exit(1)
    
    # --- Optional Plotting ---
    if args.plot:
        print("Generating frequency response plot...")
        try:
            w_h, h_freq = signal.freqz(impulse, fs=sample_rate, worN=8192)
            w_iir, h_prony = signal.sosfreqz(prony_sos, fs=sample_rate, worN=8192)

            # Also plot the impulse responses
            sim_len_plot = min(len(impulse), 2048) # Limit plotted IR length
            t = np.arange(sim_len_plot) / sample_rate
            delta_plot = np.zeros(sim_len_plot)
            delta_plot[0] = 1.0
            ir_prony_plot = signal.sosfilt(prony_sos, delta_plot)

            fig, axs = plt.subplots(2, 1, figsize=(10, 10), sharex=False) # Separate x-axis for time/freq

            # Frequency Response
            axs[0].semilogx(w_h, 20 * np.log10(np.abs(h_freq) + 1e-9), label='Original IR')
            axs[0].semilogx(w_iir, 20 * np.log10(np.abs(h_prony) + 1e-9), label=f'Prony Approx. (N={args.order}, λ={args.lambda_val}, k={k:.3f})', linestyle='--')
            axs[0].set_title('Frequency Response Comparison')
            axs[0].set_xlim(20, sample_rate / 2 * 0.95)
            # axs[0].set_ylim(bottom=np.max(20 * np.log10(np.abs(h_prony) + 1e-9)) - 80) # Dynamic bottom limit can be jumpy
            axs[0].set_xlabel('Frequency (Hz)')
            axs[0].set_ylabel('Magnitude (dB)')
            axs[0].grid(True, which='both')
            axs[0].legend()

            # Impulse Response (Initial part)
            axs[1].plot(t, impulse[:sim_len_plot], label='Original IR', alpha=0.7)
            axs[1].plot(t, ir_prony_plot, label='Prony Approx. IR', linestyle='--', alpha=0.9)
            axs[1].set_title(f'Impulse Response Comparison (First {sim_len_plot} samples)')
            axs[1].set_xlabel('Time (s)')
            axs[1].set_ylabel('Amplitude')
            axs[1].grid(True)
            axs[1].legend()

            plt.tight_layout()
            plt.show()
        except Exception as e:
            print(f"Error generating plot: {e}")

if __name__ == "__main__":
    main()