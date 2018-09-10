#include "Monospaced_plain_12.h"

#include <SSD1306Wire.h>
SSD1306Wire display(0x3c,5,4);
#include "JLInfoTicker.h"

JLInfoTicker infoTicker;

String abc[]={
              "USA wollen Palästinenser-Vertretung in Washington schließen",
              "Pensionist aus Wien war auf A3 als Geisterfahrer unterwegs",
              "Das Schauspielhaus eröffnet die neue Saison",
              "Neue Skulptur in New York: Hund balanciert Taxi auf der Nase",
              "UN-Teams sollen Schutz von Immigranten in Österreich prüfen",
              "Wifo sieht anhaltend günstige Wirtschaftslage in Österreich",
              "Zwei Geschleppte in türkischem Lkw in OÖ entdeckt",
              "Lungau: Wohnmobil krachte am Stauende auf einen Pkw - zwei Verletzte",              
              };


void setup() {
  Serial.begin(115200);

  display.init();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64,20,"JLInfoTicker");
  display.display();

  infoTicker.init(& display,Monospaced_plain_12,[](int nr){int n=nr%8;return abc[n];});
}

void loop() {
    infoTicker.tick();

}
