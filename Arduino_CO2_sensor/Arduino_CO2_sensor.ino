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
//float rzero = gasSensor.getRZero(); //When setting up, get the rzero value and change it IN THE MQ135.h LIBRARY. Each sensor is different and this makes the PPM calculation work.

//RunningAverage.h variables
RunningAverage averageCo2(300);
RunningAverage averageTemperature(300);
RunningAverage averageHumidity(300);
int samples = 0;


void setup() {
  Serial.begin(9600);      // sets the serial port to 9600
  dht.begin(); // Not sure what this does

  // Ensures that we are starting with empty running averages
  averageCo2.clear();
  averageTemperature.clear();
  averageHumidity.clear();
}

void loop() {

  // Functions for priting lots of data
  //co2Print();
  //dhtPrint();
  


  // Get the values
  float co2 = getCo2();                 // PPM
  float temperature = getTemperature(); // Degrees Celcius
  float humidity = getHumidity();       // Percentage

  Serial.print("CO2 PPM:\t");
  Serial.print(co2);
  Serial.print("\t");
  Serial.print("Temperature:\t\t");
  Serial.print(temperature);
  Serial.print("\t");
  Serial.print("Humidity:\t");
  Serial.println(humidity);

  //Add the values to running averages
  averageCo2.addValue(co2);
  averageTemperature.addValue(temperature);
  averageHumidity.addValue(humidity);

  Serial.print("Avg. CO2:\t");
  Serial.print(averageCo2.getAverage(), 0);
  Serial.print("\t");
  Serial.print("Avg. temperature:\t");
  Serial.print(averageTemperature.getAverage(), 0);
  Serial.print("\t");
  Serial.print("Avg. humidity:\t");
  Serial.println(averageHumidity.getAverage(), 0);

  // Increase the sample count
  samples++;
  
  
  //Reset runningAverage to 0 after 300 readings
  if (samples == 300)
  {
    samples = 0;
    averageCo2.clear();
    Serial.print("RESET Running Average");
  }

  delay(1000);
}



float getCo2(){
  float ppm = gasSensor.getPPM();
  return ppm;
}

float getHumidity(){
  float humidity = dht.readHumidity();
  return humidity;
}

float getTemperature(){
  float temperature = dht.readTemperature();
  return temperature;
}


 
/* Print CO2 PPM */
void co2Print(){
  float ppm = gasSensor.getPPM();
  Serial.print("CO2 PPM: ");
  Serial.println(ppm);
}

/* Get and print temperature and humidity from DHT22 */
void dhtPrint(){
// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
}

