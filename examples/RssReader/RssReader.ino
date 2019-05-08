
// THIS FILE IS PART OF A PROJECT AND FOR DEMONSTRATION ONLY!!
// (c) Johann Langhofer 2018
#include "Monospaced_plain_12.h"

//#include "SPIFFS.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
//#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//#include "SSD1306.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "OLEDDisplayUi.h"

SSD1306Wire  display(0x3c, 5,4);
#include "JLInfoTicker.h"
JLInfoTicker infoTicker;

#include "WiFiClient.h"
#include <SimpleTimer.h>
SimpleTimer timer;
WiFiClient client;
const int httpPort = 80;
const char* host = "10.0.0.43";
String url = "/rss/rss.php";
int rssLineOffset=0;
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
String rssLines[]={
"","","","","","","",""
};
//SSD1306  display(0x3C, 5, 4);//Your values may be different

//Clock-------------------------------------------------------------
class Clock {
  #define TZ_INFO "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" // Western European Time
  typedef void (*display_callback)(char *);
  time_t now;
  //struct tm timeinfo;
  tm local;
  char buf[10];
  public: 
  void set(void) {
   configTzTime(TZ_INFO, "at.pool.ntp.org", "europe.pool.ntp.org");
   Serial.print("Waiting for NTP time sync: ");
   display.drawString(10, 24, "Waiting for NTP");
    now = time(nullptr);

    int maxloops=10;
    while (now < 8 * 3600 * 2) {
      delay(500);
      Serial.print(".");
      now = time(nullptr);
      maxloops--;
      if(maxloops<=0) {
        Serial.println("!giving up!");
        break;
      }
    }
    Serial.println("done");
  }
  void run(display_callback dc) {
    time_t last=now;
    now = time(nullptr);
    if(difftime(now,last)!=0.0) {
         
         getLocalTime(&local);
         //Serial.println(&local, "Datum: %d.%m.%y  Zeit: %H:%M:%S"); // Zeit Datum Print Ausgabe formatieren
         sprintf(buf,"%02d:%02d:%02d",local.tm_hour,local.tm_min,local.tm_sec);
      
        //gmtime_r(&now, &timeinfo);
        //sprintf(buf,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
        if(dc!=nullptr) dc(buf);

    }
  }
  char * getCurrentTime() {
    getLocalTime(&local);
    return asctime(&local);
  }
};
Clock myClock;

  int threshold = 40;
  bool touch1detected = false;

  void gotTouch1(){
    touch1detected = true;
  }
bool skipResponseHeaders() { 
  char endOfHeaders[] = "\r\n\r\n"; // HTTP headers end with an empty line 
  client.setTimeout(HTTP_TIMEOUT); 
  bool ok = client.find(endOfHeaders); 
  if (!ok) { Serial.println("No response or invalid response!"); } 
  return ok; 
} 
  
void handleRss() {
  Serial.println(myClock.getCurrentTime());
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    display.clear();
    display.drawString(0, 0, "Connection failed");
    display.display();
    return;
  }
  rssLineOffset=0;
  client.print("GET " + url + "?cnt=8"+" HTTP/1.1\r\n");
  client.print("Host: " + (String)host + "\r\n");
  // If authorisation is needed it can go here
  //client.print("Authorization: Basic AUTHORISATION_HASH_CODE\r\n");
  client.print("User-Agent: Arduino/1.0\r\n");
  client.print("Cache-Control: no-cache\r\n\r\n");

  //Serial.print("GET " + url + " HTTP/1.1\r\n");
  //Serial.print("Host: " + (String)host + "\r\n");
  delay(500);
  while (!skipResponseHeaders()); 
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    rssLines[rssLineOffset]=line;
    rssLineOffset++;
    if(rssLineOffset>7 ) break;
  }
/*
    display.clear();
    //display.setFont(ArialMT_Plain_16);
    display.setFont(Open_Sans_Hebrew_Condensed_16);
    if(display.getStringWidth(line)>480) {
      display.setFont(Dialog_plain_12);
      Serial.println("font reduced...");
    }
    Serial.println(display.getStringWidth(line));
//display.setTextWrap(true);
    display.drawStringMaxWidth(0, 0,128, line);
    display.display();
    */
  //}
  client.stop();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  WiFiManager wifiManager;

  ledcSetup(0, 2000, 8);
  ledcAttachPin(13, 0);
  ledcWrite(0, 255);

  display.init();
  if (!wifiManager.autoConnect()){ //"Proto G // ESP32", "G42")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  //wifiManager.resetSettings();
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.print("local ip ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI ");
  Serial.println(WiFi.RSSI());
  
  Serial.println();
  
  //display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(10,0,WiFi.localIP().toString());
  int progress=50;
  display.drawProgressBar(70, 3, 55, 6, progress);
  display.display();
  touchAttachInterrupt(T2, gotTouch1, threshold);
  //touchAttachInterrupt(T4, [](){Serial.println("T3 detected");}, threshold);
  myClock.set();
  
  handleRss();
  timer.setInterval(300000,handleRss);
  infoTicker.init(& display,Monospaced_plain_12,[](int nr){int n=nr%8;return rssLines[n];});
  
}
void resetESP() {
    touch1detected = false;
    Serial.println("Touch 1 detected");
    WiFi.disconnect(true);   // still not erasing the ssid/pw. Will happily reconnect on next start
    WiFi.begin("0","0");       // adding this effectively seems to erase the previous stored SSID/PW
    ESP.restart();
    delay(1000);
}
void loop() {

   myClock.run([](char *buf){
    //Serial.println(buf);
   ledcWrite(0, 0);
   timer.setTimeout(20, [](){
      ledcWrite(0, 255);
      });
    
    //display.clear();
    display.setColor(BLACK);
    display.fillRect(0, 20, 128,25);

    display.setColor(WHITE);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
          
    //display.setFont(ArialMT_Plain_16);
    //display.drawString(10,0,WiFi.localIP().toString());
    display.setFont(ArialMT_Plain_24);
    display.drawString(10, 20, buf);
    display.display();
    
   });
  //delay(1000);
  infoTicker.tick();
  if(touch1detected){
    resetESP();
  }
   timer.run();

}
