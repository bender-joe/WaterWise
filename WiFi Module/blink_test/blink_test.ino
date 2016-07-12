// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.

  int i =0;
  for(i = 0; i < 54; i ++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  pinMode(80,OUTPUT);
  digitalWrite(80,LOW);
  
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(80,HIGH);
  digitalWrite(30,HIGH);
  digitalWrite(31,HIGH);
  digitalWrite(32,HIGH);
  digitalWrite(33,HIGH);
  digitalWrite(34,HIGH);
  digitalWrite(35,HIGH);
  digitalWrite(36,HIGH);
  digitalWrite(37,HIGH);
  digitalWrite(25,HIGH);
  digitalWrite(24,HIGH);
  digitalWrite(23,HIGH);
  digitalWrite(18,HIGH);
  digitalWrite(17,HIGH);
  digitalWrite(16,HIGH);
  digitalWrite(15,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);
  delay(5000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(80,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  digitalWrite(10,LOW);
  digitalWrite(9,LOW);
  digitalWrite(8,LOW);
  digitalWrite(7,LOW);
  digitalWrite(6,LOW);
  digitalWrite(30,LOW);
  digitalWrite(31,LOW);
  digitalWrite(32,LOW);
  digitalWrite(33,LOW);
  digitalWrite(34,LOW);
  digitalWrite(35,LOW);
  digitalWrite(36,LOW);
  digitalWrite(37,LOW);
  digitalWrite(25,LOW);
  digitalWrite(24,LOW);
  digitalWrite(23,LOW);
  digitalWrite(18,LOW);
  digitalWrite(17,LOW);
  digitalWrite(16,LOW);
  digitalWrite(15,LOW);
  delay(5000);              // wait for a second
}
