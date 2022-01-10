#include <Arduino.h>
#include "setup.h"
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
// #include "SPIFFS.h"
#include "spi_helper.h"
#include <WiFi.h>
#include "PubSubClient.h"
#include "mqtt.h"
#include "wm_helper.h"

#include "LegoLight.h"

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "config.h"
#define CONFIG_FILE_NAME "/config.txt"

int pin[] = {PIN_CHANNEL_0, PIN_CHANNEL_1, PIN_CHANNEL_2, PIN_CHANNEL_3, PIN_CHANNEL_4, PIN_CHANNEL_5, PIN_CHANNEL_6, PIN_CHANNEL_7};
static const char *const task_names[] = {"Lamp 0", "Lamp 1", "Lamp 2", "Lamp 3", "Lamp 4", "Lamp 5", "Lamp 6", "Lamp 7"};

Config cfg;

WiFiManager wm; // global wm instance

// #define LED_OFF_VALUE 1024

DomoticzLegoLight lights[CHANNELS];

TaskHandle_t Lamp_Tasks[CHANNELS];

WiFiClient espClient;
PubSubClient client(espClient);

void lampTask(void *pvParameters)
{
  int id = (uint32_t)pvParameters;

  for (;;)
  {
    delay(lights[id].updateOut());
  }
}

void setup()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  printf("\nBooting...\n");

  if (!spiffsInit())
  {
    wm.resetSettings(); // wipe settings
  }

  loadConfiguration(CONFIG_FILE_NAME, cfg);

  wmInit(wm, cfg);

  for (int i = 0; i < CHANNELS; i++)
  {
    lights[i] = DomoticzLegoLight(i, pin[i], cfg.type[i], 10, true, cfg.idx[i]);
  }

  mqttInit(cfg);

  for (int i = 0; i < CHANNELS; i++)
  {
    if (lights[i].getMode() != OFF)
    {
      xTaskCreatePinnedToCore( // Use xTaskCreate() in vanilla FreeRTOS
          lampTask,            // Function to be called
          task_names[i],       // Name of task
          2048,                // Stack size (bytes in ESP32, words in FreeRTOS)
          (void *)i,           // Parameter to pass to function
          0,                   // Task priority (0 to configMAX_PRIORITIES - 1)
          &Lamp_Tasks[i],      // Task handle
          0);
      if (cfg.mqtt_enabled)
      {
        String msg;
        StaticJsonDocument<256> doc;
        doc["command"] = "switchlight";
        doc["idx"] = lights[i].getIdx();
        doc["switchcmd"] = "Off";
        serializeJson(doc, msg);

        client.publish("domoticz/in", msg.c_str());
      }
    }
  }
}

void loop()
{

  mqttReconnect(cfg);

  client.loop();

  wm.process();

  delay(1);
}
