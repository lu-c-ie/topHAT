#include "FastLED.h"


#define DATA_PIN    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS   178

// Instantiate color array object, which is the code-representation of the
// physical pixel strip.
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         96
#define FRAMES_PER_SECOND  120
#define RAINBOW_CYCLER_MILLISECONDS  20
#define PATTERN_CYCLER_SECONDS  30


// Mapping constants
#define TRI_A          28
#define TRI_B          35
#define TRI_C          21
#define TRI_D          32
#define TRI_E          39
#define TRI_F          23

    
#define MAX_STRIP_LENGTH  39
#define N_STRIPS          6

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Maps of continuous linear physical pixel arrays
uint8_t triangleA[TRI_A] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
uint8_t triangleB[TRI_B] = {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62};
uint8_t triangleC[TRI_C] = {63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83};
uint8_t triangleD[TRI_D] = {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115};
uint8_t triangleE[TRI_E] = {116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154};
uint8_t triangleF[TRI_F] = {155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177};


void setup() {
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    delay(1000);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {testTriangleSolid, testTriangleA, totalAccident, sinelon, rainbow, rainbowWithGlitter, addGlitterAdd, sinelon, confetti, rainbowGlitter, juggle, bpm};

// here is the list rainbow, rainbowWithGlitter, rainbowGlitter, sinelon, confetti, bpm, starWars, lightning, juggle, addGlitterAdd

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS(RAINBOW_CYCLER_MILLISECONDS) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS(PATTERN_CYCLER_SECONDS) { nextPattern(); } // change patterns periodically
}




void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowGlitter()
{
    CRGB onePix[1];
    fill_rainbow (onePix, 1, gHue, 7);
    leds[random16(NUM_LEDS)] = onePix[0];
    fadeToBlackBy(leds, NUM_LEDS, 50);
    delay(15);
}

void addGlitterAdd()
{
    fadeToBlackBy(leds, NUM_LEDS, 50);
    leds[random16(NUM_LEDS)] += CRGB::White;
    delay(15);
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void totalAccident() {
  // random primary colour chases and glitter
 
 for (uint8_t i = 0; i < NUM_LEDS; i++) {
  leds[triangleA[i]] += CRGB::Purple;
  leds[triangleB[i]] += CRGB::Blue;
  leds[triangleC[i]] += CRGB::Green;
  leds[triangleD[i]] += CRGB::Yellow;
  leds[triangleE[i]] += CRGB::Orange;
  leds[triangleF[i]] += CRGB::Red;
  FastLED.show();
   delay(100);
   FastLED.clear();
  }
}

//void colorTri() {
//  // each triangle different colours
// 
// for (uint8_t i = 0; i < NUM_LEDS); {
//  leds[triangleA[i]] += CRGB::Purple;
//  leds[triangleB[i]] += CRGB::Blue;
//  leds[triangleC[i]] += CRGB::Green;
//  leds[triangleD[i]] += CRGB::Yellow;
//  leds[triangleE[i]] += CRGB::Orange;
//  leds[triangleF[i]] += CRGB::Red;
//  FastLED.show();
//   delay(100);
//   FastLED.clear();
//  }
//}

void testTriangleA() {
    for (uint8_t i = 0; i < TRI_A; i++) {
        leds[triangleA[i]] = CRGB::Purple;
           FastLED.show();
        delay(25);
        FastLED.clear();
    }

     for (uint8_t i = 0; i < TRI_B; i++) {
        leds[triangleB[i]] = CRGB::Blue;
           FastLED.show();
        delay(25);
        FastLED.clear();
    }

    
    for (uint8_t i = 0; i < TRI_C; i++) {
        leds[triangleC[i]] = CRGB::Green;
           FastLED.show();
        delay(25);
        FastLED.clear();
    }

     for (uint8_t i = 0; i < TRI_D; i++) {
        leds[triangleD[i]] = CRGB::Yellow;
           FastLED.show();
        delay(25);
        FastLED.clear();
    }

     for (uint8_t i = 0; i < TRI_E; i++) {
        leds[triangleE[i]] = CRGB::Orange;
           FastLED.show();
        delay(25);
        FastLED.clear();
    }

     for (uint8_t i = 0; i < TRI_F; i++) {
        leds[triangleF[i]] = CRGB::Red;
           FastLED.show();
        delay(25);
        FastLED.clear();
    }
       
}

void testTriangleSolid() {

  fill_solid(leds, TRI_A, CRGB::Red);
  fill_solid(leds, TRI_B, CRGB::Orange);
  fill_solid(leds, TRI_C, CRGB::Yellow);
  fill_solid(leds, TRI_D, CRGB::Green);
  fill_solid(leds, TRI_E, CRGB::Blue);
  fill_solid(leds, TRI_F, CRGB::Purple);

 FastLED.show();
        delay(25);
        FastLED.clear();
  
}



