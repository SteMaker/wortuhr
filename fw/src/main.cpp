#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>

#include "LedCtrl.h"
#include "Ota.h"
#include "Persistent.h"
#include "TimeGetter.h"
#include "Brightness.h"
#include "html_cfg.h"
#include "html_wifi_cfg.h"

enum Mode { MODE_INITIAL_CONFIG, MODE_NORMAL };

/* VARIABLES */
Mode mode;

LedCtrl ledCtrl;
Ota ota;
TimeGetter timeGetter;
AsyncWebServer server(80);
Persistent persistent;
Brightness brightness;

bool factoryReset = false;

void ICACHE_RAM_ATTR cfgBtnPressed(void) {
  factoryReset = true;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void getTime(int &h, int &m) {
    timeGetter.getTime(h, m);
    h += persistent.timeZoneOffset() + (persistent.dayLightSaving() ? 1 : 0);
}

void setupForInitialConfig(void) {
  mode = MODE_INITIAL_CONFIG;

  IPAddress local_IP(192, 168, 25, 1);
  IPAddress gateway(192, 168, 25, 1);
  IPAddress subnet(255, 255, 255, 0);

  // Enable WiFi accesspoint so that user can configure its WiFi SSID and
  // password
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("WORTUHR_AP");
  Serial.print("softAP IP:");
  Serial.println(WiFi.softAPIP());

  ledCtrl.setup();
  ledCtrl.showWlan();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Client connected");
    request->send(200, "text/html", FPSTR(htmlCfgWifi));
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("Configuration set");
    if (request->hasParam("ssid", true)) {
      persistent.ssid(request->getParam("ssid", true)->value().c_str());
      Serial.print("SSID set to: ");
      Serial.println(persistent.ssid().c_str());
      request->send(200, "text/html", FPSTR(htmlCfgWifiConfirm));
    } else {
      request->send(200, "text/html", FPSTR(htmlCfgReject));
      Serial.println("No SSID");
    }
    if (request->hasParam("passwd", true)) {
      persistent.wifiPwd(request->getParam("passwd", true)->value().c_str());
      Serial.print("WiFi password set to: ");
      Serial.println(persistent.wifiPwd().c_str());
    } else {
      Serial.println("No WiFi password");
    }
    persistent.updateToFlash();
    ESP.restart();
  });
}

void setColorFromJson(JsonObject &jsonObj, bool persist) {
  // Luma
  uint8_t luma = persistent.luma();
  if (jsonObj.containsKey("luma")) {
    luma = jsonObj["luma"];
    if (persist) persistent.luma(luma);
  } else {
    Serial.println("No Luma value found");
  }

  // colorMinutesNumeral
  Persistent::Color c;
  c = persistent.color(Persistent::minutesNumeral);
  if (jsonObj.containsKey("colorMinutesNumeralHue")) {
    c.hue = jsonObj["colorMinutesNumeralHue"];
  }
  if (jsonObj.containsKey("colorMinutesNumeralSat")) {
    c.sat = jsonObj["colorMinutesNumeralSat"];
  }
  if (jsonObj.containsKey("colorMinutesNumeralLumaOffset")) {
    c.lumaOffset = jsonObj["colorMinutesNumeralLumaOffset"];
  }
  if (persist) persistent.color(Persistent::minutesNumeral, c);
  Serial.print("New colorMinutesNumeral: ");
  Serial.print("hue: ");
  Serial.print(c.hue);
  Serial.print(", sat: ");
  Serial.print(c.sat);
  Serial.print(", lumaOffset: ");
  Serial.println(c.lumaOffset);
  ledCtrl.setColor(LedCtrl::minutesNumeral, c.hue, c.sat, c.lumaOffset, luma);

  // colorMinWord
  c = persistent.color(Persistent::minWord);
  if (jsonObj.containsKey("colorMinWordHue")) {
    c.hue = jsonObj["colorMinWordHue"];
  }
  if (jsonObj.containsKey("colorMinWordSat")) {
    c.sat = jsonObj["colorMinWordSat"];
  }
  if (jsonObj.containsKey("colorMinWordLumaOffset")) {
    c.lumaOffset = jsonObj["colorMinWordLumaOffset"];
  }
  if (persist) persistent.color(Persistent::minWord, c);
  Serial.print("New colorMinWord: ");
  Serial.print("hue: ");
  Serial.print(c.hue);
  Serial.print(", sat: ");
  Serial.print(c.sat);
  Serial.print(", lumaOffset: ");
  Serial.println(c.lumaOffset);
  ledCtrl.setColor(LedCtrl::minWord, c.hue, c.sat, c.lumaOffset, luma);

  // colorPreWord
  c = persistent.color(Persistent::preWord);
  if (jsonObj.containsKey("colorPreWordHue")) {
    c.hue = jsonObj["colorPreWordHue"];
  }
  if (jsonObj.containsKey("colorPreWordSat")) {
    c.sat = jsonObj["colorPreWordSat"];
  }
  if (jsonObj.containsKey("colorPreWordLumaOffset")) {
    c.lumaOffset = jsonObj["colorPreWordLumaOffset"];
  }
  if (persist) persistent.color(Persistent::preWord, c);
  Serial.print("New colorPreWord: ");
  Serial.print("hue: ");
  Serial.print(c.hue);
  Serial.print(", sat: ");
  Serial.print(c.sat);
  Serial.print(", lumaOffset: ");
  Serial.println(c.lumaOffset);
  ledCtrl.setColor(LedCtrl::preWord, c.hue, c.sat, c.lumaOffset, luma);

  // quarterWord
  c = persistent.color(Persistent::quarterWord);
  if (jsonObj.containsKey("colorQuarterWordHue")) {
    c.hue = jsonObj["colorQuarterWordHue"];
  }
  if (jsonObj.containsKey("colorQuarterWordSat")) {
    c.sat = jsonObj["colorQuarterWordSat"];
  }
  if (jsonObj.containsKey("colorQuarterWordLumaOffset")) {
    c.lumaOffset = jsonObj["colorQuarterWordLumaOffset"];
  }
  if (persist) persistent.color(Persistent::quarterWord, c);
  Serial.print("New colorQuarterWord: ");
  Serial.print("hue: ");
  Serial.print(c.hue);
  Serial.print(", sat: ");
  Serial.print(c.sat);
  Serial.print(", lumaOffset: ");
  Serial.println(c.lumaOffset);
  ledCtrl.setColor(LedCtrl::quarterWord, c.hue, c.sat, c.lumaOffset, luma);

  // hoursNumeral
  c = persistent.color(Persistent::hoursNumeral);
  if (jsonObj.containsKey("colorHoursNumeralHue")) {
    c.hue = jsonObj["colorHoursNumeralHue"];
  }
  if (jsonObj.containsKey("colorHoursNumeralSat")) {
    c.sat = jsonObj["colorHoursNumeralSat"];
  }
  if (jsonObj.containsKey("colorHoursNumeralLumaOffset")) {
    c.lumaOffset = jsonObj["colorHoursNumeralLumaOffset"];
  }
  if (persist) persistent.color(Persistent::hoursNumeral, c);
  Serial.print("New colorHoursNumeral: ");
  Serial.print("hue: ");
  Serial.print(c.hue);
  Serial.print(", sat: ");
  Serial.print(c.sat);
  Serial.print(", lumaOffset: ");
  Serial.println(c.lumaOffset);
  ledCtrl.setColor(LedCtrl::hoursNumeral, c.hue, c.sat, c.lumaOffset, luma);

  // clockWord
  c = persistent.color(Persistent::clockWord);
  if (jsonObj.containsKey("colorClockWordHue")) {
    c.hue = jsonObj["colorClockWordHue"];
  }
  if (jsonObj.containsKey("colorClockWordSat")) {
    c.sat = jsonObj["colorClockWordSat"];
  }
  if (jsonObj.containsKey("colorClockWordLumaOffset")) {
    c.lumaOffset = jsonObj["colorClockWordLumaOffset"];
  }
  if (persist) persistent.color(Persistent::clockWord, c);
  Serial.print("New colorClockWord: ");
  Serial.print("hue: ");
  Serial.print(c.hue);
  Serial.print(", sat: ");
  Serial.print(c.sat);
  Serial.print(", lumaOffset: ");
  Serial.println(c.lumaOffset);
  ledCtrl.setColor(LedCtrl::clockWord, c.hue, c.sat, c.lumaOffset, luma);

  ledCtrl.forceUpdate = true;
}

char configBuffer[1080]; // 1080 comes from ArduinoJson Assistant
char timedBuffer[75]; // 75 comes from ArduinoJson Assistant
void setupForNormal(void) {
  mode = MODE_NORMAL;

  Serial.print("Connecting to SSID: ");
  Serial.println(persistent.ssid().c_str());

  ledCtrl.setup();
  ledCtrl.showNoWlan();

  // Set WiFi to station mode and disconnect from an AP if it was previously
  // connected
  WiFi.mode(WIFI_STA);
  if (strcmp(persistent.hostname().c_str(), ""))
    WiFi.hostname(persistent.hostname().c_str());
  WiFi.begin(persistent.ssid().c_str(), persistent.wifiPwd().c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ledCtrl.showWlan();
  delay(1000);  // indicate the connection before switching to the time

  ota.setup();
  timeGetter.setup();
  brightness.setup(persistent.dim().base, persistent.dim().scale);

  // Serving the standard configuration page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Client connected");
    Serial.print("  free heap: ");
    Serial.println(ESP.getFreeHeap());
    Serial.print("  fragmentation: ");
    Serial.println(ESP.getHeapFragmentation());
    Serial.print("  max block size: ");
    Serial.println(ESP.getMaxFreeBlockSize());

    request->send_P(200, "text/html", htmlCfg);
    Serial.print("size of the page that was sent: ");
    Serial.println(sizeof(htmlCfg));
    Serial.print("  free heap: ");
    Serial.println(ESP.getFreeHeap());
    Serial.print("  fragmentation: ");
    Serial.println(ESP.getHeapFragmentation());
    Serial.print("  max block size: ");
    Serial.println(ESP.getMaxFreeBlockSize());
  });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Configuration requested");
    const size_t capacity =
        JSON_OBJECT_SIZE(30);  // adjust to the number of elements
    DynamicJsonDocument doc(capacity);
    doc["hostname"] = persistent.hostname().c_str();
    doc["timeZoneOffset"] = persistent.timeZoneOffset();
    doc["dayLightSaving"] = persistent.dayLightSaving();
    doc["luma"] = persistent.luma();
    doc["colorMinutesNumeralHue"] =
        persistent.color(Persistent::minutesNumeral).hue;
    doc["colorMinutesNumeralSat"] =
        persistent.color(Persistent::minutesNumeral).sat;
    doc["colorMinutesNumeralLumaOffset"] =
        persistent.color(Persistent::minutesNumeral).lumaOffset;
    doc["colorMinWordHue"] = persistent.color(Persistent::minWord).hue;
    doc["colorMinWordSat"] = persistent.color(Persistent::minWord).sat;
    doc["colorMinWordLumaOffset"] =
        persistent.color(Persistent::minWord).lumaOffset;
    doc["colorPreWordHue"] = persistent.color(Persistent::preWord).hue;
    doc["colorPreWordSat"] = persistent.color(Persistent::preWord).sat;
    doc["colorPreWordLumaOffset"] =
        persistent.color(Persistent::preWord).lumaOffset;
    doc["colorQuarterWordHue"] = persistent.color(Persistent::quarterWord).hue;
    doc["colorQuarterWordSat"] = persistent.color(Persistent::quarterWord).sat;
    doc["colorQuarterWordLumaOffset"] =
        persistent.color(Persistent::quarterWord).lumaOffset;
    doc["colorHoursNumeralHue"] =
        persistent.color(Persistent::hoursNumeral).hue;
    doc["colorHoursNumeralSat"] =
        persistent.color(Persistent::hoursNumeral).sat;
    doc["colorHoursNumeralLumaOffset"] =
        persistent.color(Persistent::hoursNumeral).lumaOffset;
    doc["colorClockWordHue"] = persistent.color(Persistent::clockWord).hue;
    doc["colorClockWordSat"] = persistent.color(Persistent::clockWord).sat;
    doc["colorClockWordLumaOffset"] =
        persistent.color(Persistent::clockWord).lumaOffset;
    doc["nightOffActive"] = persistent.nightOff().active;
    doc["nightOffOffHour"] = persistent.nightOff().offHour;
    doc["nightOffOffMinute"] = persistent.nightOff().offMinute;
    doc["nightOffOnHour"] = persistent.nightOff().onHour;
    doc["nightOffOnMinute"] = persistent.nightOff().onMinute;
    doc["dimActive"] = persistent.dim().active;
    doc["dimBase"] = persistent.dim().base;
    doc["dimScale"] = persistent.dim().scale;
    // Serial.print("Size of configuration: ");
    // Serial.println(measureJson(doc));
    serializeJson(doc, configBuffer, sizeof(configBuffer));
    request->send(200, "application/json", configBuffer);
  });

  // time and light intensity getter
  server.on("/timedData", HTTP_GET, [](AsyncWebServerRequest *request) {
    const size_t capacity = JSON_OBJECT_SIZE(3);
    DynamicJsonDocument doc(capacity);
    int h, m;
    getTime(h, m);
    doc["timeH"] = h;
    doc["timeM"] = m;
    doc["lightIntensity"] = 0;
    serializeJson(doc, timedBuffer, sizeof(timedBuffer));
    request->send(200, "application/json", timedBuffer);
  });


  // retrieving the configuration
  AsyncCallbackJsonWebHandler *cfgHandler = new AsyncCallbackJsonWebHandler(
      "/config", [](AsyncWebServerRequest *request, JsonVariant &json) {
        Serial.println("Configuration received");
        JsonObject jsonObj = json.as<JsonObject>();
        if (jsonObj.containsKey("hostname")) {
          const char *hostname = jsonObj["hostname"];
             persistent.hostname(hostname);
        } else {
          Serial.println("No hostname found");
        }
        if (jsonObj.containsKey("timeZoneOffset")) {
          int timeZoneOffset = jsonObj["timeZoneOffset"];
          persistent.timeZoneOffset(timeZoneOffset);
        } else {
          Serial.println("No time zone offset found");
        }
        if (jsonObj.containsKey("dayLightSaving")) {
          bool dayLightSaving = jsonObj["dayLightSaving"];
          persistent.dayLightSaving(dayLightSaving);
        } else {
          Serial.println("No day light saving found");
        }

        setColorFromJson(jsonObj, true);

        Persistent::NightOff no;
        if (jsonObj.containsKey("nightOffActive")) {
          no.active = jsonObj["nightOffActive"];
        }
        if (jsonObj.containsKey("nightOffOffHour")) {
          no.offHour = jsonObj["nightOffOffHour"];
        }
        if (jsonObj.containsKey("nightOffOffMinute")) {
          no.offMinute = jsonObj["nightOffOffMinute"];
        }
        if (jsonObj.containsKey("nightOffOnHour")) {
          no.onHour = jsonObj["nightOffOnHour"];
        }
        if (jsonObj.containsKey("nightOffOnMinute")) {
          no.onMinute = jsonObj["nightOffOnMinute"];
        }
        persistent.nightOff(no);

        Persistent::Dim d;
        if (jsonObj.containsKey("dimActive")) {
          d.active = jsonObj["dimActive"];
        }
        if (jsonObj.containsKey("dimBase")) {
          d.base = jsonObj["dimBase"];
        }
        if (jsonObj.containsKey("dimScale")) {
          d.scale = jsonObj["dimScale"];
        }
        persistent.dim(d);
        brightness.setScaleParams(d.base, d.scale);
        persistent.print();

        persistent.updateToFlash();
        request->send(200, "application/json", "{}");
      });
  server.addHandler(cfgHandler);

  // color setter
  AsyncCallbackJsonWebHandler *colorHandler = new AsyncCallbackJsonWebHandler(
      "/color", [](AsyncWebServerRequest *request, JsonVariant &json) {
        Serial.println("Color received");
        JsonObject jsonObj = json.as<JsonObject>();
        setColorFromJson(jsonObj, false);
        request->send(200, "application/json", "{}");
      });
  server.addHandler(colorHandler);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    persistent.setup();

    if (!strcmp(persistent.ssid().c_str(), "")) {
      setupForInitialConfig();
    } else {
      setupForNormal();
    }

    server.onNotFound(notFound);
    server.begin();

    pinMode(14, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(14), cfgBtnPressed, FALLING);

    Serial.println("Setup done");
}

// is (h2:m2) later or equal (h1:m1)
bool isLaterEqual(int h1, int m1, int h2, int m2) {
  if( (h2 > h1) || ((h2 == h1) && (m2 > m1)) || ((h2 == h1) && (m2 == m1)) )
      return true;
  return false;
}

bool withinActiveTimeWindow(int hour, int minute) {
  Persistent::NightOff no = persistent.nightOff();
  if(!no.active) return true;
  if(!isLaterEqual(no.offHour, no.offMinute, no.onHour, no.onMinute)) {
    // off time crosses midnight
    if(isLaterEqual(no.offHour, no.offMinute, hour, minute) ||
       !isLaterEqual(no.onHour, no.onMinute, hour, minute))
       return false;
    return true;
  } else {
    if(isLaterEqual(no.offHour, no.offMinute, hour, minute) &&
       !isLaterEqual(no.onHour, no.onMinute, hour, minute))
       return false;
    return true;
  }
}

void loop() {
    ota.loop();
    timeGetter.loop();
    if(factoryReset) {
      factoryReset = false;
      ledCtrl.clear();
      persistent.factoryReset();
      ESP.restart();
    }

    if (mode == MODE_NORMAL) {
      int h;
      int m;
      getTime(h, m);

      if(persistent.dim().active) {
        uint8_t lumaScale = brightness.getLumaScale();
        ledCtrl.setLumaScale(lumaScale);
      } else {
        ledCtrl.setLumaScale(255);
      }

      static bool wasWithinActTimeWindow = false;
      bool isWithinActTimeWindow = withinActiveTimeWindow(h, m);
      if (isWithinActTimeWindow) 
        ledCtrl.setClock(h, m);
      else if (wasWithinActTimeWindow) {
        Serial.println("now out of active time window, turning LEDs off");
        ledCtrl.clear();
      }
      wasWithinActTimeWindow = isWithinActTimeWindow;

      //delay(250);
    } else {
      static unsigned int timeout = 15*60*2;
      timeout--;
      if(!timeout) {
        // Disable the softAP after 15 minutes
        Serial.println("Disabling softAP");
        ledCtrl.showNoWlan();
        WiFi.mode(WIFI_OFF);
        WiFi.forceSleepBegin();
      }
      delay(500);
    }

}
