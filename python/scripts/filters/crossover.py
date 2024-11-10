import numpy as np
from matplotlib import pyplot as plt
from scipy import signal

fs = 48000

# First order crossover
hip_b, hip_a = signal.butter(1,800,'highpass', fs = fs)
lop_b, lop_a = signal.butter(1,800,'lowpass', fs = fs)

w, hip_h = signal.freqz(hip_b,hip_a,fs=fs)
w, lop_h = signal.freqz(lop_b,lop_a,fs=fs)
w, cross_h = signal.freqz(hip_b*lop_b,hip_a*lop_a,fs=fs)

plt.semilogx(w,20*np.log10(np.abs(hip_h)))
plt.semilogx(w,20*np.log10(np.abs(lop_h)))
plt.semilogx(w,20*np.log10(np.abs(hip_h+lop_h)))
plt.show()