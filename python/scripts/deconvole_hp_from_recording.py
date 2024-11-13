import numpy as np
import os
from scipy import io, signal
from matplotlib import pyplot as plt

def invert_filter(impulse: np.ndarray, epsilon: np.ndarray = 0):
    Impulse = np.fft.fft(impulse)
    Kernel = np.conj(Impulse) / (Impulse*np.conj(Impulse)+np.power(epsilon,2))
    return np.real(np.fft.ifft(Kernel))

def db2a(x):
    return np.power(10,x/20)

def a2db(x):
    return 20*np.log10(np.abs(x))

fs = 48000
impulse_sr, impulse = io.wavfile.read('cabinet_impulse_response.wav')


final_len = 1024
noise_floor = -100
kernel = invert_filter(impulse,db2a(noise_floor))
kernel = kernel[:final_len]*np.blackman(2*final_len)[final_len:]

model_path = 'render/model'

for file_name in os.listdir(model_path):
	if file_name.endswith('.wav'):
		file_path = os.path.join(model_path, file_name)
		file_name, ext = os.path.splitext(file_name)

		sample_rate, model = io.wavfile.read(file_path)

		output = signal.lfilter(kernel,a=1,x=model)
		output /= (np.max(output)*db2a(6)) # Recommended by AIDA-X

		io.wavfile.write('render/model_deconvolved/'+file_name+'-deconvolved.wav',fs,output)