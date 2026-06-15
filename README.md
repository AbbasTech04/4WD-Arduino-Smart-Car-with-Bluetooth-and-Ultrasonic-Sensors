 Features
Dual Operating Modes: Easily toggle between Manual and Autonomous modes directly from your smartphone app.

1- Wireless Bluetooth Control: Drive the car remotely using a mobile app (custom RC joystick app).

2- Smart Obstacle Avoidance (Auto Mode): Switch to "Auto Mode" to let the car navigate independently, scanning the environment with an ultrasonic sensor to detect and avoid collisions.

4WD Power: Equipped with a 4-wheel-drive chassis for steady movement and better traction.

Hardware Components

1- Microcontroller: Arduino Uno  or Arduino Nano

2- Chassis: 4WD Robot Car Chassis Kit + 4 DC Motors

3- Motor Driver: L298N

4- Wireless: HC-05 or HC-06 Bluetooth Module

5- Sensor: HC-SR04 Ultrasonic Sensor + Servo Motor 180 (for scanning)

6- Power: 3x 18650 Li-ion Batteries (3.7V each, 2200 mAh least) + Battery Holder 3.

How It Works
Manual Mode: The car listens for Bluetooth commands sent from your smartphone (e.g., 'F' for Forward, 'B' for Backward, 'L' for Left, 'R' for Right). It should starts with  M for manual mode Or W for auto mode.

Autonomous (Auto) Mode: When you send the "W" command from your phone, the Arduino hands over control to the ultrasonic sensor. The car will drive forward until an obstacle is detected within a safe threshold, then scan left/right to find the clearest path and turn automatically.

Setup:
3. Mobile App Setup
Download any Bluetooth RC Car app from the Play Store / App Store.
Play Store:https://play.google.com/store/apps/details?id=appinventor.ai_spiridakis84.car_testV
App Store:https://apps.apple.com/us/app/bluetooth-for-arduino/id1505096526

Pair your phone with the HC-05/06 module (Default PIN is usually 1234 or 0000).

🤝 Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page if you want to contribute.

## 📁 Project Versions

 Version 2.0 (Current - Arduino Nano)
Features: Dual mode (Bluetooth control + Autonomous obstacle avoidance).

Hardware: Arduino Nano, 3x 18650 Li-ion Batteries, HC-05 Bluetooth, HC-SR04 Ultrasonic Sensor + Servo Motor, L298n Motor Driver.

Code: Available in the default main branch.

 Version 1.0 (Legacy - Arduino Uno)
Features: Basic autonomous obstacle avoidance only (No Bluetooth).

Hardware: Arduino Uno, 2x Batteries, HC-SR04 Ultrasonic Sensor + Servo Motor, L298n Motor Driver.

Code: Available in the v1-obstacle-avoidance branch.

## Project Media & Demonstrations
You can view the full operation video, high-quality photos, and updated circuit diagrams for the Nano version through the link below:
[📂 View Project Media on Google Drive](https://drive.google.com/drive/folders/1XtN9jsJOe7HPrEMpcKbjQ8P-392dxl-e?usp=drive_link)
