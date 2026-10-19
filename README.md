<div align="center">

# 🏠 Smart Automation Home

### An Embedded Smart Home Automation & Security System

**Built with Embedded C · ATmega32 · FreeRTOS · Bluetooth**

A modular embedded system that integrates smart access control, wireless device management, environmental monitoring, and automatic fan regulation using AVR microcontroller technology.

[![Platform](https://img.shields.io/badge/Platform-AVR_ATmega32-blue?style=for-the-badge)](https://www.microchip.com/)
[![Language](https://img.shields.io/badge/Language-Embedded_C-00599C?style=for-the-badge\&logo=c)](https://en.cppreference.com/w/c)
[![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-00A98F?style=for-the-badge)](https://www.freertos.org/)
[![IDE](https://img.shields.io/badge/IDE-Eclipse-purple?style=for-the-badge\&logo=eclipseide)](https://eclipseide.org/)

</div>

---

## 📌 Overview

**Smart Automation Home** is an embedded systems project designed to demonstrate how microcontrollers, peripheral drivers, wireless communication, and real-time task management can work together to implement smart home functionality.

The system uses an **ATmega32 microcontroller** to manage access authentication, control home devices, monitor temperature and humidity, and adjust fan operation according to environmental conditions.

Users can interact with the system through a keypad or a Bluetooth serial connection. An LCD provides local feedback, while EEPROM storage allows the configured access password to persist across power cycles.

The firmware is developed in Embedded C and organized into separate **Microcontroller Abstraction Layer (MCAL)** and **Hardware Abstraction Layer (HAL)** modules, with FreeRTOS used for task scheduling.

## ✨ Key Features

### 🔐 Access Control & Security

* Four-digit password authentication.
* Password entry through the keypad and Bluetooth interface.
* Password storage and retrieval using internal EEPROM.
* Password change functionality.
* Access-controlled operation of the system.
* Buzzer alarm following repeated incorrect password attempts.
* Servo-based door locking and unlocking control.

### 📱 Bluetooth-Based Device Control

Control supported devices wirelessly through a Bluetooth serial connection.

* Turn the motor or fan ON and OFF.
* Switch the light output ON and OFF.
* Open and close the door.
* Cycle through predefined fan speeds.
* Enable or disable automatic temperature-based control.
* Lock the system and return to the password screen.

### 🌡️ Environmental Monitoring

* Interface with a DHT11 temperature and humidity sensor.
* Read temperature and humidity periodically.
* Display environmental readings on the LCD after access is granted.
* Report sensor errors through the display.
* Use temperature readings as the input to automatic fan regulation.

### ⚙️ Automatic Fan Control

* Enable or disable automatic temperature-based fan operation.
* Adjust motor speed according to the configured temperature thresholds.
* Generate software PWM through Timer0 for motor speed control.
* Provide manual fan-speed selection when automatic control is disabled.

### 🖥️ Local User Interface

* LCD-based password and system status screens.
* Keypad-based local interaction.
* Feedback for authentication, device states, fan speed, and sensor errors.
* Shared control through local input and Bluetooth commands.

### 🧩 Modular Embedded Architecture

* Separate drivers for digital I/O, UART, timers, and EEPROM.
* Dedicated hardware interface modules for LCD, keypad, Bluetooth, and DHT11.
* Reusable utility headers for bit manipulation and standard data types.
* FreeRTOS integration for application task management.

---

## 🏗️ System Architecture

The firmware is organized into layers to separate application logic from hardware-specific operations.

```text
┌───────────────────────────────────────────────┐
│                Application Layer               │
│                                               │
│  Authentication · Door Control · Fan Control  │
│  Light Control · Environmental Monitoring      │
└───────────────────────┬───────────────────────┘
                        │
┌───────────────────────▼───────────────────────┐
│           Hardware Abstraction Layer          │
│                                               │
│       LCD · Keypad · Bluetooth · DHT11         │
└───────────────────────┬───────────────────────┘
                        │
┌───────────────────────▼───────────────────────┐
│       Microcontroller Abstraction Layer       │
│                                               │
│          DIO · UART · Timer · EEPROM           │
└───────────────────────┬───────────────────────┘
                        │
┌───────────────────────▼───────────────────────┐
│               ATmega32 Hardware               │
│                                               │
│       GPIO · UART · Timer0 · Internal EEPROM   │
└───────────────────────────────────────────────┘
```

**Real-time execution:** FreeRTOS provides the task scheduling infrastructure used by the firmware. The application includes a main task for user interaction and a separate DHT task for environmental monitoring and automatic fan management.

---

## 🧰 Hardware Components

The following components are referenced by the firmware. Confirm the final physical bill of materials against the actual circuit before assembly.

| Component                                    | Purpose                              |
| -------------------------------------------- | ------------------------------------ |
| ATmega32                                     | Main microcontroller                 |
| DHT11                                        | Temperature and humidity sensing     |
| 16×2 LCD                                     | Local display and system feedback    |
| Matrix Keypad                                | Local password entry and interaction |
| HC-05 / HC-06 compatible Bluetooth interface | Wireless serial communication        |
| Servo Motor                                  | Door mechanism control               |
| DC Motor and suitable driver                 | Fan or motor control                 |
| LED                                          | Light output demonstration           |
| Buzzer                                       | Security alarm indication            |

> **Hardware note:** A DC motor must be driven through an appropriate driver stage rather than directly from a microcontroller GPIO pin. The final circuit should also account for power requirements, grounding, and protection.

---

## 🎮 Bluetooth Command Reference

After successful authentication, the firmware accepts the following single-character commands through UART.

| Command | Action              | Description                                                                     |
| :-----: | ------------------- | ------------------------------------------------------------------------------- |
|   `1`   | Motor ON            | Enables manual motor operation                                                  |
|   `2`   | Motor OFF           | Stops the motor and disables automatic control                                  |
|   `3`   | Light ON            | Sets the light output HIGH                                                      |
|   `4`   | Light OFF           | Sets the light output LOW                                                       |
|   `5`   | Door OPEN           | Commands the servo to the configured open position                              |
|   `6`   | Door CLOSED         | Commands the servo to the configured closed position                            |
|   `9`   | Change Fan Speed    | Cycles through 25%, 50%, 75%, and 100%                                          |
|   `*`   | Toggle Auto Control | Enables or disables temperature-based fan control                               |
|   `#`   | Lock System         | Stops the motor, disables automatic control, and returns to the password screen |

### Communication Configuration

| Parameter               | Configuration                      |
| ----------------------- | ---------------------------------- |
| Communication interface | UART                               |
| Baud rate               | 9600 bps                           |
| Data format             | 8 data bits, no parity, 1 stop bit |
| Input format            | Single-character commands          |

**Important:** The commands above describe the implemented firmware behavior. The actual Bluetooth module, wiring, and application used to send commands must be verified against the physical setup.

---

## 📂 Project Structure

```text
Smart-Automation-Home/
│
├── Include/
│   ├── HAL/
│   │   ├── BLUETOOTH/
│   │   ├── DHT11/
│   │   ├── KEYPAD/
│   │   └── LCD/
│   │
│   ├── MCAL/
│   │   ├── DIO/
│   │   ├── EEPROM/
│   │   ├── TIMER/
│   │   └── UART/
│   │
│   └── Lib/
│       ├── BIT_Math.h
│       └── STD_Types.h
│
├── Src/
│   ├── BLUETOOTH_Program.c
│   ├── DHT11_Program.c
│   ├── DIO_Program.c
│   ├── EEPROM_Program.c
│   ├── Keypad_Program.c
│   ├── LCD_Program.c
│   ├── main.c
│   ├── TIMER_Program.c
│   └── UART_Program.c
│
├── FreeRTOS/
│   ├── include/
│   ├── portable/
│   └── source/
│
├── .project
├── .cproject
├── README.md
├── REQUIREMENTS.md
└── .gitignore
```

*The structure above summarizes the principal source modules. Keep the documentation synchronized with the actual files included in the repository.*

---

## 🛠️ Software Requirements

To develop and build the firmware, you will need:

* **Eclipse IDE** with C/C++ development support.
* **AVR-GCC toolchain** compatible with the ATmega32 target.
* **AVR development utilities** appropriate for the selected programming workflow.
* **Git** for version control.
* The FreeRTOS source files and configuration required by the project.
* A compatible AVR programmer and physical hardware for firmware deployment and testing.

See [`REQUIREMENTS.md`](REQUIREMENTS.md) for the development environment and hardware setup details.

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR-USERNAME/Smart-Automation-Home.git
```

Replace `YOUR-USERNAME` with the GitHub account or organization that owns the repository.

### 2. Import the Project into Eclipse

1. Open Eclipse IDE.
2. Select **File → Import**.
3. Choose **General → Existing Projects into Workspace**.
4. Select the cloned project directory.
5. Import the project into the workspace.

If Eclipse does not recognize the project configuration, verify the `.project` and `.cproject` files and the installed AVR Eclipse integration.

### 3. Verify the Build Configuration

Before building, verify:

* The selected target is ATmega32.
* The AVR-GCC toolchain is installed and correctly configured.
* The MCU clock setting matches the hardware.
* The include paths contain the required project and FreeRTOS headers.
* The FreeRTOS port and configuration match the selected MCU and compiler.
* The project no longer depends on absolute paths from the original development computer.

### 4. Build the Firmware

Use Eclipse's **Project → Build Project** command.

Resolve any compiler, linker, or configuration errors before programming the device.

### 5. Flash and Test

Use a compatible AVR programming tool to upload the generated firmware.

Verify the LCD, keypad, Bluetooth interface, DHT11 sensor, motor driver, servo, and LED individually before testing the complete system.

---

## 🧪 Validation Checklist

The following tests are recommended before declaring the project release-ready.

* [ ] The project builds successfully from a clean workspace.
* [ ] The LCD initializes and displays system messages correctly.
* [ ] The keypad accepts password input.
* [ ] Bluetooth authentication works as expected.
* [ ] The password persists after a power cycle.
* [ ] Password changes are stored and retrieved correctly.
* [ ] The buzzer responds to the configured failed-attempt threshold.
* [ ] The servo operates safely at the configured positions.
* [ ] The light output responds to commands.
* [ ] Manual motor control works through the driver circuit.
* [ ] Fan speed changes match the intended PWM behavior.
* [ ] DHT11 readings are validated against a known reference.
* [ ] Automatic fan control responds correctly to temperature changes.
* [ ] FreeRTOS tasks operate without unexpected blocking or timing issues.
* [ ] The complete system passes integration testing.

---

## 🔒 Security Considerations

This project demonstrates embedded access control and should not be treated as a certified security system.

* The four-digit password is a basic demonstration mechanism.
* Password entry over Bluetooth should not be considered secure against interception.
* Physical access to the microcontroller or its programming interface may bypass the intended access controls.
* The buzzer alarm and servo mechanism are not substitutes for certified security hardware.
* Production deployments require a more comprehensive threat model and additional safeguards.

---

## 🗺️ Roadmap

Potential improvements include:

* Add a verified circuit diagram and wiring reference.
* Document all pin assignments in a dedicated hardware configuration table.
* Add system screenshots and a hardware demonstration video.
* Improve input validation and error recovery.
* Add structured diagnostics for peripheral failures.
* Review task synchronization and timing behavior.
* Add a reproducible build workflow.
* Introduce automated static analysis and build checks where practical.

---

## 🎯 Project Impact

Smart Automation Home demonstrates the practical integration of embedded software, real-time task scheduling, hardware interfacing, and wireless communication in a unified smart home automation system.

By combining access control, environmental monitoring, and intelligent device management, the project provides a foundation for exploring more advanced connected and automated living environments.

The modular firmware architecture also highlights the importance of maintainability, hardware abstraction, and structured software design in embedded systems engineering.

---

<div align="center">

### 🏠 Smart Automation Home

**Bridging Embedded Intelligence and Everyday Automation.**

*Designed to explore smarter control, safer access, and more responsive living environments through embedded systems engineering.*

</div>
