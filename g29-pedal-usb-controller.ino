/**
 * 

 */

#include "controller.h"
#include "pedal.h"

// set the pins here
Pedal       brake(A0, 0);
Pedal accelerator(A1, 1);
Pedal      clutch(A2, 2);

enum State {
    STATE_REPORT,
    STATE_CALIBRATE,
    STATE_MONITOR
};

Controller controller;

int cmd;
enum State state = STATE_REPORT;

void dump_cal() {
    Serial.print("a=");
    Serial.print(accelerator.value(), DEC);
    Serial.print(" amin=");
    Serial.print(accelerator.vmin(), DEC);
    Serial.print(" amax=");
    Serial.print(accelerator.vmax(), DEC);
    Serial.print(" b=");
    Serial.print(brake.value(), DEC);
    Serial.print(" bmin=");
    Serial.print(brake.vmin(), DEC);
    Serial.print(" bmax=");
    Serial.print(brake.vmax(), DEC);
    Serial.print(" c=");
    Serial.print(clutch.value(), DEC);
    Serial.print(" cmin=");
    Serial.print(clutch.vmin(), DEC);
    Serial.print(" cmax=");
    Serial.print(clutch.vmax(), DEC);
    Serial.println();
}

void setup() {

    Serial.begin(9600);
    //while(!Serial);
}

void loop() {

    cmd = 0;  // reset the command
    if(Serial.available()) {
        cmd = Serial.read();
    }
    
    switch(cmd) {
        case 'd':
        case 'D':
            dump_cal();
            break;
        case 'r':
        case 'R':
            state = STATE_REPORT;
            // reload the calibration in case we didn't finish it
            accelerator.load();
            brake.load();
            clutch.load();
            Serial.println("USB HID Reporting");
            break;
        case 'm':
        case 'M':
            // monitor
            state = STATE_MONITOR;
            // reload the calibration in case we didn't finish it
            accelerator.load();
            brake.load();
            clutch.load();
            Serial.println("Monitoring");
            break;
        case 'c':
        case 'C':
            state = STATE_CALIBRATE;
            Serial.println("Starting Calibration");
            accelerator.reset();
            brake.reset();
            clutch.reset();
            break;
        case 's':
        case 'S':
            if(state == STATE_CALIBRATE) {
                Serial.println("Saving Calibration");
                accelerator.save();
                brake.save();
                clutch.save();
                state = STATE_REPORT;
            }
            break;
        case 'h':
        case 'H':
        case '?':
            Serial.println("(M)onitor");
            Serial.println("(D)ump calibration");
            Serial.println("(C)alibrate");
            Serial.println("(S)ave calibration");
            Serial.println("(R)eport HID");
        case 0x0a: 
            // ignore the CR
            break;

    }

    switch(state) {
        case STATE_REPORT:
            controller.begin();
            controller.setAccelerator(accelerator.value());
            controller.setBrake(brake.value());
            //controller.setClutch(clutch.value());
            controller.setClutch( (1024 - accelerator.value() + clutch.value()) /2 );
            controller.setRudder( (1024 - accelerator.value() + clutch.value()) /2 );
            controller.end();
            break;
        case STATE_MONITOR: 
            Serial.print("a=");
            Serial.print(accelerator.value());
            Serial.print(" b=");
            Serial.print(brake.value());
            Serial.print(" c=");
            Serial.print(clutch.value());
            Serial.println();
            delay(500);
            break;
        case STATE_CALIBRATE:
            accelerator.calibrate();
            brake.calibrate();
            clutch.calibrate();
            dump_cal();
            delay(500);
            break;
        default:
            break;
    }


  
}
