#include <Wire.h>
#include <MPU6050.h>
#include <VarSpeedServo.h>

MPU6050 mpu;

Vector SensorAccel;

// jika INPUTSERIAL = true data diinputkan dari serial
// jika INPUTSERIAL = false data diinputkan dari potensiometer
boolean INPUTSERIAL = true;

// jika menggunakan potensio utk sudut roda & kecepatan
const byte PIN_RODA = A0;
const byte PIN_SPEED = A1;

const int GRAVITASI = 10;
const float JARAK_BAN = 2.45;

// speed km/h
float minSpeed = 0;
float maxSpeed = 100;

// sudut roda 40 derajat ke kiri, 40 derajat ke kanan
float minRoda = -89;  // kiri
float maxRoda =  89;  // kanan

VarSpeedServo servo1; // kiri/kanan
VarSpeedServo servo2; // depan/belakang

float minServo1 = 0;  
float maxServo1 = 180; 
float midServo1 = 90;

float minServo2 = 0;   
float maxServo2 = 180; 
float midServo2 = 90;

float speedServo1 = 255;
float speedServo2 = 255;

float defaultMax =  90;
float defaultMin = -90;

float dataServo1 = midServo1;
float dataServo2 = midServo2;

float stepCorrection1 = 0;
float stepCorrection2 = 0;

float MIN_ERROR = 0.3;

// sudut min dan max dicari oleh alat
// saat pertama kali aktif, kemiringan kirin/kanan
float minSudutRoll = -10;
float maxSudutRoll = 10;
float midSudutRoll = 0;

// sudut min dan max dicari oleh alat
// saat pertama kali aktif, kemiringan depan/belakang
float minSudutPitch = -12;
float maxSudutPitch = 12;
float midSudutPitch = 0;

// sudut kemiringan platform dari kanan/kiri
float platformRoll = 0; 
float platformRoll_last = 0;
float platformRoll_dummy = 0;

// selisih sudut kemiringan platform dan hasil perhitungan
float selisihPlatform = 0;

int dataSpeed = 0; // dari potensio atau serial
int dataRoda = 0;  // dari potensi atau serial
int tmpSpeed = 0; 
int tmpRoda = 0;

unsigned int accumSpeed = 0;
unsigned int accumRoda = 0;
byte maxSampel = 20;

unsigned long timing = 0;
unsigned long lastchange_servo1 = 0;

float lastSpeed = 0;
unsigned long speed_timing = 0;

void setup() {

    pinMode(PIN_RODA,INPUT);
    pinMode(PIN_SPEED,INPUT);
    
    Serial.begin(19200);
    while( !Serial );
    servo1.attach(8);
    servo2.attach(9);
    delay(1000);

    // koneksi ke mpu
    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
        Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
    }

    // show info
    Serial.println();
    Serial.println();
    Serial.println(F("============================================"));
    Serial.println(F("Perbandingan Sudut Servo => Sudut Platform"));
    Serial.println(F("============================================"));

    // mencari hubungan perbandigan antara
    // sudut aktual dan sudut servo samping

    

    // ------------------------------------
    // ------------------------------------
    // kalibrasi servo 1
    // ------------------------------------
    // ------------------------------------

    
    KalibrasiServo1();
        
    // sudut minimum servo

    servo1.write(minServo1);
    delay(2000);
    
    // baca akeselerasi dr sersor yg sudah dinormalisasi
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan kekiri/kanan
    minSudutRoll = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);

    Serial.print(F("Sudut Min : "));
    Serial.print(minServo1);
    Serial.print(F(" => "));
    Serial.println(minSudutRoll);

    // sudut 0 derajat servo

    servo1.write(midServo1);
    delay(2000);
    
    // baca akeselerasi dr sersor yg sudah dinormalisasi
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan kekiri/kanan
    midSudutRoll = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
    
    Serial.print(F("Sudut Mid : "));
    Serial.print(midServo1);
    Serial.print(F(" => "));
    Serial.println(midSudutRoll);


    // sudut maksimum servo
    
    servo1.write(maxServo1);
    delay(2000);
    
    // baca akeselerasi dr sersor yg sudah dinormalisasi
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan kekiri/kanan
    maxSudutRoll = getAngleRoll(SensorAccel.YAxis,SensorAccel.ZAxis);
    
    Serial.print(F("Sudut Max : "));
    Serial.print(maxServo1);
    Serial.print(F(" => "));
    Serial.println(maxSudutRoll);

    servo1.write(midServo1);

    
    //Serial.print(F("Step Correction1 : "));
    //Serial.println(stepCorrection1);
    

    // ------------------------------------
    // ------------------------------------
    // kalibrasi servo 2
    // ------------------------------------
    // ------------------------------------

    KalibrasiServo2();

    // turunkan tuas servo untuk mendapatkan sudut minimal
    servo2.write(minServo2);
    delay(2000);

    // baca akeselerasi dr sersor yg sudah dinormalisasi
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan kekiri/kanan
    minSudutPitch = getAnglePitch(SensorAccel.XAxis,SensorAccel.YAxis,SensorAccel.ZAxis);

    Serial.print(F("Sudut Min : "));
    Serial.print(minServo2);
    Serial.print(F(" => "));
    Serial.println(minSudutPitch);


    // sesuaikan untuk sudut 0
    servo2.write(midServo2);
    delay(2000);

    // baca akeselerasi dr sersor yg sudah dinormalisasi
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan kekiri/kanan
    midSudutPitch = getAnglePitch(SensorAccel.XAxis,SensorAccel.YAxis,SensorAccel.ZAxis);

    Serial.print(F("Sudut Mid : "));
    Serial.print(midServo2);
    Serial.print(F(" => "));
    Serial.println(midSudutPitch);

    // sesuaikan untuk sudut maksimal
    servo2.write(maxServo2);
    delay(2000);

    // baca akeselerasi dr sersor yg sudah dinormalisasi
    SensorAccel = mpu.readNormalizeAccel();
    // hitung sudut kemiringan kekiri/kanan
    maxSudutPitch = getAnglePitch(SensorAccel.XAxis,SensorAccel.YAxis,SensorAccel.ZAxis);
    
    Serial.print(F("Sudut Max : "));
    Serial.print(maxServo2);
    Serial.print(F(" => "));
    Serial.println(maxSudutPitch);
   
    //Serial.print(F("Step Correction2 : "));
    //Serial.println(stepCorrection2);

    servo2.write(midServo2);
   
    delay(1000);
    
}

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
            char c = Serial.read();
            if( c == 'v' ){
                Serial.read(); // buang char ':'
                String d = Serial.readString();

                float tspeed = d.toFloat();
                if( tspeed < 0 ) tmpSpeed += tspeed;
                else tmpSpeed = tspeed;
                
            } else if( c == 't' ){
                Serial.read(); // buang char ':'
                String d = Serial.readString();
                tmpRoda = d.toFloat();
                
            } else if( c == 'c' ){

                Serial.println(F("Kalibrasi Servo 1"));
                KalibrasiServo1();
                
                Serial.println(F("Kalibrasi Servo 2"));
                KalibrasiServo2();
                
            }
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

// menghitung kemiringan berdasarkan kecepatan & sudut roda
// kecepatan dalam km/jam, sudut roda dalam derajat
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

// menghitung jari-jari dengan Ackerman Steering Geometry
// R = jarak_ban / tan(sudut_roda), sudut roda dalam derajat
float FindR(float sudut_roda){
    return (JARAK_BAN / tan( toRadian(sudut_roda) ));
}

// konversi dari derajat ke radian
// sebab pada arduino fungsi tan() inputnya adalah radian (bukan derajat)
float toRadian(float derajat){
    return (derajat * PI / 180);
}

// konversi dari radian ke derajat
// sebab pada arduino, output fungsi atan() adalah radian
float toDegree(float radian){
    return (radian * 180 / PI);
}

// sudut kemiringan kiri atau kanan
float getAngleRoll( float x, float z ){
    return (atan2(x,z)*180.0)/M_PI;
}

float getAnglePitch(float x, float y, float z){
    return -(atan2(x, sqrt(y*y + z*z))*180.0)/M_PI;
}

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

