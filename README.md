![License](https://img.shields.io/github/license/lemonsloth/arcade-vending)
![Last Commit](https://img.shields.io/github/last-commit/lemonsloth/arcade-vending)
![MCU](https://img.shields.io/badge/MCU-ATmega32U4-blue)
![Protocol](https://img.shields.io/badge/protocol-SPI-orange)
![Hardware](https://img.shields.io/badge/hardware-RFID-yellow)
![Hardware](https://img.shields.io/badge/hardware-LCD-yellow)
![Hardware](https://img.shields.io/badge/hardware-Servo-yellow)
![Build](https://img.shields.io/badge/build-make-critical)
![Status](https://img.shields.io/badge/status-WIP-lightgrey)

---

# Arcade Prize Dispenser
Awarded 3rd Place in UNLV's Spring 2025 Junior Design Competition

Sure, winning arcade games is fun and all, but the REAL point of every arcade is to win amazing prizes! With this project, we will design our own prize dispensing machine utilizing the ATmega328p as the brain of the project.

# Overview

This project implements an arcade-style vending machine using an ATmega328PB and peripherals including an RFID scanner, LCD module, servo motors, buttons, variable voltage regulator, and logic level shifter. 

It demonstrates my foundational understanding of both hardware, software, and soft skills essential for embedded systems engineering.

| Category                   | Skill                                                                   |
|----------------------------|-------------------------------------------------------------------------|
| **Embedded concepts**      | SPI, GPIO, PWM, interrupts, timers, register-level hardware control     |
| **Testing tools**          | Multimeter, logic / protocol analyzer                                   |
| **Prototyping process**    | Circuit design, breadboarding, wiring                                   |
| **Version control**        | Command-line git + team collaboration                                   |
| **Soft skills**            | Communication, project management, team leadership                      |

## Features
- Entices patrons to tap their cards in order to redeem prizes.
- Greets users and displays prize options, user info, and balance.
- Reads account information: UID and balance.
- Prize selection, dispensing, card balance adjustment.
- Ensures valid transactions.
- New account registration.
- Physical vending machine prototype.

## Hardware
| Component                         | Model                 |
|-----------------------------------|-----------------------|
| **MCU**                           | ATmega328PB           |
| **16x2 Character LCD module**     | NHD-0216K1Z-NSW-BBW-L |
| **RFID scanner**                  | MFRC522               |
| **Buttons**                       | --                    |
| **Servo motors**                  | FITEC FS5115M         |
| **Variable voltage regulator**    | LM317                 |
| **Logic level shifter**           | Adafruit 757          |

## Software

Language: C
Interfaces: SPI

## Demo

- [Demo Video - #]()
- [Demo Video - #]()
- [Demo Video - #]()

PPT Presented @ the UNLV Spring 2025 Junior Design Competition
- [Slides](https://docs.google.com/presentation/d/1E5TsQLzwDdy7lnNvE0u5rrdVZENRvXk2xeuqn8TQu58/edit?usp=drive_link)

## License

This project is licensed under the [MIT License](LICENSE).