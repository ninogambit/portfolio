import os
import numpy as np
import librosa
from tqdm import tqdm

SR = 16000
N_FFT = 1024
HOP = 512
N_MELS = 64
W = 2
EPS = 1e-8

BASE = "MIMII/fan/id_06"
OUT = "features_test_id06"
os.makedirs(OUT, exist_ok=True)

norm = np.load("features_id06/norm.npy")

def logmel(w):
    y, _ = librosa.load(w, sr=SR)
    m = librosa.feature.melspectrogram(
        y=y, sr=SR, n_fft=N_FFT,
        hop_length=HOP, n_mels=N_MELS
    )
    return np.log(m + EPS).T

for label in ["normal", "abnormal"]:
    folder = os.path.join(BASE, label)

    for f in tqdm(sorted(os.listdir(folder))):
        spec = logmel(os.path.join(folder, f))
        X, Y = [], []

        for t in range(W, spec.shape[0] - W):
            X.append([spec[t+i] for i in range(-W, W+1) if i != 0])
            Y.append(spec[t])

        X = np.array(X, dtype=np.float32) / norm
        Y = np.array(Y, dtype=np.float32) / norm

        np.save(f"{OUT}/{label}_{f}_X.npy", X)
        np.save(f"{OUT}/{label}_{f}_Y.npy", Y)


