import numpy as np
from matplotlib import pyplot as plt
import soundfile as sf
import sys
np.set_printoptions(threshold=sys.maxsize)

def linear2db(x,normalize=True):
    norm_factor = np.sqrt(np.sum(np.power(x,2)))
    return 20*np.log10(np.abs(x/(norm_factor if normalize else 1)))

fs = 48000
density = 1000                                  # Density : number of impulses per secondes
noise_len = 0.03                                # Velvet noise length (ms)
number_of_impulses = int(noise_len*density)     # Number of impulses
grid_size = fs//density                         # Grid Size : Average spacing between two impulses
log_grid = noise_len*fs/100*np.power(10,2*np.arange(number_of_impulses) / number_of_impulses)
sum_grid = np.sum(log_grid)

velvet_len = int(noise_len*fs)
decay = np.exp(-np.pi*2*1/noise_len*np.arange(0,velvet_len)/fs)
velvet = np.zeros(velvet_len)
log_velvet = np.zeros(velvet_len)

for grid in range(grid_size, len(velvet), grid_size):
    rd_pos = int(np.round(grid-grid_size*np.random.rand(1)[0]))
    rd_sign = 2*np.round(np.random.rand(1)[0])-1
    velvet[rd_pos] = rd_sign

for grid in log_grid:
    rd_pos = int(np.round(grid-grid*np.random.rand(1)[0]))#int(np.round(grid-grid*np.random.rand(1)[0]))
    rd_sign = 2*np.round(np.random.rand(1)[0])-1
    log_velvet[rd_pos] = rd_sign

# velvet *= decay
# log_velvet *= decay
for i, value in enumerate(velvet):
    if value == 1 or value == -1:
        print(f"Index: {i}, Value: {value}")

Velvet = np.fft.fft(velvet)
log_Velvet = np.fft.fft(log_velvet)

sf.write('logvelvet-noise.wav', log_velvet, fs)

mask_velvet = velvet != 0
mask_logvelvet = log_velvet != 0

fft_size = (noise_len*fs)
fig, axs = plt.subplots(ncols=2,nrows=2)
axs[0][0].stem(velvet)
axs[1][0].stem(log_velvet)
axs[0][1].semilogx(fs/fft_size*np.arange(0,fft_size/2),linear2db(Velvet[int(fft_size/2):]))
axs[1][1].semilogx(fs/fft_size*np.arange(0,fft_size/2),linear2db(log_Velvet[int(fft_size/2):]))

plt.show()
exit()
