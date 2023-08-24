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
//#define LINE_TOKEN "W76Mxz5S2FP4IWEMQJk07mqbGa4TgeGUy1Rb6sIELDe"
#define LINE_TOKEN ""


void setup() {
  // เริ่มต้นการทำงาน Serial
  Serial.begin(115200);

  // แสดง line version
  Serial.println(LINE.getVersion());

  // ต่อ internet WIFI
  WiFi.begin(SSID, PASSWORD);
  // รอ Internet เชื่อมต่อ
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // เชื่อมต่อแล้วแสดงข้อความ
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // ทำการส่ง Line ตามตัวอย่าง
  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("อุณหภูมิ เกินกำหนด");

  // ตัวอย่างส่งข้อมูล ตัวเลข
  LINE.notify(2342);          // จำนวนเต็ม
  LINE.notify(212.43434, 5);  // จำนวนจริง แสดง 5 หลัก

  // เลือก Line Sticker ได้จาก https://devdocs.line.me/files/sticker_list.pdf
  LINE.notifySticker(3, 240);         // ส่ง Line Sticker ด้วย PackageID 3 , StickerID 240
  LINE.notifySticker("Hello", 1, 2);  // ส่ง Line Sticker ด้วย PackageID 1 , StickerID 2  พร้อมข้อความ

  // ตัวอย่างส่ง รูปภาพ ด้วย url
  LINE.notifyPicture("https://preview.ibb.co/j6G51n/capture25610417181915334.png");
  LINE.notifyPicture("จตุธาตุ", "https://www.fotoaparat.cz/storage/pm/09/10/23/670915_a5351.jpg");
}

void loop() {
  delay(1000);
}
