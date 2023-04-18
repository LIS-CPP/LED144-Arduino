#include <FastLED.h>

class mytimer {
  uint32_t delay;
  uint32_t prev;
public:
  mytimer(uint32_t delay);
  bool isOver();
};

void ranbow();
void confetti();
void runninglight();
void cyclone();
void focus();
void flashlight();

constexpr uint16_t LEDCNT = 144;
constexpr uint16_t PIN = 2;
constexpr uint16_t Volte = 5;
constexpr uint16_t Amper = 500;

CRGB leds[::LEDCNT];

uint8_t bright = 100;
uint8_t hue = 0;

void setup() {
  FastLED.addLeds <WS2812, ::PIN, GRB>(leds, ::LEDCNT).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(Volte, Amper);
  FastLED.setBrightness(::bright);

  pinMode(13, 1);
}

void loop() {
  static uint8_t status = 0;
  static mytimer t(60000);
  static mytimer t_inner(20);
  static mytimer t_redlight(500);
  static uint8_t redlightstatus = false;

  if(t_redlight.isOver()) { redlightstatus = redlightstatus ? false : true; digitalWrite(13, redlightstatus); }

  switch(status){
    case 0: runninglight(); break;
    case 1: if(t_inner.isOver()) { ranbow(); } break;
    case 2: confetti(); break;
    case 3: if(t_inner.isOver()) { flashlight(); } break;
    case 4: if(t_inner.isOver()) { focus(); } break;
    case 5: if(t_inner.isOver()) { stars(); } break;
  }
  FastLED.show();

  if(t.isOver() && ++status > 5) status = 0;
}

void ranbow() {
  for (int i = 0; i < ::LEDCNT; ++i)
    leds[i] = CHSV(::hue + i*3, 255, 255);
  ++hue;
}

void confetti() {
  fadeToBlackBy(leds, ::LEDCNT, 2);
  int pos = random16(::LEDCNT);
  leds[pos] += CHSV(::hue++ + random8(64), 200, 255);
}

void runninglight() {
  fadeToBlackBy(leds, ::LEDCNT, 2);
  int pos = beatsin16(13, 0, ::LEDCNT - 1);
  leds[pos] += CHSV(::hue++, 255, 192);
}

void stars() {
  fadeToBlackBy(leds, ::LEDCNT, 7);
  if (random8() < 80) { leds[ random16(::LEDCNT) ] += CHSV(180, 127, 192); }
}

void focus() {
  fadeToBlackBy(leds, ::LEDCNT, 2);
  for (int i = 0; i < 8; i++)
    leds[beatsin16(i + 7, 0, ::LEDCNT - 1)] |= CHSV(::hue += 16, 200, 255);
}

void flashlight() {
  fill_rainbow( leds, ::LEDCNT, ::hue++, 6);
  if (random8() < 80) { leds[ random16(::LEDCNT) ] += CRGB::White; }
}

// OBJECT
mytimer::mytimer(uint32_t delay)
: delay(delay)
{
  prev = millis(); // micros()
}

bool mytimer::isOver() {
  uint32_t current = millis();
  if(current - prev > delay){
    prev = current;
    return true;
  }
  return false;
}
