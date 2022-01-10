#include "wm_helper.h"
bool wm_nonblocking = true; // change to true to use non blocking
static const char *const labels_idx[] = {"Channel 1 IDX", "Channel 2 IDX", "Channel 3 IDX", "Channel 4 IDX", "Channel 5 IDX", "Channel 6 IDX", "Channel 7 IDX", "Channel 8 IDX"};
static const char *const names_idx[] = {"idx_1", "idx_2", "idx_3", "idx_4", "idx_5", "idx_6", "idx_7", "idx_8"};
static const char *const labels_type[] = {"Channel 1 Type", "Channel 2 Type", "Channel 3 Type", "Channel 4 Type", "Channel 5 Type", "Channel 6 Type", "Channel 7 Type", "Channel 8 Type"};
static const char *const names_type[] = {"type_1", "type_2", "type_3", "type_4", "type_5", "type_6", "type_7", "type_8"};
WiFiManagerParameter custom_mqtt_server; // global param ( for non blocking w params )
WiFiManagerParameter custom_mqtt_port;
WiFiManagerParameter custom_mqtt_user;
WiFiManagerParameter custom_mqtt_pass;
WiFiManagerParameter custom_mqtt_enabled;
IntParameter custom_idx[8];
WiFiManagerParameter custom_type[8];

String getParam(String name)
{
    //read parameter from server, for customhmtl input
    String value;
    if (wm.server->hasArg(name))
    {
        value = wm.server->arg(name);
    }
    return value;
}

void saveParamCallback()
{

    strcpy(cfg.mqtt_server, getParam("mqtt_server").c_str());
    strcpy(cfg.mqtt_port, getParam("mqtt_port").c_str());
    strcpy(cfg.mqtt_user, getParam("mqtt_user").c_str());
    if (getParam("mqtt_pass") != "")
    {
        strcpy(cfg.mqtt_pass, getParam("mqtt_pass").c_str());
    }
    if (getParam("mqtt_enabled") == "true")
    {
        cfg.mqtt_enabled = true;
    }
    else
    {
        cfg.mqtt_enabled = false;
    }

    for (int i = 0; i < 8; i++)
    {
        cfg.idx[i] = atoi(getParam(names_idx[i]).c_str());
        cfg.type[i] = (LightMode)atoi(getParam(names_type[i]).c_str());
    }

    saveConfiguration(CONFIG_FILE_NAME, cfg);
    // printConfigurationFile(CONFIG_FILE_NAME);
    delay(1000);
    ESP.restart();
}

void wmInit(WiFiManager &wm, Config &config)
{
    if (wm_nonblocking)
        wm.setConfigPortalBlocking(false);

    const char *custom_password_str = "<br/><label for='mqtt_pass'>Mqtt password</label><input type='password' id='mqtt_pass' name='mqtt_pass'>";
    String cstm_mqtt_enabled;

    new (&custom_mqtt_server) WiFiManagerParameter("mqtt_server", "mqtt server", cfg.mqtt_server, 40);
    new (&custom_mqtt_port) WiFiManagerParameter("mqtt_port", "mqtt port", cfg.mqtt_port, 6);
    new (&custom_mqtt_user) WiFiManagerParameter("mqtt_user", "mqtt user", cfg.mqtt_user, 20);
    new (&custom_mqtt_pass) WiFiManagerParameter(custom_password_str);

    if (cfg.mqtt_enabled)
    {
        new (&custom_mqtt_enabled) WiFiManagerParameter("mqtt_enabled", "Mqtt enabled", "true", 40, "placeholder=\"Custom Field Placeholder\" type=\"checkbox\" checked");
    }
    else
    {
        new (&custom_mqtt_enabled) WiFiManagerParameter("mqtt_enabled", "Mqtt enabled", "true", 40, "placeholder=\"Custom Field Placeholder\" type=\"checkbox\"");
    }

    wm.addParameter(&custom_mqtt_server);
    wm.addParameter(&custom_mqtt_port);
    wm.addParameter(&custom_mqtt_user);
    wm.addParameter(&custom_mqtt_pass);
    wm.addParameter(&custom_mqtt_enabled);

    for (int i = 0; i < 8; i++)
    {
        new (&custom_idx[i]) IntParameter(names_idx[i], labels_idx[i], cfg.idx[i]);
        new (&custom_type[i]) IntParameter(names_type[i], labels_type[i], cfg.type[i]);

        wm.addParameter(&custom_idx[i]);
        wm.addParameter(&custom_type[i]);
    }

    wm.setSaveParamsCallback(saveParamCallback);

    std::vector<const char *> menu = {"wifi", "info", "param", "sep", "erase", "update", "restart", "exit"};
    wm.setMenu(menu);

    // set dark theme
    // wm.setClass("invert");

    wm.setConfigPortalTimeout(30);   // auto close configportal after n seconds
    wm.setCaptivePortalEnable(true); // disable captive portal redirection
    wm.setAPClientCheck(true);       // avoid timeout if client connected to softap

    // wifi scan settings
    // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
    // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
    // wm.setShowInfoErase(false);      // do not show erase button on info page
    // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons

    // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails

    bool connect_result;
    connect_result = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

    if (!connect_result)
    {
        printf("Failed to connect or hit timeout\n");
        // ESP.restart();
    }
    else
    {
        //if you get here you have connected to the WiFi
        printf("connected...yeey :)\n");
        wm.startWebPortal();
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        printf("Connecting to WiFi..\n");
    }
    printf("Connected to the WiFi network\n");
}