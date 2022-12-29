# SpoolScale

## Overview
Have you ever run out of filament mid-print? The SpoolScale is a simple add-on device for any 3D printer that allows you to precisely know how much filament is left on your spool. The SpoolScale can be integrated with any style 3D printer and spool holder, and can be connected to any wheatstone bridge load cell. Additional functionality can be unlocked by connecting the SpoolScale to a 3D printer directly via GPIO, i2C, or UART. 


### More information can be found on the [GitHub Wiki](https://github.com/JimHeaney/spool-scale/wiki).


## Current Release
There are no stable versions for release. The latest WIP is Version 1.1

## Current State
The SpoolScale is currently being developed to Version 1.2.

## What's New
Version 1.2 will likely be the first stable release of the SpoolScale. Version 1.1 was used as a proof of concept and for small-scale field testing. A number of bugs were immediately identified, and a number of other improvements will be made based on user feedback. These include:
* Removing DIP switch configuration, instead adding RJ11 port for connecting an i2C keypad. 
  * This will allow for more versatile configuration, including typing in spool weights directly, saving/loading 4 "favorite" spool weights, etc. 
  * All of these settings will be exposed as i2C slave registers, so the keypad is not needed if using the SpoolScale as an i2C device. 
* Added more robust switch for selecting SpoolScale mode 
  * Removed UART mode, options are now GPIO, standalone, and i2C.
* Redid layout of PCB, removed secondary AtTiny footprint
* General component optimization, layout improvements, etc. 


## Instructions & Further Documentation
[See the wiki on GitHub!](https://github.com/JimHeaney/SpoolScale/wiki) 

## Photos & Media
![image](https://user-images.githubusercontent.com/20119374/209997095-e3109bc1-f0b0-4272-b7c6-e466496a7a56.png)

## Support Me
You can buy me a coffee [here](https://www.buymeacoffee.com/jimheaney)!

## License
This project is licensed under the Creative Commons 4.0 Attribution-NonCommercial-ShareAlike. For more information, click [here](https://creativecommons.org/licenses/by-nc-sa/4.0/).

If you are interested in using this project under a different license (e.g. for commercial use), please contact me. 

