/**
 * 

 */

#include "g29.h"
#include "pedal.h"

// set the pins here
Pedal       brake(A0, 0);
Pedal accelerator(A1, 1);

enum State {
    STATE_REPORT,
    STATE_CALIBRATE_START,
    STATE_CALIBRATE,
    STATE_CALIBRATE_END,
    STATE_MONITOR
};

G29 g29;

int cmd;
enum State state = STATE_REPORT;

void debug() {
    Serial.print("amin=");
    Serial.print(accelerator.vmin(), DEC);
    Serial.print(" amax=");
    Serial.print(accelerator.vmax(), DEC);
    Serial.print(" bmin=");
    Serial.print(brake.vmin(), DEC);
    Serial.print(" bmax=");
    Serial.print(brake.vmax(), DEC);
    Serial.println();
}

void setup() {

    Serial.begin(9600);
    while(!Serial);
}

void loop() {

    cmd = 0;  // reset the command
    if(Serial.available()) {
        cmd = Serial.read();
        Serial.print("Command: 0x");
        Serial.print(cmd, HEX);
        Serial.println();
    }
    
    switch(cmd) {
        case 'd':
        case 'D':
            debug();
            break;
        case 'r':
        case 'R':
            state = STATE_REPORT;
            break;
        case 'm':
        case 'M':
            // monitor
            state = STATE_MONITOR;
            break;
        case 'c':
        case 'C':
            if(state == STATE_CALIBRATE) {
                state = STATE_CALIBRATE_END;
            } else {
                state = STATE_CALIBRATE_START;
            }
            break;
        case 0x0a: 
            // ignore the CR
            break;

    }


    switch(state) {
        case STATE_REPORT:
            g29.begin();
            g29.setAccelerator(accelerator.value());
            g29.setBrake(brake.value());
            g29.end();
            break;
        case STATE_MONITOR: 
            Serial.print("a=");
            Serial.print(accelerator.value());
            Serial.print(" b=");
            Serial.print(brake.value());
            Serial.println();
            delay(500);
            break;
        case STATE_CALIBRATE_START:
            Serial.println("start calibration");
            state = STATE_CALIBRATE;
            accelerator.reset();
            brake.reset();
            break;
        case STATE_CALIBRATE:
            accelerator.calibrate();
            brake.calibrate();
            debug();
            delay(500);
            break;
        case STATE_CALIBRATE_END:
            Serial.println("end calibration");
            accelerator.save();
            brake.save();
            state = STATE_REPORT;
            break;

        default:
            break;
    }


  
}
