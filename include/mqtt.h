#ifndef MQTT_h
#define MQTT_h
#include "arduino.h"
#include "setup.h"
#include "config.h"
#include "PubSubClient.h"

extern PubSubClient client;
extern DomoticzLegoLight lights[];

void mqttReconnect(Config &config);
void mqttInit(Config &config);
void mqttCallback(char *topic, byte *payload, unsigned int length);

#endif