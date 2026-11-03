**Digital Clock - ATtiny85, Shift Registers und DS1307 RTC**

Designed and programmed a custom digital clock PCB based on the ATtiny85 microcontroller with minimized GPIO usage using two 74HC595 shift registers to control a four-digit 7-segment display.

**Key Features:**
- Communication with DS1307 Real-Time Clock (RTC) using I²C to maintain accurate time.
- Custom serial data transmission to two cascaded 74HC595 shift registers (multiplexing) using direct GPIO control and bitwise operations, without relying on the Arduino **shiftOut()** function.
- Custom digit-to-segment mapping.
- Non-blocking timing to manage display updates and time-reading intervals.
- 2-Layered PCB.

**Schematic**

<img width="761" height="540" alt="Screenshot 2026-11-03 at 20 50 04" src="https://github.com/user-attachments/assets/9eedc658-3168-46cf-8ac1-a7fdccb77af0" />

**PCB**

<img width="642" height="412" alt="Screenshot 2026-11-03 at 20 50 38" src="https://github.com/user-attachments/assets/77358cd2-7498-4ef2-8cc7-f552fcad58d2" />
