# Rick USBley

A (failed) [Stupid hackathon](https://www.stupidhackathon.se/) hack that's never gonna give you up.


## How does it work?

It's a small Arduino running a custom firmware using the [LUFA framework](http://www.fourwalledcubicle.com/LUFA.php), which allows you to emulate lots of different USB peripherals using a regular atmega32u4, when plugged into a computer it shows up as a standard usb microphone, and if you listen to it, it's generates a loop of notes from Rick Astleys never ending [hit](https://www.youtube.com/watch?v=dQw4w9WgXcQ) using librick.


## What failed

Getting stable audio stream without glitches or from the USB driver was way more difficult than expected, more work is needed.

And doing just a bit more computation than the bare minimum during rendering seems to really break playback.


## librick

A small portable, fixed point rickrolling synth library, takes a sample rate and provides a stream of 8 bit audio samples.



## Hardware

A Arduino Micro, nothing else needed.

![Arduino Pro Micro](micro.jpg)

