#include <Adafruit_NeoPixel.h>

#define PIN_LED 6
#define PIN_ACC_X 0
#define PIN_ACC_Y 1
#define PIN_ACC_Z 2
#define NUMPIXELS 60

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

int delayval = 100;

void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  int len = 5; // 同時点灯するLEDの個数
  long accx = 0;
  long accy = 0;
  long accz = 0;
  for (int i = 0; i < NUMPIXELS; i++) {
    accx = analogRead(PIN_ACC_X);
    accy = analogRead(PIN_ACC_Y);
    accz = analogRead(PIN_ACC_Z);
    Serial.print("X:");
    Serial.print(accx);
    Serial.print("  Y:");
    Serial.print(accy);
    Serial.print("  Z:");
    Serial.println(accz);
//    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // green color. 
    pixels.setPixelColor(i, pixels.Color(255*accx/1024, 255*accy/1024, 255*accz/1024));
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

