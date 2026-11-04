import tensorflow as tf
from tensorflow.keras import layers, models

def build_model(input_shape=(4, 64)):
    input = layers.Input(shape=input_shape)

    # Flatten input once for all FC branches (global view)
    flat = layers.Flatten()(input)
    
    # Time steps 
    time_steps = input_shape[0]  

    # CNN encoder with multi-scale fusion at each level
    # Level 1
    h1 = layers.Conv1D(16, 3, padding="same", activation="relu")(input)
    fc1 = layers.Dense(16 * time_steps, activation="relu")(flat)
    fc1_rep = layers.Reshape((time_steps, 16))(fc1)
    f1 = layers.Concatenate(axis=-1)([h1, fc1_rep])

    # Level 2 (feed fused f1 to next conv)
    h2 = layers.Conv1D(32, 3, padding="same", activation="relu")(f1)
    fc2 = layers.Dense(32 * time_steps, activation="relu")(flat)
    fc2_rep = layers.Reshape((time_steps, 32))(fc2)
    f2 = layers.Concatenate(axis=-1)([h2, fc2_rep])

    # Level 3
    h3 = layers.Conv1D(32, 2, padding="same", activation="relu")(f2)
    fc3 = layers.Dense(32 * time_steps, activation="relu")(flat)
    fc3_rep = layers.Reshape((time_steps, 32))(fc3)
    f3 = layers.Concatenate(axis=-1)([h3, fc3_rep])

    # Multi-head attention on f3
    proj = layers.Dense(32)(f3)
    attention = layers.MultiHeadAttention(
        num_heads=4, key_dim=8
    )(proj, proj)

    # Bottleneck latent z
    z = layers.GlobalAveragePooling1D()(attention)

    # Decoder to predict center frame
    x = layers.Dense(64, activation="relu")(z)
    out = layers.Dense(64)(x)

    return models.Model(input, out)

