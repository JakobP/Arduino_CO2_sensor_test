//Based on http://www.toptechboy.com/arduino/arduino-lesson-21-log-sensor-data-to-an-sd-card/
#include <SD.h> //Load SD card library
#include<SPI.h> //Load SPI Library

int chipSelect = 10; //chipSelect pin for the SD card Reader
File logWriter; //Data object for logging
 
void setup(){
  Serial.begin(9600); //turn on serial monitor
  
  //pinMode(10, OUTPUT); // For SD card
  SD.begin(10); //Initialize the SD card reader
  writeToLog("*** STARTING NEW LOG ***");
}
 
void loop() {

  writeToLog("hello world");
 
}

void writeToLog(String message)
{
  logWriter = SD.open("log.csv", FILE_WRITE);
   if (logWriter)
   {
     delay(250); //Pause between readings.
     logWriter.print(millis());                          //miliseconds since program started
     logWriter.print(";");                               //write a commma
     logWriter.println(message);                         //message + newline
     logWriter.close();                                  //close the file
     Serial.println("Wrote message to log: "+message);
  }
}
