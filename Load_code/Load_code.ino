#include "Wire.h"
#include "SD.h"
#include"SPI.h"
#define DS3231_I2C_ADDRESS 0x68
#define relay1 2
#define relay2 3
#define relay3 4
#define relay4 5
int relay[]={relay1,relay2,relay3,relay4};
const int CSpin = 10;
String dataString ="";
File loadData;
long randNumber;
long randNumber1;
long randNumber2;
long randNumber3;
float load1;
float load2;
float load3;
float load4;
unsigned long previousMillis = 0; 
bool flag = 1;
const int currentPin1 = A0;
const int currentPin2 = A1;
const int currentPin3 = A2;
const int currentPin4 = A3;
int sensitivity = 0;
int adcValue= 0;
float offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,41,16,5,13,02,20);
     randomSeed(analogRead(A0));
     pinMode(relay1, OUTPUT);
     pinMode(relay2, OUTPUT);
     pinMode(relay3, OUTPUT);
     pinMode(relay4, OUTPUT);
     pinMode(CSpin, OUTPUT);
     if (!SD.begin(CSpin)) 
     {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
     }
     Serial.println("card initialized.");
}

void saveData()
{
    if(SD.exists("data.csv"))
    {
      // check the card is still there
      // now append new data file
      loadData = SD.open("data.csv", FILE_WRITE);
      if (loadData)
      {
        loadData.println(dataString);
        loadData.close(); // close the file
        Serial.println("Written to file !");
        
      }
    }
    else
    {
      Serial.println("Error writing to file !");
    }
}

float current(int currentData, int sens)
{
  sensitivity = sens ;
  adcValue = currentData;
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  return currentValue;
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void loop()
{
  digitalWrite(relay[0], HIGH);
  digitalWrite(relay[1], HIGH);
  digitalWrite(relay[2], HIGH);
  digitalWrite(relay[3], HIGH);
  int condition;
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  // Toggle load during weekdays
  if(dayOfWeek>=2 && dayOfWeek<=6)
  {
    if(hour>= 7 && hour<9)
    {  unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          do
          { 
            randNumber1 = random(0,4);
            randNumber2 = random(0,4);
          } while(randNumber1==randNumber2);
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber1], LOW);
       digitalWrite(relay[randNumber2], LOW);
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber1], HIGH);
          digitalWrite(relay[randNumber2], HIGH);  
          flag = 1;          
       }
    }
    if(hour>=9 && hour<18)
    {
       unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          randNumber = random(0,4);
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber], LOW);
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber], HIGH);
          flag = 1;          
       }
    }
    
    if(hour>= 18 && hour<22)
    {
       unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          do
          { 
            randNumber1 = random(0,4);
            randNumber2 = random(0,4);
          } while(randNumber1==randNumber2);
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber1], LOW);
       digitalWrite(relay[randNumber2], LOW);
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber1], HIGH);
          digitalWrite(relay[randNumber2], HIGH);  
          flag = 1;          
       }
    }
    if(hour>= 22 && hour<7)                   //CHECK
    {
       unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          do
          { 
            randNumber1 = random(0,4);
            randNumber2 = random(0,4);
          } while(randNumber1==randNumber2);
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber1], LOW);
       digitalWrite(relay[randNumber2], LOW);
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber1], HIGH);
          digitalWrite(relay[randNumber2], HIGH);  
          flag = 1;          
       }
    }
  }
  if(dayOfWeek == 1 || dayOfWeek==7)
  {
    if(hour>=7 && hour<18)
    {
       unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          do
          { 
            randNumber1 = random(0,4);
            randNumber2 = random(0,4);
          } while(randNumber1==randNumber2);
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber1], LOW);
       digitalWrite(relay[randNumber2], LOW);
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber1], HIGH);
          digitalWrite(relay[randNumber2], HIGH);  
          flag = 1;          
       }
    }
    if(hour>=18 && hour<22)
    {
       unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          do
          { 
            randNumber1 = random(0,4);
            randNumber2 = random(0,4);
            randNumber3 = random(0,4);
          } while((randNumber1==randNumber2)||(randNumber2==randNumber3)||(randNumber3==randNumber1));
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber1], LOW);
       digitalWrite(relay[randNumber2], LOW);
       digitalWrite(relay[randNumber3], LOW);
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber1], HIGH);
          digitalWrite(relay[randNumber2], HIGH);
          digitalWrite(relay[randNumber3], HIGH);  
          flag = 1;          
       }
     }
   
    if(hour>= 22 && hour<7)
    {
       unsigned long interval;
       unsigned long currentMillis = millis();
       if (flag == 1)
       {
          do
          { 
            randNumber1 = random(0,4);
            randNumber2 = random(0,4);
          } while(randNumber1==randNumber2);
          interval = random(30e+4,66e+4);
          flag = 0;
       }
       digitalWrite(relay[randNumber1], LOW);
       digitalWrite(relay[randNumber2], LOW);
       Serial.println("Condition7");
       if (currentMillis - previousMillis >= interval) 
       {
          previousMillis = currentMillis;
          digitalWrite(relay[randNumber1], HIGH);
          digitalWrite(relay[randNumber2], HIGH);  
          flag = 1;          
       }
    }
  }
  
load1 = abs(current(analogRead(currentPin1),185));
load2 = abs(current(analogRead(currentPin2),185));
load3 = abs(current(analogRead(currentPin3),100));
load4 = abs(current(analogRead(currentPin4),100));
dataString = String(dayOfMonth) + "/" + String(month) + "/" + String(year) + "," + String(hour) + ":" + String(minute) + ":" + String(second) + "," + String(load1) + "," + String(load2) + "," + String(load3) + "," + String(load4);
Serial.println(dataString);
saveData();
delay(180000);
}
