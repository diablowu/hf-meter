// #include <Arduino.h>
// #include <TFT_eSPI.h>
// #include <SPI.h>
// #include <TJpg_Decoder.h>
// #include "font/ZdyLwFont_20.h"

// TFT_eSPI tft = TFT_eSPI(); // 引脚请自行配置tft_espi库中的 User_Setup.h文件
// TFT_eSprite clk = TFT_eSprite(&tft);
// #define LCD_BL_PIN 5 //LCD背光引脚
// uint16_t bgColor = 0x0000;


// /*
//  * 引 脚 分 配： SCK  GPIO14
//  *             MOSI  GPIO13
//  *             RES   GPIO2
//  *             DC    GPIO0
//  *             LCDBL GPIO5
//  * 
// #define TFT_RGB_ORDER TFT_RGB  
// #define TFT_CS   PIN_D8  // Chip select control pin D8
// #define TFT_DC   PIN_D3  // Data Command control pin
// #define TFT_RST  PIN_D4  // Reset pin (could connect to NodeMCU RST, see next line)
// #define TFT_BL PIN_D1  // LED back-light (only for ST7789 with backlight control pin)
//  *             
// */
// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("starting...");
//   tft.begin();                    // TFT init
//   tft.fillScreen(TFT_BLACK); 
//   clk.setColorDepth(8);
//   clk.loadFont(ZdyLwFont_20);

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);       
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("80m-40m",0,0);
//   clk.setTextColor(TFT_GREEN);  
//   clk.drawString("Good",100,0);
//   clk.setTextColor(TFT_RED);  
//   clk.drawString("Bad",180,0);
//   clk.pushSprite(10,10); // 0 20*0+10 
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("30m-20m",0,0);  
//   clk.setTextColor(TFT_YELLOW);  
//   clk.drawString("Fair",100,0);
//   clk.setTextColor(TFT_RED);  
//   clk.drawString("Bad",180,0);
//   clk.pushSprite(10,30); 20*1+10
//   clk.deleteSprite();


//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("17m-15m",0,0);  
//   clk.setTextColor(TFT_YELLOW);  
//   clk.drawString("Fair",100,0);
//   clk.setTextColor(TFT_RED);  
//   clk.drawString("Bad",180,0);
//   clk.pushSprite(10,50); 20*2+10
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("12m-10m",0,0);  
//   clk.setTextColor(TFT_YELLOW);  
//   clk.drawString("Fair",100,0);
//   clk.setTextColor(TFT_RED);  
//   clk.drawString("Bad",180,0);
//   clk.pushSprite(10,70); 
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("Aurora",0,0);  
//   clk.setTextColor(TFT_RED);  
//   clk.drawString("Closed",100,0);
//   clk.pushSprite(10,90); 
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("6m EsEU",0,0);  
//   clk.setTextColor(TFT_GREEN);  
//   clk.drawString("50MHz ES",100,0);
//   clk.pushSprite(10,110); 
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("4m EsEU",0,0);  
//   clk.setTextColor(TFT_GREEN);  
//   clk.drawString("70MHz ES",100,0);
//   clk.pushSprite(10,130); 
//   clk.deleteSprite();


//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("2m EsEU",0,0);  
//   clk.setTextColor(TFT_GREEN);  
//   clk.drawString("144MHz ES",100,0);
//   clk.pushSprite(10,150); 
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_WHITE);    
//   clk.drawString("2m EsNA",0,0);  
//   clk.setTextColor(TFT_GREEN);  
//   clk.drawString("144MHz ES",100,0);
//   clk.pushSprite(10,170); 
//   clk.deleteSprite();

//   clk.createSprite(220,20);       
//   clk.fillSprite(TFT_BLACK);    
//   clk.setTextColor(TFT_GREENYELLOW);    
//   clk.drawString("2022-03-01 04:00:00",0,0);  
//   clk.pushSprite(10,200); 
//   clk.deleteSprite();

//   pinMode(LCD_BL_PIN, OUTPUT);
//   // digitalWrite(LCD_BL_PIN,LOW);
//   analogWrite(LCD_BL_PIN, 30);
// }

// void loop()
// {
//   Serial.printf("%ld running...",micros());
//   delay(2000);
// }
