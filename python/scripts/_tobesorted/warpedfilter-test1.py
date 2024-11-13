import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

# Define the warping function and its inverse
def warp_frequency(omega, lambda_):
    """
    Warp the frequency using the given lambda parameter.
    """
    # return np.tan(omega / (2 * lambda_)) * (2 * lambda_)
    return np.arctan((1-np.power(lambda_,2))*np.sin(omega)/((1+np.power(lambda_,2)*np.cos(lambda_))-2*lambda_))

def unwarp_frequency(omega_warped, lambda_):
    """
    Apply the inverse warping.
    """
    return 2 * np.arctan(omega_warped / (2 * lambda_)) * lambda_

def unwarp_poles(poles, lambda_):
    """
    Apply the inverse warping on poles.
    """
    return (poles+lambda_)/(1+poles)

# Define your warping parameter
lambda_ = 0.1  # This value is chosen as an example; adjust based on your needs

# Define the original (unwarped) critical frequencies for the filter design
# For example, for a bandpass filter
lowcut = 0.1 * np.pi  # Low cut frequency in radians/sample
highcut = 0.2 * np.pi  # High cut frequency in radians/sample

# Apply the warping to the critical frequencies
lowcut_warped = warp_frequency(lowcut, lambda_)
highcut_warped = warp_frequency(highcut, lambda_)
# Design the filter using the warped frequencies
# Using a Butterworth bandpass filter as an example
z, p, k = signal.butter(4, [lowcut_warped, highcut_warped], btype='band', analog=False, output='zpk', fs=2*np.pi)
sos2 = signal.butter(4, [lowcut, highcut], btype='band', analog=False, output='sos', fs=2*np.pi)

for i in range(len(p)):
    p[i]=unwarp_poles(p[i],lambda_)

sos = signal.zpk2sos(z,p,k)

# Frequency response of the designed filter
w, h = signal.sosfreqz(sos, worN=2000)
w, h2 = signal.sosfreqz(sos2, worN=2000)

# Apply the inverse warping to the frequency response for plotting
# w_unwarped = unwarp_frequency(w, lambda_)

# Plot the frequency response
plt.figure()
plt.plot(w, 20 * np.log10(np.abs(h)), label='Warped IIR Filter Response')
plt.plot(w, 20 * np.log10(np.abs(h2)), label='IIR Filter Response')
plt.xlabel('Frequency (rad/sample)')
plt.ylabel('Amplitude (dB)')
plt.grid(True)
plt.legend()
plt.show()
