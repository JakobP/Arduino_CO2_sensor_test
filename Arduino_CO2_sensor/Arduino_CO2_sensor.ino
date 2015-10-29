// Based on this: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library
// MQ135 library by Georg Krocker: https://github.com/GeorgK/MQ135
// Technical explanation of how MQ135 can measure CO2: http://davidegironi.blogspot.com/2014/01/cheap-co2-meter-using-mq135-sensor-with.html#.ViqEAxArIUE
// RunningAverage library by Rob Tillaart: http://playground.arduino.cc/Main/RunningAverage
// How to connect MQ135 sensor: https://www.youtube.com/watch?v=BIf_mpnsZvY
// DHT22 code is based on File>Examples>DHT Sensor Library (the DHT Sensor Library from Adafruit)

// CALIBRATION
// When running, you should wait 30-60 minutes until the value has settled, then get the RZero using gasSensor.getRZero() and define it.

#include <MQ135.h>
#include <RunningAverage.h>
#include "DHT.h"

//DHT22 config
#define DHTTYPE DHT22   // Senor type: DHT 22  (AM2302), AM2321
#define DHTPIN 2     // Connected to digital PIN 2
DHT dht(DHTPIN, DHTTYPE); // Initialize sensor

//MQ135.h config
MQ135 gasSensor = MQ135(0);
float rzero = gasSensor.getRZero(); //When setting up, get the rzero value and change it IN THE MQ135.h LIBRARY. Each sensor is different and this makes the PPM calculation work.


//RunningAverage.h variables
int averageSamples = 10;  // Set the number of samples to be used for running averages
RunningAverage averageCo2(averageSamples);
RunningAverage averageTemperature(averageSamples);
RunningAverage averageHumidity(averageSamples);
int samples = 0;



void setup() {
  Serial.begin(9600);   // sets the serial port to 9600
  dht.begin();          // Not sure what this does. Some sort of initialization of the DHT22 temperature/humidity sensor
  
  //Ensures that we are starting with empty running averages
  averageCo2.clear();
  averageTemperature.clear();
  averageHumidity.clear();

  delay(5000);
}

void loop() {
  // Get the sensor readings
  float co2 = getCo2();                 // PPM
  float temperature = getTemperature(); // Degrees Celcius
  float humidity = getHumidity();       // Percentage

  //Add the sensorvalues to running averages
  averageCo2.addValue(co2);
  averageTemperature.addValue(temperature);
  averageHumidity.addValue(humidity);

  //Get running average of the sensor data
  float averageCo2Float = averageCo2.getAverage();
  float averageTemperatureFloat = averageTemperature.getAverage();
  float averageHumidityFloat = averageHumidity.getAverage();

  // Print the current calues
  Serial.print("Avg. CO2:\t");
  Serial.print(averageCo2Float);
  Serial.print("\t");
  Serial.print("Avg. temperature:\t");
  Serial.print(averageTemperatureFloat);
  Serial.print("\t");
  Serial.print("Avg. humidity:\t");
  Serial.println(averageHumidityFloat);

  // Increase the running average sample count
  samples++;

    
  //Reset runningAverage to 0 after 300 readings
  if (samples == averageSamples)
  {
    samples = 0;
    averageCo2.clear();
    averageTemperature.clear();
    averageHumidity.clear();
    Serial.print("RESET Running Averages");
  }

  Serial.print(updateLedsCo2(averageCo2Float));
  Serial.print(updateLedsTemperature(averageTemperatureFloat));
  Serial.print(updateLedsHumidity(averageHumidityFloat));
  
  delay(2000);
}


