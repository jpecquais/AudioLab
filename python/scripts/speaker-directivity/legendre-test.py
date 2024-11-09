import numpy as np
from matplotlib import pyplot as plt
import math
import scipy

def legendre(n):
    return scipy.special.legendre(n)

def SphericalHarmonic(l):
    return lambda theta: legendre(l)(np.cos(theta))

a2db = lambda x : 20*np.log10(np.abs(x))

fig, axs = plt.subplots(ncols=3,subplot_kw={'projection': 'polar'})

legendre2 = lambda x : 1.5 * np.power(x,2) - 0.5
legendre4 = lambda x : 4.375 * np.power(x,4) - 3.75*np.power(x,2) + 0.375
legendre8 = lambda x : 50.27 * np.power(x,8) - 93.84 * np.power(x,6) + 54.14 * np.power(x,4) - 9.844 * np.power(x,2) + 0.2734

order = [0,1,2,4,8]
theta = np.linspace(0,2*np.pi,1000)

# axs[0].plot(theta,a2db(legendre2(np.cos(theta))))
alpha = 0.8
backDir = alpha*(1+np.clip(np.cos(theta),-1,0))+(1-alpha)
alpha = 0.5
axs[0].plot(theta,a2db((legendre2(np.cos(theta)))*backDir))
axs[1].plot(theta,a2db((legendre4(np.cos(theta)))*backDir))
axs[2].plot(theta,a2db((legendre8(np.cos(theta)))*backDir))
for ax in axs:
    ax.set_ylim(-20,0)
    ax.set_rticks([-20, -12, -6, 0, 3])  # Less radial ticks

plt.show()