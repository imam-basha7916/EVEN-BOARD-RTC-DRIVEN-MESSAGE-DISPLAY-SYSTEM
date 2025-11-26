    +---------------------+
    |     4x4 KEYPAD      |
    +---------|-----------+
              |
        +-----v--------+            +-----------+
        |   LPC2148    |----------->|   LCD     |
        |  ARM7 MCU    |            +-----------+
        |              |
        |      RTC     |
SWITCH--| EINT         |-----------> GREEN LED
(Admin) |              |-----------> RED LED
        | ADC          |
LM35 -->|              |
        +--------------+


| Component                        | Description                 |
| -------------------------------- | --------------------------- |
| **LPC2148 ARM7 Microcontroller** | Core processing + RTC + ADC |
| **16x2 LCD Display**             | Message display and time    |
| **4x4 Matrix Keypad**            | Password entry + menu       |
| **Switch (INT0)**                | Admin mode activation       |
| **LM35**                         | Temperature sensor via ADC  |
| **Green LED / Red LED**          | Status indication           |
| **Buzzer (optional)**            | Alert indication            |
| **5V Power Supply & PCB/Wires**  | System integration          |


PROJECT WORK FLOW:
The EventBoard: RTC-Driven Message Display System using LPC2148 is a real-time, automated message display system 
that shows predefined messages on a 16x2 LCD with scrolling mechanism at specific times using the on-chip RTC. 

The system has 10 fixed messages stored in controller memory, and allows users to securely enter Admin Mode 
using a switch (external interrupt) and a password-protected keypad.

In Admin Mode, the user can edit the current time and select which messages should be active for the day. During normal operation,
if a scheduled message matches the RTC time, it is displayed and a green LED is turned ON to indicate active schedule display. 
If no message is scheduled, the system instead shows the real-time clock and room
temperature (measured via an LM35 sensor using the LPC2148’s on-chip ADC), and a red LED is turned ON to indicate idle display mode.

This project combines secure access, RTC-based control and environmental monitoring making it a complete embedded system application.
