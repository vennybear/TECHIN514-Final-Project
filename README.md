# The Door Detective: AI-Powered Privacy-First Entry System

## 1. Project Overview
**Concept:**
The Door Detective is a smart home device designed to act as a digital doorman without invasive cameras. It uses Edge AI to analyze vibration patterns on the door surface. The system identifies whether a visitor is a delivery driver (rapid knocking), a friend (rhythmic knocking), or a stranger, and visualizes this information on a physical desktop gauge.

**Physical Appearance:**
<img width="2360" height="1640" alt="c760b10d5f016d36a55d5775de6cfd0e" src="https://github.com/user-attachments/assets/aaaf9f5a-b542-4232-9d14-f87642a66cb3" />


---

## 2. Sensing Device (The "Ear")
**Description:**
The sensing node is a compact, wireless box attached to the back of the door. It remains in "Deep Sleep" mode to conserve battery and wakes up only when vibration is detected.
*   **How it works:** The accelerometer captures the vibration waveform of a knock. The ESP32 processor extracts features (amplitude, frequency) and classifies the pattern locally.
*   **Key Components:**
    *   **Processor:** Espressif **ESP32-WROOM-32E** (Microcontroller with Wi-Fi/BLE)
    *   **Sensor:** STMicroelectronics **LIS3DH** (3-Axis Accelerometer for vibration sensing)
    *   **Power:** 3.7V LiPo Battery (1000mAh)

**Detailed Device Sketch:**
<img width="1545" height="1446" alt="image" src="https://github.com/user-attachments/assets/c389b4a8-957d-40b3-9d0d-3a275fa005bb" />


---

## 3. Display Device (The "Face")
**Description:**
The display unit sits on the user's desk. Instead of a screen that demands attention, it uses a calm technology approach. A stepper motor rotates a needle to specific zones on a dial indicating "Delivery", "Friend", or "Unknown".
*   **How it works:** It receives the classification result via ESP-NOW wireless protocol. The ESP32 sends pulses to the motor driver to move the needle to the correct angle.
*   **Key Components:**
    *   **Processor:** Espressif **ESP32-WROOM-32E**
    *   **Actuator:** **28BYJ-48** Stepper Motor (5V)
    *   **Driver:** **ULN2003** Motor Driver Board
    *   **Power:** USB-C Connection (5V)

**Detailed Device Sketch:**
<img width="2360" height="1640" alt="51a810417ec6cd5773d7152131d298b4" src="https://github.com/user-attachments/assets/9e883fdb-1d0f-4c39-b4a2-c3b83afc45f5" />


---

## 4. System Architecture
**Figure 1: Communication Diagram**
<img width="1615" height="510" alt="image" src="https://github.com/user-attachments/assets/e4b41ce3-3df2-48b1-954d-6bc9df861718" />


**Figure 2: Data Flow Diagram**
<img width="1723" height="724" alt="image" src="https://github.com/user-attachments/assets/8e69eac6-f36a-4863-b43a-2a711af7b1f6" />
