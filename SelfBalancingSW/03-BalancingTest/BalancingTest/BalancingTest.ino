// Balancing Test


#include <MPU6050_tockn.h>
#include <Wire.h>

//=============================================================
MPU6050 mpu6050(Wire);
// MPU6050 mpu6050(Wire, 0.1, 0.9);


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
#define MAX_MOTORS_POWER 255

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
// Balancing PID variavles
#define BALANCE_ANGLE_OFFSET 1.09375
#define AKp 10.0
#define AKd 0.0
#define AKi 0.0

double AngleIntegration, OldAngleError;
double BalanceAngleOffset;

unsigned long OldBalancingMillis;

//=============================================================
// Speed PID variavles

#define MAX_TILT_ANGLE 0.5

#define LKp 0.0
#define LKd 0.0
#define LKi 0.0

bool LocationIsSaved;
long LeftLocationSaved, RightLocationSaved;
double LocationIntegration, OldLocationError;
unsigned long OldSpeedMillis;

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

//=============================================================
/* double BalancingPID(double yAngleRef)
{
    mpu6050.update();
    double yAngle = mpu6050.getAngleY() + BalanceAngleOffset;
    Serial.print(yAngle);
    Serial.print(", ");
    double error = yAngle - yAngleRef;
    AngleIntegration += AKi * error;
    
    if(AngleIntegration > MAX_MOTORS_POWER)
        AngleIntegration = MAX_MOTORS_POWER;
    else if(AngleIntegration < -MAX_MOTORS_POWER)
        AngleIntegration = -MAX_MOTORS_POWER;
    
    double correction = AKp * error + AKd * (error - OldAngleError) + AngleIntegration;
    OldAngleError = error;
    
    if(correction > 7 )
        correction += 60;
    if(correction < -7)
        correction += -60;
    
    if(correction > MAX_MOTORS_POWER)
        correction = MAX_MOTORS_POWER;
    else if(correction < -MAX_MOTORS_POWER)
        correction = -MAX_MOTORS_POWER;
    Serial.println(correction);
    return correction;
}// end BalancingPID */

//=============================================================
double yAngle1;
double BalancingPID(double yAngleRef)
{
    mpu6050.update();
    double yAngle = yAngle1 = mpu6050.getAngleY() + BalanceAngleOffset;

    //double error = yAngle - yAngleRef;
    
    // taking the absolute value of error
    bool negativeError = false;
    if(yAngle < 0.0)
    {
        negativeError = true;
        yAngle = 0.0 - yAngle;
    }
    
    double Kp;

    if(yAngle < 0.35)
        Kp = 0.0;
    else if(yAngle < 1)
        Kp = 15.38 * yAngle + 64.62;
    else if(yAngle < 2)
        Kp = 60 * yAngle + 20;
    else
        Kp = 115 * yAngle - 90;
    
    if(negativeError)
        Kp = 0.0 - Kp;
    return Kp;
}// end BalancingPID

//=============================================================
double KeepLocationPID()
{
    CopyISRCounters();
    
    if(!LocationIsSaved)
    {
        LeftLocationSaved = LeftMotorCounterCopy;
        RightLocationSaved = RightMotorCounterCopy;
        LocationIsSaved = true;
    }
    double error = LeftLocationSaved - LeftMotorCounterCopy;
    LocationIntegration += LKi * error;
    if(LocationIntegration > MAX_TILT_ANGLE)
        LocationIntegration = MAX_TILT_ANGLE;
    else if(LocationIntegration < -MAX_TILT_ANGLE)
        LocationIntegration = -MAX_TILT_ANGLE;
    
    double correction = LKp * error + LKd * (error - OldLocationError) + LocationIntegration;
    OldLocationError = error;
    
    if(correction > MAX_TILT_ANGLE)
        correction = MAX_TILT_ANGLE;
    else if(correction < -MAX_TILT_ANGLE)
        correction = -MAX_TILT_ANGLE;
    
    return correction;
    
}//end KeepLocationPID

//=============================================================
void setup() 
{
    Serial.begin(115200);
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
    
    BalanceAngleOffset = BALANCE_ANGLE_OFFSET;
    AngleIntegration = 0.0;
    LocationIsSaved = false;
    
    LocationIntegration = 0.0;
    
    Wire.begin();
    mpu6050.begin();
  //mpu6050.calcGyroOffsets();
    mpu6050.calcGyroOffsets(true);

    attachInterrupt(digitalPinToInterrupt(M1_PHASE_A), LeftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(M2_PHASE_A), RightEncoderISR, RISING);
    
    OldBalancingMillis = OldSpeedMillis = millis();
}// end setup

//=============================================================
double angle = 100.0;
bool motorControl = false;
int nl = 0;
void loop() 
{
    /* double speed = 0.0, speedAngle; */
    
    unsigned long tmpMillis = millis();
    if(Serial.available() > 0)
    {
        int ch = Serial.read();
        if(ch == 'b')
            motorControl = true;
        if(ch == 'e')
        {
            motorControl = false;
            angle = 0.0;
            LeftMotor(angle);
            RightMotor(angle);
            while(1)
                delay(1000);
        }
        if(ch == 's')
        {
            motorControl = false;
            angle = 0.0;
            LeftMotor(angle);
            RightMotor(angle);
        }
    }
    
  /*  if(tmpMillis >= OldSpeedMillis + 50)
    {
        if(speed == 0.0)
        {
            speedAngle = KeepLocationPID();
        }
        else
        {
            LocationIsSaved = false;
            speedAngle = 0.0 ; //SetSpeedPID(speed);
        }
            
        OldSpeedMillis = millis();
    } */
    
    if(tmpMillis >= OldBalancingMillis + 5L)
    {
        
        // int correction = BalancingPID(angle);
        BalancingPID(angle);

        Serial.print(yAngle1);
        Serial.print(", ");
        nl ++;
        if(nl == 10)
        {
            Serial.println();
            nl = 0;
        }
        
        if(!motorControl)
        {
            OldBalancingMillis = tmpMillis;
            return;
        }
        
        int correction = angle;
        LeftMotor(correction);
        RightMotor(correction);
        OldBalancingMillis = tmpMillis;
        
        if(yAngle1 < 0)
        {
            Serial.print("*******************");
            Serial.println(angle);
        }
        
        
        if(tmpMillis >= OldSpeedMillis + 50L)
        {
            OldSpeedMillis = tmpMillis;
            angle = 100.0;
            if(angle > 190.0)
                angle = 100.0;
        }
    }
    
}// end loop
