# JLInfoTicker
Smooth scrolling text library for the SSD1306 display / Arduino.
With this library you can display a horizontal scrolling textline on your SSD1306 display. The scrolling text will appear either on top or bottom of your display. There is no restriction about the length of the textline, this is especially useful for displaying live messages such as feeds or measurement data.

I've written this library for the Wemos LOLIN32 board, which uses an ESP32 with a SSD1306 OLED display.
![GitHub Logo](/images/JLInfoTicker.jpg)


## Features
* smooth scrolling (per pixel) 
* no text length restriction
* dynamic text support
* Font, speed and alignment parameters
* easy operation
## Restrictions
You have to use a monospaced Font. I have provided one font with one size, but you can generate more fonts with an online generator: http://oleddisplay.squix.ch/ 
## Installation
Just include the `JLInfoTicker.h`  in your arduino library path
## Usage
1. include the `JLInfoTicker.h` file
2. create an instance and initialize with following parameters:
  * the SSD1306 instance
  * the Font the library will use
  * the callback function for providing your text
3. call the `tick()` function periodically, typically in between the `loop()` function. 
## Interface
* `void init(SSD1306Wire *oled,const uint8_t *fontData, InfoTickerCallback cb )`
* `void setScrollDelay(int sdelay)` (in ms, default 25ms)
* `int getScrollDelay()`
* `void setBottomAligned(bool ba)` (true=>bottom, false=top, default bottom)
* `bool isBottomAligned()`
* `void tick()`
## Remarks
The callback function for providing your text has following signature: `typedef String (* InfoTickerCallback)(int);`
This function will be called every time a new text is required, this can be initially or if a previous text was fully scrolled. The incoming parameter is an incrementing counter value which you can use for your logic to return your desired text, 
for example: periodically alternating 4 textlines:

`String abc[]={"Text line AAA", "Text line BBB","Text line CCC,"Text line DDD"};`

` infoTicker.init(& display,Monospaced_plain_12,[](int nr){int n=nr%4;return abc[n];});`
## Example
See the file `InfoTickerTest.ino`in the examples folder.
