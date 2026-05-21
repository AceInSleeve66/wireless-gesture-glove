#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!bno.begin()) {
    Serial.println("No BNO055 detected!");
    while (1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  Serial.println("BNO055 ready!");
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  Serial.print("X: ");
  Serial.print(event.orientation.x);

  Serial.print("  Y: ");
  Serial.print(event.orientation.y);

  Serial.print("  Z: ");
  Serial.println(event.orientation.z);

  delay(200);
}
