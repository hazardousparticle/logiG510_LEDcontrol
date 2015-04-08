# logitech G510 LED color control
application for controlling LED color of logitech g510 keyboards
A simple project  for controlling the LED color of logitech g510 keyboards. by default the app sweeps through each color hue (rainbow effect).
can be modifed as an API library for other projects.


key functions calls provided:
setL510_LEDColor(HANDLE, &Color) - write new color to devices opened by HANDLE
getL510_LEDColor(HANDLE) = returns color read form device
HsvToRgb(HSVColor) - converts HSV values to rgb for writing to the keyboard

Color class for storing rgb info
HSVColor object for storing hue/sat/var info

the rest of the funxtions are low level windows APIs and parts of hidapi for searching devices


====================================================================================================================================
(c) 2015 hazardousparticle, apache license used for all code (except hidapi which is owned by its respective author)

based on hidapi (http://www.signal11.us/oss/hidapi/) by signal11
