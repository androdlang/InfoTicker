# JLInfoTicker
Smooth scrolling text library for the SSD1306 display / Arduino.
With this library you can display a horizontal scrolling textline on your SSD1306 display. The scrolling text will appear either on top or bottom of your display. There is no restriction about the length of the textline! This is especially useful for displaying live messages such as feeds or measurement data.
## Features
* smooth scrolling  
* no restriction about the text length
* text can be provided dynamically
* Font, speed and alignment are parameterizable 
* easy operation
## Restrictions
You have to use a monospaced Font
## Installation
Just include the `JLInfoTicker`  in your arduino library path
## Usage
1. create an instance and initialize with following parameters:
  * the SSD1306 instance
  * the Font the library will use
  * the callback function for providing your text
2. call the `tick()` function periodically, typically in between the `loop()` function. 
## Inteface
* `void init(SSD1306Wire *oled,const uint8_t *fontData, InfoTickerCallback cb )'
* `void setScrollDelay(int sdelay)`
* `int getScrollDelay()`
* `void setBottomAligned(bool ba)`
* `bool isBottomAligned()`
* `void tick()`
## Remarks
The callback function for providing your text has following signature: `typedef String (* InfoTickerCallback)(int);`
This function will be called every time a text is required, this can be for the first text or if a previous text was fully scrolled. The incoming parameter is an incrementing counter value which you can use for your logic to return your desired text, 
for example: periodically alternating 4 textlines:
`
String abc[]={"Text line AAA", "Text line BBB","Text line CCC,"Text line DDD"};
infoTicker.init(& display,Monospaced_plain_12,[](int nr){int n=nr%4;return abc[n];});
`
