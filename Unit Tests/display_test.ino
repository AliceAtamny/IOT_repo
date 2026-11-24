#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  4
#define TFT_SCLK 18
#define TFT_MOSI 23

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI);

  tft.init(240, 280);
  tft.setRotation(0);

  uint16_t PINK = tft.color565(255, 105, 180);
  uint16_t DARK_PINK = tft.color565(255, 20, 147);

  // White background
  tft.fillScreen(ST77XX_WHITE);

  // Pink heart
  tft.fillCircle(80, 90, 40, PINK);
  tft.fillCircle(160, 90, 40, PINK);

  tft.fillTriangle(
    43, 105,
    197, 105,
    120, 190,
    PINK
  );

  // I LOVE YOU text
  tft.setTextColor(DARK_PINK);
  tft.setTextSize(3);
  tft.setCursor(30, 220);
  tft.print("I LOVE YOU");

  Serial.println("Display Test OK!");
}

void loop() {
}