#include <ArduinoJson.h>
#include "../model/light.h"

Light deserialize(String stringJson)
{
    StaticJsonDocument<200> doc;
    const char *json = stringJson.c_str();

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);

    Light defaultLight;

    // Test if parsing succeeds.
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return defaultLight;
    }

    uint32_t color = defaultLight.color;
    bool powerOn = defaultLight.powerOn;
    uint8_t brightness = defaultLight.brightness;
    bool randomMode = defaultLight.randomMode;

    if (doc["color"])
    {
        const char *col = doc["color"];
        color = strtol(col, NULL, 16);
    }

    if (doc["powerOn"])
    {
        powerOn = doc["powerOn"];
    }

    if (doc["brightness"])
    {
        brightness = doc["brightness"];
    }

    if (doc["randomMode"])
    {
        randomMode = doc["randomMode"];
    }

    return Light(color, powerOn, brightness, randomMode);
}