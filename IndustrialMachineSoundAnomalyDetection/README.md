**RP2040-Based Acoustic Anomaly Detection - TinyML**

This project which was also part of my thesis presents an unsupervised, normal-only acoustic anomaly detection system designed for resource-constrained edge hardware. Using a lightweight, hybrid convolutional-attention autoencoder deployed on an RP2040 microcontroller using TensorFlow Lite Micro, the model detects machine failures directly on-device by evaluation reconstruction errors in log-Mel spectrogram frames.

To handle differences between machines, machine parts, and acoustic environments without retraining, a few-shot. calibration method adapts the decision threshold using only a small number of normal reference recordings.

Evaluated on the MIMII dataset, the system achieves competitive performance compared with larger autoencoder baselines while remaining suitable for embedded deployment. By performing inference and scoring locally, the system avoids cloud-based inference and continuous transmission of sensitive acoustic data, providing a practical step toward scalable and privacy-preserving industrial acoustic monitoring.

**Key Features**

- Usage of MIMII industrial machine sound dataset.
- Log-mel spectrogram feature extraction to convert audio signals into input for my ML model.
- Development of a custom lightweight autoencoder with attention using TensorFlow/Keras.
- Training of the model only on normal samples (not anomalous).
- Quantization of the model to TensorFlow Lite to INT8 for efficient embedded deployment.
- Evaluation of the model and performance on the microcontroller.
- Model inference on the RP2040 using C++ and TensorFlow Lite Micro.
