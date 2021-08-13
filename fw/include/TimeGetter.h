#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp);

class TimeGetter {
    int lastUpdateHour = 25;
    bool updateOngoing = false;
    bool dst = false;

  private:
    void calcDst(void) {
      tm t;
      time_t epoch = timeClient.getEpochTime();
      t = *localtime(&epoch);
      dst = false;

      // previous Sunday incl. today
      int prevSunday = t.tm_mday - t.tm_wday;
      
      // tm_mon: 0 = January
      if ((t.tm_mon+1 > 3) && (t.tm_mon+1 < 10)) {  // April to Sept
        dst = true;
      }
      if (t.tm_mon+1 == 3) {              // in March
        if (t.tm_wday == 0) {             // today is sunday
          if (   (prevSunday >= 25)       // on or after 25th
              && (t.tm_hour >= 1)) {      // after 2:00 am (CET!)
              dst = true;
          }
        }
        else if (prevSunday >= 25) {      // not Sunday but the last Sunday in March has passed
          dst = true;
        }
      }
      if (t.tm_mon+1 == 10) {             // in October
        if (t.tm_wday == 0) {             // today is sunday
          if (   (prevSunday >= 25)       // on or after 25th
              && (t.tm_hour < 1)) {       // before 2:00 am (CET!)
              dst = true;
          }
          else if (prevSunday < 25) {     // Sunday but not not the last Sunday yet
            dst = true;
          }
        }
        else if (prevSunday < 25) {       // not Sunday and before the last Sunday
          dst = true;
        }
      }

      Serial.printf("DST=%d   %d.%d.%d %d:%d\n", dst, t.tm_mday, t.tm_mon+1, t.tm_year+1900, t.tm_hour, t.tm_min);
    }


  public:
    void setup() {
      timeClient.begin();
      timeClient.setUpdateInterval(3600 * 1000); // once per hour
    }

    void getTime(int &hour, int &minute) {
      static int h = 0, m = 0;
      bool hourFlipped = false;
      noInterrupts();
      if(!updateOngoing) {
        hour = timeClient.getHours();
        minute = timeClient.getMinutes();
        if (h != hour) {
          hourFlipped = true;
        }
        h = hour;
        m = minute;
      } else {
        // This will only happen rarely on the web interface causing the time to be updated
        // few hundred milli seconds later
        hour = h;
        minute = m;
      }
      interrupts();
      if (hourFlipped) {
        calcDst();
      }
    }

    int getDst(void) {
      return dst;
    }

    void loop(void) {
      updateOngoing = true;
      timeClient.update();
      updateOngoing = false;
    }
};
