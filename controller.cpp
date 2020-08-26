/*
 * controller.cpp
 * 
 * Simple HID abstraction to expose 3 pedal axis
 */


#include "controller.h"
#include "HID.h"

#define REPORT_ID 0x04

Controller::Controller(void) {

    static const uint8_t ReportDescriptor[] PROGMEM = {
        0x05, 0x01,           // USAGE_PAGE (Generic Desktop)
        0x09, 0x04,           // USAGE (Joystick)

        0x26, 0xFF, 0x03,     // LOGICAL_MAXIMUM (1023)    
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

    changed = false;
}

void Controller::begin() {
    changed = false;
}

void Controller::end() {
    if(changed) {
        HID().SendReport(REPORT_ID, &report, sizeof(report_struct));
    }
}

void Controller::setAccelerator(uint16_t v) {
    if(report.accelerator != v)  {
        report.accelerator = v;
        changed = true;
    }
}

void Controller::setBrake(uint16_t v) {
    if(report.brake != v) {
        report.brake = v;
        changed = true;
    }
}

void Controller::setClutch(uint16_t v) {
    if(report.clutch != v) {
        report.clutch = v;
        changed = true;
    }

}
