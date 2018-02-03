/*********************************************************************
 *
 *********************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <VarSpeedServo.h>

MPU6050 mpu;
Vector SensorAccel;
VarSpeedServo servo1; // kiri/kanan
VarSpeedServo servo2; // depan/belakang
char c;
int spd;
int stir;
float sudutPlatform;
boolean INPUTSERIAL             = true;
const byte PIN_RODA             = A0;
const byte PIN_SPEED            = A1;
const int GRAVITASI             = 10;
const float JARAK_BAN           = 2.45;
float minSpeed                  = 0;
float maxSpeed                  = 100;
float minRoda                   = -89;  // kiri
float maxRoda                   = 89;  // kanan
float minServo1                 = 0;
float maxServo1                 = 180; 
float midServo1                 = 90;
float minServo2                 = 0;
float maxServo2                 = 180;
float midServo2                 = 90;
float dataServo1                = midServo1;
float dataServo2                = midServo2;
float speedServo1               = 255;
float speedServo2               = 255;
float defaultMax                = 90;
float defaultMin                = -90;
float stepCorrection1           = 0;
float stepCorrection2           = 0;
float MIN_ERROR                 = 0.3;
float minSudutRoll              = -10;
float maxSudutRoll              = 10;
float midSudutRoll              = 0;
float minSudutPitch             = -12;
float maxSudutPitch             = 12;
float midSudutPitch             = 0;
float platformRoll              = 0; 
float platformRoll_last         = 0;
float platformRoll_dummy        = 0;
float selisihPlatform           = 0;
int dataSpeed                   = 0; // dari potensio atau serial
int dataRoda                    = 0;  // dari potensi atau serial
int tmpSpeed                    = 0; 
int tmpRoda                     = 0;
unsigned int accumSpeed         = 0;
unsigned int accumRoda          = 0;
byte maxSampel                  = 20;
unsigned long timing            = 0;
unsigned long lastchange_servo1 = 0;
float lastSpeed                 = 0;
unsigned long speed_timing      = 0;
float sudutPlatform             = 0.0;
String raw                      = "";