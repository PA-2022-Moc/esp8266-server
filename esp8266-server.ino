#include "color/color.h"
#include "wifi/wifi.h"
#include "clientHttp/clientHttp.h"
#include "json/json.h";
#include "mqtt/mqtt.h"
#include <stdlib.h>

#define COLOR_BUFF_LENGTH 6       // length of color's value from payload
#define POWER_ON_BUFF_LENGTH 1    // length of powerOn's value from payload
#define BRIGHTNESS_BUFF_LENGTH 3  // length of brightness's value from payload
#define AUTO_BRIGHTNESS_BUFF_LENGTH 1  // length of auto brightness's value from payload
#define RANDOM_MODE_BUFF_LENGTH 1 // length of randomMode's value from payload
#define PARTY_MODE_BUFF_LENGTH 1 // length of partyMode's value from payload

#define PAYLOAD_OFFSET 2          // ignore the key of payload

#define SPEED_PARTY_EFFECT 100
#define SPEED_RANDOM_EFFECT 10

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

  light.brightness = (uint8_t)strtol(buff, NULL, 10) * 2.5;
  Serial.println(light.brightness);
}

void setAutoBrightness(byte *payload)
{
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], AUTO_BRIGHTNESS_BUFF_LENGTH);
  buff[AUTO_BRIGHTNESS_BUFF_LENGTH] = 0;
  Serial.println(buff);
  light.autoBrightness = buff[0] == 't';
}

void setRandomMode(byte *payload)
{
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], RANDOM_MODE_BUFF_LENGTH);
  buff[RANDOM_MODE_BUFF_LENGTH] = 0;
  Serial.println(buff);
  light.randomMode = buff[0] == 't';
}

void setPartyMode(byte *payload)
{
  strncpy(buff, (char *)&payload[PAYLOAD_OFFSET], PARTY_MODE_BUFF_LENGTH);
  buff[PARTY_MODE_BUFF_LENGTH] = 0;
  Serial.println(buff);
  light.partyMode = buff[0] == 't';
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
  case 'a':
    setAutoBrightness(payload);
    break;
   case 'f':
    setPartyMode(payload);
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
  pinMode(A0, INPUT);
  Serial.begin(115200);
  setup_wifi();
  setupFastled();
  String stringData = getLightData();
  light = deserialize(stringData);
  setupMQTT(callbackMQTT);
}

void loop()
{

  Serial.println(analogRead(A0));// valeur de capteur lumiere

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if(light.autoBrightness){
    Serial.println("ici");

    int value = analogRead(A0) / 4;

    Serial.print("value ");
    Serial.println(value);
    FastLED.setBrightness(255 - value);
  }else{
    FastLED.setBrightness(light.brightness);
  }

  if (light.powerOn){
    if (light.partyMode && !light.randomMode){
      uint8_t speed = SPEED_PARTY_EFFECT;
      uint8_t deltaHue = 10;
      uint8_t hue = beat8(speed, 255);
      fill_rainbow(leds, NUM_LEDS, hue, deltaHue);
      FastLED.show();
    }
    else if (light.randomMode && !light.partyMode){
      uint8_t speed = SPEED_RANDOM_EFFECT;
      uint8_t hue = beat8(speed, 255);
      for (int i = 0; i < NUM_LEDS; ++i){
        leds[i] = CHSV(hue, 255, 255);
        FastLED.show();
      }
    }
    else{
      for (int i = 0; i < NUM_LEDS; ++i){
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
