
// Robot code
// Used to control robot car with Arduino Zero

#include <IRremote.h>
#include <Servo.h>

#define F_KEY 16736925
#define B_KEY 16754775
#define L_KEY 16720605
#define R_KEY 16761405
#define S_KEY 16712445
#define UNKNOWN_F 5316027
#define UNKNOWN_B 2747854299
#define UNKNOWN_L 1386468383
#define UNKNOWN_R 553536955
#define UNKNOWN_S 3622325019
#define send

int Echo = A4;  
int Trig = A5; 
int rightDistance = 0,leftDistance = 0,middleDistance = 0;

int RECV_PIN = 12;
int in1=9;
int in2=8;
int in3=7;
int in4=6;
int ENA=10;
//int ENA = 11;
int ENB=5;
int ABS=115;
//int ABS = 130;

IRrecv irrecv(RECV_PIN);
Servo myservo; // create servo object to control servo
decode_results results;
unsigned long val;

 /*Ultrasonic distance measurement Sub function*/
 int Distance_test()   
 {
   digitalWrite(Trig, LOW);   
   delayMicroseconds(2);
   digitalWrite(Trig, HIGH);  
   delayMicroseconds(20);
   digitalWrite(Trig, LOW);   
   float Fdistance = pulseIn(Echo, HIGH);  
   Fdistance= Fdistance/58;       
   return (int)Fdistance;
 }  
 
void _mForward()
{ 
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,HIGH); // right
  digitalWrite(in2,LOW);  // right
  digitalWrite(in3,LOW);  // left
  digitalWrite(in4,HIGH); // left
  Serial.println("go forward!");
}
void _mBack()
{
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("go back!");
}
void _mleft()
{
  analogWrite(ENA,ABS);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW); 
  Serial.println("go left!");
}
void _mright()
{
  analogWrite(ENA,ABS);
  digitalWrite(ENB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("go right!");
}
void _mStop()
{
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("STOP!");  
}
void _rright()
{
  digitalWrite(ENA,ABS);
  digitalWrite(ENB,ABS);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);      
  digitalWrite(in4,HIGH);         //Left wheel forward
}
void _rleft()
{
  digitalWrite(ENA,ABS);
  digitalWrite(ENB,ABS);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);      
  digitalWrite(in4,LOW);
}

void setup() {
  myservo.attach(3);// attach servo on pin 3 to servo object
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  _mStop();
  irrecv.enableIRIn();  
  Serial.begin(9600);
}
bool forward = false;
void loop() {
  myservo.write(90);//setservo position according to scaled value
  delay(500); 
  middleDistance = Distance_test();
  #ifdef send
  Serial.print("middleDistance=");
  Serial.println(middleDistance);
  #endif

  if (forward)
  {
    if(middleDistance<=20)
    {     
      _mStop();
      delay(500);                        
      myservo.write(10);//10°-180°          
      delay(1000);      
      rightDistance = Distance_test();

      #ifdef send
      Serial.print("rightDistance=");
      Serial.println(rightDistance);
      #endif

      delay(500);
      myservo.write(90);              
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test();

      #ifdef send
      Serial.print("leftDistance=");
      Serial.println(leftDistance);
      #endif

      delay(500);
      myservo.write(90);              
      delay(1000);
      if(rightDistance>leftDistance)  
      {
        _mright();
        delay(180);
      }
      else if(rightDistance<leftDistance)
      {
        _mleft();
        delay(180);
      }
      else if((rightDistance<=20)||(leftDistance<=20))
      {
        _mBack();
        delay(180);
      }
      else
      {
        _mForward();
      }
    }  
    else
    {
      _mForward();                     
    }
  }

  if (irrecv.decode(&results)){ 
    val = results.value;
    Serial.println(val);
    irrecv.resume();
    switch(val){
      case F_KEY: 
      case UNKNOWN_F: forward=true;_mForward();break;
      case B_KEY: 
      case UNKNOWN_B: forward=false;_mBack(); break;
      case L_KEY: 
      case UNKNOWN_L: forward=true;_mleft(); break;
      case R_KEY: 
      case UNKNOWN_R: forward=true;_mright();break;
      case S_KEY: 
      case UNKNOWN_S: forward=false;_mStop(); break;
      default:break;
    }
  }
} 

