/*********************************************************************
 *
 *********************************************************************/
#include "init.h"
#include "function.h"
#include "setup.h"

/*********************************************************************
 *
 *********************************************************************/
void loop() {
    timing = millis();
    // baca data akselerasi dari sensor
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan platform
    float sudutPlatform = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
    if( INPUTSERIAL ){
        // baca data speed dan sudut roda dari serial
        // dengan format v:X (X adalah angka kecepatan), t:Y (Y adalah angka sudut)
        // jika Y negatif, maka akan dianggap roda belok ke kiri sebesar Y derajat
        // jika Y positif, maka akan dianggap roda belok ke kanan sebesar Y derajat
        if( Serial.available() ){
          String raw = "";
          raw = Serial.readString();
          c= arah(raw);
          spd= Speed(raw);
          stir= Wheel(raw);
          Serial.println(c);
          Serial.println(spd);
          Serial.println(stir);
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
        // baca data speed dan sudut roda dari potensio
        // potensio dengan 20 sampel
        accumSpeed = 0;
        accumRoda = 0;
        for( byte i=0; i<maxSampel; i++ ){
            accumSpeed += analogRead(PIN_SPEED);
            accumRoda += analogRead(PIN_RODA);
        }
        dataSpeed = accumSpeed / maxSampel;
        dataRoda  = accumRoda / maxSampel;
        // konversi data dari potensio menjadi sudut dan kecepatan
        tmpRoda  = map(dataRoda, 0, 1023, minRoda, maxRoda);
        tmpSpeed = map(dataSpeed, 0, 1023, minSpeed, maxSpeed);
    }
    
    // -------------------
    // Proses kanan kiri
    // -------------------
    tmpRoda = constrain(tmpRoda, minRoda, maxRoda);
    tmpSpeed = constrain(tmpSpeed, minSpeed, maxSpeed);
    // hitung sudut kemiringan berdasarkan rumus
    // yang sudah ditentukan
    platformRoll = FindAngle(tmpSpeed,tmpRoda);
    if( platformRoll > 0 ){
        dataServo1 = map(platformRoll,midSudutRoll,defaultMax,midServo1,minServo1);
    } else if( platformRoll < 0 ){
        dataServo1 = map(platformRoll,midSudutRoll,defaultMin,midServo1,maxServo1);
    } else {
        dataServo1 = midServo1;
    }
    // -------------------
    // Proses Speed / Rem
    // -------------------
    if( timing - speed_timing > 1000 ){
        dataServo2 = map(tmpSpeed,minSpeed,maxSpeed,midServo2,minServo2);
        speedServo2 = 255;
        float selisihSpeed = tmpSpeed-lastSpeed;
        if( selisihSpeed > 0 ){
            if( selisihSpeed > 20 ) dataServo2 = minServo2;
        } else if(selisihSpeed < 0) {
            if( selisihSpeed < -20 ) dataServo2 = maxServo2;
        } else {
            speedServo2 = 50;
        }
        lastSpeed = tmpSpeed;
        speed_timing = millis();
    }
    servo1.write(dataServo1,speedServo1);
    servo2.write(dataServo2,speedServo2);
    // Output
    Serial.print("SudutRoda: ");
    Serial.print(tmpRoda);
    Serial.print("  Speed: ");
    Serial.print(tmpSpeed);
    Serial.print("  Sudut: ");
    Serial.print(platformRoll);
    Serial.print("  SudutPlatform = ");
    Serial.print(sudutPlatform);
    Serial.print("  Servo1 = ");
    Serial.print(dataServo1);
    Serial.print("  Servo2 = ");
    Serial.print(dataServo2);
    Serial.println();
}



