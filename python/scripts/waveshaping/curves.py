import numpy as np
import matplotlib.pyplot as plt

lin = np.linspace(-1,1,4000)
print(lin)
fig, ax = plt.subplots(ncols=1)

gains = [0.33,0.34]
for gain in gains:
        tanh = np.tanh(gain*lin*np.pi)
        ax.plot(lin,tanh, label=str(gain))

# ax.plot(tanh)
ax.plot(lin,lin)
ax.set_ylim(-1,1)
ax.legend()
ax.legend()
plt.show()