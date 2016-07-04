/*
Relay
The basic Energia example.
This example code is in the public domain.
*/
// Write this code for turning on and off the components via the menu, itegrate with the LCD functionality


#define RELAY_PIN 31

// the setup routine runs once when you press reset:
void setup()
{
         pinMode(RELAY_PIN, OUTPUT); // initialize the digital pin as an output.
        //  Serial.begin();
}

// the loop routine runs over and over again forever:
void loop()
{
         digitalWrite(RELAY_PIN, HIGH); // turn the relay on (HIGH is the voltage level)
         delay(1000);   // wait for a second
         digitalWrite(RELAY_PIN, LOW);   // turn the relay o by making the voltage LOW
         delay(1000);   // wait for a second
}
