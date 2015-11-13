// Based on this: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library
// MQ135 library by Georg Krocker: https://github.com/GeorgK/MQ135
// Technical explanation of how MQ135 can measure CO2: http://davidegironi.blogspot.com/2014/01/cheap-co2-meter-using-mq135-sensor-with.html#.ViqEAxArIUE
// RunningAverage library by Rob Tillaart: http://playground.arduino.cc/Main/RunningAverage
// How to connect MQ135 sensor: https://www.youtube.com/watch?v=BIf_mpnsZvY
// DHT22 code is based on File>Examples>DHT Sensor Library (the DHT Sensor Library from Adafruit)

// CALIBRATION
// When running, you should wait 30-60 minutes until the value has settled, then get the RZero using gasSensor.getRZero() and define it.
#include<stdlib.h> // Converting floats to string

#include <MQ135.h>      // CO2 sensor
#include "DHT.h"        // Temperature + humidity sensor
#include "FastLED.h"    // Led strips
#include <math.h>       // For Apple style breathing led

//DHT22 config
#define DHTTYPE DHT22   // Senor type: DHT 22  (AM2302), AM2321
#define DHTPIN 2     // Connected to digital PIN 2
DHT dht(DHTPIN, DHTTYPE); // Initialize sensor

//MQ135.h config
MQ135 gasSensor = MQ135(0);
//float rzero = gasSensor.getRZero(); //When setting up, get the rzero value and change it IN THE MQ135.h LIBRARY. Each sensor is different and this makes the PPM calculation work.

// Fastled config
#define NUM_LEDS 18 // Number of leds in strip

// Breathing led config
// #define BREATHING_LED_PIN 9

// Initialize the array of leds
CRGB leds[NUM_LEDS];

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 24
#define CLOCK_PIN 22


//RunningAverage.h variables
#include <RunningAverage.h>

int averageSamples = 5;  // Set the number of samples to be used for running averages
RunningAverage averageCo2(averageSamples);
RunningAverage averageTemperature(averageSamples);
RunningAverage averageHumidity(averageSamples);
int samples = 0;


// SD CARD config
#include <SD.h> //Load SD card library
#include<SPI.h> //Load SPI Library

int chipSelect = 53; //chipSelect pin for the SD card Reader
File logWriter; //Data object for logging

/*
 * GND to GND
 * CS (Chip select) to 10 UNO / 53 MEGA
 * MOSI to 11 UNO / 51 MEGA
 * CLK (SCK) to 13 UNO / 52 MEGA
 * MISO to 12 UNO / 50 MEGA
*/

void setup() {
  Serial.begin(9600);

  //pinMode(BREATHING_LED_PIN, OUTPUT);

  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS); // Init leds
  
  dht.begin();          // Not sure what this does. Some sort of initialization of the DHT22 temperature/humidity sensor
  
  //Ensures that we are starting with empty running averages
  averageCo2.clear();
  averageTemperature.clear();
  averageHumidity.clear();
  
  // Begin writing to SD card
  pinMode(53,OUTPUT);
  SD.begin(53); //Initialize the SD card reader
  writeToLog("*** STARTING NEW LOG ***");
  writeToLog("CO2; Temperature; Humidity;RZERO");
  
  setAllLedds(CRGB::Green);
  delay(500); 
  setAllLedds(CRGB::Black);
  delay(2000);
}

void loop() {

  // Show the rzero - this is important for configuration of the MQ135 library
  Serial.print("rzero: ");
  Serial.println(gasSensor.getRZero());
  
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


  char buff[10]; // Buffer for converting float to string for the 
  String avgCo2String = dtostrf(averageCo2Float, 1, 2, buff);  //1 is mininum width, 2 is characters after decimal point
  String avgHumidityString = dtostrf(averageHumidityFloat, 1, 2, buff);
  String avgTemperatureString = dtostrf(averageTemperatureFloat, 1, 2, buff);
  String rzeroString = dtostrf(gasSensor.getRZero(), 1, 2, buff);
  
  
  // Write data to log
  writeToLog(avgCo2String + ";" + avgTemperatureString + ";" + avgHumidityString + ";" + rzeroString);

  // Increase the running average sample count
  samples++;

    
  //Reset runningAverage
  if (samples >= averageSamples)
  {
    samples = 0;
    averageCo2.clear();
    averageTemperature.clear();
    averageHumidity.clear();
    Serial.println("RESET Running Averages");
  }

  setLedsCo2(averageCo2Float);
  setLedsTemperature(averageTemperatureFloat);
  setLedsHumidity(averageHumidityFloat);

  
  delay(5000);

  // Makes leds blink, so you can see that the code arduino is not frozen.
  //setAllLedds(CRGB::Black);
  //delay(1);

  //Make single led "breathe" so you can see that the code is not frozen
  //ledBreathe(BREATHING_LED_PIN);
}
