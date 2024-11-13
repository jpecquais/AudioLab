import numpy as np
import soundfile as sf
from scipy import signal
from matplotlib import pyplot as plt

file = sf.read('./python/channel-extraction/no_surprises.wav')

sig = np.array(file[0])
sig = np.transpose(sig)

lt_sig = sig[0]
rt_sig = sig[1]
fs = file[1]

Lt_sig = np.fft.fft(lt_sig)
Rt_sig = np.fft.fft(rt_sig)

# Icc = signal.correlate(Lt_sig,Rt_sig)
# signal.coherence()

Crosscorr_LtRt = Lt_sig*np.conj(Rt_sig)
Crosscorr_RtLt = np.conj(Lt_sig)*Rt_sig
Autocorr_Lt = Lt_sig*np.conj(Lt_sig)
Autocorr_Rt = Rt_sig*np.conj(Rt_sig)
# Icc = np.sqrt((Crosscorr_LtRt*np.conj(Crosscorr_RtLt))/(Autocorr_Lt*Autocorr_Rt))
Icc = (Crosscorr_LtRt*np.conj(Crosscorr_RtLt))/(Autocorr_Lt*Autocorr_Rt)
# print(Icc)
plt.stem(Icc)
plt.show()
# Icc = 0.5
exit()
C_sig = Icc*(Lt_sig+Rt_sig)
L_sig = (1-Icc)*Lt_sig
R_sig = (1-Icc)*Rt_sig

c_sig = np.real(np.fft.ifft(C_sig))
l_sig = np.real(np.fft.ifft(L_sig))
r_sig = np.real(np.fft.ifft(R_sig))

sf.write('./python/channel-extraction/c_no_surprises.wav',c_sig,fs)
sf.write('./python/channel-extraction/l_no_surprises.wav',l_sig,fs)
sf.write('./python/channel-extraction/r_no_surprises.wav',r_sig,fs)
