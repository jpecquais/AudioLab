import numpy as np
from matplotlib import pyplot as plt

fs = 48000
f = 7
t = np.arange(0,2/f,1/fs)

sin = np.sin(2*np.pi*f*t)
sin2 = np.sin(2*np.pi*f*t*1/2+np.pi/4)

plt.plot(t,sin2)
plt.plot(t,(sin+1)/2)
# plt.plot(t,np.copysign((sin+1)/2,sin2))
# plt.plot(t,sin)
plt.show()