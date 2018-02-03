/*********************************************************************
 * INCLUDE DATA
 *********************************************************************/
#include <SoftwareSerial.h>
#include "init.h"
#include "function.h"
#include "setup.h"

/*********************************************************************
 * MAIN LOOP
 *********************************************************************/
String raw = "";
void loop() {
   // timing = millis();
   // baca data akselerasi dari sensor
   // SensorAccel = mpu.readNormalizeAccel();
   // hitung sudut kemiringan platform
   // sudutPlatform = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
    

    // MODE PLATFORM
    if( INPUTSERIAL ){ // MODE: READY
        if( mySerial.available() ){
            raw = mySerial.readStringUntil('\r\n');
            c = arah(raw);
            spd = Speed(raw);
            stir = Wheel(raw);
        }
    
    Serial.print(c);
    Serial.print(spd);
    Serial.println(stir);

    tmpRoda = constrain(stir, minRoda, maxRoda);
    tmpSpeed = constrain(spd, minSpeed, maxSpeed);
    
    platformRoll = FindAngle(tmpSpeed,tmpRoda);
    Serial.print("  PlatformRoll = "); Serial.print(platformRoll);
    //platformRoll  = 0;
    if (c=='L') platformRoll = -80;
    else if (c=='R') platformRoll = 80;
    Serial.print("  PlatformRoll = "); Serial.println(platformRoll);

    if ( platformRoll > 0 ) dataServo1 = map(platformRoll,midSudutRoll,defaultMax,midServo1,minServo1);
    else if ( platformRoll < 0 ) dataServo1 = map(platformRoll,midSudutRoll,defaultMin,midServo1,maxServo1);
    else dataServo1 = midServo1;

    if ( timing - speed_timing > 1000 ){
        dataServo2 = map(tmpSpeed,minSpeed,maxSpeed,midServo2,minServo2);
        speedServo2 = 255;

        float selisihSpeed = tmpSpeed-lastSpeed;

        if( selisihSpeed > 0 ) if( selisihSpeed > 20 ) dataServo2 = minServo2;
        else if(selisihSpeed < 0) if( selisihSpeed < -20 ) dataServo2 = maxServo2;
        else speedServo2 = 50;

        lastSpeed = tmpSpeed;
        speed_timing = millis();
    }

    servo1.write(dataServo1,speedServo1);
    servo2.write(dataServo2,speedServo2);
    
    Serial.print("arah: "); Serial.print(c);
    Serial.print("  SudutRoda: "); Serial.print(tmpRoda);
    Serial.print("  Speed: "); Serial.print(tmpSpeed);
    Serial.print("  Sudut: "); Serial.print(platformRoll);
    Serial.print("  SudutPlatform = "); Serial.print(sudutPlatform);
    Serial.print("  Servo1 = "); Serial.print(dataServo1);
    Serial.print("  Servo2 = "); Serial.println(dataServo2);


    } else { // MODE: CALIBRATION

    }
}



