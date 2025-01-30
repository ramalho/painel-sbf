# SourceSelector

Arduino code for a control panel to select power source for one model railroad track.



## Maintenance mode

Panel has a jumper that is normally connected from ground to pin configured as INPUT_PULLUP.

To disable panel for maintenace, disconnect this jumper.
This turns off all leds and relays.
For details, see disable_panel() function, called from loop().