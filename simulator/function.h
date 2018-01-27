#include <Arduino.h>

/*********************************************************************
 * konversi dari radian ke derajat
 * sebab pada arduino, output fungsi atan() adalah radian
 *********************************************************************/
float toDegree(float radian){
    return (radian * 180 / PI);
}

/*********************************************************************
 * konversi dari derajat ke radian
 * sebab pada arduino fungsi tan() inputnya adalah radian (bukan derajat)
 *********************************************************************/
float toRadian(float derajat){
    return (derajat * PI / 180);
}

/*********************************************************************
 * menghitung jari-jari dengan Ackerman Steering Geometry
 * R = jarak_ban / tan(sudut_roda), sudut roda dalam derajat
 *********************************************************************/
float FindR(float sudut_roda){
    return (JARAK_BAN / tan( toRadian(sudut_roda) ));
}

/*********************************************************************
 * menghitung kemiringan berdasarkan kecepatan & sudut roda
 * kecepatan dalam km/jam, sudut roda dalam derajat
 *********************************************************************/
float FindAngle(float kecepatan, float sudut_roda){
    // konversi kecepatan menjadi m/s
    kecepatan = kecepatan * 1000/3600;
    // hitung jari2
    float R = FindR(sudut_roda);
    // hitung sudut (dalam radian)
    float radian = atan( pow(kecepatan,2) / (GRAVITASI * R) );
    // konversi ke derajat
    return toDegree(radian);
}

/*********************************************************************
 * sudut kemiringan kiri atau kanan
 *********************************************************************/
float getAngleRoll( float x, float z ){
    return (atan2(x,z)*180.0)/M_PI;
}

/*********************************************************************
 * 
 *********************************************************************/
float getAnglePitch(float x, float y, float z){
    return -(atan2(x, sqrt(y*y + z*z))*180.0)/M_PI;
}

/*********************************************************************
 *
 *********************************************************************/
void KalibrasiServo1(){
    Serial.println();
    Serial.println("Sudut Min: ");
    for( int i=30; i>=0; i-=2 ){
        // turunkan tuas servo untuk mendapatkan sudut minimal
        servo1.write(i);
        delay(1000);
        // baca akeselerasi dr sersor yg sudah dinormalisasi
        SensorAccel = mpu.readNormalizeAccel();
        // hitung sudut kemiringan kekiri/kanan
        float sudut = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
        if( abs(sudut) > abs(minSudutRoll) ){
            minSudutRoll = sudut;
            minServo1 = i;
        }
        Serial.print(i); Serial.print(" => "); Serial.println(sudut);
    }
    // naikkan tuas servo untuk mendapatkan sudut max
    Serial.println();
    Serial.println("Sudut Max: ");
    for( int i=150; i<=180; i+=2 ){
        // turunkan tuas servo untuk mendapatkan sudut minimal
        servo1.write(i);
        delay(1000);
        // baca akeselerasi dr sersor yg sudah dinormalisasi
        SensorAccel = mpu.readNormalizeAccel();
        // hitung sudut kemiringan kekiri/kanan
        float sudut = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
        if( abs(sudut) > abs(maxSudutRoll) ){
            maxSudutRoll = sudut;
            maxServo1 = i;
        }
        Serial.print(i); Serial.print(" => "); Serial.println(sudut);
    }
    //untuk keperluan mengoreksi kesalahan jika perlu
    stepCorrection1 = abs((maxServo1-minServo1)/(maxSudutRoll-minSudutRoll));
    // ke kondisi normal, mencari titik sejajar platform, sudut = 0;
    Serial.println();
    Serial.println("Sudut Mid: ");
    boolean done = false;
    float selisih = 0;
    // proses pencarian sudut sejajar
    while( !done ){
        servo1.write(midServo1);
        delay(1000);
        // baca akeselerasi dr sersor yg sudah dinormalisasi
        SensorAccel = mpu.readNormalizeAccel();
        // hitung sudut kemiringan kekiri/kanan
        midSudutRoll  = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
        // selisih
        selisih = 0 + midSudutRoll;
        if( abs(selisih) < MIN_ERROR ){
            done = true;
        } else {
            midServo1 += ((selisih * stepCorrection1)/2) * -1;
        }
        Serial.print(midServo1); Serial.print(" => "); Serial.println(midSudutRoll);
    }
    Serial.println();
    Serial.print(F("Sudut Minimum : "));
    Serial.println(minServo1);
    Serial.print(F("Sudut Midle : "));
    Serial.println(midServo1);
    Serial.print(F("Sudut Maximum : "));
    Serial.println(maxServo1);
}

/*********************************************************************
 *
 *********************************************************************/
void KalibrasiServo2(){
    Serial.println();
    Serial.println("Sudut Min: ");
    float minServo2_tmp = minServo2;
    for( int i=30; i>=0; i-=2 ){
        // turunkan tuas servo untuk mendapatkan sudut minimal
        servo2.write(i);
        delay(1000);
        // baca akeselerasi dr sersor yg sudah dinormalisasi
        SensorAccel = mpu.readNormalizeAccel();
        // hitung sudut kemiringan kekiri/kanan
        float sudut = getAnglePitch(SensorAccel.XAxis,SensorAccel.YAxis,SensorAccel.ZAxis);
        if( abs(sudut) > abs(minSudutPitch) ){
            minSudutPitch = sudut;
            minServo2 = i;
        }
        Serial.print(i); Serial.print(" => "); Serial.println(sudut);
    }
    // naikkan tuas servo untuk mendapatkan sudut maksimal
    Serial.println();
    Serial.println("Sudut Max: ");
    float maxServo2_tmp = maxServo2;
    for( int i=150; i<=180; i+=2 ){
        // turunkan tuas servo untuk mendapatkan sudut minimal
        servo2.write(i);
        delay(1000);
        // baca akeselerasi dr sersor yg sudah dinormalisasi
        SensorAccel = mpu.readNormalizeAccel();
        // hitung sudut kemiringan kekiri/kanan
        float sudut = getAnglePitch(SensorAccel.XAxis,SensorAccel.YAxis,SensorAccel.ZAxis);
        if( abs(sudut) > abs(maxSudutPitch) ){
            maxSudutPitch = sudut;
            maxServo2 = i;
        }
        Serial.print(i); Serial.print(" => "); Serial.println(sudut);
    }
    //untuk keperluan mengoreksi kesalahan jika perlu
    stepCorrection2 = abs((maxServo2-minServo2)/(maxSudutPitch-minSudutPitch));
    // ke kondisi normal, mencari titik sejajar platform, sudut = 0;
    Serial.println();
    Serial.println("Sudut Mid: ");
    boolean done = false;
    float selisih = 0;
    // proses pencarian sudut sejajar
    while( !done ){
        servo2.write(midServo2);
        delay(1000);
        // baca akeselerasi dr sersor yg sudah dinormalisasi
        SensorAccel = mpu.readNormalizeAccel();
        // hitung sudut kemiringan kekiri/kanan
        midSudutPitch = getAnglePitch(SensorAccel.XAxis,SensorAccel.YAxis,SensorAccel.ZAxis);
        // selisih
        selisih = 0 + midSudutPitch;
        if( abs(selisih) < MIN_ERROR ){
            done = true;
        } else {
            midServo2 += ((selisih * stepCorrection2)/2) * -1;
        }
        Serial.print(midServo2); Serial.print(" => "); Serial.println(midSudutPitch);
    }
    Serial.println();
    Serial.print(F("Sudut Minimum : "));
    Serial.println(minServo2);
    Serial.print(F("Sudut Midle : "));
    Serial.println(midServo2);
    Serial.print(F("Sudut Maximum : "));
    Serial.println(maxServo2);

}

/*********************************************************************
 * PARSING DIRECTION
 *********************************************************************/
char direction(String raw) {
    return raw.charAt(0);
}

/*********************************************************************
 * PARSING SPEED
 *********************************************************************/
int Speed(String raw) {
    String string = String(raw.charAt(1) + raw.charAt(2) + raw.charAt(3));
    return string.toInt();
}

/*********************************************************************
 * PARSING WHEEL
 *********************************************************************/
int Wheel(String raw) {
    String string = String(raw.charAt(4) + raw.charAt(5) + raw.charAt(6));
    return string.toInt();
}