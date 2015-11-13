/**
 * @Param String message to write to the log
 */
void writeToLog(String message)
{
  logWriter = SD.open("log.csv", FILE_WRITE);
   if (logWriter)
   {
     int seconds = (millis() + 500) / 1000;
     //delay(250); //Pause between readings.
     logWriter.print(seconds);                           //seconds since program started
     logWriter.print(";");                               //write a commma
     logWriter.println(message);                         //message + newline
     logWriter.close();                                  //close the file
     Serial.println("Wrote message to log: "+message);
  }
}
