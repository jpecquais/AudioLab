import numpy as np
from matplotlib import pyplot as plt
from scipy import signal

def db2linear(x):
    return np.power(10,x/20)

Fe = 48000
Te = 1/Fe
N = np.arange(0,1024,1)

f = 100 
gain = 20 #dB

sig = np.sin(2*np.pi*f*N*Te)
shaped_sig = np.tanh(db2linear(gain)*sig)
control_sig = np.maximum(np.abs(shaped_sig),0) / np.maximum(np.abs(sig),1e-30)

sos = signal.butter(1,Wn=1000,fs=Fe,output='sos')
control_sig = signal.sosfilt(sos,control_sig)

fig, axs = plt.subplots(ncols=3)
axs[0].plot(sig)
axs[1].plot(shaped_sig)
axs[2].plot(control_sig*sig)
plt.show()