#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

//  The overall fire brightness
//  (this can affect both color levels and power consumption)
int brightness = 128;

struct RGB {
  byte r;
  byte g;
  byte b;
};

//  The flame color array (the first is the default):
RGB flameColors[] = {
  { 226, 121, 35},  // Orange flame
  { 158, 8, 148},   // Purple flame 
  { 74, 150, 12},   // Green flame
  { 226, 15, 30}    // Red flame
  };

//  Number of flame colors
int NUMBER_OF_COLORS = sizeof(flameColors) / sizeof(RGB);

//  Tracks the current color
int currentColorIndex = 0;

//  The button pin
const int buttonPin = 2;

//  Variable for reading the pushbutton status
int buttonState = 0;         

//  Tracking if it's ok to shift colors or not
bool okToChangeColors = true;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
// For the ultimate NeoPixel guide, check out:
// https://learn.adafruit.com/adafruit-neopixel-uberguide/overview

void setup() {
  Serial.begin(9600);
  Serial.print(NUMBER_OF_COLORS);
  
  strip.begin();
  strip.setBrightness(brightness);
  strip.show(); // Initialize all pixels to 'off'

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  //  Get the current colors based on the color index:
  RGB currentColor = flameColors[currentColorIndex];

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  //  Button is being pressed and it's OK to change colors:
  if (buttonState == HIGH) {
    if(okToChangeColors){
      okToChangeColors = false;
      
      //  Shift to another color flame:
      if(currentColorIndex < (NUMBER_OF_COLORS - 1)){ 
        currentColorIndex++;  
      } else {
        currentColorIndex = 0;
      }
    }
  }
  else {
    okToChangeColors = true;
  }

  //  Flicker, based on our initial RGB values
  for(int i=0; i<strip.numPixels(); i++) {
    int flicker = random(0,55);
    int r1 = currentColor.r-flicker;
    int g1 = currentColor.g-flicker;
    int b1 = currentColor.b-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
    strip.setPixelColor(i,r1,g1, b1);
  }
  strip.show();

  //  Adjust the delay here, if you'd like.  Right now, it randomizes the 
  //  color switch delay to give a sense of realism
  delay(random(10,113));
}

