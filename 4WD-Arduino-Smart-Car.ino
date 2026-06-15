#include <Servo.h>

/*
 * Obstacle Avoiding Robot Car
 * ----------------------------------
 * Hardware:
 * - Arduino Uno
 * - HC-SR04 Ultrasonic Sensor
 * - SG90 Servo Motor
 * - L298N Motor Driver
 * - 4 DC Motors
 *
 * Features:
 * - Detects obstacles using an ultrasonic sensor.
 * - Automatically slows down when approaching an obstacle.
 * - Reverses and scans both sides when an obstacle is too close.
 */

// Ultrasonic Sensor Pins
const int trigPin = 11;
const int echoPin = 12;

// Motor Driver Pins (L298N)
const int RightForward  = 8; // IN1
const int RightBackward = 7; // IN2
const int LeftBackward  = 4; // IN3
const int LeftForward   = 2; // IN4

// PWM pins for motor speed control
const int ENA = 5; // Right motor speed
const int ENB = 6; // Left motor speed

Servo myServo;

int speed = 75;     // Default reverse speed
int distance = 0;   // Current measured distance

void setup() {

  // Configure motor control pins
  pinMode(RightForward, OUTPUT);
  pinMode(RightBackward, OUTPUT);
  pinMode(LeftForward, OUTPUT);
  pinMode(LeftBackward, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Configure ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach servo motor
  myServo.attach(3);

  // Center position of the sensor
  myServo.write(70);

  // Safety delay before starting
  delay(2000);
}

void loop() {

  // Measure distance in front of the robot
  distance = getDistance();

  // Obstacle detected at close range
  if (distance <= 25) {

    stopCar();
    delay(200);

    // Move backward slightly to create turning space
    moveBackward();
    delay(400);

    stopCar();

    // Scan both directions
    int rightDist = lookRight();
    delay(200);

    int leftDist = lookLeft();
    delay(200);

    // Turn toward the side with more available space
    if (rightDist >= leftDist) {

      turnRight();
      delay(522);

    } else {

      turnLeft();
      delay(522);
    }

    stopCar();
    delay(200);
  }

  // Long clear path ahead -> move faster
  else if (distance > 100) {

    moveForwardFast();
  }

  // Medium distance -> move slowly
  else {

    moveForward();
  }
}


// Movement Functions
/*
 * Move forward at low speed
 */
void moveForward() {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);

  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
}

/*
 * Move forward at high speed
 */
void moveForwardFast() {

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);

  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);

  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
}

/*
 * Move backward
 */
void moveBackward() {

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);

  digitalWrite(RightForward, LOW);
  digitalWrite(RightBackward, HIGH);

  digitalWrite(LeftForward, LOW);
  digitalWrite(LeftBackward, HIGH);
}

/*
 * Rotate robot to the right
 */
void turnRight() {

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);

  // Right wheel backward
  digitalWrite(RightForward, LOW);
  digitalWrite(RightBackward, HIGH);

  // Left wheel forward
  digitalWrite(LeftForward, HIGH);
  digitalWrite(LeftBackward, LOW);
}

/*
 * Rotate robot to the left
 */
void turnLeft() {

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);

  // Right wheel forward
  digitalWrite(RightForward, HIGH);
  digitalWrite(RightBackward, LOW);

  // Left wheel backward
  digitalWrite(LeftForward, LOW);
  digitalWrite(LeftBackward, HIGH);
}

/*
 * Stop all motors
 */
void stopCar() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(RightForward, LOW);
  digitalWrite(RightBackward, LOW);

  digitalWrite(LeftForward, LOW);
  digitalWrite(LeftBackward, LOW);
}

// Sensor Functions
/*
 * Measure distance using HC-SR04
 * Returns distance in centimeters.
 * If no valid reading is received,
 * returns 400 cm as a default value.
 */
int getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  int d = duration * 0.034 / 2;

  return (d <= 0) ? 400 : d;
}

/*
 * Scan right side and return distance
 */
int lookRight() {

  myServo.write(5);
  delay(600);

  int d = getDistance();

  // Return sensor to center
  myServo.write(70);
  delay(200);

  return d;
}

/*
 * Scan left side and return distance
 */
int lookLeft() {

  myServo.write(150);
  delay(600);

  int d = getDistance();

  // Return sensor to center
  myServo.write(70);
  delay(200);

  return d;
}
