**RP2040 Gesture Recognition Interface for Youtube Control - TinyML**

Developed a gesture-recognition system using an RP2040 microcontroller and MPU6050 Sensor to control YouTube playback through three predefined gestures.

- Integrated the MPU6050 using **I²C** and Arm Mbed OS for accelerometer data acquisition and multi threading.
- Collected and labeled motion data and trained a compact classification model using **Edge Impulse**.
- Used **spectral features** to extract useful motion patterns for gesture classification.
- Deployed the trained model on the RP2040 using the Edge Impulse C++ inference library.
- Used concurrent sensor acquisition and model inference on the microcontroller with **Arm Mbed OS multithreading.**
- Developed a Python automation script using **PyAutoGUI** to translate recognized gestures into keyboard commands for Youtube Control.
