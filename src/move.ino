#include <stdint.h>
#include <stdlib.h>
#include <Servo.h> 
#include <Wire.h>
#include <LIDARLite_v3HP.h>
#include <Stepper.h>
int pos = 30;
int inc = 1;
const int stepsPerRevolution = 200;
int stepperIndex;

Servo myservo;  // create servo object to control a servo 
LIDARLite_v3HP myLidarLite;

void setup() 
{ 

//  Serial.begin(115200);
//  myservo.attach(8);  // attaches the servo on GIO2 to the servo object 
//  pinMode(4, OUTPUT); //Direction
//  pinMode(5, OUTPUT); //Step

    uint8_t dataByte;

    // Initialize Arduino serial port (for display of ASCII output to PC)
    Serial.begin(115200);

    // Initialize Arduino I2C (for communication to LidarLite)
    Wire.begin();
    #ifdef FAST_I2C
        #if ARDUINO >= 157
            Wire.setClock(400000UL); // Set I2C frequency to 400kHz (for Arduino Due)
        #else
            TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
        #endif
    #endif

    // Configure the LidarLite internal parameters so as to lend itself to
    // various modes of operation by altering 'configure' input integer to
    // anything in the range of 0 to 5. See LIDARLite_v3HP.cpp for details.
    myLidarLite.configure(0);
} 

void loop() 
{ 
  uint16_t distance;
  uint8_t newDistance = 0;

//  if (stepperIndex > 6400) {
//    stepperIndex = 0;
//  }
//
//  int origIndex = stepperIndex; //keeping track of the index of the stepper motor before every loop()
//
//  digitalWrite(4, HIGH);
//  for (stepperIndex; stepperIndex < origIndex + 24; stepperIndex++) {
//    digitalWrite(5, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(5, LOW);
//    delayMicroseconds(500);
//  }


  if (stepperIndex % 24 == 0) {
    myservo.write(pos);
  } 
  inc = (pos == 90) ? -1 : inc;
  inc = (pos == 0) ? 1 : inc;
  pos += inc; // tell servo to go to position in variable 'pos' 
  newDistance = distanceContinuous(&distance);
  Serial.println(distance);
  //myStepper.step(stepsPerRevolution*1);
  delay(15);
}

uint8_t distanceContinuous(uint16_t * distance)
{
    uint8_t newDistance = 0;

    // Check on busyFlag to indicate if device is idle
    // (meaning = it finished the previously triggered measurement)
    if (myLidarLite.getBusyFlag() == 0)
    {
        // Trigger the next range measurement
        myLidarLite.takeRange();

        // Read new distance data from device registers
        *distance = myLidarLite.readDistance();

        // Report to calling function that we have new data
        newDistance = 1;
    }

    return newDistance;
}
