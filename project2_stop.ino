#include <math.h>
#include <Servo.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#define M_RH1 8
#define M_RH2 9
#define M_LH1 6
#define M_LH2 7
#define M_RF1 2
#define M_RF2 3
#define M_LF1 5
#define M_LF2 4
#define OT1 A0 //left
#define OT2 A1 //right
#define SLAVE_ADDR 9

Servo claw;  // 创建舵机对象
Servo front;
Servo base;
Servo back;
Servo table;

int RIGHT=1,LEFT=-1,MIDDLE=0;
int IDnum=0;
const int v_forward=70;
const int v_high=70;
const int v_low=60;
const int v_forwardfast=120;

const int v_front=150;
const int v_back=110;
double num1,num2;
HUSKYLENS huskylens;
SoftwareSerial mySerial(12, 13); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result){
  if (result.command == COMMAND_RETURN_BLOCK){
      // Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
      // Serial.println(String()+F(",ID=")+result.ID);
      Serial.println(result.ID);
  // else if (result.command == COMMAND_RETURN_ARROW){
  //     Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
  }
  else{
      Serial.println("Object unknown!");
  }
}
/*----setup----*/
void setup()
{
  pinMode(M_RH1, OUTPUT);
  pinMode(M_RH2, OUTPUT);
  pinMode(M_LH1, OUTPUT);
  pinMode(M_LH2, OUTPUT);
  pinMode(M_RF1, OUTPUT);
  pinMode(M_RF2, OUTPUT);
  pinMode(M_LF1, OUTPUT);
  pinMode(M_LF2, OUTPUT);
  
  Wire.begin();
  //table.attach(A3);
  //claw.attach(A7);  //claw
  //front.attach(A5);  //front
  //back.attach(A6);
  //base.attach(11);  //base

  Serial.begin(115200);
  mySerial.begin(9600);
  while (!huskylens.begin(mySerial))
  {
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }
}
/*----move function----*/
void move_forward(){
  analogWrite(M_RF1,0);
  analogWrite(M_RF2,v_forward);
  analogWrite(M_RH1,0);
  analogWrite(M_RH2,v_forward);
  analogWrite(M_LF1,0);
  analogWrite(M_LF2,v_forward);
  analogWrite(M_LH1,0);
  analogWrite(M_LH2,v_forward);
}
void forward_fast(){
  analogWrite(M_RF1,0);
  analogWrite(M_RF2,v_forwardfast);
  analogWrite(M_RH1,0);
  analogWrite(M_RH2,v_forwardfast);
  analogWrite(M_LF1,0);
  analogWrite(M_LF2,v_forwardfast);
  analogWrite(M_LH1,0);
  analogWrite(M_LH2,v_forwardfast);
}
void stop(){
  analogWrite(M_RF1,0);
  analogWrite(M_RF2,0);
  analogWrite(M_RH1,0);
  analogWrite(M_RH2,0);
  analogWrite(M_LF1,0);
  analogWrite(M_LF2,0);
  analogWrite(M_LH1,0);
  analogWrite(M_LH2,0);
}
void right(){
  analogWrite(M_RF1,v_front);
  analogWrite(M_RF2,0);
  analogWrite(M_RH1,0);
  analogWrite(M_RH2,v_back);
  analogWrite(M_LF1,0);
  analogWrite(M_LF2,v_front);
  analogWrite(M_LH1,v_back);
  analogWrite(M_LH2,0);
}
void turn_normal_angle(int direction){
  if (direction == LEFT){
    analogWrite(M_RF1,0);
    analogWrite(M_RF2,v_high);
    analogWrite(M_RH1,0);
    analogWrite(M_RH2,v_high);
    analogWrite(M_LF1,0);
    analogWrite(M_LF2,v_low);
    analogWrite(M_LH1,0);
    analogWrite(M_LH2,v_low);
  }
  else if (direction == RIGHT){
    analogWrite(M_RF1,0);
    analogWrite(M_RF2,v_low);
    analogWrite(M_RH1,0);
    analogWrite(M_RH2,v_low);
    analogWrite(M_LF1,0);
    analogWrite(M_LF2,v_high);
    analogWrite(M_LH1,0);
    analogWrite(M_LH2,v_high);
  }
}
int juduge_the_direction(){
  num1=analogRead(OT1); //left
  num2=analogRead(OT2); //right
  //black=154; white=735
  if (num1>512)  return RIGHT; //左侧越出
  else if (num2>512)  return LEFT; //右侧越出
  else  return MIDDLE;
}

void linetrack(){
  int flag=juduge_the_direction();
  if (flag == LEFT){
    turn_normal_angle(LEFT);
    delay(10);
  }
  else if (flag == RIGHT){
    turn_normal_angle(RIGHT);
    delay(10);
  }else{
    move_forward();
    delay(10);
  }
}
/*----servo function----*/
void act_init(){
  front.write(70);
  back.write(135);
  base.write(107);
  delay(1000);
  claw.write(60);
  delay(1000);
}
void base_pick(){
  for(int i=107;i>=7;i--){
    base.write(i);
    delay(15);
  }
  delay(1000);
}
void base_drop(){
  for(int j=7;j<=107;j++){
    base.write(j);
    delay(20);
  }
  delay(1000);
}
void pick_process(){
  stop();
  act_init();
  front.write(30);
  back.write(135);
  delay(1000);
  base_pick();
  front.write(90);
  delay(500);
  back.write(90);
  
  delay(1000);
  claw.write(0);
  delay(1000);
  front.write(30);
  back.write(135);
  delay(1000);
  back.write(145);
  delay(1000);
  base_drop();
  act_init();
}

/*----loop----*/
void loop() 
{
  HUSKYLENSResult result = huskylens.read();
  if(huskylens.request())
  {
    HUSKYLENSResult result = huskylens.read();
    Serial.println(result.ID);
    Serial.println();
    if(result.ID==5||result.ID==9){
      stop();
      //delay(100);
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(1);
      Wire.endTransmission();
      delay(15000);
      forward_fast();
      delay(150);
      IDnum++;
    }
    else{
      linetrack();
    }
  }
  else{
    linetrack();
  }
  if(IDnum==2){
    right();
    delay(1500);
    move_forward();
    delay(3000);
    stop();
    IDnum=0;
    delay(10000);
  }
}
