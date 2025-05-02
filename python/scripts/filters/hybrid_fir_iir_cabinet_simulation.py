import numpy as np
import scipy.signal as signal
import soundfile as sf
import matplotlib.pyplot as plt
import audio_dspy as adsp # Assuming this library is installed and works as expected
from typing import List

# This function seems unnecessary if adsp.prony handles complex pairs correctly,
# and np.roots on real polynomials should yield conjugate pairs.
# Keep it if you are sure you need it, otherwise remove.
# def add_missing_complex_conjugate(array: np.ndarray):
#     list_out = array.tolist() # Create a copy to modify
#     seen = set(np.round(array, decimals=10)) # Use rounded values for comparison robustness
#     for cnum in array:
#         conj_cnum = np.conj(cnum)
#         # Check if conjugate is approximately present
#         is_present = False
#         for x in seen:
#             if np.isclose(x, conj_cnum):
#                 is_present = True
#                 break
#         if not is_present and not np.isreal(cnum):
#             list_out.append(conj_cnum)
#             seen.add(np.round(conj_cnum, decimals=10))
#     return np.array(list_out)

# This function is not used in the main script
# def crossover(order:int, fc: float, fs: float = 48000, output: str = 'sos'):
#     # ... (implementation) ...
#     pass

# --- Prony Function ---
# Let's refine this, especially the gain calculation
def impulse_to_zpk(impulse: np.ndarray, order: int, lambda_: float = 0.0):
    """
    Uses Prony's method (with optional warping) to find ZPK representation.
    Calculates gain k.
    """
    if order <= 0:
        return np.array([]), np.array([]), 1.0 # Return empty poles/zeros and gain 1 if order is 0

    # Ensure order is reasonable
    if order * 2 > len(impulse) - 1:
        print(f"Warning: Prony order ({order}) is high relative to impulse length ({len(impulse)}). Reducing order.")
        order = (len(impulse) - 1) // 2
        if order <= 0:
             return np.array([]), np.array([]), 1.0

    a_order = order
    b_order = order

    # Apply warping if lambda_ is specified
    if lambda_ != 0.0:
        h_proc = adsp.allpass_warp(lambda_, impulse)
    else:
        h_proc = impulse

    # Use Prony from audio_dspy
    # Ensure it handles potential errors (e.g., if impulse is too short or zero)
    try:
        b_coeffs, a_coeffs = adsp.prony(h_proc, b_order, a_order)
    except Exception as e:
        print(f"Error during adsp.prony: {e}")
        print(f"Impulse length: {len(h_proc)}, b_order: {b_order}, a_order: {a_order}")
        # Handle error: return a simple gain filter or raise exception
        return np.array([]), np.array([]), np.mean(impulse) if len(impulse)>0 else 1.0


    # Get poles and zeros from the *warped* coefficients
    z_w = np.roots(b_coeffs)
    p_w = np.roots(a_coeffs)

    # Stabilize poles (move poles outside unit circle strictly inside)
    p_w[np.abs(p_w) > 1.0] = 1.0 / np.conj(p_w[np.abs(p_w) > 1.0])

    # Unwarp poles and zeros if warping was applied
    if lambda_ != 0.0:
        z = (z_w + lambda_) / (1 + lambda_ * z_w)
        p = (p_w + lambda_) / (1 + lambda_ * p_w)
    else:
        z = z_w
        p = p_w

    # --- Calculate Gain (k) ---
    # The gain `k` should scale the ZPK representation so its response
    # matches the original impulse's magnitude.
    # One way is to match the first sample (or initial energy).
    # Let's try matching the first non-zero sample's value.

    # Create a temporary system with gain=1 to find its initial response
    # Need to handle potential pole/zero cancellations or issues if roots are identical
    try:
        # Use zpk2tf for potentially better stability in gain calculation?
        b_temp, a_temp = signal.zpk2tf(z, p, 1.0)
        # Get the impulse response of the ZPK system with gain=1
        imp_temp = signal.lfilter(b_temp, a_temp, np.concatenate(([1.0], np.zeros(len(impulse)-1))))

        # Find the first significant sample in both original and temp impulse
        # (Handles potential leading zeros)
        first_orig_idx = np.argmax(np.abs(impulse) > 1e-9) # Threshold against noise
        first_temp_idx = np.argmax(np.abs(imp_temp) > 1e-9)

        if first_orig_idx < len(impulse) and first_temp_idx < len(imp_temp) and \
           imp_temp[first_temp_idx] != 0:
            # Align based on first significant sample
            k = impulse[first_orig_idx] / imp_temp[first_temp_idx]
        elif b_coeffs[0] != 0 and a_coeffs[0] != 0:
             # Fallback: use ratio of leading coefficients (less robust after unwarping/stabilizing)
             # This assumes prony returns b[0],a[0] scaled appropriately for the *warped* domain
             # Scaling might change after unwarping and stabilization
             k = b_coeffs[0] / a_coeffs[0]
             # Try simple energy matching as another fallback
             # k = np.sqrt(np.sum(impulse**2) / np.sum(imp_temp**2))

        else:
             k = 1.0 # Default if calculation fails
    except Exception as e:
        print(f"Error during gain calculation: {e}")
        k = 1.0 # Default gain if calculation fails

    # Ensure poles and zeros are complex conjugate pairs for sos conversion
    # It's safer to rebuild polynomials from roots and then use tf2sos
    # z = add_missing_complex_conjugate(z) # Use your function if needed
    # p = add_missing_complex_conjugate(p)

    # Check pole stability again *after* unwarping
    p[np.abs(p) >= 1.0] = 0.99 * p[np.abs(p) >= 1.0] / np.abs(p[np.abs(p) >= 1.0]) # Nudge inside


    return z, p, k

# --- Main Script ---

impulse, sample_rate = sf.read('projects/bela.PAM/ressources/impulses_responses/final_IR_1024.wav')
# Ensure impulse is 1D
if impulse.ndim > 1:
    impulse = impulse[:, 0] # Take first channel if stereo


# Constant declaration
impulse_length = len(impulse)
split_point = 8 # Where the FIR part ends and IIR modeling begins
fade_length = 4 # Length of the crossfade region

if split_point + fade_length // 2 > impulse_length:
    print("Warning: Impulse too short for chosen split/fade lengths.")
    # Adjust lengths if necessary
    fade_length = max(0, (impulse_length - split_point) * 2 - 2)
    if fade_length % 2 != 0: fade_length -=1 # Ensure even fade length
    if fade_length < 0: fade_length = 0

if split_point - fade_length // 2 < 0:
     print("Warning: Fade region extends before start.")
     fade_length = split_point * 2
     if fade_length % 2 != 0: fade_length -=1


print(f"Impulse Length: {impulse_length}")
print(f"Split Point: {split_point}")
print(f"Fade Length: {fade_length}")


# --- Splitting IR with Proper Crossfade ---
# Create windows
fade_out_win = np.ones(impulse_length)
fade_in_win = np.zeros(impulse_length)

# Define transition zone indices
start_fade = max(0, split_point - fade_length // 2)
end_fade = min(impulse_length, split_point + fade_length // 2)
actual_fade_len = end_fade - start_fade

if actual_fade_len > 0 and actual_fade_len % 2 != 0:
    # Ensure even length for Hann window generation if possible
    if end_fade < impulse_length:
        end_fade += 1
    elif start_fade > 0:
        start_fade -=1
    actual_fade_len = end_fade - start_fade


if actual_fade_len > 1 : # Need at least 2 points for a Hann window
    # Use Hann window for smooth crossfade
    # We need sqrt(hann) for power complementary crossfade
    hann_win_segment = np.sin(np.pi * np.arange(actual_fade_len) / (actual_fade_len -1))**2 #signal.hann(actual_fade_len) # Standard Hann is power complementary

    # Apply fade-out to the FIR part's window
    fade_out_win[start_fade:end_fade] = np.cos(0.5 * np.pi * np.arange(actual_fade_len) / (actual_fade_len -1))**2 #hann_win_segment[::-1] # Decreasing part
    fade_out_win[end_fade:] = 0.0 # Zero after fade

    # Apply fade-in to the IIR part's window
    fade_in_win[start_fade:end_fade] = np.sin(0.5 * np.pi * np.arange(actual_fade_len) / (actual_fade_len -1))**2 #hann_win_segment # Increasing part
    fade_in_win[:start_fade] = 0.0 # Zero before fade
else:
    # Handle edge case: no real fade possible, make it an abrupt cut
    print("Warning: Fade length too small, using abrupt cut.")
    fade_out_win[split_point:] = 0.0
    fade_in_win[split_point:] = 1.0
    fade_in_win[:split_point] = 0.0


# Apply windows to get the two parts
impulse_fir = impulse * fade_out_win
impulse_iir_target = impulse * fade_in_win

# --- IIR Modeling ---
# The IIR part should model the impulse_iir_target
# It's best to model starting from the first non-zero sample of the target
iir_model_start_idx = np.argmax(np.abs(impulse_iir_target) > 0) # Find first sample
iir_input_signal = impulse_iir_target[iir_model_start_idx:]

# Ensure the input signal is long enough for the desired order
iir_order = 64 # Reduced default order, cab IR tails might be simple
lambda_ = 0.85 # Common warping value for audio

if len(iir_input_signal) < 2 * iir_order + 1:
    print(f"Warning: Target IIR impulse too short ({len(iir_input_signal)}) for order {iir_order}. Reducing order.")
    iir_order = max(1, (len(iir_input_signal) - 1) // 2)

print(f"Using IIR Order: {iir_order}")

z, p, k = impulse_to_zpk(iir_input_signal, iir_order, lambda_)

# Convert ZPK to SOS (Second-Order Sections) for filtering
# Handle cases where z, p, or k might be problematic
if len(p) > 0 or len(z) > 0 : # Make sure we have poles or zeros
    try:
        # Use zpk2sos for numerical stability
        prony_sos = signal.zpk2sos(z, p, k, pairing='nearest')
    except ValueError as e:
        print(f"Error during zpk2sos conversion: {e}")
        print(f"z={z}\np={p}\nk={k}")
        # Handle error - e.g., create a dummy gain filter
        prony_sos = np.array([[np.mean(iir_input_signal) if len(iir_input_signal)>0 else 1.0, 0, 0, 1, 0, 0]]) # Simple gain sos
else:
     # Handle case with no poles/zeros (should just be gain k)
     prony_sos = np.array([[k, 0, 0, 1, 0, 0]])


# --- Frequency Response Calculation ---
worN = 8192 # Frequency points for analysis
w, h_original = signal.freqz(impulse, fs=sample_rate, worN=worN)

# Freq response of the FIR part (windowed head)
w, h_fir = signal.freqz(impulse_fir, fs=sample_rate, worN=worN)

# Freq response of the IIR part (Prony model output)
# We need to account for the delay introduced by starting the IIR model later
# The IIR filter models `iir_input_signal` which starts at `iir_model_start_idx`
w_iir, h_iir_model = signal.sosfreqz(prony_sos, fs=sample_rate, worN=worN)

# Add the delay phase shift to the IIR model's response
delay_samples = iir_model_start_idx
phase_shift = np.exp(-1j * w * delay_samples / sample_rate)
h_iir_delayed = h_iir_model * phase_shift


# --- Combine FIR and IIR responses ---
h_hybrid = h_fir + h_iir_delayed*20

# --- Plotting ---
plt.figure(figsize=(12, 8))

# Magnitude Response
plt.subplot(2, 1, 1)
plt.semilogx(w, 20 * np.log10(np.abs(h_original) + 1e-9), label='Original IR')
plt.semilogx(w, 20 * np.log10(np.abs(h_hybrid) + 1e-9), label='Hybrid (FIR + IIR)', linestyle='--')
# plt.semilogx(w, 20 * np.log10(np.abs(h_fir) + 1e-9), label='FIR Part', linestyle=':')
# plt.semilogx(w, 20 * np.log10(np.abs(h_iir_delayed) + 1e-9), label='IIR Part (Delayed)', linestyle='-.')
plt.title('Frequency Response Comparison')
plt.ylabel('Magnitude (dB)')
plt.ylim(bottom = np.max(20 * np.log10(np.abs(h_original) + 1e-9))-100) # Adjust ylim dynamically
plt.grid(True, which='both')
plt.legend()
plt.xlim(20, sample_rate / 2)

# Error Plot
plt.subplot(2, 1, 2)
error_db = 20 * np.log10(np.abs(h_original - h_hybrid) + 1e-9)
plt.semilogx(w, error_db, label='Error (Original - Hybrid)')
plt.title('Error Spectrum')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Error Magnitude (dB)')
plt.grid(True, which='both')
plt.legend()
plt.xlim(20, sample_rate / 2)
plt.ylim(bottom = np.max(error_db)-80) # Adjust ylim


plt.tight_layout()
plt.show()

# Optional: Plot time-domain signals for debugging
plt.figure(figsize=(12, 6))
plt.plot(np.arange(impulse_length)/sample_rate, impulse, label='Original IR', alpha=0.5)
plt.plot(np.arange(len(impulse_fir))/sample_rate, impulse_fir, label='FIR Part (Windowed)', linestyle='--')
# Generate IIR impulse response
dirac = np.zeros(impulse_length)
dirac[0] = 1.0
iir_impulse_resp_model = signal.sosfilt(prony_sos, dirac)
# Add delay manually for plotting
iir_impulse_plot = np.zeros_like(impulse)
len_to_copy = min(len(iir_impulse_resp_model), impulse_length - iir_model_start_idx)
if len_to_copy > 0:
    iir_impulse_plot[iir_model_start_idx : iir_model_start_idx + len_to_copy] = iir_impulse_resp_model[:len_to_copy]

plt.plot(np.arange(impulse_length)/sample_rate, iir_impulse_plot, label='IIR Impulse (Delayed)', linestyle=':')
plt.title('Time Domain Signals')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend()
plt.grid(True)
plt.show()

# Optional: Compare IIR target vs IIR model frequency response
plt.figure(figsize=(12, 6))
w_tgt, h_iir_target_freq = signal.freqz(impulse_iir_target, fs=sample_rate, worN=worN)
w_mod, h_iir_model_freq_no_delay = signal.sosfreqz(prony_sos, fs=sample_rate, worN=worN)
h_iir_model_freq_no_delay = h_iir_model_freq_no_delay*20

plt.semilogx(w_tgt, 20*np.log10(np.abs(h_iir_target_freq)+1e-9), label='Target IIR Tail (Freq)')
plt.semilogx(w_mod, 20*np.log10(np.abs(h_iir_model_freq_no_delay)+1e-9), label='Prony IIR Model (Freq)', linestyle='--')
plt.title('IIR Target vs. Prony Model Frequency Response')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude (dB)')
plt.grid(True, which='both')
plt.legend()
plt.xlim(20, sample_rate / 2)
plt.show()