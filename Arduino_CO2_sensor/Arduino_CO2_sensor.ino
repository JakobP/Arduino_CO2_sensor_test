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
int averageSamples = 10;
RunningAverage averageCo2(averageSamples);
RunningAverage averageTemperature(averageSamples);
RunningAverage averageHumidity(averageSamples);
int samples = 0;

//LED config
int co2Led3 = 3; // green
int co2Led4 = 4; // yellow
int co2Led5 = 5; // red
int temperatureLed6 = 6; // green
int temperatureLed7 = 7; // yellow
int temperatureLed8 = 8; // red
int humidityLed9 = 9; // green
int humidityLed10 = 10; // yellow
int humidityLed11 = 11; // red


void setup() {
  Serial.begin(9600);      // sets the serial port to 9600
  dht.begin(); // Not sure what this does

  // Setup of LEDs
  pinMode(co2Led3, OUTPUT);
  pinMode(co2Led4, OUTPUT);
  pinMode(co2Led5, OUTPUT);
  pinMode(temperatureLed6, OUTPUT);
  pinMode(temperatureLed7, OUTPUT);
  pinMode(temperatureLed8, OUTPUT);
  pinMode(humidityLed9, OUTPUT);
  pinMode(humidityLed10, OUTPUT);
  pinMode(humidityLed11, OUTPUT);

  // Power all leds for 5 seconds, so we can see that they work
  digitalWrite(co2Led3, HIGH);
  digitalWrite(co2Led4, HIGH);
  digitalWrite(co2Led5, HIGH);
  digitalWrite(temperatureLed6, HIGH);
  digitalWrite(temperatureLed7, HIGH);
  digitalWrite(temperatureLed8, HIGH);
  digitalWrite(humidityLed9, HIGH);
  digitalWrite(humidityLed10, HIGH);
  digitalWrite(humidityLed11, HIGH);

  delay(5000);
  
  // Ensures that we are starting with empty running averages
  averageCo2.clear();
  averageTemperature.clear();
  averageHumidity.clear();
}

void loop() {
  // Get the values
  float co2 = getCo2();                 // PPM
  float temperature = getTemperature(); // Degrees Celcius
  float humidity = getHumidity();       // Percentage

/*
  Serial.print("CO2 PPM:\t");
  Serial.print(co2);
  Serial.print("\t");
  Serial.print("Temperature:\t\t");
  Serial.print(temperature);
  Serial.print("\t");
  Serial.print("Humidity:\t");
  Serial.println(humidity);
*/
  //Add the values to running averages
  averageCo2.addValue(co2);
  averageTemperature.addValue(temperature);
  averageHumidity.addValue(humidity);

  float averageCo2Float = averageCo2.getAverage();
  float averageTemperatureFloat = averageTemperature.getAverage();
  float averageHumidityFloat = averageHumidity.getAverage();


  Serial.print("Avg. CO2:\t");
  Serial.print(averageCo2Float);
  Serial.print("\t");
  Serial.print("Avg. temperature:\t");
  Serial.print(averageTemperatureFloat);
  Serial.print("\t");
  Serial.print("Avg. humidity:\t");
  Serial.println(averageHumidityFloat);

  // Increase the sample count
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

/*
  digitalWrite(co2Led3, HIGH);
  digitalWrite(co2Led4, HIGH);
  digitalWrite(co2Led5, HIGH);
  digitalWrite(temperatureLed6, HIGH);
  digitalWrite(temperatureLed7, HIGH);
  digitalWrite(temperatureLed8, HIGH);
  digitalWrite(humidityLed9, HIGH);
  digitalWrite(humidityLed10, HIGH);
  digitalWrite(humidityLed11, HIGH);
*/
  updateLedsCo2(averageCo2Float);
  updateLedsTemperature(averageTemperatureFloat);
  updateLedsHumidity(averageHumidityFloat);
  
  delay(2000);
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

void updateLedsCo2(float value){
  //Serial.println("Updating CO2 Leds"); 
  if(value<1500){
    Serial.println("CO2 below 1000");
    digitalWrite(co2Led3, HIGH);
    digitalWrite(co2Led4, LOW);
    digitalWrite(co2Led5, LOW);
  }
  if(value>=1500 && value<3000){
    Serial.println("CO2 between 2000 and 3000");
    digitalWrite(co2Led3, LOW);
    digitalWrite(co2Led4, HIGH);
    digitalWrite(co2Led5, LOW);
  }
  if(value>=3000){
    Serial.println("CO2 above 3000");
    digitalWrite(co2Led3, LOW);
    digitalWrite(co2Led4, LOW);
    digitalWrite(co2Led5, HIGH);
  }
}

void updateLedsTemperature(float value){
  //Serial.println("Updating Temperature Leds"); 
  if(value<21){
    Serial.println("Temperature below 21");
    digitalWrite(temperatureLed6, HIGH);
    digitalWrite(temperatureLed7, LOW);
    digitalWrite(temperatureLed8, LOW);
  }
  if(value>=21 && value<22){
    Serial.println("Temperature between 24 and 25");
    digitalWrite(temperatureLed6, LOW);
    digitalWrite(temperatureLed7, HIGH);
    digitalWrite(temperatureLed8, LOW);
  }

  if(value>=22){
    Serial.println("Temperature above 22");
    digitalWrite(temperatureLed6, LOW);
    digitalWrite(temperatureLed7, LOW);
    digitalWrite(temperatureLed8, HIGH);
  }
}

void updateLedsHumidity(float value){
  //Serial.println("Updating Humidity Leds"); 
  if(value>40 && value<60){
    Serial.println("Humidity between 40-60");
    digitalWrite(humidityLed9, HIGH);
    digitalWrite(humidityLed10, LOW);
    digitalWrite(humidityLed11, LOW);
  }
  if((value>30 && value<40) || (value>60 && value<70)){
    Serial.println("Humidity btween 30-40 OR  60-70");
    digitalWrite(humidityLed9, LOW);
    digitalWrite(humidityLed10, HIGH);
    digitalWrite(humidityLed11, LOW);
  }
  if(value<30 || value>70){
    Serial.println("Humidity below 30 OR above 70");
    digitalWrite(humidityLed9, LOW);
    digitalWrite(humidityLed10, LOW);
    digitalWrite(humidityLed11, HIGH);
  }
}


