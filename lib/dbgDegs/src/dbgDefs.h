#ifndef DBGDEFS_H
#define DBGDEFS_H

#define USE_ARDUINO_OTA
#define USE_MDNS 
#define WEBSOCKET_DISABLED true
//#define DEBUG_DISABLE

#include <ESP8266WiFi.h>        // rmeotedebug needs

#ifdef USE_MDNS
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#endif

#include <RemoteDebug.h>        //https://github.com/karol-brejna-i/RemoteDebug

#ifndef ESP8266
#error "The board must be ESP8266 "
#endif

RemoteDebug Debug;


// DEBUG
#define DEBUG_THIS
#define DBG_OUTPUT_PORT Serial // Still needed for printf
#ifdef DEBUG_THIS
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

#endif // DBGDEFS_H