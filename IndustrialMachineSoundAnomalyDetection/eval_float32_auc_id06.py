import os
import numpy as np
import tensorflow as tf
from sklearn.metrics import roc_auc_score

MODEL = "model_id06_float32.tflite"
DIR = "features_test_id06"

interpreter = tf.lite.Interpreter(model_path=MODEL)
interpreter.allocate_tensors()

input_details = interpreter.get_input_details()[0]
output_details = interpreter.get_output_details()[0]

scores, labels = [], []

for f in sorted(os.listdir(DIR)):
    if not f.endswith("_X.npy"):
        continue

    X = np.load(os.path.join(DIR, f)).astype(np.float32)
    Y = np.load(os.path.join(DIR, f.replace("_X.npy", "_Y.npy"))).astype(np.float32)

    preds = []

    for x in X:
        x = x[None, ...].astype(np.float32)  # (4, 64) -> (1, 4, 64)

        interpreter.set_tensor(input_details["index"], x)
        interpreter.invoke()

        pred = interpreter.get_tensor(output_details["index"])[0]
        preds.append(pred)

    preds = np.array(preds)

    mse = np.mean((preds - Y) ** 2, axis=1)
    file_score = np.mean(mse)

    scores.append(file_score)

    if f.startswith("normal"):
        labels.append(0)
    else:
        labels.append(1)

scores = np.array(scores)
labels = np.array(labels)

auc = roc_auc_score(labels, scores)

print("AUC Float32 TFLite:", auc)

