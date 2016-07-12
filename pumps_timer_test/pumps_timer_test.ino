#include <SimpleTimer.h>

SimpleTimer timer;
static int runningPump = 0;
static bool PumpOn = false;

void stopPump()
{
    digitalWrite(runningPump, LOW);
    Serial.println("Pump stopped");
}

void runPump(int pump, unsigned long durationMS)
{

  Serial.print("Running the pump for: ");
  Serial.print(durationMS/1000);
  Serial.println(" seconds");
  unsigned long prev = 0;
  unsigned long current = millis();
  runningPump = pump;
  digitalWrite(pump, HIGH);
  PumpOn = true;
  timer.setTimeout(durationMS, stopPump );
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Begin testing runPump and stop pump");
  runPump(22, 5000);
}

void loop() {
  if(runPump)
  {
    timer.run();
  }


}
