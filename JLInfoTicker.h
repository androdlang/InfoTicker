/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by Johann Langhofer (johann@langhofer.net, johann.langhofer.net)
  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifndef JLInfoTicker_h
#define JLInfoTicker_h
#include "SSD1306Wire.h"

#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

class JLInfoTicker{
  typedef String (* InfoTickerCallback)(int);
  private:
    uint8_t *font;
    SSD1306Wire *oleddisplay;
    int pixelcounter=0;
    int soffset=0;
    int messageCounter=0;
    InfoTickerCallback tcb;
    unsigned int next_scroll=0;
    int scroll_delay=20;
    bool initialized=false;
    bool bottomAligned=true;
    String spacer="";
    uint8_t font_height;
    uint8_t font_width;
    String text;
    void createSpacer() {
      int cnt=1+(int)SCREEN_WIDTH/font_width;
      for(int i=0;i<cnt;i++) {
        spacer=spacer+" ";
      }
    }
  public:

  //use monospaced fonts only
  void init(SSD1306Wire *oled,const uint8_t *fontData, InfoTickerCallback cb ) {
    font=(uint8_t *)fontData;
    this->oleddisplay=oled;
    font_height=fontData[1];
    font_width=fontData[0];
    this->tcb=cb;
    createSpacer();
    this->text=spacer+cb(this->messageCounter);
    this->initialized=true;
  }
  void setScrollDelay(int sdelay) {
    this->scroll_delay=sdelay;
  }
  int getScrollDelay() {
    return  this->scroll_delay;
  }
  
  void setBottomAligned(bool ba) {
    this->bottomAligned=ba;
  }
  bool isBottomAligned() {
    return  this->bottomAligned;
  }
  
  void tick() {
    if(!initialized) return;
    unsigned long NOW = millis();
  
    if (NOW >= next_scroll) {
      next_scroll = NOW + scroll_delay;
      
      oleddisplay->setFont(font);
      oleddisplay->setTextAlignment(TEXT_ALIGN_LEFT);
      oleddisplay->setColor(BLACK);  // blank out the scrollable area
      oleddisplay->fillRect(0, bottomAligned?SCREEN_HEIGHT-font_height:0, SCREEN_WIDTH,font_height);
      oleddisplay->setColor(WHITE);  // draw text in opposite color
      oleddisplay->drawString(pixelcounter % font_width,bottomAligned?SCREEN_HEIGHT-font_height:0, text.substring(soffset,1+soffset+spacer.length()));
      pixelcounter--;
      if((pixelcounter % font_width)==0) {
        //skip multibyte chars
        if((text[soffset]&0x80)==0x80) {
          if((text[soffset]&0xE0)==0xC0) {
            soffset++;
          } else if((text[soffset]&0xF0)==0xE0) {
            soffset+=2;
          } else if((text[soffset]&0xF8)==0xF0) {
            soffset+=3;
          }
        }
        soffset=(soffset+1)%(text.length()+1);
        if(soffset==0) {
          pixelcounter=0;
          messageCounter++;
          this->text=spacer+tcb(messageCounter);
        }
      }
      oleddisplay->display();
    }
  }
};


#endif
