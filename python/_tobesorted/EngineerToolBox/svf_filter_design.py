import numpy as np
import matplotlib.pyplot as plt

fs = 48000
ts = 1/fs

f = np.arange(20,fs/2,1)
z = np.exp(1j*2*np.pi*f*ts)

fc=1000
F1=2*np.sin(np.pi*fc/fs)
r=F1
Q=0.1
Q1=1/Q
q=1-F1*Q1

H=1+Q1*(r*(1-np.power(z,-1))/(1+(np.power(r,2)-q-1)*np.power(z,-1)+q*np.power(z,-2)))
print(20*np.log10(np.abs(H[fc-20])))

fig, axs = plt.subplots(nrows=2)
axs[0].plot(f,20*np.log10(np.abs(H)))
axs[0].set_xscale('log')
axs[0].set_ylim(-12,+12)
axs[0].grid(visible=True)
# axs[0].set_grid()

axs[1].plot(f,180*np.angle(H)/np.pi)
axs[1].set_xscale('log')
axs[1].set_ylim(-180,180)
axs[1].grid(visible=True)

plt.show()