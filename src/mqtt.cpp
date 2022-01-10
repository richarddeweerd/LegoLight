#include "mqtt.h"

void mqttInit(Config &config)
{
    if (config.mqtt_enabled)
    {
        client.setServer(config.mqtt_server, atoi(config.mqtt_port));
        client.setCallback(mqttCallback);
    }
    mqttReconnect(config);
}

void mqttReconnect(Config &config)
{
    if (config.mqtt_enabled && !client.connected())
    {
        // Loop until we're reconnected
        while (!client.connected())
        {
            printf("Attempting MQTT connection...");
            // Attempt to connect
            if (client.connect("ESP32Client", config.mqtt_user, config.mqtt_pass))
            {
                printf("connected\n");
                // Subscribe
                for (int i = 0; i < CHANNELS; i++)
                {
                    if (lights[i].getIdx() != 0 && lights[i].getMode() != OFF)
                    {
                        String topic = "domoticz/out/" + (String)lights[i].getIdx();
                        client.subscribe(topic.c_str());
                    }
                }
            }
            else
            {
                printf("failed, rc=%i try again in 5 seconds\n", client.state());
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload, length);

    // Test if parsing succeeds.
    if (error)
    {
        printf("deserializeJson() failed: %s\n", error.f_str());
        return;
    }
    int idx = doc["idx"];
    int level = doc["Level"];
    int status = doc["nvalue"];
    for (int i = 0; i < CHANNELS; i++)
    {
        if (lights[i].getIdx() == idx)
        {
            lights[i].setStatus(status, level);
            break;
        }
    }
}