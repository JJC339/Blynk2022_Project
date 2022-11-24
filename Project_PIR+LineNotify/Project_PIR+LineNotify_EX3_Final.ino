/*
  DIY of Things - Project Base Learning
  Project PIR+LineNotify
  2022-11-24

  >> ไลบรารี่ TridentTD_LineNotify <<
*/

// เพิ่ม library เข้ามาใช้งานในโค้ด
#include <TridentTD_LineNotify.h>

// กำหนด wifi pass lineToken
#define SSID ""
#define PASSWORD ""
#define LINE_TOKEN ""

#define ON LOW
#define OFF HIGH

const int pirPin = D1; 
const int ledPin = D4;   

int pirState = 0;

void setup() {
  // เริ่มต้นการทำงาน Serial
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  // แสดง line version
  Serial.println(LINE.getVersion());

  // ต่อ internet WIFI
  WiFi.begin(SSID, PASSWORD);
  // รอ Internet เชื่อมต่อ
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }

  // เชื่อมต่อแล้วแสดงข้อความ
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // ทำการส่ง Line ตามตัวอย่าง
  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("เริ่มการทำงาน");

  // ตัวอย่างส่งข้อมูล ตัวเลข
  //LINE.notify(2342);          // จำนวนเต็ม
  //LINE.notify(212.43434, 5);  // จำนวนจริง แสดง 5 หลัก

  // เลือก Line Sticker ได้จาก https://devdocs.line.me/files/sticker_list.pdf
  //LINE.notifySticker(3, 240);         // ส่ง Line Sticker ด้วย PackageID 3 , StickerID 240
  //LINE.notifySticker("Hello", 1, 2);  // ส่ง Line Sticker ด้วย PackageID 1 , StickerID 2  พร้อมข้อความ

  // ตัวอย่างส่ง รูปภาพ ด้วย url
  //LINE.notifyPicture("https://preview.ibb.co/j6G51n/capture25610417181915334.png");
  //LINE.notifyPicture("จตุธาตุ", "https://www.fotoaparat.cz/storage/pm/09/10/23/670915_a5351.jpg");
}

void loop() {
  // อ่านค่าสถานะของขา PIR เก็บลงตัวแปล pirState
  pirState = digitalRead(pirPin);
  // สั่ง print ออกมาดูค่า
  Serial.println(pirState);

  // ตรวจสอบค่า
  if (pirState == HIGH) {
    digitalWrite(ledPin, ON);
    LINE.notifySticker("ตรวจพบบางอย่าง", 6359, 11069861); 
    // https://developers.line.biz/en/docs/messaging-api/sticker-list/#sticker-definitions
  } else {
    digitalWrite(ledPin, OFF);
  }

  // หน่วงเวลา
  delay(5000);
}
