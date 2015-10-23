// Based on this: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library
// MQ135 library by Georg Krocker: https://github.com/GeorgK/MQ135
// RunningAverage library by Rob Tillaart: http://playground.arduino.cc/Main/RunningAverage
// How to connect MQ135 sensor: https://www.youtube.com/watch?v=BIf_mpnsZvY

// CALIBRATION
// When running, you should wait 30-60 minutes until the value has settled, then get the RZero using gasSensor.getRZero() and define it.

#include <MQ135.h>
#include <RunningAverage.h>

//MQ135.h config
MQ135 gasSensor = MQ135(0);
float rzero = gasSensor.getRZero();
//#define RZERO 76.63

//RunningAverage.h config
RunningAverage runningAverage(10);
int samples = 0;


void setup() {
  Serial.begin(9600);      // sets the serial port to 9600

  runningAverage.clear(); // explicitly start RunningAverageclean
}

void loop() {

  // Get and echo CO2 PPM
  float ppm = gasSensor.getPPM();
  Serial.print("Current PPM reading: ");
  Serial.println(ppm);

  // Calculate and echo running average of PPM. Only trigger on readings above 200 PPM to sort out bad sensor data
  // Since we measure indoors we can be pretty sure that the CO2 level is above 300 PPM, which is what you would expect outdoors in the countryside. Cities tend to have around 500 PPM outside.
  if(ppm>200)
  {
    runningAverage.addValue(ppm);  
    Serial.print("Running Average: ");
    Serial.println(runningAverage.getAverage(), 0);
    samples++;
  }
  
  //Reset runningAverage to 0 after 300 readings
  if (samples == 300)
  {
    samples = 0;
    runningAverage.clear();
    Serial.print("RESET Running Average");
  }

  delay(1000);
}
