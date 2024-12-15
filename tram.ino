#include<FastLED.h>

#define LED_AMOUNT 48
#define DATA_PIN 6
#define GLOBAL_BRIGHTNESS 60

CRGB leds[LED_AMOUNT];

enum Animations {
  TEST,
  RAINBOW,
  WAVE,
  RED_WHITE_WAVE,
  MOVE,
  LAVA,
  OLD_METRO,
};

const Animations animation = LAVA;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_AMOUNT);
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  Serial.begin(115200);
}

void loop() {
  if (animation == RAINBOW) rainbowAnimationStep();
  if (animation == WAVE) waveAnimationStep();
  if (animation == RED_WHITE_WAVE) redWhiteWaveAnimationStep();
  if (animation == MOVE) moveAnimationStep();
  if (animation == LAVA) lavaAnimationStep();
  if (animation == OLD_METRO) oldMetroAnimationStep();

  // handle20MinPomodoro();
  // handle40MinPomodoro();

  FastLED.show();
}

void rainbowAnimationStep() {
  const unsigned short int sat = 180;
  static float animation = 0;

  for(int i=0; i<LED_AMOUNT; ++i){
    float hue = sin(animation + (i+1) * 0.1);
    hue = (hue + 1) / 2;
    hue *= 255;
    leds[i] = CHSV(hue, sat, 255);
    if (hue==0) {
      animation = 0;
    };
    if (i==0) { Serial.println(">hue: " + String(hue)); }
  }

  animation += 0.02;
}

void waveAnimationStep() {
  const unsigned short int hue = 20;
  const unsigned short int sat = 200;

  static float animation = 0;
  static float fadeAnimation = 0;

  const float fadeEffect = (sin(fadeAnimation) + GLOBAL_BRIGHTNESS) / 2;

  FastLED.setBrightness(fadeEffect * GLOBAL_BRIGHTNESS);

  for(int i=0; i<LED_AMOUNT; ++i){

    float brightness = sin(animation + i * 0.3);
    brightness = (brightness + 1) / 2;
    brightness *= 255;

    leds[i] = CHSV(hue, sat, brightness);
  }

  animation += 0.04;
  fadeAnimation += 0.04;
}

void redWhiteWaveAnimationStep() {
  static float animation = 0;
  for(int i=0; i<LED_AMOUNT; ++i){

    float saturation = sin(animation + i * 0.04);
    saturation = (saturation + 1) / 2;
    saturation *= 255;
    saturation = max(saturation, 30);

    leds[i] = CHSV(0, saturation, 255);
  }

  animation += 0.02;
}

void moveAnimationStep() {
  static float move = 0;
  static float freq = 0;

  const float shrinkage = (sin(freq) + 10) / 2;

  for(int i=0; i<LED_AMOUNT; ++i) {
    float saturation = sin(move + i * shrinkage);
    saturation = (saturation + 1) / 2;
    saturation *= 255;
    saturation = max(saturation, 40);

    leds[i] = CHSV(190, saturation, 255);
  }

  freq += 0.003;
  move += 0.1;
}

void lavaAnimationStep() {
  const static unsigned short int shift = 130;
  const static uint8_t scale = 30; // Scale of the perlin noise
  const float step = 0.4;
  static float y = 0;
  static boolean isForward = true;

  for (int i = 0; i < LED_AMOUNT; ++i) {
    uint8_t hue = (shift + inoise8(i * scale, y))%255;
    leds[i] = CHSV(hue, 255, 255);
  }

  // Move through the noise space over time
  if (y < 0) isForward = true;
  if (y > 3E+6) isForward = false;
  y += isForward ? step : -step;
}

void oldMetroAnimationStep() {
  const static unsigned short int shift = 130;
  const static uint8_t scale = 30; // Scale of the perlin noise
  const static unsigned short int hue = 15;

  static float y = 0;

  for (int i = 0; i < LED_AMOUNT; ++i) {
    uint8_t bright = shift + inoise8(i * scale, y);

    leds[i] = CHSV(hue, 255, bright);
  }

  // Move through the noise space over time
  y += 0.04;
}

void handle20MinPomodoro() {
  const unsigned short int current = 30;
  const unsigned short int ledAmount = 10;
  const unsigned short int diff = 255 / ledAmount;

  for (int i = current; i>=0; i--) {
    leds[i] = CHSV(HUE_GREEN, 255, 255);
  }
}

void handle40MinPomodoro() {
  const unsigned short int current = 15;
  const unsigned short int ledAmount = 10;
  const unsigned short int diff = 255 / ledAmount;

  for (int i = current; i>=0; i--) {
    leds[i] = CHSV(HUE_ORANGE, 255, 255);
  }
}
