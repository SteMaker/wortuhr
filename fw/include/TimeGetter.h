#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp);

class TimeGetter {
    int lastUpdateHour = 25;
  public:
    void setup() {
      timeClient.begin();
      timeClient.setUpdateInterval(3600 * 1000); // once per hour
    }

    void getTime(int &hour, int &minute) {
      timeClient.update();
      hour = timeClient.getHours();
      minute = timeClient.getMinutes();
    }
};
