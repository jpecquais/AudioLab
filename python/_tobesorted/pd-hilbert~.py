import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal

# Define the biquad coefficients
# Each sub-array is one biquad filter in the format [b0, b1, b2, a1, a2, a0]
biquad_coeffs = [
    [-0.260502, 0.02569, 1, 1, 0.02569, -0.260502],
    [0.870686, -1.8685, 1, 1, -1.8685, 0.870686,]
]
biquad_coeffs2 = [
    [0.94657, -1.94632, 1, 1, -1.94632, 0.94657],
    [0.06338, -0.83774, 1, 1, -0.83774, 0.06338]
]

# Convert to numpy array
sos = np.array(biquad_coeffs)
sos2 = np.array(biquad_coeffs2)
print(sos)
print(sos2)

w, h = signal.sosfreqz(sos)
w2, h2 = signal.sosfreqz(sos2)

magnitude = 20 * np.log10(np.abs(h))
magnitude2 = 20 * np.log10(np.abs(h2))

angle = 0
a = np.cos(angle)
b = np.sin(angle)
# Plotting
fig, ax = plt.subplots()
ax.semilogx(w / np.pi, np.angle(a*h+b*h2))  # Normalize frequency to [0, 1] range
# ax.plot(w / np.pi, np.angle(h2))  # Normalize frequency to [0, 1] range
ax.set_title('Frequency Response')
ax.set_xlabel('Normalized Frequency [×π rad/sample]')
ax.set_ylabel('Magnitude [dB]')
plt.show()