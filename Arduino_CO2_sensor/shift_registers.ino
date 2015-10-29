//**************************************************************//
//  Name    : shiftOutCode, Predefined Dual Array Style         //
//  Author  : Carlyn Maw, Tom Igoe                              //
//  Date    : 25 Oct, 2006                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                            //
//****************************************************************
// From https://www.arduino.cc/en/Tutorial/ShiftOut

// NEXT STEP - Consider turning the 2 arrays into 3. You just do a shiftOut() on each array, so it would be fairly easy. Then you can feed the right numbers into it.
// You may be able to do a binary string -> integer -> hex conversion to make the code easier to understand. See http://stackoverflow.com/questions/5759999/translating-a-string-containing-a-binary-value-to-hex

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

/**
 * Converts a binary string to Int
 * See http://stackoverflow.com/questions/5702931/convert-integer-decimal-to-hex-on-an-arduino
 * @param String a binary string
 * @return int the input binary string converted to an integer.
 */
 
int binaryStringToInt(String binaryString)
{
  char s[50];
  binaryString.toCharArray(s, 50) ;

  int value = 0;
  for (int i=0; i< strlen(s); i++)  // for every character in the string  strlen(s) returns the length of a char array
  {
    value *= 2; // double the result so far
    if (s[i] == '1') value++;  //add 1 if needed
  }
  
  return value;
}


/**
 * @Return reversed string
 */
String reverseString(String inputString)
{
  String output = "";
  
  //Using substring to reverse the string.
  for( int i = 0; i< inputString.length(); i++)
  {
    String currentValue = inputString.substring(inputString.length()-(i+1),inputString.length()-i);
    //Serial.println("Number is: "+currentValue);
    output = output + currentValue;
    //Serial.println("current output:"+output);
  }
  
  return output;
}

/**
 * @param String 16 character binary string with the sequence of LEDs to be lit/not lit.
 */
void updateShiftRegisterLeds(String binaryString)
{

  String combinedBinaryStringSixteen = binaryString;
  // SHIFT REGISTER
  // 16 character binary string
  //Serial.println("combinedBinaryStringSixteen: "+combinedBinaryStringSixteen);

  // 8 character binary string
  String shiftRegisterOneString = combinedBinaryStringSixteen.substring(0,8);
  String shiftRegisterTwoString = combinedBinaryStringSixteen.substring(8,16);
  //Serial.println("shiftRegisterOneString: " +shiftRegisterOneString);
  //Serial.println("shiftRegisterTWOString: " +shiftRegisterTwoString);

  //Reverse strings. This is to make them in the right order for the shiftout() function (I think) - this seems a bit weird...
  shiftRegisterOneString = reverseString(shiftRegisterOneString);
  shiftRegisterTwoString = reverseString(shiftRegisterTwoString);
  //Serial.println("Reversed shiftRegisterOneString: "+ shiftRegisterOneString);
  //Serial.println("Reversed shiftRegisterTwoString: "+ shiftRegisterTwoString);
  
  
  // Convert binary strings to integer
  int ShiftRegisterOneInt = binaryStringToInt(shiftRegisterOneString);
  int ShiftRegisterTwoInt = binaryStringToInt(shiftRegisterTwoString);

  dataArrayOne[0] = ShiftRegisterOneInt;
  //Serial.print("DataarrayOne: ");
  //Serial.println(dataArrayOne[0]);
  
  dataArrayTwo[0] = ShiftRegisterTwoInt;
  //Serial.print("DataarrayTwo: ");
  //Serial.println(dataArrayTwo[0]);

    //load the light sequence you want from array
    dataOne = dataArrayOne[0];
    dataTwo = dataArrayTwo[0];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin, dataTwo);   
    shiftOut(dataPin, clockPin, dataOne);
    
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
  
}
