#include <Arduino.h>

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
