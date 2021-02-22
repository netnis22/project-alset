#inclode "Motor.h"

// Stabilizing PID
#define Kp 90.0
#define Kd  40.0 
#define Ki 0.0

#define VKp 0.07
#define VKd -0.01
#define VKi 0.0  //0.001

double OldError; // for calculating derivative

double ErrorSum; // for integration  
unsigned long OldTimer, OldSpeedTimer ;

double ReferenceSpeed;
double OldSpeedError; // for calculating derivative
double SpeedErrorSum; // for speed integration

double TurnReference;
double OldTurnError;
double TurnErrorSum;

double RightWheelSpeed;

int TurnCorrection_R;

//---------------------------------------
void setup() 
{
  Serial.begin(9600);

}

//---------------------------------------
void loop() 
{

}
