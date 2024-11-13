import argparse
import subprocess
import soundfile as sf
import numpy as np
from pathlib import Path
from scipy import signal

def convolve(sig, ir):
    return signal.lfilter(ir,1,x=sig)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='''This script trained a model based on clean and target audio files.'''
    )
    parser.add_argument("name")
    parser.add_argument("--clean_audio_file_path","-cpath",type=str,help="Clean audio file path")
    parser.add_argument("--target_audio_file_path","-tpath",type=str,help="Target audio file path")
    parser.add_argument("--samplerate","-sr",type=int,help="Sample rate")
    parser.add_argument("--epoch","-e",type=str,default='150',help="Number of Epoch (default is 150)")
    parser.add_argument("--header", "-c", type=bool, default=True, help="Generate a .h header file from the model.")
    parser.add_argument("--convolve", type=str, default=None, help="Convolve the target file by an impulse response.")
    args = parser.parse_args()

    target_file_extname = ''

    if args.convolve:
        target_file_path = Path(args.target_audio_file_path)
        audio = sf.read(target_file_path)
        audio_data = np.transpose(audio[0])

        ir = sf.read(args.convolve)
        ir_data = np.transpose(ir[0])

        audio_data = convolve(audio_data, ir_data)
        target_file_newpath = target_file_path.parent / (target_file_path.stem + '-convolved.wav')
        sf.write(target_file_newpath, audio_data, args.samplerate)

    Path('Results').mkdir(parents=True, exist_ok=True)
    Path('Data/train').mkdir(parents=True, exist_ok=True)
    Path('Data/val').mkdir(parents=True, exist_ok=True)
    Path('Data/test').mkdir(parents=True, exist_ok=True)
    Path('TensorboardData').mkdir(parents=True, exist_ok=True)

    subprocess.run(["python3", "prep_wav.py", args.name, "-s", args.clean_audio_file_path, args.target_audio_file_path+target_file_extname, "--normalize", "true"])
    subprocess.run(["python3", "dist_model_recnet.py", "-l", "RNN3-"+args.name, "-eps", args.epoch])
    if args.header:
        subprocess.run(["python3", "convert_json_to_c_header.py", "Results/"+args.name+"-RNN3-"+args.name+"/model_best.json"])