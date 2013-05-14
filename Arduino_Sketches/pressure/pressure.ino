

int a,b;
int pressure;
int count=1;
int val=0;
void setup()
{
  Serial.begin(115200);  
  pinMode(A1,INPUT);
}

void loop()
{
  a=0;
  b=0;
  for(int i=0;i<5;i++)
  {
    pressure=analogRead(A1);
    if(pressure < 800)
    {
      a++;
    }
    else 
    b++;
  }
  if(b==5 && count==1)
  {
    count=0;
    b=0;
  }
  if(a==5 && count==0)
  {
   
    count=1;
    a=0;
    Serial.println('i');
    digitalWrite(7,HIGH);
    delayMicroseconds(20);
    digitalWrite(7,LOW);
    delay(50);
  }
}
