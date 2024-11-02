import argparse
import subprocess

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='''This script trained a model based on clean and target audio files.'''
    )
    parser.add_argument("--clean_audio_file_path","-cpath",type=str,help="Clean audio file path")
    parser.add_argument("--target_audio_file_path","-tpath",type=str,help="Target audio file path")
    parser.add_argument("--samplerate","-sr",type=int,help="Sample rate")

    args = parser.parse_args()

    subprocess.run(["python3", "prep_wav.py", "bela", "-s", args.clean_audio_file_path, args.target_audio_file_path, "--normalize", "true"])
    subprocess.run(["python3", "dist_model_recnet.py", "-l", "RNN3-bela", "-eps", "150"])