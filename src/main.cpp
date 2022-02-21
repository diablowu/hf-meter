#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_ST7789.h>
const char *ssid = "wlan_wlan_wlan_0";
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
void tftPrintText(int16_t x, int16_t y, uint16_t color, uint8_t fontSize, String text);
void printHFRow(int16_t y, String band, String day, String night);
void printVHFRow(int16_t y, String name, String status);
uint16_t condColor(String cond);

#define TFT_DC D1  // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
#define TFT_RST D0 // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
#define TFT_CS D8  // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)
#define MOSI D7
#define CLK D5
// initialize ST7789 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, MOSI, CLK, TFT_RST);

void setup()
{
  
  Serial.begin(9600);
  setupTFT();
  setupWIFI();
}

void loop()
{
  ESP.wdtFeed();
  updateSolarData();
  ESP.wdtFeed();
  delay(DELAY_1_MINUTE*10);
}

void setupTFT()
{
  tft.init(240, 240);
  tft.fillScreen(ST77XX_BLACK);
}

void setupWIFI()
{
  tftPrintText(10,10,ST77XX_GREEN,3,F("Connecting..."));
  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.printf("Waiting...%d\n", retries++);
    delay(200);
  }

  Serial.println("WiFi is ok, local ip:");
  tftPrintText(10,50,ST77XX_GREEN,3,F("WiFi is ok, local ip:"));
  tftPrintText(10,90,ST77XX_GREEN,3,WiFi.localIP().toString());
  // Serial.println(WiFi.localIP().toString());
}

void updateSolarData()
{


  // tft.availableForWrite
  tft.fillScreen(ST77XX_BLACK);
  // banner
  tftPrintText(0, 0, ST77XX_WHITE, 3, "Band   D   N");

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
      printHFRow(i * 30, band, day, night);
      continue;
    }

    // skip Aurora
    if (i == 5)
    {
      tft.drawFastHLine(0, 145, 240, ST77XX_WHITE);
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
      printVHFRow(150 + (i - 6) * 20, name, status);
      continue;
    }
  }

  tft.drawFastHLine(0, 228, 240, ST77XX_WHITE);
  tftPrintText(0, 230, ST77XX_BLUE, 1, updateTS);
}

void printHFRow(int16_t y, String band, String day, String night)
{
  Serial.printf("y:%d, band:%s, day:%s, night:%s\n", y, band.c_str(), day.c_str(), night.c_str());
  uint16_t dayColor = condColor(day);
  uint16_t dayNight = condColor(night);
  tftPrintText(0, y, ST77XX_WHITE, 3, band);
  tftPrintText(125, y, dayColor, 3, day);
  tftPrintText(200, y, dayNight, 3, night);
}
void printVHFRow(int16_t y, String name, String status)
{
  Serial.printf("y:%d, name:%s, status:%s\n", y, name.c_str(), status.c_str());
  uint16_t color = ST77XX_GREEN;
  if (status == "Closed")
  {
    color = ST77XX_RED;
  }

  tftPrintText(0, y, ST77XX_WHITE, 2, name);
  tftPrintText(125, y, color, 2, status);
}

void tftPrintText(int16_t x, int16_t y, uint16_t color, uint8_t fontSize, String text)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(fontSize);
  tft.println(text);
}

uint16_t condColor(String cond)
{
  if (cond == "G")
  {
    return ST77XX_GREEN;
  }
  else if (cond == "F")
  {
    return ST77XX_YELLOW;
  }
  else if (cond == "P")
  {
    return ST77XX_RED;
  }
  else
  {
    return ST77XX_WHITE;
  }
}