#include <Servo.h>

Servo servo_2;

int max_pos1 = 180;
int max_pos2 = 140;
int min_pos1 = 130;
int min_pos2 = 20;

int ledPin=3;
int light=6;
int fan=9;
int fanout=11;
int lightfan=10;
int PIR=13;

int pos1 = 90;
int pos2 = 0;
int waittime = 1;
int darkness = 1000;
int flag = 0;
int fadeValue=0;
int counter = 0;
unsigned long previousMillis = 0;
int nighttime = 20;
bool light_fan_off=false;
int PIR_state=0;
int last_PIR_state=0;
bool man_present=false;

int LDR1;
int LDR2;
int LDR3;
int LDR4;
int LDR5;
void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(light, INPUT);
  pinMode(fan, INPUT);
  pinMode(lightfan, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(fanout, OUTPUT);
  servo_2.attach(12);
  Serial.begin(9600);

}

void loop() {
  unsigned long currentMillis = millis();
  LDR1 = analogRead(A0);
  LDR2 = analogRead(A1);
  LDR3 = analogRead(A2);
  LDR4 = analogRead(A3);
  LDR5 = analogRead(A4);
  if (LDR1 > darkness && LDR2 > darkness && LDR3 > darkness && LDR4 > darkness ) flag = 1;
  else flag = 0;
  Serial.println(LDR1);
  Serial.println(LDR2);
  Serial.println(LDR3);
  Serial.println(LDR4);
//***************************PIR sensor********************************************
  PIR_state=digitalRead(PIR);
    if (PIR_state!= last_PIR_state) {
    if (PIR_state == HIGH) {
      man_present=!man_present;
    } 
  }
  last_PIR_state = PIR_state;
  //*******************************************************************************
  
  Serial.print("Pir state=");
  Serial.println(PIR_state);
   Serial.print("man present=");
  Serial.println(man_present);
  
  Serial.print("Light sensor value:");
  Serial.println(LDR5);
  
  //************************PIR READ and light fan off function*****************

  if((man_present==false) && (digitalRead(light)==HIGH || digitalRead(fan)==HIGH)) light_fan_off=true;
  else light_fan_off=false;

  if(light_fan_off) digitalWrite(lightfan, HIGH);
  else digitalWrite(lightfan, LOW);
   Serial.print("light fan off=");
  Serial.println(light_fan_off);
  Serial.println("-------------------------------------------------------");
  // ***********************light function**********************************
  if(digitalRead(light)==HIGH){
  if(LDR5>700){
    fadeValue++;
    analogWrite(ledPin, fadeValue);
  }
  else if(LDR5<400){
    fadeValue--;
    analogWrite(ledPin, fadeValue);
  }
  }
  else analogWrite(ledPin, 0);
  if(fadeValue>=255) fadeValue=254;
  if(fadeValue<=0) fadeValue=1;
  delay(30);  
  Serial.print("fade value:");
  Serial.println(fadeValue);


//****************************fan function********************************

if(digitalRead(fan)==HIGH)
  digitalWrite(fanout, HIGH);
else digitalWrite(fanout, LOW);
  
  if (!flag) {
    if (LDR1 > LDR2) {
      pos1 = pos1 + 1;
      servo_1anticlock();
      delay(waittime);
    }
    if (LDR4 > LDR3) {
      pos1 = pos1 + 1;
      servo_1anticlock();
      delay(waittime);
    }
    if (LDR1 < LDR2) {
      pos1 = pos1 - 1;
      servo_1clock();
      delay(waittime);
    }
    if (LDR4 < LDR3) {
      pos1 = pos1 - 1;
      servo_1clock();
      delay(waittime);
    }

    if (pos1 > max_pos1) {
      pos1 = max_pos1;
    }
    if (pos1 < min_pos1) {
      pos1 = min_pos1;
    }

    if (LDR1 > LDR4) {
      pos2 = pos2 + 1;
      servo_2.write(pos2);
      delay(waittime);
    }
    if (LDR2 > LDR3) {
      pos2 = pos2 + 1;
      servo_2.write(pos2);
      delay(waittime);
    }
    if (LDR1 < LDR4) {
      pos2 = pos2 - 1;
      servo_2.write(pos2);
      delay(waittime);
    }
    if (LDR2 < LDR3) {
      pos2 = pos2 - 1;
      servo_2.write(pos2);
      delay(waittime);
    }
    if (pos2 > max_pos2) {
      pos2 = max_pos2;
    }
    if (pos2 < min_pos2) {
      pos2 = min_pos2;
    }

    counter = 0;
  }
  else if (flag) {
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      counter += 1;
      Serial.print("counter=");
      Serial.println(counter);
    }

    if (counter > nighttime) {
      Serial.print("pos2=");
      Serial.println(pos2);
      if (pos2 > 90) {
        Serial.print("pos21=");
        Serial.println(pos2);
        while (pos2 != 90) {
          pos2--;
          servo_2.write(pos2);
          delay(20);
          Serial.println("****************************************************");
        }
      }
      else if (pos2 < 90) {
        Serial.print("pos22=");
        Serial.println(pos2);
        while (pos2 != 90) {
          pos2++;
          servo_2.write(pos2);
          delay(20);
          Serial.println("****************************************************");
        }
      }

    }
  }
   //delay(1000);
}

void servo_1clock() {
  digitalWrite(7, LOW);   //rotates servo 1 CLOCKWISE
  digitalWrite(8, HIGH);
  delay(50);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void servo_1anticlock() {
  digitalWrite(7, HIGH);   //rotates servo 1 ANTI-CLOCKWISE
  digitalWrite(8, LOW);
  delay(50);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}
