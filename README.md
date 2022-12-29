# SpoolScale

## Overview
Have you ever run out of filament mid-print? The SpoolScale is a simple add-on device for any 3D printer that allows you to precisely know how much filament is left on your spool.

The SpoolScale uses an HX711 amplifier to interface with many different styles of load cell, depending on what works best for your specific 3D printer and spool size. The amount of filament remaining is then shown on a large 7-segment display. A user-configurable empty spool offset allows you to swap out your filament brand and type but still get accurate readings. The SpoolScale can be run fully independent of any other system, or can be integrated with your 3D printer through GPIO, I2C, or UART to allow for more advanced operation and configuration. The SpoolScale has a high-current warning output that will trigger when the amount of filament remaining drops below a user-configurable threshold, which can be used to light an indicator or sound a buzzer, for instance. The brightness of the display is user-configurable through an analog voltage input, allowing for a wide range of implementations. Some examples include using a GPIO pin on your printer motherboard to turn off the 7-segment displays while printing, wire up a potentiometer to manually set the desired brightness, or plugging in a photoresistor so the screen turns off when the room is dark, and only turns on when you turn on the lights. 

### More information can be found on the [GitHub Wiki](https://github.com/JimHeaney/spool-scale/wiki).

## Photos & Media
(Coming Soon)

## Current Release
There are no stable versions for release. The latest WIP is Version 1.1

## Current State
The SpoolScale is currently being developed to Version 1.2.

## Instructions & Further Documentation
[See the wiki on GitHub!](https://github.com/JimHeaney/SpoolScale/wiki) 

## Support Me
You can buy me a coffee [here](https://www.buymeacoffee.com/jimheaney)!

## License
This project is licensed under the Creative Commons 4.0 Attribution-NonCommercial-ShareAlike. For more information, click [here](https://creativecommons.org/licenses/by-nc-sa/4.0/).

If you are interested in using this project under a different license (e.g. for commercial use), please contact me. 
