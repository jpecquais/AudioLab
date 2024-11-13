import numpy as np
import matplotlib.pyplot as plt

lin = np.linspace(0,4,4000)
print(lin)
tanh = np.tanh(lin*np.pi)
fig, axs = plt.subplots(ncols=2)

alphas = [1]
betas = [1]
for alpha in alphas:
    for beta in betas:
        # xlim = lin/(alpha*np.abs(lin)+1)
        xlim2 = lin/(np.power(np.power(alpha*lin,beta)+1,1/beta))
        # xlim = lin/(alpha*np.abs(lin)+1/alpha)
        # axs[0].plot(lin,4*xlim, label=str(beta))
        axs[1].plot(lin,np.power(2,1/beta)*xlim2, label=str(beta))

# ax.plot(tanh)
axs[1].plot(lin,lin)
axs[1].set_ylim(0,1.75)
axs[0].legend()
axs[1].legend()
plt.show()