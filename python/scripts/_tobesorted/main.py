import pd
import random
# import audio_dspy as adsp
import numpy as np
from scipy import signal, fftpack
import scipy.linalg as lnalg

def tf2minphase(h):
    """Converts a transfer function to minimum phase

    Parameters
    ----------
    h : ndarray
        Numpy array containing the original transfer function

    Returns
    -------
    h_min : ndarray
        Numpy array containing the minimum phase transfer function
    """
    H = np.abs(np.fft.fft(h))
    arg_H = -1*fftpack.hilbert(np.log(H))
    H_min = H * np.exp(-1j*arg_H)
    h_min = np.real(np.fft.ifft(H_min))
    return h_min

def allpass_warp(rho, h):
    """Performs allpass warping on a transfer function

    Parameters
    ----------
    rho : float
        Amount of warping to perform. On the range (-1, 1). Positive warping
        "expands" the spectrum, negative warping "shrinks"
    h : ndarray
        The transfer function to warp

    Returns
    -------
    h_warped : ndarray
        The warped transfer function
    """
    b_ap = np.array([rho, 1])
    a_ap = np.array([1, rho])

    x = np.zeros(len(h))
    x[0] = 1
    h_warp = np.zeros(len(h))
    for n, _ in enumerate(h):
        h_warp += h[n] * x
        x = signal.lfilter(b_ap, a_ap, x)
    return h_warp

def prony(x, nb, na):
    """Uses Prony's method to generate IIR filter coefficients
    that optimally match a given transfer function

    Parameters
    ----------
    x : ndarray
        Numpy array containing the transfer function
    nb : int
        Number of feedforward coefficients in the resulting filter
    na : int
        Number of feedback coefficients in the resulting filter

    Returns
    -------
    b : ndarray
        Feedforward coefficients
    a : ndarray
        Feedback coefficients
    """
    h = tf2minphase(x)
    k = len(h)-1
    H = np.mat(lnalg.toeplitz(np.array(h), np.append([1], np.zeros(k))))
    H = H[:, 0:(na+1)]
    H1 = H[0:(nb+1), :]
    h1 = H[(nb+1):(k+1), 0]
    H2 = H[(nb+1):(k+1), 1:(na+1)]
    a = np.vstack((np.mat(1), -H2.I * h1))
    aT = a.T
    b = aT * H1.T

    return b.getA()[0], aT.getA()[0]

def randomNumber():
    return random.randint(0, 100)

def prony_series_iir(h, b_order, a_order, lambda_):

    # Prony's method to approximate the impulse response
    # Adjust `a_order` and `b_order` as needed

    h_warped = allpass_warp(lambda_,h)
    bwp, awp = prony(h_warped, b_order, a_order)
    zwp = np.roots(bwp)
    pwp = np.roots(awp)
    z = (zwp+lambda_)/(1+lambda_*zwp)
    p = (pwp+lambda_)/(1+lambda_*pwp)
    k = 0.1
    sos = signal.zpk2sos(z, p, k)
    print(sos)
    output = []
    # for i in range(len(sos)):
    #     output.append(np.delete(sos[i],3).tolist())

    for so in sos:
        output.append(-so[4])
        output.append(-so[5])
        output.append(so[0])
        output.append(so[1])
        output.append(so[2])
    
    return output

def onBang():
    return 0

def py4pdLoadObjects():
    # pd.add_object(randomNumber,"test",py_out=True) # without this, py.random will not be available in the patch.
    # pd.add_object(prony_series_iir,"prony_series_iir",py_out=True)
    prony_series_iir = pd.new_object("prony_series_iir")
    prony_series_iir.addmethod_bang(onBang)