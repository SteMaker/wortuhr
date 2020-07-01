#include <CRCx.h>
#include <ESP_EEPROM.h>

class Persistent {
  public:
    struct Color {
      Color() { r=g=b=0; }
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };

    struct NightOff {
      NightOff(void) { active=false;offHour=offMinute=onHour=onMinute=0; };
      bool active; // true -> Turn off LEDs at night
      uint8_t offHour;
      uint8_t offMinute;
      uint8_t onHour;
      uint8_t onMinute;
    };

    struct Dim {
      Dim(void) { active=false; base=scale=0; }
      bool active; // true -> dim based on measured light
      uint8_t base; // lowest level to use
      uint8_t scale; // How aggressivly to dim based on measured light
    };

    enum ColorType {
        minutesNumeral,
        minWord,
        preWord,
        quarterWord,
        hoursNumeral,
        clockWord
    };

   private:
    struct Config {
      char magic[4];
      char hostname[16];
      char ssid[33]; // Max SSID length: 32, plus \0
      char wifiPwd[64]; // Max WPA2 Key: 63, plus \0
      int timeZoneOffset;
      bool dayLightSaving;
      Color colorMinutesNumeral;
      Color colorMinWord;
      Color colorPreWord;
      Color colorQuarterWord;
      Color colorHoursNumeral;
      Color colorClockWord;
      int dummy;
      NightOff nightOff;
      Dim dim;
      uint8_t crc; // always keep this as the last element
    };

    Config config;

    void setToDefaults() {
      Serial.println("Initialize EEPROM to defaults");
      strlcpy(config.magic, "CLK", 4);
      strlcpy(config.hostname, "", sizeof(config.hostname));
      strlcpy(config.ssid, "", sizeof(config.ssid));
      strlcpy(config.wifiPwd, "", sizeof(config.wifiPwd));
      config.timeZoneOffset = 1; // CET
      config.dayLightSaving = false;
      // 0xff is too bright and draws too much current
      config.colorMinutesNumeral.r = 50;
      config.colorMinutesNumeral.g = 50;
      config.colorMinutesNumeral.b = 0;
      config.colorMinWord.r = 0;
      config.colorMinWord.g = 0;
      config.colorMinWord.b = 50;
      config.colorPreWord.r = 50;
      config.colorPreWord.g = 0;
      config.colorPreWord.b = 0;
      config.colorQuarterWord.r = 50;
      config.colorQuarterWord.g = 50;
      config.colorQuarterWord.b = 50;
      config.colorHoursNumeral.r = 0;
      config.colorHoursNumeral.g = 50;
      config.colorHoursNumeral.b = 0;
      config.colorClockWord.r = 50;
      config.colorClockWord.g = 50;
      config.colorClockWord.b = 50;
      config.nightOff.active = false;
      config.nightOff.offHour = 22;
      config.nightOff.offMinute = 30;
      config.nightOff.onHour = 6;
      config.nightOff.onMinute = 0;
      config.dim.active = false;
      config.dim.base = 128; // lowest level to use
      config.dim.scale = 255;
    }

    bool isConsistent(void) {
      if (strncmp(config.magic, "CLK", 4) != 0) {
        Serial.println("MAGIC in EEPROM is wrong!");
        return false;
      }
      uint8_t crc = calcCrc();
      if (crc != config.crc) {
        Serial.println("CRC is wrong!");
        Serial.print("Calc CRC8: ");
        Serial.println(crc);
        Serial.print("Stored CRC8: ");
        Serial.println(config.crc);
        return false;
      }
      return true;
    }

  public:
    void hostname(const char *n) {
      strlcpy(config.hostname, n, sizeof(config.hostname));
    }
    const char* hostname(void) {
      return config.hostname;
    }
    void ssid(const char *s) {
      strlcpy(config.ssid, s, sizeof(config.ssid));
    }
    const char* ssid(void) {
      return config.ssid;
    }
    void wifiPwd(const char *p) {
      strlcpy(config.wifiPwd, p, sizeof(config.wifiPwd));
    }
    const char* wifiPwd(void) {
      return config.wifiPwd;
    }
    void timeZoneOffset(int t) {
      config.timeZoneOffset = t;
    }
    int timeZoneOffset(void) {
      return config.timeZoneOffset;
    }
    void dayLightSaving(bool d) {
      config.dayLightSaving = d;
    }
    bool dayLightSaving(void) {
      return config.dayLightSaving;
    }
    void color(ColorType t, Color c) {
      switch (t) {
        case minutesNumeral:
          config.colorMinutesNumeral = c;
        case minWord:
          config.colorMinWord = c;
        case preWord:
          config.colorPreWord = c;
        case quarterWord:
          config.colorQuarterWord = c;
        case hoursNumeral:
          config.colorHoursNumeral = c;
        case clockWord:
          config.colorClockWord = c;
      }
    }
    Color color(ColorType t) {
      switch (t) {
        case minutesNumeral:
          return config.colorMinutesNumeral;
        case minWord:
          return config.colorMinWord;
        case preWord:
          return config.colorPreWord;
        case quarterWord:
          return config.colorQuarterWord;
        case hoursNumeral:
          return config.colorHoursNumeral;
        case clockWord:
          return config.colorClockWord;
        default:
          Color dummy;
          return dummy;
      }
    }
    void nightOff(NightOff n) {
      config.nightOff = n;
    }
    NightOff nightOff (void) {
      return config.nightOff;
    }
    void dim(Dim d) {
      config.dim = d;
    }
    Dim dim(void) {
      return config.dim;
    }

    void setup(void) {
      EEPROM.begin(sizeof(Config));

      if (EEPROM.percentUsed() < 0) {
        // If nothing has been written to the EEPROM yet, we need to initialize it
        Serial.println("Fresh EEPROM");
        setToDefaults();
        updateToFlash();
      } else {
        // Load the EEPROM
        EEPROM.get(0, config);
        if (!isConsistent()) {
          // Something is broken in the flash content, we'll wipe everything to be on the safe side
          EEPROM.wipe();
          setToDefaults();
          updateToFlash();
        } else {
          Serial.println("EEPROM loaded and valid");
        }
      }
    }

    void updateToFlash() {
      Serial.println("Committing EEPROM");
      config.crc = calcCrc();
      EEPROM.put(0, config);
      boolean ok = EEPROM.commit();
      Serial.println((ok) ? "Commit OK" : "Commit failed");
    }

    void factoryReset(void) {
      Serial.println("Performing factory reset");
      setToDefaults();
      updateToFlash();
    }

    void print(void) {
      Serial.print("hostname: ");
      Serial.println(hostname());
      Serial.print("ssid: ");
      Serial.println(ssid());
      Serial.print("wifiPwd: ");
      Serial.println("**********");
      //Serial.println(wifiPwd());
      Serial.print("timeZoneOffset: ");
      Serial.println(timeZoneOffset());
      Serial.print("dayLightSaving: ");
      Serial.println(dayLightSaving());
      Serial.println("colorMinutesNumeral:");
      Serial.print("red: ");
      Serial.print(color(minutesNumeral).r);
      Serial.print(", green: ");
      Serial.print(color(minutesNumeral).g);
      Serial.print(", blue: ");
      Serial.println(color(minutesNumeral).b);
      Serial.println("colorMinWord:");
      Serial.print("red: ");
      Serial.print(color(minWord).r);
      Serial.print(", green: ");
      Serial.print(color(minWord).g);
      Serial.print(", blue: ");
      Serial.println(color(minWord).b);
      Serial.println("colorPreWord:");
      Serial.print("red: ");
      Serial.print(color(preWord).r);
      Serial.print(", green: ");
      Serial.print(color(preWord).g);
      Serial.print(", blue: ");
      Serial.println(color(preWord).b);
      Serial.println("colorQuarterWord:");
      Serial.print("red: ");
      Serial.print(color(quarterWord).r);
      Serial.print(", green: ");
      Serial.print(color(quarterWord).g);
      Serial.print(", blue: ");
      Serial.println(color(quarterWord).b);
      Serial.println("colorHoursNumeral:");
      Serial.print("red: ");
      Serial.print(color(hoursNumeral).r);
      Serial.print(", green: ");
      Serial.print(color(hoursNumeral).g);
      Serial.print(", blue: ");
      Serial.println(color(hoursNumeral).b);
      Serial.println("colorClockWord:");
      Serial.print("red: ");
      Serial.print(color(clockWord).r);
      Serial.print(", green: ");
      Serial.print(color(clockWord).g);
      Serial.print(", blue: ");
      Serial.println(color(clockWord).b);
      Serial.print("nightOff active: ");
      Serial.println(nightOff().active);
      Serial.print("nightOff offH: ");
      Serial.println(nightOff().offHour);
      Serial.print("nightOff offM: ");
      Serial.println(nightOff().offMinute);
      Serial.print("nightOff onH: ");
      Serial.println(nightOff().onHour);
      Serial.print("nightOff onM: ");
      Serial.println(nightOff().onMinute);
      Serial.print("dim active: ");
      Serial.println(dim().active);
      Serial.print("dim base: ");
      Serial.println(dim().base);
      Serial.print("dim scale: ");
      Serial.println(dim().scale);
    }

    private:
    uint8_t calcCrc() {
      unsigned int size = offsetof(Config, crc);
      return crcx::crc8((uint8_t *)&config, size);
    }
};
