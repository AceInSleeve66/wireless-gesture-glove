#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5); // CE, CSN pins

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println("NRF24 not detected!");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("ESP Transmitter ready");
}

void loop() {
  const char text[] = "HELLO";

  bool ok = radio.write(&text, sizeof(text));

  if (ok) {
    Serial.println("Sent: HELLO");
  } else {
    Serial.println("Send failed");
  }

  delay(1000);
}
