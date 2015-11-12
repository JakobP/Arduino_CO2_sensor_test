/**
 * Set a fastled led strip to a specific color. Hardcoded.
 * @param CRGB the color that the leds should be set to. Looks like this CRGB::Black . You can see it in the fasled examples.
 */
void setAllLedds(CRGB color)
{
  leds[0]  = color;
  leds[1]  = color;
  leds[2]  = color;
  leds[3]  = color;
  leds[4]  = color;
  leds[5]  = color;
  leds[6]  = color;
  leds[7]  = color;
  leds[8]  = color;
  leds[9]  = color;
  leds[10] = color;
  leds[11] = color;
  leds[12] = color;
  leds[13] = color;
  leds[14] = color;
  leds[15] = color;
  leds[16] = color;
  leds[17] = color;

  FastLED.show();
}

/**
 * Apple style "breathing led" - handy for showing that the box is on and the loop is running
 * Using example from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
 */
void ledBreathe(int ledPin){
  float val = (exp(sin(millis()/10000.0*PI)) - 0.36787944)*108.0;
  analogWrite(ledPin, val);
  }


