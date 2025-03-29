#include <RH_ASK.h>
#include <SPI.h>

#define TRIG_PIN 4
#define ECHO_PIN 5
#define TANK_HEIGHT 10
#define SENSOR_OFFSET 2

RH_ASK driver(2000, 2, 2, 0);

void setup()
{
    Serial.begin(9600);
    if (!driver.init())
    {
        Serial.println("RF initialization failed!");
        while (1)
            ;
    }
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop()
{
    // Measure distance using SR04
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    int distance = duration * 0.034 / 2;

    int waterLevel = TANK_HEIGHT - distance + SENSOR_OFFSET;
    int waterLevelPercentage = (waterLevel * 100) / TANK_HEIGHT;
    if (waterLevelPercentage > 100)
        waterLevelPercentage = 100;
    if (waterLevelPercentage < 0)
        waterLevelPercentage = 0;

    char data[20];
    sprintf(data, "%d", waterLevelPercentage);

    driver.send((uint8_t *)data, strlen(data));
    driver.waitPacketSent();

    Serial.print("Water Level: ");
    Serial.print(waterLevelPercentage);
    Serial.println("%");

    delay(100);
}