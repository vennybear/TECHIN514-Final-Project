# Door Detective: Privacy-First Ambient Entry System

> **"A smart doorbell that hears the rhythm, not the person."**

**Door Detective** is a "Calm Technology" smart home solution designed to replace invasive cameras and distracting phone notifications. By using acoustic vibration sensing and temporal pattern analysis, it identifies visitors based on their knocking rhythm and provides a physical, non-intrusive notification on a desktop gauge.

---

## Hero Shot
<img width="1792" height="889" alt="image" src="https://github.com/user-attachments/assets/c248c865-c55e-4254-b272-8bc9a3410a2c" />


---

## Core Concept
Most smart doorbells compromise domestic privacy by recording video and demanding constant attention through push notifications. 
**Door Detective solves this by:**
1. **Privacy-Preserving Sensing**: Using an accelerometer instead of a camera.
2. **Ambient Notification**: Using a physical needle and an analog gauge for "at-a-glance" awareness.
3. **Edge Intelligence**: Classifying visitors locally using vibration signatures.

---

## System Architecture
The system consists of two nodes communicating via the **ESP-NOW** protocol for ultra-low latency (<100ms) without the need for a router.

*   **Sensing Node (The "Ear")**: Mounted on the door. It captures high-frequency vibration data using an LIS3DH sensor and processes it on an ESP32 DevKit.
*   **Display Node (The "Face")**: Sits on the desk. A Seeed Studio XIAO ESP32C3 receives signals and actuates an X27 stepper motor to move the needle.

---

## Hardware Specifications

| Component | Category | Details |
| :--- | :--- | :--- |
| **MCU 1** | Sensing | ESP32 DevKit V1 |
| **MCU 2** | Display | Seeed Studio XIAO ESP32C3 |
| **Sensor** | Vibration | Adafruit LIS3DH (3-Axis Accelerometer) |
| **Actuator** | Motor | X27.168 Bipolar Stepper Motor |
| **Power** | Battery | 3.7V 1000mAh LiPo (Sensing Node) |
| **Interface** | Button | 16mm Latching Push Button |
| **Enclosure** | Manufacturing | Custom 3D-printed (Fusion 360) |

---

## Software & DSP Algorithm
To fulfill the machine learning/DSP requirements, I developed a **Temporal Feature Extraction** pipeline:

1. **Signal Magnitude Vector (SMV)**: Combines X, Y, and Z axes to normalize energy readings regardless of sensor orientation.
2. **Inter-Knock Interval (IKI) Analysis**: Measures the time between peaks to classify intent:
    *   **Urgent (Delivery)**: IKI < 500ms (Rapid double-tap) ➜ Needle points Left.
    *   **Social (Friend)**: IKI 500-1200ms (Rhythmic knock) ➜ Needle points Right.
    *   **General (Guest)**: Single tap / Timeout ➜ Needle points Center.

**Accuracy**: ~85% during internal validation testing.

---

## Hardware Iteration & Lessons Learned
This project was a journey of engineering resilience.
*   **PCB Rev 1.0 Fail**: A footprint pitch mismatch caused the ESP32 headers to be too narrow.
*   **Scrappy Prototyping**: Instead of waiting for a re-milling, I pivoted to a fly-wire assembly strategy to validate the I2C bus and electrical logic.
*   **Final Revision**: The sensing node now features a custom single-sided PCB with integrated pull-up resistors and optimized power traces.

---

## Power Management
*   **Strategy**: Implemented a software-based **Timed Light Sleep** mode (50ms cycles).
*   **Performance**: Reduced average idle current from 80mA to ~15mA, allowing the 1000mAh battery to last through long periods of monitoring.
*   **Future Work**: Routing the LIS3DH interrupt pin to enable true hardware Deep Sleep (uA levels).

---

## Repository Structure
*   `/src`: Firmware for both Sensing and Display nodes.
*   `/hardware`: KiCad schematics, PCB layout, and Gerber files.
*   `/cad`: Fusion 360 3D models (.f3d and .step).
*   `/docs`: Datasheets, Power calculation spreadsheet, and FMEA report.

---

## Author
**Yuna Xiong**
Technology Innovation (MSTI) Graduate Student at the University of Washington.

---
