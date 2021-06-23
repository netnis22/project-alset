
int ComSpeed;
int TurnReference;

//=================================================
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

void setup() 
{
  Serial.begin(9600);

}

void loop()
{
  CommProcess();
  Translation();
  
  
}



