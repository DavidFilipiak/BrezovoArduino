#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const byte tempPinSO = 6;   //serial out pin
const byte tempPinCS = 5;   //chip select pin
const byte tempPinSCK = 4;  //serial clock pin

LiquidCrystal_I2C lcd(0x23, 16, 2);

Thermocouple* thermocouple;

//MAX6675 TeplomerSpaliny(tempPinSCK, tempPinCS, tempPinSO);  // create instance object of MAX6675

const String stupneZnak = String(char(223));

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     //zaháji komunikáciu so seriálovým monitorom (užitočné pri hľadaní chýb v programe)
  Serial.print("lalala");

  thermocouple = new MAX6675_Thermocouple(tempPinSCK, tempPinCS, tempPinSO);

  lcd.init();
  lcd.backlight();

}

void loop() {
  // put your main code here, to run repeatedly:
  int teplotaSpaliny = thermocouple->readCelsius();
  Serial.println(String(teplotaSpaliny));

  lcd.setCursor(0,0);
  lcd.print(String(teplotaSpaliny) + " " + stupneZnak + "C    ");

  delay(200);

}
