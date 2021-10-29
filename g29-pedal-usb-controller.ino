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
    STATE_MONITOR,
    STATE_PLOT
};

Controller controller;

int cmd;
enum State state = STATE_REPORT;
int yawMode = 0; // all inputs reported

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
    accelerator.load();
    brake.load();
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
        case 'p':
        case 'P':
            // plot
            state = STATE_PLOT;
            // reload the calibration in case we didn't finish it
            accelerator.load();
            brake.load();
            Serial.println("Plotting");
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
        case 'y':
        case 'Y':
          yawMode = (yawMode + 1) % 3;
          Serial.print("Yaw mode: ");
          switch(yawMode){
            case 0:
              Serial.println("Rudder and Pedals");
              break;
            case 1:
              Serial.println("Rudder only");
              break;
            case 2:
              Serial.println("Pedals only");
              break;
            default:
              Serial.println(yawMode);
              break;
          }
          break;
        case 'h':
        case 'H':
        case '?':
            Serial.println("(C)alibrate");
            Serial.println("(D)ump calibration");
            Serial.println("(M)onitor");
            Serial.println("(P)lot accelerator/brake/clutch");
            Serial.println("(R)eport HID");
            Serial.println("(S)ave calibration");
            Serial.println("(Y)aw mode toggle");

        case 0x0a: 
        case 0x00:
            // ignore the CR and 0
            break;
            
        default:
            Serial.print("Unknown cmd: ");
            Serial.println(cmd);

    }

    switch(state) {
        case STATE_REPORT:
            controller.begin();
            if(yawMode == 0 || yawMode == 1) {
              controller.setRudder( 512 + ( clutch.value() / 2) - (accelerator.value() /2)  );
            } 
            if(yawMode == 0 || yawMode == 2) {
              controller.setAccelerator(accelerator.value());
              controller.setBrake(brake.value());
              controller.setClutch(clutch.value());
            }
            
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
         case STATE_PLOT: 
            Serial.print(accelerator.raw());
            Serial.print("\t");
            Serial.print(brake.raw());
            Serial.println();
            delay(50);
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
