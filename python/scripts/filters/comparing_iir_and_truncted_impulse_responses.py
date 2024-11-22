import numpy as np
import scipy.signal as signal
import soundfile as sf 
import matplotlib.pyplot as plt
import audio_dspy as adsp
from typing import List

impulse, sample_rate = sf.read('projects/bela.PAM/ressources/impulses_responses/final_IR_1024.wav')

# lambda_ = 0.15
lambda_ = 0.75
iir_order = 96
fir_order = 128

def add_missing_complex_conjugate(array: np.ndarray):
    list = array.tolist()
    for cnum in list:
        if not np.conj(cnum) in list:
            list.insert(list.index(cnum)+1,np.conj(cnum))
    return np.array(list)

def crossover(order:int, fc: float, fs: float = 48000, output: str = 'sos'):
    if order % 2 != 0: return
    low_pass = []
    high_pass = []
    for n in range(2):
        new_lowpass = signal.butter(order//2,fc,'lowpass',fs=fs, output=output)[0].tolist()
        low_pass.append(new_lowpass)
        high_pass.append(signal.butter(order//2,fc,'highpass',fs=fs, output=output)[0].tolist())
    return low_pass,high_pass

def iir_to_fir(order,lambda_,impulse):
    # Prony's method to approximate the impulse response
    # Adjust `a_order` and `b_order` as needed
    a_order = order//2  # Denominator polynomial order (related to the number of biquad sections)
    b_order = order//2 # Numerator polynomial order

    h_warped = adsp.allpass_warp(lambda_,impulse)
    bwp, awp = adsp.prony(h_warped, b_order, a_order)
    zwp = np.roots(bwp)
    pwp = np.roots(awp)
    z = (zwp+lambda_)/(1+lambda_*zwp)
    p = (pwp+lambda_)/(1+lambda_*pwp)
    # b = adsp.allpass_warp_roots(-lambda_,b)
    # a = adsp.allpass_warp_roots(-lambda_,a)
    k=0.5
    # Convert the filter into a series of second-order sections for stability
    return z,p,k#signal.zpk2sos(z,p,k)

impulse_for_fir = impulse#np.concatenate([impulse[:fir_order],np.zeros(len(impulse)-fir_order)])
impulse_for_iir = impulse#np.concatenate([np.zeros(fir_order),impulse[fir_order:]])
zpk_prony = iir_to_fir(iir_order,lambda_,impulse_for_iir)
num_zp = fir_order//2
index = num_zp//2

sos_lowpass, sos_highpass = crossover(8,2000,fs=sample_rate)

prony = signal.zpk2sos(zpk_prony[0],zpk_prony[1],zpk_prony[2])

# iir_hybrid = signal.zpk2sos(zpk_hybrid[0],zpk_hybrid[1],zpk_hybrid[2])

# Calculate the frequency response of the original impulse response
w_h, h_freq = signal.freqz(impulse, fs=sample_rate, worN=8192)
w_h, h_freq_trunc = signal.freqz(impulse[:fir_order], fs=sample_rate, worN=8192)

# Calculate the frequency response of the IIR filter
w_iir, h_prony = signal.sosfreqz(prony, fs=sample_rate, worN=8192)

h_freq = h_freq/max(h_freq)
h_freq_trunc = h_freq_trunc*0.707/max(h_freq_trunc)
h_prony = h_prony/max(h_prony)

h_db_freq_trunc_error = np.abs(20 * np.log10(abs(h_freq_trunc)/h_freq))
h_db_prony_error = np.abs(20 * np.log10(abs(h_prony)/h_freq))

cumulated_error1 = np.std(h_db_freq_trunc_error)
cumulated_error2 = np.std(h_db_prony_error)
print(cumulated_error1, cumulated_error2)

dirac = np.zeros(1024)
dirac[0] = 1
prony_impulse = signal.sosfilt(prony,dirac)
w_hybrid, h_hybrid = signal.freqz(impulse_for_fir+prony_impulse, fs=sample_rate, worN=8192)
h_hybrid = h_hybrid/max(h_hybrid)
# sf.write("prony_impulse.wav",prony_impulse,sample_rate)

# Plot both frequency responses on the same plot
fig, axs = plt.subplots()
# axs[0].figure(figsize=(10, 6))
axs.semilogx(w_h, 20 * np.log10(abs(h_freq)), label='Original Impulse Response')
axs.semilogx(w_h, 20 * np.log10(abs(h_freq_trunc)), label='Trunc Impulse Response')
axs.semilogx(w_h, 20 * np.log10(abs(h_hybrid)), label='IIR Filter (Prony)', linestyle='--')
axs.set_title('Frequency Response Comparison')
axs.set_xlim(80,20000)
axs.set_xlabel('Frequency')
axs.set_ylabel('Magnitude (dB)')
axs.grid(True)
axs.legend()

plt.show()
