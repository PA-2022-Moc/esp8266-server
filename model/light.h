#include <cstdint>

class Light
{
public:
    uint32_t color;      // color set to leds
    bool powerOn;        // power's state of leds
    uint8_t brightness;  // brightness set to leds
    bool autoBrightness; // auto brightness effect set to leds
    bool randomMode;     // random effect set to leds
    bool partyMode;      // rainbow effect set to leds
    Light(uint32_t color, bool powerOn, uint8_t brightness, bool autoBrightness, bool randomMode, bool partyMode)
    {
        this->color = color;
        this->powerOn = powerOn;
        this->brightness = brightness;
        this->autoBrightness = autoBrightness;
        this->randomMode = randomMode;
        this->partyMode = partyMode;
    }
    Light()
    {
        this->color = 0xffffff;
        this->powerOn = true;
        this->brightness = 50;
        this->autoBrightness = false;
        this->randomMode = false;
        this->partyMode = false;
    }
};
