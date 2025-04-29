# Script details
# 1- transpile FAUST code to cpp
# 2- Update audiolab cpp lib to bela
# 3- copy project code to bela, don't include FAUST code.
# 4- If possible, compile and run the code on Bela.
import subprocess
from pathlib import Path
from fabric import Connection, task

# Define paths and constants
PROJECT_ROOT = Path(__file__)
DSP_FOLDER = PROJECT_ROOT / 'dsp'
SOURCE_FOLDER = PROJECT_ROOT / 'sources'
LIBRARY_FOLDER = PROJECT_ROOT/'../../cpp'
BELA_IP = '192.168.7.2'
BELA_USER = 'root'
BELA_LIBRARY_PATH = 'Bela/libraries/AudioLab'
BELA_PROJECT_PATH = 'Bela/projects/bela.PAM'

def send_folder_to_bela(folder_path, destination_path, c, exclude=None):
    exclude_options = ''
    if exclude:
        exclude_options = ' '.join(f"--exclude={pattern}" for pattern in exclude)
    rsync_command = f"rsync -avz {exclude_options} {folder_path}/ {BELA_USER}@{BELA_IP}:{destination_path}"
    c.local(rsync_command)


# Function to transpile FAUST code
def transpile_faust():
    for faust_file in DSP_FOLDER.glob('*.dsp'):
        cpp_path = SOURCE_FOLDER / faust_file.with_suffix('.h').name
        subprocess.run(['faust', '-a', '../../faust/tools/faustMinimalInlined.h', '-o', str(cpp_path), "-cn", faust_file.stem, str(faust_file)])

# Function to update audiolab library on Bela
@task
def update_audiolab_lib(c):
    # Check if the audiolab folder exists, create it if it doesn't
    send_folder_to_bela(LIBRARY_FOLDER,BELA_LIBRARY_PATH,c)

# Function to copy project code to Bela, excluding FAUST code
@task
def copy_project_code(c):
    exclude_list = ['.venv', 'dsp', 'training']  # Add more patterns as needed 
    send_folder_to_bela(PROJECT_ROOT, BELA_PROJECT_PATH, c, exclude=exclude_list)

# Main function to execute the tasks
def main():
    transpile_faust()

    # Establish connection to Bela
    c = Connection(host=BELA_IP, user=BELA_USER, port = 22, connect_kwargs={'password': ''})

    if not c.run(f'test -d {BELA_LIBRARY_PATH}', warn=True).ok:
        c.run(f'mkdir -p {BELA_LIBRARY_PATH}')
    else:
        c.run(f'rm -rf {BELA_LIBRARY_PATH}')
        c.run(f'mkdir -p {BELA_LIBRARY_PATH}')

    if not c.run(f'test -d {BELA_PROJECT_PATH}', warn=True).ok:
        c.run(f'mkdir -p {BELA_PROJECT_PATH}')
    else:
        c.run(f'rm -rf {BELA_PROJECT_PATH}')
        c.run(f'mkdir -p {BELA_PROJECT_PATH}')

    # Execute tasks
    update_audiolab_lib(c)
    copy_project_code(c)

if __name__ == '__main__':
    main()
