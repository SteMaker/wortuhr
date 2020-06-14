/*
  10 | ZWEINEDREIZ | 0
  11 | VIERONFUNFE | 21
  32 | SECHSIEBENH | 22
  33 | ACHTWLANEUN | 43
  54 | MINOVORNACH | 44
  55 | DREIVIERTEL | 65
  76 | HALBIZWEINS | 66
  77 | SIEBENSECHS | 87
  98 | FUNFRDRACHT | 88
  99 | NEUNDEIZEHN | 109
 120 | ELFZWOLFUHR | 110
*/

#include <FastLED.h>

#define NUM_LEDS 11*11
#define LED_DATA_PIN 12
#define MAX_WORD_LEN 12
#define MAX_NUM_WORDS 6
#define LED_OFF CRGB::Black

struct WordPattern {
  uint8_t pattern[MAX_WORD_LEN];
};

const WordPattern patterns[] = {
    {.pattern = {8, 7, 6, 5, 255}},                  // EINE
    {.pattern = {10, 9, 8, 7, 255}},                 // ZWEI
    {.pattern = {4, 3, 2, 1, 255}},                  // DREI
    {.pattern = {11, 12, 13, 14, 255}},              // VIER
    {.pattern = {17, 18, 19, 20, 255}},              // FUENF
    {.pattern = {32, 31, 30, 29, 28, 255}},          // SECHS
    {.pattern = {28, 27, 26, 25, 24, 23, 255}},      // SIEBEN
    {.pattern = {33, 34, 35, 36, 255}},              // ACHT
    {.pattern = {40, 41, 42, 43, 255}},              // NEUN
    {.pattern = {0, 21, 22, 43, 255}},               // ZEHN
    {.pattern = {69, 68, 67, 255}},                  // EIN
    {.pattern = {69, 68, 67, 66, 255}},              // EINS
    {.pattern = {71, 70, 69, 68, 255}},              // ZWEI
    {.pattern = {93, 92, 104, 105, 255}},            // DREI-SQUARE
    {.pattern = {59, 72, 81, 94, 255}},              // VIER
    {.pattern = {98, 97, 96, 95, 255}},              // FUENF
    {.pattern = {83, 84, 85, 86, 87, 255}},          // SECHS
    {.pattern = {77, 78, 79, 80, 81, 82, 255}},      // SIEBEN
    {.pattern = {91, 90, 89, 88, 255}},              // ACHT
    {.pattern = {99, 100, 101, 102, 255}},           // NEUN
    {.pattern = {106, 107, 108, 109, 255}},          // ZEHN
    {.pattern = {120, 119, 118, 255}},               // ELF
    {.pattern = {117, 116, 115, 114, 113, 255}},     // ZWOELF
    {.pattern = {50, 49, 48, 255}},                  // VOR
    {.pattern = {47, 46, 45, 44, 255}},              // NACH
    {.pattern = {112, 111, 110, 255}},               // UHR
    {.pattern = {54, 53, 52, 255}},                  // MIN
    {.pattern = {59, 60, 61, 62, 63, 64, 65, 255}},  // VIERTEL
    {.pattern = {55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 255}}, // DREIVIERTEL
    {.pattern = {76, 75, 74, 73, 255}},              // HALB
    {.pattern = {6, 15, 37, 38, 39, 40, 255}},       // NO WLAN
    {.pattern = {37, 38, 39, 40, 255}},              // WLAN
};

class LedCtrl {
 public:
  LedCtrl() {
    for (int i = 0; i < MAX_NUM_WORDS; i++) prevWordIndices[i] = WORDIDX_STOP;
  }

  ~LedCtrl() {}

  void setup(uint8_t r, uint8_t g, uint8_t b) {
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
    setColor(r, g, b);
  }

  void setColor(uint8_t r, uint8_t g, uint8_t b) {
    ledColor.setRGB(r, g, b);
    luma = ledColor.getLuma();
    Serial.print("LedCtrl::setColor: luma=");
    Serial.println(luma);
    ledColor.maximizeBrightness();
    Serial.print("LedCtrl::setColor: r,g,b=");
    Serial.print(ledColor.r);
    Serial.print(",");
    Serial.print(ledColor.g);
    Serial.print(",");
    Serial.println(ledColor.b);
  }

  void setLuma(uint8_t _luma) {
    if(_luma != luma) {
      Serial.print("LedCtrl::setLuma: luma=");
      Serial.println(luma);
      forceUpdate = true;
    }
    luma = _luma;
  }

  bool forceUpdate = false;

  void setClock(int hour, int minute) {
    if (hour != currentHour || minute != currentMinute || forceUpdate) {
      Serial.print("newClock: ");
      Serial.print(hour);
      Serial.print(":");
      Serial.println(minute);
      clearClockLeds();
      setClockLeds(hour, minute);
      FastLED.show();
      currentHour = hour;
      currentMinute = minute;
      forceUpdate = false;
    }
  }

  void clear(void) {
      Serial.println("Led clear");
      clearClockLeds();
  }

  void showNoWlan(void) {
      Serial.println("showNoWlan");
      CRGB col = ledColor;
      col.nscale8_video(luma);
      clearClockLeds();
      const uint8_t *pattern = patterns[WORDIDX_NOWLAN].pattern;
      int letterCnt = 0;
      while (pattern[letterCnt] != 255) {
        leds[pattern[letterCnt]] = col;
        letterCnt++;
      }
      prevWordIndices[0] = WORDIDX_NOWLAN;
      prevWordIndices[1] = WORDIDX_STOP;
      FastLED.show();
  }

  void showWlan(void) {
      CRGB col = ledColor;
      col.nscale8_video(luma);
      clearClockLeds();
      const uint8_t *pattern = patterns[WORDIDX_WLAN].pattern;
      int letterCnt = 0;
      while (pattern[letterCnt] != 255) {
        leds[pattern[letterCnt]] = col;
        letterCnt++;
      }
      prevWordIndices[0] = WORDIDX_WLAN;
      prevWordIndices[1] = WORDIDX_STOP;
      FastLED.show();
  }

 private:
  enum {
    WORDIDX_M_EINE = 0,
    WORDIDX_M_ZWEI = 1,
    WORDIDX_M_DREI = 2,
    WORDIDX_M_VIER = 3,
    WORDIDX_M_FUENF = 4,
    WORDIDX_M_SECHS = 5,
    WORDIDX_M_SIEBEN = 6,
    WORDIDX_M_ACHT = 7,
    WORDIDX_M_NEUN = 8,
    WORDIDX_M_ZEHN = 9,
    WORDIDX_H_EIN = 10,
    WORDIDX_H_EINS = 11,
    WORDIDX_H_ZWEI = 12,
    WORDIDX_H_DREI = 13,
    WORDIDX_H_VIER = 14,
    WORDIDX_H_FUENF = 15,
    WORDIDX_H_SECHS = 16,
    WORDIDX_H_SIEBEN = 17,
    WORDIDX_H_ACHT = 18,
    WORDIDX_H_NEUN = 19,
    WORDIDX_H_ZEHN = 20,
    WORDIDX_H_ELF = 21,
    WORDIDX_H_ZWOELF = 22,
    WORDIDX_VOR = 23,
    WORDIDX_NACH = 24,
    WORDIDX_UHR = 25,
    WORDIDX_MIN = 26,
    WORDIDX_VIERTEL = 27,
    WORDIDX_DREIVIERTEL = 28,
    WORDIDX_HALB = 29,
    WORDIDX_NOWLAN = 30,
    WORDIDX_WLAN = 31,
    WORDIDX_BASE_MIN = WORDIDX_M_EINE - 1,
    WORDIDX_BASE_HOUR = WORDIDX_H_EINS - 1,
    WORDIDX_STOP = 255,
  };

  int currentHour = 25;
  int currentMinute = 61;

  CRGB leds[NUM_LEDS];
  CRGB ledColor;
  uint8_t luma;

  uint8_t wordIndices[MAX_NUM_WORDS];
  uint8_t prevWordIndices[MAX_NUM_WORDS];

  int hourToWord(int hour, bool pre) {
    hour %= 12;
    if (hour == 1) {
      if (pre)
        return WORDIDX_H_EIN;
      else
        return WORDIDX_H_EINS;
    } else if (hour == 0) {
      return WORDIDX_H_ZWOELF;
    } else
      return hour + WORDIDX_BASE_HOUR;
  }

  int minuteToWord(int minute) { return minute + WORDIDX_BASE_MIN; }

  void getWords(int hour, int minute) {
    hour %= 12;
    hour = hour == 0 ? 12 : hour;

    if (minute == 0) {
      wordIndices[0] = hourToWord(hour, true);
      wordIndices[1] = WORDIDX_UHR;
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute <= 10) {
      wordIndices[0] = minuteToWord(minute);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_NACH;
      wordIndices[3] = hourToWord(hour, false);
      wordIndices[4] = WORDIDX_UHR;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute < 15) {
      wordIndices[0] = minuteToWord(15 - minute);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_VOR;
      wordIndices[3] = WORDIDX_VIERTEL;
      wordIndices[4] = hourToWord(hour + 1, false);
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute == 15) {
      wordIndices[0] = WORDIDX_VIERTEL;
      wordIndices[1] = hourToWord(hour + 1, false);
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute < 20) {
      wordIndices[0] = minuteToWord(minute - 15);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_NACH;
      wordIndices[3] = WORDIDX_VIERTEL;
      wordIndices[4] = hourToWord(hour + 1, false);
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute < 30) {
      wordIndices[0] = minuteToWord(30 - minute);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_VOR;
      wordIndices[3] = WORDIDX_HALB;
      wordIndices[4] = hourToWord(hour + 1, false);
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute == 30) {
      wordIndices[0] = WORDIDX_HALB;
      wordIndices[1] = hourToWord(hour + 1, false);
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute <= 40) {
      wordIndices[0] = minuteToWord(minute - 30);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_NACH;
      wordIndices[3] = WORDIDX_HALB;
      wordIndices[4] = hourToWord(hour + 1, false);
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute < 45) {
      wordIndices[0] = minuteToWord(45 - minute);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_VOR;
      wordIndices[3] = WORDIDX_DREIVIERTEL;
      wordIndices[4] = hourToWord((hour + 1) % 12, false);
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute == 45) {
      wordIndices[0] = WORDIDX_DREIVIERTEL;
      wordIndices[1] = hourToWord(hour + 1, false);
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute < 50) {
      wordIndices[0] = minuteToWord(minute - 45);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_NACH;
      wordIndices[3] = WORDIDX_DREIVIERTEL;
      wordIndices[4] = hourToWord(hour + 1, false);
      wordIndices[5] = WORDIDX_STOP;
    } else {
      wordIndices[0] = minuteToWord(60 - minute);
      wordIndices[1] = WORDIDX_MIN;
      wordIndices[2] = WORDIDX_VOR;
      wordIndices[3] = hourToWord(hour + 1, true);
      wordIndices[4] = WORDIDX_UHR;
      wordIndices[5] = WORDIDX_STOP;
    }
  }

  void setClockLeds(int hour, int minute) {
    int wordCnt = 0;
    CRGB col = ledColor;
    col.nscale8_video(luma);

    // First we evaluate which words we need to highlight
    getWords(hour, minute);

    // We remember them for clearing one minute later
    memcpy(prevWordIndices, wordIndices, sizeof(prevWordIndices));
    Serial.print("words to set: ");
    while (wordIndices[wordCnt] != WORDIDX_STOP) {
      Serial.print(wordIndices[wordCnt]);
      Serial.print(" ");
      // for each word we take the letters to be enabled and set the leds
      const uint8_t *pattern = patterns[wordIndices[wordCnt]].pattern;
      int letterCnt = 0;
      while (pattern[letterCnt] != 255) {
        leds[pattern[letterCnt]] = col;
        letterCnt++;
      }
      wordCnt++;
    }
    Serial.println("");
  }

  void clearClockLeds(void) {
    int wordCnt = 0;
    while (prevWordIndices[wordCnt] != WORDIDX_STOP) {
      // for each word we take the letters to be enabled and set the leds
      const uint8_t *pattern = patterns[prevWordIndices[wordCnt]].pattern;
      int letterCnt = 0;
      while (pattern[letterCnt] != 255) {
        leds[pattern[letterCnt]] = LED_OFF;
        letterCnt++;
      }
      wordCnt++;
    }
    prevWordIndices[0] = WORDIDX_STOP;
  }
};
