import numpy as np
import scipy.signal as signal
from scipy.io import wavfile
from scipy import linalg
from scipy import fftpack
import matplotlib.pyplot as plt
import audio_dspy as adsp
# from scipy.linalg import lstsq

def normalize(x):
    return x/np.max(np.abs(x))

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
    return normalize(h)

def prony(imp, b_order, a_order):
    """
    Estimates the a and b coefficients of a transfer function from its impulse response
    using the Prony method.

    Parameters:
    - impulse_response: The impulse response of the system.
    - a_order: The order of the denominator polynomial (number of a coefficients).
    - b_order: The order of the numerator polynomial (number of b coefficients).

    Returns:
    - a_coefficients: Estimated a coefficients of the transfer function.
    - b_coefficients: Estimated b coefficients of the transfer function.
    """
    # Ensure the impulse response is a numpy array
    imp = adsp.tf2minphase(imp)
    imp = np.array(imp)
    N = len(imp)

    c = imp[b_order-1::-1]
    r = imp[:b_order]
    mtrx = linalg.toeplitz(c,r)

    a_coefficients = linalg.inv(mtrx)@imp[b_order:2*b_order]
    a_coefficients = np.append([1], -a_coefficients)
    
    mtrx = linalg.toeplitz(imp[:b_order],imp[:b_order])
    mtrx = np.tril(mtrx,0)
    # print(mtrx)
    b_coefficients = mtrx@a_coefficients[:b_order]
    # print(b_coefficients)

    # h = imp
    # na = a_order
    # nb = b_order

    # k = len(h)-1
    # H = np.mat(linalg.toeplitz(np.array(h), np.append([1], np.zeros(k))))
    # H = H[:, 0:(na+1)]
    # H1 = H[0:(nb+1), :]
    # h1 = H[(nb+1):(k+1), 0]
    # H2 = H[(nb+1):(k+1), 1:(na+1)]
    # a = np.vstack((np.mat(1), -H2.I * h1))
    # aT = a.T
    # b = aT * H1.T

    # return b.getA()[0], aT.getA()[0]

    return b_coefficients, a_coefficients

# Example usage of the function (commented out to prevent execution)
# impulse_response = [...]  # Your impulse response data here
# a_order = 2  # Order of the denominator polynomial
# b_order = 2  # Order of the numerator polynomial
# a_coefficients, b_coefficients = estimate_transfer_function_coefficients(impulse_response, a_order, b_order)
# print("a coefficients:", a_coefficients)
# print("b coefficients:", b_coefficients)


def residue_to_parallel_sos(residues, poles, k):
    # Preprocess poles and residues: Group into conjugate pairs if necessary
    # This example assumes residues and poles are already matched and ordered
    
    # Number of sections
    num_sections = len(poles) // 2 if len(poles) % 2 == 0 else (len(poles) + 1) // 2
    
    # Initialize SOS matrix
    sos = np.zeros((num_sections, 6))  # Each row: [b0, b1, b2, 1, a1, a2]
    
    section = 0
    for i in range(0, len(poles), 2):
        # Assuming poles are complex conjugates or real
        if i+1 < len(poles) and np.iscomplex(poles[i]):
            # Complex-conjugate pole pair
            p1, p2 = poles[i], poles[i+1]
            r1, r2 = residues[i], residues[i+1]
            
            # Convert to second-order coefficients
            sos[section, 3] = 1  # a0 is always 1 for normalized filters
            sos[section, 4] = -(p1 + p2).real  # a1
            sos[section, 5] = (p1 * p2).real  # a2
            
            # Assuming direct term affects the first section or distributed equally
            if i == 0:
                sos[section, :3] = [k + 2*r1.real, -2*r1.imag, 0]  # Example conversion
            else:
                sos[section, :3] = [2*r1.real, -2*r1.imag, 0]  # Adjust based on actual conversion logic
            
        elif i < len(poles):
            # Real pole, this example doesn't handle a single real pole at the end
            # Ideally, pair it with another real pole or a synthetic zero
            pass  # Placeholder for real pole handling
        
        section += 1
    
    # Handle remaining real pole if odd number of poles (not covered in this template)
    
    return sos

def parallel_filter(Bm,Am,FIR,x):
    y = np.zeros_like(x)  # Initialize output signal y with the same shape as x
    s = Am.shape  # Get the shape of the Am matrix

    for i in range(s[1]):  # Iterate over columns
        # Apply IIR filter for each column of Bm and Am to x and accumulate the result in y
        b = [Bm[0][i],Bm[1][i],0]
        a = [Am[0][i],Am[1][i],Am[2][i]]        
        y += signal.lfilter(b, a, x)

    # Add the output of the FIR filter applied to x
    y += signal.lfilter(FIR, [1], x)  # FIR filter coefficients in numerator, 1 in denominator for FIR

    return y

def parallel_filter_design(impresp,p,NFIR):
    # Remove poles at the origin
    p = p[p != 0]

    # Make the filter stable by flipping poles inside the unit circle
    p = np.array([1/np.conj(x) if np.abs(x) > 1 else x for x in p])

    # Order poles as complex conjugate pairs followed by real poles
    p = np.sort_complex(p)

    # Handle odd number of poles
    pnum = len(p)
    ppnum = 2 * (pnum // 2)
    ODD = pnum > ppnum

    # Initialize impulse response for modeling
    L = len(impresp)
    imp = np.zeros(L)
    imp[0] = 1

    # Construct the modeling signal matrix
    M = np.zeros((L, ppnum + NFIR))
    for k in range(0, ppnum, 2):
        resp = signal.lfilter([1], np.poly(p[k:k+2]), imp)
        M[:, k] = resp
        if k+1 < ppnum:
            M[:, k+1] = np.concatenate(([0], resp[:-1]))

    if ODD:
        resp = signal.lfilter([1], np.poly([p[-1]]), imp)
        M[:, ppnum] = resp  # Corrected to use ppnum for indexing

    for k in range(NFIR):
        M[:, ppnum + k] = np.concatenate((np.zeros(k), imp[:L-k]))

    y = impresp.reshape(-1, 1)  # Ensure y is a column vector

    # Least squares solution
    A = M.T @ M
    b = M.T @ y
    par = np.linalg.solve(A, b)

    # Construct Bm and Am matrices
    Bm = np.zeros((2, ppnum//2 + ODD))
    Am = np.zeros((3, ppnum//2 + ODD))
    for k in range(ppnum//2):
        Am[:, k] = np.poly(p[2*k:2*k+2])
        Bm[0:2, k] = par[2*k:2*k+2].flatten()

    if ODD:
        Am[:, -1] = np.concatenate((np.poly([p[-1]]), [0]))  # Handle first-order as second-order by adding a zero
        Bm[0, -1] = par[ppnum].flatten()  # Adjusted indexing for Bm

    # Construct the FIR part
    FIR = par[ppnum:ppnum+NFIR].flatten() if NFIR > 0 else np.zeros(NFIR)

    return Bm, Am, FIR

# Open impulse response
sample_rate, imp = wavfile.read('projects/bela.PAM/ressources/impulses_responses/final_IR_1024.wav')
imp = imp.astype(float)/8388608 #24int to float

# Filter parameters
lambda_ = 0.9
FIR_size = 128
b_order = 256  # Numerator polynomial order
a_order = 256  # Denominator polynomial order (related to the number of biquad sections)

# Prony's method to approximate the impulse response
start_trunc_imp = imp[:]
imp_warped = adsp.allpass_warp(lambda_,start_trunc_imp) # Warp the impulse
bwp, awp = adsp.prony(imp_warped, b_order, a_order)
zwp = np.roots(bwp) # Compute zeros
pwp = np.roots(awp) # Compute poles
z = (zwp+lambda_)/(1+lambda_*zwp) # Unwarp zeros
p = (pwp+lambda_)/(1+lambda_*pwp) # Unwarp poles
k = 0.02705
sos = signal.zpk2sos(z, p, k) # Compute sos

# Plotting
dirac = np.zeros(len(imp))
dirac[0]=75
iir_resp = signal.sosfilt(sos,dirac)
hanning = np.hanning(2*FIR_size)
short_ir_window = imp[:FIR_size]
iir_resp = np.concatenate([short_ir_window,iir_resp])

w,h_1 = signal.freqz(imp, worN=16000)
w,h_2 = signal.freqz(short_ir_window, worN=16000)
# w,h_3 = signal.freqz(imp_end, worN=16000)
w,h_4 = signal.freqz(iir_resp, worN=16000)
# w,h_5 = signal.freqz(resp, worN=16000)
# h_5 = h_2*h_4

fig,axs = plt.subplots(ncols=2,nrows=2)

axs[0][0].plot(imp/max(imp))
axs[0][0].plot(iir_resp/max(iir_resp))
# ax.plot(np.concatenate([imp_begin,np.zeros(len(imp_end))]))
# ax.plot(iir_resp)

axs[0][1].semilogx(20*np.log10(abs(h_1)/max(h_1)), label="IR")
# ax.semilogx(20*np.log10(abs(h_2/np.max(h_2))), label="FIR approx")
# axs[1].semilogx(20*np.log10(abs(h_3)))
axs[0][1].semilogx(20*np.log10(abs(h_4)/max(h_4)),  label="IIR approx")
# axs[1].semilogx(20*np.log10(abs(h_5)))

axs[1][0].plot(short_ir_window)
axs[1][1].semilogx(20*np.log10(abs(h_1)), label="IR")
axs[1][1].plot(20*np.log10(abs(h_2)))

plt.show()