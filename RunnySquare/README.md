**Runny Square - Embedded Game**

Arcade-style game on a custom-designed PCB using an SSD1306 128x64 OLED display and an ATmega328P microcontroller. Implemented a custom graphics-rendering system in C++ without using external OLED graphics libraries. The display is controlled through low-level SSD1306 commands and direct manipulation of its framebuffer.

**Key Features:**
- Button debouncing using millis().
- Custom graphics functions for pixel drawing, rectangle rendering, text rendering, and bitmap display.
- Collision detection using coordinate comparisons.
- Non-blocking game timing.
- EEPROM storage for persistent high scores.
- Buzzer control and sound feedback.
- Bitmap and font storage in PROGMEM.
- Partial framebuffer updates for moving objects instead of clearing and redrawing everything every frame.
