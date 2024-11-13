import numpy as np
from matplotlib import pyplot as plt

vals = np.linspace(-1,1,4000)

g = 10
waveshaper = (1+g)*vals / (np.abs(vals) + g)

fig, ax = plt.subplots()
ax.plot(waveshaper)
plt.show()