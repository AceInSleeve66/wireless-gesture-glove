#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN pins

void setup() {
  Serial.begin(115200);

  Serial.println("Starting NRF24 test...");

  if (!radio.begin()) {
    Serial.println("NRF24 not detected!");
    while (1);
  }

  Serial.println("NRF24 detected!");

  radio.setPALevel(RF24_PA_LOW); // Lower power for stability

  if (radio.isChipConnected()) {
    Serial.println("Chip responding correctly!");
  } else {
    Serial.println("Chip NOT responding properly!");
  }

  radio.printDetails();
}

void loop() {
}
