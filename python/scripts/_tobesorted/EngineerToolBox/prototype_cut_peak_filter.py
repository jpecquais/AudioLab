import numpy as np
import matplotlib.pyplot as plt

""" 
Prototype d'un filtre atténuateur de bande, basé sur un passe-tout. Plus l'atténuation est importante, plus le facteur de qualité augmente.
"""

fs = 48000
ts = 1/fs
r=0.9

f = np.arange(20,fs/2,1)
z = np.exp(1j*2*np.pi*f*ts)

fc = 1000
g_db=-6
g=(np.power(10,g_db/20)-1)/2
print(g)
Q = 0.5*(1-g)
K = np.tan(np.pi*fc/fs)

a=[1,2*Q*(np.power(K,2)-1)/(np.power(K,2)*Q+K+Q),(np.power(K,2)*Q-K+Q)/(np.power(K,2)*Q+K+Q)]
b=[a[2],a[1],a[0]]

H = (1+g)-g*(b[0]+b[1]*np.power(z,-1)+b[2]*np.power(z,-2))/(a[0]+a[1]*np.power(z,-1)+a[2]*np.power(z,-2))#((z-z1)*(z-z2))/((z-p1)*(z-p2))
print(20*np.log10(np.abs(H[980])))
print(20*np.log10(np.abs(H[0])))

fig, axs = plt.subplots(nrows=2)
axs[0].plot(f,20*np.log10(np.abs(H)))
axs[1].plot(f,180*np.angle(H)/np.pi)
axs[0].set_xscale('log')
axs[0].set_ylim(-12,+12)
axs[1].set_xscale('log')
axs[1].set_ylim(-180,180)
plt.show()