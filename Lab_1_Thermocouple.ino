/* ENSC 475 Lab 1 - Thermocouple
 *  Date: 23 Jan 2018
 *  Authors: Isaac Cheng Hui Tan
 *           Nathanael Davis Tan
 *  
*/


int analogPin = 3;                            // pin input of arduino for ADC
unsigned long int rawVal = 0;                 // variable to store the value read
float temp = 0;                               // Temperature in Celcius
float m = .07034; //.0617;                    // slope coefficient of conversion to celcius
float c = -7.1198; //-6.232;                  // y-intercept coefficient of conversion to celcius
unsigned long previousMillis = 0;             // variable to store the last checked time.
int avgtime = 1000;                           // number of milliseconds for averaging.
int count  = 0;                               // number of samples taken for averaging
float sampleVal = 0.0;                        // Variable to store the averaged Value
float correctedVal = 0.0;

void setup()
{
  Serial.begin(9600);                         //  setup serial
}

void loop()
{
  rawVal = analogRead(analogPin) + rawVal;    // read the input pin [0-1023, 0-5v], add it to storage variable
  count++;                                    // increment the number of samples.
  //Serial.println(rawVal);                   // debug value
  //read values for room temp - ADC [457, 22.8C]
  //read values for cold pack - ADC [80, -1.5C] 
  //read values for warm body - ADC [581, 29.0C]  
  //fitting to linear plot gives m = .0617, c = -6.2320
  //With different power supply
  // room temp [410, 22.6]
  // body temp [598 , 34.4]
  // ice       [106 , 0]
  if(millis() - previousMillis >= avgtime)    // Check if it is time to process the data and print to serial
  {
    sampleVal = rawVal/count;                 // Average the collected values to obtain an averaged ADC reading
    //Serial.println(sampleVal);                 // For Debugging
    //Serial.println(count);
    correctedVal = sampleVal; 
    temp = correctedVal*m + c;                   // converting ADC value into Celcius
    count =0;                                 // Resets the variables for next cycle of averaging
    rawVal =0;
    Serial.println(temp);                     // Print the temperature in Celcius onto serial. 
    previousMillis = millis();                // Resets the timer for the next cycle of sampling.
        
  }
}
