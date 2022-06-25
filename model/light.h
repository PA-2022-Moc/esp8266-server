#include <cstdint>

class Light
{
public:
    uint32_t color;     // color set to leds
    bool powerOn;       // power's state of leds
    uint8_t brightness; // brightness set to leds
    bool randomMode;    // rainbow effect set to leds
    Light(uint32_t color, bool powerOn, uint8_t brightness, bool randomMode)
    {
        this->color = color;
        this->powerOn = powerOn;
        this->brightness = brightness;
        this->randomMode = randomMode;
    }
    Light()
    {
        this->color = 0xffffff;
        this->powerOn = true;
        this->brightness = 50;
        this->randomMode = false;
    }
};
