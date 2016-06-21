/*************************************************************************************

  Joseph Bender (2016)
  Senior Design Project
  WaterWise atmega2560 code
  Dev Items:
    - LCD Menu          - Complete
    - Wifi Comms        - TODO
    - Sensor Pull Data  - COMPLETE
    - Relay Signaling   - COMPLETE
    - P_Pumps Signaling - TODO

**************************************************************************************/
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include "DHT.h"
#include <SimpleTimer.h>

// PH CONSTANTS
#define SensorPin A5            //pH meter Analog output to Arduino Analog Input 0
#define pHOffset 0.08            //deviation compensate updated to compensate
#define LED 13
#define samplingInterval 20
#define ArrayLenth  40    //times of collection
// EC CONSTANTS
#define StartConvert 0
#define ReadTemperature 1
// WATER LEVEL CONSTANTS
#define wlHigh    31
#define wlMed     33
#define wlLow     35
// LCD CONSTANTS
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
// MENU DISPLAY STATUS CONSTANTS
#define LCDSLEEP             98
#define LCDAWAKE             99
#define mainMenu            100
#define powerAll            101
#define powerMainPump       102
#define powerAirStone       103
#define powerLight          104
#define sensorEC            105
#define sensorPH            106
#define sensorWTemp         107
#define sensorATemp         108
#define sensorHumid         109
#define sensorWLevel        110
#define mainMenu2           111
#define menuScrollingSpeed  25
#define DHTPIN 3     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
// RELAY PINS
#define LIGHTPIN    20
#define PUMPPIN     21
#define AIRSTONEPIN 22
#define TOGGLEALL   23
// P_PUMPPINS
#define PH_UP     40
#define PH_DOWN   42
#define NUTRIENT  44

//DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

// LCD Globals
int lcd_key     = 0;
int adc_key_in  = 0;
int currentMenu = 0;
int lcdStatus   = 0;

// PH GLOBALS
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
float pHValue = 0.0;

// EC GLOBAL
const float ECfactor = 0.0;
const byte numReadings = 20;     //the number of sample times
byte ECsensorPin = A3;  //EC Meter analog output,pin on analog 1
byte DS18B20_Pin = 2; //DS18B20 signal, pin on digital 2
unsigned int AnalogSampleInterval=25,printInterval=700,tempSampleInterval=850;  //analog sample interval;serial print interval;temperature sample interval
unsigned int readings[numReadings];      // the readings from the analog input
byte index = 0;                  // the index of the current reading
unsigned long AnalogValueTotal = 0;                  // the running total
unsigned int AnalogAverage = 0,averageVoltage=0;                // the average
unsigned long AnalogSampleTime,printTime,tempSampleTime;
float temperature,ECcurrent;

// AIR MEASUREMENT GLOBALS
unsigned int airTempSampleInterval = 2000;
float airTemp = 0.0;
float humidity = 0.0;

// WATER LEVEL GLOBALS
byte wlSensorPins[] = {31, 33, 35}; // {low, med, high}
int waterLevel = 0;
String waterLevelStr;

SimpleTimer timer;
int runningPump = 0;


//Temperature chip i/o
OneWire ds(DS18B20_Pin);  // on digital pin 2

/*
    LCD Code
*/
// process putton input from the LCD Buttons
int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE;
   // For V1.0 comment the other threshold and use the one below:

     if (adc_key_in < 50)   return btnRIGHT;
     if (adc_key_in < 195)  return btnUP;
     if (adc_key_in < 380)  return btnDOWN;
     if (adc_key_in < 555)  return btnLEFT;
     if (adc_key_in < 790)  return btnSELECT;

    return btnNONE;                // when all others fail, return this.
}

// scroll right to the indicated menu
// Param: menu : the menu that is to be scrolled righ to display
void displayMenuR(int menu)
{
  // scroll the previous text left off of the screen
  for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(menuScrollingSpeed);
  }
  // clear the current screen
  // need to scroll in the text
  lcd.clear();
  //which menu to print
  switch(menu)
  {
    case mainMenu:
      lcd.print("Power");               // print a simple message on the LCD
      lcd.setCursor(0,1);               // set the LCD cursor   position
      lcd.print("Sensors");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case mainMenu2:
      lcd.print("System Sleep");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case powerAll:
      lcd.print("All On");
      lcd.setCursor(0,1);
      lcd.print("All Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case powerMainPump:
      lcd.print("Main Pump On");
      lcd.setCursor(0,1);
      lcd.print("Main Pump Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case powerAirStone:
      lcd.print("Air Stone On");
      lcd.setCursor(0,1);
      lcd.print("Air Stone Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case powerLight:
      lcd.print("Lighting On");
      lcd.setCursor(0,1);
      lcd.print("Lighting Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorPH:
      lcd.print("pH Sensor");
      lcd.setCursor(0,1);
      lcd.print(pHValue, 2);
      lcd.print(" HH:MM:SS");    // Get latest ph reading here
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorEC:
      lcd.print("EC Sensor ms/cm");
      lcd.setCursor(0,1);
      lcd.print(ECcurrent, 2);
      lcd.print(" HH:MM:SS");    // Get latest ph reading here
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorWTemp:
      lcd.print("Water Temp Cel");
      lcd.setCursor(0,1);
      lcd.print(temperature, 2);
      lcd.print(" HH:MM:SS");    // get latest water temp here
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorWLevel:
      lcd.print("Water Level HML");
      lcd.setCursor(0,1);
      lcd.print(waterLevelStr);
      lcd.print(" HH:MM:SS");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorATemp:
      lcd.print("Air Temp Faren");
      lcd.setCursor(0,1);
      lcd.print(airTemp, 2);
      lcd.print(" HH:MM:SS");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorHumid:
      lcd.print("Air Humidity %");
      lcd.setCursor(0,1);
      lcd.print(humidity, 2);
      lcd.print(" HH:MM:SS");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(menuScrollingSpeed);
      }
      break;

    default: break;
  }
}

// scroll left to the indicated menu
// Param: menu : the menu that is to be scrolled righ to display
void displayMenuL(int menu)
{
  // scroll the current screen to the Right
  for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
    lcd.scrollDisplayRight();
    delay(menuScrollingSpeed);
  }
  // need to scroll in the text left
  lcd.clear();
  //which menu to print
  switch(menu)
  {
    case mainMenu:
      lcd.print("Power");               // print a simple message on the LCD
      lcd.setCursor(0,1);               // set the LCD cursor   position
      lcd.print("Sensors");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

    case mainMenu2:
      lcd.print("System Sleep");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

    case powerAll:
      lcd.print("All On");
      lcd.setCursor(0,1);
      lcd.print("All Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

    case powerMainPump:
      lcd.print("Main Pump On");
      lcd.setCursor(0,1);
      lcd.print("Main Pump Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

    case powerAirStone:
      lcd.print("Air Stone On");
      lcd.setCursor(0,1);
      lcd.print("Air Stone Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

    case powerLight:
      lcd.print("Lighting On");
      lcd.setCursor(0,1);
      lcd.print("Lighting Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

    case sensorPH:
      lcd.print("pH Sensor");
      lcd.setCursor(0,1);
      lcd.print(pHValue, 2);
      lcd.print(" HH:MM:SS");    // Get latest ph reading here
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(menuScrollingSpeed);
      }
      break;

      case sensorEC:
        lcd.print("EC Sensor ms/cm");
        lcd.setCursor(0,1);
        lcd.print(ECcurrent,2);
        lcd.print(" HH:MM:SS");    // get latest ec reading here
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(menuScrollingSpeed);
        }
        break;

      case sensorWTemp:
        lcd.print("Water Temp Cel");
        lcd.setCursor(0,1);
        lcd.print(temperature, 2);
        lcd.print(" HH:MM:SS");    // get latest water temp here
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(menuScrollingSpeed);
        }
        break;

      case sensorWLevel:
        lcd.print("Water Level HML");
        lcd.setCursor(0,1);
        lcd.print(waterLevelStr);
        lcd.print(" HH:MM:SS");
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(menuScrollingSpeed);
        }
        break;

      case sensorATemp:
        lcd.print("Air Temp Faren");
        lcd.setCursor(0,1);
        lcd.print(airTemp, 2);
        lcd.print(" HH:MM:SS");
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(menuScrollingSpeed);
        }
      break;

      case sensorHumid:
        lcd.print("Air Humidity %");
        lcd.setCursor(0,1);
        lcd.print(humidity, 2);
        lcd.print(" HH:MM:SS");
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(menuScrollingSpeed);
        }
        break;

    default: break;
  }
}

// display the menu normally
// Param: menu : the menu that is to be scrolled righ to display
void displayMenu(int menu)
{
  // clear the current screen
  lcd.clear();
  //which menu to print
  switch(menu){
    case mainMenu:
      lcd.print("Power");               // print a simple message on the LCD
      lcd.setCursor(0,1);               // set the LCD cursor   position
      lcd.print("Sensors");
      break;
    case powerAll:
      lcd.print("All On");
      lcd.setCursor(0,1);
      lcd.print("All Off");
      break;
    case powerMainPump:
      lcd.print("Main Pump On");
      lcd.setCursor(0,1);
      lcd.print("Main Pump Off");
      break;
    case powerAirStone:
      lcd.print("Air Stone On");
      lcd.setCursor(0,1);
      lcd.print("Air Stone Off");
      break;
    case powerLight:
      lcd.print("Lighting On");
      lcd.setCursor(0,1);
      lcd.print("Lighting Off");
      break;
    case sensorPH:
      lcd.print("pH Sensor");
      lcd.setCursor(0,1);
      lcd.print(pHValue, 2);
      lcd.print(" HH:MM:SS");    // Get latest ph reading here
      break;
    case sensorEC:
      lcd.print("EC Sensor ms/cm");
      lcd.setCursor(0,1);
      lcd.print(ECcurrent, 2);
      lcd.print(" HH:MM:SS");    // get latest ec reading here
      break;
    case sensorWTemp:
      lcd.print("Water Temp Faren");
      lcd.setCursor(0,1);
      lcd.print(temperature, 2);
      lcd.print(" HH:MM:SS");    // get latest water temp here
      break;
    case sensorWLevel:
      lcd.print("Water Level HML");
      lcd.setCursor(0,1);
      lcd.print(waterLevelStr);
      lcd.print(" HH:MM:SS");
      break;
    case sensorATemp:
      lcd.print("Air Temp Faren");
      lcd.setCursor(0,1);
      lcd.print(airTemp, 2);
      lcd.print(" HH:MM:SS");
      break;
    case sensorHumid:
      lcd.print("Air Humidity %");
      lcd.setCursor(0,1);
      lcd.print(humidity, 2);
      lcd.print(" HH:MM:SS");
      break;
    default: break;
  }
}

// handles the input of the button presses
// detemines what to do for each button press
void processDisplay()
{
    lcd_key = read_LCD_buttons();
    switch(lcd_key)
    {
        // Process Up Button pushes
        case btnUP:                     // power select - go to power menu
          switch(currentMenu)           // for Button UP check which menu is at
          {
            case mainMenu:              // @ main, goto powerAll
              currentMenu = powerAll;   // set the curren menu status
              displayMenu(currentMenu); // display power all
              break;
            case mainMenu2:
              // System Sleep option, turn off the LCD
              /*lcd.noDisplay();*/
              break;
            case powerAll:              // @ power all menu
              toggleRelayComponent(TOGGLEALL, 1);
              break;                    // power on all relay
            case powerMainPump:         // @ power main pump menu
              toggleRelayComponent(PUMPPIN, 1);
              break;                    // turn on main pump relay
            case powerAirStone:         // @ power air stone menu
              toggleRelayComponent(AIRSTONEPIN, 1);
              break;                    // turn on air stone relay
            case powerLight:            // @ power light menu
              toggleRelayComponent(LIGHTPIN, 1);
              break;                    // turn on light relay
            default: break;
          }
          break;

        // Process Down button pushes
        case btnDOWN:
          switch(currentMenu)           // for Button Down check which menu is at
          {
            case mainMenu:              // @ main, goto powerAll
              currentMenu = sensorPH;   // set the curren menu status
              displayMenu(currentMenu); // display power all
              break;
            case powerAll:              // @ power all menu
              toggleRelayComponent(TOGGLEALL, 0);
              break;                    // power off all relay
            case powerMainPump:         // @ power main pump menu
              toggleRelayComponent(PUMPPIN, 0);
              break;                    // turn off main pump relay
            case powerAirStone:         // @ power air stone menu
              toggleRelayComponent(AIRSTONEPIN, 0);
              break;                    // turn off air stone relay
            case powerLight:            // @ power light menu
              toggleRelayComponent(LIGHTPIN, 0);
              break;                    // turn off light relay
            default: break;
          }
          break;

        // Process Right Button Presses
        case btnRIGHT:
          switch(currentMenu)               // for Button Right check which menu is at
          {
            case mainMenu:
              currentMenu = mainMenu2;
              displayMenuR(currentMenu);
              break;

            case mainMenu2:
              currentMenu = mainMenu;
              displayMenuR(currentMenu);
              break;

            case powerAll:                  // @ power all menu
              currentMenu = powerMainPump;  // go to the main pump menu
              displayMenuR(currentMenu);
              break;                        // display the main pump menu

            case powerMainPump:             // @ power main pump menu
              currentMenu = powerAirStone;
              displayMenuR(currentMenu);
              break;                        // turn off main pump relay

            case powerAirStone:             // @ power air stone menu
              currentMenu = powerLight;
              displayMenuR(currentMenu);
              break;                        // turn off air stone relay

            case powerLight:                // @ power light menu
              currentMenu = powerAll;
              displayMenuR(currentMenu);
              break;                        // turn off light relay

            case sensorPH:
              currentMenu = sensorEC;
              displayMenuR(currentMenu);
              break;

            case sensorEC:
              currentMenu = sensorWTemp;
              displayMenuR(currentMenu);
              break;

            case sensorWTemp:
              currentMenu = sensorWLevel;
              displayMenuR(currentMenu);
              break;

            case sensorWLevel:
              currentMenu = sensorATemp;
              displayMenuR(currentMenu);
              break;

            case sensorATemp:
              currentMenu = sensorHumid;
              displayMenuR(currentMenu);
              break;

            case sensorHumid:
              currentMenu = sensorPH;
              displayMenuR(currentMenu);
              break;

            default: break;
          }
          break;

        // process left button pushes
        case btnLEFT:
          switch (currentMenu)
          {
            case mainMenu:
              currentMenu = mainMenu2;
              displayMenuR(currentMenu);
              break;

            case mainMenu2:
              currentMenu = mainMenu;
              displayMenuR(currentMenu);
              break;

            case powerAll:                  // @ power all menu
              currentMenu = powerLight;  // go to the main pump menu
              displayMenuL(currentMenu);
              break;                        // display the main pump menu

            case powerMainPump:             // @ power main pump menu
              currentMenu = powerAll;
              displayMenuL(currentMenu);
              break;                        // turn off main pump relay

            case powerAirStone:             // @ power air stone menu
              currentMenu = powerMainPump;
              displayMenuL(currentMenu);
              break;                        // turn off air stone relay

            case powerLight:                // @ power light menu
              currentMenu = powerAirStone;
              displayMenuL(currentMenu);
              break;                        // turn off light relay

            case sensorPH:
              currentMenu = sensorHumid;
              displayMenuL(currentMenu);
              break;

            case sensorEC:
              currentMenu = sensorPH;
              displayMenuL(currentMenu);
              break;

            case sensorWTemp:
              currentMenu = sensorEC;
              displayMenuL(currentMenu);
              break;

            case sensorWLevel:
              currentMenu = sensorWTemp;
              displayMenuL(currentMenu);
              break;

            case sensorATemp:
              currentMenu = sensorWLevel;
              displayMenuL(currentMenu);
              break;

            case sensorHumid:
              currentMenu = sensorATemp;
              displayMenuL(currentMenu);
              break;

            default: break;
          }
          break;

        // process select button presses, should only go back to All Menu
        case btnSELECT:
          switch(currentMenu){
            case powerAll:
            case powerMainPump:
            case powerAirStone:
            case powerLight:
            case sensorPH:
            case sensorEC:
            case sensorATemp:
            case sensorHumid:
            case sensorWTemp:
            case sensorWLevel:
            if(lcdStatus == LCDAWAKE)
            {
              currentMenu = mainMenu;
              displayMenu(currentMenu);
            }
            if(lcdStatus == LCDSLEEP)
            {
              lcdStatus = LCDAWAKE;
              lcd.display();
            }
            break;
          }
          break;
    }
}

/*
    Sensor Measurement Functions
*/
// Measure the air temp & humidity from the DHT11
void measureAirTemp()
{
  static unsigned long samplingTime = millis();
  if(millis()-samplingTime > airTempSampleInterval)
  {
    airTemp = dht.readTemperature(true);
    humidity = dht.readHumidity();
    samplingTime = millis();
  }
  if (isnan(airTemp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature = ");
  Serial.println(airTemp);
  Serial.print("Humidity = ");
  Serial.println(humidity);
}

// Measure the pH from the DFRobot ph Sensor
void measurePH()
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+pHOffset;
      samplingTime=millis();
  }
}

// Measure the EC and Water Temp
void measureEC()
{
  /*
   Every once in a while,sample the analog value and calculate the average.
  */
  if(millis()-AnalogSampleTime>=AnalogSampleInterval)
  {
    AnalogSampleTime=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - readings[index];
    // read from the sensor:
    readings[index] = analogRead(ECsensorPin);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + readings[index];
    // advance to the next position in the array:
    index = index + 1;
    // if we're at the end of the array...
    if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / numReadings;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */
   if(millis()-tempSampleTime>=tempSampleInterval)
  {
    tempSampleTime=millis();
    temperature = TempProcess(ReadTemperature);  // read the current temperature from the  DS18B20
    TempProcess(StartConvert);                   //after the reading,start the convert for next reading
  }
   /*
   Every once in a while,print the information on the serial monitor.
  */
  if(millis()-printTime>=printInterval)
  {
    printTime=millis();
    averageVoltage=AnalogAverage*(float)5000/1024;
    Serial.print("Analog value:");
    Serial.print(AnalogAverage);   //analog average,from 0 to 1023
    Serial.print("    Voltage:");
    Serial.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
    Serial.print("mV    ");
    Serial.print("temp:");
    Serial.print(temperature);    //current temperature
    Serial.print("^C     EC:");

    float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;
    if(CoefficientVolatge<150)Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)Serial.println("Out of the range!");  //>20ms/cm,out of the range
    else
    {
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
      ECcurrent/=1000;    //convert us/cm to ms/cm
      // ECcurrent/=ECfactor;
      // Serial.print(ECcurrent/ECfactor,2);  //two decimal
      Serial.println("ms/cm");
    }
  }
}

// Measure Water Level
void measureWL()
{
  waterLevel = 0;
  // Go read the water level
  for(int i = 0; i < 3; i++)
  {
    if(digitalRead(wlSensorPins[i]) == HIGH)
    {
      waterLevel = 3 - i;
    }
  }

  switch(waterLevel)
  {
    case 1:
      waterLevelStr = String("FULL");
      break;
    case 2:
      waterLevelStr = String("MID");
      break;
    case 3:
      waterLevelStr = String("LOW");
      break;
    default:
      waterLevelStr = String("EMPTY");
      break;
  }
}

/*
    Relay and Pump Control Functions
*/
/*  turn on or off a relay controlled component
    Params:
    int component : identifies which component to toggle
    int newPowStatus : power status to change the component to
*/

void toggleRelayComponent(int component, int newPowStatus)
{
  switch (newPowStatus)
  {
    case 1:
      if(component == TOGGLEALL)
      {
        digitalWrite(LIGHTPIN, HIGH);
        digitalWrite(AIRSTONEPIN, HIGH);
        digitalWrite(PUMPPIN, HIGH);
      } else
      {
        digitalWrite(component, HIGH);
      }
      break;
    case 0:
    if(component == TOGGLEALL)
    {
      digitalWrite(LIGHTPIN, LOW);
      digitalWrite(AIRSTONEPIN, LOW);
      digitalWrite(PUMPPIN, LOW);
    } else
    {
      digitalWrite(component, LOW);
    }
      break;
    default:
      break;
  }
}

void stopPump()
{
    digitalWrite(runningPump, LOW);
}

void runPump(int pump, unsigned long durationMS)
{
  unsigned long prev = 0;
  unsigned long current = millis();
  runningPump = pump;
  digitalWrite(pump, HIGH);
  timer.setTimeout(durationMS, stopPump );
}
/*
    Helper Functions
*/

//ch=0,let the DS18B20 start the convert;ch=1,MCU read the current temperature from the DS18B20.
float TempProcess(bool ch)
{
  //returns the temperature from one DS18B20 in DEG Celsius
  static byte data[12];
  static byte addr[8];
  static float TemperatureSum;
  if(!ch){
          if ( !ds.search(addr)) {
              Serial.println("no more sensors on chain, reset search!");
              ds.reset_search();
              return 0;
          }
          if ( OneWire::crc8( addr, 7) != addr[7]) {
              Serial.println("CRC is not valid!");
              return 0;
          }
          if ( addr[0] != 0x10 && addr[0] != 0x28) {
              Serial.print("Device is not recognized!");
              return 0;
          }
          ds.reset();
          ds.select(addr);
          ds.write(0x44,1); // start conversion, with parasite power on at the end
  }
  else{
          byte present = ds.reset();
          ds.select(addr);
          ds.write(0xBE); // Read Scratchpad
          for (int i = 0; i < 9; i++) { // we need 9 bytes
            data[i] = ds.read();
          }
          ds.reset_search();
          byte MSB = data[1];
          byte LSB = data[0];
          float tempRead = ((MSB << 8) | LSB); //using two's compliment
          TemperatureSum = tempRead / 16;
    }
          return TemperatureSum;
}

// used to get an accurate ph value
double avergearray(int* arr, int number)
{
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

/*
    Arduino Setup Loop Code
*/
void setup(){
  // LCD INIT
  lcdStatus = LCDAWAKE;
  lcd.begin(16, 2);               // start the library
  lcd.setCursor(0,0);             // set the LCD cursor   position
  lcd.print("Power");              // print a simple message on the LCD
  lcd.setCursor(0,1);             // set the LCD cursor   position
  lcd.print("Sensors");
  currentMenu = mainMenu;
  dht.begin();

  //SETUP PINMODES
  // WL
  pinMode(wlHigh, INPUT);
  pinMode(wlMed, INPUT);
  pinMode(wlLow, INPUT);
  // RELAY
  pinMode(LIGHTPIN, OUTPUT);
  pinMode(PUMPPIN, OUTPUT);
  pinMode(AIRSTONEPIN, OUTPUT);
  pinMode(TOGGLEALL, OUTPUT);
  // P_PUMP
  pinMode(PH_DOWN, OUTPUT);
  pinMode(PH_UP, OUTPUT);
  pinMode(NUTRIENT, OUTPUT);
  //make sure the pumps are off
  digitalWrite(PH_DOWN, LOW);
  digitalWrite(PH_UP, LOW);
  digitalWrite(NUTRIENT, LOW);


}
void loop()
{
  measurePH();
  measureEC();
  measureWL();
  measureAirTemp();
  processDisplay();
}
