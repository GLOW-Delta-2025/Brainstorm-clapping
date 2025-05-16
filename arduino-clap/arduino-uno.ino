#include <Adafruit_NeoPixel.h>

#define LED_PIN     6
#define NUM_LEDS    60

// this must match Python’s scale (volume_norm*100)
#define VOL_THRESHOLD 120  

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();  // all off
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    // only handle lines starting with 'C'
    if (line.length()>1 && line.charAt(0)=='C') {
      int volInt = line.substring(1).toInt();
      // map volume to number of LEDs (1…NUM_LEDS)
      int n = map(volInt, VOL_THRESHOLD, VOL_THRESHOLD*3, 1, NUM_LEDS);
      n = constrain(n, 1, NUM_LEDS);
      // light up the first n LEDs in green
      for (int i = 0; i < NUM_LEDS; i++) {
        if (i < n) strip.setPixelColor(i, strip.Color(0, 255, 0));
        else       strip.setPixelColor(i, 0);
      }
      strip.show();
      // hold briefly, then clear
      delay(200);
      strip.clear();
      strip.show();
    }
  }
}