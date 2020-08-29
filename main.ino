/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <Timer.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;
Timer t500ms;

void get_web_info()
{
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {

    WiFiClient client;

    HTTPClient http;

    //Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://192.168.4.101/data.json")) 
    {  // HTTP


      //Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) 
      {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          String payload = http.getString();
          //Serial.println(payload);  
          payload.replace("[","");
          payload.replace("]","");
          payload.replace("{","");
          payload.replace("}","");
          payload.replace(":",",");
          payload.replace("\"","");
          payload.trim();
          payload+=",";
          payload.replace("ttime_m","Am");
          payload.replace("ttime_s","As");
          payload.replace("stime_m","Bm");
          payload.replace("stime_s","Bs");
          payload.replace("nstate","C");
          payload.replace("ftime","D");
          payload.replace("ptime","E");
          //Serial.println(payload);
          Serial.print(payload);
        }
      } 
      
      else 
      
      {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    }
    else 
    {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}

void setup() 
{

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) 
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  t500ms.every(500,get_web_info);
  WiFi.mode(WIFI_STA);
  WiFi.config(IPAddress(192,168,4,102),IPAddress(192,168,4,254),IPAddress(255,255,255,0));//固定IP
  WiFiMulti.addAP("ESP8266_AP", "@Aass_esp8266");

}

void loop() 
{
  t500ms.update();
}
