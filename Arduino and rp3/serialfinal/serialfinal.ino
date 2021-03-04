


int incomingByte = 0;
char letter = '\0';
String data = String('$');
String ComplitData = String();

void CommProcess()
{
  
  if(Serial.available() <= 0)
    {
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
      Serial.println(data);
      ComplitData = String(data);
     
      data = String('$');
      
    }
  }
}


//=============================================================
void setup() 
{
  Serial.begin(9600);

}

void loop()
{
  
  CommProcess();
}



