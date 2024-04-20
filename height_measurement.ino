#include <SoftwareSerial.h>

SoftwareSerial mysim(10, 11); // GSM modem pins
String smstext4;

const int trigPin1 = 8; // Ultrasonic sensor trig pin
const int echoPin1 = 7; // Ultrasonic sensor echo pin

long duration1; // Time taken for ultrasonic wave to travel
int distance_cm; // Distance measured by ultrasonic sensor

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
}

void loop() {
  // Height measurement
  int height_ft, height_in;
  measureHeight(height_ft, height_in);

  // Send SMS of person's height
  smstext4 = "\nPerson's Height: " + String(height_ft) + " ft " + String(height_in) + " in";
  sendSMS(smstext4);
  Serial.println(smstext4);

  delay(5000); // Delay between measurements
}

void measureHeight(int& personHeight_ft, int& personHeight_in) {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance_cm = duration1 * 0.0343 / 2;

  // Ensure distance is non-negative
  distance_cm = max(0, distance_cm);

  float personHeight_diff_cm = 205.2 - distance_cm;

  personHeight_ft = int(personHeight_diff_cm / 30.48); // Convert height from cm to feet (1 foot = 30.48 cm)
  personHeight_in = (personHeight_diff_cm / 30.48 - personHeight_ft) * 12; // Convert remaining distance to inches
}

void sendSMS(String message) {
  mysim.print("AT+CMGF=1\r");
  updateSerial();
  delay(1000);
  
  mysim.println("AT + CMGS = \"+2347069254588\"");
  updateSerial();
  delay(1000);
  
  mysim.println(message);
  updateSerial();
  delay(1000);
  
  mysim.println((char)26);
  updateSerial();
  delay(1000); 
  
  mysim.println();
  delay(100); // Give module time to send SMS
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mysim.write(Serial.read());
  }
  while (mysim.available()) {
    Serial.write(mysim.read());
  }
}
