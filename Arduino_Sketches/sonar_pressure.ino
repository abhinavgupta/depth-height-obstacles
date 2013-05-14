
int a,b;
int pressure,sonar;
char response[20];
int count=1;
int val=0;
void setup()
{
  Serial.begin(115200);
  
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(13,OUTPUT);
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
    sonar=analogRead(A0)/2;
    
      while(!Serial.available());
      int dataCounter = 0;
    while((val = Serial.read()) != ':') {
      if(val!= -1) {
        response[dataCounter] = val;
        ++dataCounter;
  
    }
 
  }
    
   
    
    if(sonar<100 && response[0]=='1')
    {
      digitalWrite(6,HIGH);
     
      delay(2000);
      digitalWrite(6,LOW);
    }
    else
    if(sonar>100 && response[0]=='1')
    {
    
      for(int j=0;j<5;j++)
      {
        digitalWrite(6,HIGH);
        delay(200);
        digitalWrite(6,LOW);
        delay(200);
      }
    }
    else
    if(sonar<100 && response[0]!='1')
    {
    
      digitalWrite(6,HIGH);
      delay(1000);
      digitalWrite(6,LOW);
    }
    else
    {
    digitalWrite(6,LOW);
    }
  }
}
