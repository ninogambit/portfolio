import os
import numpy as np
import tensorflow as tf
from sklearn.metrics import roc_auc_score, confusion_matrix

model = tf.keras.models.load_model("model_id02.keras")

scores = []
labels = []

DIR = "features_test_id02"

for f in sorted(os.listdir(DIR)):
    if not f.endswith("_X.npy"):
        continue

    X = np.load(os.path.join(DIR, f))
    Y = np.load(os.path.join(DIR, f.replace("_X.npy", "_Y.npy")))

    pred = model.predict(X, verbose=0)

    mse = np.mean((pred - Y) ** 2, axis=1)

    file_score = np.mean(mse)

    scores.append(file_score)
    labels.append(0 if f.startswith("normal") else 1)

scores = np.array(scores)
labels = np.array(labels)

auc = roc_auc_score(labels, scores)
print("AUC:", auc)

def compute_gmean(y_true, scores, threshold):
    y_pred = (scores >= threshold).astype(int)

    tn, fp, fn, tp = confusion_matrix(y_true, y_pred).ravel()

    tpr = tp / (tp + fn + 1e-8)
    tnr = tn / (tn + fp + 1e-8)

    return np.sqrt(tpr * tnr), tn, fp, fn, tp

# Search best threshold
thresholds = np.linspace(scores.min(), scores.max(), 200)

best_gmean = 0
best_threshold = 0
best_stats = None

for t in thresholds:
    g, tn, fp, fn, tp = compute_gmean(labels, scores, t)
    if g > best_gmean:
        best_gmean = g
        best_threshold = t
        best_stats = (tn, fp, fn, tp)

print("Best G-Mean:", best_gmean)
print("Best Threshold:", best_threshold)

tn, fp, fn, tp = best_stats
print("\nConfusion Matrix at Best Threshold:")
print("TN:", tn)
print("FP:", fp)
print("FN:", fn)
print("TP:", tp)


