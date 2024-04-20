#include <SoftwareSerial.h>
SoftwareSerial mysim(10, 11);  // GSM modem pins

String smstextsound;
String smstextultra;
String smstextmotion;

const int soundSensorPin = A0; // Sound sensor pin
const int pirPin = 12;         // Motion sensor pin
const int trigPin = 5;         // Ultrasonic sensor trig pin
const int echoPin = 6;         // Ultrasonic sensor echo pin
const int buzzer = 3;          // Buzzer pin
const int ledPin = 2;          // LED pin

int motion;
long duration;
int distance;
bool objectDetected = false;

unsigned long lastSoundTime = 0;
unsigned long lastMotionTime = 0;
const unsigned long soundInterval = 1000;  // Interval between sound checks in milliseconds
const unsigned long motionInterval = 200;  // Interval between motion checks in milliseconds

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  mysim.begin(9600);
  delay(5000);
}

void loop() {
  unsigned long currentMillis = millis();

  // Sound Sensor Check
  if (currentMillis - lastSoundTime >= soundInterval) {
    lastSoundTime = currentMillis;
    int sensorValue = readSoundSensor();
    if (sensorValue > 533) {
      handleSoundDetected();
    }
  }

  // Motion Sensor Check
  if (currentMillis - lastMotionTime >= motionInterval) {
    lastMotionTime = currentMillis;
    motion = readMotionSensor();
    if (motion) {
      handleMotionDetected();
    }
  }
}

int readSoundSensor() {
  return analogRead(soundSensorPin);
}

int readMotionSensor() {
  return digitalRead(pirPin);
}

void handleSoundDetected() {
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
  smstextsound = "\nSound sensed...!!!";
  sendSMS(smstextsound);
  Serial.println(smstextsound);
}

void handleMotionDetected() {
  objectDetected = true;
  smstextmotion = "\nAlert1: Someone is around...";
  digitalWrite(buzzer, HIGH);
  delay(1500);
  digitalWrite(buzzer, LOW);
  delay(2000);
  sendSMS(smstextmotion);
  Serial.println(smstextmotion);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 50) {
    smstextultra = "\nAlert2: The person is 50 cm closer to the target...";
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
    sendSMS(smstextultra);
    Serial.println(smstextultra);
    digitalWrite(ledPin, HIGH);
  } else if (objectDetected && distance > 50) {
    objectDetected = false;
    Serial.println("The person left 50 cm away from the target.");
    digitalWrite(ledPin, LOW);
  }
}

void sendSMS(String message) {
  mysim.print("AT+CMGF=1\r");
  delay(100);
  mysim.println("AT + CMGS = \"+2347069254588\"");
  delay(100);
  mysim.println(message);
  delay(100);
  mysim.println((char)26);
  delay(100);
  mysim.println();
  delay(100);
}

void updateSerial() {
  while (Serial.available()) {
    mysim.write(Serial.read());
  }
  while (mysim.available()) {
    Serial.write(mysim.read());
  }
}
