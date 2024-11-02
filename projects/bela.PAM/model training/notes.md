Prepare .wav : python3 ./prep_wav.py "bela" -s input-44100.wav laney-poweramp-p5-deconvolved-32FP-44100.wav --normalize true

train model : python3 dist_model_recnet.py -l "RNN3-bela" -eps 150 ///(150 seems enough)