#include "config.h"

void loadConfiguration(const char *filename, Config &config)
{
    // Open file for reading
    printf("\nLoading config...\n");
    File file = SPIFFS.open(filename);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
        printf("Failed to read file, using default configuration\n");

    // Copy values from the JsonDocument to the Config
    // config.mqtt_port = doc["mqtt_port"];

    strlcpy(config.mqtt_port,                     // <- destination
            doc["mqtt_port"] | DEFAULT_MQTT_PORT, // <- source
            sizeof(config.mqtt_port));            // <- destination's capacity

    strlcpy(config.mqtt_server,                       // <- destination
            doc["mqtt_server"] | DEFAULT_MQTT_SERVER, // <- source
            sizeof(config.mqtt_server));              // <- destination's capacity
    strlcpy(config.mqtt_user,                         // <- destination
            doc["mqtt_user"] | "",                    // <- source
            sizeof(config.mqtt_user));                // <- destination's capacity
    strlcpy(config.mqtt_pass,                         // <- destination
            doc["mqtt_pass"] | "",                    // <- source
            sizeof(config.mqtt_pass));                // <- destination's capacity
    config.mqtt_enabled = doc["mqtt_enabled"];

    for (int i = 0; i < 8; i++)
    {
        config.idx[i] = doc["idx"][i];
        config.type[i] = doc["type"][i];
    }
    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, const Config &config)
{
    // Delete existing file, otherwise the configuration is appended to the file
    SPIFFS.remove(filename);

    // Open file for writing
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file)
    {
        printf("Failed to create file\n");
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Set the values in the document
    doc["mqtt_server"] = config.mqtt_server;
    doc["mqtt_port"] = config.mqtt_port;
    doc["mqtt_user"] = config.mqtt_user;
    doc["mqtt_pass"] = config.mqtt_pass;
    doc["mqtt_enabled"] = config.mqtt_enabled;
    JsonArray idx = doc.createNestedArray("idx");
    JsonArray type = doc.createNestedArray("type");
    for (int i = 0; i < 8; i++)
    {
        idx.add(config.idx[i]);
        type.add(config.type[i]);
    }

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {
        printf("Failed to write to file\n");
    }

    // Close the file
    file.close();
}

// Prints the content of a file to the Serial
void printConfigurationFile(const char *filename)
{
    // Open file for reading
    File file = SPIFFS.open(filename);
    if (!file)
    {
        printf("Failed to read file\n");
        return;
    }

    // Extract each characters by one by one
    while (file.available())
    {
        Serial.print((char)file.read());
    }
    Serial.println();

    // Close the file
    file.close();
}