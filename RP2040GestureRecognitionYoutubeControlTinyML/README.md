**RP2040 Gesture Recognition Interface for Youtube Control - TinyML**

Developed a gesture-recognition system using an RP2040 microcontroller and MPU6050 Sensor to control YouTube playback through three predefined gestures.

- Integrated the MPU6050 using **I²C** and Arm Mbed OS for accelerometer data acquisition and multi threading.
- Collected and labeled motion data and trained a compact classification model using **Edge Impulse**.
- Used **spectral features** to extract useful motion patterns for gesture classification.
- Deployed the trained model on the RP2040 using the Edge Impulse C++ inference library.
- Used concurrent sensor acquisition and model inference on the microcontroller with **Arm Mbed OS multithreading.**
- Developed a Python automation script using **PyAutoGUI** to translate recognized gestures into keyboard commands for Youtube Control.

**Board Used** 

<img width="529" height="695" alt="Screenshot 2026-11-18 at 22 22 56" src="https://github.com/user-attachments/assets/857ba186-838b-418e-9502-4ee14486cbec" />

**MPU6050 Module**

<img width="742" height="640" alt="Screenshot 2026-11-18 at 22 23 14" src="https://github.com/user-attachments/assets/1624c8a9-4e4e-469a-8b9e-68774f8d42c6" />

**Demo**

https://github.com/user-attachments/assets/3fb6b421-21e0-45b9-8fd6-93aef945f303

https://github.com/user-attachments/assets/418d7b05-ec09-4244-9364-1c28ae798fb5





