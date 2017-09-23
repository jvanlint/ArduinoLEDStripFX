// Cine Lights Standby FX
// This Arduino script is a cleanup of the original Cine lights
// effects script for standby modes on his project:
// Link to project
// Link to original code

// The original code was written in a combination of FastLed and Adafruit's
// Neopixel library. This implementation is exclusively Neopixel to cut down
// on the size of code being loaded into memory.

// Include the Adafruit NeoPixel Library
#include <Adafruit_NeoPixel.h>

// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

// Define the Arduino pin that will send data to the strip.
#define PIN 6

// Specify the number of LEDs/Pixels we will be controlling.
#define NUM_LEDS 100

#define BUTTON_PIN 8

// Create an Adafruit Neopixel strip object as a global variable.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Function prototype allowing optional parameters
uint32_t Wheel(byte WheelPos, float opacity = 1);
void Twinkle (float fadeAmt = 0.96);
void Ripple(boolean addBackgroundColor = true);

// Twinkle function global vars
float redStates[NUM_LEDS];
float blueStates[NUM_LEDS];
float greenStates[NUM_LEDS];

//Ripple function Global vars
//background color
int color;
int center = 0;
int iterationNum = -1;

uint32_t currentBgColor = random(256);
uint32_t nextBgColor = currentBgColor;

int buttonPushCounter = 1;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;

void setup() {
     pinMode(BUTTON_PIN, INPUT);
     digitalWrite(BUTTON_PIN, HIGH);
    // Required function for Neopixel Library.
    strip.begin();

    lcd.begin(16,2);

    // Set the strip brightness if required (may be overidden by some FX)
    //strip.setBrightness(20);

    // Initialize all pixels to 'off'
    strip.show();
}

void loop() {
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    
    if (buttons & BUTTON_UP) {
      lcd.setBacklight(RED);
      buttonPushCounter --;
      if (buttonPushCounter == 0)
      {
        buttonPushCounter = 4;
      }
    }
    if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(YELLOW);
      buttonPushCounter ++;
      if (buttonPushCounter == 5)
      {
        buttonPushCounter = 1;
      }
    }
    if (buttons & BUTTON_LEFT) {
      //lcd.print("LEFT ");
      //lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
      //lcd.print("RIGHT ");
      //lcd.setBacklight(TEAL);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.print("SELECT ");
      lcd.setBacklight(VIOLET);
      buttonPushCounter=0;
    }
  }

switch (buttonPushCounter){
  
           
  case 1:
     buttonPushCounter==1; {
      lcd.setCursor(0,0);
      lcd.print("Twinkle");
      Twinkle(); 
     break;}
       
  case 2:
     buttonPushCounter==2; {
       lcd.setCursor(0,0);
       lcd.print("Ripple (With BG)");
       Ripple(); 
      break;}
      
   case 3:
     buttonPushCounter==3; {
      lcd.setCursor(0,0);
      lcd.print("Ripple (No BG)");
    Ripple(false); // 
      break;}
         
    case 4:
     buttonPushCounter==4; {
      lcd.setCursor(0,0);
       lcd.print("Rainbow");
    Rainbow(20); // 
      break;}  

}
}

/* These routines came from VUMeter on YouTube */

int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// Used by several routines below.
// Notes: The opacity parameter defaults to 1 unless explicitly passed in.
uint32_t Wheel(byte WheelPos, float opacity) {

  if(WheelPos < 85) {
    return strip.Color((WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity, 0);
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color((255 - WheelPos * 3) * opacity, 0, (WheelPos * 3) * opacity);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, (WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity);
  }
}

// Twinkle
// Desc: Randomly displays pixels as flashes of light across the strip.
// Parameters:
//              - fadeAmt : The amount of fade each pixel should exhibit.
// Returns: void
// Notes: Selects color and position at random. Relies on several global vars.
//        The fadeAmt defaults to 0.96 unless explicitly passed in as a parameter.
void Twinkle (float fadeAmt) {

  if (random(25) == 1) {
      uint16_t randomLEDNumber = random(NUM_LEDS);
      if (redStates[randomLEDNumber] < 1 && greenStates[randomLEDNumber] < 1 && blueStates[randomLEDNumber] < 1) {
        redStates[randomLEDNumber] = random(256);
        greenStates[randomLEDNumber] = random(256);
        blueStates[randomLEDNumber] = random(256);
      }
    }

    for(uint16_t pixelPos = 0; pixelPos < NUM_LEDS; pixelPos++) {
      if (redStates[pixelPos] > 1 || greenStates[pixelPos] > 1 || blueStates[pixelPos] > 1) {
        strip.setPixelColor(pixelPos, redStates[pixelPos], greenStates[pixelPos], blueStates[pixelPos]);

        if (redStates[pixelPos] > 1) {
          redStates[pixelPos] = redStates[pixelPos] * fadeAmt;
        } else {
          redStates[pixelPos] = 0;
        }

        if (greenStates[pixelPos] > 1) {
          greenStates[pixelPos] = greenStates[pixelPos] * fadeAmt;
        } else {
          greenStates[pixelPos] = 0;
        }

        if (blueStates[pixelPos] > 1) {
          blueStates[pixelPos] = blueStates[pixelPos] * fadeAmt;
        } else {
          blueStates[pixelPos] = 0;
        }

      } else {
        strip.setPixelColor(pixelPos, 0, 0, 0);
      }
    }
    strip.show();
    delay(10);
}

void Ripple (boolean addBackgroundColor) {
    int maxSteps = 16;
    float fadeRate = 0.80;

    if (addBackgroundColor)     //We need to cater for background LED lighting
    {
        if (currentBgColor == nextBgColor)  //Check to see if they match. This is set at global var level.
        {
          nextBgColor = random(256);
        }
        else if (nextBgColor > currentBgColor)
        {
          currentBgColor++;
        } else
        {
          currentBgColor--;
        }

        for(uint16_t pixelPos = 0; pixelPos < NUM_LEDS; pixelPos++) {
          strip.setPixelColor(pixelPos, Wheel(currentBgColor, 0.1));
        }
    } else
    {
        for(uint16_t pixelPos = 0; pixelPos < NUM_LEDS; pixelPos++)
        {
            strip.setPixelColor(pixelPos, 0, 0, 0);
        }
    }

    if (iterationNum == -1) {
        center = random(NUM_LEDS);
        color = random(256);
        iterationNum = 0;
    }

  if (iterationNum == 0) {
    strip.setPixelColor(center, Wheel(color, 1));
    iterationNum ++;
  }
  else {
    if (iterationNum < maxSteps) {

      strip.setPixelColor(wrap(center + iterationNum), Wheel(color, pow(fadeRate, iterationNum)));
      strip.setPixelColor(wrap(center - iterationNum), Wheel(color, pow(fadeRate, iterationNum)));
      if (iterationNum > 3) {
        strip.setPixelColor(wrap(center + iterationNum - 3), Wheel(color, pow(fadeRate, iterationNum - 2)));
        strip.setPixelColor(wrap(center - iterationNum + 3), Wheel(color, pow(fadeRate, iterationNum - 2)));
      }
      iterationNum ++;
    }
    else {
      iterationNum = -1;
    }
  }

  strip.show();
  delay(50);
}

void Rainbow(uint8_t wait) {
  uint16_t pixelPos, colorValue;

  for(colorValue=0; colorValue<256; colorValue++) {
    for(pixelPos=0; pixelPos < strip.numPixels(); pixelPos++) {
      strip.setPixelColor(pixelPos, Wheel((pixelPos + colorValue) & 255));
    }
    strip.show();
    if (digitalRead(BUTTON_PIN) != lastButtonState)  // <------------- add this
       return;         // <------------ and this
    delay(wait);
  }
}
