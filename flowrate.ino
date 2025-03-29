#include <SPI.h>

#define FLOW_SENSOR_PIN 2

volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned long previousMillis = 0;
const long interval = 1000;

const float calibrationFactor = 4.5;

void pulseCounter()
{
    pulseCount++;
}

void setup()
{

    Serial.begin(9600);

    pinMode(FLOW_SENSOR_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        flowRate = (pulseCount / calibrationFactor);
        pulseCount = 0;

        Serial.print("Flow rate: ");
        Serial.print(flowRate);
        Serial.println(" L/min");

        delay(100);
    }
}