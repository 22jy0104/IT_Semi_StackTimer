#include <Adafruit_ST7735.h>       
#include <Adafruit_ST7789.h>
#include <Adafruit_ST77xx.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

//#include "Fonts/FreeSansBold24pt7b.h"  //Adafruit_GFX

#define SW_PIN_1 32   //INPUT_SWITCH 両押しのボタン
#define SW_PIN_2 25   //INPUT_SWITCH 赤タクトスイッチ左
#define SW_PIN_3 26   //INPUT_SWITCH 赤タクトスイッチ右

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

int Push_Count_Mode = 0; //モード選択。１回目ならSINGLE、２回目ならAO5、３回目ならAO12
int Push_Count_State = 0; //何回押したか。１回目ならスタート、２回目ならストップ＆表示
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
  ledcWrite(PWM_CH_GREEN, 0);
  ledcWrite(PWM_CH_BLUE, 31);

  //スイッチのピンをINPUT_PULLUPに
  pinMode(SW_PIN_1, INPUT_PULLUP);
  pinMode(SW_PIN_2, INPUT_PULLUP);
  pinMode(SW_PIN_3, INPUT_PULLUP);

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
  B_Switch = digitalRead(SW_PIN_2);//ボタンが押されたかを確認する
  if(B_Switch == LOW){
    delay(10); //チャタリングじゃないかを判断するために10ms後も押し続けたかで判断
    B_Switch = digitalRead(SW_PIN_2);//ボタンが押されたかを確認する
    if(B_Switch == LOW){
      Push_Flg = 1;
    }else{
      Push_Flg = 0;
    }
  }else{
    Push_Flg = 0;
  }

  if (Push_Flg == 1){
    Click_konkai = millis();
    Work_int = Click_konkai - Click_zenkai;
    if (Work_int > 500 ){ //1秒未満の再クリックは押しっぱなしの可能性があるので無視する

      Click_zenkai = Click_konkai;

      switch(Push_Count_Mode){
      //Home
      case 0:
        ledcWrite(PWM_CH_RED, 0);
        ledcWrite(PWM_CH_GREEN, 0);
        ledcWrite(PWM_CH_BLUE, 63);

        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(15, 20);
        tft.print("SINGLE");
        tft.setCursor(15, 50);
        tft.print("AO5");
        tft.setCursor(15, 80);
        tft.print("SEND");
        Push_Count_Mode = 1;
        break;
      //Menu1
      case 1:
        ledcWrite(PWM_CH_RED, 0);
        ledcWrite(PWM_CH_GREEN, 0);
        ledcWrite(PWM_CH_BLUE, 63);

        tft.drawRect(11, 76, 78, 22, ST77XX_BLACK); //一つ前の枠を消す
        tft.drawRect(10, 75, 80, 24, ST77XX_BLACK); //一つ前の枠を消す
        tft.drawRect(11, 16, 78, 22, ST77XX_BLUE); //選択枠の描画
        tft.drawRect(10, 15, 80, 24, ST77XX_BLUE); //選択枠の描画
        
        Push_Count_Mode = 2;
        break;
      //Menu2
      case 2:
        ledcWrite(PWM_CH_RED, 0);
        ledcWrite(PWM_CH_GREEN, 0);
        ledcWrite(PWM_CH_BLUE, 63);

        tft.drawRect(11, 16, 78, 22, ST77XX_BLACK); //一つ前の枠を消す
        tft.drawRect(10, 15, 80, 24, ST77XX_BLACK); //一つ前の枠を消す
        tft.drawRect(11, 46, 78, 22, ST77XX_BLUE); //選択枠の描画
        tft.drawRect(10, 45, 80, 24, ST77XX_BLUE); //選択枠の描画
        
        Push_Count_Mode = 3;
        break;
      //Menu2
      case 3:
        ledcWrite(PWM_CH_RED, 0);
        ledcWrite(PWM_CH_GREEN, 0);
        ledcWrite(PWM_CH_BLUE, 63);

        tft.drawRect(11, 46, 78, 22, ST77XX_BLACK); //一つ前の枠を消す
        tft.drawRect(10, 45, 80, 24, ST77XX_BLACK); //一つ前の枠を消す
        tft.drawRect(11, 76, 78, 22, ST77XX_BLUE); //選択枠の描画
        tft.drawRect(10, 75, 80, 24, ST77XX_BLUE); //選択枠の描画
        
        Push_Count_Mode = 1;
        break;
      }
    }
  }
}
