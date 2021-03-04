#include "Motors.h" 

volatile long RightMotorCounter;
volatile bool RightWheeltIsMovingForward;

long OlddRightMotorCounter;
int OldRightCurrentEncoderTicks;

long RightMotorCounterCopy;
bool RightWheeltIsMovingForwardCopy;

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

void CopyISRCounters()
{
    noInterrupts();
    
    RightMotorCounterCopy = RightMotorCounter;
    RightWheeltIsMovingForwardCopy = RightWheeltIsMovingForward;

    interrupts();
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
  
    //LeftMotorCounter = 0L;
    RightMotorCounter = 0L;
}
