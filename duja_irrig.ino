#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3); // RX, TX (HC-05 TX -> D2, RX -> D3 via voltage divider)

const int trigPin = 10;
const int echoPin = 11;
const int soilPin = A0;     // Analog input for soil moisture sensor
const int relayPin = 7;

long duration;
int distanceCm;
int soilValue;              // Raw sensor reading (0–1023)
int soilPercent;            // Converted to % (0–100%)

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(soilPin, INPUT);

  digitalWrite(relayPin, HIGH); // Pump OFF by default

  Serial.println("Smart Irrigation Ready");
  BT.println("Smart Irrigation Ready");
}

void loop() {
  // --- SOIL SENSOR ---
  soilValue = analogRead(soilPin);
  soilPercent = map(soilValue, 1023, 0, 0, 100); // convert to percentage (dry=0%, wet=100%)

  // --- ULTRASONIC SENSOR ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;

  // --- DISPLAY & BLUETOOTH ---
  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  BT.print("Soil Moisture: ");
  BT.print(soilPercent);
  BT.println("%");

  BT.print("Distance(cm): ");
  BT.println(distanceCm);

  // --- WATER CONTROL LOGIC ---
{  if (soilPercent < 40) {         // If soil is too dry
    digitalWrite(relayPin, LOW);  // Turn pump ON
    BT.println("Pump: ON (Dry Soil)");
  } else {
    digitalWrite(relayPin, HIGH); // Turn pump OFF
    BT.println("Pump: OFF (Soil Moist)");
  }

  delay(1500);
}
}
