#include "spi_helper.h"

bool spiffsInit()
{
    if (SPIFFS.begin())
    {
        printf("SPIFFS Initialize...ok\n");
    }
    else
    {
        printf("SPIFFS Initialization...failed\n");
        //Format File System
        if (SPIFFS.format())
        {
            printf("File System Formated\n");
        }
        else
        {
            printf("File System Formatting Error\n");
        }
        return false;
    }
    return true;
}