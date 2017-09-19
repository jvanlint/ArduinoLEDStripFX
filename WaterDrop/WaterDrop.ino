// Water Drop FX for Neopixel Strips
// This is the water drop effect written in C and included in an Arduino script.
// Notes: This won't run on a ATTiny85 for over 50 pixels (it runs out of memory as each pixel takes 3 bytes).

// Include the Adafruit NeoPixel Library
#include <Adafruit_NeoPixel.h>

// Include the waterdrop torture C++ lib
#include "water_torture.h"

// Define the Arduino pin that will send data to the strip.
#define PIN 6

// Specify the number of LEDs/Pixels we will be controlling.
#define NUM_LEDS 100

// Create an Adafruit Neopixel strip object as a global variable.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Initialise a Water torture object variable
WaterTorture water_torture = WaterTorture(&strip);

// Global Vars for Drip/cycle fucntions
long lastTime = 0;


void setup() {
  
    // Required function for Neopixel Library.
    strip.begin();

    // Initialize all pixels to 'off'
    strip.show();
}

void loop() {
  
  //Call Drip function
  Drip();
}

void Drip()
{
 // Specify a boolean indicating if you want to reverse the drip animation (ie. run from end of strip to begining) 
 bool reverseAnimation = true;
 
 MODE_WATER_TORTURE: 
 if (cycle())
        {
        strip.setBrightness(255);
        water_torture.animate(reverseAnimation);
        strip.show();
        
        }
  }

bool cycle()
{
  int cycleMillis = 20;
  
  if (millis() - lastTime >= cycleMillis)
  {
    lastTime = millis();
    return true;
  }
  return false;
}
