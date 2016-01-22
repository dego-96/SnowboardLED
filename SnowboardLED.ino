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
  int r, g, b;

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
      r = r - dx * 2;  // 減算
      g = g + dx * 2;  // 加算
      b = b - dx * 2;  // 減算
    }
    else {
      r = r + dx * 2;  // 減算
      g = g + dx * 2;  // 減算
      b = b - dx * 2;  // 加算
    }
    if (dy > 0) {
      r = r + dy;  // 加算
      g = g + dy;  // 加算
      b = b - dy;  // 減算
    }
    else {
      r = r + dy;  // 減算
      g = g - dy;  // 加算
      b = b - dy;  // 加算
    }
    // 上下限処理
    r = (r > 255) ? 255 : ((r < 0) ? 0 : r);
    g = (g > 255) ? 255 : ((g < 0) ? 0 : g);
    b = (b > 255) ? 255 : ((b < 0) ? 0 : b);

    print_vals(r, g, b, dx, dy); // for debug

    // LEDに色情報を設定
    pixels.setPixelColor(i, (((uint32_t)r << 16) | ((uint32_t)g << 8) | b));
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

/*****************************************************************
/* print_vals                                                   */
/*                                                              */
/*   各種値をシリアルモニターに出力する                         */
/****************************************************************/
void print_vals(int r, int g, int b, long dx, long dy) {
  Serial.print("Color: (");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.print(b);
  Serial.print(")    Acc: (");
  Serial.print(dx);
  Serial.print(", ");
  Serial.print(dy);
  Serial.println(")");
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



