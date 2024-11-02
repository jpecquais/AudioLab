# Model training

This folder stores a modified copy of [Automated-GuitarAmpModelling](https://github.com/GuitarML/Automated-GuitarAmpModelling.git). It has been slightly modified.

+ No parametric model, as I have no use for them at this point
+ Object based refactoring of the "prep_wav.py"
+ Additional `model_trainer.py` that act as a helper script.
+ A `requirements.txt` that should work.

The global motivation for this duplication is to be able to easily reproduce the training conditions. There are a few things that I'd like to change in the future, but at this point, my own input into this code is almost zero.

If the helper script is too simplified for your use case, you can still call the `prep_wav.py` script, then the `dist_model_recnet.py` script.

## Model Training Script

The `model_trainer.py` is designed to train a model based on clean and target audio files. It utilizes several command-line arguments to specify the input files, sample rate, number of epochs, and whether to generate a header file from the model.

### Usage

```bash
python3 script_name.py name --clean_audio_file_path CLEAN_AUDIO_FILE_PATH --target_audio_file_path TARGET_AUDIO_FILE_PATH --samplerate SAMPLE_RATE --epoch EPOCH --header HEADER
```

### Arguments

- `name`: The name of the model.
- `--clean_audio_file_path`, `-cpath`: Path to the clean audio file.
- `--target_audio_file_path`, `-tpath`: Path to the target audio file.
- `--samplerate`, `-sr`: Sample rate of the audio files.
- `--epoch`, `-e`: Number of epochs for training. Default is 150.
- `--header`, `-c`: Generate a `.h` header file from the model. Default is `True`.

### Example

```bash
python3 script_name.py my_model --clean_audio_file_path /path/to/clean_audio.wav --target_audio_file_path /path/to/target_audio.wav --samplerate 44100 --epoch 200 --header True
```

### Directory Structure

The script creates the following directory structure:

```
Results/
Data/
  train/
  val/
  test/
TensorboardData/
```

### Workflow

1. **Prepare WAV Files**: The script runs `prep_wav.py` to prepare the audio files.
   ```bash
   python3 prep_wav.py name -s clean_audio_file_path target_audio_file_path --normalize true
   ```

2. **Train the Model**: The script runs `dist_model_recnet.py` to train the model.
   ```bash
   python3 dist_model_recnet.py -l RNN3-name -eps epoch
   ```

3. **Generate Header File**: If the `--header` flag is set to `True`, the script runs `convert_json_to_c_header.py` to generate a `.h` header file from the model.
   ```bash
   python3 convert_json_to_c_header.py Results/name-RNN3-name/model_best.json
   ```

### Notes

- Ensure that the necessary scripts (`prep_wav.py`, `dist_model_recnet.py`, `convert_json_to_c_header.py`) are in the same directory as this script or are accessible via the system's PATH.
- The script uses `subprocess.run` to execute the commands, which means it will wait for each command to complete before proceeding to the next one.

## Data Preparation Script

This script prepares audio data for training by loading, preprocessing, and splitting the data into training, validation, and test sets. It utilizes the `soundfile` library for reading and writing audio files and `numpy` for numerical operations.

### Usage

```bash
python3 script_name.py name --samplerate SAMPLE_RATE --snapshot TRAINING_IN TRAINING_OUT OPTIONAL_TEST_IN OPTIONAL_TEST_OUT --normalize NORMALIZE --mod_split MOD_SPLIT --random_split RANDOM_SPLIT --path PATH
```

### Arguments

- `name`: The name of the dataset.
- `--samplerate`, `-sr`: Sample rate of the processing. Default is 44100.
- `--snapshot`, `-s`: Snapshot configuration. Specify the paths to the training input and output files, and optionally the test input and output files.
- `--normalize`, `-n`: Normalize the audio data. Default is `False`.
- `--mod_split`, `-ms`: The default splitting mechanism. Splits the training and validation data on a 5 mod (or 20 percent). Default is 5.
- `--random_split`, `-rs`: By default, the training is split on a modulus. However, designating a percentage between 0 and 100 will create a random data split between the training and validation sets.
- `--path`: Path to store the processed data. Default is "Data".

### Example

```bash
python3 script_name.py my_dataset --samplerate 44100 --snapshot /path/to/train_in.wav /path/to/train_out.wav /path/to/test_in.wav /path/to/test_out.wav --normalize True --mod_split 5 --random_split 20 --path /path/to/data
```

### Workflow

1. **Load and Preprocess Data**: The script loads and preprocesses the audio files specified in the `--snapshot` argument.
2. **Normalize Data**: If the `--normalize` flag is set to `True`, the script normalizes the audio data.
3. **Split Data**: The script splits the data into training, validation, and test sets based on the `--mod_split` or `--random_split` arguments.
4. **Save WAV Files**: The script saves the processed audio data as WAV files in the specified directories.

### Notes

- Ensure that the necessary libraries (`soundfile`, `numpy`) are installed.
- The script uses `argparse` for command-line argument parsing.
- The script handles stereo data by using only the first channel (left channel).
- The script checks for valid sample rates and data types in the output audio file.


## Neural Network Training Script

This script implements training for neural network amplifier/distortion effects modeling.

### Usage

```bash
python3 script_name.py --device DEVICE --data_location DATA_LOCATION --file_name FILE_NAME --load_config LOAD_CONFIG --config_location CONFIG_LOCATION --save_location SAVE_LOCATION --load_model LOAD_MODEL --seed SEED --segment_length SEGMENT_LENGTH --epochs EPOCHS --validation_f VALIDATION_F --validation_p VALIDATION_P --batch_size BATCH_SIZE --iter_num ITER_NUM --learn_rate LEARN_RATE --init_len INIT_LEN --up_fr UP_FR --cuda CUDA --loss_fcns LOSS_FCNS --pre_filt PRE_FILT --val_chunk VAL_CHUNK --test_chunk TEST_CHUNK --model MODEL --input_size INPUT_SIZE --output_size OUTPUT_SIZE --num_blocks NUM_BLOCKS --hidden_size HIDDEN_SIZE --unit_type UNIT_TYPE --skip_con SKIP_CON
```

### Arguments

- `--device`, `-p`: Label describing the device being modeled. Default is `ht1`.
- `--data_location`, `-dl`: Location of the "Data" directory. Default is `..`.
- `--file_name`, `-fn`: Filename of the WAV file to be loaded as the input/target data. Default is `ht1`.
- `--load_config`, `-l`: File path to a JSON config file. Arguments listed in the config file will replace the defaults. Default is `RNN3`.
- `--config_location`, `-cl`: Location of the "Configs" directory. Default is `Configs`.
- `--save_location`, `-sloc`: Directory where trained models will be saved. Default is `Results`.
- `--load_model`, `-lm`: Load a pretrained model if it is found. Default is `True`.
- `--seed`: Seed all of the random number generators if desired.
- `--segment_length`, `-slen`: Training audio segment length in samples. Default is 22050.
- `--epochs`, `-eps`: Max number of training epochs to run. Default is 2000.
- `--validation_f`, `-vfr`: Validation frequency (in epochs). Default is 2.
- `--validation_p`, `-vp`: How many validations without improvement before stopping training. Default is 25.
- `--batch_size`, `-bs`: Training mini-batch size. Default is 50.
- `--iter_num`, `-it`: Overrides `--batch_size` and instead sets the batch size so that a total of `--iter_num` batches are processed in each epoch.
- `--learn_rate`, `-lr`: Initial learning rate. Default is 0.005.
- `--init_len`, `-il`: Number of sequence samples to process before starting weight updates. Default is 200.
- `--up_fr`, `-uf`: For recurrent models, number of samples to run in between updating network weights. Default is 1000.
- `--cuda`, `-cu`: Use GPU if available. Default is 1.
- `--loss_fcns`, `-lf`: Which loss functions to use (ESR, ESRPre, DC). Argument is a dictionary with each key representing a loss function name and the corresponding value being the multiplication factor applied to that loss function. Default is `{'ESRPre': 0.75, 'DC': 0.25}`.
- `--pre_filt`, `-pf`: FIR filter coefficients for pre-emphasis filter. Can also read in a CSV file. Default is `high_pass`.
- `--val_chunk`, `-vs`: Number of sequence samples to process in each chunk of validation. Default is 100000.
- `--test_chunk`, `-tc`: Number of sequence samples to process in each chunk of validation. Default is 100000.
- `--model`, `-m`: Model architecture. Default is `SimpleRNN`.
- `--input_size`, `-is`: 1 for mono input data, 2 for stereo, etc. Default is 1.
- `--output_size`, `-os`: 1 for mono output data, 2 for stereo, etc. Default is 1.
- `--num_blocks`, `-nb`: Number of recurrent blocks. Default is 1.
- `--hidden_size`, `-hs`: Recurrent unit hidden state size. Default is 16.
- `--unit_type`, `-ut`: LSTM or GRU or RNN. Default is `LSTM`.
- `--skip_con`, `-sc`: Is there a skip connection for the input to the output. Default is 1.

### Example

```bash
python3 script_name.py --device ht1 --data_location .. --file_name ht1 --load_config RNN3 --config_location Configs --save_location Results --load_model True --seed 42 --segment_length 22050 --epochs 2000 --validation_f 2 --validation_p 25 --batch_size 50 --iter_num None --learn_rate 0.005 --init_len 200 --up_fr 1000 --cuda 1 --loss_fcns '{"ESRPre": 0.75, "DC": 0.25}' --pre_filt high_pass --val_chunk 100000 --test_chunk 100000 --model SimpleRNN --input_size 1 --output_size 1 --num_blocks 1 --hidden_size 16 --unit_type LSTM --skip_con 1
```

### Directory Structure

The script creates the following directory structure:

```
TensorboardData/
```

### Workflow

1. **Initialize Model**: The script initializes the model by either loading an existing model or creating a new one based on the provided arguments.
2. **Load Dataset**: The script loads the dataset from the specified directory.
3. **Train Model**: The script trains the model using the specified parameters and saves the model and training statistics.
4. **Validate Model**: The script validates the model at specified intervals and saves the best model based on validation loss.
5. **Test Model**: The script tests the final model and the best model based on validation loss.

### Notes

- Ensure that the necessary libraries (`CoreAudioML`, `numpy`, `random`, `torch`, `torch.optim`, `torch.utils.tensorboard`, `argparse`, `time`, `os`, `csv`, `scipy.io.wavfile`) are installed.
- The script uses `argparse` for command-line argument parsing.
- The script handles CUDA device availability and uses GPU if available.
- The script saves training statistics and model weights in the specified save location.

<!-- 
HIDDEN NOTES:
Just in case :
Prepare .wav : python3 ./prep_wav.py "bela" -s input-44100.wav laney-poweramp-p5-deconvolved-32FP-44100.wav --normalize true

train model : python3 dist_model_recnet.py -l "RNN3-bela" -eps 150 ///(150 seems enough)
-->