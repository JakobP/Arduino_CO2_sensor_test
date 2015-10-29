// Functions for getting data from sensors and determining which leds to light



/**
 * @return Float with CO2 value as PPM (parts per millie)
 */
float getCo2(){
  float ppm = gasSensor.getPPM();
  return ppm;
}

/**
 * @return Float humidity as percentage
 */
float getHumidity(){
  float humidity = dht.readHumidity();
  return humidity;
}

/**
 * @return Float temperature in celcius
 */
float getTemperature(){
  float temperature = dht.readTemperature();
  return temperature;
}

/**
 * @return String (binary) with code for CO2 leds
 */
String updateLedsCo2(float value){
  //Serial.println("Updating CO2 Leds"); 
  if(value<1500){
    Serial.println("CO2 below 1000");
    return "100";
  }
  if(value>=1500 && value<3000){
    Serial.println("CO2 between 2000 and 3000");
    return "010";
  }
  if(value>=3000){
    Serial.println("CO2 above 3000");
    return "001";
  }
}

/**
 * @return String (binary) with code for temperature leds
 */
 String updateLedsTemperature(float value){
  //Serial.println("Updating Temperature Leds"); 
  if(value<21){
    Serial.println("Temperature below 21");
    return "100";
  }
  if(value>=21 && value<22){
    Serial.println("Temperature between 24 and 25");
    return "010";
  }

  if(value>=22){
    Serial.println("Temperature above 22");
    return "001";
  }
}

/**
 * @return String (binary) with code for humidity leds
 */
String updateLedsHumidity(float value){
  //Serial.println("Updating Humidity Leds"); 
  if(value>40 && value<60){
    Serial.println("Humidity between 40-60");
    return "100";
  }
  if((value>30 && value<40) || (value>60 && value<70)){
    Serial.println("Humidity btween 30-40 OR  60-70");
    return "010";
  }
  if(value<30 || value>70){
    Serial.println("Humidity below 30 OR above 70");
    return "001";
  }
}
