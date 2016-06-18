/*
Relay
The basic Energia example.
This example code is in the public domain.
*/
//NOTE DARLINGTON DRIVER NEEDS TO CONNECTED VIN ---> PUMP <---- OUTPUT

#define PUMP_PIN 39
#define PUMPONS  5
int pumpOnms = (1000*PUMPONS);

// the setup routine runs once when you press reset:
void setup() {
         pinMode(PUMP_PIN, OUTPUT); // initialize the digital pin as an output.
}

// the loop routine runs over and over again forever:
void loop() {
         digitalWrite(PUMP_PIN, HIGH); // turn the relay on (HIGH is the voltage level)
         delay(pumpOnms);   // wait for a second
         digitalWrite(PUMP_PIN, LOW);   // turn the relay o by making the voltage LOW
         delay(pumpOnms);   // wait for a second
}
