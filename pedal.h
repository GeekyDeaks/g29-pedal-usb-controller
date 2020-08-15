/*
 * 
 */

#pragma once

#include "Arduino.h"

typedef struct {
  uint16_t vmin;
  uint16_t vmax;

} pedal_calibration_struct;


class Pedal {
    private:
        uint16_t _pin;
        uint16_t _addr;
        pedal_calibration_struct _cal;

    public:
        Pedal(uint16_t pin, uint16_t index);
        void reset(void);
        void calibrate(void);
        uint16_t value(void);
        void save(void);
        uint16_t vmin(void);
        uint16_t vmax(void);

};
