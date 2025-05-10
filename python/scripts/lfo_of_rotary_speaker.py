import numpy as np
from matplotlib import pyplot as plt
from scipy import signal

fs = 48000
Ts = 1/fs
rot_speed = 5.7 #typical fast speed is 5.7
t = np.arange(0,1/rot_speed,Ts)
angle_ramp = 2*np.pi*rot_speed*t

normalized_distance_to_listener = np.abs(np.sin(0.5*angle_ramp))
normalized_diffraction = np.abs(np.sin(angle_ramp))
normalized_directivity = np.abs(np.sin(angle_ramp))
normalized_phase_shift = (angle_ramp-0.5*np.sin(2*angle_ramp))/(2*np.pi)

#Smoothing the lfo shapes
filter = signal.butter(1,25,'low',False,fs=fs,output='ba')
normalized_distance_to_listener = signal.lfilter(filter[0],filter[1],normalized_distance_to_listener)
normalized_diffraction = signal.lfilter(filter[0],filter[1],normalized_diffraction)
normalized_directivity = signal.lfilter(filter[0],filter[1],normalized_directivity)
normalized_phase_shift = signal.lfilter(filter[0],filter[1],normalized_phase_shift)

g_att_db = -20
g_att = 10**(g_att_db/20)

angles = np.linspace(0,2*np.pi,1000)
lfo_t = np.abs(np.sin(angles))

g_t = g_att*lfo_t
rms_g_t = np.sqrt(np.mean(np.power(g_t,2)))
K = 1/rms_g_t
K_dB = 20*np.log10(K)

print(K_dB)

# plt.plot(angle_ramp)
plt.plot(normalized_distance_to_listener)
plt.plot(normalized_diffraction)
plt.plot(normalized_directivity)
plt.plot(normalized_phase_shift)
plt.show()