/*
 Name:    test.ino
 Created: 7/19/2019 7:59:59 AM
 Author:  moshe
*/

// the setup function runs once when you press reset or power the board
#define U_SENSOR 2

unsigned long count = 0, oldcount = 0;
unsigned long myMillis, oldMillis;

const byte interruptPin = 3;
volatile unsigned long encoderCount = 0;

int prevUsensor;
void setup() 
{
  Serial.begin(38400);
  while (!Serial);
  Serial.print("Waiting 5 seconds");
  delay(5000);
  pinMode(2, INPUT);
  while (digitalRead(U_SENSOR) == 0);
  while (digitalRead(U_SENSOR) == 1);
  while (digitalRead(U_SENSOR) == 0);
  attachInterrupt(digitalPinToInterrupt(interruptPin), EncoderISR, RISING);
  prevUsensor = 1;
  oldMillis = millis();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
  int usense = digitalRead(U_SENSOR);
  if (prevUsensor == 0 && usense == 1)
  {
    count++;
    if(count == 1000)
      detachInterrupt(interruptPin);

    myMillis = millis();
    Serial.print(count + String(", "));
    Serial.println(myMillis - oldMillis);
    oldMillis = myMillis;

    if(count == 1000)
    {
      Serial.print("\nencoder conter == ");
      Serial.println(encoderCount);
      while(1);
    }
    
  }
  prevUsensor = usense;
}

void EncoderISR()
{
  encoderCount ++;
}
