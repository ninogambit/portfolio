**Runny Square - Embedded Game**

Arcade-style game on a custom-designed PCB using an SSD1306 128x64 OLED display and an ATmega328P microcontroller. Implemented a custom graphics-rendering system in C++ without using external OLED graphics libraries. The display is controlled through low-level SSD1306 commands and direct manipulation of its framebuffer.

**Key Features:**
- Button input handling with software debouncing.
- Custom graphics functions for pixel drawing, rectangle rendering, text rendering, and bitmap display.
- Communication with SSD1306 controller over I²C using custom display initialization.
- Non-blocking game timing.
- EEPROM storage for persistent high scores.
- Buzzer control and sound feedback.
- Screen-state architecture to organize the game's different interfaces and transitions.
- Partial framebuffer updates for moving objects instead of clearing and redrawing everything every frame.
- 2-Layer PCB.

**Schematic**

<img width="751" height="533" alt="Screenshot 2026-11-03 at 18 24 19" src="https://github.com/user-attachments/assets/cb83660d-54c7-4d1c-b43b-b6f155fa7e29" />

**PCB**

<img width="664" height="398" alt="Screenshot 2026-11-03 at 18 25 00" src="https://github.com/user-attachments/assets/d6fa0a92-8a27-4217-8675-350e37a2ab60" />

<img width="931" height="673" alt="Screenshot 2026-11-12 at 22 07 08" src="https://github.com/user-attachments/assets/f9df46c8-4ab4-4970-8b6f-1ac77cf6928d" />



**PCB Live Gameplay**

https://github.com/user-attachments/assets/b17c1c51-9253-46d1-9533-de497d563570

https://github.com/user-attachments/assets/595a2b85-70f8-4deb-912f-23c4f9f22e44

https://github.com/user-attachments/assets/ff2a66c2-3247-43ad-a074-7fb4a57e962d
