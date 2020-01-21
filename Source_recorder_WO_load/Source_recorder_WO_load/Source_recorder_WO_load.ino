#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

/******************************************************************VALUES FOR millis******************************************************************/
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
const long interval1 = 300000;
const long interval2 = 1000;

/******************************************************************VALUES FOR RTC******************************************************************/
const char *monthName[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
tmElements_t tm;

/******************************************************************INITIALLZING PINS******************************************************************/
int relay_Out = 7;
int SC_Current = A1 ;
//int load_Current = A2 ;
int OC_Voltage = A0 ;

/******************************************************************VALUES FOR ACS712******************************************************************/
int sensitivity = 66; //100 for 20A Module and 66 for 30A Module
int volt_read= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;

/******************************************************************VALUES FOR VOLTMETER******************************************************************/
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; 
float R2 = 7500.0; 
int value = 0;

/***************************************************************FUNCTION FOR READING CURRENT***************************************************************/
float readCurrent(float raw_Current)
{
    volt_read = analogRead(raw_Current);
    Voltage = (volt_read / 1024.0) * 5000; //in mV
    Amps = ((Voltage - ACSoffset) / sensitivity);
    return Amps;
}

/***************************************************************FUNCTION FOR READING VOLTAGE***************************************************************/
float readVoltage(float raw_Voltage)
{
    value = analogRead(raw_Voltage);
    vout = (value * 4.8) / 1024.0; // see text
    vin = (vout / (R2/(R1+R2)))*100;
    return vin;
}

void setup() 
{
  Serial.begin(9600);
  pinMode(relay_Out, OUTPUT);
  pinMode(SC_Current, INPUT);
  //pinMode(load_Current, INPUT);
  pinMode(OC_Voltage, INPUT);
  while (!Serial) ; //Wait for serial monitor
/******************************************************************SET TIME FOR RTC******************************************************************/
  bool parse=false;
  bool config=false;
  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) 
  {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) 
    {
      config = true;
    }
  }
  delay(200);

/***************************************************************NEED TO BE COMMENTED BEFORE RECORDING***************************************************************/ 
  /*if (parse && config) 
  {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } 



  else if (parse) 
  {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } 
  else 
  {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }*/
}

/******************************************************************RTC FUNCTIONS******************************************************************/
bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) 
  {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

void loop() 
{
  unsigned long currentMillis1 = millis();
  unsigned long currentMillis2 = millis();
  if (currentMillis1 - previousMillis1 >= interval1)
  {
    // save the last time you transmitted data
    previousMillis1 = currentMillis1;
    if (RTC.read(tm)) 
    {
      Serial.print(tm.Day);//Day
      Serial.write('/');
      Serial.print(tm.Month);//Month
      Serial.write('/');
      Serial.print(tmYearToCalendar(tm.Year));//year
      Serial.print(",");
      Serial.print(tm.Hour);//hour
      Serial.print(':');
      Serial.print(tm.Minute);//minute
      Serial.write(':');
      Serial.print(tm.Second);//second
      Serial.print(",");
    }
    //Send Load Current
    /*Serial.print(readCurrent(load_Current));
    Serial.print(",");*/
    //Send Open Circuit Voltage
    Serial.print(readVoltage(OC_Voltage));
    Serial.print(",");
    digitalWrite(relay_Out, LOW);
    delay(100);
    // Send Short Circuit Current 
    Serial.println(readCurrent(SC_Current));
    digitalWrite(relay_Out, HIGH);
  }
}
