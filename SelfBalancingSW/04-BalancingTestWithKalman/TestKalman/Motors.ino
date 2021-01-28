#include "Motors.h"

volatile long LeftMotorCounter; 
volatile long RightMotorCounter;
volatile bool LeftWheelIsMovingForward;
volatile bool RighWheeltIsMovingForward;

long OldLeftMotorCounter, OldRightMotorCounter;
int OldLeftCurrentEncoderTicks, OldRightCurrentEncoderTicks;

// Copied out by CopyISRCounters() under critical section
long LeftMotorCounterCopy; 
long RightMotorCounterCopy;
bool LeftWheelIsMovingForwardCopy;
bool RighWheeltIsMovingForwardCopy;

//=============================================================
double GetLeftWheelSpeed()
{
    // 1. CopyISRCounters() must be called right before calling this function
    // 2. this function must be called every 50 ms but caculats average 
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

    return Vcurr;
}// end LeftWheelPID

//=============================================================
double GetRightWheelSpeed()
{
    // 1. CopyISRCounters() must be called right before calling this function
    // 2. this function must be called every 50 ms but caculats average 
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
    
    return Vcurr;
}// end RightWheelPID

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
void MotorsInit()
{
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
}

