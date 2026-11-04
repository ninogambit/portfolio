# convert_float32.py
import tensorflow as tf

MODEL_PATH = "model_id06.keras"   
OUT_PATH = "model_id06_float32.tflite"

model = tf.keras.models.load_model(MODEL_PATH, compile=False)

converter = tf.lite.TFLiteConverter.from_keras_model(model)

# Try to keep only normal TFLite ops for microcontroller compatibility
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS]

tflite_model = converter.convert()

with open(OUT_PATH, "wb") as f:
    f.write(tflite_model)

print("Float32 TFLite model saved:", OUT_PATH)
print("Size:", len(tflite_model), "bytes")
