/*
 * 
 */

#pragma once

#include "Arduino.h"

typedef struct {

    uint16_t accelerator;
    uint16_t brake;
    uint16_t clutch;

} report_struct;

class Controller {
    private:
        report_struct report;
        boolean changed;

    public: 
        Controller(void);

        void setAccelerator(uint16_t v);
        void setBrake(uint16_t v);
        void setClutch(uint16_t v);

        void begin();
        void end();
};
