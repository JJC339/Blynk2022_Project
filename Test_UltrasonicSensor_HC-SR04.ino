// ---------------------------------------------------------------- //
// DIY of Things
// Ultrasonic Sensor HC-SR04 Test Code
// 2022-12-18
// original by : https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6
// ---------------------------------------------------------------- //

#define echoPin D6 // กำหนดขา echo pin ที่ D6
#define trigPin D5 // กำหนดขา trig pin ที่ D5

// สร้างตัวแปล
long duration; // ตัวแปลสำหรับรับค่ามิลิวินาที
int distance; // ตัวแปลระยะทาง

void setup() {
  Serial.begin(115200);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");

  pinMode(trigPin, OUTPUT); // กำหนดขา output
  pinMode(echoPin, INPUT); // กำหนนดขา input
}

void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  // The distance travelled by the sound send out is duration divided by the
  // pace of sound which is 29.1 ms/cm. We divide by 29.1 to get the amount
  // of centimeters and we divide by two because the sound travel to the object
  // and back, so the distance to the object is half the distance traveled
  // by the sound.
  distance = (duration/2) / 29.1;
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(400);
}