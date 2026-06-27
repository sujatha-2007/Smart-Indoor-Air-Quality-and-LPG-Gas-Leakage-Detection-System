# Smart-Indoor-Air-Quality-and-LPG-Gas-Leakage-Detection-System
An IoT-based air quality monitoring system that detects LPG gas leaks and monitors temperature and humidity using ESP32, MQ-2, and DHT11 sensors. The system displays real-time data on an OLED screen and sends instant alerts through the Blynk mobile app.
📌 Overview

The Air Quality Monitoring System is an IoT-based project designed to continuously monitor indoor environmental conditions and detect hazardous LPG gas leaks in real time. The system collects sensor data using an ESP32 microcontroller and provides instant notifications through the Blynk mobile application while displaying live readings on an OLED display.

This project demonstrates the integration of embedded systems, IoT, and cloud connectivity to improve safety and environmental awareness.

🚀 Features
Real-time monitoring of temperature and humidity.
LPG gas leak detection using the MQ-2 sensor.
Live sensor data displayed on an OLED display.
Remote monitoring through the Blynk mobile application.
Instant alerts when gas levels exceed the safety threshold.
Wi-Fi-enabled data transmission using the ESP32.
🛠️ Technologies Used
ESP32
Arduino IDE
MQ-2 Gas Sensor
DHT11 Temperature & Humidity Sensor
OLED Display
Blynk IoT Platform
🔧 Hardware Components
ESP32 Development Board
MQ-2 Gas Sensor
DHT11 Sensor
0.96" OLED Display
Jumper Wires
USB Cable
⚙️ Working
The ESP32 continuously reads data from the MQ-2 and DHT11 sensors.
Temperature, humidity, and gas concentration are processed in real time.
Sensor values are displayed on the OLED screen.
The ESP32 sends the readings to the Blynk IoT platform over Wi-Fi.
If gas levels exceed the predefined threshold, an instant alert is sent to the user's mobile device.
🎯 Future Improvements
Integrate additional air quality sensors (CO₂, PM2.5, VOC).
Store historical sensor data using cloud databases.
Add AI-based air quality prediction and analytics.
Develop a dedicated mobile application.
