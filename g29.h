/*
 * 
 */

#pragma once

#include "Arduino.h"

typedef struct {

    uint16_t accelerator;
    uint16_t brake;
    uint16_t clutch;

} g29_report_struct;

class G29 {
    private:
        g29_report_struct report;
        boolean changed;

    public: 
        G29(void);

        void setAccelerator(uint16_t v);
        void setBrake(uint16_t v);
        void setClutch(uint16_t v);

        void begin();
        void end();
};
