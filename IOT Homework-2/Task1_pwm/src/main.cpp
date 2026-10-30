#include <Arduino.h>

// ---- Pins ----
const uint8_t PIN_RED  = 18;   // LED 1 (red)
const uint8_t PIN_BLUE = 19;   // LED 2 (blue)

// ---- LEDC channels (0 and 2 are on different timers) ----
const uint8_t CH_RED  = 0;
const uint8_t CH_BLUE = 2;

// ---- PWM settings ----
const uint32_t FREQ_RED  = 1000;   // 1 kHz
const uint32_t FREQ_BLUE = 5000;   // 5 kHz
const uint8_t  RES_BITS  = 8;
const int      DUTY_MAX  = (1 << RES_BITS) - 1;   // 255
const int      STEP_MS   = 10;

void setup() {
  Serial.begin(115200);

  double realRed  = ledcSetup(CH_RED,  FREQ_RED,  RES_BITS);
  double realBlue = ledcSetup(CH_BLUE, FREQ_BLUE, RES_BITS);

  if (realRed == 0 || realBlue == 0) {
    Serial.println("ERROR: LEDC setup failed");
    while (true) { delay(1000); }
  }

  ledcAttachPin(PIN_RED,  CH_RED);
  ledcAttachPin(PIN_BLUE, CH_BLUE);
  ledcWrite(CH_RED, 0);
  ledcWrite(CH_BLUE, 0);

  Serial.println("Dual LED PWM Fade Started");
  Serial.print("LED 1 Frequency: ");
  Serial.print(realRed);
  Serial.println(" Hz");
  Serial.print("LED 2 Frequency: ");
  Serial.print(realBlue);
  Serial.println(" Hz");
}

void loop() {
  // fade up
  for (int d = 0; d <= DUTY_MAX; d++) {
    ledcWrite(CH_RED, d);
    ledcWrite(CH_BLUE, d);
    delay(STEP_MS);
  }
  // fade down
  for (int d = DUTY_MAX; d >= 0; d--) {
    ledcWrite(CH_RED, d);
    ledcWrite(CH_BLUE, d);
    delay(STEP_MS);
  }
}
