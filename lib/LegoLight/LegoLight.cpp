#include "LegoLight.h"
#include "arduino.h"

LegoLight::LegoLight()
{
    _mode = NORMAL;
}

LegoLight::LegoLight(int id, int pin, LightMode mode, int bits, bool inverted)
{
    _id = id;
    _pin = pin;
    _mode = mode;
    _bits = bits;
    _inverted = inverted;
    _max_level = pow(2, _bits);
    ledcSetup(_id, PWM_FREQ, bits);
    ledcAttachPin(_pin, _id);
    setOutput(0);
}

int LegoLight::getLevel()
{
    return _level;
}

bool LegoLight::getStatus()
{
    return _status;
}

int LegoLight::getPin()
{
    return _pin;
}

LightMode LegoLight::getMode()
{
    return _mode;
}

void LegoLight::setStatus(bool stat, int val)
{

    setLevelPercent(val);
    setStatus(stat);
}
void LegoLight::setStatus(bool stat)
{
    _status = stat;
    if (!_status)
    {
        setOutput(0);
    }
}

void LegoLight::setOutput(int val)
{
    float lev = _level;
    lev = lev / _max_level;
    lev = val * lev;
    if (_inverted)
    {
        ledcWrite(_id, _max_level - (int)lev);
    }
    else
    {
        ledcWrite(_id, (int)lev);
    }
}

void LegoLight::setLevel(int val)
{
    _level = val;
}

void LegoLight::setLevelPercent(int val)
{
    _level = map(val, 0, 100, 0, MAX_LEVEL);
}

int LegoLight::updateOut()
{
    if (_status)
    {
        switch (_mode)
        {
        case LightMode::OFF:
        {
            setOutput(0);
            break;
        }
        case LightMode::NORMAL:
            setOutput(_max_level);
            break;
        case LightMode::FIRE:
        {
            setOutput((_max_level / 2) + random(_max_level / 2));
            break;
        }
        case LightMode::BLINKING:
        {
            if (_blinkstatus)
            {
                setOutput(0);
                _blinkstatus = false;
            }
            else
            {
                setOutput(_max_level);
                _blinkstatus = true;
            }
            break;
        }
        }
    }
    if (_mode == LightMode::FIRE)
    {
        return random(10, 100);
    }
    return 500;
}

DomoticzLegoLight::DomoticzLegoLight() : LegoLight()
{
}

DomoticzLegoLight::DomoticzLegoLight(int id, int pin, LightMode mode, int bits, bool inverted, int idx) : LegoLight(id, pin, mode, bits, inverted)
{
    _idx = idx;
}

void DomoticzLegoLight::setIdx(int idx)
{
    _idx = idx;
}

int DomoticzLegoLight::getIdx()
{
    return _idx;
}