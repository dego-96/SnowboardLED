#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIN_LED 6
#define PIN_ACC_X 0
#define PIN_ACC_Y 1
#define PIN_ACC_Z 2
#define PIN_VOL 3
#define NUMPIXELS 104

#define INIT_COUNT 10
int init_x;
int init_y;
int init_z;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

int delayval = 10;
int ledint = 255;
int ledsat = 250;

void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

  // 加速度センサーの初期値を設定
  getInitAccCensorVal();
}

void loop() {
  int len = 5; // 同時点灯するLEDの個数
  long accx = 0;
  long accy = 0;
  long accz = 0;
  for (int i = 0; i < NUMPIXELS; i++) {
    // ボリュームの値を取得
    ledint = analogRead(PIN_VOL) / 4;
    // 加速度センサの値を取得
    accx = analogRead(PIN_ACC_X);
    accy = analogRead(PIN_ACC_Y);
    accz = analogRead(PIN_ACC_Z);
    print_acc(accx, accy, accz); // 加速度センサの値をシリアルポートに出力

    pixels.setPixelColor(i, HSItoRGB(getLean360(accy - init_y, accx - init_x), ledsat, ledint));
    Serial.println(getLean360(accy - init_y, accx - init_x));

    // hue cycle
    //    pixels.setPixelColor(i, HSItoRGB((i * 20)%360, 250, 50));
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

void getInitAccCensorVal() {
  float sum_x = 0.0f;
  float sum_y = 0.0f;
  float sum_z = 0.0f;
  for (int i = 0; i < INIT_COUNT; i++) {
    sum_x += analogRead(PIN_ACC_X);
    sum_y += analogRead(PIN_ACC_Y);
    sum_z += analogRead(PIN_ACC_Z);
  }
  init_x = (int)(sum_x/INIT_COUNT);
  init_y = (int)(sum_y/INIT_COUNT);
  init_z = (int)(sum_z/INIT_COUNT);
}

int getLean360(int x, int y) {
  int res = 0;

  double rad = atan2((double)y, (double)x);
  res = (int)(rad * 360.0 / (2 * M_PI));

  if (res < 0) {
    res += 360;
  }

  return res;
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

void print_acc(long x, long y, long z) {
    Serial.print("ACC Val: (");
    Serial.print(accx);
    Serial.print(", ");
    Serial.print(accy);
    Serial.print(", ");
    Serial.print(accz);
    Serial.println(")");
}

