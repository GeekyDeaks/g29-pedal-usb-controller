/*
 * g29.cpp
 * 
 * Simple HID abstraction to expose 3 generic axis
 */


#include "g29.h"
#include "HID.h"

#define REPORT_ID 0x04

G29::G29(void) {

    static const uint8_t ReportDescriptor[] PROGMEM = {
        0x05, 0x01,           // USAGE_PAGE (Generic Desktop)
        0x09, 0x04,           // USAGE (Joystick)

        0x26, 0x00, 0x04,     // LOGICAL_MAXIMUM (1024)    
        0x75, 0x10,           // REPORT_SIZE (16)

        0xA1, 0x01,           // COLLECTION (Application)

        0x85, REPORT_ID,      //   REPORT_ID

        0x05, 0x02,           //   USAGE_PAGE (Simulation Controls)
        0x95, 0x02,           //   REPORT_COUNT (2)

        0xA1, 0x00,           //   COLLECTION (Physical
        0x09, 0xC4,           //     USAGE (Accelerator)
        0x09, 0xC5,           //     USAGE (Brake)
        //0x09, 0xC6,         //     USAGE (Clutch)  
        0x81, 0x02,           //     INPUT (Data,Var,Abs)
        0xC0,                 //   END_COLLECTION

        // for some reason, the HID clutch is not picked up
        // so we have to resort to using the X axis
        
        0x05, 0x01,           //   USAGE_PAGE (Generic Desktop)
        0x95, 0x01,           //   REPORT_COUNT (1)

        0xA1, 0x00,           //   COLLECTION (Physical
        0x09, 0x30,           //     USAGE (X) - clutch
        0x81, 0x02,           //     INPUT (Data,Var,Abs)
        0xC0,                 //   END_COLLECTION
        
        0xc0                  // END_COLLECTION
    };
    static HIDSubDescriptor descriptor(ReportDescriptor, sizeof(ReportDescriptor));
    HID().AppendDescriptor(&descriptor);

    report.accelerator = 10;
    report.brake = 20;
    report.clutch = 30;

    changed = true;
}

void G29::begin() {
    changed = false;
}

void G29::end() {
    HID().SendReport(REPORT_ID, &report, sizeof(g29_report_struct));
}

void G29::setAccelerator(uint16_t v) {
    report.accelerator = v;
    changed = true;
}

void G29::setBrake(uint16_t v) {
    report.brake = v;
    changed = true;
}

void G29::setClutch(uint16_t v) {
    report.clutch = v;
    changed = true;
}