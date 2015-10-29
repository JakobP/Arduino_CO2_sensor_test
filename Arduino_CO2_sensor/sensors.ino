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
String getLedsCo2(float value){
  String ledCode = "";
  if(value<1500){
    ledCode = "100";
    Serial.println("CO2 below 1500 - ledCode: "+ledCode);
    return ledCode;
  }
  if(value>=1500 && value<3000){
    ledCode = "010";
    Serial.println("CO2 between 1500 and 3000 - ledCode: "+ledCode);
    return "010";
  }
  if(value>=3000){
    ledCode = "001";
    Serial.println("CO2 above 3000 - ledCode: "+ledCode);
    return ledCode;
  }
}

/**
 * @return String (binary) with code for temperature leds
 */
String getLedsTemperature(float value){
  String ledCode = "";
  if(value<21){
    ledCode = "100";
    Serial.println("Temperature below 21 - ledCode: "+ledCode);
    return ledCode;
  }
  if(value>=21 && value<22){
    ledCode = "010";
    Serial.println("Temperature between 24 and 25 - ledCode: "+ledCode);
    return ledCode;
  }

  if(value>=22){
    ledCode = "001";
    Serial.println("Temperature above 22 - ledCode: "+ledCode);
    return ledCode;
  }
}

/**
 * @return String (binary) with code for humidity leds
 */
String getLedsHumidity(float value){
  String ledCode = "";

  if(value>40 && value<60){
    ledCode = "100";
    Serial.println("Humidity between 40-60 - ledCode: " +ledCode);
    return ledCode;
  }
  if((value>30 && value<40) || (value>60 && value<70)){
    ledCode = "010";
    Serial.println("Humidity btween 30-40 OR  60-70 - ledCode: " + ledCode);
    return "010";
  }
  if(value<30 || value>70){
    ledCode = "001";
    Serial.println("Humidity below 30 OR above 70 - LED: " + ledCode);
    return ledCode;
  }
}
