/* ENSC 475 Lab 3 - Pulse Sensor
    Date: 27 March 2018
    Authors: Isaac Cheng Hui Tan
             Nathanael Davis Tan

    Version 1.0

    This code takes in values from the arduino pulse sensor, calibrattes for 5 seconds which finds the max and min values, 
    sets the threshold values then proceeds to count the pulses and gives beats per min.

*/

  int state = 1;                                // State machine State
  unsigned long currentTime = millis();         // current time of machine
  unsigned int caliTime = 5000;                 // Calibration state time length (ms)
  int inputPin = 3;                             // pin input of arduino for ADC
  int readValue = 0;                            // Value read from ADC
  int highVal = 0;                              // 10 bit ADC, min and max readings during calibration
  int lowVal = 1024;
  int variation =  0;                           // Difference between lowVal and highVal values
  int highThresh = 0;                           // Threshold Values set during calibration
  int lowThresh = 0;
  unsigned long startPulseTime = 0;             // Time at which the pulse starts
  unsigned long endPulseTime = 0;               // Time at which the pulse ends
  unsigned long totalPulseLength = 0;           // Total time of all pulses detected since last update
  int pulseCount = 0;                           // Total number of pulses dectected since last update
  int pulseLength = 0;                          // Avg length of pulse
  float beatsPM = 0;                            // BPM of signal
  unsigned long bpmUpdateTimer = 2000;          // Time between BPM updates
  unsigned long lastUpdateTime = 0;             // Time last updated BPM
  bool lowFoundFlag = false;                    // Flag if a previous low was found
  bool breakFlag = false;                       // If a break is called.
  
void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis()- lastUpdateTime) > bpmUpdateTimer) // Soft timer for updating of the BPM through serial.
  {
    // Serial.print(pulseLength);
    pulseLength = totalPulseLength/pulseCount;    // Calculations for BPM
    beatsPM = (1/(float(pulseLength)/1000))*60;
    Serial.print("Beats Per Minute: ");           // Prints to Serial
    Serial.print(beatsPM);
    Serial.print("\n");
    pulseCount =0;                                // Resets the counter variables
    totalPulseLength =0;
    lastUpdateTime =millis();
  }
  
  switch (state) {
    case 1:                                       // Calibration State
      Serial.print("Calibrating \n");
      pulseCount = 0;                             // Reset Variables
      highVal = 0;
      lowVal = 1024;
      currentTime = millis();
      while ((millis() - currentTime) < caliTime) // Sampling for "caliTime" milliseconds (amount of time to stay in calibration state)
      {
        readValue =  analogRead(inputPin);        // Read from ADC
        //Serial.print("Reading\n");
        if (readValue > highVal)                  // Checks and updates for maximum and minimum readings
        {
          highVal = readValue;
        }
        if (readValue < lowVal)
        {
          lowVal = readValue;
        }
        
      }
      state = 2;
      break;
    case 2:                                       // Calibrated State
      Serial.print("Calibrated \n");
      variation = highVal - lowVal;               // Finds the difference between the highest and lowest measured values from ADC
      lowThresh = lowVal + (variation*0.3);       // Sets the thresholds based on 30% of the difference.
      highThresh = highVal - (variation*0.3);
      Serial.print("Low: ");
      Serial.print(lowThresh);
      Serial.print("\nHigh: ");
      Serial.print(highThresh);
      Serial.print("\n");
      state = 3;
      break;
    case 3:                                       // Finding High State
      //Serial.print("Finding High\n");
      highVal =0;
      breakFlag = false;
      currentTime = millis();
      while ((millis() - currentTime) < caliTime) // Sample for "caliTime" milliseconds at most
      {
        readValue =  analogRead(inputPin);        // Read from ADC
        /*if (readValue > highVal)                  // for Debugging to show the highest value
        {
          highVal = readValue;
        }
        */
        if (readValue > highThresh)               // if the sampled value is higher than the threshold
        {
          if(lowFoundFlag)                        // if there was previously a value lower than the low threshold found
          {                                       // in other words, a cycle has past since the last high value
            endPulseTime = millis();
            totalPulseLength = totalPulseLength+ (endPulseTime - startPulseTime); // record the time taken since last pulse
            pulseCount++;                         // count the pulses
            // Serial.print(pulseLength);
          }
          
          startPulseTime = millis();
          //Serial.print("High Found\n");
          state = 4;                              // go to "Finding low state"
          breakFlag = true;
          break;
          
        }
      }
      
      /*Serial.print("Highest Value: ");
      Serial.print(highVal);
      Serial.print("\n");*/
      if(breakFlag)
      {
        state = 4;
      }
      else
      {
        state = 1;                                  // else if no high value found, go back to calibration state
      }
      break;
    case 4:                                       // Finding Low State
      //Serial.print("Finding Low State\n");
      breakFlag = false;
      lowFoundFlag = false;                       // Reset flags
      currentTime = millis();
      while(((millis() - currentTime) < caliTime))// Sample for "caliTime" milliseconds.
      {
        readValue = analogRead(inputPin);         // Read from ADC 
        if (readValue < lowThresh)                // if sampled value is lower than the threshold
        {
          state = 3;                              // go to high state.
          lowFoundFlag = true;                    // tell high state that there was a low found.
          breakFlag = true;
          break;
        }
      }
      if(breakFlag)
      {
        state = 3;
      }
      else
      {
        state = 1;                                  // if no low value is found, go back to calibration
      }
      break;
    default: //Something wrong happened state.
      break;
  }
}
