
#include <Wire.h>              //including libraries of I2C
#include <IRremote.h>          //including libraries of remote control
#define RECV_PIN  12        //pin 12 of IR remoter control receiver
#include <Servo.h>
IRrecv irrecv(RECV_PIN);      //defining pin 12 of IR remoter control
Servo myservo;
decode_results res;
decode_results results;         //cache of decode of IR remoter control
#define IR_Go       0x00ff629d //going forward
#define IR_Back     0x00ffa857  //going backward
#define IR_Left     0x00ff22dd  //turning left
#define IR_Right    0x00ffc23d  //turning right
#define IR_Stop     0x00ff02fd  //stop 
#define IR_Servo_L  0x00ff6897  //motor turning left
#define IR_Servo_R  0x00ff9867  //motor turning right
#define IR_Speed_UP     0x00ffb04f //increasing speed
#define IR_Speed_DOWN   0x00ff30cf //decreasing speed
#define IR_XunJi_Mode   0x00ff18e7
#define IR_Self_Control  0x00ff7a85  //ultrasonic distance detecting 
#define echoPin A0 // attach pin D2 Arduino to pin Echo of HC-SR04 back
#define trigPin A1 //attach pin D3 Arduino to pin Trig of HC-SR04 back
#define echoPin1 A2 // attach pin D2 Arduino to pin Echo of HC-SR04 left
#define trigPin1 A3 //attach pin D3 Arduino to pin Trig of HC-SR04 left
#define echoPin2 A4 // attach pin D2 Arduino to pin Echo of HC-SR04 right
#define trigPin2 A5 //attach pin D3 Arduino to pin Trig of HC-SR04 right
//////////////////////////////////////////////////
#define Lpwm_pin  5     //pin of controlling speed---- ENA of motor driver board
#define Rpwm_pin  6    //pin of controlling speed---- ENB of motor driver board
int pinLB=2;             //pin of controlling turning---- IN1 of motor driver board
int pinLF=4;             //pin of controlling turning---- IN2 of motor driver board
int pinRB=7;            //pin of controlling turning---- IN3 of motor driver board
int pinRF=8;            //pin of controlling turning---- IN4 of motor driver board
int flag=0;
unsigned char Lpwm_val = 140; //initialized left wheel speed at 100
unsigned char Rpwm_val = 140; //initialized right wheel speed at 100
int Car_state=0;             //the working state of car
int myangle;                //defining variable of angle
int pulsewidth;              //defining variable of pulse width
unsigned char DuoJiao=90;    //initialized angle of motor at 90Â°
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int back_sensor, right_sensor, left_sensor;

void M_Control_IO_config(void)
{
  pinMode(pinLB,OUTPUT); // /pin 2
  pinMode(pinLF,OUTPUT); // pin 4
  pinMode(pinRB,OUTPUT); // pin 7
  pinMode(pinRF,OUTPUT);  // pin 8
  pinMode(Lpwm_pin,OUTPUT);  // pin 5 (PWM) 
  pinMode(Rpwm_pin,OUTPUT);  // pin6(PWM)   
}
void Set_Speed(unsigned char Left,unsigned char Right) //function of setting speed
{
  analogWrite(Lpwm_pin,Left);
  analogWrite(Rpwm_pin,Right);
}
void advance()    //  going forward
    {
     digitalWrite(pinRB,LOW);  // making motor move towards right rear
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  // making motor move towards left rear
     digitalWrite(pinLF,HIGH); 
     Car_state = 1; 
       
    }
void turnR()        //turning right(dual wheel)
    {
     digitalWrite(pinRB,LOW);  //making motor move towards right rear
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,LOW);  //making motor move towards left front
     Car_state = 4;
    
    }
void turnL()         //turning left(dual wheel)
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,LOW );   //making motor move towards right front
     digitalWrite(pinLB,LOW);   //making motor move towards left rear
     digitalWrite(pinLF,HIGH);
     Car_state = 3;
     
    }    
void stopp()        //stop
    {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
     Car_state = 5;
    
    }
void back()         //back up
    {
     digitalWrite(pinRB,HIGH);  //making motor move towards right rear     
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);  //making motor move towards left rear
     digitalWrite(pinLF,LOW);
     Car_state = 2;
         
    }
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin1, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
   M_Control_IO_config();     //motor controlling the initialization of IO
   Set_Speed(Lpwm_val,Rpwm_val);  //setting initialized speed
   //pinMode(outputPin, OUTPUT);    //IO of ultrasonic module 
    myservo.write(DuoJiao);
   stopp();                       //stop
   delay(1000);
   myservo.detach();
}
void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration * 0.034 / 2;
} 
void loop() 
{
SonarSensor(trigPin, echoPin);
back_sensor = distance;
SonarSensor(trigPin1, echoPin1);
left_sensor = distance;
SonarSensor(trigPin2, echoPin2);
right_sensor = distance;
//Serial.print(back_sensor);
//Serial.print(" - ");
//Serial.print(left_sensor);
//Serial.print(" - ");
//Serial.println(right_sensor);

  if ( back_sensor > 20 ){
    back();
    Serial.print("backing up");
    delay(20);
  }
  else if (right_sensor < 20){
    stopp();
    Serial.print("stopping");
    delay(2);
    turnL();
    Serial.print("turning left ");    
    delay(20);
    stopp();
    Serial.print("stopping");
  }
  else if (left_sensor < 20){
    stopp();
    Serial.print("stopping");
    delay(2);
    turnR();
    Serial.print("turning left "); 
    delay (20);
    stopp();
  }
   if (back_sensor < 20)         
   {stopp();
    Serial.print("stopping ");
    delay(20);             
       }
}
