
// These constants won't change:
const int sensorPin = A5;    // pin that the sensor is attached to
const int ledPin = 9;        // pin that the LED is attached to

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value=0
int sensorMax = 0;           // maximum sensor value=670
int servo_pos=90;
int tol=2;
int flag=1;
void setup() {
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(13, HIGH);

  // calibrate during the first five seconds
  while (millis() < 5000) {
    sensorValue = analogRead(sensorPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
  }

  // signal the end of the calibration period
  digitalWrite(13, LOW);
  Serial.begin(9600);
  Serial.println(sensorMin);
  Serial.println(sensorMax);
}

void loop() {
 sensorValue=sensorRead();
 int err=abs(servo_pos-sensorValue);
 if(err<tol) flag=0;
  if((servo_pos-sensorValue)>0 && err>tol && flag){
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
    delay(50);
  }
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  delay(500);
  sensorValue=sensorRead();
  err=abs(servo_pos-sensorValue);
  if(err<tol) flag=0;
  if((servo_pos-sensorValue)<0 && err>tol && flag){
    digitalWrite(6,HIGH);
    digitalWrite(5,LOW);
    delay(50);
  }
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  delay(500);
  
}
int sensorRead(){
   // read the sensor:
  int s = analogRead(sensorPin);
  delay(10);
  // apply the calibration to the sensor reading
  s = map(s, 0, 670, 0, 180);
  // in case the sensor value is outside the range seen during calibration
  s = constrain(s, 0, 180);
  Serial.println(s);
  return s;
}
