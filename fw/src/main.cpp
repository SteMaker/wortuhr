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
  if(strcmp(persistent.hostname().c_str(), ""))
      WiFi.hostname(persistent.hostname().c_str());
  WiFi.begin(persistent.ssid().c_str(), persistent.wifiPwd().c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ledCtrl.showWlan();
  delay(1000);       // indicate the connection before switching to the time

  ota.setup();
  timeGetter.setup();
  brightness.setup(persistent.dim().base, persistent.dim().scale);

  // Serving the standard configuration page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Client connected");
    Serial.print("  free heap: "); Serial.println(ESP.getFreeHeap());
    Serial.print("  fragmentation: "); Serial.println(ESP.getHeapFragmentation());
    Serial.print("  max block size: "); Serial.println(ESP.getMaxFreeBlockSize());

    request->send_P(200, "text/html", htmlCfg);
    Serial.print("size of the page that was sent: "); Serial.println(sizeof(htmlCfg));
    Serial.print("  free heap: "); Serial.println(ESP.getFreeHeap());
    Serial.print("  fragmentation: "); Serial.println(ESP.getHeapFragmentation());
    Serial.print("  max block size: "); Serial.println(ESP.getMaxFreeBlockSize());
  });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Configuration requested");
    const size_t capacity = JSON_OBJECT_SIZE(29);   // adjust to the number of elements
    DynamicJsonDocument doc(capacity);
    doc["hostname"] = persistent.hostname().c_str();
    doc["timeZoneOffset"] = persistent.timeZoneOffset();
    doc["dayLightSaving"] = persistent.dayLightSaving();
    doc["colorMinutesNumeralRed"] = persistent.color(Persistent::minutesNumeral).r;
    Serial.print("config r: ");
    Serial.println(persistent.color(Persistent::minutesNumeral).r);
    doc["colorMinutesNumeralGreen"] = persistent.color(Persistent::minutesNumeral).g;
    doc["colorMinutesNumeralBlue"] = persistent.color(Persistent::minutesNumeral).b;
    doc["colorMinWordRed"] = persistent.color(Persistent::minWord).r;
    doc["colorMinWordGreen"] = persistent.color(Persistent::minWord).g;
    doc["colorMinWordBlue"] = persistent.color(Persistent::minWord).b;
    doc["colorPreWordRed"] = persistent.color(Persistent::preWord).r;
    doc["colorPreWordGreen"] = persistent.color(Persistent::preWord).g;
    doc["colorPreWordBlue"] = persistent.color(Persistent::preWord).b;
    doc["colorQuarterWordRed"] = persistent.color(Persistent::quarterWord).r;
    doc["colorQuarterWordGreen"] = persistent.color(Persistent::quarterWord).g;
    doc["colorQuarterWordBlue"] = persistent.color(Persistent::quarterWord).b;
    doc["colorHoursNumeralRed"] = persistent.color(Persistent::hoursNumeral).r;
    doc["colorHoursNumeralGreen"] = persistent.color(Persistent::hoursNumeral).g;
    doc["colorHoursNumeralBlue"] = persistent.color(Persistent::hoursNumeral).b;
    doc["colorClockWordRed"] = persistent.color(Persistent::clockWord).r;
    doc["colorClockWordGreen"] = persistent.color(Persistent::clockWord).g;
    doc["colorClockWordBlue"] = persistent.color(Persistent::clockWord).b;
    doc["nightOffActive"] = persistent.nightOff().active;
    doc["nightOffOffHour"] = persistent.nightOff().offHour;
    doc["nightOffOffMinute"] = persistent.nightOff().offMinute;
    doc["nightOffOnHour"] = persistent.nightOff().onHour;
    doc["nightOffOnMinute"] = persistent.nightOff().onMinute;
    doc["dimActive"] = persistent.dim().active;
    doc["dimBase"] = persistent.dim().base;
    doc["dimScale"] = persistent.dim().scale;
    //Serial.print("Size of configuration: "); Serial.println(measureJson(doc));
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

        Persistent::Color c;
        if (jsonObj.containsKey("colorMinutesNumeralRed")) {
          c.r = jsonObj["colorMinutesNumeralRed"];
        }
        if (jsonObj.containsKey("colorMinutesNumeralGreen")) {
          c.g = jsonObj["colorMinutesNumeralGreen"];
        }
        if (jsonObj.containsKey("colorMinutesNumeralBlue")) {
          c.b = jsonObj["colorMinutesNumeralBlue"];
        }
        persistent.color(Persistent::minutesNumeral, c);
        ledCtrl.setColor(LedCtrl::minutesNumeral, c.r, c.g, c.b);

        if (jsonObj.containsKey("colorMinWordRed")) {
          c.r = jsonObj["colorMinWordRed"];
        }
        if (jsonObj.containsKey("colorMinWordGreen")) {
          c.g = jsonObj["colorMinWordGreen"];
        }
        if (jsonObj.containsKey("colorMinWordBlue")) {
          c.b = jsonObj["colorMinWordBlue"];
        }
        persistent.color(Persistent::minWord, c);
        ledCtrl.setColor(LedCtrl::minWord, c.r, c.g, c.b);

        if (jsonObj.containsKey("colorPreWordRed")) {
          c.r = jsonObj["colorPreWordRed"];
        }
        if (jsonObj.containsKey("colorPreWordGreen")) {
          c.g = jsonObj["colorPreWordGreen"];
        }
        if (jsonObj.containsKey("colorPreWordBlue")) {
          c.b = jsonObj["colorPreWordBlue"];
        }
        persistent.color(Persistent::preWord, c);
        ledCtrl.setColor(LedCtrl::preWord, c.r, c.g, c.b);

        if (jsonObj.containsKey("colorQuarterWordRed")) {
          c.r = jsonObj["colorQuarterWordRed"];
        }
        if (jsonObj.containsKey("colorQuarterWordGreen")) {
          c.g = jsonObj["colorQuarterWordGreen"];
        }
        if (jsonObj.containsKey("colorQuarterWordBlue")) {
          c.b = jsonObj["colorQuarterWordBlue"];
        }
        persistent.color(Persistent::quarterWord, c);
        ledCtrl.setColor(LedCtrl::quarterWord, c.r, c.g, c.b);

        if (jsonObj.containsKey("colorHoursNumeralRed")) {
          c.r = jsonObj["colorHoursNumeralRed"];
        }
        if (jsonObj.containsKey("colorHoursNumeralGreen")) {
          c.g = jsonObj["colorHoursNumeralGreen"];
        }
        if (jsonObj.containsKey("colorHoursNumeralBlue")) {
          c.b = jsonObj["colorHoursNumeralBlue"];
        }
        persistent.color(Persistent::hoursNumeral, c);
        ledCtrl.setColor(LedCtrl::hoursNumeral, c.r, c.g, c.b);

        if (jsonObj.containsKey("colorClockWordRed")) {
          c.r = jsonObj["colorClockWordRed"];
        }
        if (jsonObj.containsKey("colorClockWordGreen")) {
          c.g = jsonObj["colorClockWordGreen"];
        }
        if (jsonObj.containsKey("colorClockWordBlue")) {
          c.b = jsonObj["colorClockWordBlue"];
        }
        persistent.color(Persistent::clockWord, c);
        ledCtrl.setColor(LedCtrl::clockWord, c.r, c.g, c.b);

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
        ledCtrl.forceUpdate = true;
        request->send(200, "application/json", "{}");
      });
  server.addHandler(cfgHandler);

  // color setter
  AsyncCallbackJsonWebHandler *colorHandler = new AsyncCallbackJsonWebHandler(
      "/color", [](AsyncWebServerRequest *request, JsonVariant &json) {
        Serial.println("Color received");
        uint8_t r, g, b;
        JsonObject jsonObj = json.as<JsonObject>();
        r = persistent.color(Persistent::minutesNumeral).r;
        g = persistent.color(Persistent::minutesNumeral).g;
        b = persistent.color(Persistent::minutesNumeral).b;
        if (jsonObj.containsKey("colorMinutesNumeralRed")) {
          r = jsonObj["colorMinutesNumeralRed"];
        }
        if (jsonObj.containsKey("colorMinutesNumeralGreen")) {
          g = jsonObj["colorMinutesNumeralGreen"];
        }
        if (jsonObj.containsKey("colorMinutesNumeralBlue")) {
          b = jsonObj["colorMinutesNumeralBlue"];
        }
        Serial.print("New colorMinutesNumeral: ");
        Serial.print("red: "); Serial.print(r);
        Serial.print(", green: ");  Serial.print(g);
        Serial.print(", blue: ");  Serial.println(b);
        ledCtrl.setColor(LedCtrl::minutesNumeral, r, g, b);

        r = persistent.color(Persistent::minWord).r;
        g = persistent.color(Persistent::minWord).g;
        b = persistent.color(Persistent::minWord).b;
        if (jsonObj.containsKey("colorMinWordRed")) {
          r = jsonObj["colorMinWordRed"];
        }
        if (jsonObj.containsKey("colorMinWordGreen")) {
          g = jsonObj["colorMinWordGreen"];
        }
        if (jsonObj.containsKey("colorMinWordBlue")) {
          b = jsonObj["colorMinWordBlue"];
        }
        Serial.print("New colorMinWord: ");
        Serial.print("red: "); Serial.print(r);
        Serial.print(", green: ");  Serial.print(g);
        Serial.print(", blue: ");  Serial.println(b);
        ledCtrl.setColor(LedCtrl::minWord, r, g, b);

        r = persistent.color(Persistent::preWord).r;
        g = persistent.color(Persistent::preWord).g;
        b = persistent.color(Persistent::preWord).b;
        if (jsonObj.containsKey("colorPreWordRed")) {
          r = jsonObj["colorPreWordRed"];
        }
        if (jsonObj.containsKey("colorPreWordGreen")) {
          g = jsonObj["colorPreWordGreen"];
        }
        if (jsonObj.containsKey("colorPreWordBlue")) {
          b = jsonObj["colorPreWordBlue"];
        }
        Serial.print("New colorPreWord: ");
        Serial.print("red: "); Serial.print(r);
        Serial.print(", green: ");  Serial.print(g);
        Serial.print(", blue: ");  Serial.println(b);
        ledCtrl.setColor(LedCtrl::preWord, r, g, b);

        r = persistent.color(Persistent::quarterWord).r;
        g = persistent.color(Persistent::quarterWord).g;
        b = persistent.color(Persistent::quarterWord).b;
        if (jsonObj.containsKey("colorQuarterWordRed")) {
          r = jsonObj["colorQuarterWordRed"];
        }
        if (jsonObj.containsKey("colorQuarterWordGreen")) {
          g = jsonObj["colorQuarterWordGreen"];
        }
        if (jsonObj.containsKey("colorQuarterWordBlue")) {
          b = jsonObj["colorQuarterWordBlue"];
        }
        Serial.print("New colorQuarterWord: ");
        Serial.print("red: "); Serial.print(r);
        Serial.print(", green: ");  Serial.print(g);
        Serial.print(", blue: ");  Serial.println(b);
        ledCtrl.setColor(LedCtrl::quarterWord, r, g, b);

        r = persistent.color(Persistent::hoursNumeral).r;
        g = persistent.color(Persistent::hoursNumeral).g;
        b = persistent.color(Persistent::hoursNumeral).b;
        if (jsonObj.containsKey("colorHoursNumeralRed")) {
          r = jsonObj["colorHoursNumeralRed"];
        }
        if (jsonObj.containsKey("colorHoursNumeralGreen")) {
          g = jsonObj["colorHoursNumeralGreen"];
        }
        if (jsonObj.containsKey("colorHoursNumeralBlue")) {
          b = jsonObj["colorHoursNumeralBlue"];
        }
        Serial.print("New colorHoursNumeral: ");
        Serial.print("red: "); Serial.print(r);
        Serial.print(", green: ");  Serial.print(g);
        Serial.print(", blue: ");  Serial.println(b);
        ledCtrl.setColor(LedCtrl::hoursNumeral, r, g, b);

        r = persistent.color(Persistent::clockWord).r;
        g = persistent.color(Persistent::clockWord).g;
        b = persistent.color(Persistent::clockWord).b;
        if (jsonObj.containsKey("colorClockWordRed")) {
          r = jsonObj["colorClockWordRed"];
        }
        if (jsonObj.containsKey("colorClockWordGreen")) {
          g = jsonObj["colorClockWordGreen"];
        }
        if (jsonObj.containsKey("colorClockWordBlue")) {
          b = jsonObj["colorClockWordBlue"];
        }
        Serial.print("New colorClockWord: ");
        Serial.print("red: "); Serial.print(r);
        Serial.print(", green: ");  Serial.print(g);
        Serial.print(", blue: ");  Serial.println(b);
        ledCtrl.setColor(LedCtrl::clockWord, r, g, b);

        ledCtrl.forceUpdate = true;
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
        uint8_t luma = brightness.getLuma();
        ledCtrl.setLuma(luma);
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
