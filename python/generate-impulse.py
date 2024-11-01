import numpy as np
from matplotlib import pyplot as plt
from scipy import signal
import soundfile as sf

impulse = np.zeros(4096)
impulse[0] = 1

sf.write("impulse.wav",impulse,samplerate=48000)