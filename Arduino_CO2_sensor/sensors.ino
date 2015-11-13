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
  float temperature = dht.readTemperature() - 2.2; // Temperature adjusted 2.2 degrees down, since it seems to consistently measure too high
  return temperature;
}

/**
 * @return String (binary) with code for humidity leds
 */
void setLedsHumidity(float value){

  if(value>40 && value<60){
    leds[0] = CRGB::Black;  //Top
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Green;
    leds[3] = CRGB::Green;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    Serial.println("Humidity between 40-60");
  }
  if(value<40){
    leds[0] = CRGB::Black;  //Top
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
    Serial.println("Humidity below 40");
  }
  if(value>60){
    leds[0] = CRGB::Red;  //Top
    leds[1] = CRGB::Red;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    Serial.println("Humidity above 60");
  }
  FastLED.show();
}

/**
 * @return String (binary) with code for temperature leds
 */
void setLedsTemperature(float value){
  if(value<18){
    leds[6] = CRGB::Blue; // Bottom
    leds[7] = CRGB::Black;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;  // Top
    Serial.println("Temperature below 18");
  }
  if(value>=18 && value<19){
    leds[6] = CRGB::Blue; // Bottom
    leds[7] = CRGB::Blue;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;  // Top
    Serial.println("Temperature between 18 and 19");
  }
  if(value>=19 && value<20){
    leds[6] = CRGB::Green; // Bottom
    leds[7] = CRGB::Green;
    leds[8] = CRGB::Green;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;  // Top
    Serial.println("Temperature between 19 and 20");
  }
  if(value>=20 && value<21){
    leds[6] = CRGB::Green; // Bottom
    leds[7] = CRGB::Green;
    leds[8] = CRGB::Green;
    leds[9] = CRGB::Green;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;  // Top
    Serial.println("Temperature between 20 and 21");
  }
  if(value>=21 && value<22){
    leds[6] = CRGB::Yellow; // Bottom
    leds[7] = CRGB::Yellow;
    leds[8] = CRGB::Yellow;
    leds[9] = CRGB::Yellow;
    leds[10] = CRGB::Yellow;
    leds[11] = CRGB::Black;  // Top
    Serial.println("Temperature between 21 and 22");
  }
  if(value>=22){
    leds[6] = CRGB::Red; // Bottom
    leds[7] = CRGB::Red;
    leds[8] = CRGB::Red;
    leds[9] = CRGB::Red;
    leds[10] = CRGB::Red;
    leds[11] = CRGB::Red;  // Top
    Serial.println("Temperature above 22");
  }
  FastLED.show();
}


/**
 * @return String (binary) with code for CO2 leds
 */
void setLedsCo2(float value){
  
  if(value<1200){
    leds[12] = CRGB::Black; // Top
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Black;
    leds[16] = CRGB::Green;
    leds[17] = CRGB::Green;
    Serial.println("CO2 below 1200");
  }
  if(value>=1200 && value<3000){
    leds[12] = CRGB::Black; // Top
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Yellow;
    leds[15] = CRGB::Yellow;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    Serial.println("CO2 between 1200 and 3000");
  }
  if(value>=3000){
    leds[12] = CRGB::Red; // Top
    leds[13] = CRGB::Red;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Black;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    Serial.println("CO2 above 3000");
  }
  FastLED.show();

}
