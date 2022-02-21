#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_ST7789.h>
#include "config.h"

const char *ssid = "inblackhole";
const char *password = "wubo13264332849";
const char *solarDataHost = "js.tkpdevops.com";
const char *solarDataPath = "/solar-data.txt";

static const uint32_t DELAY_1_S = 1000UL;
static const uint32_t DELAY_1_MINUTE = DELAY_1_S * 60UL;
static const uint32_t DELAY_1_HOUR = DELAY_1_MINUTE * 60UL;

static const uint32_t MAX_INTERVAL = DELAY_1_MINUTE * 10;

void setupWIFI();
void setupTFT();
void updateSolarData();
void printTFTText(int16_t x, int16_t y, uint16_t color, uint16_t fontSize, String text);
void printHFRow(int16_t y, String band, String day, String night);
void printVHFRow(int16_t y, String name, String status);

WiFiClient wifiClient;
HTTPClient http;

#define CS D1
#define DC D2
#define MOSI D3
#define SCLK D4
#define RST D5

Adafruit_ST7789 tft = Adafruit_ST7789(CS, DC, MOSI, SCLK, RST);

void setup()
{
  Serial.begin(9600);
  setupWIFI();

  
}

void loop()
{

  updateSolarData();
  delay(MAX_INTERVAL);
}

void setupTFT()
{
  tft.init(240, 240);
  tft.fillScreen(ST77XX_BLACK);
}

void setupWIFI()
{
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.printf("Waiting...%d\n", retries++);
    delay(1000);
  }

  Serial.println("WiFi is ok, local ip:");
  Serial.println(WiFi.localIP().toString());
}

void updateSolarData()
{
  http.begin(wifiClient, solarDataHost, 80, solarDataPath);
  int httpCode = http.GET();
  Serial.printf("solardata response code %d\n", httpCode);
  String payload = http.getString();

  String updateTS = "";
  String rest = payload;
  String current = "";
  int crlfIdx = 0;
  for (int i = 0; i < 10; i++)
  {
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
      printHFRow(i * 30, band, day, night);
      continue;
    }

    // skip Aurora
    if (i == 5)
    {
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
      printVHFRow(i * 20, name, status);
    }
  }
}

void printHFRow(int16_t y, String band, String day, String night)
{
  Serial.printf("y:%d, band:%s, day:%s, night:%s\n", y, band.c_str(), day.c_str(), night.c_str());
}
void printVHFRow(int16_t y, String name, String status)
{
  Serial.printf("y:%d, name:%s, status:%s\n", y, name.c_str(), status.c_str());
}