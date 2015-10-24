// Based on this: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library
// MQ135 library by Georg Krocker: https://github.com/GeorgK/MQ135
// Technical explanation of how MQ135 can measure CO2: http://davidegironi.blogspot.com/2014/01/cheap-co2-meter-using-mq135-sensor-with.html#.ViqEAxArIUE
// RunningAverage library by Rob Tillaart: http://playground.arduino.cc/Main/RunningAverage
// How to connect MQ135 sensor: https://www.youtube.com/watch?v=BIf_mpnsZvY

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
float rzero = gasSensor.getRZero();
//#define RZERO 76.63

//RunningAverage.h config
RunningAverage runningAverage(10);
int samples = 0;


void setup() {
  Serial.begin(9600);      // sets the serial port to 9600

  dht.begin(); // Not sure what this does
  
  runningAverage.clear(); // explicitly start RunningAverageclean
}

void loop() {


/*============ BEGIN MQ135 ===================*/
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
/*============ END MQ135 ===================*/

/*============ BEGIN DHT22 ===================*/
// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

/*============ END DHT22 ===================*/


  delay(1000);
}




