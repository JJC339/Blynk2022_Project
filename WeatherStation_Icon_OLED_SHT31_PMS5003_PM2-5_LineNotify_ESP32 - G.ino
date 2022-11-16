/*
** DEV By DIYofThings
** CODE FOR EXPERMENTAL ONLY
** Contract Us : https://www.facebook.com/diyofthings
** Contract Us : https://www.youtube.com/diyofthings
** Only Shop   : https://shopee.co.th/diyofthings.shop
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Adafruit_SHT31.h"
#include <TridentTD_LineNotify.h>

#define SSID        ""
#define PASSWORD    ""
#define LINE_TOKEN  ""
#define LED_BRIGH_MAX 20
#define LED_RED 4
#define LED_GREEN 16
#define LED_BLUE 17
#define RXD2 16
#define TXD2 17

Adafruit_SHT31 sht31 = Adafruit_SHT31();

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c  //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'diy30x30', 30x30px
const unsigned char epd_bitmap_diy30x30[] PROGMEM = {
  0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xf8,
  0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0xff, 0xfc,
  0x00, 0x01, 0xff, 0xbc, 0x00, 0x03, 0xff, 0x7c, 0x00, 0x07, 0xfe, 0x7c, 0x00, 0x07, 0xfe, 0x7c,
  0x00, 0x07, 0xdd, 0xfc, 0x00, 0x0f, 0xd3, 0xf8, 0x00, 0x0f, 0xe7, 0xf8, 0xfe, 0x07, 0xdf, 0xf0,
  0x7f, 0x87, 0x3f, 0xf0, 0x3f, 0xc2, 0x7f, 0xe0, 0x3f, 0xc2, 0xff, 0xc0, 0x1f, 0xc6, 0xff, 0x80,
  0x1f, 0xc4, 0x1e, 0x00, 0x0f, 0xcc, 0x00, 0x00, 0x07, 0xa8, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
};

float temp, humi    = 0;
unsigned int pm1    = 0;
unsigned int pm2_5  = 0;
unsigned int pm10   = 0;

void setup() {

  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("starting...");

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  Wire.begin(23, 19);
  sht31.begin(0x44);


  delay(250);                        // wait for the OLED to power up
  display.begin(i2c_Address, true);  // Address 0x3C default
  display.setContrast(0);            // dim display

  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Weather");

  display.setTextSize(1);
  display.setCursor(86, 2);
  display.println("station");
  display.setCursor(86, 8);
  display.println("-------");
  display.setCursor(86, 9);
  display.println("-------");
  display.setCursor(86, 10);
  display.println("-------");

  //display.drawBitmap(2, 22,  logo16_glcd_bmp, 16, 16, 1);
  display.drawBitmap(0, 18, epd_bitmap_diy30x30, 30, 30, 1);

  display.fillRect(49, 22, 78, 20, 1);
  display.setTextColor(SH110X_BLACK);
  display.setTextSize(2);
  display.setCursor(53, 25);
  display.println("START.");

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.println("Temp&Humi: ");
  display.setCursor(0, 56);
  display.println("PM2.5    : ");
  display.setCursor(86, 48);
  display.println("C");
  display.fillRect(92, 47, 2, 2, 1);
  display.setCursor(122, 48);
  display.println("%");
  display.setCursor(98, 56);
  display.println("ug/m3");
  display.display();

  delay(5000);
}

void loop() {
  temp = sht31.readTemperature();
  humi = sht31.readHumidity();

  display.fillRect(49, 22, 78, 20, 1);
  display.setTextColor(SH110X_BLACK);
  display.setTextSize(2);
  display.setCursor(58, 25);
  display.println((float)temp, 2);

  display.fillRect(62, 48, 23, 15, 0);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(62, 48);
  display.println((float)temp, 1);
  display.fillRect(98, 48, 23, 7, 0);
  display.setCursor(98, 48);
  display.println((float)humi, 1);
  //display.println((float)random(200,400)/10.0,1);
  display.setCursor(62, 56);
  display.println((int)pm2_5, 1);
  //display.println((float)random(600,999)/10.0,1);

  if(pm2_5 > 100)
  {
    LINE.notify("PM2.5 Danger");
    LINE.notify((int)pm2_5);
  }

  display.display();
  checkPM25();
  delay(10000);
  //display.fillRect(49,22,78,20,1);
  //display.display();
  //delay(1);
}

void checkPM25() {
  int index = 0;
  char value;
  char previousValue;

  while (Serial1.available()) {
    value = Serial1.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)) {
      Serial.println("Cannot find the data header.");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("\"pm1\": ");
      Serial.print(pm1);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("\"pm2_5\": ");
      Serial.print(pm2_5);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;
      Serial.print("\"pm10\": ");
      Serial.print(pm10);
      Serial.print(" ug/m3");
    } else if (index > 15) {
      break;
    }
    index++;
  }
  while (Serial1.available()) Serial1.read();
  Serial.println(" }");
  
  //return pm2_5;
}
