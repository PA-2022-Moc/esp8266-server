#include <functional>
#include <PubSubClient.h>
#include "../config/mqtt.config.h"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        const char *username = USERNAME_AUTH_MQTT;
        const char *password = PASSWORD_AUTH_MQTT;

        //  Attempt to connect
        if (client.connect(clientId, username, password))
        {
            Serial.println("connected");
            // ... and resubscribe
            Serial.println(client.subscribe(topic));
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setupMQTT(std::function<void(char *topic, byte *payload, unsigned int length)> callback)
{
    client.setServer(mqtt_server, PORT_MQTT);
    client.setCallback(callback);
}