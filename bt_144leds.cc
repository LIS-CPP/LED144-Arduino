#include <SoftwareSerial.h>
#include <FastLED.h>

constexpr uint8_t rxPin = 3;
constexpr uint8_t txPin = 2;

static SoftwareSerial bt (rxPin, txPin);
static uint8_t it = 1;

void getSignal(){
  static uint8_t dataByte = 0;

  if(bt.available()){
    dataByte = bt.read();
    Serial.write(dataByte);
  }

  if(dataByte == '1') it = 1;
  else if(dataByte == '2') it = 2;
  else if(dataByte == '3') it = 3;
  else if(dataByte == '4') it = 4;
  else if(dataByte == '5') it = 5;
  else if(dataByte == '6') it = 6;
  else if(dataByte == '0') it = 0;

  if(Serial.available()){
    dataByte = Serial.read();
    bt.write(dataByte);
  }
}

class mytimer {
  uint32_t delay;
  uint32_t prev;
  uint32_t (*f)(void);
public:
  mytimer() : delay(1000), prev(0), f(millis) {};
  mytimer::mytimer(uint32_t delay, uint32_t (*func)(void) = millis) : delay(delay), f(func) { prev = f(); }
  bool isEnd() {
    uint32_t current = f();
    if(current - prev > delay){
      prev = current;
      return true;
    }
    return false;
  }
  void setDelay(uint32_t delay){ this->delay = delay; }
  void resetTimer() { this->prev = f(); }
};

void turnoff(), ranbow(), confetti(), runninglight(), cyclone(), focus(), flashlight(), stars();
constexpr void (*effects[])() = {turnoff, runninglight, ranbow, confetti, flashlight, focus, stars}; 

constexpr uint16_t LEDCNT = 144;

CRGB leds[::LEDCNT];

uint8_t hue = 0;

void setup() {
  pinMode(::rxPin, INPUT);
  pinMode(::txPin, OUTPUT);
  Serial.begin(9600);
  bt.begin(9600);

  //TCCR0B = TCCR0B & 0b11111000 | 0b1;
  attachInterrupt(0, getSignal, RISING);

  auto&& LEDID = FastLED.addLeds<WS2812, 6, GRB>(leds, ::LEDCNT);
  LEDID.setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(100);
}

static mytimer t_inner(20);
//static mytimer t_outro(10000);
//static mytimer tx(1000);

void loop() {
  effects[it]();
  FastLED.show();
}

void ranbow() {
  if(!t_inner.isEnd()) return;
  for (int i = 0; i < ::LEDCNT; ++i)
    leds[i] = CHSV(::hue + i*13, 255, 255);
  ++hue;
}

void confetti() {
  fadeToBlackBy(leds, ::LEDCNT, 8);
  int pos = random16(::LEDCNT);
  leds[pos] += CHSV(::hue++ + random8(64), 200, 255);
}

void runninglight() {
  fadeToBlackBy(leds, ::LEDCNT, 8);
  int pos = beatsin16(13, 0, ::LEDCNT - 1);
  leds[pos] = CHSV(::hue++, 255, 192);
}

void stars() {
  if(!t_inner.isEnd()) return;
  fadeToBlackBy(leds, ::LEDCNT, 7);
  if (random8() < 80) { leds[ random16(::LEDCNT) ] += CHSV(180, 127, 192); }
}

void focus() {
  if(!t_inner.isEnd()) return;
  fadeToBlackBy(leds, ::LEDCNT, 2);
  for (int i = 0; i < 8; i++)
    leds[beatsin16(i + 7, 0, ::LEDCNT - 1)] |= CHSV(::hue += 16, 200, 255);
}

void flashlight() {
  if(!t_inner.isEnd()) return;
  fill_rainbow( leds, ::LEDCNT, ::hue++, 6);
  if (random8() < 80) { leds[ random16(::LEDCNT) ] += CRGB::White; }
}

void turnoff(){
  if(!t_inner.isEnd()) return;
  for(auto&& led : ::leds){
    led.b = led.g = led.r = 0;
  }
}
