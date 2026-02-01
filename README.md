# 🩺 Intelligent Medical Monitoring System  
### Preventive Patient Care using ESP32 & IoT

## 📌 Project Overview
The **Intelligent Medical Monitoring System** is an IoT-based healthcare solution designed for **preventive patient care and remote health supervision**.  
It continuously monitors essential vital signs and provides **real-time alerts** both locally and remotely.

The system is built around an **ESP32 microcontroller**, integrating **high-precision biomedical sensors** and an **IoT dashboard** for visualization and historical data analysis.

This project was developed as part of an **Electromechanical Engineering – Mechatronics** academic program.

---

## 🎯 Project Objectives
- Continuous monitoring of vital signs
- Early detection of abnormal physiological conditions
- Real-time local alerts (LCD & buzzer)
- Remote monitoring via IoT dashboard
- Historical data logging for medical analysis
- Preventive intervention to reduce emergency hospitalizations

---

## 👥 Target Users
- Elderly patients living alone
- Post-operative patients
- Individuals with chronic diseases
- Family caregivers
- Medical staff and healthcare professionals

---

## 🧠 System Architecture
The system follows a **centralized architecture** with the ESP32 acting as the core controller.

### 🔹 Hardware Components
- **ESP32** – Main processing unit & WiFi connectivity
- **MAX30102** – Heart rate and SpO₂ sensor
- **MLX90614** – Non-contact body temperature sensor
- **20x4 I2C LCD** – Local data display
- **Buzzer** – Audible alerts for critical states

### 🔹 Communication
- **I2C Protocol** for sensor communication
- **WiFi** for cloud connectivity
- **MQTT** protocol for real-time data transmission
- **JSON** data format

---

## 🖥️ User Interfaces
### 📟 Local Interface
- Real-time display of vital signs on LCD
- Audible alerts for abnormal conditions

### 🌐 Remote Interface
- Web-based IoT dashboard
- Real-time monitoring
- Color-coded health indicators
- Historical data visualization (graphs)

---

## 🚨 Health Status Scenarios
The system automatically detects and classifies health conditions into three states:

### 🟢 Normal State
- Temperature ≈ 36–37°C
- SpO₂ ≥ 95%
- Heart Rate ≈ 60–80 bpm  
✔ No alerts triggered

### 🟠 Warning State
- Hypothermia or mild hypoxia detected  
⚠ Visual warnings on dashboard

### 🔴 Critical State
- Fever (>38°C)
- Severe hypoxia
- Tachycardia  
🚨 Red alerts + continuous buzzer activation

---

## 📊 Historical Data Analysis
- Continuous data logging
- Trend visualization for:
  - Temperature
  - Heart Rate
  - SpO₂
- Helps differentiate temporary anomalies from chronic conditions

---

## 🧪 Experimental Validation
The prototype was tested under multiple physiological scenarios and demonstrated:
- Reliable sensor synchronization
- Accurate measurements
- Stable IoT communication
- Effective alert mechanisms

---

## ⚙️ Technologies Used
- ESP32
- MAX30102
- MLX90614
- MQTT
- WiFi
- JSON
- IoT Dashboard
- Embedded C / Arduino Framework

---

## 👨‍💻 Project Team
- Hani Khedhiri  
- Omar Mejri  
- Ghassen Ben Salem  
- Yahia Jbeli  
- Ala Eddine Chibouni  
- Aymen Badereddine  
- Mahjoub Henchi  

**Supervisor:** Professor Imen Saidi  
**Academic Year:** 2025 – 2026

---

## ✅ Conclusion
This project demonstrates how **embedded systems and IoT technologies** can be combined to create an **accessible, reliable, and efficient medical monitoring solution**.  
By integrating local and remote supervision, the system enhances patient safety, improves quality of life, and supports preventive healthcare.

---

## 📄 License
This project is intended for **academic and educational use**.

