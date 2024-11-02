import soundfile as sf
import numpy as np
import argparse
import random

class DataProcessor:
    def __init__(self, sample_rate=44100):
        self.sample_rate = sample_rate

    def save_wav(self, name, data):
        sf.write(name, data.flatten().astype(np.float32), self.sample_rate)

    def normalize(self, data):
        data_max = max(data)
        data_min = min(data)
        data_norm = max(data_max,abs(data_min))
        if data_norm == 0:
            print("[WARNING]: Audio file appears to contain all 0's, indicating a completely silent wav file. Check your out.wav file.")
        return data / data_norm

    def load_and_preprocess(self, file, normalize_flag):
        data, rate = sf.read(file)

        if len(data.shape) > 1:
            print(f"[WARNING] Stereo data detected for {file}, only using first channel (left channel)")
            data = data[:,0]

        data = data.astype(np.float32).flatten()

        if normalize_flag:
            data = self.normalize(data).reshape(len(data),1)

        return data, rate

    def split_data(self, clean_data, target_data, random_split, mod_split):
        if random_split:
            return self.sliceRandomPercentage(clean_data, target_data, random_split)
        else:
            return self.sliceOnMod(clean_data, target_data, mod_split)

    def sliceOnMod(self, input_data, target_data, mod):
        # Split the data on a modulus.

        # type cast to an integer the modulus
        mod = int(mod)

        # Split the data into 100 pieces
        input_split = np.array_split(input_data, 100)
        target_split = np.array_split(target_data, 100)

        val_input_data = []
        val_target_data = []
        # Traverse the range of the indexes of the input signal reversed and pop every 5th for val
        for i in reversed(range(len(input_split))):
            if i%mod == 0:
                # Store the validation data
                val_input_data.append(input_split[i])
                val_target_data.append(target_split[i])
                # Remove the validation data from training
                input_split.pop(i)
                target_split.pop(i)

        # Flatten val_data down to one dimension and concatenate
        val_input_data = np.concatenate(val_input_data)
        val_target_data = np.concatenate(val_target_data)

        # Concatinate b back together
        training_input_data = np.concatenate(input_split)
        training_target_data = np.concatenate(target_split)
        return (training_input_data, training_target_data, val_input_data, val_target_data)

    def sliceRandomPercentage(self, input_data, target_data, percentage):
        # Do a random split of the data by slicing the data into 100 pieces and randomly 
        # chosing some number of them.

        if percentage < 0 and percentage > 100:
            raise ValueError("Perentage must be between 0 and 100")

        # Split the data into 100 pieces
        input_split = np.array_split(input_data, 100)
        target_split = np.array_split(target_data, 100)
        validationChunks = int(percentage)

        # Skip the first entry because it has a different array size if the array can't be 
        # devided evenly which screws things
        selection = random.sample(range(1,99), validationChunks)

        val_input_data = []
        val_target_data = []
        # Store the randomly selected values in C
        for i, val in enumerate(selection):
            val_input_data.append(input_split[val])
            val_target_data.append(target_split[val])

        # Flatten val_data down to one dimension and concatenate
        val_input_data = np.concatenate(val_input_data)
        val_target_data = np.concatenate(val_target_data)

        # remove the validation selections from b
        for i in sorted(selection, reverse=True):
            input_split.pop(i)
            target_split.pop(i)

        # Concatinate b back together
        training_input_data = np.concatenate(input_split)
        training_target_data = np.concatenate(target_split)
        return (training_input_data, training_target_data, val_input_data, val_target_data)


    def process(self, args):
        # Load and Preprocess Data
        in_data, in_rate = self.load_and_preprocess(args.snapshot[0], args.normalize)
        out_data, out_rate = self.load_and_preprocess(args.snapshot[1], args.normalize)

        if out_rate != args.samplerate:
            print("\n\n\n[ERROR] The out.wav file has an invalid samplerate (NeuralSeed / Daisy Seed uses 48kHz) " +"("+ str(out_rate) +")")
            print("[ERROR] Please re-export your wav file as 48000 samplerate (48kHz).\n\n\n")
            return

        if out_data.dtype != "int16" and out_data.dtype != "float32":
            print("\n\n\n[ERROR] The out.wav file has an invalid data type " +"("+ str(out_data.dtype) +")")
            print("[ERROR] Please re-export your wav file as either PCM16 or FP32 data type (bit depth).\n\n\n")
            return

        # Trim the length of audio to equal the smaller wav file
        if len(in_data) > len(out_data):
            print("Trimming input audio to match output audio")
            in_data = in_data[0:len(out_data)]
        if len(out_data) > len(in_data):
            print("Trimming output audio to match input audio")
            out_data = out_data[0:len(in_data)]

        clean_data = in_data.astype(np.float32).flatten()
        target_data = out_data.astype(np.float32).flatten()

        if args.random_split:
            in_train, out_train, in_val, out_val = self.sliceRandomPercentage(clean_data, target_data, args.random_split)
        else:
            # Split the data on a twenty percent mod
            in_train, out_train, in_val, out_val = self.sliceOnMod(clean_data, target_data, args.mod_split)

        # If separate test files are given process them.
        # Else, use validation data sets.
        if (len(args.snapshot) == 4):
            test_in_data, test_in_rate = self.load_and_preprocess(args.snapshot[2], args.normalize)
            test_out_data, test_out_rate = self.load_and_preprocess(args.snapshot[3], args.normalize)
            in_test = test_in_data.astype(np.float32).flatten()
            out_test = test_out_data.astype(np.float32).flatten()

            if (args.normalize):
                in_test = self.normalize(in_test).reshape(len(test_in_data),1)
                out_test = self.normalize(out_test).reshape(len(test_out_data),1)
        else:
            # It is NOT advisable to use the same data set for testing and validation
            # This gives a strong bias in evaluating the effectiveness of the training
            print("The test set and validation set are the same.")
            in_test = in_val
            out_test = out_val

        self.save_wav(args.path + "/train/" + args.name + "-input.wav", in_train)
        self.save_wav(args.path + "/train/" + args.name + "-target.wav", out_train)

        self.save_wav(args.path + "/test/" + args.name + "-input.wav", in_test)
        self.save_wav(args.path + "/test/" + args.name + "-target.wav", out_test)

        self.save_wav(args.path + "/val/" + args.name + "-input.wav", in_val)
        self.save_wav(args.path + "/val/" + args.name + "-target.wav", out_val)





if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='''This script prepairs the data data to be trained'''
    )
    parser.add_argument("name")
    parser.add_argument("--samplerate", "-sr", type=int, default=44100, help="Define the sample rate of the processing. Default is 44100.")
    parser.add_argument("--snapshot", "-s", nargs="+", help="Snapshot configuration. TRAINING_IN TRAINING_OUT OPTIONAL_TEST_IN OPTIONAL_TEST_OUT")
    parser.add_argument("--normalize", "-n", type=bool, default=False)
    parser.add_argument("--mod_split", '-ms', default=5, help="The default splitting mechanism. Splits the training and validation data on a 5 mod (or 20 percent).")
    parser.add_argument("--random_split", '-rs', type=float, default=None, help="By default, the training is split on a modulus. However, desingnating a percentage between 0 and 100 will create a random data split between the training and validatation sets.")
    parser.add_argument("--path", type=str, default="Data", help="Path to store the processed data.")

    args = parser.parse_args()

    dp = DataProcessor(args.samplerate)
    dp.process(args)
