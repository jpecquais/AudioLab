import numpy as np
from matplotlib import pyplot as plt
from scipy import signal
import soundfile as sf

def linear2db(x,normalize=True):
    norm_factor = np.sqrt(np.sum(np.power(x,2)))
    return 20*np.log10(np.abs(x/(norm_factor if normalize else 1)))

def BarkScale():
    # Return the upper limit of the Bark Scale
    return np.array([100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500])

def tf2pdtf(b,a):
    pd_coef = []
    pd_coef.append(-a[1])
    pd_coef.append(-a[2])
    pd_coef.extend(b)
    return pd_coef

prime_number = [1,2,3,5,7,11,13,17,19,23,29,31,37,41,43]    

fs = 48000
alpha = 0.9
f_range = BarkScale()
fig, axs = plt.subplots(ncols=3)
number_of_iter = 3


number_of_bands = len(f_range)
number_of_allpass = 64
number_of_ap_per_bands = number_of_allpass // number_of_bands

delay_is_random = False


for iter in range(number_of_iter):
    print("iter"+str(iter))
    rho = []
    for f in f_range:
        for ap in range(number_of_ap_per_bands):
            rho.append(alpha*np.exp(1j*2*np.pi*f*(ap-np.random.rand(1)[0])/number_of_ap_per_bands))

    rho = np.array(rho)

    allpasses = []
    impulse = np.zeros(8192)
    impulse[0] = 1

    for r in rho:
        n = 1 if not delay_is_random else prime_number[int(np.floor(np.random.rand()*len(prime_number)))] # delay per section

        max_size = 3 if not delay_is_random else 2*prime_number[-1]+1

        b = np.zeros(max_size)
        b[0] = np.abs(r)**2
        b[n] = -2*np.real(r)
        b[2*n] = 1

        a = np.zeros(max_size)
        a[0] = 1
        a[n] = -2*np.real(r)
        a[2*n] = np.abs(r)**2
        
        allpasses.append(np.array([b,a]))
        impulse = signal.lfilter(b,a,impulse)

    print(allpasses)
    allpasses = np.array(allpasses)

    w = []
    h = []
    for allpass in allpasses:
        w, h_temp = signal.freqz(allpass[0],allpass[1],fs=fs)
        h.append(h_temp)

    h = np.array(h)
    h = np.prod(h, axis=0)

    axs[0].plot(w,linear2db(h,False))
    axs[0].set_ylim(-40,6)
    axs[1].plot(w,np.rad2deg(np.angle(h)))
    axs[2].stem(impulse)

    sf.write('perceptually-informed-allpasses-'+str(iter)+'.wav', np.real(impulse), fs)

plt.show()

exit()