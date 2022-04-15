#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_I2CDevice.h>
#include <TFT_eSPI.h>
#include "font/ZdyLwFont_20.h"

const char *ssid = "<ssid>";
const char *password = "<passcode>";
const char *solarDataHost = "js.tkpdevops.com";
const char *solarDataPath = "/solar-data.txt";

static const uint32_t DELAY_1_S = 1000UL;
static const uint32_t DELAY_1_MINUTE = DELAY_1_S * 60UL;
static const uint32_t DELAY_1_HOUR = DELAY_1_MINUTE * 60UL;

static const uint32_t MAX_INTERVAL = DELAY_1_MINUTE * 10;

void setupWIFI();
void setupTFT();
void updateSolarData();
void printHFRow(int16_t y, String band, String day, String night);
void printVHFRow(int16_t y, String name, String status);
uint16_t condColor(String cond);


/*
// Display SDO/MISO  to NodeMCU pin D6 (or leave disconnected if not reading TFT)
// Display LED       to NodeMCU pin VIN (or 5V, see below)
// Display SCK       to NodeMCU pin D5
// Display SDI/MOSI  to NodeMCU pin D7
// Display DC (RS/AO)to NodeMCU pin D3
// Display RESET     to NodeMCU pin D4 (or RST, see below)
// Display CS        to NodeMCU pin D8 (or GND, see below)
// Display GND       to NodeMCU pin GND (0V)
// Display VCC       to NodeMCU 5V or 3.3V


#define TFT_CS   PIN_D8  // Chip select control pin D8
#define TFT_DC   PIN_D3  // Data Command control pin
#define TFT_RST  PIN_D4 
*/

// #define TFT_DC 0  // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
// #define TFT_RST 2 // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
// #define TFT_CS -1  // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)
// #define MOSI 13
// #define CLK 14
// initialize ST7789 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)


TFT_eSPI tft = TFT_eSPI(); // 引脚请自行配置tft_espi库中的 User_Setup.h文件
TFT_eSprite clk = TFT_eSprite(&tft);
#define LCD_BL_PIN 5 //LCD背光引脚
uint16_t bgColor = 0x0000;

void setup()
{
  Serial.println("staring...");
  Serial.begin(9600);
  setupTFT();
  setupWIFI();
}

void loop()
{
  ESP.wdtFeed();
  updateSolarData();
  ESP.wdtFeed();
  delay(DELAY_1_MINUTE*1);
}

void setupTFT()
{
  tft.begin();                    // TFT init
  tft.fillScreen(TFT_BLACK); 
  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);

  pinMode(LCD_BL_PIN, OUTPUT);
  analogWrite(LCD_BL_PIN, 30);
}

void setupWIFI()
{
  clk.createSprite(220,20);       
  clk.fillSprite(TFT_BLACK);       
  clk.setTextColor(TFT_GREEN);    
  clk.drawString(F("Connecting..."),0,0);
  clk.pushSprite(10,10);
  clk.deleteSprite();
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.printf("Waiting...%d\n", retries++);
    delay(200);
  }

  Serial.println("WiFi is ok, local ip:");
  clk.createSprite(220,60);       
  clk.fillSprite(TFT_BLACK);       
  clk.setTextColor(TFT_GREEN);    
  clk.drawString(F("WiFi is ok, local ip:"),0,0);
  clk.drawString(WiFi.localIP().toString(),0,20);
  clk.pushSprite(10,50);
  clk.deleteSprite();
  // Serial.println(WiFi.localIP().toString());
  // delay(2000);
  // tft.fillScreen(TFT_BLACK);
}

void updateSolarData()
{


  // tft.availableForWrite
  tft.fillScreen(TFT_BLACK);
  // banner
  // tftPrintText(0, 0, ST77XX_WHITE, 3, "Band   D   N");

  WiFiClient wifiClient;
  HTTPClient http;

  http.begin(wifiClient, solarDataHost, 80, solarDataPath);
  int httpCode = http.GET();
  ESP.wdtFeed();
  Serial.printf("solardata response code %d\n", httpCode);
  String payload = http.getString();

  String updateTS = "";
  String rest = payload;
  String current = "";
  int crlfIdx = 0;
  for (int i = 0; i < 10; i++)
  {
    ESP.wdtFeed();
    crlfIdx = rest.indexOf("\n", 0);
    current = rest.substring(0, crlfIdx);
    current.trim();
    rest = rest.substring(crlfIdx);
    rest.trim();

    if (i == 0)
    {
      updateTS = current;
      continue;
    }

    // hf
    if (i > 0 && i <= 4)
    {
      // 80-40,F,G
      int idx = 0;
      String band;
      String day;
      String night;
      String rest = current;
      idx = rest.indexOf(",", 0);
      band = rest.substring(0, idx);
      rest = rest.substring(idx + 1);

      idx = rest.indexOf(",", 0);
      day = rest.substring(0, idx);
      rest = rest.substring(idx + 1);

      idx = rest.indexOf(",", 0);
      night = rest.substring(0, idx);
      printHFRow(i, band, day, night);
      continue;
    }

    // skip Aurora
    if (i == 5)
    {
      // tft.drawFastHLine(0, 145, 240, TFT_WHITE);
      continue;
    }

    // vhf
    if (i > 5 && i <= 9)
    {
      int idx = 0;
      String name;
      String status;
      idx = current.indexOf(",", 0);
      name = current.substring(0, idx);
      status = current.substring(idx + 1);
      printVHFRow(i, name, status);
      continue;
    }
  }

  // tft.drawFastHLine(0, 228, 240, TFT_WHITE)
  clk.createSprite(220,20);       
  clk.fillSprite(TFT_BLACK);       
  clk.setTextColor(TFT_BLUE);    
  clk.drawString(updateTS,0,0);
  clk.pushSprite(10,220); 
  clk.deleteSprite();
}

void printHFRow(int16_t y, String band, String day, String night)
{
  Serial.printf("y:%d, band:%s, day:%s, night:%s\n", y, band.c_str(), day.c_str(), night.c_str());
  uint16_t dayColor = condColor(day);
  uint16_t nightColor = condColor(night);

  clk.createSprite(220,20);       
  clk.fillSprite(TFT_BLACK);       
  clk.setTextColor(TFT_WHITE);    
  clk.drawString(band,0,0);
  clk.setTextColor(dayColor);  
  clk.drawString(day,100,0);
  clk.setTextColor(nightColor);  
  clk.drawString(night,180,0);
  clk.pushSprite(10,20*y+10); 
  clk.deleteSprite();
  // tftPrintText(0, y, TFT_WHITE, 3, band);
  // tftPrintText(125, y, dayColor, 3, day);
  // tftPrintText(200, y, dayNight, 3, night);
}
void printVHFRow(int16_t y, String name, String status)
{
  Serial.printf("y:%d, name:%s, status:%s\n", y, name.c_str(), status.c_str());
  uint16_t color = TFT_GREEN;
  if (status == "Closed")
  {
    color = TFT_RED;
  }
  clk.createSprite(220,20);       
  clk.fillSprite(TFT_BLACK);    
  clk.setTextColor(TFT_WHITE);    
  clk.drawString(name,0,0);  
  clk.setTextColor(color);  
  clk.drawString(status,100,0);
  clk.pushSprite(10,20*y+10); 
  clk.deleteSprite();
  // tftPrintText(0, y, TFT_WHITE, 2, name);
  // tftPrintText(125, y, color, 2, status);
}


uint16_t condColor(String cond)
{
  if (cond == "G")
  {
    return TFT_GREEN;
  }
  else if (cond == "F")
  {
    return TFT_YELLOW;
  }
  else if (cond == "P")
  {
    return TFT_RED;
  }
  else
  {
    return TFT_WHITE;
  }
}
