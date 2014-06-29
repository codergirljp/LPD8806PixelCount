#include "LPD8806.h"
#include "SPI.h"

// Determine the number of LEDS in an LPD8806-based RGB LED strip
// using the SPI MISO pin and DO on the strip
/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 0;   // initialize LED pixel count to 0

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 51;
int clockPin = 52;

// The maximum number of Pixels to check for (change this this to your maximum)
// This should be a number that is greater than the number of LEDs on the longest light strip that might be used.
// Note: This is needed because if the DO is not connected we will never receive data back so we want to eventually stop 
// checking when that happens. 
int maxLEDs = 100;

// MISO pin on the AT Mega 2560 is pin 50. 
// To measure the length of the strip, you must 
// connect pin 50 to DO on other side of LPD8806 strip

LPD8806 strip = LPD8806(nLEDs);

byte dataReceived = false;

void setup()
{
    Serial.begin(9600); 
    Serial.print("First Read "); Serial.println(SPDR);
    
    strip.begin();   
    strip.show(); // init to off
    
    while(dataReceived == false )
    {
      // increment the number of leds
      nLEDs++;
      
      strip.updateLength(nLEDs);
      
      colorTest(strip.Color(0,0,127), 5); // blue
      
      Serial.print("Tested ");
      Serial.print(nLEDs-1);
      Serial.println(" LED pixels");
      
      // We tested the maxumim number of LEDs
      // and didn't find the end of the strip
      // The DO pin may not be connected
      if(maxLEDs == nLEDs)
      {
        Serial.print("No Data received after testing the maximum number of LEDs ");
        Serial.println(nLEDs);
        Serial.println("Is the DO on the light strip connected to the MISO pin?");
        break;
      }
      
    }
    
    if(dataReceived == true)
    {
     // number of pixels is one less than the number of registers needed to fill the receive buffer
     nLEDs--;
     
    // print the result
     Serial.print("Found ");
     Serial.print(nLEDs);
     Serial.println(" LED pixels in the strip.");
    }
}

void loop()
{
    // Light show programming here
}

// Light up LEDs on the strip
// Check to see if we got data back
void colorTest(uint32_t c, uint8_t wait) 
{
  int i;

  for (i=0; i < strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
  }
  
  strip.show();

  // set the dataRecieved flag if we got data back
  if(SPDR>0) dataReceived=true;
      
  delay(wait);
}
