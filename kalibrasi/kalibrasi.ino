/*********************************************************************
 * INCLUDE
 *********************************************************************/
#include <SoftwareSerial.h>
#include "init.h"
#include "function.h"
#include "setup.h"

/*********************************************************************
 * MAIN LOOP
 *********************************************************************/
void loop() {
	timing = millis();
    SensorAccel = mpu.readNormalizeAccel();
    sudutPlatforms = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);

    if( mySerial.available() ){
    	raw = mySerial.readStringUntil('\r\n');
    	c = arah(raw);
    	spd = Speed(raw);
    	stir = Wheel(raw);
        switch(c) {
        case 'L':
        case 'R':
            // SIMULATION MODE
            simulation(c,stir,spd,sudutPlatforms);
            break;
        case 'F':
            // FINDING MAX/MIN SENSOR
            findMinMaxSensor();
            break;
        case 'T':
            // STARTING TUNNING
            KalibrasiServo1();
            KalibrasiServo2();
            break;
        case 'P':
            // POTENSIOMETER
            while(1) {potentio();}
            break;
        }
    }
}