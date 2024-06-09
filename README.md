# ESP32 Electrocardiogram Project

---

## Description:

This project involves the development of an electrocardiogram using the ESP32 microcontroller. The device captures the heart's electrical signals and displays them in real-time through a graphical interface. It has been implemented using the Visual Studio Code development environment with the PlatformIO plugin, which facilitates programming and project management.

---

## Features:

- **Signal Capture:** The ESP8266 and the pulse meter AD8232 records the heart's electrical signals through electrodes connected to the patient.

- **Real-Time Processing:** The captured signals are processed and displayed in real-time on the OLED display.

- **MatLab Processing:** To ensure a clean representation of ECG data, both manual Fourier transformation filtering and implementation of a bandpass filter using Matlab were performed.

---

## Technologies Used:

- **ESP8266:** Low-power microcontroller with WiFi and Bluetooth connectivity, ideal for IoT applications.

- **Visual Studio Code:** Powerful development environment with support for multiple programming languages.

- **PlatformIO:** Visual Studio Code plugin that simplifies development and project management on microcontroller platforms like Arduino, ESP32, etc.

---

## Installation:

1. Clone this repository to your local machine.

2. Open the project in Visual Studio Code.

3. Install the necessary dependencies using PlatformIO.

4. Connect the microcontroller, the pulse meter and the display to the computer and upload the project's firmware.

5. Connect the electrodes to the patient.

6. Ready to use!

---

## Contributions:

Contributions are welcome! Feel free to improve this project by opening issues or sending pull requests.

---

## Authors:

- Christian Sanchez
- Fernando Gallardo

---

## License:

This project is licensed under the MIT License. See the `LICENSE` file for more information.
