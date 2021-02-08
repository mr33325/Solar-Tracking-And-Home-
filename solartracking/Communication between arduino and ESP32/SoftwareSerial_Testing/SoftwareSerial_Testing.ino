
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX
int i=0;
int j=0;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
//  if (mySerial.available()) {
//    Serial.write(mySerial.read());
//  }
//  if (Serial.available()) {
//    mySerial.write(Serial.read());
//  }
  //  int i=10;
  //  while(i){
  mySerial.write('T');
  mySerial.write(i);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);
  i++;
  mySerial.write('H');
  mySerial.write(j);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);
  j++;
  delay(10000);
}
