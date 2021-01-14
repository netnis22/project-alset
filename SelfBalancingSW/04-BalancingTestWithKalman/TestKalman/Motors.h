
#ifndef MOTORS_H    
#define MOTORS_H    

// Place your main header code here.



// Motors


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

// distance between wheels
#define L_DISTANCE 18.5

void MotorsInit();
void RightMotor(int power);
void LeftMotor(int power);
double GetRightWheelSpeed();
double GetLeftWheelSpeed();

#endif // MOTORS_H