# Automatic Cat Feeder

This Arduino-based Automatic Cat Feeder dispenses food into four bowls at scheduled times or upon manual activation via a button. The project uses a servo-controlled flap mechanism to dispense food, an LCD for real-time display, and a real-time clock (RTC) for precise scheduling.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Components Used](#components-used)
- [Setup and Usage](#setup-and-usage)
- [How It Works](#how-it-works)
- [Future Improvements](#future-improvements)

## Project Overview
The Automatic Cat Feeder is designed to dispense food to multiple bowls twice daily, providing flexibility for pet owners to manage feeding schedules. Additionally, the feeder includes a button for manual feeding and can display the current date and time on an LCD screen.

## Features
- **Automated Feeding**: Scheduled feeding at set times (e.g., 12:00 PM and 8:00 PM).
- **Manual Feeding**: Manual feeding option activated by a button press.
- **Bidirectional Dispensing**: Alternating left-to-right and right-to-left for bowl distribution.
- **LCD Display**: Real-time display of the current date and time, along with notifications for each feeding stage.
- **Power Efficiency**: LCD backlight activates only when needed.

## Components Used
- **Arduino UNO R3** - The main microcontroller for the feeder.
- **Servo Motor SG90** - Controls the flap mechanism to release food.
- **Real-Time Clock (RTC) DS1302** - Keeps track of date and time for scheduled feeding.
- **Liquid Crystal Display (LCD) 1602 (I2C)** - Displays the current time, date, and feeding notifications.
- **Button** - Allows manual feeding on demand.
- **Wiring and Resistors** - Various connections to complete the circuit.

## Setup and Usage
1. **Upload the Code**: Compile and upload the provided code to the Arduino UNO.
2. **Assembly**: Connect all components according to the manufacturers' instructions.
3. **Initial Run**: Once powered on, the LCD will display “Cat Feeder Ready.” Feeding will be initiated either at the set times (12:00 PM and 8:00 PM) or by pressing the manual feed button.
4. **Customizing Feeding Times**: Adjust the code to set different feeding hours if desired.

## How It Works
- The RTC module keeps track of the current time, while the Arduino reads the RTC's time and compares it to preset feeding times.
- The servo motor positions the flap based on the bowl number, moving across four positions to dispense food.
- The LCD provides feedback, showing real-time information and updates during each feeding cycle.
- The button allows manual feeding; if pressed, the feeder initiates the dispensing sequence immediately, overriding the current schedule if needed.

## Future Improvements
- **Web Interface**: Add a web interface for remote scheduling and manual feeding.
- **Battery Backup**: Integrate a backup power source to ensure functionality during power outages.

## License
This project is open-source and available under the MIT License. Feel free to fork, modify, and use it as a foundation for your own projects.

## Acknowledgements
This project was developed as a personal project to explore Arduino-based automation and real-time control.

---

Enjoy your hassle-free cat feeding experience, but don't forget to pet them for a happy relationship!
