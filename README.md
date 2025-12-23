# 🚗 Smart & Secure Garage System

> **An intelligent, secure, and fully automated parking solution built with Arduino, smart sensors, and real‑time visual feedback.**

---

## 🌟 Project Overview

The **Smart & Secure Garage System** is an embedded systems project designed to automate parking management with **high security, real‑time monitoring, and user‑friendly interaction**.  
It intelligently detects vehicle presence, controls entry and exit gates, Show parking availability, all working together as a complete smart garage ecosystem.

This project demonstrates how **hardware integration + logical control** can transform a traditional garage into a **smart, secure, and efficient system**.

---

## 🎯 Key Objectives

- Automate vehicle **entry and exit** safely
- Secure the garage using **RFID authentication**
- Display clear information to users
- Provide visual status indicators for each slot
- Ensure smooth and reliable operation using multiple controllers

---

## 🧠 System Architecture

The system is built using **two Arduino boards**, each responsible for specific tasks to ensure stability, scalability, and fast response.

### 🔹 Core Components

| Component             | Purpose                                                                  |
| --------------------- | ------------------------------------------------------------------------ |
| **Arduino (x2)**      | Distributed control for entry/exit & slot management                     |
| **LDR Sensors**       | Detect car presence in parking slots                                     |
| **IR Sensors**        | Detect vehicles entering or exiting                                      |
| **RFID Module**       | Secure access control                                                    |
| **Servo Motors (x2)** | Control entrance & exit barriers                                         |
| **LCD Display**       | Show available slots & welcome messages                                  |
| **RGB LEDs**          | Visual indication of slot status                                         |
| **Flame Sensor**      | Detect if there is a fire or flame in the garage                         |
| **Buzzer**            | Make Sound at entrance, exit and when fire been detected it act as alert |

---

## 🚨 Safety & Emergency Handling (Fire Detection System)

> **Safety is not optional — it is built into the system.**

To ensure maximum protection for people, vehicles, and infrastructure, the Smart & Secure Garage System integrates an **intelligent fire‑response mechanism** using a **flame sensor and buzzer**.

### 🔥 Flame Sensor – Emergency Logic

- Continuously monitors the garage for fire or flame presence
- When a flame is detected:
  - 🚪 **Exit gate opens immediately** to allow vehicles to leave safely
  - ⛔ **Entry gate remains closed** to prevent new vehicles from entering
  - The system stays in this emergency state **until the flame is completely cleared**
- Normal operation resumes automatically once no flame is detected

### 🔊 Buzzer – Audio Alerts

- Emits a sound during **gate opening and closing** for safety awareness
- Activates a **continuous alert** when fire is detected
- Enhances human awareness in emergency situations

This safety layer transforms the project from a smart system into a **reliable, real‑world‑ready solution**.

---

## 🔐 Security Layer – RFID Access

Only **authorized vehicles** can enter the garage.

- Each driver uses an **RFID card/tag**
- The system validates the card before opening the gate
- Unauthorized access is rejected automatically

This ensures **controlled access and enhanced security**.

<p align="centre">
<img height=20px alt="RFID image in the project" src="\imgs & video\entrance-&-RFID.jpeg">
📸 Image: RFID module & card scanning
</p>

---

## 🚘 Vehicle Detection Logic

### 🔹 Entry & Exit Detection

- **IR sensors** detect whether a car is approaching the **entry** or **exit** gate
- The system identifies direction (IN / OUT)
- Corresponding **servo motor** opens the correct barrier
<p align="centre">
<img width=20px alt="detection image" src="\imgs & video\img1.jpeg">
</p>

### 🔹 Parking Slot Detection

- **LDR sensors** are placed at each parking slot
- They detect whether a car is present or not
- Slot status updates instantly

---

## 🚦 Visual Feedback System

### 🔹 RGB LEDs (Per Slot)

| Color    | Meaning        |
| -------- | -------------- |
| 🔴 Red   | Slot occupied  |
| 🟢 Green | Slot available |

This allows drivers to **instantly identify free parking spaces**.

<p align="centre">
<img width=20px alt="RFID image in the project" src="\imgs & video\LDRs-&-LEDs-image.jpeg">
📸 *Image: LDR sensor detecting a parked car & RGB LEDs showing slot status*
</p>

---

## 🖥️ LCD User Interface

The LCD screen provides a clear and friendly interface:

- Welcome message upon entry
- Real‑time number of **available parking slots**
- System status updates

---

## ⚙️ Gate Control System

- **Two servo motors** operate the entrance and exit barriers
- Gates open only when:
  - RFID authentication is valid
  - IR sensors confirm vehicle presence
- Gates close automatically after vehicle passes

This ensures **smooth traffic flow and safety**.

---

## 🔄 System Workflow

1. Vehicle approaches entry gate
2. IR sensor detects the car
3. Servo motor opens the barrier
4. LCD updates available slots
5. Car parks in an available slot
6. LDR updates slot status
7. RGB LED changes color
8. RFID authentication is verified
9. Exit process follows the same logic

---

## 🧪 Reliability & Smart Design

- Dual‑Arduino architecture reduces system overload
- Sensors work independently for accuracy
- Real‑time updates ensure consistency
- Modular design allows easy expansion

---

## 📷 Project Gallery

<p align="centre">
<img alt="RFID image in the project" src="\imgs & video\whole-project.jpeg">
</p>

1. **Complete system – wide view**
2. **Garage from near view**
3. **LDR sensor in parking slot**
4. **RFID module & card**
5. **IR sensor at gate**
6. **Servo motor barrier**
7. **RGB LEDs status view**
8. **LCD display close‑up**

---

## 🎥 Live Demo Video

<a href="\imgs & video\project-record.mp4">🎬 _Watch the system working in real time:_ </a>

The video demonstrates:

- Secure entry using RFID
- Automatic gate control
- Slot detection & LED updates
- LCD live information
- Smooth exit operation

---

## 🚀 Future Enhancements

- 📱 Mobile Application Control (Bluetooth Integration)
  Add a Bluetooth module (e.g., HC-05 / HC-06) to enable a mobile application that can remotely open and close the garage gates, monitor system status, and provide manual override control when needed.

- 🔗 Arduino-to-Arduino Communication
  Establish direct communication between the two Arduino boards (via I2C, UART, or SPI) so they can share real-time data, synchronize decisions, and operate as a fully integrated distributed system rather than independent controllers.

- 🔄 Bidirectional Slot Management (Entry & Exit Handling)
  Enhance the logic so that when a car exits the garage, the number of available parking slots automatically increases and updates correctly on the LCD display, ensuring accurate real-time parking availability instead of only decrementing on entry.

---

## 🏆 Conclusion

The **Smart & Secure Garage System** is a complete demonstration of how **embedded systems, sensors, and smart logic** can solve real‑world problems.

It combines **security, automation, and usability** into a single powerful solution — making it not just a project, but a **real‑world smart system prototype**.

---

💡 _Built with passion, teamwork, and innovation._
