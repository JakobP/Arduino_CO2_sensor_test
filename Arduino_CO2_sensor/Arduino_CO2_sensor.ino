// Based on this: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library
// MQ135 library by Georg Krocker: https://github.com/GeorgK/MQ135
// Technical explanation of how MQ135 can measure CO2: http://davidegironi.blogspot.com/2014/01/cheap-co2-meter-using-mq135-sensor-with.html#.ViqEAxArIUE
// RunningAverage library by Rob Tillaart: http://playground.arduino.cc/Main/RunningAverage
// How to connect MQ135 sensor: https://www.youtube.com/watch?v=BIf_mpnsZvY
// DHT22 code is based on File>Examples>DHT Sensor Library (the DHT Sensor Library from Adafruit)

// CALIBRATION
// When running, you should wait 30-60 minutes until the value has settled, then get the RZero using gasSensor.getRZero() and define it.
#include<stdlib.h> // Converting floats to string

#include <MQ135.h>
#include "DHT.h"

//DHT22 config
#define DHTTYPE DHT22   // Senor type: DHT 22  (AM2302), AM2321
#define DHTPIN 2     // Connected to digital PIN 2
DHT dht(DHTPIN, DHTTYPE); // Initialize sensor

//MQ135.h config
MQ135 gasSensor = MQ135(0);
float rzero = gasSensor.getRZero(); //When setting up, get the rzero value and change it IN THE MQ135.h LIBRARY. Each sensor is different and this makes the PPM calculation work.


//RunningAverage.h variables
#include <RunningAverage.h>

int averageSamples = 10;  // Set the number of samples to be used for running averages
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

// SHIFT REGISTER config
//Pin connected to ST_CP of 74HC595
int latchPin = 3;
//Pin connected to SH_CP of 74HC595
int clockPin = 5;
////Pin connected to DS of 74HC595
int dataPin = 4;

//holders for infromation you're going to pass to shifting function
byte dataOne;
byte dataTwo;
byte dataArrayOne[10];
byte dataArrayTwo[10];


void setup() {
  pinMode(latchPin, OUTPUT); // FOr shift register. It is used in the loop
  Serial.begin(9600);
  
  dht.begin();          // Not sure what this does. Some sort of initialization of the DHT22 temperature/humidity sensor
  
  //Ensures that we are starting with empty running averages
  averageCo2.clear();
  averageTemperature.clear();
  averageHumidity.clear();

  //function that blinks all the LEDs
  //gets passed the number of blinks and the pause time
  blinkAll_2Bytes(1,500);

  // Begin writing to SD card
  pinMode(53,OUTPUT);
  SD.begin(53); //Initialize the SD card reader
  writeToLog("*** STARTING NEW LOG ***");
  
  delay(5000);
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
  String co2Test = dtostrf(averageCo2Float, 1, 2, buff);  //1 is mininum width, 2 is characters after decimal point
  String humidityTest = dtostrf(averageCo2Float, 1, 2, buff);
  String temperatureTest = dtostrf(averageCo2Float, 1, 2, buff);
  // Write data to log
  writeToLog("Avg. CO2;"+ co2Test + ";" + "Avg. temperature;" + temperatureTest + "Avg. humidity;" + humidityTest);

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

  // Setting LEDs for shift register
  String temperatureBinaryString  = getLedsTemperature(averageTemperatureFloat);
  String humidityBinaryString     = getLedsHumidity(averageHumidityFloat);
  String co2BinaryString          = getLedsCo2(averageCo2Float);
  String paddingBinaryString      = "0000000"; // This is the wires that can be connected to extra leds.
  
  String combinedBinaryStringSixteen  = temperatureBinaryString + humidityBinaryString + co2BinaryString + paddingBinaryString;

  // Send the data for lighting the connect leds to the shift register
  updateShiftRegisterLeds(combinedBinaryStringSixteen);
  
  delay(2000);
}
