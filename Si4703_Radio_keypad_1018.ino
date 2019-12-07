#include <Si4703_Breakout.h>
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#define I2CADDR 0x38  // PCF8574AP BASE ADDR

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
int channel;
int volume;
char rdsBuffer[10];
// keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'f','b','7','3'},
  {'e','a','6','2'},
  {'d','9','5','1'},
  {'c','8','4','0'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );

void setup()
{
  Wire.begin( );
  kpd.begin( makeKeymap(keys) );

  Serial.begin(9600);
  Serial.println("\n\nSi4703_Breakout Test Sketch");
  Serial.println("===========================");  
  Serial.println("a b     Favourite stations");
  Serial.println("+ -     Volume (max 15)");
  Serial.println("u d     Seek up / down");
  Serial.println("r       Listen for RDS Data (15 sec timeout)");
  Serial.println("Send me a command letter.");
  

  radio.powerOn();
  radio.setVolume(3);
  channel = 915; // HBC FM
  volume = 3;
  radio.setChannel(channel);
  displayInfo();

}

void loop()
{
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'u') 
    {
      channel = radio.seekUp();
      displayInfo();
    } 
    else if (ch == 'd') 
    {
      channel = radio.seekDown();
      displayInfo();
    } 
    else if (ch == '+') 
    {
      volume ++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == '-') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == 'a')
    {
      //channel = 930; // Rock FM
      channel = 904; // STV FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'b')
    {
      channel = 915; // HBC FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'r')
    {
      Serial.println("RDS listening");
      radio.readRDS(rdsBuffer, 15000);
      Serial.print("RDS heard:");
      Serial.println(rdsBuffer);      
    }
  }
  char key = kpd.getKey();
  if (key){
    Serial.println(key);
    if (key=='0') {
      channel = 804; // AIRG FM
      radio.setChannel(channel);
      displayInfo();      
    } else if (key=='1') {
      channel = 825; // NW FM
      radio.setChannel(channel);
      displayInfo();      
    } else if (key=='2') {
      channel = 852; // NHK FM
      radio.setChannel(channel);
      displayInfo();      
    }else if (key=='3') {
      channel = 904; // STV FM
      radio.setChannel(channel);
      displayInfo();      
    } else if (key=='4') {
      channel = 915; // HBC FM
      radio.setChannel(channel);
      displayInfo();      
    }else if (key=='5') {
      channel = 762; // sankakuyama FM
      radio.setChannel(channel);
      displayInfo();      
    }else if (key=='6') {
      channel = 781; // karosu sapporo FM
      radio.setChannel(channel);
      displayInfo();      
    }else if (key=='7') {
      channel = 786; // nosutarujia FM
      radio.setChannel(channel);
      displayInfo();      
    } else if (key == 'f') {
      volume ++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } else if (key == 'e') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } else {
      // nop
    }
  }
}

void displayInfo()
{
   Serial.print("Channel:"); Serial.print(channel); 
   Serial.print(" Volume:"); Serial.println(volume); 
}
