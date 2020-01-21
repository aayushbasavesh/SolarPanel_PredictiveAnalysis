#include <Wire.h>
#include <TimeLib.h>

//******************************************************************VALUES FOR millis******************************************************************
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
const long interval1 = 180000;
const long interval2 = 1000;

//******************************************************************INITIALLZING PINS******************************************************************
int relay_Out = 2;
int SC_Current = A0 ;
//int load_Current = A2 ;
int OC_Voltage = A1 ;

//******************************************************************VALUES FOR ACS712******************************************************************
int sensitivity = 100; //100 for 20A Module and 66 for 30A Module
int volt_read= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;

//******************************************************************VALUES FOR VOLTMETER******************************************************************
float vout = 0.0;
float vin = 0.0;
float R1 = 100000; 
float R2 = 22000; 
int value = 0;

//***************************************************************FUNCTION FOR READING CURRENT***************************************************************
float readCurrent(float raw_Current)
{
   float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;
   for (int x = 0; x < 150; x++){ //Get 150 samples
   AcsValue = analogRead(raw_Current);     //Read current sensor values   
   Samples = Samples + AcsValue;  //Add samples together
   delay (3); // let ADC settle before next sample 3ms
}
AvgAcs=Samples/150.0;//Taking Average of Samples

//((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
//2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
//out to be 2.5 which is out offset. If your arduino is working on different voltage than 
//you must change the offset according to the input voltage)
//0.100v(100mV) is rise in output voltage when 1A current flows at input
AcsValueF = (2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.100;

return AcsValueF; //Print the read current on Serial monitor

}

//***************************************************************FUNCTION FOR READING VOLTAGE***************************************************************
float readVoltage(float raw_Voltage)
{
    double VolValue=0.0,Samples=0.0,AvgVol=0.0;
    for (int x = 0; x < 500; x++)
    { //Get 150 samples
    VolValue = analogRead(raw_Voltage);     //Read current sensor values   
    Samples = Samples + VolValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
    }
    AvgVol = Samples/500.0;
    AvgVol = ((AvgVol*5.0)/1023);
    double vin = (AvgVol * 5.545454 );
    return vin;
}

void setup() 
{
  Serial.begin(9600);
  pinMode(relay_Out, OUTPUT);
  pinMode(SC_Current, INPUT);
  pinMode(OC_Voltage, INPUT);
  while (!Serial) ; //Wait for serial monitor

}
void loop() 
{
  unsigned long currentMillis1 = millis();
  //unsigned long currentMillis2 = millis();
  if (currentMillis1 - previousMillis1 >= interval1)
  {
    // save the last time you transmitted data
    previousMillis1 = currentMillis1;
    //Send Load Current
    /*Serial.print(readCurrent(load_Current));
    Serial.print(",");*/
    //Send Open Circuit Voltage
    digitalWrite(relay_Out, LOW);
    unsigned long currentMillis2 = millis();
    previousMillis2=currentMillis2;
    while(currentMillis2-previousMillis2<=interval2)
    {
      currentMillis2 = millis();
    }
    Serial.print(readVoltage(OC_Voltage));
    Serial.print(",");
    currentMillis2 = millis();
    previousMillis2=currentMillis2;
    while(currentMillis2-previousMillis2<=interval2)
    {
      currentMillis2 = millis();
    }
    digitalWrite(relay_Out, HIGH);
    currentMillis2 = millis();
    previousMillis2=currentMillis2;
    while(currentMillis2-previousMillis2<=interval2)
    {
      currentMillis2 = millis();
    }
    // Send Short Circuit Current 
    Serial.println(abs(readCurrent(SC_Current)-0.27));
    
  }
}
