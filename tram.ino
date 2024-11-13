#include<FastLED.h>

#define LED_AMOUNT 48
#define DATA_PIN 6
#define GLOBAL_BRIGHTNESS 30

CRGB leds[LED_AMOUNT];

enum Animations {
  TEST,
  RAINBOW,
  WAVE,
  RED_WHITE_WAVE,
  MOVE,
  LAVA,
};

const Animations animation = LAVA;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, LED_AMOUNT); 
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  Serial.begin(115200);
}

void loop() {
  if (animation == RAINBOW) rainbowAnimationStep();
  if (animation == WAVE) wave();
  if (animation == RED_WHITE_WAVE) redWhiteWave();
  if (animation == MOVE) move();
  if (animation == LAVA) lava();

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

void wave() {
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

void redWhiteWave() {
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

void move() {
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


void lava() {
  static float x = 0;
  static float y = 0;
  static float z = 0;
  const static unsigned short int shift = 150;
  uint8_t scale = 30; // Scale of the perlin noise

  for (int i = 0; i < LED_AMOUNT; ++i) {
    uint8_t hue = inoise8(x + i * scale, y, z); // Generate perlin noise for hue
    leds[i] = CHSV((hue+shift)%255, 255, 255);
  }
  
  // Move through the noise space over time
  x += 0.4;
  y += 0.4;
  z += 0.4;
}
