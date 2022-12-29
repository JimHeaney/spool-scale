//Spool Scale 
//Version 1.1.1
//By: Jim Heaney, www.jim-heaney.com
//December 2022

//GitHub: https://github.com/JimHeaney/spool-scale

//License: 
//Creative Commons 4.0 Attribution-NonCommercial-ShareAlike
//https://creativecommons.org/licenses/by-nc-sa/4.0/

//This version of the SpoolScale firmware is intended for use with the WIP release V1.1 of the hardware. 
//There were multiple bugs and shortcomings with the V1.1 hardware. This firmware is available solely for the purpose of documentation. 
//See V1.2 and beyond for a more useable version.

#define abs(x) ((x)>0?(x):-(x))

//Include Libraries:
#include <HX711.h>
#include <TM1616.h>
#include <TM16xxDisplay.h>
#include <EEPROM.h>

//Pin Names:
#define HX711_DAT PIN_PA4
#define HX711_CLK PIN_PA5
#define TM16_DAT PIN_PC1
#define TM16_CLK PIN_PC0
#define TM16_STB PIN_PC2
#define INTERFACE1 PIN_PA2
#define INTERFACE2 PIN_PA1
#define DIMIN PIN_PB0
#define WARN PIN_PA6
#define BUTTON PIN_PA7
#define DIP1 PIN_PB3
#define DIP2 PIN_PB2
#define DIP3 PIN_PB1
#define DIP4 PIN_PA3
#define DIP5 PIN_PC3
#define DIPR1 PIN_PB4
#define DIPR2 PIN_PB5

//Create Objects:
HX711 scale;
TM1616 module(TM16_DAT, TM16_CLK, TM16_STB);
TM16xxDisplay display(&module, 4);

//Variables:
int spool = 0; //The weight of an empty spool
int safety = 10; //A safety weight to subtract from the reading, to ensure we always show less filament than there actually is.
bool mode = 0; //The mode of the IO. Set to 1 i2C, 0 for GPIO.
float calibration = 0; //The calibratation value found in calibration routine or retrieved from EEPROM.
long zero = 0; //The output of the HX711 with nothing on it, found in calibration routint or retrieved from EEPROM
byte brightness = 0; //The current brigtness level of the screen. Stored globally to allow for some hysteresis
bool warnon = 0; //Set to 1 when the warning output is turned on
float warn = 50.0; //The number of grams to trigger the warning alarm. Currently hard-coded, may make a setting in the future.
float oldweight = 100000; //Stores the last measurement of the scale, to prevent jittering between 2 values. Starts very high so the code falls into start working.

void setup() {
  // put your setup code here, to run once:
  
  //Start scale and set pinModes:
  scale.begin(HX711_DAT, HX711_CLK);
  pinMode(WARN, OUTPUT);

  //Write "SPOL" then "SCLE" then "rxxx" (version number) to the screen during startup for a few seconds
  display.println("SPOL");
  delay(1000);
  display.println("SCLE");
  delay(1000);
  display.println("r111");
  delay(1000);
  display.println("");

  //Read in the values of all the setting switches
  //Due to a hardware issue, the DIP switches do not work. So instead, the weight of a hatchbox spool is hard-coded
  spool = 244;

  //Read if the button is held down on startup. If so, then run the calibration/reset routine by calling calibrate()
  if(digitalRead(BUTTON) == 1){
    calibrate();
  }
  
  //Load the calibration settings from EEPROM
  EEPROM.get(0, zero);
  EEPROM.get(10, calibration);
  scale.set_scale();
  scale.tare();
  scale.set_scale(calibration);
  scale.set_offset(zero);

  //Activate the proper communication standard based on the input switches
  //Hardware bugs mean this version will only support HIGH/LOW GPIO, so set to input pullup
  pinMode(INTERFACE1, INPUT_PULLUP);
  pinMode(INTERFACE2, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:

  //Read the states of the communication pins based on the mode
  //In GPIO mode, if the pins are low, just skip everything and turn the screen off
  if((digitalRead(INTERFACE1) == 0) || (digitalRead(INTERFACE2) == 0)){
    digitalWrite(WARN, 0);
    display.println("----");
    display.setIntensity(0);
    goto idle;
  }

  //Set the screen brightness:
  //Read the state of the DIMIN pin
  int brightin = analogRead(DIMIN);
  //If resistance connected is less than 1K (0.455V, 93), turn the display fully on
  if(brightin < 90){
    brightness = 7;  
  }
  //If resistance connected is between 1k (0.455V, 93) and 9k (2.368V, 483) (steps of 65), vary the brightness accordingly with some hysteresis
  if((brightin > 100) && (brightin < 165)){
    brightness = 6;
  }
  if((brightin > 175) && (brightin < 230)){
    brightness = 5;
  }
  if((brightin > 240) && (brightin < 285)){
    brightness = 4;
  }
  if((brightin > 295) && (brightin < 355)){
    brightness = 3;
  }
  if((brightin > 365) && (brightin < 420)){
    brightness = 2;
  }
  if((brightin > 430) && (brightin < 164805)){
    brightness = 1;
  }

  //If resistance connected is more than 9k (2.368V, 483) but less than 20k (3.333v, 680), turn the display fully off
  if((brightin > 490) && (brightin < 680)){
    brightness = 0;
  }
  //If resistance connected is more than 20k (3.333v, 680) (e.g. no connection), turn the display fully on
  if(brightin > 700){
    brightness = 7;
  }

  display.setIntensity(brightness);

  //Read the current scale weight, subtract spool weight
  float weight = scale.get_units(10);
  weight = weight - float(spool);

  //Subtract some extra "safety" weight to make sure that the scale never reads there is filament remaining when there isn't. 
  weight = weight - float(safety);
  
  //Stop the weight from going back up if the difference is just 1, to prevent jittering
  if((weight > oldweight) && ((weight - 1.0) <= oldweight)){
    weight = oldweight;
  } else{
    oldweight = weight;    
  }

  //If the weight is less than 0, just set the weight to 0
  if(weight <= 0.0){
    weight = 0.0;
  }

  //Report weight on the display
  display.println(weight,0);

  //If the weight is below the critical value, set the WARN pin high. 
  if((weight <= warn) && (warnon == 0)){
    warnon = 1;    
  }
  if((weight >= (warn + 10)) && (warnon == 1)){
    warnon = 0;
  }
  digitalWrite(WARN, warnon);

  idle:

  //Delay for a little bit (100ms?)
  delay(100);

}

void calibrate(){
  //Routine to run to calibrate the system

  //Write "CAL" on the screen
  display.println("CAL");
  //Wait until the button is released
  while(digitalRead(BUTTON) == 1){
    delay(1);    
  }
  //Wait another 1 second
  delay(1000);  
  
  //Write "PUT0" on the screen, indicating to put nothing on the spool holder
  display.println("PUT0");
  //Wait for button to be pressed
  while(digitalRead(BUTTON) == 0){
    delay(1);
  }
  //Once button is pressed, read in current scale weight as the 0 point of the scale
  scale.set_scale();
  scale.tare();
  zero = scale.read_average();
  
  //Write "ESPL" on the screen, indicating to put an empty spool on the scale 
  display.println("ESPL");  
  //Wait for the button to be pressed
  while(digitalRead(BUTTON) == 0){
    delay(1);
  }
  //Once button is pressed, enter into a loop to calibrate
  display.println("STby");
  unsigned long caltime = millis() + 120000; //Note the time and add 120 second. If calibration not done by then, error out. 
  bool calibrating = 1;
  float temp = scale.get_units(10); 
  calibration = temp / float(spool); //Start with a rough approximation of what the calibration value should be
  

  while(calibrating){
    scale.set_scale(calibration);
    temp = scale.get_units(10);
    float difference = temp - spool;
    if(abs(difference) <= 0.25){
      //If within 0.25 grams, mark as complete
      calibrating = 0;
      goto exitcal;
    }
    if(difference > 100.0){
      //If the weight is way outside the ok range and high, increase the calibration value by 10
      calibration = calibration + 10;
    }
    if(difference > 0.0){
      //If the weight is outside the ok range and high, increase the calibration value by 1
      calibration = calibration + 1;
    }
    if(difference < 0.0){
      //If the weight is less than 0, multiply calibration value by -1
      calibration = calibration * -1;
    }
    if(caltime <= millis()){
      //If a good value wasn't found in 60 seconds, error out.
      display.println("ERRO");
      delay(1000);
      display.println();
      calibrating = 0;
    }
    display.println(temp);
  }
  exitcal:
  EEPROM.put(0, zero);
  EEPROM.put(10, calibration);
  display.println("DONE");
  delay(1000);
  scale.set_scale(calibration);
  temp = scale.get_units(10);
  display.println(temp,0);
  delay(100);
  display.println("");
}
