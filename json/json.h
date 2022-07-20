#include <ArduinoJson.h>
#include "../model/light.h"

Light deserialize(String stringJson)
{
    StaticJsonDocument<220> doc;
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
    bool autoBrightness = defaultLight.autoBrightness;
    bool randomMode = defaultLight.randomMode;
    bool partyMode = defaultLight.partyMode;

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

    if (doc["autoBrightness"])
    {
        autoBrightness = doc["autoBrightness"];
    }

    if (doc["randomMode"])
    {
        randomMode = doc["randomMode"];
    }
    if (doc["partyMode"])
    {
        partyMode = doc["partyMode"];
    }

    return Light(color, powerOn, brightness, autoBrightness, randomMode, partyMode);
}