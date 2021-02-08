#define RXD2 16
#define TXD2 17

void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  pinMode(2,OUTPUT);
}

void loop() { //Choose Serial1 or Serial2 as required
    if (Serial2.available()) {
    char c=Serial2.read();
    if(c=='T'){
      int i=Serial2.read();
    Serial.println("Temperature:"+String(i));
    digitalWrite(2,HIGH);
    delay(50);
    digitalWrite(2,LOW);
    }
    else if(c=='H'){
      int j=Serial2.read();
    Serial.println("Humidity:"+String(j));
    digitalWrite(2,HIGH);
    delay(50);
    digitalWrite(2,LOW);
    }
  }
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }
delay(500);
}
