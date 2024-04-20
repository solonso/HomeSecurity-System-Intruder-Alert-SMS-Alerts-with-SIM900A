# HomeSecurity-System-Intruder-Alert-SMS-Alerts-with-SIM900A

## Overview

The Arduino Home Security System with SIM900A integrates multiple sensors like PIR, ultrasonic, sound, and vibration sensors to create a robust home security solution. It detects intruders, measures distances, monitors sound levels, and senses vibrations. Upon detection of suspicious activity, the system triggers SMS alerts using the SIM900A GSM module, providing real-time notifications to the user. LED indicators and a buzzer provide visual and audible alerts for added security. The project offers a customizable and expandable platform for enhancing home security, making it an ideal choice for DIY enthusiasts and developers looking to build a comprehensive security system.

## Features

- **Intruder Detection:** Uses PIR sensor for human presence detection.
- **Distance Measurement:** Utilizes ultrasonic sensors for distance measurement and object proximity detection.
- **Sound Monitoring:** Monitors sound levels using a sound sensor.
- **Vibration Detection:** Detects vibrations using a vibration sensor.
- **GSM Communication:** Communicates via GSM for SMS alerts and call notifications.
- **Visual and Audible Alerts:** LED indicators and a buzzer provide visual and audible alerts for detected events.

## Hardware Requirements

- Arduino Uno or compatible board
- PIR sensor
- Ultrasonic sensors
- Sound sensor
- Vibration sensor
- GSM module (SIM900A)
- LEDs and a buzzer for alerts

## Installation

1. Connect the sensors, GSM module, LED, and buzzer to the Arduino board as per the provided circuit diagram.
2. Upload the Arduino sketch (`main.ino`) to your Arduino board using the Arduino IDE.
3. Configure the GSM module with the required SIM card and network settings.
4. Power on the Arduino board and GSM module.

## Usage

1. The system starts monitoring for intruders and sensor events upon power-up.
2. When motion is detected by the PIR sensor, the system triggers a sequence of actions:
   - Measures distance using an ultrasonic sensor.
   - Checks for vibrations using the vibration sensor.
   - Monitors sound levels using the sound sensor.
   - Sends SMS alerts and activates LED/buzzer indicators for detected events.
3. The system supports making calls to predefined numbers for critical events.

## Troubleshooting

- Ensure all connections are secure and correctly wired based on the circuit diagram.
- Verify that the GSM module has a valid SIM card and is powered correctly.
- Check the serial monitor output for debugging information and sensor readings.
- Adjust sensor thresholds and settings as needed for optimal performance.

## Contributing

Contributions to enhance the project are welcome! Fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
