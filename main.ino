#include <SoftwareSerial.h>
SoftwareSerial mysim(10, 11);  // Connect GSM modem on pins 10 (RX) and 11 (TX)

// SMS message strings for different sensor events
String smstextsound;  // SMS message for sound sensor event
String smstextultra;  // SMS message for ultrasonic sensor event
String smstextmotion; // SMS message for motion sensor event
String smstextvibrate;  // SMS message for vibration sensor event
String smstextheight;   // SMS message for height measurement

const int buttonPin = 9;        // Vibration sensor pin
const int soundSensorPin = A0;  // Analog pin for the sound sensor
const int buzzer = 3;           // Digital pin for the LED
const int ledPin = 2;           // Digital pin for the LED
const int pirPin = 12;          // PIR sensor pin
const int trigPin = 5;          // Ultrasonic sensor trig pin
const int echoPin = 6;          // Ultrasonic sensor echo pin
const int trigPin1 = 8;         // Ultrasonic sensor 2 trig pin
const int echoPin1 = 7;         // Ultrasonic sensor 2 echo pin

int motion;                     // Motion sensor variable
long duration;                  // Ultrasonic sensor duration
int distance;                   // Ultrasonic sensor distance
bool objectDetected = false;    // Flag for object detection
float distance_cm;              // Distance in centimeters
float personHeight_ft;          // Person's height in feet
float personHeight_in;          // Person's height in inches
long duration1;                 // Ultrasonic sensor 2 duration

// Function prototypes
void setupGSM();                // Initialize GSM module
void checkVibrationSensor();    // Check vibration sensor
void handleUltrasonicDistance();// Handle ultrasonic distance measurement
int readSoundSensor();          // Read sound sensor value
int readMotionSensor();         // Read motion sensor value
void handleSoundDetected();     // Handle sound sensor event
void measureHeight();           // Measure person's height
void handleMotionDetected();    // Handle motion sensor event
void sendSMS(String message);   // Send SMS function
void updateSerial();            // Update serial communication

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);  // Initialize serial communication
  delay(5000);         // Delay for stability
  setupGSM();          // Initialize GSM module
}

void loop() {
  int sensorValue = readSoundSensor();  // Read sound sensor value
  int soundThreshold = 533;  // Sound threshold for detection
  if (sensorValue > soundThreshold) {  // If sound detected above threshold
    handleSoundDetected();  // Handle sound sensor event
  }

  motion = readMotionSensor();  // Read motion sensor value
  if (motion) {  // If motion detected
    handleMotionDetected();  // Handle motion sensor event
    delay(100);
    handleUltrasonicDistance();  // Handle ultrasonic distance measurement
    delay(100);
    checkVibrationSensor();  // Check vibration sensor
    delay(300);
    measureHeight();  // Measure person's height
    delay(100);
    sensorValue = readSoundSensor();  // Read sound sensor value again
    if (sensorValue > soundThreshold) {  // If sound detected again above threshold
      handleSoundDetected();  // Handle sound sensor event
      delay(100);
    }
  }
  delay(500);  // Delay for stability
}


void setupGSM() {
  mysim.begin(9600);  // Initialize GSM communication
  delay(5000);        // Delay for stability
}

// Ultrasonic sensor distance measurement function
void handleUltrasonicDistance() {
  digitalWrite(trigPin, LOW);  // Ensure trig pin is low
  delayMicroseconds(2);  // Wait for stability
  digitalWrite(trigPin, HIGH);  // Send ultrasonic signal
  delayMicroseconds(10);  // Wait for signal to bounce back
  digitalWrite(trigPin, LOW);  // Turn off trigger

  duration = pulseIn(echoPin, HIGH);  // Measure echo duration
  distance = duration * 0.034 / 2;  // Calculate distance in centimeters

  Serial.print("Distance: ");  // Print distance to serial monitor
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);  // Delay for stability

  if (distance <= 50) {  // If distance is less than or equal to 50 cm
    smstextultra = "\nAlert: The person is approaching the target within 50 cm. Take precautionary measures.";  // Prepare SMS message
    digitalWrite(buzzer, HIGH);  // Turn on buzzer
    delay(2000);  // Delay for buzzer sound
    digitalWrite(buzzer, LOW);  // Turn off buzzer
    delay(1000);  // Delay for stability
    sendSMS(smstextultra);  // Send SMS notification
    Serial.println(smstextultra);  // Print SMS message to serial monitor
    digitalWrite(ledPin, HIGH);  // Turn on LED
  } else if (objectDetected && distance > 50) {  // If object detected and distance is greater than 50 cm
    objectDetected = false;  // Reset object detection flag
    Serial.println("The person left 50 cm away from the target.");  // Print message to serial monitor
    delay(100);  // Delay for stability
    digitalWrite(ledPin, LOW);  // Turn off LED
  }
}

// Read sound sensor function
int readSoundSensor() {
  return analogRead(soundSensorPin);  // Read sound sensor analog value and return
}

// Read motion sensor function
int readMotionSensor() {
  return digitalRead(pirPin);  // Read motion sensor digital value and return
}

// Vibration sensor handling function
void checkVibrationSensor() {
  if (digitalRead(buttonPin) == HIGH) {  // If vibration detected
    smstextvibrate = "\nIntruder detected. Immediate action required!";  // Prepare SMS message
    sendSMS(smstextvibrate);  // Send SMS notification
    digitalWrite(buzzer, HIGH);  // Turn on buzzer
    delay(3000);  // Delay for buzzer sound
    digitalWrite(buzzer, LOW);  // Turn off buzzer
    
    mysim.println("ATD+234...;");  // Make a call
    delay(30000);  // Wait for call duration
    mysim.println("ATH");  // Hang up call
  }
}

// Sound sensor handling function
void handleSoundDetected() {
  digitalWrite(buzzer, HIGH);  // Turn on buzzer
  digitalWrite(ledPin, HIGH);  // Turn on LED
  delay(1000);  // Delay for buzzer and LED
  digitalWrite(buzzer, LOW);  // Turn off buzzer
  digitalWrite(ledPin, LOW);  // Turn off LED
  delay(1500);  // Delay for stability
  smstextsound = "\nSound detected in the vicinity.";  // Prepare SMS message
  sendSMS(smstextsound);  // Send SMS notification
  Serial.println(smstextsound);  // Print SMS message to serial monitor
}

// Ultrasonic sensor 2 height measurement function
void measureHeight() {
  digitalWrite(trigPin1, LOW);  // Ensure trig pin is low
  delayMicroseconds(2);  // Wait for stability
  digitalWrite(trigPin1, HIGH);  // Send ultrasonic signal
  delayMicroseconds(10);  // Wait for signal to bounce back
  digitalWrite(trigPin1, LOW);  // Turn off trigger
  duration1 = pulseIn(echoPin1, HIGH);  // Measure echo duration
  distance_cm = duration1 * 0.0343 / 2;  // Calculate distance in centimeters

  distance_cm = max(0, distance_cm);  // Ensure distance is non-negative

  float personHeight_diff_cm = 205.2 - distance_cm;  // Calculate person's height difference from ground

  int height_ft = int(personHeight_diff_cm / 30.48);  // Convert height from cm to feet
  int height_in = (personHeight_diff_cm / 30.48 - height_ft) * 12;  // Convert remaining distance to inches

  smstextheight = "\nPerson's height measured: " + String(height_ft) + " feet " + String(height_in) + " inches";  // Prepare SMS message
  sendSMS(smstextheight);  // Send SMS notification
  Serial.println(smstextheight);  // Print SMS message to serial monitor
}

// Motion sensor handling function
void handleMotionDetected() {
  objectDetected = true;  // Set object detection flag
  smstextmotion = "\nMotion detected nearby. Please investigate.";  // Prepare SMS message
  digitalWrite(ledPin, HIGH);  // Turn on LED
  sendSMS(smstextmotion);  // Send SMS notification
  Serial.println(smstextmotion);  // Print SMS message to serial monitor
  handleUltrasonicDistance();  // Handle ultrasonic distance measurement
}

// Send SMS function with AT commands
void sendSMS(String message) {
  mysim.print("AT+CMGF=1\r");  // Set SMS mode to text
  updateSerial();  // Update serial communication
  delay(1000);  // Delay for stability

  mysim.println("AT + CMGS = \"+234.....\"");  // Set SMS destination number
  updateSerial();  // Update serial communication
  delay(1000);  // Delay for stability

  mysim.println(message);  // Send SMS message
  updateSerial();  // Update serial communication
  delay(1000);  // Delay for stability

  mysim.println((char)26);  // Send Ctrl+Z to indicate end of SMS
  updateSerial();  // Update serial communication
  delay(1000);  // Delay for stability

  mysim.println();  // Send new line character
  delay(100);  // Delay for stability
}

// Update serial communication function
void updateSerial() {
  delay(500);  // Delay for stability
  while (Serial.available()) {
    mysim.write(Serial.read());  // Read from serial and write to GSM module
  }
  while (mysim.available()) {
    Serial.write(mysim.read());  // Read from GSM module and write to serial
  }
}