
#include "Motors.h"
#include "mpuangle.h"

#define ANGLE_OFFSET 1.0625
//=============================================================
// Stabilizing PID
#define Kp 90.0
#define Kd  40.0 
#define Ki 0.0

#define VKp 0.07
#define VKd -0.01
#define VKi 0.0  //0.001

#define OKp 2.0
#define OKd 0.0
#define OKi 0.0

double OldError; // for calculating derivative

double ErrorSum; // for integration  
unsigned long OldTimer, OldSpeedTimer ;
double ReferenceAngleY;

double ReferenceSpeed;
double OldSpeedError; // for calculating derivative
double SpeedErrorSum; // for speed integration

double TurnReference;
double OldTurnError;
double TurnErrorSum;

double RightWheelSpeed, LeftWheelSpeed;

int TurnCorrection_R;
int TurnCorrection_L;

//=============================================================
void StabilizeRobot()
{
    double compAngleY, kalAngleY;
    CalcYangle(kalAngleY, compAngleY);
    if(abs(kalAngleY) > 50.0)
    {
        RightMotor(0);
        LeftMotor(0);
        while(abs(kalAngleY) > 50.0);
    }
    double error = kalAngleY - ReferenceAngleY;
    
    double Kcorrection = Kp * error;
    double Dcorrection = Kd * (error - OldError);
    OldError = error;
    ErrorSum += Ki * error;
    double correction = Kcorrection + Dcorrection + ErrorSum;
    
    // trying to overcome the electronic backlash
    if(error > 0.0)
        Kcorrection += 60;
    if(error < 0.0)
        Kcorrection -= 60;
     
    RightMotor((int)correction + TurnCorrection_R);
    LeftMotor((int)correction + TurnCorrection_L);

}

//=============================================================
void SpeedControl()
{
    double vCurr = (RightWheelSpeed + LeftWheelSpeed) / 2.0;
    
    double error = ReferenceSpeed - vCurr;
    
    double Kcorrection = VKp * error;
    double Dcorrection = VKd * (error - OldSpeedError);
    OldSpeedError = error ;
    SpeedErrorSum += VKi * error;
    
    if(SpeedErrorSum >= 3.0)
        SpeedErrorSum = 3.0;
    else if(SpeedErrorSum <= -3.0)
        SpeedErrorSum = -3.0;
    
    double correction = Kcorrection + Dcorrection + SpeedErrorSum;
    if(correction >= 3.0)
        correction = 3.0;
    else if(correction <= -3.0)
        correction = -3.0;
    
    ReferenceAngleY = correction + ANGLE_OFFSET;
}

//=============================================================
void setup() 
{
    MotorsInit();
  
    Serial.begin(9600);
    
    if(!MPUangleInit())
    {
        Serial.print(F("Error reading sensor"));
        while(1);
    }
    
    ErrorSum = SpeedErrorSum = TurnErrorSum = 0.0 ;
    ReferenceAngleY = ANGLE_OFFSET;
    ReferenceSpeed = 0.0;
    TurnReference = 0.0; //(15.0/180) * PI; 
    TurnCorrection_L = TurnCorrection_R = 0;
    
    attachInterrupt(digitalPinToInterrupt(M1_PHASE_A), LeftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(M2_PHASE_A), RightEncoderISR, RISING);
    
    OldTimer = OldSpeedTimer = millis();
}// end setup

//=============================================================
String rxbuff = String();
bool startFound = false;
bool speedData = false;

void CommProcess()
{
    if(Serial.available() <= 0)
        return;
    
    int ch = Serial.read();
    if(ch == 'S')
    {
        speedData = true;
        return;       
    }
    else if(ch == 'T')
    {
        speedData = false;
        return;
    }
    else if(ch == '#')
    {
        if(speedData)
            ReferenceSpeed = rxbuff.toFloat();
        else
            TurnReference = rxbuff.toFloat();
        
        rxbuff = String() ;
        return;
    }
    
    rxbuff += (char)ch ;
}

//=============================================================
void loop() 
{
    CommProcess();
    
    unsigned long currMillis = millis();
    if(currMillis - OldTimer >= 10L)
    {
        OldTimer = currMillis;
        StabilizeRobot();   
    }// end if
    
    if (currMillis - OldSpeedTimer >= 50L)
    {
        OldSpeedTimer = currMillis;
        CopyISRCounters();
        
        // get wheelSpeed must be called only once per 50 ms
        RightWheelSpeed = GetRightWheelSpeed();
        LeftWheelSpeed = GetLeftWheelSpeed();
        
        SpeedControl();
        
        double omegaCurr = (RightWheelSpeed - LeftWheelSpeed) / L_DISTANCE;
        double error = TurnReference - omegaCurr;
        double Kcorrection = OKp * error;
        double Dcorrection = OKd * (error - OldTurnError);
        OldTurnError = error;
        TurnErrorSum += OKi * error;
        double correction = Kcorrection + Dcorrection + TurnErrorSum;
        TurnCorrection_R = correction;
        TurnCorrection_L = -correction;
        
         Serial.print(TurnCorrection_R);
        Serial.print(", ");
        Serial.println(TurnCorrection_L);

    }
} //end loop
