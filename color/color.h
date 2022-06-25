#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <functional>

#define NUM_LEDS 26    // how many leds to set
#define SATURATION 255 // control the saturation of the leds

#define DATA_PIN 4 // PIN

CRGB leds[NUM_LEDS]; // Define the array of leds

void setupFastled()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
}

void displayColorFrom(uint32_t *value)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = *value;
        FastLED.show();
    }
}
