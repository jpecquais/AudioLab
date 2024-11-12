import numpy as np
import soundfile as sf
import argparse
import sys
from matplotlib import pyplot as plt
from scipy import signal
from pathlib import Path

# Get the current script's directory
current_dir = Path(__file__).resolve().parent

# Construct the path to the directory two levels above
parent_dir = current_dir.parent.parent

# Add the parent directory to the system path
sys.path.append(str(parent_dir))

# Now you can import the script as a module
from AudioLab import math

FFT_SIZE = 8192*2
SR = 48000

fig, axs = plt.subplots(ncols=2)

def ir_trunc(args):
    file = Path(args.file)
    ir = sf.read(file)
    ir_data = np.array(ir[0])
    ir_data = np.transpose(ir_data)

    max_ir_len = len(ir_data)
    number_iteration = 4

    for i in range(number_iteration):
        ir_len = max_ir_len//np.power(2,i)
        _ir =  ir_data[:ir_len]#*np.hanning(2*ir_len)[ir_len:]
        w, h = signal.freqz(_ir, fs=SR, worN=FFT_SIZE)
        line, = axs[0].semilogx(w,math.a2db(h))
        line.set_label(str(ir_len))
        line, = axs[1].plot(math.a2db(_ir))
        line.set_label(str(ir_len))


    

def main():
    parser = argparse.ArgumentParser(
        description='''This script studies the effect of IR truncature on its frequency response.'''
    )
    parser.add_argument("file")
    args = parser.parse_args()

    ir_trunc(args)
    axs[0].legend()
    axs[1].legend()
    axs[0].set_xlim(80,20000)
    plt.show()
    print("done")

if __name__ == "__main__":
    main()