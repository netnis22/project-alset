/*
const int IN1 = 6;
const int IN2 = 7;
const int ENA = 9;

#define motor_A1 2
#define motor_A2 3
#define motor_B1 4
#define ENA 5
#define ENB 6
#define motor_B2 7
#define INT1 8
#define INT2 9
#define INT3 10
#define INT4 11
*/

// L298N defines
#define IN1 8 
#define IN2 9
#define IN3 10
#define IN4 12
#define ENA 5
#define ENB 6

int SPEED;

//---------------------------------------
void setup() 
{
  Serial.begin(9600);
  
  for(int pin = IN1; pin <= IN4; pin++)
    pinMode (pin, OUTPUT);
  
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  SPEED = 128;
  //control speed 
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
}

//---------------------------------------
void move(int speed, int omega)
{
  SPEED = speed;
  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);
  
  if(direction)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

//---------------------------------------
void loop() 
{
  move(80, 0);

  delay(1000);
}
