
int incomingByte = 0;
char letter = '\0';
String data = String('$');

void setup() {
  Serial.begin(9600);

}

void loop() {
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
      data = String('$');
    }
  }

}
