
int a,b;
int pressure,sonar;
char response[20];
int count=1;
int val=0;
int flag=0;
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
  while(!Serial.available() > 0)
  {
  a=0;
  b=0;
  for(int i=0;i<5;i++)
  {
    pressure=analogRead(A1);
    //Serial.println(pressure);
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
    digitalWrite(7,HIGH);
    delayMicroseconds(20);
    digitalWrite(7,LOW);
    delay(50);
    sonar=analogRead(A0)/2;
    //Serial.println(sonar);
    flag=1;
  }
  
  }
  
  int dataCounter = 0;
    while((val = Serial.read()) != ':') {
      if(val!= -1) {
        response[dataCounter] = val;
        ++dataCounter;
    }
    }
    
  if(flag==1)
{
   if(sonar<100 && response[0]=='1')
    {
      digitalWrite(6,HIGH);
      //Serial.println("cs");
      delay(2000);
      digitalWrite(6,LOW);
    }
    else
    if(sonar>100 && response[0]=='1')
    {
      //Serial.println('s');
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
      //Serial.println('c');
      digitalWrite(6,HIGH);
      delay(1000);
      digitalWrite(6,LOW);
    }
    else
    {
    digitalWrite(6,LOW);
    }
  
flag=0;
}
    }
  
