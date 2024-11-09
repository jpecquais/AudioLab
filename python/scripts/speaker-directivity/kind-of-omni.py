import numpy as np
from matplotlib import pyplot as plt

theta = np.linspace(0,2*np.pi,2000)

fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})

sides = np.abs(np.cos(theta))
omni = np.ones(len(theta))
a = 0.5
diff = (1-a)*omni+a*sides

ax.plot(theta,diff)

plt.show()