# Optimized Velvet-Noise Decorrelator algorithm from the paper "Optimized Velvet-Noise Decorrelator" (Schlecht et al., DAFx-18).

# Here's a Python implementation covering the core aspects:

# Generation of initial Exponential Velvet Noise (EVN)

# Calculation of magnitude response and objective function (RMSE of smoothed dB magnitude)

# Optimization of impulse gains (γ) and locations (τ̃)

# Rounding continuous locations τ̃ to integer locations τ

# Efficient sparse convolution

# (Optional but included) Coherence calculation and pair selection

# We'll use numpy for numerical operations and scipy.optimize for the optimization part. scipy.signal will be used for comparisons and potentially for filtering in the coherence calculation.

import numpy as np
import scipy.optimize
import scipy.signal
import matplotlib.pyplot as plt
from typing import Tuple, List, Optional

# --- Constants and Helper Functions ---

def _calculate_alpha(target_level_db: float, decay_time: int) -> float:
    """Calculates the exponential decay factor alpha (Eq. 6)."""
    # Target decay L_dB is negative, e.g., -60
    if decay_time <= 1: return 0.0
    return -np.log(10**(target_level_db / 20.0)) / (decay_time - 1) # Decay over Ls-1 samples

def _calculate_target_gain(tau_m: float, alpha: float) -> float:
    """Calculates the target exponential gain for a given delay."""
    return np.exp(-tau_m * alpha)

def calculate_magnitude_response(
    taus: np.ndarray,
    sigmas: np.ndarray,
    gammas: np.ndarray,
    freqs: np.ndarray,
    fs: int
) -> np.ndarray:
    """
    Calculates the magnitude response |H(e^jω)| at given frequencies.

    Args:
        taus: Impulse locations (continuous or discrete).
        sigmas: Impulse signs (-1 or 1).
        gammas: Impulse gains (positive).
        freqs: Frequencies (in Hz) to evaluate the response.
        fs: Sampling rate.

    Returns:
        Magnitude response at the given frequencies.
    """
    omega = 2 * np.pi * freqs / fs
    # Use complex exponentials: H(e^jω) = Σ σ[m]γ[m] * exp(-jωτ[m])
    # Correct phase for negative sigma: exp(jπ) = -1
    # H(e^jω) = Σ γ[m] * exp(-jωτ[m] + jπ * (1-σ[m])/2)
    # This is equivalent to simply multiplying by sigma:
    response = np.sum(
        sigmas[:, np.newaxis] * gammas[:, np.newaxis] *
        np.exp(-1j * omega[np.newaxis, :] * taus[:, np.newaxis]),
        axis=0
    )
    return np.abs(response)

def third_octave_smoothing(
    db_magnitude_response: np.ndarray,
    freqs: np.ndarray
) -> Tuple[np.ndarray, np.ndarray]:
    """
    Applies approximate third-octave smoothing to the dB magnitude response.

    Uses a moving average filter on the log-frequency dB response.
    The window size approximates 1/3 octave.

    Args:
        db_magnitude_response: Magnitude response in dB.
        freqs: Corresponding frequencies (logarithmically spaced).

    Returns:
        Smoothed dB magnitude response, mean smoothed response.
    """
    if len(db_magnitude_response) < 3:
        return db_magnitude_response, np.mean(db_magnitude_response)

    # Approximate 1/3 octave width in terms of log frequency indices
    # A third octave corresponds to a frequency ratio of 2^(1/3)
    # log(f2) - log(f1) = log(f2/f1) = log(2^(1/3)) = (1/3) * log(2)
    log_freqs = np.log(freqs + 1e-9) # Add epsilon for freq=0 if present
    log_freq_range = log_freqs[-1] - log_freqs[0]
    points_per_log_unit = len(log_freqs) / log_freq_range if log_freq_range > 0 else 1
    window_width_log = np.log(2) / 3.0

    # Window size in number of bins (must be odd)
    window_size = int(np.ceil(window_width_log * points_per_log_unit))
    if window_size % 2 == 0:
        window_size += 1
    window_size = max(3, window_size) # Ensure minimum size

    # Apply moving average using convolution with a flat kernel
    kernel = np.ones(window_size) / window_size
    # Use 'reflect' mode for handling boundaries, similar to how smoothing works
    smoothed_db_response = np.convolve(db_magnitude_response, kernel, mode='same')

    # Handle boundary effects more carefully (e.g., shorter windows at edges)
    # For simplicity here, we'll just use the 'same' mode result, acknowledging
    # potential edge inaccuracies which are less critical for RMSE calculation.
    # A more rigorous approach might involve band-specific averaging.

    mean_smoothed_response = np.mean(smoothed_db_response)
    return smoothed_db_response, mean_smoothed_response

def objective_function(x: np.ndarray, params: dict) -> float:
        """Calculates RMSE of smoothed dB magnitude response (Eq. 15)."""
        M = params["M"]
        mid = M - 1 # Index separating taus and gammas in x

        # Unpack variables
        tau_cont = np.concatenate(([params["tau0"]], x[:mid]))
        gamma_mult = np.concatenate(([1.0], x[mid:])) # gamma_mult[0] is fixed to 1

        # Calculate actual gammas based on multipliers and target decay
        target_gammas = _calculate_target_gain(tau_cont, params["alpha"])
        # Ensure gamma0 uses its original target (usually 1.0)
        target_gammas[0] = params["gamma0_target"]
        gammas = gamma_mult * target_gammas

        # Calculate magnitude response
        mag_response = calculate_magnitude_response(
            tau_cont, params["sigma"], gammas, params["log_freqs"], params["fs"]
        )

        # Convert to dB (avoid log(0))
        db_response = 20 * np.log10(mag_response + 1e-9)

        # Apply smoothing
        smoothed_db, mean_smoothed = third_octave_smoothing(db_response, params["log_freqs"])

        # Calculate RMSE (Eq. 15)
        rmse = np.sqrt(np.mean((smoothed_db - mean_smoothed)**2))
        return rmse

# --- Core VNS Generation and Optimization ---

def generate_evn(
    duration_sec: float,
    density_Nd: float,
    fs: int,
    decay_L_dB: float = -60.0
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, int, float]:
    """
    Generates an Exponential Velvet Noise (EVN) sequence (Eq. 1-6).

    Args:
        duration_sec: Filter duration in seconds.
        density_Nd: Impulse density (impulses per second).
        fs: Sampling rate.
        decay_L_dB: Total decay in dB (e.g., -60).

    Returns:
        tau_int: Integer impulse locations (indices).
        sigma: Impulse signs (-1 or 1).
        gamma: Exponential impulse gains (positive).
        Ls: Total filter length in samples.
        Ta: Grid size in samples.
    """
    noise_len_samples = int(np.ceil(duration_sec * fs))
    average_spacing = fs / density_Nd  # Average spacing (grid size)
    num_of_impulses = int(np.floor(duration_sec * density_Nd)) # Total number of impulses (Eq. 2 approx)
    if num_of_impulses == 0:
      print("Warning: Zero impulses requested. Check duration and density.")
      return np.array([0]), np.array([1.0]), np.array([1.0]), noise_len_samples, average_spacing

    # --- Generate Impulse Times (Eq. 4) ---
    impulses_position = np.zeros(num_of_impulses, dtype=int) # impulse_position is defined in samples.
    # r2 ~ Uniform(0, 1] - use -np.random.rand() for (0, 1) then adjust if needed
    r2 = 1.0 - np.random.rand(num_of_impulses - 1) # Ensures r2 > 0
    # Calculate non-zero impulse times
    # Add 1e-9 to avoid log(0) issues if Ta or m is zero, though m starts at 1 here.
    impulses_position[1:] = np.ceil(average_spacing * (np.arange(num_of_impulses - 1) + r2)).astype(int)

    # Ensure times are within bounds and unique - ceiling might exceed Ls
    impulses_position = np.minimum(impulses_position, noise_len_samples - 1)
    impulses_position = np.unique(impulses_position) # Keep only unique locations
    num_of_impulses = len(impulses_position) # Update M based on unique locations

    # --- Generate Signs (Eq. 3) ---
    r1 = np.random.rand(num_of_impulses)
    impulses_sign = (2 * np.round(r1) - 1).astype(float)
    # Ensure first impulse is non-zero sign if desired (paper normalizes first gain later)
    if impulses_sign[0] == 0: impulses_sign[0] = 1.0

    # --- Generate Exponential Gains (Eq. 5, 6) ---
    alpha = _calculate_alpha(decay_L_dB, noise_len_samples)
    impulses_gain = _calculate_target_gain(impulses_position.astype(float), alpha)

    return impulses_position, impulses_sign, impulses_gain, noise_len_samples, average_spacing


def optimize_vns(
    init_impulses_pos: np.ndarray,
    impulses_sign: np.ndarray,
    init_impulses_gain: np.ndarray,
    noise_len_samples: int,
    average_density: float,
    fs: int,
    decay_L_dB: float = -60.0,
    gain_deviation_factor: float = 2.0, # Corresponds to +/- 6dB (chi=2)
    freq_min: float = 20.0,
    freq_max: float = 20000.0,
    num_freq_points: int = 200,
    optimizer_options: Optional[dict] = None
) -> Tuple[np.ndarray, np.ndarray]:
    """
    Optimizes VNS impulse locations (tau) and gains (gamma) for flatness.

    Args:
        tau_init: Initial integer impulse locations.
        sigma: Impulse signs (fixed during optimization).
        gamma_init: Initial impulse gains.
        Ls: Total filter length in samples.
        Ta: Grid size (for constraints).
        fs: Sampling rate.
        decay_L_dB: Target decay for gain constraints.
        gain_deviation_factor: Allowed gain multiplier/divisor (chi).
        freq_min: Minimum frequency for evaluation.
        freq_max: Maximum frequency for evaluation (<= fs/2).
        num_freq_points: Number of log-spaced points for evaluation.
        optimizer_options: Dictionary of options for scipy.optimize.minimize.

    Returns:
        tau_opt_int: Optimized integer impulse locations.
        gamma_opt: Optimized impulse gains.
    """
    num_of_impulses = len(init_impulses_pos)
    if num_of_impulses <= 1:
        print("Warning: Cannot optimize sequence with <= 1 impulse.")
        return init_impulses_pos, init_impulses_gain

    freq_max = min(freq_max, fs / 2.0 - 1e-6) # Ensure below Nyquist
    # Logarithmically spaced frequencies for evaluation (Eq. 11)
    log_freqs = np.geomspace(freq_min, freq_max, num_freq_points) # create an array of "geometrically space" values. Each value are separated by a ratio.

    alpha = _calculate_alpha(decay_L_dB, noise_len_samples)

    # Optimization variables: Combine continuous tau and gamma
    # Exclude tau[0]=0 and gamma[0]=1 (fixed normalization)
    impulses_pos_cont_init = init_impulses_pos[1:].astype(float)
    gain_multiplier_init = np.ones(num_of_impulses-1) # Multiplier relative to target gain

    # Initial packed variables [tau_cont[1:], ..., tau_cont[M-1], gamma_mult[1:], ..., gamma_mult[M-1]]
    x0 = np.concatenate((impulses_pos_cont_init, gain_multiplier_init))

    # Fixed parameters to pass to objective function
    fixed_params = {
        "sigma": impulses_sign,
        "alpha": alpha,
        "log_freqs": log_freqs,
        "fs": fs,
        "M": num_of_impulses,
        "gain_deviation_factor": gain_deviation_factor,
        "tau0": init_impulses_pos[0], # Should be 0
        "gamma0_target": init_impulses_gain[0] # Should be 1.0 or close
    }

    

    # --- Define Bounds and Constraints (Eq. 16) ---
    bounds = []
    # Tau bounds: Ta*(m-1) < tau_cont[m] <= Ta*m (for m=1..M-1)
    # Need small epsilon because optimizer might hit exact boundary otherwise.
    eps_tau = 1e-6
    tau_lower_bounds = average_density * np.arange(num_of_impulses - 1) + eps_tau
    tau_upper_bounds = average_density * (np.arange(num_of_impulses - 1) + 1)
    # Clip bounds to valid sample range [0, Ls-1]
    tau_lower_bounds = np.maximum(0.0, tau_lower_bounds)
    tau_upper_bounds = np.minimum(noise_len_samples - 1.0, tau_upper_bounds)
    for i in range(num_of_impulses - 1):
         bounds.append((tau_lower_bounds[i], tau_upper_bounds[i]))

    # Gamma multiplier bounds: 1/chi <= gamma_mult[m] <= chi (for m=1..M-1)
    gamma_lower_bound = 1.0 / gain_deviation_factor
    gamma_upper_bound = gain_deviation_factor
    for _ in range(num_of_impulses - 1):
        bounds.append((gamma_lower_bound, gamma_upper_bound))

    # --- Run Optimization ---
    default_options = {'maxiter': 60, 'disp': True, 'ftol': 1e-6} # Like paper's limit
    if optimizer_options:
        default_options.update(optimizer_options)

    result = scipy.optimize.minimize(
        objective_function,
        x0,
        args=(fixed_params,),
        method='SLSQP', # Suitable for constrained problems
        # method='trust-constr', # Another option, often better for bounds
        bounds=bounds,
        options=default_options
    )

    if not result.success:
        print(f"Warning: Optimization did not converge: {result.message}")

    # --- Unpack and Round Results ---
    x_opt = result.x
    mid = num_of_impulses - 1
    tau_cont_opt = np.concatenate(([fixed_params["tau0"]], x_opt[:mid]))
    opt_impulses_gain = np.concatenate(([1.0], x_opt[mid:]))

    # Round taus to nearest integer (Eq. 18 implied)
    opt_impulses_pos = np.round(tau_cont_opt).astype(int)
    # Ensure unique integer locations after rounding
    opt_impulses_pos, unique_idx = np.unique(opt_impulses_pos, return_index=True)
    # Keep corresponding sigmas and multipliers
    opt_impulses_sign = impulses_sign[unique_idx]
    opt_impulses_gain = opt_impulses_gain[unique_idx]
    # Update M
    M_opt = len(opt_impulses_pos)

    # Recalculate final optimized gammas using the *integer* taus
    target_gammas_opt = _calculate_target_gain(opt_impulses_pos.astype(float), alpha)
    # Ensure gamma0 target is correct if tau[0] somehow changed (shouldn't)
    target_gammas_opt[0] = _calculate_target_gain(opt_impulses_pos[0].astype(float), alpha)
    gamma_opt = opt_impulses_gain * target_gammas_opt

    # Renormalize first gain to +/- 1 as per Fig 2 constraint description
    # Note: Paper Eq 16 says gamma(0)=1, but implies fixed sign.
    # We keep the original sign sigma[0] and normalize magnitude.
    if M_opt > 0 and gamma_opt[0] != 0:
         gamma_opt = gamma_opt / np.abs(gamma_opt[0])


    return opt_impulses_pos, opt_impulses_sign, gamma_opt


# --- Sparse Convolution ---

def sparse_convolve(
    x: np.ndarray,
    taus: np.ndarray,
    sigmas: np.ndarray,
    gammas: np.ndarray,
    Ls: int
) -> np.ndarray:
    """
    Performs efficient sparse convolution (Eq. 7 logic).

    y[n] = Σ_m sigma[m] * gamma[m] * x[n - tau[m]]

    Args:
        x: Input signal.
        taus: Impulse locations (integer indices).
        sigmas: Impulse signs.
        gammas: Impulse gains.
        Ls: Length of the FIR filter (only needed for output length).

    Returns:
        y: Output signal (convolved). Length = len(x) + Ls - 1.
    """
    len_x = len(x)
    len_y = len_x + Ls - 1
    y = np.zeros(len_y, dtype=x.dtype)
    M = len(taus)

    for m in range(M):
        tau_m = taus[m]
        val = sigmas[m] * gammas[m]
        # Add the scaled and shifted input signal
        # Ensure slicing is within bounds of y and x
        start_y = tau_m
        end_y = tau_m + len_x
        if start_y < len_y:
            y[start_y : min(end_y, len_y)] += val * x[:max(0, len_y - start_y)]

    return y

# --- Coherence Calculation and Pair Selection (Optional) ---

def get_third_octave_bands(fmin=20, fmax=20000, fs=44100, nfft=None):
    """Defines center frequencies and edges for 1/3 octave bands."""
    # Standard center frequencies based on 1kHz
    base_freq = 1000.0
    n_bands_up = np.ceil(3 * np.log2(fmax / base_freq)).astype(int)
    n_bands_down = np.ceil(3 * np.log2(base_freq / fmin)).astype(int)
    indices = np.arange(-n_bands_down, n_bands_up + 1)
    center_freqs = base_freq * (2**(indices / 3.0))

    # Filter out frequencies outside the desired range
    center_freqs = center_freqs[(center_freqs >= fmin) & (center_freqs <= fmax)]

    # Calculate band edges (geometric mean between centers)
    edges = np.sqrt(center_freqs[:-1] * center_freqs[1:])
    f_min_edge = center_freqs[0] / (2**(1/6.0))
    f_max_edge = center_freqs[-1] * (2**(1/6.0))
    band_edges = np.concatenate(([f_min_edge], edges, [f_max_edge]))
    band_edges = np.clip(band_edges, 0, fs / 2.0)

    if nfft is not None:
        # Find corresponding FFT bin indices for edges
        freq_bins = np.fft.rfftfreq(nfft, 1/fs)
        edge_bins = np.searchsorted(freq_bins, band_edges).tolist()
        return center_freqs, band_edges, edge_bins, freq_bins
    else:
        return center_freqs, band_edges

def calculate_coherence(
    seq_a: Tuple[np.ndarray, np.ndarray, np.ndarray], # tau, sigma, gamma
    seq_b: Tuple[np.ndarray, np.ndarray, np.ndarray],
    Ls: int,
    fs: int,
    nfft: int = 2048
) -> Tuple[float, np.ndarray]:
    """
    Calculates frequency mean absolute coherence (Eq. 20, 21 approximation).

    Approximates Eq. 20 by filtering the impulse responses in 1/3 octave bands
    using FFT multiplication, then computes correlation within bands.

    Args:
        seq_a, seq_b: Tuples containing (tau, sigma, gamma) for each sequence.
        Ls: Filter length.
        fs: Sampling rate.
        nfft: FFT size for spectral analysis.

    Returns:
        mean_abs_coherence: Frequency mean absolute coherence (Eq. 21).
        band_coherence: Absolute coherence for each 1/3 octave band.
    """
    tau_a, sigma_a, gamma_a = seq_a
    tau_b, sigma_b, gamma_b = seq_b

    # Get impulse responses as dense arrays
    h_a = np.zeros(Ls); h_a[tau_a] = sigma_a * gamma_a
    h_b = np.zeros(Ls); h_b[tau_b] = sigma_b * gamma_b

    # Compute FFTs
    H_a = np.fft.rfft(h_a, n=nfft)
    H_b = np.fft.rfft(h_b, n=nfft)

    # Get 1/3 octave band definitions
    center_freqs, _, edge_bins, freq_bins = get_third_octave_bands(fs=fs, nfft=nfft)
    num_bands = len(center_freqs)
    band_coherence = np.zeros(num_bands)

    # Calculate coherence per band (approximation of Eq. 20)
    for j in range(num_bands):
        start_bin = edge_bins[j]
        end_bin = edge_bins[j+1]
        if start_bin >= end_bin: continue # Skip empty bands

        # Slice FFT data for the current band
        H_a_band = H_a[start_bin:end_bin]
        H_b_band = H_b[start_bin:end_bin]

        # Calculate cross power spectral density (CPSD) and PSDs within the band
        S_ab = np.sum(H_a_band * np.conj(H_b_band))
        S_aa = np.sum(np.abs(H_a_band)**2)
        S_bb = np.sum(np.abs(H_b_band)**2)

        # Calculate coherence for the band (magnitude of complex coherence)
        if S_aa > 1e-12 and S_bb > 1e-12:
             coh_j = np.abs(S_ab) / np.sqrt(S_aa * S_bb)
             band_coherence[j] = coh_j
        else:
             band_coherence[j] = 0.0 # Or NaN, depending on desired handling

    # Calculate frequency mean absolute coherence (Eq. 21)
    mean_abs_coherence = np.mean(band_coherence)

    return mean_abs_coherence, band_coherence

def find_best_pair(
    sequences: List[Tuple[np.ndarray, np.ndarray, np.ndarray]], # List of (tau, sigma, gamma)
    Ls: int,
    fs: int,
    lambda_coherence: float = 0.8, # Weighting factor from paper (Eq. 23)
    mu_norm: float = 0.1, # Normalization factor from paper (Eq. 23)
    nfft: int = 2048,
    freq_min: float = 20.0,
    freq_max: float = 20000.0,
    num_freq_points: int = 200
) -> Tuple[int, int, float]:
    """
    Finds the best pair of sequences minimizing the combined metric (Eq. 23).

    Args:
        sequences: A list of optimized sequences (tau, sigma, gamma).
        Ls: Filter length.
        fs: Sampling rate.
        lambda_coherence: Weighting factor (0 to 1). Higher means more emphasis on flatness.
        mu_norm: Normalization factor for flatness penalty.
        nfft: FFT size for coherence calculation.
        freq_min, freq_max, num_freq_points: Params for flatness calculation.

    Returns:
        idx_a: Index of the first sequence in the best pair.
        idx_b: Index of the second sequence in the best pair.
        min_metric: The minimum combined metric value found.
    """
    num_sequences = len(sequences)
    if num_sequences < 2:
        raise ValueError("Need at least two sequences to find a pair.")

    best_pair = (-1, -1)
    min_metric = np.inf

    # Pre-calculate flatness (objective function value) for each sequence
    flatness_values = np.zeros(num_sequences)
    log_freqs = np.geomspace(freq_min, min(freq_max, fs/2-1e-6), num_freq_points)
    for i in range(num_sequences):
        tau, sigma, gamma = sequences[i]
        alpha = _calculate_alpha(-60.0, Ls) # Assume -60dB for flatness calc here
        params = { "sigma": sigma, "alpha": alpha, "log_freqs": log_freqs,
                   "fs": fs, "M": len(tau), "gain_deviation_factor": 2.0,
                   "tau0": 0.0, "gamma0_target": 1.0 }
        # Pack dummy optimization variables (not used for calculation, only structure)
        mid = len(tau) - 1
        dummy_x = np.zeros(2 * mid)
        dummy_x[:mid] = tau[1:] # Use actual taus
        target_gammas = _calculate_target_gain(tau.astype(float), alpha)
        target_gammas[0] = 1.0
        # Calculate multipliers needed (approximate, assumes optimization worked)
        multipliers = gamma / (target_gammas + 1e-9)
        dummy_x[mid:] = multipliers[1:]

        flatness_values[i] = objective_function(dummy_x, params)


    # Iterate through all unique pairs
    for i in range(num_sequences):
        for j in range(i + 1, num_sequences):
            # Calculate coherence
            mean_coh, _ = calculate_coherence(sequences[i], sequences[j], Ls, fs, nfft)

            # Calculate flatness penalty
            L_a = flatness_values[i]
            L_b = flatness_values[j]
            flatness_penalty = mu_norm * (L_a + L_b)

            # Calculate combined metric (Eq. 23)
            metric = (1.0 - lambda_coherence) * mean_coh + lambda_coherence * flatness_penalty

            if metric < min_metric:
                min_metric = metric
                best_pair = (i, j)

    return best_pair[0], best_pair[1], min_metric


# --- Example Usage ---

if __name__ == "__main__":
    # Parameters
    fs = 44100
    duration = 0.030  # 30 ms
    density = 1000    # Impulses per second (for OVN30)
    decay_db = -60.0
    gain_dev_factor = 2.0 # +/- 6dB

    # 1. Generate Initial EVN
    print("Generating initial EVN...")
    evn_pos, evn_sign, env_gain, len_samples, average_spacing = generate_evn(
        duration_sec=duration,
        density_Nd=density,
        fs=fs,
        decay_L_dB=decay_db
    )
    print(f"Generated EVN with M={len(evn_pos)} impulses, Ls={len_samples} samples.")

    # 2. Optimize to OVN
    print("\nOptimizing VNS...")
    tau_ovn, sigma_ovn, gamma_ovn = optimize_vns(
        init_impulses_pos=evn_pos,
        impulses_sign=evn_sign,
        init_impulses_gain=env_gain,
        noise_len_samples=len_samples,
        average_density=average_spacing,
        fs=fs,
        decay_L_dB=decay_db,
        gain_deviation_factor=gain_dev_factor,
        optimizer_options={'disp': False} # Quieter optimization
    )
    print(f"Optimization complete. OVN has M={len(tau_ovn)} impulses.")

    # 3. Analyze and Compare Responses
    print("\nAnalyzing frequency responses...")
    freqs = np.geomspace(20, fs / 2, 500)
    mag_evn = calculate_magnitude_response(evn_pos, evn_sign, env_gain, freqs, fs)
    mag_ovn = calculate_magnitude_response(tau_ovn, sigma_ovn, gamma_ovn, freqs, fs)

    db_evn = 20 * np.log10(mag_evn + 1e-9)
    db_ovn = 20 * np.log10(mag_ovn + 1e-9)

    smoothed_db_evn, mean_evn = third_octave_smoothing(db_evn, freqs)
    smoothed_db_ovn, mean_ovn = third_octave_smoothing(db_ovn, freqs)

    rmse_evn = np.sqrt(np.mean((smoothed_db_evn - mean_evn)**2))
    rmse_ovn = np.sqrt(np.mean((smoothed_db_ovn - mean_ovn)**2))
    print(f"Smoothed dB RMSE (Flatness): EVN={rmse_evn:.2f} dB, OVN={rmse_ovn:.2f} dB")

    # Plotting
    plt.figure(figsize=(12, 10))

    # Impulse Responses (like Fig 1)
    plt.subplot(3, 1, 1)
    markerline_evn, stemlines_evn, _ = plt.stem(evn_pos / fs * 1000, evn_sign * env_gain, linefmt='C0-', markerfmt='C0o', basefmt=' ', label=f'EVN (M={len(evn_pos)})')
    plt.setp(stemlines_evn, 'linewidth', 0.5)
    plt.setp(markerline_evn, 'markersize', 4)
    markerline_ovn, stemlines_ovn, _ = plt.stem(tau_ovn / fs * 1000, sigma_ovn * gamma_ovn, linefmt='C1--', markerfmt='C1x', basefmt=' ')
    plt.setp(stemlines_ovn, 'linewidth', 0.5)
    plt.setp(markerline_ovn, 'markersize', 5, 'color', 'C1')
    # Add optimized stems later so they are on top
    plt.stem(tau_ovn / fs * 1000, sigma_ovn * gamma_ovn, linefmt='C1--', markerfmt='C1x', basefmt=' ', label=f'OVN (M={len(tau_ovn)})')

    plt.xlabel("Time [ms]")
    plt.ylabel("Amplitude")
    plt.title("Impulse Responses (EVN vs OVN)")
    plt.legend()
    plt.grid(True, axis='y', linestyle=':', alpha=0.6)

    # Magnitude Responses (Smoothed dB)
    plt.subplot(3, 1, 2)
    plt.semilogx(freqs, smoothed_db_evn - mean_evn, label=f'EVN (Smoothed, RMSE={rmse_evn:.2f} dB)', alpha=0.8)
    plt.semilogx(freqs, smoothed_db_ovn - mean_ovn, label=f'OVN (Smoothed, RMSE={rmse_ovn:.2f} dB)', alpha=0.8)
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("Magnitude [dB re: Mean]")
    plt.title("Smoothed Magnitude Response Deviation")
    plt.legend()
    plt.grid(True, which='both', linestyle=':', alpha=0.6)
    plt.ylim(-6, 6) # Zoom in on deviations

    # Original Magnitude Responses (dB)
    plt.subplot(3, 1, 3)
    plt.semilogx(freqs, db_evn, label='EVN (Raw)', alpha=0.6, linewidth=1)
    plt.semilogx(freqs, db_ovn, label='OVN (Raw)', alpha=0.8, linewidth=1)
    plt.xlabel("Frequency [Hz]")
    plt.ylabel("Magnitude [dB]")
    plt.title("Raw Magnitude Response")
    plt.legend()
    plt.grid(True, which='both', linestyle=':', alpha=0.6)

    plt.tight_layout()
    plt.show()

    # 4. Demonstrate Sparse Convolution (Optional)
    print("\nDemonstrating sparse convolution...")
    signal = np.random.randn(fs // 10) # 0.1 seconds of white noise
    print(f"Input signal length: {len(signal)}")

    # Sparse method
    import time
    start_sparse = time.time()
    output_sparse = sparse_convolve(signal, tau_ovn, sigma_ovn, gamma_ovn, len_samples)
    time_sparse = time.time() - start_sparse

    # Dense method (for comparison)
    h_ovn_dense = np.zeros(len_samples)
    h_ovn_dense[tau_ovn] = sigma_ovn * gamma_ovn
    start_dense = time.time()
    output_dense = scipy.signal.convolve(signal, h_ovn_dense, mode='full')
    time_dense = time.time() - start_dense

    print(f"Sparse convolution time: {time_sparse:.6f} s")
    print(f"Dense convolution time: {time_dense:.6f} s")
    print(f"Outputs match: {np.allclose(output_sparse, output_dense)}")

    # 5. Find Best Pair (Optional Example)
    print("\nGenerating multiple sequences to find best pair...")
    num_seq_to_generate = 10 # Generate a few sequences
    all_sequences = []
    for i in range(num_seq_to_generate):
        print(f" Generating & Optimizing Sequence {i+1}/{num_seq_to_generate}")
        tau_i, sigma_i, gamma_i, Ls_i, Ta_i = generate_evn(duration, density, fs, decay_db)
        tau_o, sigma_o, gamma_o = optimize_vns(
            tau_i, sigma_i, gamma_i, Ls_i, Ta_i, fs, decay_db, gain_dev_factor,
            optimizer_options={'disp': False}
        )
        all_sequences.append((tau_o, sigma_o, gamma_o))

    if len(all_sequences) >= 2:
        print("\nFinding best pair based on coherence and flatness...")
        idx_a, idx_b, metric = find_best_pair(all_sequences, len_samples, fs, lambda_coherence=0.8)
        print(f"Best pair found: Sequence {idx_a} and Sequence {idx_b}")
        print(f"Combined Metric (Eq. 23): {metric:.4f}")

        mean_coh, band_coh = calculate_coherence(all_sequences[idx_a], all_sequences[idx_b], len_samples, fs)
        print(f"Mean Absolute Coherence of best pair: {mean_coh:.4f}")

        # Plot coherence of best pair
        center_freqs, _ = get_third_octave_bands(fs=fs)
        plt.figure()
        plt.semilogx(center_freqs, band_coh, 'o-')
        plt.xlabel("Frequency [Hz]")
        plt.ylabel("Absolute Coherence")
        plt.title(f"1/3 Octave Coherence (Best Pair: {idx_a} & {idx_b})")
        plt.grid(True, which='both', linestyle=':', alpha=0.6)
        plt.ylim(0, 1)
        plt.show()

# Explanation and Key Points:

# EVN Generation (generate_evn): Implements equations 1-6 to create the initial exponentially decaying velvet noise based on density, duration, and decay rate. It handles potential edge cases like zero impulses.

# Magnitude Response (calculate_magnitude_response): Efficiently calculates the frequency response using complex exponentials and numpy broadcasting.

# Third-Octave Smoothing (third_octave_smoothing): Approximates the smoothing described in the paper (Eq. 11-14) using a moving average filter on the logarithmically spaced dB magnitude response. The window size is chosen to approximate 1/3 octave. Note: This is a simplification of potentially more complex filter-bank based smoothing, but captures the essence for the RMSE calculation.

# Objective Function (objective_function): Calculates the RMSE of the smoothed dB response relative to its mean (Eq. 15), which the optimizer aims to minimize. It unpacks the optimization variables (continuous taus and gain multipliers).

# Optimization (optimize_vns):

# Sets up the optimization problem for scipy.optimize.minimize.

# Variables: Continuous locations τ̃[1:] and gain multipliers γ_mult[1:] are packed into a single vector x.

# Bounds: Constraints from Eq. 16 (time boundaries based on Ta, gain deviation based on chi) are implemented using the bounds argument.

# Method: SLSQP is chosen as it handles constraints and bounds. trust-constr is another good option.

# Post-processing: The optimized continuous locations τ̃ are rounded to the nearest integer τ. Gains γ are recalculated using these integer τ and the optimized multipliers. The first gain is normalized to magnitude 1. Uniqueness of integer taus is ensured.

# Sparse Convolution (sparse_convolve): Implements the efficient convolution described by Eq. 7, avoiding multiplications/additions with zeros. Much faster than dense convolution for sparse filters.

# Coherence (get_third_octave_bands, calculate_coherence): Calculates the frequency mean absolute coherence (Eq. 21) by approximating the per-band correlation (Eq. 20). It uses FFTs and sums power/cross-power within 1/3 octave bands. Note: This FFT-based method is an approximation of filtering the actual time-domain impulse responses.

# Pair Selection (find_best_pair): Iterates through pairs of generated OVN sequences, calculates their coherence and flatness (using the objective function value), and finds the pair minimizing the combined metric from Eq. 23.

# Example Usage (if __name__ == "__main__":): Demonstrates the workflow: generating EVN, optimizing to OVN, comparing frequency responses, showing sparse convolution speedup, and finding a good decorrelating pair.

# This implementation provides a functional version of the algorithm described in the paper. You can adjust parameters like duration, density, decay, and optimization settings to explore different results.