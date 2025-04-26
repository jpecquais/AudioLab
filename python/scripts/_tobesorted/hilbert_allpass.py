import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal

N = 14

z = []
p = []

z1 = []
p1 = []

z2 = []
p2 = []

for n in range(N):
    _z = np.exp(np.pi/(np.power(2,n)))
    z.append(_z)
    p.append(1/_z)

# print(z,p)
# print(z2,p2)

z1 = z[::2]
p1 = p[::2]

z2 = z[1::2]
p2 = p[1::2]

for n in range(len(z1)):
    z1.append(-z1[n])
    p1.append(-p1[n])
for n in range(len(z2)):
    z2.append(-z2[n])
    p2.append(-p2[n])

w, tf1 = signal.freqz_zpk(z1,p1,1)
w, tf2 = signal.freqz_zpk(z2,p2,1)
w, tf3 = signal.freqz([0,1],[1,0])
tf1/=abs(tf1[0])
tf2/=abs(tf2[0])
tf2*=tf3

print(z1, p1)
print(z2, p2)

tf = tf1*tf2
phi1 = np.unwrap(np.angle(tf1))*180/np.pi
phi2 = np.unwrap(np.angle(tf2))*180/np.pi
diff_phi = phi1-phi2

fig, axs = plt.subplots(3,2)
axs[0][0].semilogx(w,20*np.log10(abs(tf1)))
axs[0][1].semilogx(w,np.angle(tf1,deg=True))
axs[1][0].semilogx(w,20*np.log10(abs(tf2)))
axs[1][1].semilogx(w,np.angle(tf2,deg=True))
axs[2][0].semilogx(w,20*np.log10(abs(tf)))
axs[2][1].semilogx(w,diff_phi)
axs[2][1].set_ylim(0,180)
# axs[all][all].set_ylim(6,-40)
for ax in axs:
    ax[0].set_ylim(-40,6)

# print("Maximal phase error: "+str(np.max(abs(diff_phi))))
# axs[0].stem(z1,np.zeros(len(z1)))

plt.show()