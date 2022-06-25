#include "color/color.h"
#include "wifi/wifi.h"
#include "clientHttp/clientHttp.h"
#include "json/json.h";
#include "mqtt/mqtt.h"
#include <stdlib.h>

#define COLOR_BUFF_LENGTH 6       // length of color's value from payload
#define POWER_ON_BUFF_LENGTH 1    // length of powerOn's value from payload
#define BRIGHTNESS_BUFF_LENGTH 3  // length of brightness's value from payload
#define PAYLOAD_OFFSET 2          // ignore the key of payload
#define RANDOM_MODE_BUFF_LENGTH 1 // length of randomMode's value from payload

#define SPEED_RAINBOW_EFFECT 100

static char buff[COLOR_BUFF_LENGTH + 1]; // buffer to contain payload's value

Light light;

void setColor(byte *payload)
{
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], COLOR_BUFF_LENGTH);
  buff[COLOR_BUFF_LENGTH] = 0;
  Serial.println(buff);
  light.color = strtol(buff, NULL, 16);
  Serial.println(light.color);
  Serial.println();
}

void setPowerOn(byte *payload)
{
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], POWER_ON_BUFF_LENGTH);
  buff[POWER_ON_BUFF_LENGTH] = 0;
  Serial.println(buff);
  light.powerOn = buff[0] == 't';
}

void setBrightness(byte *payload, unsigned int length)
{
  unsigned int len = min(BRIGHTNESS_BUFF_LENGTH, (int)(length - PAYLOAD_OFFSET));
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], len);
  buff[len] = 0;
  Serial.println(buff);

  light.brightness = (uint8_t)strtol(buff, NULL, 10);
}

void setRandomMode(byte *payload)
{
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], RANDOM_MODE_BUFF_LENGTH);
  buff[RANDOM_MODE_BUFF_LENGTH] = 0;
  Serial.println(buff);
  light.randomMode = buff[0] == 't';
}

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  switch (payload[0])
  {
  case 'c':
    setColor(payload);
    break;
  case 'p':
    setPowerOn(payload);
    break;
  case 'b':
    setBrightness(payload, length);
    break;
  case 'r':
    setRandomMode(payload);
    break;
  default:
    Serial.println("Unknown request payload key");
    break;
  }

  Serial.print("Responding to server...");
  snprintf(msg, MSG_BUFFER_SIZE, "ok");
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(topicState, msg);
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  setupFastled();
  String stringData = getLightData();
  light = deserialize(stringData);
  setupMQTT(callbackMQTT);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  FastLED.setBrightness(light.brightness);

  if (light.powerOn)
  {
    if (light.randomMode)
    {
      uint8_t speed = SPEED_RAINBOW_EFFECT;
      uint8_t deltaHue = 10;
      uint8_t hue = beat8(speed, 255);
      fill_rainbow(leds, NUM_LEDS, hue, deltaHue);
      FastLED.show();
    }
    else
    {
      for (int i = 0; i < NUM_LEDS; ++i)
      {
        leds[i] = light.color;
        FastLED.show();
      }
    }
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; ++i)
    {
      leds[i] = CRGB::Black;
      FastLED.show();
    }
  }
}
