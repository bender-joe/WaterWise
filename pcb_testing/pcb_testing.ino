void setup() {
  // write all the pins to low to avoid bad signals
  int i;
  for (i = 0; i < 54; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }



}

void loop()
{
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  // digitalWrite(10, HIGH);
  // digitalWrite(9, HIGH);
  // digitalWrite(8, HIGH);
  // digitalWrite(7, HIGH);
  // digitalWrite(6, HIGH);

  // digitalWrite(30, HIGH);
  // digitalWrite(31, HIGH);
  // digitalWrite(32, HIGH);
  // digitalWrite(33, HIGH);
  // digitalWrite(34, HIGH);
  // digitalWrite(35, HIGH);
  // digitalWrite(36, HIGH);
  // digitalWrite(37, HIGH);
  //
  // digitalWrite(44, HIGH);

  delay(5000);

  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  // digitalWrite(10, LOW);
  // digitalWrite(9, LOW);
  // digitalWrite(8, LOW);
  // digitalWrite(7, LOW);
  // digitalWrite(6, LOW);

  // digitalWrite(30, LOW);
  // digitalWrite(31, LOW);
  // digitalWrite(32, LOW);
  // digitalWrite(33, LOW);
  // digitalWrite(34, LOW);
  // digitalWrite(35, LOW);
  // digitalWrite(36, LOW);
  // digitalWrite(37, LOW);
  //
  // digitalWrite(44, LOW);

  delay(5000);

}
