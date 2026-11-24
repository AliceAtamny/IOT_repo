#include <Adafruit_NeoPixel.h>

#define LED_PIN 23
#define NUM_LEDS 3

Adafruit_NeoPixel strip(
  NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800
);

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(30);
  strip.clear();
  strip.show();
}

void loop() {
  Serial.println("RED");
  strip.fill(strip.Color(255, 0, 0));
  strip.show();
  delay(1000);

  Serial.println("GREEN");
  strip.fill(strip.Color(0, 255, 0));
  strip.show();
  delay(1000);

  Serial.println("BLUE");
  strip.fill(strip.Color(0, 0, 255));
  strip.show();
  delay(1000);

  Serial.println("Individual LED test");

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.clear();
    strip.setPixelColor(i, strip.Color(255, 255, 255));
    strip.show();
    delay(1000);
  }

  strip.clear();
  strip.show();
  delay(1000);
}