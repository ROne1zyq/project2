#include <Wire.h>
#include <Servo.h>

Servo claw;  // 创建舵机对象
Servo front;
Servo base;
Servo back;
Servo table;
#define SLAVE_ADDR 9
int LED = 13;
int rd;
void setup() {
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  claw.attach(10);  //claw
  front.attach(6);  //front
  back.attach(11);
  base.attach(5);
  table.attach(3);
  Serial.begin(9600);
  Serial.println("I2C slave Demonstration");
  table.write(35);
  front.write(30);
  back.write(135);
  claw.write(90);
  base.write(107);
}

void act_init(){
  //table.write(35);
  front.write(70);
  back.write(135);
  base.write(107);
  delay(1000);
  claw.write(45);
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
void action(){
  base_pick();
  front.write(90);
  delay(500);
  back.write(90);
  
  delay(1000);
  claw.write(0);
  delay(1000);
  front.write(30); //抬升
  back.write(135);
  delay(1000);
  back.write(145);
  delay(1000);
  base_drop();
  act_init();
  front.write(30);
  back.write(135);
  delay(2000);
  int deg=table.read();
  for(int i=deg;i<=deg+50;i++){
    table.write(i);
    delay(15);
  }
}
void receiveEvent(){
  //int c=0;
  rd=Wire.read();
  Serial.println(rd);
}
void loop() {
  if(rd==1){
    action();
    rd=0;
  }
}

