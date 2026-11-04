import os
import numpy as np
import tensorflow as tf
from sklearn.metrics import roc_auc_score

model = tf.keras.models.load_model("model_id00.keras", safe_mode=False)

scores, labels = [], []

DIR = "features_test_id00"

for f in sorted(os.listdir(DIR)):
    if not f.endswith("_X.npy"):
        continue

    X = np.load(os.path.join(DIR, f))
    Y = np.load(os.path.join(DIR, f.replace("_X.npy", "_Y.npy")))

    pred = model.predict(X, verbose=0)
    mse = np.mean((pred - Y) ** 2, axis=1)

    scores.append(np.mean(mse))
    labels.append(0 if f.startswith("normal") else 1)

auc = roc_auc_score(labels, scores)
print("AUC (id_00):", auc)
