byte sensorPin[] = {31, 33, 35};

byte ledPin[] = {11, 12, 13}; // number of leds = numbers of sensors

const byte sensors = 3;

int level = 0;

void setup()

{

Serial.begin(115200);

for(int i = 0; i < sensors; i++)

{

pinMode(sensorPin[i], INPUT);

pinMode(ledPin[i], OUTPUT);

}

}

void loop()
{
  level = 0;
  for(int i = 0; i < sensors; i++)
  {
    if(digitalRead(sensorPin[i]) == LOW)
    {
      digitalWrite(ledPin[i], HIGH);
      level = sensors - i;
    }
    else
    {
      digitalWrite(ledPin[i], LOW);
    }
  }
  Serial.println("Water level");

  switch(level)
  {
    case 1:
      Serial.println("HIGH");
      break;
    case 2:
      Serial.println("AVERAGE");
      break;
    case 3:
      Serial.println("LOW");
      break;
    default:
      Serial.println("NO WATER");
      break;
  }
  delay(50);
}
