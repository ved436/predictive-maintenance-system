# Predictive Maintenance System for Rotating Machinery

## Overview
This project implements a **rule-based predictive maintenance system** for rotating machines.
It focuses on early fault detection using **sensor fusion**, **trend analysis**, and
**confidence scoring**, similar to methods used in industrial condition monitoring.

The system is developed as a **simulation-based prototype** using Arduino to validate
maintenance logic before real hardware deployment.

---

## Key Features
- Temperature trend-based degradation detection
- Vibration baseline learning and anomaly detection
- Sensor fusion for reliable fault diagnosis
- Trend memory with weighted confidence scoring
- Preventive shutdown to avoid catastrophic failure
- Human–Machine Interface using LCD and RGB LED

---

## System Architecture

---

## Components Used
- Arduino Uno  
- Potentiometers (temperature & vibration simulation)  
- DC Motor  
- RGB LED (machine health indication)  
- Buzzer (audible alarm)  
- LCD 16×2 (diagnostic display)

---

## Health States & Diagnosis
| Condition | Diagnosis |
|---------|-----------|
| Normal temperature + normal vibration | Healthy |
| Rising temperature + normal vibration | Thermal Wear |
| Normal temperature + abnormal vibration | Mechanical Fault |
| Rising temperature + abnormal vibration | Bearing Wear |

---

## Applications
- Industrial rotating machinery (motors, pumps)
- Predictive maintenance research
- Embedded systems prototyping
- Condition monitoring systems

---

## Future Enhancements
- Real vibration sensors (accelerometers)
- Cloud-based dashboards
- Machine learning–based fault classification

---

## Author
**Ved Kale**  
Electronics Engineering Student  
Walchand College of Engineering, Sangli
