# Compute poles and zeros for producing to signal dephased by 90°. Abusively named Hilbert Transform
# From Traitement Numérique du Signal - 8eme edition - Bellanger

import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import quad
from scipy.special import ellipj
import scipy.signal as signal


fs = 48000

f_min = 0.028#10/fs
f_max = 0.33#20000/fs
epsilon = 2*np.pi/360

k = np.tan(np.pi*f_min)/np.tan(np.pi*f_max)
k1 = np.power((1-np.tan(epsilon*0.5))/(1+np.tan(epsilon*0.5)),2)

def dK(x,k):
    return 1/np.power((1-k**2*np.sin(x)**2),0.5)

K_k1, error = quad(lambda x: dK(x,k1), 0, np.pi/2)
K_k, error = quad(lambda x: dK(x,k), 0, np.pi/2)
K_1_k1, error = quad(lambda x: dK(x,np.power(1-k1**2,0.5)), 0, np.pi/2)
K_1_k, error = quad(lambda x: dK(x,np.power(1-k**2,0.5)), 0, np.pi/2)

order = K_k1*K_1_k/(K_k*K_1_k1)
order = round(order)

p = []
z = []

for i in range(order):
    sn, cn, dn, phi = ellipj((4*i+1)*K_1_k/(2*order), np.sqrt(1-k**2))
    p.append(-np.tan(np.pi*f_min)*sn/(np.sqrt(1-sn**2)))
    z.append((1+p[i])/(1-p[i]))

print(order)

p = [-0.0395,-0.3893,-3.8360,-1.0039,-0.1509]
z = [0.9240,0.4396,-0.5864,-0.00197,-0.7377]

z1 = z[:int(np.ceil(order/2))]
z2 = z[int(np.ceil(order/2)):]

print(z1,z2)

w, tf1 = signal.freqz_zpk(z1,p,1)
w, tf2 = signal.freqz_zpk(z2,p,1)
tf = tf1+tf2
# w, tf = signal.freqz_zpk(zf,p,1)

fig, axs = plt.subplots(1,3)
axs[0].semilogx(w, 20*np.log10(abs(tf)))
# axs[1].semilogx(w, 20*np.log10(abs(tf)))
axs[2].semilogx(w,np.angle(tf))

plt.show()
# print(k)
# print(k1)
# print(z)
# print(p)