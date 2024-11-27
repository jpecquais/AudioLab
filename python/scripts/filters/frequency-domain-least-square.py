import numpy as np
import scipy.signal as signal
import soundfile as sf 
import matplotlib.pyplot as plt
import audio_dspy as adsp

impulse, fs = sf.read('projects/bela.PAM/ressources/impulses_responses/final_IR_1024.wav')
# impulse *= np.blackman(2*len(impulse))[len(impulse):]

order = 256
number_of_b = int(2/4*order)
number_of_a = int(2/4*order)
number_of_solutions = 10*order

max_number_of_coefficient = max(number_of_a,number_of_b)
freqs = np.power(2,np.linspace(np.log2(5000),np.log2(fs/2),number_of_solutions))
# freqs = np.linspace(20,fs/2,number_of_solutions)

output = np.zeros(number_of_solutions)
matrix = np.zeros((number_of_solutions, number_of_b + number_of_a - 1))

lambda_ = 0.2
# impulse = adsp.allpass_warp(lambda_,impulse)

for i, f in enumerate(freqs):
    input_sig = np.cos(2 * np.pi * f * range(number_of_b + number_of_a) / fs)
    output_sig = signal.lfilter(impulse, 1, input_sig)
    # print(len(input_sig),len(output_sig))
    input_sig = np.flip(input_sig)
    output_sig = np.flip(output_sig)
    output[i] = output_sig[0]
    matrix[i, :] = np.concatenate([output_sig[1:number_of_a] * (-1), input_sig[:number_of_b]])

# Compute the coefficients
coefficient_list = np.linalg.pinv(matrix) @ output

awp = coefficient_list[:number_of_a-1]
bwp = coefficient_list[number_of_a-1:number_of_a + number_of_b - 1]
normalizing_factor = max(np.max(awp),np.max(bwp))
awp /= normalizing_factor
bwp /= normalizing_factor
awp = np.concatenate([[1],awp])

# zwp = np.roots(bwp)
# pwp = np.roots(awp)
# z = (zwp+lambda_)/(1+lambda_*zwp)
# p = (pwp+lambda_)/(1+lambda_*pwp)
# k=0.5
# exit()
w, h_fir = signal.freqz(impulse,1,worN=8192,fs=fs)
w, h_iir = signal.freqz(bwp,awp,worN=8192,fs=fs)

plt.semilogx(w,20*np.log10(np.abs(h_fir/max(np.abs(h_fir)))))
plt.semilogx(w,20*np.log10(np.abs(h_iir/max(np.abs(h_iir)))))
plt.xlim(80,20000)

plt.show()