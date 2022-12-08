#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define SENSOR1_PIN 3
#define SENSOR2_PIN 5
#define SENSOR3_PIN 7

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;
WidgetLED alertLED(V4);


#include <TridentTD_LineNotify.h>
#define LINE_TOKEN  "EEBqJ5uRLaX9SMCeMct5APAMlvUHDChIWMf8xfyf9ca"

int sensor1,sensor2,sensor3,sensorAve,limit,alert = 0;

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V3);
}

BLYNK_WRITE(V3)
{
  Serial.print("setup limit ppm value to ");
  limit = param.asInt();
  Serial.print(limit);
  Serial.println("ppm");
}

void setup()
{
  Serial.begin(115200);  
  analogReadResolution(10);                             
  delay(10000);
  Serial.print("Start...\n");
  LINE.setToken(LINE_TOKEN);              

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L,updateSensor);
}

void loop()
{
   Blynk.run();
   timer.run();
}

void updateSensor()
{
  Serial.println("update sensor");
  sensor1 = analogRead(SENSOR1_PIN);
  sensor2 = analogRead(SENSOR2_PIN);
  sensor3 = analogRead(SENSOR3_PIN);

  Serial.print(sensor1);
  Serial.print(",");
  Serial.print(sensor2);
  Serial.print(",");
  Serial.println(sensor3);

  Blynk.virtualWrite(V0,sensor1);
  Blynk.virtualWrite(V1,sensor2);
  Blynk.virtualWrite(V2,sensor3);

  sensorAve = (sensor1+sensor2+sensor3)/3;
  Serial.print("Sensr Average... ");
  Serial.println(sensorAve);
  
  Serial.println();
  Serial.println("Start compare...");
  if(sensorAve > limit)
  {
    alertLED.on();
    LINE.notifySticker(789,10888);
    LINE.notify("ตรวจพบควันเกินค่ากำหนด!!");
    LINE.notify(sensorAve);
  }
  else
  {
    alertLED.off();
  }
}
