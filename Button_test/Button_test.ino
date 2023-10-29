/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 32;  // the number of the pushbutton pin


// PWM出力ピン
const int RED = 13;
const int GREEN = 14;
const int BLUE = 12;

// PWMチャンネル
const int PWM_CH_RED = 0;
const int PWM_CH_GREEN = 1;
const int PWM_CH_BLUE = 2;

int val = 0;     // 読み取った値を保持する変数

int red_val = 0;
int green_val = 0;
int blue_val = 0;


// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  ledcSetup(PWM_CH_RED, 1000, 8);
  ledcAttachPin(RED, PWM_CH_RED);
  ledcSetup(PWM_CH_GREEN, 1000, 8);
  ledcAttachPin(GREEN, PWM_CH_GREEN);
  ledcSetup(PWM_CH_BLUE, 1000, 8);
  ledcAttachPin(BLUE, PWM_CH_BLUE);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // read the state of the pushbutton value:
  val = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (val == LOW) {
    // turn LED on:
    red_val = 255;
    green_val = 0;
    blue_val = 0;
    ledcWrite(PWM_CH_RED, red_val);
    ledcWrite(PWM_CH_GREEN, green_val);
    ledcWrite(PWM_CH_BLUE, blue_val);
  } else {
    // turn LED off:
    ledcWrite(PWM_CH_RED, LOW);
    ledcWrite(PWM_CH_GREEN, LOW);
    ledcWrite(PWM_CH_BLUE, LOW);
  }
}
