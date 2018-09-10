# JLInfoTicker
Smooth scrolling text library for the SSD1306 display / Arduino.
With this library you can display a horizontal scrolling textline on your SSD1306 display. The scrolling text will appear either on top or bottom of your display. There is no restriction about the length of the textline, the text must be provided via a callback function.
## Features
* smooth scrolling  
* no restriction about the text length
* Font, speed and alignment are parameterizable 
* easy operation
## Restrictions
You have to use a monospaced Font
## Installation
Just include the `JLInfoTicker`  in your arduino library path
## Usage
1. create an instance and initialize with following parameters:
* a SSD1306 instance
* a Font the library sould use
* a callback function 
1. call the `tick()` function periodically, typically in between the `loop()` function. 

Once the library is initialized, t
You have to call the `tick()` function periodically, typically in between the `loop()` function. 

There are only few parameters to initialize the libraray:  

## Features
