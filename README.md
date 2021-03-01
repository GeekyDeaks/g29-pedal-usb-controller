# G29 pedal USB controller

Simple USB controller to allow G29 pedals to be used independently of the wheelbase

aka poors man's Leo Bodnar

# Synopsis

The G29 wheelbase firmware applies a two stage mapping to the brake input causing the lower 70%
of brake application to be approximately 2x more sensitive than the upper 30%

The purpose of this project is to allow the pedals to be used independently of the 
wheelbase and therefore use the pedal travel or pressure linearly

# Getting started

If you see type errors for the HID, check the board type supports HID

The sketch automatically starts up reporting values via USB, but it will
listend for characters on the serial port in order to switch to different
modes

    C   Start Calibration
    D   Dump Calibration Values
    M   Monitor Values
    P   Output values for plotting
    R   USB HID Reporting
    S   Save Calibration
    Y   Toggle yaw mode to allow setup of clutch and throttle for rudder
        0 = all inputs, 1 = clutch and throttle in yaw mode, 2 = clutch and throttle normal

# appendix

https://www.usb.org/sites/default/files/documents/hid1_11.pdf

https://usb.org/sites/default/files/hut1_2.pdf

https://www.thesycon.de/eng/usb_descriptordumper.shtml