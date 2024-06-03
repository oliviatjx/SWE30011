#include <Servo.h>

// Define pin connections
const int irSensorPin = A0;
const int trigPin = 6;
const int echoPin = 5;
const int ledPin = 4;
const int servoPin = 3;

// Create Servo object
Servo myServo;

// Variables
int irSensorValue = 0;
long duration;
int distance;

// Array to store IR sensor values
const int IR_ARRAY_SIZE = 10; // Adjust this value to set the array size
int irSensorArray[IR_ARRAY_SIZE] = {0}; // Initialize with zeros
int irArrayIndex = 0; // Index for storing values in the array

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Attach servo motor to pin
  myServo.attach(servoPin);
  myServo.write(160); // Initial position

  // Start with LED off
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Measure distance with the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // If the ultrasonic sensor detects something within 2cm
  if (distance <= 20) {
    // Move the servo motor
    myServo.write(30); 
    delay(500); 
    myServo.write(160); 
    delay(500); 
  } else {
    // Return servo to initial position
    myServo.write(160);
  }

  // Read IR sensor value (analog)
  irSensorValue = analogRead(irSensorPin);

  // Store the IR sensor value in the array
  irSensorArray[irArrayIndex] = irSensorValue;
  irArrayIndex = (irArrayIndex + 1) % IR_ARRAY_SIZE; // Increment and wrap around

  // If the IR sensor detects something (value above threshold)
  if (irSensorValue > 240) {
    // Turn on LED
    digitalWrite(ledPin, HIGH);
    delay(1000);
  } else {
    // Turn off LED
    digitalWrite(ledPin, LOW);
  }

  // Send data to serial monitor in a single line
  Serial.print(distance);
  Serial.print(", ");
  Serial.println(irSensorValue);

  // Read and handle response from the server (if any)
  if (Serial.available() > 0) {
    String response = Serial.readStringUntil('\n');
    Serial.println(response);
    if (response.equals("LED_OFF")) {
      digitalWrite(ledPin, LOW);
      delay(2000);
    }

    char command = Serial.read();
    if (command = 'L')
    {
      digitalWrite(ledPin, LOW);
      delay(2000);
    }

  }

  // Small delay before next loop
  delay(100);
}