#include <Arduino.h>
 
constexpr uint8_t LED_PIN = 2;
constexpr uint32_t PWM_FREQUENCY = 5000;
constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint16_t PWM_MAX_DUTY = 255;
 
void setup()
{
    Serial.begin(115200);
 
    bool attached = ledcAttach
    (
        LED_PIN,
        PWM_FREQUENCY,
        PWM_RESOLUTION
    );
 
    if (!attached)
    {
        Serial.println("ERROR: PWM attachment failed.");
        while (true) { delay(1000); }
    }
 
    ledcWrite(LED_PIN, 0);
    Serial.println("ESP32 PWM LED Fade Started");
}
 
void loop()
{
    for (int duty = 0; duty <= PWM_MAX_DUTY; duty++)
    {
        ledcWrite(LED_PIN, duty);
        delay(10);
    }
 
    for (int duty = PWM_MAX_DUTY; duty >= 0; duty--)
    {
        ledcWrite(LED_PIN, duty);
        delay(10);
    }
}