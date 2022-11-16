/*
 * DIYofThings
 * CODE FOR EXPERMENTAL ONLY
 * Contract Us : https://www.facebook.com/diyofthings
 * Contract Us : https://www.youtube.com/diyofthings
 * Only Shop   : https://shopee.co.th/diyofthings.shop
*/

#define BLYNK_PRINT Serial
#define GATE_PIN D6

// กำหนดค่าเริ่มต้นของ Blynk
// ในส่วนนี้ copy จากเว็บ Blynk Dashboard
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// กำหนดค่า Wifi และ Pass
char ssid[] = "";
char pass[] = "";

// #include ไลบารีเกี่ยวข่องในการเขียนโค้ด
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

Servo petGate;
int gateStart     = 180;
int gateEnd       = 0;
int gateOpenTime  = 1000;
int gateLoop      = 1;

BLYNK_CONNECTED()
{
  Serial.println("Blynk Connected Sync...");
  //Blynk.syncAll();
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
}

BLYNK_WRITE(V0)
{
  Serial.println("Feeding...");
  if(param.asInt() == 1)
  {
    for(int i=1;i<=gateLoop;i++)
    {
      petGate.write(gateEnd);
      delay(gateOpenTime);
      petGate.write(gateStart);
      delay(gateOpenTime);
    }
  }
}

BLYNK_WRITE(V1)
{
  Serial.print("set feed time: ");
  Serial.println(param.asInt());
  gateLoop = param.asInt();
}

void setup()
{
  // กำหนดความเร็ว Serial
  Serial.begin(115200);

  petGate.attach(GATE_PIN);
  petGate.write(gateStart);
  
  // เริ่มทำการเชื่อมต่อ Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
}
