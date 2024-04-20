#include <SoftwareSerial.h>

SoftwareSerial mysim(10, 11); // GSM modem pins

String smstextvibrate;
const int buttonPin = 9;     // Vibration sensor pin
const int buzzer =  3;       // Buzzer pin

// Function prototypes
void setupGSM();
void checkVibrationSensor();

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  delay(5000);
  setupGSM(); // Initialize GSM module
}

void loop() {
  checkVibrationSensor(); // Check vibration sensor
  delay(100); // Small delay for stability
}

void setupGSM() {
  mysim.begin(9600);
  delay(5000);
}

void checkVibrationSensor() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    smstextvibrate = "\nIntruder detected!!!";
    sendSMS(smstextvibrate);
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
    
    // Make a call
    mysim.println("ATD+2348107760338;"); // Phone number to dial
    delay(3000); // Wait for 3 seconds before hanging up
    mysim.println("ATH"); // Hang up call
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
