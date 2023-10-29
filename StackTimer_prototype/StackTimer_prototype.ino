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

/*Display*/
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

/*Timer*/
unsigned long W_time = 0; //タイマーの値
unsigned long W_counttime = 0; //経過時間
double W_displaytime = 0.0;

int W_msec = 0; //ミリ秒の保存
int W_sec = 0; //秒の保存
int W_min = 0; //分の保存

int Push_Count = 0; //何回押したか。１回目ならスタート、２回目ならストップ＆表示
int Push_Flg = 0; //現在押しているかのフラグ

unsigned long Click_konkai = 0; //長押し誤動作の対策
unsigned long Click_zenkai = 0;

int Work_int = 0;
int B_Switch = 0; //スイッチの判断用

/*RGB_LED*/
// PWM出力ピン
const int RED = 13;
const int GREEN = 14;
const int BLUE = 12;

// PWMチャンネル
const int PWM_CH_RED = 0;
const int PWM_CH_GREEN = 1;
const int PWM_CH_BLUE = 2;


void setup(void) {
  Serial.begin(115200);

  //LEDの設定
  ledcSetup(PWM_CH_RED, 1000, 8);
  ledcAttachPin(RED, PWM_CH_RED);
  ledcSetup(PWM_CH_GREEN, 1000, 8);
  ledcAttachPin(GREEN, PWM_CH_GREEN);
  ledcSetup(PWM_CH_BLUE, 1000, 8);
  ledcAttachPin(BLUE, PWM_CH_BLUE);

  ledcWrite(PWM_CH_RED, 0);
  ledcWrite(PWM_CH_GREEN, 31);
  ledcWrite(PWM_CH_BLUE, 0);

  //スイッチのピンをINPUT_PULLUPに
  pinMode(SW_PIN, INPUT_PULLUP);

  tft.initR(INITR_GREENTAB);
  tft.setRotation(1);
  tft.setTextWrap(false);

  //tft.setFont(&FreeSansBold24pt7b);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(15, 50); tft.print("PRESS THE");
  tft.setCursor(15, 80); tft.print("BUTTON");

  tft.setTextSize(2);
}

void loop() {
  B_Switch = digitalRead(SW_PIN);//ボタンが押されたかを確認する
  if(B_Switch == LOW){
    delay(10); //チャタリングじゃないかを判断するために10ms後も押し続けたかで判断
    B_Switch = digitalRead(SW_PIN);//ボタンが押されたかを確認する
    if(B_Switch == LOW){
      Push_Flg = 1;
    }else{
      Push_Flg = 0;
    }
  }else{
    Push_Flg = 0;
  }

  if(Push_Count == 1){
    ledcWrite(PWM_CH_RED, 0);
    ledcWrite(PWM_CH_GREEN, 0);
    ledcWrite(PWM_CH_BLUE, 31);

    W_displaytime = millis() - W_time;

    //char buff[4];
    //sprintf(buff, "%2d", nowTime);  // 数字を右寄せで表示させるため
    tft.setTextWrap(false);

    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(15, 50);
    tft.print("START");

    tft.setCursor(15, 100);
    tft.print(W_displaytime / 1000);
  }

  if (Push_Flg == 1){
    Click_konkai = millis();
    Work_int = Click_konkai - Click_zenkai;
    if (Work_int > 1000 ){ //1秒未満の再クリックは押しっぱなしの可能性があるので無視する

      Click_zenkai = Click_konkai;

      switch(Push_Count){
        case 0:
          ledcWrite(PWM_CH_RED, 0);
          ledcWrite(PWM_CH_GREEN, 31);
          ledcWrite(PWM_CH_BLUE, 0);

          tft.fillScreen(ST77XX_BLACK);
          tft.setCursor(15, 50);
          tft.print("READY");
          while(digitalRead(SW_PIN) == LOW){
          }
          W_time = millis(); //開始時刻を記録
          Push_Count = 1;
          break;
        case 1:
          ledcWrite(PWM_CH_RED, 31);
          ledcWrite(PWM_CH_GREEN, 0);
          ledcWrite(PWM_CH_BLUE, 0);

          W_counttime = millis() - W_time; //経過時間を導く
          W_msec = W_counttime % 1000; //1000で割った余りをmsにする
          W_counttime = W_counttime /1000; //経過時間を1000で割って最小単位を秒にする。
          W_min = W_counttime / 60; //経過時間を60で割って分を導く。
          W_sec = W_counttime % 60; //経過時間を60で割った余りを秒にする。
          tft.fillScreen(ST77XX_BLACK);
          tft.setCursor(15, 50);
          tft.print("STOP");
          tft.setCursor(15, 100);
          if(W_min > 0){
            tft.print(W_min); tft.print("m:");  //1分未満の時は表示しない
            if(W_sec < 10){
              tft.print(0);   //10秒未満の時は先頭に0を表示
            }
          }
          tft.print(W_sec); tft.print("s:");
          if(W_msec < 100){
            tft.print(0);   //100ミリ秒未満の時は先頭に0を表示
          }
          tft.print(W_msec); tft.print("ms");
          while(digitalRead(SW_PIN) == LOW){
          }
          Push_Count = 0;
          break;
      }
    }
  }
}
