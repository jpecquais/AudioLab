import numpy as np
import scipy.signal as signal
import soundfile as sf 
import matplotlib.pyplot as plt
import audio_dspy as adsp
from typing import List


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
    k=0.5
    
    return z,p,k#signal.zpk2sos(z,p,k)

original_impulse, fs = sf.read('projects/bela.PAM/ressources/impulses_responses/final_IR_1024.wav')

total_iir_order = 128
num_passes = 8
iir_order = total_iir_order // num_passes
dirac = np.zeros(len(original_impulse))
dirac[0] = 1
prony_sos = []

fig, axs = plt.subplots(ncols=2)
axs[0].set_title('Frequency Response Comparison')
axs[0].set_xlim(80,20000)
axs[0].set_xlabel('Frequency')
axs[0].set_ylabel('Magnitude (dB)')


w_h, h_freq = signal.freqz(original_impulse, fs=fs, worN=8192)
h_freq = h_freq/max(h_freq)
impulse = np.copy(original_impulse)
axs[0].semilogx(w_h, 20 * np.log10(abs(h_freq)), label='Original Impulse Response')

for i in range(num_passes):
    lambda_ = i/num_passes
    zpk_prony = iir_to_fir(iir_order,lambda_,impulse)

    prony = signal.zpk2sos(zpk_prony[0],zpk_prony[1],zpk_prony[2])
    prony_sos.extend(prony)
    # print(prony)
    w_iir, h_prony = signal.sosfreqz(prony, fs=fs, worN=8192)
    h_prony = h_prony/max(h_prony)

    prony_impulse = signal.sosfilt(prony,dirac)*np.blackman(2*len(dirac))[len(dirac):]
    impulse = signal.lfilter(np.real(np.fft.ifft(1/np.fft.fft(prony_impulse))),1,impulse)
    w_,h_ = signal.freqz(impulse,fs=fs,worN=8192)
    h_ = h_/max(h_)

    axs[0].semilogx(w_h, 20 * np.log10(abs(h_prony)), label='iter: '+str(i))
    axs[1].semilogx(w_, 20 * np.log10(abs(h_)), label='impulse iter: '+str(i))

w_final, h_final = signal.sosfreqz(prony_sos, fs=fs, worN=8192)
h_final = h_final/max(h_final)
axs[0].semilogx(w_h, 20 * np.log10(abs(h_final)), label='Final Prony Filter')
axs[0].legend()
axs[0].grid(True)

plt.show()
