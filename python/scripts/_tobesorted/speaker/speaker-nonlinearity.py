import numpy as np
import matplotlib.pyplot as plt

fs = 48e3
f = 71.2
w = 2*np.pi*f/fs
Qts = 0.64
K = 0.136
b = {0,0,0}

b[0] = (1+1/(w*Qts)+1/(np.power(w,2)))/K
b[1] = (-1/(w*Qts)-2/(np.power(w,2)))/K
b[2] = 1/(w*K)

voltage = np.linspace(0,1,1000)

fig, ax = plt.subplot()
ax.plot()