#ifndef CONFIG_h
#define CONFIG_h
#include "LegoLight.h"
#include <ArduinoJson.h>
#include "SPIFFS.h"

#define DEFAULT_MQTT_SERVER "mqtt_server.com"
#define DEFAULT_MQTT_PORT "1883"

struct Config
{
    char mqtt_server[40] = "";
    char mqtt_port[6] = "8080";
    char mqtt_user[20] = "";
    char mqtt_pass[30] = "";
    bool mqtt_enabled = false;
    int idx[8];
    LightMode type[8];
};

void loadConfiguration(const char *filename, Config &config);
void saveConfiguration(const char *filename, const Config &config);
void printConfigurationFile(const char *filename);

#endif