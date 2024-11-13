import numpy as np
import scipy.signal as signal
from scipy.io import wavfile
import matplotlib.pyplot as plt
import audio_dspy as adsp



sample_rate, h = wavfile.read('IR.wav')

lambda_ = 0.65

# Prony's method to approximate the impulse response
# Adjust `a_order` and `b_order` as needed
a_order = 500  # Denominator polynomial order (related to the number of biquad sections)
b_order = 500  # Numerator polynomial order

h_warped = adsp.allpass_warp(lambda_,h)
bwp, awp = adsp.prony(h_warped, b_order, a_order)
zwp = np.roots(bwp)
pwp = np.roots(awp)
z = (zwp+lambda_)/(1+lambda_*zwp)
p = (pwp+lambda_)/(1+lambda_*pwp)
# b = adsp.allpass_warp_roots(-lambda_,b)
# a = adsp.allpass_warp_roots(-lambda_,a)
k=0.5
# Convert the filter into a series of second-order sections for stability
sos = signal.zpk2sos(z, p, k)
# sos = signal.tf2sos(b, a)

# Display the coefficients of the second-order sections (biquad filters)
print("Second-order sections (SOS) coefficients:")
print(abs(p))
# print(sos)

# Calculate the frequency response of the original impulse response
w_h, h_freq = signal.freqz(h, worN=8000)

# Calculate the frequency response of the IIR filter
w_iir, h_iir = signal.freqz_zpk(z, p, k, worN=8000)

# Plot both frequency responses on the same plot
plt.figure(figsize=(10, 6))
plt.semilogx(w_h / np.pi, 20 * np.log10(abs(h_freq)), label='Original Impulse Response')
plt.semilogx(w_iir / np.pi, 20 * np.log10(abs(h_iir)), label='IIR Filter (Prony)', linestyle='--')
plt.title('Frequency Response Comparison')
plt.xlabel('Normalized Frequency (π rad/sample)')
plt.ylabel('Magnitude (dB)')
plt.grid(True)
plt.legend()
plt.show()
