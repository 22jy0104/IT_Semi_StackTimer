#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST77xx.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

//#include "Fonts/FreeSansBold24pt7b.h"  //Adafruit_GFX

#define SW_PIN 32   //INPUT_SWITCH

#if defined(ESP32)
#define TFT_RST 4  // IO4
#define TFT_DC 2   // IO2
//#define TFT_MOSI 23      // VSPID
//#define TFT_SCLK 18      // VSPICLK
//#define TFT_BACKLIGHT 7  // IO7
#define TFT_CS 5  // VSPICS0

#else
  #define TFT_CS        10
  #define TFT_RST        9
  #define TFT_DC         8
#endif

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

double startTime;
double nowTime;

void setup(void) {
  pinMode(SW_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  tft.initR(INITR_GREENTAB);
  tft.setRotation(1);
  tft.setTextWrap(false);

  //tft.setFont(&FreeSansBold24pt7b);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.setCursor(20, 100);

  tft.print("Ready.");
  delay(1000);

  tft.setCursor(20, 100);
  tft.print("Ready..");
  delay(1000);

  tft.setCursor(20, 100);
  tft.print("Ready...");
  delay(1000);
}

void loop() {
  startTime = millis();

  while(true){
    //if(nowTime / 1000 > 15.0){
    //  startTime = millis();
    //}

    nowTime = millis() - startTime;

    //char buff[4];
    //sprintf(buff, "%2d", nowTime);  // 数字を右寄せで表示させるため
    tft.setTextWrap(false);
  
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.setCursor(20, 100);
    
    tft.print(nowTime / 1000);

    delayMicroseconds(10);
  }
}
