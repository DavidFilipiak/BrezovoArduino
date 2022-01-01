#include <OneWire.h>      // zahrnuté knižnice, ktoré program používa
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <max6675.h>

//PIXEL ART----------------------------------------------------------------------------------
byte LeftFace[8] =
{
0b00011,
0b00100,
0b01010,
0b01000,
0b01010,
0b01011,
0b00100,
0b00011
};

byte RightFace[8] =
{
0b11000,
0b00100,
0b01010,
0b00010,
0b01010,
0b11010,
0b00100,
0b11000
};
//PIXEL ART END------------------------------------------------------------------------------



const byte senzorPinLava = 12;    /*nastavenie hodnoty premennej senzorPin ako 12, lebo dátový
                              kábel senzorov je pripojený do pinu 12. Keby sme kábel
                              pripojili k inému pinu, číslo treba zmeniť.
                              const -> konštantná, nemeniaca sa premenná
                              byte -> premenná zaberá 1 byte (8 bitov) RAM pamäte*/
const byte senzorPinPrava = 13;

const byte teplomerPinCerpadlo2 = 10;
                              
const byte photoResPin = A0;

const byte relayPin = 7;
bool rel_zopnute = false;

const byte tempPinSO = 2;   //serial out pin
const byte tempPinCS = 3;   //chip select pin
const byte tempPinSCK = 4;  //serial clock pin
MAX6675 TeplomerSpaliny(tempPinSCK, tempPinCS, tempPinSO);  // create instance object of MAX6675

LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  /* nastavenie displeja pomocou I2C komunikácie
                                                                 a použitím knižnice LiquidCrystal_I2C
                                                                 !!! TOTO NASTAVENIE NEMENIŤ !!!*/
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //displej vlavo dole
LiquidCrystal_I2C lcd3(0x25, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // žltý displej vpravo hore
LiquidCrystal_I2C lcd4(0x23, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //displej vpravo dole
//LiquidCrystal_I2C lcd1 (0x27, 20, 4);

OneWire oneWire(senzorPinLava);     //pomocou knižnice OneWire zapíše dáta z pinu 12(senzorPin) do premennej "oneWire"
DallasTemperature senzoryL(&oneWire);  //pomocou knižnice  a premennej "oneWire" DallasTemperatue vytvorí premennú "senzory"

OneWire twoWire(senzorPinPrava);
DallasTemperature senzoryP(&twoWire);

OneWire wireCerpadlo2(teplomerPinCerpadlo2);
DallasTemperature teplomerCerpadlo2(&wireCerpadlo2);

void setup() {    //časť setup sa vykoná raz, na začiatku programu
  Serial.begin(9600);     //zaháji komunikáciu so seriálovým monitorom (užitočné pri hľadaní chýb v programe)
  senzoryL.begin();       //zaháji komunikáciu so senzormi
  senzoryP.begin();
  lcd1.begin(20,4);        //zaháji komunikáciu s displejom, a nastaví ho na 20 stĺpcov a 4 riadky
  lcd2.begin(16,2);
  lcd3.begin(20,4);
  lcd4.begin(16,2);
  
  lcd1.backlight();        //zapne podsvietenie displeja
  lcd2.backlight();
  lcd3.backlight();
  lcd4.backlight();

  lcd2.createChar(0,LeftFace);
  lcd2.createChar(1,RightFace);

  pinMode(photoResPin, INPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {   //časť loop sa bude opakovať donekonečna, asi 4000 krát za sekundu

  String stupneZnak = String(char(223));
  
  lcd4.setCursor(0,0);
  teplomerCerpadlo2.requestTemperatures();
  Serial.println(analogRead(photoResPin));
  
  if (analogRead(photoResPin) > 300){  // hodnota medzi 0 a 1024
    lcd4.print("Cerpadlo 2: ON ");
  }
  else{
    lcd4.print("Cerpadlo 2: OFF");
  }
  float teplotaCerpadlo2 = teplomerCerpadlo2.getTempCByIndex(0);
  lcd4.setCursor(0,1);
  lcd4.print("Obeh.voda:");
  lcd4.setCursor(11,1);
  lcd4.print(teplotaCerpadlo2,0);
  lcd4.setCursor(14,1);
  lcd4.print(stupneZnak + "C ");
  
  
  /*
  //pixel art
  lcd2.setCursor(14,0);
  lcd2.write(byte(0));
  lcd2.setCursor(15,0);
  lcd2.write(byte(1));

  //pixel art*/

  lcd2.setCursor(0,1);
  int teplotaSpaliny = round(TeplomerSpaliny.readCelsius());
  lcd2.print("Spaliny: " + String(teplotaSpaliny) + " " + stupneZnak +"C ");
  
  lcd2.setCursor(0,0);
  //Serial.println(digitalRead(relayPin));
  if(!rel_zopnute){
    if(teplotaSpaliny > 250){
      digitalWrite(relayPin, HIGH);
      rel_zopnute = true;
      lcd2.print("Cerpadlo 1: ON ");
    }
    else{
      digitalWrite(relayPin, LOW);
      rel_zopnute = false;
      lcd2.print("Cerpadlo 1: OFF");
    }
  }
  else{
    if(teplotaSpaliny < 80){
      digitalWrite(relayPin, LOW);
      rel_zopnute = false;
      lcd2.print("Cerpadlo 1: OFF");
    }
    else{
      lcd2.print("Cerpadlo 1: ON ");
      rel_zopnute = true;
      digitalWrite(relayPin, HIGH);
    }
  }

  senzoryL.requestTemperatures();    //vypýta si dáta od senzorov
  senzoryP.requestTemperatures();
  for (byte x = 0; x < 4; x++)      //4 krát zopakuje nasledujúce kroky (4 krát preto, lebo sú 4 riadky displeja)
  {
    /*
    lcd1.setCursor(0,x);           //nastaví kurzor displeja na 1 stĺpec a (x+1)-tý riadok
    lcd1.print("Teplota L" + String(4 - x) + ":");    //na x-tý riadok napíše text
    lcd1.setCursor(12,x);          //nastaví kurzor displeja na 12 stĺpec a (x+1)-tý riadok
    lcd1.print(senzoryL.getTempCByIndex(3 - x),1);        //napíše teplotu x-tého senzora v stupňoch Celzia
    lcd1.setCursor(16,x);          //nastaví kurzor displeja na 17 stĺpec a (x+1)-tý riadok
    lcd1.print(" " + stupneZnak + "C ");            //vyplní to medzerami (iba preto, lebo keď dôjde k chybe, tak sa to opraví)
    */

    lcd1.setCursor(0,x);           //nastaví kurzor displeja na 1 stĺpec a (x+1)-tý riadok
    lcd1.print("Teplota L" + String(4 - x) + ":");    //na x-tý riadok napíše text
    lcd1.setCursor(12,x);          //nastaví kurzor displeja na 12 stĺpec a (x+1)-tý riadok
    lcd1.print(senzoryL.getTempCByIndex(3 - x),1);        //napíše teplotu x-tého senzora v stupňoch Celzia
    lcd1.setCursor(16,x);          //nastaví kurzor displeja na 17 stĺpec a (x+1)-tý riadok
    lcd1.print(" " + stupneZnak + "C ");            //vyplní to medzerami (iba preto, lebo keď dôjde k chybe, tak sa to opraví)
  }

    lcd3.setCursor(0,0);
    lcd3.print("Teplota P4:");
    lcd3.setCursor(12,0);
    lcd3.print(senzoryP.getTempCByIndex(3),1);
    lcd3.setCursor(16,0);
    lcd3.print(" " + stupneZnak + "C ");

    lcd3.setCursor(0,1);
    lcd3.print("Teplota P3:");
    lcd3.setCursor(12,1);
    lcd3.print(senzoryP.getTempCByIndex(1),1);
    lcd3.setCursor(16,1);
    lcd3.print(" " + stupneZnak + "C ");

    lcd3.setCursor(0,2);
    lcd3.print("Teplota P2:");
    lcd3.setCursor(12,2);
    lcd3.print(senzoryP.getTempCByIndex(0),1);
    lcd3.setCursor(16,2);
    lcd3.print(" " + stupneZnak + "C ");

    lcd3.setCursor(0,3);
    lcd3.print("Teplota P1:");
    lcd3.setCursor(12,3);
    lcd3.print(senzoryP.getTempCByIndex(2),1);
    lcd3.setCursor(16,3);
    lcd3.print(" " + stupneZnak + "C ");
    
  delay(500);    //počká pol sekundy (500 milisekúnd) pred ďalším pokračovaním programu
}
