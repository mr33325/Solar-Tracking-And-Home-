void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT); //CONNECT BLACK WIRE
  pinMode(8, OUTPUT); //CONNECT BROWN WIRE

}

void loop() {
  // put your main code here, to run repeatedly
  digitalWrite(7,LOW);    //CLOCKWISE
  digitalWrite(8,HIGH);
  delay(2000);
  digitalWrite(7,HIGH);   //ANTI CLOCKWISE
  digitalWrite(8,LOW);
  delay(2000);
}
