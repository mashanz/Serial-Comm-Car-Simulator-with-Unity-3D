#include <Arduino.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
    pinMode(PIN_RODA,INPUT);
    pinMode(PIN_SPEED,INPUT);
    Serial.begin(19200);
    while( !Serial );
    mySerial.begin(19200);
    servo1.attach(8);
    servo2.attach(9);
    delay(1000);
}
