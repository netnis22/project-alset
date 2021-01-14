//Motors Test

#include <MPU6050_tockn.h>
#include <Wire.h>

//=============================================================
MPU6050 mpu6050(Wire);

//=============================================================
// Motor encoders
// Motor 1 == Left Motor
// Motor 2 == Right Motor

#define ENCODER_TICKS_PER_ROUND 824.424

// pins 2, 3 supporting interrupts
#define M1_PHASE_A 2
#define M2_PHASE_A 3
#define M1_PHASE_B 4
#define M2_PHASE_B 7

//=============================================================
// L298N controls
// PWM for motors
#define ENA 5
#define ENB 6
// motors supply 12V, motors spec 9V
// (9/12) = 191 // MAX Power
#define MAX_MOTORS_POWER 191

#define EN1 8
#define EN2 9
#define EN3 10
#define EN4 12

//=============================================================
// Wheels
// Wheel diameter 6.45 cm
#define WHEEL_DIAMETER 6.45
volatile long LeftMotorCounter; 
volatile long RightMotorCounter;
volatile bool LeftWheelIsMovingForward;
volatile bool RighWheeltIsMovingForward;

// Copied out by CopyISRCounters() under critical section
long LeftMotorCounterCopy; 
long RightMotorCounterCopy;
bool LeftWheelIsMovingForwardCopy;
bool RighWheeltIsMovingForwardCopy;

//=============================================================
// Wheels PID variavles
#define WKp 5.0
#define WKd 1.0
#define WKi 0.0

long OldLeftMotorCounter, OldRightMotorCounter;
unsigned long OldPIDMillis;
int OldLeftCurrentEncoderTicks, OldRightCurrentEncoderTicks; // keeps old 50ms count
double OldLeftError, OldRightError;
double LeftIntegration, RightIntegration;

//=============================================================
// Balancing PID variavles
#define AKp 20.0
#define AKd 5.0
#define AKi 0.2
double AngleIntegration, OldAngleError;

//=============================================================
void WheelsPID(double LeftVr, double RifgtVr)
{
    CopyISRCounters();
    LeftWheelPID(LeftVr);
    RightWheelPID(RifgtVr);
}// end WheelPID

//=============================================================
void LeftWheelPID(double LeftVr)
{
    // this function is called every 50 ms but caculats average 
    // speed for the last 100 ms
    int currentEncoderTics = LeftMotorCounterCopy - OldLeftMotorCounter;
    OldLeftMotorCounter = LeftMotorCounterCopy;
    // V = (2*PI*mu*R)/E 
    // mu - ticks per second, E - encoder tick per round
    // (currentEncoderTics + OldLeftCurrentEncoderTicks) is mu per 100 ms, should be per second
    // that's why it is multilied by 10
    double Vcurr = (PI * (double)(currentEncoderTics + OldLeftCurrentEncoderTicks)
                                   * 10 * WHEEL_DIAMETER) / ENCODER_TICKS_PER_ROUND ;
    
    OldLeftCurrentEncoderTicks =  currentEncoderTics;
    
    double error = LeftVr - Vcurr;
    LeftIntegration += error * WKi;
    int power = error * WKp + (error - OldLeftError)*WKd + LeftIntegration;
    
    OldLeftError = error;
    
    LeftMotor(power);
    
}// end LeftWheelPID

//=============================================================
void RightWheelPID(double RightVr)
{
        // this function is called every 50 ms but caculats average 
    // speed for the last 100 ms
    int currentEncoderTics = RightMotorCounterCopy - OldRightMotorCounter;
    OldRightMotorCounter = RightMotorCounterCopy;
    // V = (2*PI*mu*R)/E 
    // mu - ticks per second, E - encoder tick per round
    // (currentEncoderTics + OldLeftCurrentEncoderTicks) is mu per 100 ms, should be per second
    // that's why it is multilied by 10
    double Vcurr = (PI * (double)(currentEncoderTics + OldRightCurrentEncoderTicks)
                                   * 10 * WHEEL_DIAMETER) / ENCODER_TICKS_PER_ROUND ;
    
    OldRightCurrentEncoderTicks =  currentEncoderTics;
    
    double error = RightVr - Vcurr;
    RightIntegration += error * WKi;
    int power = error * WKp + (error - OldRightError)*WKd + RightIntegration;
    
    OldRightError = error;
            
    RightMotor(power);
}// end RightWheelPID

//=============================================================
double BalancingPID(double yAngleRef)
{
    mpu6050.update();
    double yAngle = mpu6050.getAngleY();
    Serial.println(yAngle);
    double error = yAngle - yAngleRef;
    AngleIntegration += AKi * error;
    
    double correction = AKp * error + AKd * (error - OldAngleError) + AKi * error;
    if(correction > MAX_MOTORS_POWER)
        correction = MAX_MOTORS_POWER;
    else if(correction < -MAX_MOTORS_POWER)
        correction = -MAX_MOTORS_POWER;
    
    return correction;
}// end BalancingPID

//=============================================================
void LeftEncoderISR()
{
    if(digitalRead(M1_PHASE_B))
    {
        LeftWheelIsMovingForward = true;
        LeftMotorCounter ++;
    }
    else
    {
        LeftWheelIsMovingForward = false;
        LeftMotorCounter --;
    }
    
}// end LeftEncoderISR

//=============================================================
void RightEncoderISR()
{
     if(digitalRead(M2_PHASE_B))
    {
        RighWheeltIsMovingForward = true;
        RightMotorCounter --;
    }
    else
    {
        RighWheeltIsMovingForward = false;
        RightMotorCounter ++;
    }
    
}// end RightEncoderISR


//=============================================================
void setup() 
{
    Serial.begin(38400);
    while(!Serial);
    
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    pinMode(M1_PHASE_B, INPUT);
    pinMode(M2_PHASE_B, INPUT);
    
    pinMode(EN1, OUTPUT);
    pinMode(EN2, OUTPUT);
    pinMode(EN3, OUTPUT);
    pinMode(EN4, OUTPUT);
  
    LeftMotorCounter = 0L;
    RightMotorCounter = 0L;
    LeftIntegration = 0;
    RightIntegration = 0;
    
    AngleIntegration = 0;
    OldAngleError = 0;

    Wire.begin();
    mpu6050.begin();
  //mpu6050.calcGyroOffsets();
    mpu6050.calcGyroOffsets(true);

    attachInterrupt(digitalPinToInterrupt(M1_PHASE_A), LeftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(M2_PHASE_A), RightEncoderISR, RISING);
    
    OldPIDMillis = millis();

}// end setup

//=============================================================
void CopyISRCounters()
{
    noInterrupts();
    
    LeftMotorCounterCopy = LeftMotorCounter; 
    RightMotorCounterCopy = RightMotorCounter;
    LeftWheelIsMovingForwardCopy = LeftWheelIsMovingForward;
    RighWheeltIsMovingForwardCopy = RighWheeltIsMovingForward;

    interrupts();
}

//=============================================================
void loop() 
{
    // Wheels' PID is working on window of 100 ms that slids forward
    // every 50 ms, i.e. PID caloculates errors and produces corrections
    // every 50 ms on the last 100 ms (with ovrlapping)
    
    unsigned long tmpMillis = millis();
    
    if(tmpMillis - OldPIDMillis >= 50)
    {
        int correction = BalancingPID(0.0);
        
        Serial.println(correction);
        WheelsPID(correction, correction);
        OldPIDMillis = tmpMillis;
    }

}// end loop

//=============================================================
void LeftMotor(int power)
{
    // if power >= 0 then move forward, direction = true
    // otherwise move backward, direction = false and make power positive
    // 
    bool direction = false;
    
    if (power >= 0)
        direction = true;
    else
        power *= -1;
        
    if(power > MAX_MOTORS_POWER)
            power = MAX_MOTORS_POWER;
    
    if(direction)
    {
        digitalWrite(EN1, LOW);
        digitalWrite(EN2, HIGH);
    }
    else
    {
        digitalWrite(EN1, HIGH);
        digitalWrite(EN2, LOW);
    }
    
    analogWrite(ENA, power);
}// end LeftMotor

//=============================================================
void RightMotor(int power)
{
    // if power >= 0 then move forward, direction = true
    // otherwise move backward, direction = false and make power positive
    // 
    bool direction = false;
    
    if (power >= 0)
        direction = true;
    else
        power *= -1;
        
    if(power > MAX_MOTORS_POWER)
            power = MAX_MOTORS_POWER;
    
    if(direction)
    {
        digitalWrite(EN3, LOW);
        digitalWrite(EN4, HIGH);
    }
    else
    {
        digitalWrite(EN3, HIGH);
        digitalWrite(EN4, LOW);
    }
    
    analogWrite(ENB, power);
}// end LeftMotor

