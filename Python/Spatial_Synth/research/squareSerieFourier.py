import matplotlib.pyplot as plt
import numpy as np

def fourierCoef(N):
    cn = []
    for n in np.arange(1,2*N+1,2):
        buf = 4/(n*np.pi)#(1-np.exp(-1j*np.pi*n))/(-1j*2*np.pi*n) if n !=0 else 0
        cn.append(buf)
    return np.array(cn)

# Sampling rate
fe = 384e+3
Te = 1/fe

# Square signal params
f0 = 100
T0 = 1/f0

# Time generation
t = np.linspace(0,T0,int(T0*fe))

square_wave=np.zeros(len(t))
for i in range(len(t)):
    square_wave[i] = 1 if np.sin(2*np.pi*f0*t[i]) > 0 else 0

# Calculate Fourier Coefficien
N = 3
n = np.arange(-N,N)
cn = np.zeros_like(n,dtype=complex)
for i in range(len(n)):
    cn[i] = np.sum(square_wave*np.exp(-1j*2*np.pi*n[i]*t))/T0

# cn computation

fig, axs = plt.subplots(nrows=2)

# Samples
L = 4*T0/Te
n = np.arange(0,L)

fSqr=[]
for i in np.arange(-N,N):
    buf=cn[i+N]*np.exp(-1j*2*np.pi*i*f0*n*Te)
    # f=f+f0
    fSqr.append(buf)
    axs[0].plot(n*Te,buf)

Sqr = np.sum(fSqr, axis=0)
axs[1].plot(n*Te,Sqr)

plt.show()




# # Square sig
# x = []
# for n in N:
#     buf = 1 if (np.sin(2*np.pi*f0*n*Te) > 0) else -1
#     x.append(buf)