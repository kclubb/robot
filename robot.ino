
// Robot code
// Used to control robot car with Arduino Zero
// For Terumobct project

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

int start_speed = 20;
int RECV_PIN = 12;
int in1=9;
int in2=8;
int in3=7;
int in4=6;
int ENA=10;
//int ENA = 11;
int ENB=5;
int abs_a=start_speed;
int abs_b=start_speed;
//int ABS_A = 130;

bool forward = false;
bool moving = false;
long lastKey = 0L;


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
 
void _moveStraight(bool bforward, bool speedup)
{
  moving = true;
  forward = bforward;
  if (speedup)
  {
    abs_a += 20;
    if (abs_a >= 255) abs_a = 255;
  }
  abs_b = abs_a;
  if (forward)
  {
    digitalWrite(in1,HIGH); // right
    digitalWrite(in2,LOW);  // right
    digitalWrite(in3,LOW);  // left
    digitalWrite(in4,HIGH); // left
    Serial.println("go forward!");
  } 
  else
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    Serial.println("go backwad!");
  }
  analogWrite(ENA,abs_a);
  analogWrite(ENB,abs_b);
} 
void _stop()
{
  moving = false;
  abs_a = start_speed;
  abs_b = start_speed;
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  Serial.println("STOP!");  
}
void _turn(bool left, bool sharper)
{
  forward = true;
  moving = true;
  if (abs_a == 255 || abs_b == 255)
  {
    abs_a = start_speed;
  } abs_b = start_speed;
  if (left)
    {
      if (sharper)
      {
        abs_b += 20;
      }
      else
      {
        abs_b = abs_a + 20;
      }
      Serial.println("go left!");
    }
    else
    {
      if (sharper)
      {
        abs_a += 20;
      }
      else
      {
        abs_a = abs_b + 20;
      }
      Serial.println("go right!");
    }
    digitalWrite(in1,HIGH); // right
    digitalWrite(in2,LOW);  // right
    digitalWrite(in3,LOW);  // left
    digitalWrite(in4,HIGH); // left
    analogWrite(ENA,abs_a);
    analogWrite(ENB,abs_b);
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
  _stop();
  irrecv.enableIRIn();  
  Serial.begin(9600);
}

void loop() {
  myservo.write(90);//setservo position according to scaled value
  delay(500); 
  middleDistance = Distance_test();
  #ifdef send
  Serial.print("middleDistance=");
  Serial.println(middleDistance);
  #endif

  if (forward && moving)
  {
    if(middleDistance<=20)
    {     
      _stop();
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
        _turn(false,false);
        delay(180);
      }
      else if(rightDistance<leftDistance)
      {
        _turn(true,false);
        delay(180);
      }
      else if((rightDistance<=20)||(leftDistance<=20))
      {
        _moveStraight(false,false);
        delay(180);
      }
      else
      {
        _moveStraight(true,false);
      }
    }  
    else
    {
      _moveStraight(true,false);                     
    }
  }

  if (irrecv.decode(&results)){ 
    val = results.value;
    Serial.println(val);
    irrecv.resume();
    switch(val){
      case F_KEY: 
      case UNKNOWN_F:
        if (lastKey == val) _moveStraight(true,true);
        else _moveStraight(true,false);
        break;
      case B_KEY: 
      case UNKNOWN_B: 
        if (lastKey == val) _moveStraight(false,true);
        else _moveStraight(false,false);
        break;
      case L_KEY: 
      case UNKNOWN_L: 
        if (lastKey == val) _turn(true,true);
        else _turn(true,false);
        break;
      case R_KEY: 
      case UNKNOWN_R: 
        if (lastKey == val) _turn(false,true);
        else _turn(false,false);
        break;
      case S_KEY: 
      case UNKNOWN_S: 
        _stop(); 
        break;
      default:break;
      lastKey = val;
    }
  }
} 

