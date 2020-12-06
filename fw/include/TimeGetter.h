#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp);

class TimeGetter {
    int lastUpdateHour = 25;
    bool updateOngoing = false;
  public:
    void setup() {
      timeClient.begin();
      timeClient.setUpdateInterval(3600 * 1000); // once per hour
    }

    void getTime(int &hour, int &minute) {
      static int h = 0, m = 0;
      noInterrupts();
      if(!updateOngoing) {
        h = hour = timeClient.getHours();
        m = minute = timeClient.getMinutes();
      } else {
        // This will only happen rarely on the web interface causing the time to be updated
        // few hundred milli seconds later
        hour = h;
        minute = m;
      }
      interrupts();
    }

    void loop(void) {
      updateOngoing = true;
      timeClient.update();
      updateOngoing = false;
    }
};
