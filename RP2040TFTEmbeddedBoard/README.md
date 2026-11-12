**RP2040 TFT Embedded Board - Custom 4-Layer PCB Design**

Designed a custom four-layer PCB based on the RP2040 microcontroller, integrating a 240x320 TFT display, external flash memory, and a battery-backed Real-Time Clock(RTC).

**Key Features:**
- 4-layer PCB integrating the RP2040 microcontroller and supporting circuitry.
- 240x320 TFT display interface with transistor-based backlight control.
- 16 MB external QSPI flash memory.
- MCP7940N Real-Time Clock with a 32.768 kHz crystal and backup battery.
- AMS1117-3.3 voltage regulator for the power supply.
- USB-C power input with protection circuitry.
- User buttons, status LEDs, and SWD debugging interface.

**Firmware development:**
- Custom TFT display initialization and communication routines without relying on external display or graphics libraries.
- Low-level display control and custom graphics handling.
- RTC communication to retrieve and display real-time clock information.

**Schematic**

<img width="734" height="516" alt="Screenshot 2026-11-05 at 15 28 11" src="https://github.com/user-attachments/assets/93c9087f-e1d0-44ff-a4dc-1710d93dbc45" />

**PCB**

<img width="544" height="511" alt="Screenshot 2026-11-05 at 15 28 33" src="https://github.com/user-attachments/assets/42bc6632-793e-4744-80b9-a08fe6b84cec" />

<img width="900" height="671" alt="Screenshot 2026-11-12 at 18 26 24" src="https://github.com/user-attachments/assets/aa6e8fe1-3b9e-4bfb-9801-060e35e0b6ec" />

<img width="898" height="670" alt="Screenshot 2026-11-12 at 18 26 44" src="https://github.com/user-attachments/assets/d68f6a60-57db-4738-94af-d9db4a3616ed" />

<img width="600" height="425" alt="IMG_5919" src="https://github.com/user-attachments/assets/a84e3890-b130-45b5-a379-15b0772da35f" />
