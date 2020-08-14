# G29 pedal USB controller

Simple USB controller to allow G29 pedals to be used independently of the wheelbase

# Synopsis

The G29 wheelbase firmware applies a curve to the brake input causing the lower 25%
of brake application to be approximately 5x more sensitive than the upper 25%

The purpose of this project is to allow the pedals to be used independently of the 
wheelbase and therefore use the pedal travel or pressure linearly

# Getting started

You will need to copy or symlink the lib/g29 directory into the ardunio libraries 

e.g.

    ln -s ~/Documents/Arduino/g29-pedal-usb-controller/lib/g29 ~/Documents/Arduino/libraries/g29

If you see type errors for the HID, check the board type supports HID


# appendix

https://usb.org/sites/default/files/hut1_2.pdf