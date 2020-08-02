#include "Wire.h"
#include <BH1750FVI.h>
#include <TSL2561.h>

// use _LOW, _HIGH or _FLOAT address setting
#define TSL2561_ADDR    TSL2561_ADDR_FLOAT

/* calculates an 8-bit luma value based on current light intensity
   and configuration:
   luma = base + scale*intensity

   base is 8-bit
   scale is 8-bit
   intensity is 16-bit

   when scale is at max we want to reach max luma at 1/64 of max intensity
   therefore we need another scaling by 1/1024. Max intensity is 65k LUX!
*/
class Brightness {
    public:
    Brightness() {}
    ~Brightness() {}

    void setup(uint8_t _base, uint8_t _scale) {
        setScaleParams(_base, _scale);

        Wire.begin();

        uint8_t addrToCheck[] = {
            BH1750FVI::k_DevAddress_L,
            TSL2561_ADDR,
        };

        uint8_t devFound = 0;
        for (uint8_t i=0; i < (sizeof(addrToCheck)/sizeof(uint8_t)); i++) {
            Wire.beginTransmission(addrToCheck[i]);
            if (Wire.endTransmission() == 0) {
                devFound = addrToCheck[i];
                Serial.print("I2C device found at address 0x");
                Serial.println(devFound, HEX);
            }
        }

        switch (devFound) {
            case BH1750FVI::k_DevAddress_L:
                lightSensorType = sensorTypeBh1750;
                lightSensorBh1750.begin();
                break;
            case TSL2561_ADDR:
                lightSensorType = sensorTypeTsl2561;
                lightSensorTsl2651.begin();
                lightSensorTsl2651.setGain(TSL2561_GAIN_16X);
                lightSensorTsl2651.setTiming(TSL2561_INTEGRATIONTIME_13MS);
                break;
            default:
                Serial.println("no I2C brightness sensor found!");
                break;
        }
    }

    void setScaleParams(uint8_t _base, uint8_t _scale) {
        base = _base;
        scale = _scale;
    }

    uint8_t getLumaScale() {
        uint32_t intensity;
        uint32_t lum;
        switch (lightSensorType) {
            case sensorTypeBh1750:
                intensity = lightSensorBh1750.GetLightIntensity();
                //Serial.print("BH1750 Light Intensity: ");
                //Serial.println(intensity);
                break;
            case sensorTypeTsl2561:
                lum = lightSensorTsl2651.getFullLuminosity();
                // top 16 bits are IR, lower 16 bits are full spectrum
                intensity = lightSensorTsl2651.calculateLux(lum & 0xFFFF, lum >> 16);
                //Serial.print("TSL2651 Light Intensity: ");
                //Serial.println(intensity);
                break;
            default:
                intensity = 0xFFFF;
        }
        uint32_t luma32 = intensity * uint32_t(scale) / 32;
        luma32 = (luma32 < base) ? base : luma32;
        //Serial.print("luma: ");
        //Serial.println(luma32);
        return (luma32 > 255) ? 255 : luma32;
    }

    private:

    typedef enum eLightSensorType{
        sensorTypeNone,
        sensorTypeBh1750,
        sensorTypeTsl2561
    } eLightSensorType_t;

    eLightSensorType_t lightSensorType = sensorTypeNone;

    BH1750FVI lightSensorBh1750 = BH1750FVI(BH1750FVI::k_DevModeContHighRes);
    TSL2561   lightSensorTsl2651 = TSL2561(TSL2561_ADDR);

    uint8_t base;
    uint8_t scale;
};
