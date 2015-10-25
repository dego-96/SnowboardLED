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

    //    // shift 5 leds
    //    pixels.setPixelColor(i, pixels.Color(255*accx/1024, 255*accy/1024, 255*accz/1024));
    //    if (i - len > -1 || NUMPIXELS - len + i) {
    //      pixels.setPixelColor(i - len, pixels.Color(0, 0, 0));
    //    }
    //    if (len > i) {
    //      pixels.setPixelColor(NUMPIXELS - len + i, pixels.Color(0, 0, 0));
    //    }

    // hue cycle
    pixels.setPixelColor(i, HSItoRGB((i * 20)%360, 250, 250));
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

uint32_t HSItoRGB(int hue, int saturation, int intensity) {
  uint8_t r, g, b;
  if (0 <= hue && hue < 60) {
    r = intensity; // MAX
    b = (uint8_t)((float)intensity - (((float)saturation / 255.0f) * (float)intensity)); // MIN
    g = (uint8_t)(((float)hue / 60.0f) * ((float)intensity - (float)b) + (float)b); // 
  } 
  else if (60 <= hue && hue < 120) {
    g = intensity; // MAX
    b = (uint8_t)((float)intensity - (((float)saturation / 255.0f) * (float)intensity)); // MIN
    r = (uint8_t)(((120.0f - (float)hue) / 60.0f) * ((float)intensity - (float)b) + (float)b); // 
  } 
  else if (120 <= hue && hue < 180) {
    g = intensity; // MAX
    r = (uint8_t)((float)intensity - (((float)saturation / 255.0f) * (float)intensity)); // MIN
    b = (uint8_t)((((float)hue - 120) / 60.0f) * ((float)intensity - (float)r) + (float)r); // 
  } 
  else if (180 <= hue && hue < 240) {
    b = intensity; // MAX
    r = (uint8_t)((float)intensity - (((float)saturation / 255.0f) * (float)intensity)); // MIN
    g = (uint8_t)(((240.0f - (float)hue) / 60.0f) * ((float)intensity - (float)r) + (float)r); // 
  } 
  else if (240 <= hue && hue < 300) {
    b = intensity; // MAX
    g = (uint8_t)((float)intensity - (((float)saturation / 255.0f) * (float)intensity)); // MIN
    r = (uint8_t)((((float)hue - 240.0f) / 60.0f) * ((float)intensity - (float)g) + (float)g); // 
  } 
  else if (300 <= hue && hue < 360) {
    r = intensity; // MAX
    g = (uint8_t)((float)intensity - (((float)saturation / 255.0f) * (float)intensity)); // MIN
    b = (uint8_t)(((360.0f - (float)hue) / 60.0f) * ((float)intensity - (float)g) + (float)g); // 
  } 
  else {
    r = 0;
    g = 0;
    b = 0;
  }
  return pixels.Color(r, g, b);
}


