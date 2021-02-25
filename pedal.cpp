/**
 * 
 * 
 */

#include "Arduino.h"
#include "EEPROM.h"
#include "pedal.h"

#define MAX_VAL 1023

Pedal::Pedal (uint16_t pin, uint16_t index) {

    _pin = pin;
    _addr = index * sizeof(pedal_calibration_struct);
    load();

}

uint16_t Pedal::value(void) {
    uint16_t val = analogRead(_pin);

    val = constrain(val, _cal.vmin, _cal.vmax);
    val -= _cal.vmin;

    // manipulate the value based on min/max
    uint16_t range = _cal.vmax - _cal.vmin;
    // sanity check
    if(range == 0) range = 1;

    uint16_t mul = (MAX_VAL << 6) / range;
    return (val * mul) >> 6;
}

uint16_t Pedal::raw(void) {
    uint16_t val = analogRead(_pin);
    return val;
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

void Pedal::load(void) {
    // load up the current min/max
    EEPROM.get(_addr, _cal);
    // do some sanity checking
    if(_cal.vmax > MAX_VAL || _cal.vmin > _cal.vmax) {
        reset();
    }
}

void Pedal::save(void) {

    pedal_calibration_struct t;
    EEPROM.get(_addr, t);
    // check if we really need to update
    if(t.vmax != _cal.vmax || t.vmin != _cal.vmin) {
        EEPROM.put(_addr, _cal);
    }
    
}

uint16_t Pedal::vmin(void) {
    return _cal.vmin;
}

uint16_t Pedal::vmax(void) {
    return _cal.vmax;
}
