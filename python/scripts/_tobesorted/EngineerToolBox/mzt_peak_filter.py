import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

fs = 48000
ts = 1/fs

f = np.arange(20,fs/2,1)
Z = np.exp(1j*2*np.pi*f*ts)

fc = 1000
wc = 2*np.pi*fc #np.sqrt(w1 * w2)
g0=1
g=1+g0
# q=1
Q=1#q*g/2

w = 2 * np.pi * f

# Peak filter coefficient, just for the sake of it

b = [1,np.sqrt(g)*wc/Q,np.power(wc,2)] 
a = [1,wc/(np.sqrt(g)*Q),np.power(wc,2)]

# print(signal.butter(2,fc/fs,'low',False))

# Compute zeros and poles to prepare match Z transform

num_delta = np.power(b[1],2)-4*b[2]*b[0]
denom_delta = np.power(a[1],2)-4*a[2]*a[0]

print(b)
print(a)
print(num_delta)
print(denom_delta)

if num_delta > 0:
    z_a = [(-b[1]-np.sqrt(num_delta))/2,(-b[1]+np.sqrt(num_delta))/2]
else:
    z_a = [(-b[1]-1j*np.sqrt(-num_delta))/2,(-b[1]+1j*np.sqrt(-num_delta))/2]

if denom_delta >= 0:
    p_a = [(-a[1]-np.sqrt(denom_delta))/2,(-a[1]+np.sqrt(denom_delta))/2]
else:
    p_a = [(-a[1]-1j*np.sqrt(-denom_delta))/2,(-a[1]+1j*np.sqrt(-denom_delta))/2]


ws, hs = signal.freqs_zpk(z_a,p_a,1,fs) # analog transfer function

print(z_a)
print(p_a)

# Maps s plane zeros and poles to z plane

z_n = []
p_n = []

for zz in z_a:
    z_n.append(np.exp(zz*ts))

for pp in p_a:
    p_n.append(np.exp(pp*ts))

print(z_n)
print(p_n)

zc = np.exp(1j*2*np.pi*fc*ts)
bbb, aaa = signal.zpk2tf(z_n,p_n,1)

print(bbb)
print(aaa)

wz, hz = signal.freqz_zpk(z_n,p_n,1,fs)
h0 = hz[0]
hNyq = hz[47999]
hz = hz/min(h0,hNyq)

# Now, we are computing the corretion FIR filter

b1 = (0.05*((fc)/(fs))+0.3*np.power((fc)/(fs),2)+0.5*np.power(fc/(fs),3))*(Q if Q<1 else 1/Q)
b0 = 1-b1

w_fir,h_fir = signal.freqz([1,b1/b0],1,fs)
h_fir=h_fir*1/h_fir[0]


fig, ax = plt.subplots()
ax.plot(ws/(2*np.pi),20*np.log10(np.abs(hs)))
# ax.plot(fs/2*wz/np.pi,20*np.log10(np.abs(hz)))
ax.plot(fs/2*wz/np.pi,20*np.log10(np.abs(hz*h_fir)))
ax.plot(fs/2*wz/np.pi,np.zeros(len(wz)))
ax.set_xscale('log')
ax.set_xlim(20,20000)
# ax.set_ylim(-24,24)
plt.show()
