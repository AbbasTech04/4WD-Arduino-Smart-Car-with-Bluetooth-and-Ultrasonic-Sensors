#include <Servo.h> // Import servo motor library

// --- Pin Definitions ---

//Ultra Sonic sensor pins
const int trig = 11;
const int echo = 12;

// L298N Motor Driver pins
const int RightForward  = 8; // input1
const int RightBackward = 7; // input2
const int LeftBackward  = 4; // input3
const int LeftForward   = 2; // input4
const int ENA = 5; // PWM pin for Right Motors speed
const int ENB = 6; // PWM pin for Left Motors speed

Servo myservo; // Create a servo object to control the radar mount
int speed = 75; // this speed for moving forwards or backwards its normal speed on smooth surface like (tiles), If the surface rough like (carpet), then the speed should be increased to 200 or more up to 255 it depends.
int turnSpeed = 200;
int lowSpeed = 0; // Stop speed
int distance = 0; // initial value will be used for UltraSonic
char command = '\0'; // Initialize command with null character
bool autoMode = false; // Initial state is manual mode; car remains stopped until a command is received from the smartphone

void setup() {

Serial.begin(9600); // Initialize serial communication at 9600 bps
  pinMode(trig, OUTPUT); // transmitter that sends out a sound wave
  pinMode(echo, INPUT); // acts as the receiver that listens for the wave to bounce back

  pinMode(RightForward, OUTPUT);
  pinMode(RightBackward, OUTPUT);
  pinMode(LeftForward, OUTPUT);
  pinMode(LeftBackward, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  myservo.attach(3); // Attach servo to digitalpin 3 in arduino
  myservo.write(65); // Set servo to center position (forward)
  delay(500); // Wait for servo to reach the center position
  stopCar(); // Ensure the car is stopped at startup

}
void loop() {
  // Check if there is incoming data from the Bluetooth module
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();    
    if(incomingChar == 'W') {
      autoMode = true;
      stopCar();
      command = '\0'; // Clear any manual command immediately when switching to Auto Mode
    }
    else if(incomingChar == 'M') {
      autoMode = false;
      stopCar();
      command = '\0'; //Flush previous inputs to prevent immediate movement upon switching modes
    }
    else {
      // Accept manual commands ONLY if Auto Mode is disabled
      if (!autoMode) {
        command = incomingChar;
      }
    }
  }
  // Execute the behavior based on the current mode
  if (autoMode) {
    handleAutoMode(); // Execute obstacle avoidance logic
  } else {
    if (command != '\0') {
      handleManualMode(command); // Execute manual Bluetooth controls
    }
  }
}

// --- Manual Mode Management ---
void handleManualMode(char cmd) {
  switch (cmd) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': turnLeft(); break;
    case 'R': turnRight(); break;
    case 'S': stopCar(); command = '\0'; break; // Stop and clear command to prevent continuous execution
    case 'G': forwardLeft(); break;
    case 'I': forwardRight(); break;
    case 'H': backwardLeft(); break;
    case 'J': backwardRight(); break;
  }
}

// --- Autonomous Mode Management ---
void handleAutoMode() {
  distance = getDistance();
  // If obstacle is too close (<= 30cm) or sensor returns 0 (error/stuck)
  if (distance == 0 || distance <= 30) {
    stopCar();
    delay(800); // Wait for electrical and physical stability
    moveBackward();
    delay(500); // Back up to give the servo and car room to maneuver
    stopCar();
    delay(200);
    // Look and measure distance to the right
    int rightDist = lookRight();
    delay(300);
    // Look and measure distance to the left
    int leftDist = lookLeft();
    delay(300);
    // Decision making based on the clearest path
    if (rightDist > leftDist && rightDist > 25) {
        turnRight();
        delay(400); // Time required to perform the turn
    }
    else if (leftDist > rightDist && leftDist > 25) {
        turnLeft();
        delay(400);
    }
    else {
        // Emergency Case: Both sides are blocked, retreat further and spin around
        moveBackward();
        delay(500);
        turnRight();
        delay(600);
    }
    stopCar();
    delay(200);
  }
  else {
    // Path is completely clear, safe to drive forward
    moveForward();
  }
}

// --- Basic Movement Functions ---
void moveForward() {
  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

}
void moveBackward() {
  digitalWrite(RightForward, LOW);
  digitalWrite(RightBackward, HIGH);
  digitalWrite(LeftForward, LOW);
  digitalWrite(LeftBackward, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}
void turnRight() {
  digitalWrite(RightForward, LOW);
  digitalWrite(RightBackward, HIGH);
  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);
}
void turnLeft() {
  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, LOW);
  digitalWrite(LeftBackward, HIGH);
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);
}

// Diagonal functions allow smoother joystick handling. 
// They process combined directions (e.g., Forward + Left) using single-character 
// Bluetooth commands, preventing data collisions and input lag.

void forwardLeft() {
  analogWrite(ENA, 220);
  analogWrite(ENB, lowSpeed);
  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
}
void forwardRight() {
  analogWrite(ENA, lowSpeed);
  analogWrite(ENB, 220);
  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
}
void backwardLeft() {
  analogWrite(ENA, 220);
  analogWrite(ENB, lowSpeed);
  digitalWrite(RightBackward, HIGH);
  digitalWrite(RightForward, LOW);
  digitalWrite(LeftBackward, HIGH);
  digitalWrite(LeftForward, LOW);
}
void backwardRight() {
  analogWrite(ENA, lowSpeed);
  analogWrite(ENB, 220);
  digitalWrite(RightBackward, HIGH);
  digitalWrite(RightForward, LOW);
  digitalWrite(LeftBackward, HIGH);
  digitalWrite(LeftForward, LOW);
}
void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(RightForward, LOW);
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, LOW);
  digitalWrite(LeftBackward, LOW);
}

// --- Sensor and Servo Functions ---
int getDistance() {
  digitalWrite(trig, LOW); // clean the pin
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // send the waves
  delayMicroseconds(10);
  digitalWrite(trig, LOW); // stop sending the waves
  // Measure the pulse duration with a 26ms timeout to prevent software freezing
  long duration = pulseIn(echo, HIGH, 26000);

  // If timeout occurs (no reflection), assume path is clear (return 100cm)

  if (duration == 0) return 100;
  int d = duration / 58; // Calculate distance in cm
  return d;
}
int lookRight() {
  myservo.write(0); // the degree depends on the servo position might be different from case to case mine midpoint is 65
  delay(600); // Allow sufficient time for the servo motor to reach 0 degrees
  int d = getDistance();
  myservo.write(65); // Return to center position
  delay(400); // Wait for the servo to stabilize at center
  return d;
}
int lookLeft() {
  myservo.write(150);
  delay(600); // Allow sufficient time for the servo motor to reach 150 degrees
  int d = getDistance();
  myservo.write(65); // Return to center position
  delay(400); // Wait for the servo to stabilize at center
  return d; 
}
