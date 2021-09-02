
#ifndef MOTOR_H
#define MOTOR_H
//===============================================
//motor encoder
#define encoder_Ticks_Rate 824.424

//===============================================
//motor power
#define Max_Motors_Power 191

//===============================================
// L298N defines
#define IN1 8 
#define IN2 9
#define IN3 10
#define IN4 12
#define ENA 5
#define ENB 6
#define M1_PHASE_A 2
#define M2_PHASE_A 3
#define M1_PHASE_B 4
#define M2_PHASE_B 7

//===============================================
//Whells
#define Weel_Diameter 6.45

// distance between wheels
#define DISTANCE 20

void MotorsInit();
void RightMotor(int power);
void LeftMotor(int power);
double GetRightWheelSpeed();
double GetLeftWheelSpeed();


#endif
