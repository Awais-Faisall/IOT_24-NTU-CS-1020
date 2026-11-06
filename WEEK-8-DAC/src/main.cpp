#include <Arduino.h>

#define DAC_PIN 26

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32 DAC2 LED Test");
}

void loop()
{
  int values[] = {0, 64, 128, 192, 255};

  for (int i = 0; i < 5; i++)
  {
    dacWrite(DAC_PIN, values[i]);

    Serial.print("DAC2 Value: ");
    Serial.println(values[i]);

    delay(2000);
  }
}