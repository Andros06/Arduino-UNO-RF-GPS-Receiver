#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define LED 13

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);
  Serial.println("Arduino LoRa RX Test!");

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH);
      
      // Convert received message to string
      String receivedMessage = (char*)buf;
      
      // Check if the received message is a GPS data message
      if (receivedMessage.startsWith("GPS:")) {
        // Extract GPS data from the message
        String gpsData = receivedMessage.substring(4); // Remove "GPS:" prefix
        Serial.print("Received GPS data: ");
        Serial.println(gpsData);
        
        // Process the GPS data further as needed
      } else {
        // Print other types of received messages
        Serial.print("Received: ");
        Serial.println(receivedMessage);
      }
      
      digitalWrite(LED, LOW);
    } else {
      Serial.println("Receive failed");
    }
  }
}
