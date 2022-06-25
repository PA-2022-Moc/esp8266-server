
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "../config/clientHttp.config.h";

String getLightData()
{
    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, uriApiServerGet))
    { // HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = http.getString();
                Serial.println(payload);
                return payload;
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    else
    {
        Serial.printf("[HTTP} Unable to connect\n");
    }
    return "";
}