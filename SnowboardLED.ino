#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 60

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  int len = 5; // 同時点灯するLEDの個数
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // green color. 
    if (i - len > -1 || NUMPIXELS - len + i) {
      pixels.setPixelColor(i - len, pixels.Color(0, 0, 0));
    }
    if (len > i) {
      pixels.setPixelColor(NUMPIXELS - len + i, pixels.Color(0, 0, 0));
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

