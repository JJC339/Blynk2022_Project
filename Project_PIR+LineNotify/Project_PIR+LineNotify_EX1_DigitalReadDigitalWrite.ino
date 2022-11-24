/*
  DIY of Things - Project Base Learning
  Project PIR+LineNotify
  2022-11-24
*/

//  กำหนดค่าคงที่สำหรับปิดเปิดอุปกรณ์
#define ON LOW
#define OFF HIGH

const int pirPin = D1; 
const int ledPin = D4;   

int pirState = 0;

void setup() {
  // กำหนดการทำงานของขา GPIO
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  // เริ่มการทำงาน Serial 
  Serial.begin(115200);
}

void loop() {
  // อ่านค่าสถานะของขา PIR เก็บลงตัวแปล pirState
  pirState = digitalRead(pirPin);
  // สั่ง print ออกมาดูค่า
  Serial.println(pirState);

  // ตรวจสอบค่า
  if (pirState == HIGH) {
    digitalWrite(ledPin, ON);
  } else {
    digitalWrite(ledPin, OFF);
  }

  // หน่วงเวลา
  delay(1000);
}
