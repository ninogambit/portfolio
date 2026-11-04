import os
import numpy as np
import tensorflow as tf
from sklearn.metrics import roc_auc_score

MODEL = "model_id06_int8.tflite"
DIR = "features_test_id06"

interpreter = tf.lite.Interpreter(model_path=MODEL)
interpreter.allocate_tensors()

input_details = interpreter.get_input_details()[0]
output_details = interpreter.get_output_details()[0]

input_scale, input_zero = input_details["quantization"]
output_scale, output_zero = output_details["quantization"]

scores, labels = [], []

for f in sorted(os.listdir(DIR)):
    if not f.endswith("_X.npy"):
        continue

    X = np.load(os.path.join(DIR, f)).astype(np.float32)
    Y = np.load(os.path.join(DIR, f.replace("_X.npy", "_Y.npy"))).astype(np.float32)

    preds = []

    for x in X:
        x = x[None, ...].astype(np.float32)  # (4, 64) -> (1, 4, 64)

        # Quantize input: float32 -> int8
        x_q = np.round(x / input_scale + input_zero)
        x_q = np.clip(x_q, -128, 127).astype(np.int8)

        interpreter.set_tensor(input_details["index"], x_q)
        interpreter.invoke()

        pred_q = interpreter.get_tensor(output_details["index"])

        pred = (pred_q.astype(np.float32) - output_zero) * output_scale

        preds.append(pred[0])

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

print("AUC INT8 TFLite:", auc)
