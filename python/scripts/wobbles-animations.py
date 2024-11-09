import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
import functools
from scipy import signal

def wobbles(frame,ax,num_allpasses,imp_real,imp_imag):
    angles = np.append(np.linspace(-np.pi,np.pi,50),np.linspace(np.pi,-np.pi,50))
    angle = angles[frame]
    phase_rotated_imp = imp_real*np.cos(angle)+imp_imag*np.sin(angle)
    allpassed_imp = imp_real

    for i in range(num_allpasses):
        allpassed_imp = signal.sosfilt(ap_sos,allpassed_imp)

    impulse_response = phase_rotated_imp*0.707+allpassed_imp*0.707
    Impulse_Response = np.fft.fft(impulse_response)
    Impulse_Response = np.fft.fftshift(Impulse_Response)
    fs = 48000
    f = np.linspace(-fs/2,fs/2,len(Impulse_Response))
    pos_f = int(len(Impulse_Response)/2)
    ax.clear()
    ax.set_ylim(-40,12)
    ax.set_xlim(20,20e+3)
    ax.semilogx(f[pos_f:],20*np.log10(np.abs(Impulse_Response[pos_f:])))

impulse = np.zeros(8192*4)
impulse[0] = 1

hil_real_sos = []
hil_real_sos1 = [-0.260502,0.02569,1.,1.,0.02569,-0.260502]
hil_real_sos2 = [0.870686,-1.8685,1.,1,-1.8685,0.870686]
hil_real_sos.append(hil_real_sos1)
hil_real_sos.append(hil_real_sos2)
hil_real_sos = np.array(hil_real_sos)

hil_imag_sos = []
hil_imag_sos1 = [0.94657,-1.94632,1.,1.,-1.94632,0.94657]
hil_imag_sos2 = [0.06338,-0.83774,1.,1,-0.83774,0.06338]
hil_imag_sos.append(hil_imag_sos1)
hil_imag_sos.append(hil_imag_sos2)
hil_imag_sos = np.array(hil_imag_sos)

ap_sos= [-0.96,1,0,1,-0.96,0]
ap_sos = np.array(ap_sos)

imp_real = signal.sosfilt(hil_real_sos,impulse)
imp_imag = signal.sosfilt(hil_imag_sos,impulse)

fig,ax = plt.subplots()
angles = np.linspace(0,2*np.pi,10)
num_allpasses = 1

# for angle in angles:
#     wobbles(ax,angle,num_allpasses,imp_real,imp_imag)

ani = animation.FuncAnimation(fig, functools.partial(wobbles,ax=ax,num_allpasses=num_allpasses,imp_real=imp_real,imp_imag=imp_imag),frames=100, interval=1)
plt.show()