#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- OLED ----
#define OLED_W      128
#define OLED_H      64
#define OLED_RESET  -1
#define OLED_ADDR   0x3C
Adafruit_SSD1306 oled(OLED_W, OLED_H, &Wire, OLED_RESET);

// ---- Pins ----
const uint8_t PIN_RED  = 18;
const uint8_t PIN_BLUE = 19;

// ---- LEDC channels (different timers) ----
const uint8_t CH_RED  = 0;
const uint8_t CH_BLUE = 2;

// ---- PWM settings ----
const uint32_t FREQ_RED  = 1000;
const uint32_t FREQ_BLUE = 5000;
const uint8_t  RES_BITS  = 8;
const int      DUTY_MAX  = (1 << RES_BITS) - 1;
const int      STEP_MS   = 10;

int dutyRed  = 0;
int dutyBlue = 0;

void showOLED() {
  int pctRed  = dutyRed  * 100 / DUTY_MAX;
  int pctBlue = dutyBlue * 100 / DUTY_MAX;

  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);

  oled.setCursor(0, 0);
  oled.println("PWM MONITOR");

  oled.setCursor(0, 18);
  oled.print("LED1: ");
  oled.print(pctRed);
  oled.print("% F:");
  oled.print(FREQ_RED);
  oled.println("Hz");

  oled.setCursor(0, 38);
  oled.print("LED2: ");
  oled.print(pctBlue);
  oled.print("% F:");
  oled.print(FREQ_BLUE);
  oled.println("Hz");

  oled.display();
}

void setPwm(int d) {
  dutyRed  = d;
  dutyBlue = d;
  ledcWrite(CH_RED, dutyRed);
  ledcWrite(CH_BLUE, dutyBlue);
  showOLED();
  delay(STEP_MS);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed!");
    while (true) { delay(1000); }
  }

  double realRed  = ledcSetup(CH_RED,  FREQ_RED,  RES_BITS);
  double realBlue = ledcSetup(CH_BLUE, FREQ_BLUE, RES_BITS);
  if (realRed == 0 || realBlue == 0) {
    Serial.println("ERROR: LEDC setup failed");
    while (true) { delay(1000); }
  }

  ledcAttachPin(PIN_RED,  CH_RED);
  ledcAttachPin(PIN_BLUE, CH_BLUE);

  Serial.println("Dual LED PWM + OLED Started");
  Serial.print("LED 1 Frequency: ");
  Serial.print(realRed);
  Serial.println(" Hz");
  Serial.print("LED 2 Frequency: ");
  Serial.print(realBlue);
  Serial.println(" Hz");

  setPwm(0);
}

void loop() {
  for (int d = 0; d <= DUTY_MAX; d++)  { setPwm(d); }
  for (int d = DUTY_MAX; d >= 0; d--)  { setPwm(d); }
}
