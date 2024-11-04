# Script details
# 1- transpile FAUST code to cpp
# 2- Update audiolab cpp lib to bela
# 3- copy project code to bela, don't include FAUST code.
# 4- If possible, compile and run the code on Bela.
import subprocess
from pathlib import Path
from fabric import Connection, task

# Define paths and constants
PROJECT_ROOT = Path('.')
DSP_FOLDER = PROJECT_ROOT / 'dsp'
SOURCE_FOLDER = PROJECT_ROOT / 'sources'
BELA_IP = 'bela.local'
BELA_USER = 'root'
BELA_LIBRARY_PATH = '/libraries/audiolab'
BELA_PROJECT_PATH = '/project/bela.PAM'

# Function to transpile FAUST code
def transpile_faust():
    for faust_file in DSP_FOLDER.glob('*.dsp'):
        cpp_path = SOURCE_FOLDER / faust_file.with_suffix('.cpp').name
        subprocess.run(['faust', '-o', str(cpp_path), str(faust_file)])

# Function to update audiolab library on Bela
@task
def update_audiolab_lib(c):
    # Check if the audiolab folder exists, create it if it doesn't
    c.run(f'mkdir -p {BELA_LIBRARY_PATH}')
    c.put(str(PROJECT_ROOT / 'audiolab'), BELA_LIBRARY_PATH, mirror_local_mode=True)

# Function to copy project code to Bela, excluding FAUST code
@task
def copy_project_code(c):
    for file in PROJECT_ROOT.rglob('*'):
        if DSP_FOLDER in file.parents or file.is_dir():
            continue

        relative_path = file.relative_to(PROJECT_ROOT)
        remote_path = Path(BELA_PROJECT_PATH) / relative_path

        remote_dir = remote_path.parent
        c.run(f'mkdir -p {remote_dir}')
        c.put(str(file), str(remote_path), mirror_local_mode=True)

# Main function to execute the tasks
def main():
    transpile_faust()

    # Establish connection to Bela
    c = Connection(host=BELA_IP, user=BELA_USER)

    # Execute tasks
    update_audiolab_lib(c)
    copy_project_code(c)

if __name__ == '__main__':
    main()
