import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

def pdtf2tf(pd_coef):
    # Step 1: Multiply the first two elements by -1
    first_two_negated = [-pd_coef[0], -pd_coef[1]]

    # Step 2: Remove the first two elements
    del pd_coef[0:2]

    # Step 3: Insert "1" at the third index
    pd_coef.append(1)

    # Step 4: Append the modified first two elements to the end
    pd_coef.extend(first_two_negated)

    b = pd_coef[0:3]
    a = pd_coef[3:6]
    return b, a

def tf2pdtf(b,a):
    pd_coef = []
    pd_coef.append(-a[1])
    pd_coef.append(-a[2])
    pd_coef.extend(b)
    return pd_coef

pd_list = [1.75437, -0.767787, 0.00335511, 0.00671022, 0.00335511]

b,a = pdtf2tf(pd_list)
w, h = signal.freqz(b,a)

new_list = tf2pdtf(b,a)
print(new_list)

fig, ax = plt.subplots()
ax.semilogx(20*np.log10(abs(h)))
plt.show()