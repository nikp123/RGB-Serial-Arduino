## What's this?

This is a project that tries it's best to make lighting your room easier and fun.

This is the Arduino part of it.


## What's the idea and how does it work?

The idea is simple. The Arduino recieves a serial signal telling it what task to do.

The tasks looks like this:
    
    rcolor gcolor bcolor instruction arg1 arg2

All of the variables are decimal strings, and all of them must be filled even if the instruction doesn't use them.

The following are implemented instructions:

 - instruction 0 - just change the color
 - instruction 1 - abort any operation without reseting the colors
 - instruction 2 - fade to a new color (arg1 is how long it should take in 100th's of a second)
 - instruction 3 - clear everything
 - instruction 4 - blink every arg1\*10 milliseconds
 - instruction 5 - fade to a random color every arg1\*10 milliseconds
 - instruction 6 - same as above but arg2 is the randomness threshold
 - instruction 7 - cycle through a rainbow like pattern every arg1\*10 milliseconds


## What will I need for this?

 - A computer (running Linux, macOS or Windows) with Arduino IDE and drivers installed
 - An arduino (any will suffice, but I recommend a Nano because of the small form factor and lower power usage)
 - A micro USB cable (to program the thing)
 - Some wires, a breadboard (or a circuit board if you have the patience)
 - For bluetooth (optional): an HC-05 or HC-06 module and a switch


## Getting started

When wiring up the circuit, decide whether or not to have bluetooth as a feature.

With bluetooth:

![With bluetooth](/schematic/RGBserial-with-bluetooth.png)

Without bluetooth:

![Without bluetooth](/schematic/RGBserial-without-bluetooth.png)

NOTE: Using USB serial may reset your colors when disconnecting from the client.

And upload the included INO file to your Arduino board.

Once you're done, you can use a [client program](https://github.com/nikp123/RGB-Serial-Client) that I made.

Or write you're own way of comunicating/interfacing to it ;)


## Contribute

Contributions of code and ideas are very welcome.

Just make sure that you follow the code style if you are contributing code.


### Code style

 - Use tabs.
 - Don't use some overbloated library. (try to avoid using libraries entirely)
 - Don't remove stuff unless necesarry.
 - Don't reinvent the wheel.
 - Don't reimagine the format.
 - Don't do something stupid.


## License

```
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
                    Version 2, December 2004 

 Copyright (C) 2018 Nikola Pavlica <pavlica.nikola@gmail.com> 

 Everyone is permitted to copy and distribute verbatim or modified 
 copies of this license document, and changing it is allowed as long 
 as the name is changed. 

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 

  0. You just DO WHAT THE FUCK YOU WANT TO.

```
