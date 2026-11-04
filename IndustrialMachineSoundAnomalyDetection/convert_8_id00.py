# convert_int8.py
import numpy as np
import tensorflow as tf

MODEL_PATH = "model_id00.keras"
OUT_PATH = "model_id00_int8.tflite"

X_train = np.load("features_id00/X_train.npy").astype(np.float32)

model = tf.keras.models.load_model(MODEL_PATH, compile=False)

def representative_dataset():
    n = min(300, len(X_train))
    indices = np.random.choice(len(X_train), n, replace=False)

    for i in indices:
        sample = X_train[i:i+1].astype(np.float32)  # shape: (1, 4, 64)
        yield [sample]

converter = tf.lite.TFLiteConverter.from_keras_model(model)

converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_dataset

converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

tflite_model = converter.convert()

with open(OUT_PATH, "wb") as f:
    f.write(tflite_model)

print("INT8 TFLite model saved:", OUT_PATH)
print("Size:", len(tflite_model), "bytes")
