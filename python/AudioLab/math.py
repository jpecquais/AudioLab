import numpy as np

def a2db(x):
    return 20*np.log10(np.abs(x))