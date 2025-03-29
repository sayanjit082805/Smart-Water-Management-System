#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
#define LED_FULL_PIN 6 
#define LED_EMPTY_PIN 3 
 
RH_ASK driver(2000, 2, 2, 0);   
LiquidCrystal_I2C lcd(0x3F, 16, 2); 
 
 
void setup() {
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight();
  pinMode(LED_FULL_PIN, OUTPUT);
  pinMode(LED_EMPTY_PIN, OUTPUT);
  if (!driver.init()) {
    Serial.println("RF initialization failed!");
    while (1);
  }
}
 
 
void loop() {
  uint8_t buf[20];
  uint8_t buflen = sizeof(buf);
 
  digitalWrite(LED_FULL_PIN, LOW);
  digitalWrite(LED_EMPTY_PIN, LOW);
  
  if (driver.recv(buf, &buflen)) {
    buf[buflen] = 0; 
 
 
    int waterLevelPercentage;
    sscanf((char *)buf, "%d", &waterLevelPercentage); 
 
 
    Serial.print("Water Level: ");
    Serial.print(waterLevelPercentage);
    Serial.println("%");
 
 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water : ");
    lcd.print(waterLevelPercentage);
    lcd.print(" %");
 
    if (waterLevelPercentage >= 100) {
      digitalWrite(LED_FULL_PIN, HIGH); 
    }
 
    if (waterLevelPercentage <= 10 && waterLevelPercentage > 0) {
      digitalWrite(LED_EMPTY_PIN, HIGH);
    }
 
    if (waterLevelPercentage == 0) {
      digitalWrite(LED_EMPTY_PIN, HIGH);
    }
 
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Disconnected");
  }
 
 
  delay(500);
}