import matplotlib.pyplot as plt
import numpy as np

fig, axs = plt.subplots(nrows=1, ncols=2)

fe = 384000
Te = 1/fe

f0 = 1
T0 = 1/f0

N = T0/Te
n = np.arange(0,N)

sine = np.cos(2*np.pi*f0*n*Te)

square = []
for nn in n:
    buf = 1 if (np.sin(2*np.pi*f0*nn*Te) >= 0) else -1
    square.append(buf)

phasor = 2*n/N-1

triangle = []
for nn in n:
    buf = (4*nn/N-1) if (np.sin(2*np.pi*f0*nn*Te) >= 0) else (-4*nn/N+3)
    triangle.append(buf)

Sine = (1/N)*np.fft.fft(sine)
Sine = np.fft.fftshift(Sine)

Square = (1/N)*np.fft.fft(square)
Square = np.fft.fftshift(Square)



Phasor = (1/N)*np.fft.fft(phasor)
Phasor = np.fft.fftshift(Phasor)
# print(abs(Phasor))

Triangle = (1/N)*np.fft.fft(triangle)
Triangle = np.fft.fftshift(Triangle)
# print(abs(Phasor))

middle_index = len(Phasor) // 2  # Finding the middle index of the list
end_index = middle_index + 40  # Calculating the end index
print(abs(Triangle[middle_index:end_index]))

k = np.arange(-N/2,N/2)

axs[0].plot(n*Te,triangle)
# axs[0].set_xlim([0,3*T0])

axs[1].stem(k*f0,np.abs(Triangle))
axs[1].set_xlim([0,10])

plt.show()