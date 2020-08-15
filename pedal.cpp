/**
 * 
 * 
 */

#include "Arduino.h"
#include "EEPROM.h"
#include "pedal.h"

#define MAX_VAL 1024

Pedal::Pedal (uint16_t pin, uint16_t index) {

    _pin = pin;
    _addr = index * sizeof(pedal_calibration_struct);

    // load up the current min/max
    EEPROM.get(_addr, _cal);
    // do some sanity checking
    if(_cal.vmax > MAX_VAL) _cal.vmax = MAX_VAL;
    if(_cal.vmin > _cal.vmax) _cal.vmin = _cal.vmax;

}

uint16_t Pedal::value(void) {
    uint16_t val = analogRead(_pin);

    if(val < _cal.vmin) val = _cal.vmin;
    if(val > _cal.vmax) val = _cal.vmax;

    val -= _cal.vmin;
    // manipulate the value based on min/max
    uint16_t range = _cal.vmax - _cal.vmin;
    if(range == 0) range = 1;
    uint16_t mul = (MAX_VAL << 4) / range;
    return (val * mul) >> 4;
}

void Pedal::reset(void) {
    _cal.vmin = MAX_VAL;
    _cal.vmax = 0;
    calibrate();
}

void Pedal::calibrate(void) {
    // update the min/max
    uint16_t val = analogRead(_pin);
    if(val < _cal.vmin) _cal.vmin = val;
    if(val > _cal.vmax) _cal.vmax = val;

}

void Pedal::save(void) {
    EEPROM.put(_addr, _cal);
}

uint16_t Pedal::vmin(void) {
    return _cal.vmin;
}

uint16_t Pedal::vmax(void) {
    return _cal.vmax;
}
