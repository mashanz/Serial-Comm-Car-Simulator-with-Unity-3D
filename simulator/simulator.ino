/*********************************************************************
 *
 *********************************************************************/
#include <SoftwareSerial.h>
#include "init.h"
#include "function.h"
#include "setup.h"

/*********************************************************************
 *
 *********************************************************************/
String raw = "";
void loop() {
    //timing = millis();
    // baca data akselerasi dari sensor
    //SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan platform
    //sudutPlatform = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
    if( INPUTSERIAL ){
        //Serial.println("TEST");
        if( mySerial.available() ){
            raw = mySerial.readStringUntil('\r\n');
            c= arah(raw);
            spd= Speed(raw);
            stir= Wheel(raw);
            Serial.println(c);
            Serial.println(spd);
            Serial.println(stir);
            
            // baca data speed dan sudut roda dari serial
            // dengan format v:X (X adalah angka kecepatan), t:Y (Y adalah angka sudut)
            // jika Y negatif, maka akan dianggap roda belok ke kiri sebesar Y derajat
            // jika Y positif, maka akan dianggap roda belok ke kanan sebesar Y derajat
//            char c = Serial.read();
//            if( c == 'v' ){
//                Serial.read(); // buang char ':'
//                String d = Serial.readString();
//                float tspeed = d.toFloat();
//                if( tspeed < 0 ) tmpSpeed += tspeed;
//                else tmpSpeed = tspeed;
//            } else if( c == 't' ){
//                Serial.read(); // buang char ':'
//                String d = Serial.readString();
//                tmpRoda = d.toFloat();
//            } else if( c == 'c' ){
//                Serial.println(F("Kalibrasi Servo 1"));
//                KalibrasiServo1();
//                Serial.println(F("Kalibrasi Servo 2"));
//                KalibrasiServo2();
//            }
        }
    } else {

    }
}



