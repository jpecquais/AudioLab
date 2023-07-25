import numpy as np


# Sampling rate
fe = 48e+3*16
Te = 1/fe

#Sig parameters
f0 = 100 #Hz
T0 = 1/f0 #s
t = np.arange(0,T0,int(T0*fe))

def complexFourierCoef(sig,N):
    n = np.arange(-N,N)
    cn = np.zeros_like(n,dtype=complex)
    for i in range(len(n)):
        cn[i] = np.sum(sig*np.exp(-1j*2*np.pi*n[i]*t))/T0
    return cn

sine = np.sin(2*np.pi*f0*t)
square = np.zero(len(t))
for i in range(len(t)):
    square[i] = 1 if np.sin(2*np.pi*f0*t[i]) > 0 else -1


N = 10
cn = complexFourierCoef(square,N)


