import numpy as np
from matplotlib import pyplot as plt
import math
import scipy

def legendre(n):
    return scipy.special.legendre(n)

def SphericalHarmonic(l):
    return lambda theta: legendre(l)(np.cos(theta))

a2db = lambda x : 20*np.log10(np.abs(x))

fig, axs = plt.subplots(ncols=2,subplot_kw={'projection': 'polar'})

order = [0,1,2,4,8]
theta = np.linspace(0,2*np.pi,1000)

# print(scipy.special.legendre(3))
poly = []
for l in order:
    legendre_poly = legendre(l)
    poly.append(legendre_poly)
    # print(legendre_poly)
    axs[0].plot(theta,a2db(legendre_poly(np.cos(theta))))
    axs[0].set_ylim(-20,0)
    axs[0].set_rticks([-20, -12, -6, 0, 3])  # Less radial ticks


# dir = 1/4*(a*harm[0](theta)+b*harm[1](theta)+c*harm[3](theta)+d*harm[4](theta))
# axs[1].plot(theta,a2db())
axs[1].set_ylim(-20,0)
axs[1].set_rticks([-20, -12, -6, 0, 3])  # Less radial ticks

plt.show()