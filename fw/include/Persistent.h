#include <CRCx.h>
#include <ESP_EEPROM.h>

class Persistent {
  public:
    struct Color {
      Color() { hue=sat=lumaOffset=0; }
      uint8_t hue;
      uint8_t sat;
      sint8_t lumaOffset;
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
      bool showIp;
      int timeZoneOffset;
      bool dayLightSaving;
      uint8_t luma;
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
      config.showIp = true;   // always show the IP on a fresh clock
      config.timeZoneOffset = 1; // CET
      config.dayLightSaving = false;
      config.colorMinutesNumeral.hue = 42;
      config.colorMinutesNumeral.sat = 255;
      config.colorMinutesNumeral.lumaOffset = 0;
      config.colorMinWord.hue = 170;
      config.colorMinWord.sat = 255;
      config.colorMinWord.lumaOffset = 0;
      config.colorPreWord.hue = 0;
      config.colorPreWord.sat = 255;
      config.colorPreWord.lumaOffset = 0;
      config.colorQuarterWord.hue = 0;
      config.colorQuarterWord.sat = 0;
      config.colorQuarterWord.lumaOffset = 0;
      config.colorHoursNumeral.hue = 85;
      config.colorHoursNumeral.sat = 255;
      config.colorHoursNumeral.lumaOffset = 0;
      config.colorClockWord.hue = 0;
      config.colorClockWord.sat = 0;
      config.colorClockWord.lumaOffset = 0;
      config.luma = 25;
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
      noInterrupts();
      strlcpy(config.hostname, n, sizeof(config.hostname));
      interrupts();
    }
    std::string hostname(void) {
      noInterrupts();
      std::string t(config.hostname);
      interrupts();
      return t;
    }
    void ssid(const char *s) {
      noInterrupts();
      strlcpy(config.ssid, s, sizeof(config.ssid));
      interrupts();
    }
    std::string ssid(void) {
      noInterrupts();
      std::string t(config.ssid);
      interrupts();
      return t;
    }
    void wifiPwd(const char *p) {
      noInterrupts();
      strlcpy(config.wifiPwd, p, sizeof(config.wifiPwd));
      interrupts();
    }
    std::string wifiPwd(void) {
      noInterrupts();
      std::string t(config.wifiPwd);
      interrupts();
      return t;
    }
    void showIp(bool s) {
      // writing a bool is atomic
      config.showIp = s;
    }
    bool showIp(void) {
      // reading a bool is atomic
      return config.showIp;
    }
    void timeZoneOffset(int t) {
      // writing an int is atomic
      config.timeZoneOffset = t;
    }
    int timeZoneOffset(void) {
      // reading an int is atomic
      return config.timeZoneOffset;
    }
    void dayLightSaving(bool d) {
      // writing a bool is atomic
      config.dayLightSaving = d;
    }
    bool dayLightSaving(void) {
      // reading a bool is atomic
      return config.dayLightSaving;
    }
    void color(ColorType t, Color c) {
      noInterrupts();
      switch (t) {
        case minutesNumeral:
          config.colorMinutesNumeral = c;
          break;
        case minWord:
          config.colorMinWord = c;
          break;
        case preWord:
          config.colorPreWord = c;
          break;
        case quarterWord:
          config.colorQuarterWord = c;
          break;
        case hoursNumeral:
          config.colorHoursNumeral = c;
          break;
        case clockWord:
          config.colorClockWord = c;
          break;
      }
      interrupts();
    }
    Color color(ColorType t) {
      noInterrupts();
      Color c;
      switch (t) {
        case minutesNumeral:
          c = config.colorMinutesNumeral;
          break;
        case minWord:
          c = config.colorMinWord;
          break;
        case preWord:
          c = config.colorPreWord;
          break;
        case quarterWord:
          c = config.colorQuarterWord;
          break;
        case hoursNumeral:
          c = config.colorHoursNumeral;
          break;
        case clockWord:
          c = config.colorClockWord;
          break;
        default:
          Color dummy;
          c = dummy;
          break;
      }
      interrupts();
      return c;
    }
    void luma(uint8_t l) {
      noInterrupts();
      config.luma = l;
      interrupts();
    }
    uint8_t luma(void) {
      return config.luma;
    }
    void nightOff(NightOff n) {
      noInterrupts();
      config.nightOff = n;
      interrupts();
    }
    NightOff nightOff (void) {
      noInterrupts();
      NightOff t = config.nightOff;
      interrupts();
      return t;
    }
    void dim(Dim d) {
      noInterrupts();
      config.dim = d;
      interrupts();
    }
    Dim dim(void) {
      noInterrupts();
      Dim t = config.dim;
      interrupts();
      return t;
    }

    void setup(void) {
      EEPROM.begin(sizeof(Config));

      int eepPercentUsed = EEPROM.percentUsed();
      if (eepPercentUsed < 0) {
        // If nothing has been written to the EEPROM yet, we need to initialize it
        Serial.println("Fresh EEPROM");
        setToDefaults();
        updateToFlash();
      } else {
        Serial.printf("EEPROM usage: %d%%\n", eepPercentUsed);
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
      Serial.println(hostname().c_str());
      Serial.print("ssid: ");
      Serial.println(ssid().c_str());
      Serial.print("wifiPwd: ");
      Serial.println("**********");
      //Serial.println(wifiPwd());
      Serial.print("show IP: ");
      Serial.println(showIp());
      Serial.print("timeZoneOffset: ");
      Serial.println(timeZoneOffset());
      Serial.print("dayLightSaving: ");
      Serial.println(dayLightSaving());
      Serial.print("luma: ");
      Serial.println(luma());
      Serial.println("colorMinutesNumeral:");
      Serial.print("hue: ");
      Serial.print(color(minutesNumeral).hue);
      Serial.print(", sat: ");
      Serial.print(color(minutesNumeral).sat);
      Serial.print(", lumaOffset: ");
      Serial.println(color(minutesNumeral).lumaOffset);
      Serial.println("colorMinWord:");
      Serial.print("hue: ");
      Serial.print(color(minWord).hue);
      Serial.print(", sat: ");
      Serial.print(color(minWord).sat);
      Serial.print(", lumaOffset: ");
      Serial.println(color(minWord).lumaOffset);
      Serial.println("colorPreWord:");
      Serial.print("hue: ");
      Serial.print(color(preWord).hue);
      Serial.print(", sat: ");
      Serial.print(color(preWord).sat);
      Serial.print(", lumaOffset: ");
      Serial.println(color(preWord).lumaOffset);
      Serial.println("colorQuarterWord:");
      Serial.print("hue: ");
      Serial.print(color(quarterWord).hue);
      Serial.print(", sat: ");
      Serial.print(color(quarterWord).sat);
      Serial.print(", lumaOffset: ");
      Serial.println(color(quarterWord).lumaOffset);
      Serial.println("colorHoursNumeral:");
      Serial.print("hue: ");
      Serial.print(color(hoursNumeral).hue);
      Serial.print(", sat: ");
      Serial.print(color(hoursNumeral).sat);
      Serial.print(", lumaOffset: ");
      Serial.println(color(hoursNumeral).lumaOffset);
      Serial.println("colorClockWord:");
      Serial.print("hue: ");
      Serial.print(color(clockWord).hue);
      Serial.print(", sat: ");
      Serial.print(color(clockWord).sat);
      Serial.print(", lumaOffset: ");
      Serial.println(color(clockWord).lumaOffset);
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
