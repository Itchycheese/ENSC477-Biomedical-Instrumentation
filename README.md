# ENSC477-Biomedical-Instrumentation
Usage of Arduino to create various physiological measurement instruments

There are three labs that involve creation of new code for this course.

Lab 1  - Thermocouple
                -involves the creation a an algorthim to show the current temperature of a J type Thermocouple

Lab 3 has two pieces of code.
      - Pulse Sensor 
                - Creation of a Finite State Machine that shows the beats per minute of a Red Diode based Optical Blood Flow and 
                     Absorption measurement Sensor
                     The Finite State Machine has 4 States
                            - Initiate Calibration, The starting state and error state.
                            - Calibration, the machine finds the max and min values for 5 seconds and sets a threshold at 30% and 70%
                            - Detection of a High State, system waits for the measured value to reach the 70% max threshold.
                            - Detection of a Low State, system waits for the measured value to reach the 30% min threshold.
      - Sampling Rate 
                - Creation of a simple program that outputs the sampled data at a certain rate through serial for in a CSV Format.
                        This allows us to view waveform distortion when a certain sampling rate is used.

Lab 5 - Pulse Display
                - This code takes in values from the arduino ADC and prints it to serial for display in CSV Format
                            
