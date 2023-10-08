#include <IRremote.h>

const int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);

const int LED_PIN1 = 10;
const int LED_PIN2 = 9;
const int LED_PIN3 = 8;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode()) {
    
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
    
    
    switch (irrecv.decodedIRData.decodedRawData) {
      case 0xBA45FF00:
        digitalWrite(LED_PIN1, !digitalRead(LED_PIN1));
            Serial.println("si");
        break;
      case 0xBB44FF00:
        digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
        break;
      case 0xF807FF00:
        digitalWrite(LED_PIN3, !digitalRead(LED_PIN3));
        break;
    }
    delay(200);
    irrecv.resume(); // Receive the next value
  }
}
