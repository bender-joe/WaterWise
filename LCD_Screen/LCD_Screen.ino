#include "DHT.h"
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A2
LiquidCrystal lcd(8,9,4,5,6,7); 
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

void setup(void) {
  lcd.begin(16, 2);
  lcd.print("Reading sensor...");
  sensors.begin();
}

void loop() {  
  sensors.requestTemperatures();
  delay(2000); 

 lcd.clear();


 lcd.print("Temp:"); 
 lcd.print(sensors.getTempCByIndex(0));
 //lcd.print((char)223);
 lcd.print("C ");
 //lcd.print("F");
 lcd.print(sensors.getTempFByIndex(0));
 lcd.print("F ");
 //lcd.print("%");
}
