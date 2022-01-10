#ifndef WM_HELPER_h
#define WM_HELPER_h
#include "arduino.h"
#include "config.h"
#include <WiFiManager.h>

#define CONFIG_FILE_NAME "/config.txt"

extern Config cfg;

extern WiFiManager wm;

class IntParameter : public WiFiManagerParameter
{
public:
    IntParameter() : WiFiManagerParameter("")
    {
    }
    IntParameter(const char *id, const char *placeholder, long value, const uint8_t length = 10)
        : WiFiManagerParameter("")
    {
        init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
    }

    long getValue()
    {
        return String(WiFiManagerParameter::getValue()).toInt();
    }
};

void wmInit(WiFiManager &wm, Config &config);

#endif