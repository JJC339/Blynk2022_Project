#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Adafruit_SHT31.h" 

#include <TridentTD_LineNotify.h>           //<<<<<<<<<<<< 
#define SSID        "--------------------"  //<<<<<<<<<<<< ชื่อ wifi ถ้าแชร์จากมือถือต้องเป็นเครื่อง android
#define PASSWORD    "--------------------"  //<<<<<<<<<<<< password wifi
#define LINE_TOKEN  ""  //<<<<<<<<<<<< line token

#define LED_BRIGH_MAX 20
#define LED_RED 4
#define LED_GREEN 16
#define LED_BLUE 17

Adafruit_SHT31 sht31 = Adafruit_SHT31();

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c  //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#define NUMFLAKES 10
//#define XPOS 0
//#define YPOS 1
//#define DELTAY 2
/*
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};
*/
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

float temp, humi = 0;

void setup() {

  Serial.begin(115200);
  Serial.println("starting...");

  /* เชื่อมต่อ Wifi และ ส่งข้อความเริ่มทำงาน*/
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Hello Weather Station");

  //pinMode(LED_RED   ,OUTPUT);
  //pinMode(LED_GREEN ,OUTPUT);
  //pinMode(LED_BLUE  ,OUTPUT);

  //digitalWrite(LED_RED    ,LOW);
  //digitalWrite(LED_GREEN  ,LOW);
  //digitalWrite(LED_BLUE   ,LOW);

  //analogWrite(LED_RED   ,LED_BRIGH_MAX);
  //analogWrite(LED_GREEN ,LED_BRIGH_MAX);
  //analogWrite(LED_BLUE  ,LED_BRIGH_MAX);

  ledcSetup(0, 120, 8);
  ledcAttachPin(LED_BLUE, 0);
  ledcWrite(0, 20);

  Wire.begin(23, 19);
  sht31.begin(0x44);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

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
  display.println("Temperature: ");
  display.setCursor(0, 56);
  display.println("Humidity   : ");
  display.setCursor(98, 48);
  display.println("C <<<");
  display.fillRect(104, 47, 2, 2, 1);
  display.setCursor(98, 56);
  display.println("% >>>");
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

  display.fillRect(72, 48, 23, 15, 0);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(72, 48);
  display.println((float)temp, 1);
  //display.println((float)random(200,400)/10.0,1);
  display.setCursor(72, 56);
  display.println((float)humi, 1);
  //display.println((float)random(600,999)/10.0,1);

  /* ตรวจสอบค่าอุณหภูมิ เกิน 35 ให้ส่ง Line แจ้งเตือน*/
  if((int)temp < 25){
    LINE.notify("อุณหภูมิต่ำกว่า 25 องศา");
  }
  if((int)temp > 35){
    LINE.notify("อุณหภูมิสูงกว่า 35 องศา");
  }

  // ความชื่น
  if((int)humi < 30){
    LINE.notify("ความชื้นต่ำกว่า 30%");
  }
  if((int)humi > 80){
    LINE.notify("ความชื้นเกิน 80%");
  }

  display.display();
  delay(10000);
  //display.fillRect(49,22,78,20,1);
  //display.display();
  //delay(1);
}