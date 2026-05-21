#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN pins

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!radio.begin()) {
    Serial.println("NRF24 not detected!");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);

  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("Leonardo Receiver ready");
}

void loop() {
  if (radio.available()) {
    char text[32] = "";

    radio.read(&text, sizeof(text));

    Serial.print("Received: ");
    Serial.println(text);
  }
}
