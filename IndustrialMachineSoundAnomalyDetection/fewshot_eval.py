import os
import random
import numpy as np
import tensorflow as tf
from sklearn.metrics import confusion_matrix

MODEL = "model_id06.keras"
DIR = "features_test_id02"

SHOTS = [5, 15, 30]
SEED = 42

model = tf.keras.models.load_model(MODEL, safe_mode=False)

# Calculate one file-level anomaly score
def file_score(x_file, y_file):

    X = np.load(x_file)
    Y = np.load(y_file)

    pred = model.predict(X, verbose=0)

    # MSE for every window
    mse = np.mean((pred - Y) ** 2, axis=1)

    # one anomaly score for entire file
    return np.mean(mse)


# Calculate scores for all ID02 files
files = sorted([
    f for f in os.listdir(DIR)
    if f.endswith("_X.npy")
])

scores = {}
labels = {}

for f in files:

    X_path = os.path.join(DIR, f)
    Y_path = os.path.join(
        DIR,
        f.replace("_X.npy", "_Y.npy")
    )

    scores[f] = file_score(X_path, Y_path)

    if f.startswith("normal"):
        labels[f] = 0
    else:
        labels[f] = 1


normal_files = [f for f in files if labels[f] == 0]
abnormal_files = [f for f in files if labels[f] == 1]

# Evaluate 5, 15 and 30 shots
for shots in SHOTS:

    rng = random.Random(SEED + shots)

    # Random healthy ID02 calibration files
    support_files = rng.sample(normal_files, shots)

    support_scores = np.array([
        scores[f] for f in support_files
    ])

    # Few-shot threshold
    # ONLY NORMAL SUPPORT FILES ARE USED HERE

    t1 = (
        np.mean(support_scores)
        + 3.0 * np.std(support_scores)
    )

    t2 = (
        np.max(support_scores) * 1.10
        + 0.0001
    )

    threshold = max(t1, t2)


    # Evaluation set
    # Remove calibration files

    eval_normal = [
        f for f in normal_files
        if f not in support_files
    ]

    eval_files = eval_normal + abnormal_files

    y_true = np.array([
        labels[f] for f in eval_files
    ])

    y_score = np.array([
        scores[f] for f in eval_files
    ])

    y_pred = (y_score >= threshold).astype(int)


    # G-Mean

    tn, fp, fn, tp = confusion_matrix(
        y_true,
        y_pred
    ).ravel()

    sensitivity = tp / (tp + fn)
    specificity = tn / (tn + fp)

    gmean = np.sqrt(
        sensitivity * specificity
    )


    # Results

    print("\n==============================")
    print("Shots:", shots)

    print("Few-shot threshold:", threshold)

    print("Sensitivity:", sensitivity)
    print("Specificity:", specificity)

    print("G-Mean:", gmean)

    print("TN:", tn)
    print("FP:", fp)
    print("FN:", fn)
    print("TP:", tp)
