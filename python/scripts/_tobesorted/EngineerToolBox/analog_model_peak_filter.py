import numpy as np
import matplotlib.pyplot as plt


def H(s, wc, B, g):
    return (1+ g*np.power((wc * s * B) / (
            np.power(s, 2) + wc * s * B + np.power(wc, 2)),1))


fs = 48000
te = 1 / fs

n = 1
g = 1

fc = 10000
f_width = 0.2*(1+g)
f1 = fc - f_width / 2
f2 = fc + f_width / 2
w1 = 2 * np.pi * f1
w2 = 2 * np.pi * f2
B = 2 * np.pi * f_width

wc = np.sqrt(w1 * w2)

f = np.arange(20, 20000, 0.1)
w = 2 * np.pi * f


fig, ax = plt.subplots()
ax.set_xscale("log")
ax.set_ylim(-12, 12)

ax.plot(f, 20 * np.log10(np.power(H(1j * w, wc, B, g),n)))
approxS = 2 / te * (np.exp(1j * w * te) - 1) / (np.exp(1j * w * te) + 1)
ax.plot(f, 20 * np.log10(np.power(H(approxS, wc, B, g),n)))
ax.plot(f, 20 * np.log10(np.power(H(approxS, 2 / te * np.tan(wc * te / 2), B, g),n)))
plt.show()
