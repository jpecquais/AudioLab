import numpy as np
from scipy import signal, linalg
import soundfile as sf 
import matplotlib.pyplot as plt
import audio_dspy as adsp

impulse, fs = sf.read('projects/bela.PAM/ressources/impulses_responses/final_IR_1024.wav')

order = 256
number_of_b = order//2
number_of_a = order//2
number_of_solutions = 20*order

freqs = np.power(2,np.linspace(np.log2(20),np.log2(fs/2),number_of_solutions))

output = np.zeros(number_of_solutions)
matrix = np.zeros((number_of_solutions, number_of_b + number_of_a - 1))

t = np.arange(number_of_b + number_of_a) / fs
for i, f in enumerate(freqs):
    input_sig = np.sin(2 * np.pi * f * t)
    output_sig = signal.lfilter(impulse, 1, input_sig)
    input_sig = np.flip(input_sig)
    output_sig = np.flip(output_sig)
    output[i] = output_sig[0]
    matrix[i, :] = np.concatenate([output_sig[1:number_of_a] * (-1), input_sig[:number_of_b]])

# Compute the coefficients
coefficient_list = linalg.pinv(matrix) @ output

a = coefficient_list[:number_of_a-1]
b = coefficient_list[number_of_a-1:number_of_a + number_of_b - 1]
a = np.concatenate([[1],a])

sos = signal.tf2sos(b,a)

w, h_fir = signal.freqz(impulse,1,worN=8192,fs=fs)
w, h_iir = signal.sosfreqz(sos,worN=8192,fs=fs)
# w, h_iir = signal.freqz(b,a,worN=8192,fs=fs)

plt.semilogx(w,20*np.log10(np.abs(h_fir/max(np.abs(h_fir)))))
plt.semilogx(w,20*np.log10(np.abs(h_iir/max(np.abs(h_iir)))))
plt.xlim(80,20000)

plt.show()