/* ENSC 475 Lab 5 - Pulse Display
    Date: 13 April 2018
    Authors: Isaac Cheng Hui Tan
             Nathanael Davis Tan

    Version 1.0

    This code takes in values from the arduino ADC and prints it to serial for display.

*/

int inputPin = 3;
int value = 0;
unsigned sampleRate = 50; // per second
unsigned sampleTime = 0;
unsigned long lastSampleTime = 0;
float voltage = 0.0;
double timeNow = 0.0;
unsigned long count = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sampleTime = 1000/sampleRate;
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis()-lastSampleTime) > sampleTime)
  {
    value = analogRead(inputPin);
    voltage = float(value)*(5.0/1024.0);
    timeNow = float(count)*(sampleTime/1000.0);
    count++;
//    Serial.print(timeNow, DEC);  // send the first value   
//    Serial.print(",");          // separated by a comma
    Serial.print(voltage, DEC);  // other values get sent as above      
    Serial.println();           // print a linefeed character
    lastSampleTime = millis();      
  }
}
