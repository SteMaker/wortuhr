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
    {.pattern = {103, 255}},                         // D for DOT
    {.pattern = {96, 101, 119, 114, 255}},           // N-U-L-L
};

class LedCtrl {
 public:
  enum ColorType {
    minutesNumeral,
    minWord,
    preWord,
    quarterWord,
    hoursNumeral,
    clockWord
  };

  LedCtrl() {
    for (int i = 0; i < MAX_NUM_WORDS; i++) prevWordIndices[i] = WORDIDX_STOP;
  }

  ~LedCtrl() {}

  void setup(void) {
    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  }

  void setColor(ColorType t, uint8_t hue, uint8_t sat, sint8_t lumaOffset, uint8_t _luma) {
    sint16_t luma = _luma + lumaOffset;
    if(luma < 0) luma = 0;
    if(luma > 255) luma = 255;
    CRGB c = CRGB(CHSV(hue, sat, (uint8_t)luma));
    Serial.print("LedCtrl::setColor: r,g,b=");
    Serial.print(c.r);
    Serial.print(",");
    Serial.print(c.g);
    Serial.print(",");
    Serial.println(c.b);
    switch (t) {
        case minutesNumeral:
          colorMinutesNumeral = c;
          break;
        case minWord:
          colorMinWord = c;
          break;
        case preWord:
          colorPreWord = c;
          break;
        case quarterWord:
          colorQuarterWord = c;
          break;
        case hoursNumeral:
          colorHoursNumeral = c;
          break;
        case clockWord:
          colorClockWord = c;
          break;
      }
  }

  void setLumaScale(uint8_t _lumaScale) {
    if(_lumaScale != lumaScale) {
      Serial.print("LedCtrl::setLumaScale: lumaScale=");
      Serial.println(_lumaScale);
      forceUpdate = true;
    }
    lumaScale = _lumaScale;
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
      clearClockLeds();
      FastLED.show();
  }

  void showNoWlan(void) {
      Serial.println("showNoWlan");
      CRGB col = CRGB::DarkRed;
      col.nscale8_video(lumaScale);
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

  void showWlan(CRGB col = CRGB::Green) {
      Serial.println("showWlan");
      col.nscale8_video(lumaScale);
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

#define IP_OCTET_NUM  4
  CRGB ipColor[IP_OCTET_NUM] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow};

  void testAllWords() {
      Serial.print("test all words");
      for (unsigned int i = 0; i < (sizeof(patterns)/sizeof(patterns[0])); i++) {
            clearClockLeds();
            prevWordIndices[0] = i;
            const uint8_t *pattern = patterns[prevWordIndices[0]].pattern;
            int letterCnt = 0;
            while (pattern[letterCnt] != 255) {
              leds[pattern[letterCnt]] = ipColor[i % 4];
              letterCnt++;
            }
            FastLED.show();
            delay(1000);
      }
  }

  void showIp(IPAddress ip) {
      Serial.print("showIp: ");
      Serial.println(ip);
      for (uint8_t i=0; i<IP_OCTET_NUM; i++) {
        uint8_t octet = ip[i];
        bool leadingNonZeroDigit = false;
        for (int8_t j=0; j<3; j++) {
          uint8_t digit = (octet / (uint8_t)(pow(10,2-j))) % 10;
          if (digit) {
            leadingNonZeroDigit = true;
            clearClockLeds();
            prevWordIndices[0] = hourToWord(digit, false);
            const uint8_t *pattern = patterns[prevWordIndices[0]].pattern;
            int letterCnt = 0;
            while (pattern[letterCnt] != 255) {
              leds[pattern[letterCnt]] = ipColor[i];
              letterCnt++;
            }
            FastLED.show();
            delay(750);
          } else if (leadingNonZeroDigit) {
            /* a zero in the middle or end of an octet */
            clearClockLeds();
            prevWordIndices[0] = WORDIDX_NULL;
            const uint8_t *pattern = patterns[prevWordIndices[0]].pattern;
            int letterCnt = 0;
            while (pattern[letterCnt] != 255) {
              leds[pattern[letterCnt]] = ipColor[i];
              letterCnt++;
            }
            FastLED.show();
            delay(750);
          }
        }
        if (i<IP_OCTET_NUM-1) {
          clearClockLeds();
          prevWordIndices[0] = WORDIDX_DOT;
          const uint8_t *pattern = patterns[prevWordIndices[0]].pattern;
          int letterCnt = 0;
          while (pattern[letterCnt] != 255) {
            leds[pattern[letterCnt]] = CRGB::White;
            letterCnt++;
          }
          FastLED.show();
        }
        delay(1000);
      }
      prevWordIndices[1] = WORDIDX_STOP;
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
    WORDIDX_DOT = 32,
    WORDIDX_NULL = 33,
    WORDIDX_BASE_MIN = WORDIDX_M_EINE - 1,
    WORDIDX_BASE_HOUR = WORDIDX_H_EINS - 1,
    WORDIDX_STOP = 255,
  };

  int currentHour = 25;
  int currentMinute = 61;

  CRGB leds[NUM_LEDS];
  uint8_t lumaScale = 128;

  uint8_t wordIndices[MAX_NUM_WORDS];       /**< the indices off all words to be shown */
  uint8_t prevWordIndices[MAX_NUM_WORDS];   /**< shadow of the indices to clear them in the next minute */
  CRGB    wordColor[MAX_NUM_WORDS];         /**< the color for each word, aligned with wordIndices[] */

  CRGB colorMinutesNumeral = CRGB::Yellow;  /**< numeral for the minute, e.g. VIER */
  CRGB colorMinWord = CRGB::Blue;           /**< the word MIN */
  CRGB colorPreWord = CRGB::Red;            /**< the words VOR and NACH */
  CRGB colorQuarterWord = CRGB::White;      /**< the words VIERTEL, HALB, DREIVIERTEL */
  CRGB colorHoursNumeral = CRGB::Green;     /**< numeral for the hour, e.g DREI */
  CRGB colorClockWord = CRGB::Pink;         /**< the word UHR */

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
      wordIndices[0] = hourToWord(hour, true);                wordColor[0] = colorHoursNumeral;
      wordIndices[1] = WORDIDX_UHR;                           wordColor[1] = colorClockWord;
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute <= 10) {
      wordIndices[0] = minuteToWord(minute);                  wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_NACH;                          wordColor[2] = colorPreWord;
      wordIndices[3] = hourToWord(hour, true);                wordColor[3] = colorHoursNumeral;
      wordIndices[4] = WORDIDX_UHR;                           wordColor[4] = colorClockWord;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute < 15) {
      wordIndices[0] = minuteToWord(15 - minute);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_VOR;                           wordColor[2] = colorPreWord;
      wordIndices[3] = WORDIDX_VIERTEL;                       wordColor[3] = colorQuarterWord;
      wordIndices[4] = hourToWord(hour + 1, false);           wordColor[4] = colorHoursNumeral;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute == 15) {
      wordIndices[0] = WORDIDX_VIERTEL;                       wordColor[0] = colorQuarterWord;
      wordIndices[1] = hourToWord(hour + 1, false);           wordColor[1] = colorHoursNumeral;
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute < 20) {
      wordIndices[0] = minuteToWord(minute - 15);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_NACH;                          wordColor[2] = colorPreWord;
      wordIndices[3] = WORDIDX_VIERTEL;                       wordColor[3] = colorQuarterWord;
      wordIndices[4] = hourToWord(hour + 1, false);           wordColor[4] = colorHoursNumeral;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute < 30) {
      wordIndices[0] = minuteToWord(30 - minute);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_VOR;                           wordColor[2] = colorPreWord;
      wordIndices[3] = WORDIDX_HALB;                          wordColor[3] = colorQuarterWord;
      wordIndices[4] = hourToWord(hour + 1, false);           wordColor[4] = colorHoursNumeral;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute == 30) {
      wordIndices[0] = WORDIDX_HALB;                          wordColor[0] = colorQuarterWord;
      wordIndices[1] = hourToWord(hour + 1, false);           wordColor[1] = colorHoursNumeral;
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute <= 40) {
      wordIndices[0] = minuteToWord(minute - 30);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_NACH;                          wordColor[2] = colorPreWord;
      wordIndices[3] = WORDIDX_HALB;                          wordColor[3] = colorQuarterWord;
      wordIndices[4] = hourToWord(hour + 1, false);           wordColor[4] = colorHoursNumeral;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute < 45) {
      wordIndices[0] = minuteToWord(45 - minute);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_VOR;                           wordColor[2] = colorPreWord;
      wordIndices[3] = WORDIDX_DREIVIERTEL;                   wordColor[3] = colorQuarterWord;
      wordIndices[4] = hourToWord((hour + 1) % 12, false);    wordColor[4] = colorHoursNumeral;
      wordIndices[5] = WORDIDX_STOP;
    } else if (minute == 45) {
      wordIndices[0] = WORDIDX_DREIVIERTEL;                   wordColor[0] = colorQuarterWord;
      wordIndices[1] = hourToWord(hour + 1, false);           wordColor[1] = colorHoursNumeral;
      wordIndices[2] = WORDIDX_STOP;
    } else if (minute < 50) {
      wordIndices[0] = minuteToWord(minute - 45);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_NACH;                          wordColor[2] = colorPreWord;
      wordIndices[3] = WORDIDX_DREIVIERTEL;                   wordColor[3] = colorQuarterWord;
      wordIndices[4] = hourToWord(hour + 1, false);           wordColor[4] = colorHoursNumeral;
      wordIndices[5] = WORDIDX_STOP;
    } else {
      wordIndices[0] = minuteToWord(60 - minute);             wordColor[0] = colorMinutesNumeral;
      wordIndices[1] = WORDIDX_MIN;                           wordColor[1] = colorMinWord;
      wordIndices[2] = WORDIDX_VOR;                           wordColor[2] = colorPreWord;
      wordIndices[3] = hourToWord(hour + 1, true);            wordColor[3] = colorHoursNumeral;
      wordIndices[4] = WORDIDX_UHR;                           wordColor[4] = colorClockWord;
      wordIndices[5] = WORDIDX_STOP;
    }
  }

  void setClockLeds(int hour, int minute) {
    int wordCnt = 0;
    CRGB col;

    // First we evaluate which words we need to highlight
    getWords(hour, minute);

    // We remember them for clearing one minute later
    memcpy(prevWordIndices, wordIndices, sizeof(prevWordIndices));
    Serial.print("  words to set: ");
    while (wordIndices[wordCnt] != WORDIDX_STOP) {
      Serial.print(wordIndices[wordCnt]);
      Serial.print(" ");
      // scale the brightness
      col = wordColor[wordCnt];
      col.nscale8_video(lumaScale);
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
