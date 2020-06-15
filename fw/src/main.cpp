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

  ledCtrl.setup(persistent.color().r, persistent.color().g, persistent.color().b);
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
      Serial.println(persistent.ssid());
      request->send(200, "text/html", FPSTR(htmlCfgWifiConfirm));
    } else {
      request->send(200, "text/html", FPSTR(htmlCfgReject));
      Serial.println("No SSID");
    }
    if (request->hasParam("passwd", true)) {
      persistent.wifiPwd(request->getParam("passwd", true)->value().c_str());
      Serial.print("WiFi password set to: ");
      Serial.println(persistent.wifiPwd());
    } else {
      Serial.println("No WiFi password");
    }
    persistent.updateToFlash();
    ESP.restart();
  });
}

char configBuffer[428]; // 428 comes from ArduinoJson Assistant
char timedBuffer[75]; // 75 comes from ArduinoJson Assistant
void setupForNormal(void) {
  mode = MODE_NORMAL;

  Serial.print("Connecting to SSID: ");
  Serial.println(persistent.ssid());

  ledCtrl.setup(persistent.color().r, persistent.color().g, persistent.color().b);
  ledCtrl.showNoWlan();

  // Set WiFi to station mode and disconnect from an AP if it was previously
  // connected
  WiFi.mode(WIFI_STA);
  if(strcmp(persistent.hostname(), ""))
      WiFi.hostname(persistent.hostname());
  WiFi.begin(persistent.ssid(), persistent.wifiPwd());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ledCtrl.showWlan();

  ota.setup();
  timeGetter.setup();
  brightness.setup(persistent.dim().base, persistent.dim().scale);

  // Serving the standard configuration page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Client connected");
    request->send(200, "text/html", FPSTR(htmlCfg));
  });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Configuration requested");
    const size_t capacity = JSON_OBJECT_SIZE(14);
    DynamicJsonDocument doc(capacity);
    doc["hostname"] = persistent.hostname();
    doc["timeZoneOffset"] = persistent.timeZoneOffset();
    doc["dayLightSaving"] = persistent.dayLightSaving();
    doc["colorRed"] = persistent.color().r;
    doc["colorGreen"] = persistent.color().g;
    doc["colorBlue"] = persistent.color().b;
    doc["nightOffActive"] = persistent.nightOff().active;
    doc["nightOffOffHour"] = persistent.nightOff().offHour;
    doc["nightOffOffMinute"] = persistent.nightOff().offMinute;
    doc["nightOffOnHour"] = persistent.nightOff().onHour;
    doc["nightOffOnMinute"] = persistent.nightOff().onMinute;
    doc["dimActive"] = persistent.dim().active;
    doc["dimBase"] = persistent.dim().base;
    doc["dimScale"] = persistent.dim().scale;
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
        if (jsonObj.containsKey("colorRed")) {
          c.r = jsonObj["colorRed"];
        }
        if (jsonObj.containsKey("colorGreen")) {
          c.g = jsonObj["colorGreen"];
        }
        if (jsonObj.containsKey("colorBlue")) {
          c.b = jsonObj["colorBlue"];
        }
        persistent.color(c);
        ledCtrl.setColor(c.r, c.g, c.b);

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
        uint8_t r = persistent.color().r;
        uint8_t g = persistent.color().g;
        uint8_t b = persistent.color().b;
        JsonObject jsonObj = json.as<JsonObject>();
        if (jsonObj.containsKey("colorRed")) {
          r = jsonObj["colorRed"];
        }
        if (jsonObj.containsKey("colorGreen")) {
          g = jsonObj["colorGreen"];
        }
        if (jsonObj.containsKey("colorBlue")) {
          b = jsonObj["colorBlue"];
        }
        Serial.print("New Color: ");
        Serial.print(r);
        Serial.print(" ");
        Serial.print(g);
        Serial.print(" ");
        Serial.print(b);
        ledCtrl.setColor(r, g, b);
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

    if (!strcmp(persistent.ssid(), "")) {
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

      if (withinActiveTimeWindow(h, m)) ledCtrl.setClock(h, m);
      else ledCtrl.clear();

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
