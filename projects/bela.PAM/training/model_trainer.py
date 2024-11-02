import argparse
import subprocess
from pathlib import Path

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

    args = parser.parse_args()

    Path('Results').mkdir(parents=True, exist_ok=True)
    Path('Data/train').mkdir(parents=True, exist_ok=True)
    Path('Data/val').mkdir(parents=True, exist_ok=True)
    Path('Data/test').mkdir(parents=True, exist_ok=True)
    Path('TensorboardData').mkdir(parents=True, exist_ok=True)

    subprocess.run(["python3", "prep_wav.py", args.name, "-s", args.clean_audio_file_path, args.target_audio_file_path, "--normalize", "true"])
    subprocess.run(["python3", "dist_model_recnet.py", "-l", "RNN3-"+args.name, "-eps", args.epoch])
    if args.header:
        subprocess.run(["python3", "convert_json_to_c_header.py", "Results/"+args.name+"-RNN3-"+args.name+"/model_best.json"])

