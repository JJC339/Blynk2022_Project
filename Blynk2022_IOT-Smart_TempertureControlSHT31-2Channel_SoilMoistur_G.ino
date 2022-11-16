/*
 * DIYofThings
 * CODE FOR EXPERMENTAL ONLY
 * Contract Us : https://www.facebook.com/diyofthings
 * Contract Us : https://www.youtube.com/diyofthings
 * Only Shop   : https://shopee.co.th/diyofthings.shop
*/

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// define PIN Mapping
#define PIN_RELAY1  D5  // relay 1 HEATER
#define PIN_RELAY2  D6  // relay 2 PUMP
#define I2C_SDA     D2  
#define I2C_SCL     D1  
#define SOIL_MOISTU D2  
#define ON          LOW
#define OFF         HIGH

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Adafruit_SHT31.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

Adafruit_SHT31 sht31 = Adafruit_SHT31();

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

int   soilMoistureData    = 0;
int   soilMoisturePercen  = 0;
int   soilMoistureLow     = 0;
int   soilMoistureHigh    = 1023;
float tempSensorData      = 0;
float humiSensorData      = 0;
bool  autoCondition       = 0;
int   limitHigh           = 35;
int   limitLow            = 33;

BlynkTimer timer;

BLYNK_WRITE(V2)
{
  limitLow = param.asInt();
}

BLYNK_WRITE(V3)
{
  limitHigh = param.asInt();
}

BLYNK_WRITE(V4)
{
  autoCondition = param.asInt();

  if(autoCondition == 0)
  {
    digitalWrite(PIN_RELAY1,OFF);
    digitalWrite(PIN_RELAY2,OFF);
    Blynk.virtualWrite(V5,LOW);
    Blynk.virtualWrite(V6,LOW);
  }
}

BLYNK_WRITE(V5)
{
  if(!autoCondition)
  {
    if(param.asInt())
    {
      digitalWrite(PIN_RELAY1,ON);
    }
    else
    {
      digitalWrite(PIN_RELAY1,OFF);
    }
  }
  else
  {
    Blynk.virtualWrite(V5,!param.asInt());
  }
}

BLYNK_WRITE(V6)
{
  if(!autoCondition)
  {
    if(param.asInt())
    {
      digitalWrite(PIN_RELAY2,ON);
    }
    else
    {
      digitalWrite(PIN_RELAY2,OFF);
    }
  }
  else
  {
    Blynk.virtualWrite(V6,!param.asInt());
  }
}

BLYNK_WRITE(V7)
{
  soilMoistureLow = param.asInt();
}

BLYNK_WRITE(V8)
{
  soilMoistureHigh = param.asInt();
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
  Blynk.syncVirtual(V2,V3,V4,V7,V8);
}

int readSensorMoisture()
{
  return analogRead(A0);
}

void readSensorTempHumi(float* temp,float* humi)
{
  *temp = sht31.readTemperature();
  *humi = sht31.readHumidity();
}

void updateLCD()
{
    if(autoCondition)
    {
      lcd.setCursor(13,1);
      lcd.print("[A]");
    }
    else
    {
      lcd.setCursor(13,1);
      lcd.print("[M]");
    }
    lcd.setCursor(5,1);
    lcd.print("    ");
    lcd.setCursor(5,1);
    lcd.print(soilMoisturePercen);
    lcd.print("%RH");
    lcd.setCursor(2,0);
    lcd.print(tempSensorData);
    lcd.setCursor(10,0);
    lcd.print(humiSensorData);
}

void updateBlynk()
{
  Blynk.virtualWrite(V0,soilMoistureData);
  Blynk.virtualWrite(V1,soilMoisturePercen);
  Blynk.virtualWrite(V9,tempSensorData);
  Blynk.virtualWrite(V10,humiSensorData);
}

void programProcess()
{
  if(autoCondition)
  {
    Serial.println(limitHigh);
    Serial.println(limitLow);
    Serial.println(soilMoisturePercen);

    if(soilMoisturePercen < limitLow)
    {
      //เปิดปั้มน้ำ
      Serial.println("Pump On");
      digitalWrite(PIN_RELAY1,ON);
      digitalWrite(PIN_RELAY2,ON);
      Blynk.virtualWrite(V5,HIGH);
      Blynk.virtualWrite(V6,HIGH);
    }
  
    if(soilMoisturePercen > limitLow)
    {
      //ปิดป้มน้ำ
      Serial.println("Pump Off");
      digitalWrite(PIN_RELAY1,OFF);
      digitalWrite(PIN_RELAY2,OFF);
      Blynk.virtualWrite(V5,LOW);
      Blynk.virtualWrite(V6,LOW);
    }
  }
}

void updateData()
{
  Serial.println("updateData start");
  Serial.println("update soil moisutre sensor...");
  soilMoistureData = readSensorMoisture();
  soilMoisturePercen = map(soilMoistureData,soilMoistureHigh,soilMoistureLow,0,100);

  Serial.println("update temp and humidity sensor...");
  readSensorTempHumi(&tempSensorData, &humiSensorData);

  Serial.println("update LCD data");
  updateLCD();

  Serial.println("update Blynk data");
  updateBlynk();

  Serial.println("update Condition");
  programProcess();
}

void setup()
{
  // Set Pin Mode Output
  pinMode(PIN_RELAY1, OUTPUT);     
  pinMode(PIN_RELAY2  , OUTPUT);   
  
  // Set default PIN Value HIG=0,LOW=1
  digitalWrite(PIN_RELAY1 , OFF);
  digitalWrite(PIN_RELAY2 , OFF);
  
  // Debug console
  Serial.begin(115200);
  sht31.begin(0x44);
  
  lcd.init();  
  lcd.backlight();
  lcd.setCursor(0,1);
  lcd.print("Soil:  %RH");
  lcd.setCursor(13,1);
  lcd.print("[-]");
  lcd.setCursor(0,0);
  lcd.print("T:      H:   ");
  
  Blynk.begin(auth, ssid, pass);
  //updateSensor();
  timer.setInterval(5000L,updateData);
  //timer.setInterval(1000L,programProcess);
  //timer.setInterval(3000L,updateLCD);
}

void loop()
{
  Blynk.run();
  timer.run();
}
