// PWM出力ピン
const int RED = 13;
const int GREEN = 14;
const int BLUE = 12;

// PWMチャンネル
const int PWM_CH_RED = 0;
const int PWM_CH_GREEN = 1;
const int PWM_CH_BLUE = 2;

int red_val = 0;
int green_val = 0;
int blue_val = 0;

void setup() {
  ledcSetup(PWM_CH_RED, 1000, 8);
  ledcAttachPin(RED, PWM_CH_RED);
  ledcSetup(PWM_CH_GREEN, 1000, 8);
  ledcAttachPin(GREEN, PWM_CH_GREEN);
  ledcSetup(PWM_CH_BLUE, 1000, 8);
  ledcAttachPin(BLUE, PWM_CH_BLUE);

  ledcWrite(PWM_CH_RED, red_val);
  ledcWrite(PWM_CH_GREEN, green_val);
  ledcWrite(PWM_CH_BLUE, blue_val);
}

void loop() {
  red_val = 255;
  green_val = 0;
  blue_val = 0;
  ledcWrite(PWM_CH_RED, red_val);
  ledcWrite(PWM_CH_GREEN, green_val);
  ledcWrite(PWM_CH_BLUE, blue_val);

  delay(1000);

  red_val = 0;
  green_val = 255;
  blue_val = 0;
  ledcWrite(PWM_CH_RED, red_val);
  ledcWrite(PWM_CH_GREEN, green_val);
  ledcWrite(PWM_CH_BLUE, blue_val);

  delay(1000);

  red_val = 0;
  green_val = 0;
  blue_val = 255;
  ledcWrite(PWM_CH_RED, red_val);
  ledcWrite(PWM_CH_GREEN, green_val);
  ledcWrite(PWM_CH_BLUE, blue_val);

  delay(1000);
}