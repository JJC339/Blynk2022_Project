#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLaqxCWyEz"
#define BLYNK_DEVICE_NAME "WaterLevel"
#define BLYNK_AUTH_TOKEN ""
#define LINE_TOKEN  ""
#define TRIG_PIN D5
#define ECHO_PIN D6
#define SDA_PIN D2
#define SCL_PIN D1
#define RAIN_PIN D7

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <TridentTD_LineNotify.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";
long duration, distance;
bool rainStatus,rainAlert = 0;
int lvSetup,lv1,lv2,lv3,lvNow,alertStatus;
BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  // Debug console
  Serial.begin(115200);
  delay(2000);
  Serial.println();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);

  lcd.init();  
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Water LV      CM");
  lcd.setCursor(0,1);
  lcd.print("Rain Status: ");
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L,checkStatus);

  LINE.setToken(LINE_TOKEN);
  LINE.notify("water level monitoring system start");
}

void loop()
{
  Blynk.run();
  timer.run();
}

void checkStatus()
{
  Serial.print("Rain status: ");
  getRainStatus();
  Serial.println(rainStatus);
  
  Serial.print("Distant: ");
  getDistant();
  Serial.println(distance);
  processData();
  Serial.println(lvNow);
  Serial.println(alertStatus);
  updateDisplay();

  /*
  Serial.println(lvSetup);
  Serial.println(lv1);
  Serial.println(lv2);
  Serial.println(lv3);
  */
}

long getDistant()
{
    digitalWrite(TRIG_PIN, LOW);  
    delayMicroseconds(2); 
    
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10); 
    
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration/2) / 29.1;

    return distance;
}

bool getRainStatus()
{
  rainStatus = !digitalRead(RAIN_PIN);
  return rainStatus;
}

void processData()
{
  lvNow = lvSetup - distance;

  if(lvNow < lv1)
  {
    Serial.println("normal");
    alertStatus = 0;
  }
  else if(lvNow >= lv1 && lvNow < lv2)
  {
    if(alertStatus != 1)
    {
      alertStatus = 1;
      Serial.println("lv 1");
      LINE.notifySticker(789,10888);
      LINE.notify("ตรวจพบระดับน้ำเพิ่มขึ้น");
      LINE.notify(alertStatus);
      LINE.notify(lvNow);
    }
  }
  else if(lvNow >= lv2 && lvNow < lv3)
  {
    if(alertStatus != 2)
    {
      alertStatus = 2;
      Serial.println("lv 2");
      LINE.notifySticker(789,10888);
      LINE.notify("ตรวจพบระดับน้ำเพิ่มขึ้น");
      LINE.notify(alertStatus);
      LINE.notify(lvNow);
    }
  }
  else if(lvNow >= lv3)
  {
    if(alertStatus != 3)
    {
      alertStatus = 3;
      Serial.println("lv 3");
      LINE.notifySticker(789,10888);
      LINE.notify("ตรวจพบระดับน้ำเพิ่มขึ้น");
      LINE.notify(alertStatus);
      LINE.notify(lvNow);
    }
  }

  if(rainStatus == 1)
  {
    if(rainAlert == 0)
    {
      rainAlert = 1;
      Serial.println("ฝนตก");
      LINE.notifySticker(789,10888);
      LINE.notify("ฝนตก");
    }
  }else{
    if(rainAlert != 0)
    {
      rainAlert = 0;
    }
  }
}

void updateDisplay()
{
  lcd.setCursor(10,0);
  lcd.print("    ");
  lcd.setCursor(13,1);
  lcd.print("    ");
  
  lcd.setCursor(10,0);
  lcd.print(lvNow);
  lcd.setCursor(13,1);
  lcd.print(rainStatus);
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
}

BLYNK_WRITE(V0)
{
  lvSetup = param.asInt();
}

BLYNK_WRITE(V1)
{
  lv1 = param.asInt();
}

BLYNK_WRITE(V2)
{
  lv2 = param.asInt();
}

BLYNK_WRITE(V3)
{
  lv3 = param.asInt();
}
