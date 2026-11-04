import os
import numpy as np
import librosa
from tqdm import tqdm

# PARAMETERS
SR = 16000
N_FFT = 1024
HOP = 512
N_MELS = 64
W = 2
EPS = 1e-8

DATA_DIR = "MIMII/fan/id_06/normal"
OUT_DIR = "features_id06"
os.makedirs(OUT_DIR, exist_ok=True)

# FUNCTIONS
def logmel(wav):
    y, _ = librosa.load(wav, sr=SR)
    m = librosa.feature.melspectrogram(
        y=y, sr=SR, n_fft=N_FFT,
        hop_length=HOP, n_mels=N_MELS
    )
    return np.log(m + EPS).T

# EXTRACTION
X, Y = [], []

for f in tqdm(sorted(os.listdir(DATA_DIR))):
    spec = logmel(os.path.join(DATA_DIR, f))
    T = spec.shape[0]

    for t in range(W, T - W):
        context = [spec[t+i] for i in range(-W, W+1) if i != 0]
        X.append(context)
        Y.append(spec[t])

X = np.array(X, dtype=np.float32)
Y = np.array(Y, dtype=np.float32)

# NORMALIZATION
max_val = np.max(np.abs(X))
X /= max_val
Y /= max_val

np.save(f"{OUT_DIR}/X_train.npy", X)
np.save(f"{OUT_DIR}/Y_train.npy", Y)
np.save(f"{OUT_DIR}/norm.npy", max_val)

print("Training data:", X.shape, Y.shape)


