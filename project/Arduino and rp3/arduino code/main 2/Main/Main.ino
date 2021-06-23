#include "Motors.h"


#define VKp 10
#define VKd 0// vk/dt -0.5/50
#define VKi 0.0  //0.001

#define OKp 10
#define OKd 0.0
#define OKi 0.0

double OldError; 

double ErrorSum; 
unsigned long OldTimer, OldSpeedTimer ;

int ComSpeed;
double OldSpeedError; 
double SpeedErrorSum; 

double TurnReference;
double OldTurnError;
double TurnErrorSum;

double ReferenceAngleY;

double RightWheelSpeed, LeftWheelSpeed;

double correction;

int TurnCorrection_R;
int TurnCorrection_L;

//=============================================================


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
    TurnReference = TurnReferenceSTR.toDouble(); 
    
    
  }

    //Serial.print(ComSpeed);
    //Serial.print('\n');
}
//=============================================================
int Er;

void StabilizeRobot()
{

    double correction = ReferenceAngleY;

    




    double FinalPowerR=((int)correction + (int)TurnCorrection_R );
    double FinalPowerL((int)correction + (int)TurnCorrection_L);

    //Print(correction,FinalPowerR,FinalPowerL);
    
    RightMotor(FinalPowerR);
    LeftMotor(FinalPowerL);
   
   
   
 

}

//=============================================================

void Print(double correction, double FinalspeedR, double FinalspeedL)
{
  Serial.print(" FinalspeedR: ");
  Serial.print(FinalspeedR);
  Serial.print(" FinalspeedL: ");
  Serial.print(FinalspeedL);
  Serial.print(" correction: ");
  Serial.print(correction);
  Serial.print('\n');
}
void PrintSpeetControl(double RightWheelSpeed ,double LeftWheelSpeed, double vCurr, double error, double Kcorrection, double Dcorrection, double OldSpeedError, double SpeedErrorSum, double correction , int Er )
{
  //Serial.print(" RightWheelSpeed: ");
  //Serial.print(RightWheelSpeed);
  //Serial.print(" LeftWheelSpeed: ");
  //Serial.print(LeftWheelSpeed);
  Serial.print(" vCurr: ");
  Serial.print(vCurr);
  Serial.print(" error: ");
  Serial.print(error);
  //Serial.print(" Kcorrection: ");
  //Serial.print(Kcorrection);
  //Serial.print(" Dcorrection: ");
  //Serial.print(Dcorrection);
  //Serial.print(" OldSpeedError: ");
  //Serial.print(OldSpeedError);
  //Serial.print(" SpeedErrorSum: ");
  //Serial.print(SpeedErrorSum);
  Serial.print(" correction: ");
  Serial.print(correction);
  Serial.print(" Er: ");
  Serial.print(Er);
  Serial.print('\n');
  
}


//=============================================================
void SpeedControl()
{
  Er = (RightWheelSpeed - LeftWheelSpeed) / (RightWheelSpeed /ComSpeed);


  
  double vCurr = (RightWheelSpeed + LeftWheelSpeed) / 2.0;
  double error = (ComSpeed - vCurr);
  
  double Kcorrection = VKp * error;
   //
   if(ComSpeed >=0 && Kcorrection < 0)
    Kcorrection = 0;
   else if(ComSpeed <= 0 && Kcorrection > 0)
    Kcorrection = 0;
  

  
  double Dcorrection = VKd * (error - OldSpeedError);

  OldSpeedError = error ;
  //SpeedErrorSum += VKi * error;
/*
  if(SpeedErrorSum >= 3.0)
  {
    SpeedErrorSum = 3.0;
  }
  else if(SpeedErrorSum <= -3.0)
  {
    SpeedErrorSum = -3.0;
  }*/
  
    
 // correction = Kcorrection + Dcorrection + SpeedErrorSum;
  correction = Kcorrection + Dcorrection;
  /*if(correction >= 3.0)
  {
    correction = 3.0;
  }
  else if(correction <= -3.0)
  {
    correction = -3.0;
  }*/
  
  ReferenceAngleY = correction;
  //PrintSpeetControl( RightWheelSpeed , LeftWheelSpeed,  vCurr,  error,  Kcorrection,  Dcorrection,  OldSpeedError,  SpeedErrorSum,  correction ,Er);
  
}


//---------------------------------------
void setup() 
{
  MotorsInit();
  Serial.begin(9600);

  ErrorSum = SpeedErrorSum = TurnErrorSum = 0.0;
  ReferenceAngleY = 0;
  ComSpeed = 0.0;
  TurnReference = 0.0;
  TurnCorrection_L = TurnCorrection_R = 0;
  RightWheelSpeed = LeftWheelSpeed = 0.0;
  Er = 0;
  attachInterrupt(digitalPinToInterrupt(M1_PHASE_A), LeftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(M2_PHASE_A), RightEncoderISR, RISING);
  OldTimer = OldSpeedTimer = millis();
}

//---------------------------------------
void loop() 
{
  CommProcess();
  Translation();
    
  
  
    unsigned long currMillis = millis();
    if(currMillis>10000){
    if(currMillis - OldTimer >= 10L)
    {
        OldTimer = currMillis;
         StabilizeRobot(); 
    }
   if (currMillis - OldSpeedTimer >= 50L)
    {
      OldSpeedTimer = currMillis;
      CopyISRCounters();

      RightWheelSpeed = GetRightWheelSpeed();
      LeftWheelSpeed = GetLeftWheelSpeed();

      SpeedControl();

       double omegaCurr = (RightWheelSpeed - LeftWheelSpeed) / DISTANCE;
       double error = TurnReference*10-omegaCurr;
       double Kcorrection = OKp * error;
       double Dcorrection = OKd * (error - OldTurnError);
       OldTurnError = error;
       TurnErrorSum += OKi * error;
       double correction = Kcorrection + Dcorrection + TurnErrorSum;

       TurnCorrection_R = correction;
       TurnCorrection_L = -correction;
       
       
      
    }
    }
  
}
