void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  long result = (long)(1.9*1.232*15/0.77*1000);
  Serial.println("Result was:");
  Serial.println(result);
}

void loop() {
  // put your main code here, to run repeatedly:

}
