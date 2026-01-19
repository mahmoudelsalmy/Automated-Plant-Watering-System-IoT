# 🌱 Automated Plant Watering System (IoT)

An IoT-based automated plant watering system using **PIC16F877A** and **ESP8266**.
The system monitors soil moisture and water tank level, controls a water pump
automatically or manually, and provides a real-time web dashboard.

---

## 🔧 Hardware Components
- PIC16F877A Microcontroller
- ESP8266 WiFi Module
- Soil Moisture Sensor (Analog)
- Water Level Sensor
- Relay Module
- Water Pump
- 16x2 LCD
- Power Supply (5V / 12V)

---

## 💻 Software & Tools
- MikroC PRO for PIC
- Arduino IDE (ESP8266)
- Proteus 8 Professional
- HTML / CSS / JavaScript
- Chart.js & Gauge.js

---

## ⚙️ System Features
- Real-time soil moisture and tank monitoring
- Automatic and manual pump control
- Web-based dashboard (Mobile & PC)
- UART communication with handshaking
- LCD local display
- Safe relay-based pump control

---

## 📡 Communication Protocol
- UART (115200 baud)
- Custom data framing
  - PIC → ESP: `#tank,soil,threshold!`
  - ESP → PIC: `#mode,threshold!`


