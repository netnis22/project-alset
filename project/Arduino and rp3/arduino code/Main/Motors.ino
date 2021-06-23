#include "Motors.h" 


volatile long LeftMotorCounter; 
volatile long RightMotorCounter;
volatile bool LeftWheelIsMovingForward;
volatile bool RightWheeltIsMovingForward;

long OldLeftMotorCounter, OlddRightMotorCounter;
int OldLeftCurrentEncoderTicks, OldRightCurrentEncoderTicks;

long LeftMotorCounterCopy; 
long RightMotorCounterCopy;
bool RightWheeltIsMovingForwardCopy;
bool LeftWheelIsMovingForwardCopy;

//======================================
double GetRightWheelSpeed()
{
    // 1. CopyISRCounters() must be called right before calling this function
    // 2. this function must be called every 50 ms but caculats average 
    // speed for the last 100 ms
    
    int currentEncoderTics = (RightMotorCounterCopy - OlddRightMotorCounter);
    OlddRightMotorCounter = RightMotorCounterCopy;
    
    // V = (2*PI*mu*R)/E 
    // mu - ticks per second, E - encoder tick per round
    // (currentEncoderTics + OldLeftCurrentEncoderTicks) is mu per 100 ms, should be per second
    // that's why it is multilied by 10
    double Vcurr = (PI * (double)(currentEncoderTics + OldRightCurrentEncoderTicks) * 10 * Weel_Diameter) / encoder_Ticks_Rate;
    
    OldRightCurrentEncoderTicks =  currentEncoderTics;
    
    return Vcurr;
}// end RightWheelPID

//======================================
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
    double Vcurr = (PI * (double)(currentEncoderTics + OldLeftCurrentEncoderTicks)* 10 * Weel_Diameter) / encoder_Ticks_Rate ;
    
    OldLeftCurrentEncoderTicks =  currentEncoderTics;

    return Vcurr;
}

//======================================

void CopyISRCounters()
{
    noInterrupts();
    LeftMotorCounterCopy = LeftMotorCounter; 
    RightMotorCounterCopy = RightMotorCounter;
    LeftWheelIsMovingForwardCopy = LeftWheelIsMovingForward;
    RightWheeltIsMovingForwardCopy = RightWheeltIsMovingForward;

    interrupts();
}

//======================================

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
    
}
//======================================
void RightEncoderISR()
{
     if(digitalRead(M2_PHASE_B))
    {
        RightWheeltIsMovingForward = true;
        RightMotorCounter --;
    }
    else
    {
        RightWheeltIsMovingForward = false;
        RightMotorCounter ++;
    }
    
}// end RightEncoderISR

//======================================

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
        
    if(power >  Max_Motors_Power)
            power =  Max_Motors_Power;
    
    if(direction)
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    else
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    
    analogWrite(ENB, power);
}
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
        
    if(power > Max_Motors_Power)
            power = Max_Motors_Power;
    
    if(direction)
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
    else
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    
    analogWrite(ENA, power);
}// end LeftMotor

//=============================================================
void MotorsInit()
{
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    
    pinMode(M1_PHASE_B, INPUT);
    pinMode(M2_PHASE_B, INPUT);
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  
    LeftMotorCounter = 0L;
    RightMotorCounter = 0L;
}
