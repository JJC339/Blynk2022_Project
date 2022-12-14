/*
** DIY of Things
** Learning With MQ-2 Sensor Smoke and Gas Detector
** 2022-12-14
*/

#define LED D6
#define ALARM D7
#define SMOKE A0
#define ALARM_ON LOW
#define ALARM_OFF HIGH

int limitAlert = 500; //1024
int sampleCount = 5;
int sampleDelay = 200; //1sec = 1000ms
int sampleTemp = 0;
int smokeValue = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.print("Starting... ");
  Serial.println("Done!!");

  pinMode(LED,OUTPUT);
  pinMode(ALARM,OUTPUT);
  digitalWrite(LED,LOW);
  digitalWrite(ALARM,ALARM_OFF);

  Serial.println("Test LED...");
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  delay(2000);

  Serial.println("Test ALARM...");
  digitalWrite(ALARM,ALARM_ON);
  delay(1000);
  digitalWrite(ALARM,ALARM_OFF);
  delay(2000);

  Serial.println("Test Sensor");
  for(int i=0;i<5;i++)
  {
    Serial.print("analog value: ");
    Serial.println(analogRead(SMOKE));
    delay(1000);
  }
  
  Serial.println("Starting loop...");
}

void loop() {
  sampleTemp = 0;
  smokeValue = 0;


  // sample sensor value
  for(int i=0; i<sampleCount; i++)
  {
    sampleTemp = sampleTemp+analogRead(SMOKE);
    delay(sampleDelay);
  }

  smokeValue = sampleTemp/sampleCount;
  Serial.print("Smoke Sensor Sample Value: ");
  Serial.println(smokeValue);

  // compare
  if(smokeValue > limitAlert)
  {
    Serial.println("Smoke Value Over Limit!!");
    digitalWrite(ALARM,ALARM_ON);
    digitalWrite(LED,HIGH);
    delay(1000);
    digitalWrite(LED,LOW);
    delay(1000);
  }
  else
  {
    Serial.println("No Smoke");
    digitalWrite(ALARM,ALARM_OFF);
  }
}
