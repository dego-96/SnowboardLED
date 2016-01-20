#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIN_LED 6
#define PIN_ACC_X 0
#define PIN_ACC_Y 1
#define PIN_ACC_Z 2
#define PIN_VOL 3
#define NUMPIXELS 104

#define INIT_COUNT 1
int init_x;
int init_y;
int init_z;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

int delayval = 1;
int ledint = 255;
int ledsat = 250;

/*****************************************************************
/* setup                                                        */
/****************************************************************/
void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

  // 加速度センサーの初期値を設定
  getInitAccSensorVal();
}

/*****************************************************************
/* loop                                                         */
/****************************************************************/
void loop() {
  long accx = 0;
  long accy = 0;
  long accz = 0;
  long dx;
  long dy;
  uint8_t r, g, b;

  for (int i = 0; i < NUMPIXELS; i++) {
    // ボリュームの値を取得
    ledint = analogRead(PIN_VOL) / 4;

    // 加速度センサの値を取得
    accx = analogRead(PIN_ACC_X);
    accy = analogRead(PIN_ACC_Y);
    accz = analogRead(PIN_ACC_Z);
    //  print_acc(accx, accy, accz); // 加速度センサの値をシリアルポートに出力

    /***********************************************/
    /* (255, 255, 255) : white                     */
    /* (255, 255, 0)   : yellow                    */
    /* (0, 255, 255)   : cyan                      */
    /* (0, 255, 0)     : green                     */
    /* (0, 0, 255)     : blue                      */
    /*                                             */
    /*            green                            */
    /*              ↑                             */
    /*   cyan  ← white  → yellow                 */
    /*              ↓                             */
    /*             blue                            */
    /*                                             */
    /***********************************************/

    // 必要な変数を宣言
    dx = accx - init_x;
    dy = accy - init_y;

    r = ledint;
    g = ledint;
    b = ledint;

    // 傾きに応じて色を設定
    if (dx > 0) {
      g = ledint - dx;
    }
    else {
      b = ledint + dx;
    }
    if (dy > 0) {
      b = ledint - dy;
      r = ledint - dy;
    }
    else {
      g = ledint + dy;
      r = ledint + dy;
    }
    // 上下限処理
    r = (r > 255) ? 255 : ((r < 0) ? 0 : r);
    g = (g > 255) ? 255 : ((g < 0) ? 0 : g);
    b = (b > 255) ? 255 : ((b < 0) ? 0 : b);

    print_acc(r, g, b); // for debug

    // LEDに色情報を設定
    pixels.setPixelColor(i, (((uint32_t)r << 16) | ((uint32_t)g << 8) | b));
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

/*****************************************************************
/* getInitAccSensorVal                                          */
/*                                                              */
/*   加速度センサーの基準値を設定する                           */
/****************************************************************/
void getInitAccSensorVal() {
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

/*****************************************************************
/* getLean360                                                   */
/*   int x : X座標                                              */
/*   int y : Y座標                                              */
/*                                                              */
/*   x, y座標から360度の向きに変換する                          */
/****************************************************************/
int getLean360(int x, int y) {
  int res = 0;

  double rad = atan2((double)y, (double)x);
  res = (int)(rad * 360.0 / (2 * M_PI));

  if (res < 0) {
    res += 360;
  }

  return res;
}

/*****************************************************************
/* HSItoRGB                                                     */
/*   int hue        : 色相                                      */
/*   int saturation : 彩度                                      */
/*   int intensity  : 明度                                      */
/*                                                              */
/*   HSI表色系からRGB表色系に変換する                           */
/****************************************************************/
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

/*****************************************************************
/* print_acc                                                    */
/*   long x : X方向の加速度                                     */
/*   long y : Y方向の加速度                                     */
/*   long z : Z方向の加速度                                     */
/*                                                              */
/*   加速度センサーの値を表示する                               */
/****************************************************************/
void print_acc(long x, long y, long z) {
  Serial.print("ACC Val: (");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(z);
  Serial.println(")");
}




