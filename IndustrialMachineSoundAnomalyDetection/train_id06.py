import numpy as np
import tensorflow as tf
from hybrid_model import build_model

# Load features
X = np.load("features_id06/X_train.npy")
Y = np.load("features_id06/Y_train.npy")

# Build model
model = build_model(input_shape=X.shape[1:])  # Dynamically get input shape

# Compile model
model.compile(
    optimizer=tf.keras.optimizers.Adam(1e-3),
    loss="mse"
)

# Show model summary
model.summary()

# Train
history = model.fit(
    X, Y,
    validation_split=0.1,
    epochs=30,
    batch_size=16,
    shuffle=True
)

# Save trained model
model.save("model_id06.keras")

import matplotlib.pyplot as plt

plt.figure(figsize=(10, 5))
plt.plot(history.history['loss'], label='Training Loss')
plt.plot(history.history['val_loss'], label='Validation Loss')
plt.title('Training and Validation Loss')
plt.xlabel('Epoch')
plt.ylabel('MSE Loss')
plt.legend()
plt.grid(True)
plt.show()


