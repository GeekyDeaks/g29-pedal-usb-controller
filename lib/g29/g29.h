/*
 * 
 */

#pragma once

#include "HID.h"

typedef struct {

  uint16_t accelerator;
  uint16_t brake;
  uint16_t clutch;

} g29pedals_report_struct;

class G29Pedals {
  private:
    g29pedals_report_struct report;
    boolean changed;

  public: 
    G29Pedals(void);

    void setAccelerator(uint16_t v);
    void setBrake(uint16_t v);
    void setClutch(uint16_t v);

    void begin();
    void end();
};
