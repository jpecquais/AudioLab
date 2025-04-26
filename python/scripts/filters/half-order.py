import numpy as np
from scipy import signal
from matplotlib import pyplot as plt


fs = 48000
fc = 1000
fft_len = 8192

filter1 = signal.butter(1,fc,'highpass',fs=fs, output='sos')
filter2 = signal.butter(2,fc,'highpass',fs=fs, output='sos')

w, h1 = signal.sosfreqz(filter1,worN=fft_len,fs=fs)
w, h2 = signal.sosfreqz(filter2,worN=fft_len,fs=fs)


fig, axs = plt.subplots(ncols=2)
gains = np.arange(0,1,0.125)

for gain in gains:
    axs[0].semilogx(w,20*np.log10(np.abs(h2*gain+h1*(1-gain))))
    axs[1].semilogx(w,np.angle(h2*gain+h1*(1-gain)))
plt.show()