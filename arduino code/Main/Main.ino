#include "Motors.h"

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

int ComSpeed;
double OldSpeedError; // for calculating derivative
double SpeedErrorSum; // for speed integration

int TurnReference;
double OldTurnError;
double TurnErrorSum;

double RightWheelSpeed;




//=============================================================
int incomingByte = 0;
char letter = '\0';
String data = String('$');
String ComplitData = String();

void CommProcess()
{
  if(Serial.available() <= 0)
    {
      ComplitData = String('$');
      return;
    }
 if(Serial.available()>0)
  {
    int incomingByte = Serial.read();
    if(incomingByte != 95)
    {
      letter = incomingByte;
      if(data == "$")
      {
        data = String(letter);
      }
      else
      {
        data +=letter;
      }
    }
    if(incomingByte == 95)
    {
      ComplitData = String(data);
      data = String('$');
      
    }
  }
}
//=============================================================
int ComplitDataLength;
String ComSpeedSTR = String();
String TurnReferenceSTR = String();
int IndexOfT;

void Translation()
{
  if(ComplitData != "$")
  {
    ComplitDataLength = ComplitData.length();
    IndexOfT = ComplitData.indexOf("T");
    
    ComSpeedSTR = String(ComplitData.substring(1,IndexOfT));
    TurnReferenceSTR = String(ComplitData.substring(IndexOfT+1,ComplitDataLength));

    ComSpeed = ComSpeedSTR.toInt();
    TurnReference = TurnReferenceSTR.toInt();
    Serial.print(ComplitData);
    Serial.print('\n');
    Serial.print(ComSpeed);
    Serial.print('\n');
    Serial.print(TurnReference);
  }
}
//=============================================================

//=============================================================
void SpeedControl()
{
  double vCurr = RightWheelSpeed;
  double error = (ComSpeed - vCurr);
  double Kcorrection = VKp * error;
  double Dcorrection = VKd * (error - OldSpeedError);

  OldSpeedError = error ;
  SpeedErrorSum += VKi * error;

  if(SpeedErrorSum >= 3.0)
  {
    SpeedErrorSum = 3.0;
  }
  else if(SpeedErrorSum <= -3.0)
  {
    SpeedErrorSum = -3.0;
  }
    
  double correction = Kcorrection + Dcorrection + SpeedErrorSum;
  if(correction >= 3.0)
  {
    correction = 3.0;
  }
  else if(correction <= -3.0)
  {
    correction = -3.0;
  }
  
}


//---------------------------------------
void setup() 
{
  MotorsInit();
  Serial.begin(9600);

  ErrorSum = SpeedErrorSum = TurnErrorSum = 0.0;
  ReferenceSpeed = 0.0;
  TurnReference = 0.0;

  OldTimer = OldSpeedTimer = millis();
}

//---------------------------------------
void loop() 
{
  CommProcess();
  Translation();
  
}
