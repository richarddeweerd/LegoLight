
#ifndef LegoLight_h
#define LegoLight_h

#define MAX_LEVEL 1024
#define PWM_FREQ 5000

enum LightMode
{
    OFF = 0,
    NORMAL = 1,
    FIRE = 2,
    BLINKING = 3
};

class LegoLight
{
public:
    LegoLight();
    LegoLight(int id, int pin, LightMode mode, int bits, bool inverted);
    int getLevel();
    int updateOut();
    bool getStatus();
    int getPin();
    LightMode getMode();
    void setStatus(bool stat);
    void setStatus(bool stat, int val);
    void setOutput(int val);
    void setLevel(int val);
    void setLevelPercent(int val);

private:
    int _id = 0;
    int _pin = 0;
    int _bits = 0;
    bool _inverted;
    LightMode _mode = LightMode::NORMAL;

    bool _blinkstatus;
    int _level = 0;
    bool _status = false;
    int _max_level;
};

class DomoticzLegoLight : public LegoLight
{
public:
    DomoticzLegoLight();
    DomoticzLegoLight(int id, int pin, LightMode mode, int bits, bool inverted, int idx);
    void setIdx(int idx);
    int getIdx();

private:
    int _idx = 0;
};

#endif