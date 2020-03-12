const int currentPin = A2;
int sensitivity = 100;
int adcValue= 0;
float offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  //pinMode(5,OUTPUT);
  //digitalWrite(5,LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  Serial.print("Raw Sensor Value = " );
  Serial.print(adcValue);
  Serial.print("\t Voltage(mV) = ");
  Serial.print(adcValue);
  Serial.print("\t Current = ");
  Serial.println(currentValue,2);
  delay(1000);
}
