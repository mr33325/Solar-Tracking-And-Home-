#include <Servo.h>

Servo servo_2;

int max_pos1=180;
int max_pos2=140;
int min_pos1=130;
int min_pos2=20;

int pos1=90;
int pos2=90;
int waittime=1;
int callibration=550;

 int LDR1;
 int LDR2;
 int LDR3;
 int LDR4;
  
void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
servo_2.attach(12);
Serial.begin(9600);
servo_2.write(pos2);
delay(500);

}

void loop() {
  LDR1=analogRead(A0);
  LDR2=analogRead(A1);
  LDR3=analogRead(A2);
  LDR4=analogRead(A3);
  
  Serial.println(LDR1);
Serial.println(LDR2);
Serial.println(LDR3);
Serial.println(LDR4);
Serial.println("-------------------------------------------------------");


  if (LDR1 > LDR2) {
   pos1= pos1+1;
   servo_1anticlock();
    delay(waittime);
  }
  if (LDR4 >LDR3) {
     pos1= pos1+1;
    servo_1anticlock();
    delay(waittime);
  }
  if (LDR1 < LDR2) {
     pos1= pos1-1;
    servo_1clock();
    delay(waittime);
  }
  if (LDR4 <LDR3) {
     pos1= pos1-1;
    servo_1clock();
    delay(waittime);
  }
 
 if (pos1 > max_pos1) {
   pos1= max_pos1;
  }
  if (pos1 < min_pos1) {
    pos1= min_pos1;
  }
  
  if (LDR1 > LDR4) {
    pos2=pos2+1;
    servo_2.write(pos2);
    delay(waittime);
  }
  if (LDR2 >LDR3) {
     pos2=pos2+1;
    servo_2.write(pos2);
    delay(waittime);
  }
  if (LDR1 < LDR4) {
    pos2=pos2-1;
   servo_2.write(pos2);
    delay(waittime);
  }
  if (LDR2 <LDR3) {
     pos2=pos2-1;
    servo_2.write(pos2);
    delay(waittime);
  }
 if (pos2 > max_pos2) {
    pos2=max_pos2;
  }
  if (pos2 < min_pos2) {
    pos2=min_pos2;
  }
 
//delay(1000);
}

void servo_1clock(){
  digitalWrite(7,LOW);    //rotates servo 1 CLOCKWISE
  digitalWrite(8,HIGH);
  delay(20);
  digitalWrite(7,LOW); 
  digitalWrite(8,LOW);
}

void servo_1anticlock(){
  digitalWrite(7,HIGH);    //rotates servo 1 ANTI-CLOCKWISE
  digitalWrite(8,LOW);
  delay(20);
  digitalWrite(7,LOW); 
  digitalWrite(8,LOW);
}
