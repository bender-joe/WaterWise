/*************************************************************************************

  Joseph Bender (2016)
  Senior Design Project
  WaterWise atmega2560 cdoe

**************************************************************************************/
#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int currentMenu = 0;

// Button Constants
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Menu Display Status Constants
#define mainMenu        100
#define powerAll        101
#define powerMainPump   102
#define powerAirStone   103
#define powerLight      104
#define sensorEC        105
#define sensorPH        106
#define sensorWTemp     107
#define sensorATemp     108
#define sensorHumid    109
#define sensorWLevel    110

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

void displayMenuR(int menu)
{
  // clear the current screen
  // need to scroll in the text
  lcd.clear();
  //which menu to print
  switch(menu){
    case powerAll:
      lcd.print("All On");
      lcd.setCursor(0,1);
      lcd.print("All Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(75);
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
        delay(75);
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
        delay(75);
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
        delay(75);
      }
      break;
    case sensorPH:
      lcd.print("pH Sensor");
      lcd.setCursor(0,1);
      lcd.print("xx.xx HH:MM:SS");    // Get latest ph reading here
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(75);
      }
      case sensorEC:
        lcd.print("EC Sensor ms/cm");
        lcd.setCursor(0,1);
        lcd.print("xx.xx  HH:MM:SS");    // Get latest ph reading here
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
          delay(75);
        }
      break;
    default: break;
  }
}

void displayMenuL(int menu)
{
  // clear the current screen
  // need to scroll in the text left
  lcd.clear();
  //which menu to print
  switch(menu){
    case powerAll:
      lcd.print("All On");
      lcd.setCursor(0,1);
      lcd.print("All Off");
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(75);
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
        delay(75);
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
        delay(75);
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
        delay(75);
      }
      break;
    case sensorPH:
      lcd.print("pH Sensor");
      lcd.setCursor(0,1);
      lcd.print("xxx.xxx HH:MM:SS");    // Get latest ph reading here
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayLeft();
      }
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
        lcd.scrollDisplayRight();
        delay(75);
      }
      case sensorEC:
        lcd.print("EC Sensor ms/cm");
        lcd.setCursor(0,1);
        lcd.print("xxx.xxx HH:MM:SS");    // get latest ec reading here
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(75);
        }
      break;
      case sensorWTemp:
        lcd.print("Water Temp Faren");
        lcd.setCursor(0,1);
        lcd.print("xxx.xxx HH:MM:SS");    // get latest water temp here
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(75);
        }
      break;
      case sensorWLevel:
        lcd.print("Water Level HML");
        lcd.setCursor(0,1);
        lcd.print("xxxxxxx HH:MM:SS");
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(75);
        }
      break;
      case sensorATemp:
        lcd.print("Air Temp Faren");
        lcd.setCursor(0,1);
        lcd.print("xxxxxxx HH:MM:SS");
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(75);
        }
      break;
      case sensorHumid:
        lcd.print("Air Humidity %");
        lcd.setCursor(0,1);
        lcd.print("xxxxxxx HH:MM:SS");
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayLeft();
        }
        for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
          lcd.scrollDisplayRight();
          delay(75);
        }
      break;
    default: break;
  }
}

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
      lcd.print("xxx.xxx HH:MM:SS");    // Get latest ph reading here
      break;
    case sensorEC:
      lcd.print("EC Sensor ms/cm");
      lcd.setCursor(0,1);
      lcd.print("xxx.xxx HH:MM:SS");    // get latest ec reading here
      break;
    case sensorWTemp:
      lcd.print("Water Temp Faren");
      lcd.setCursor(0,1);
      lcd.print("xxx.xxx HH:MM:SS");    // get latest water temp here
      break;
    case sensorWLevel:
      lcd.print("Water Level HML");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxx HH:MM:SS");
      break;
    case sensorATemp:
      lcd.print("Air Temp Faren");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxx HH:MM:SS");
      break;
    case sensorHumid:
      lcd.print("Air Humidity %");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxx HH:MM:SS");
      break;
    default: break;
  }
}

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
            case powerAll:              // @ power all menu
              break;                    // power on all relay
            case powerMainPump:         // @ power main pump menu
              break;                    // turn on main pump relay
            case powerAirStone:         // @ power air stone menu
              break;                    // turn on air stone relay
            case powerLight:            // @ power light menu
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
              break;                    // power off all relay
            case powerMainPump:         // @ power main pump menu
              break;                    // turn off main pump relay
            case powerAirStone:         // @ power air stone menu
              break;                    // turn off air stone relay
            case powerLight:            // @ power light menu
              break;                    // turn off light relay
            default: break;
          }
        break;

        // Process Right Button Presses
        case btnRIGHT:
        switch(currentMenu)               // for Button Right check which menu is at
        {
          case powerAll:                  // @ power all menu
            currentMenu = powerMainPump;  // go to the main pump menu
            for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
              lcd.scrollDisplayLeft();
              delay(75);
            }
            displayMenuR(currentMenu);
            break;                        // display the main pump menu
          case powerMainPump:             // @ power main pump menu
            currentMenu = powerAirStone;
            for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
              lcd.scrollDisplayLeft();
              delay(75);
            }
            displayMenuR(currentMenu);
            break;                        // turn off main pump relay
          case powerAirStone:             // @ power air stone menu
            currentMenu = powerLight;
            for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
              lcd.scrollDisplayLeft();
              delay(75);
            }
            displayMenuR(currentMenu);
            break;                        // turn off air stone relay
          case powerLight:                // @ power light menu
          currentMenu = powerAll;
            for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
              lcd.scrollDisplayLeft();
              delay(75);
            }
            displayMenuR(currentMenu);
            break;                        // turn off light relay
          default: break;
        }
        break;

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
            currentMenu = mainMenu;
            displayMenu(currentMenu); break;
          }
          break;
    }
}

void setup(){
   lcd.begin(16, 2);               // start the library
   lcd.setCursor(0,0);             // set the LCD cursor   position
   lcd.print("Power");              // print a simple message on the LCD
   lcd.setCursor(0,1);             // set the LCD cursor   position
   lcd.print("Sensors");
   currentMenu = mainMenu;
}

void loop()
{
  processDisplay();
}
