#ifndef DWIFI_H
#define DWIFI_H

#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <tasks.h>

// function prototypes
void startWiFi(); 
void startOTA();
void startDebug();
void startWebServer();
void handleRoot();
void handleNotFound();

// Wifi passwords
#include <secrets.h>

// WifiMulti
ESP8266WiFiMulti wifiMulti;

// WebServer
ESP8266WebServer server(80);

void startWebServer(){
  server.on("/", handleRoot);
  
  server.on("/manMowOn=1", [](){
    manMowOn = 1;
    EEPROM.put(0, manMowOn);
    EEPROM.commit();
    debugI("Writing manMowOn=1.");
    String message = "Turned ON manual mode override.<br>";
    message += "<a href=\"/\">Go back</a>";
    server.send(200, "text/html", message);
    
  });

  server.on("/manMowOn=0", [](){
    manMowOn = 0;
    EEPROM.put(0, manMowOn);
    EEPROM.commit();
    debugI("Writing manMowOn=0.");
    String message = "Turned OFF manual mode override.<br>";
    message += "<a href=\"/\">Go back</a>";
    server.send(200, "text/html", message);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  DEBUG_PRINTLN("Webserver start.");
}

void handleRoot() {
  String message = "<h1>Automower ESP8266 configuration.</h1>";
  message += "Override auto mode: ";
  message += manMowOn ? "ON " : "OFF ";
  message += "<a href=\"manMowOn=";
  message += manMowOn ? "0\">Turn OFF" : "1\"> Turn ON";
  message += "</a><br>";
  message += "Stop mowing after: ";
  message += maxMowTime;
  message += " minutes.<br>";
  server.send(200, "text/html", message); 
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// StartWiFi
void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
  //WiFi.softAP(ssid, password);             // Start the access point
  WiFi.mode(WIFI_STA);

  wifiMulti.addAP(ssid_ap1, ssid_pass_ap1);   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP(ssid_ap2, ssid_pass_ap2);
  wifiMulti.addAP(ssid_ap3, ssid_pass_ap3);
  
  DEBUG_PRINTLN("Connecting");
  //while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
  while (wifiMulti.run() != WL_CONNECTED) {  // Wait for the Wi-Fi to connect
    delay(250);
    DEBUG_PRINT('.');
  }
  DEBUG_PRINTLN("\r\n");
  
  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
    DEBUG_PRINT("Connected to ");
    DEBUG_PRINTLN(WiFi.SSID());             // Tell us what network we're connected to
    DEBUG_PRINT("IP address:\t");
    DEBUG_PRINT(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
  } else {                                   // If a station is connected to the ESP SoftAP
    DEBUG_PRINT("Station connected to ESP8266 AP");
  }
  // register in wifi and mdns
  //String hostNameWifi = STASSID;
  //hostNameWifi.concat(".local");
  //WiFi.hostname(hostNameWifi);
  #ifdef USE_MDNS
  MDNS.begin(STASSID);
  MDNS.addService("telnet", "tcp", 23); // for remotedebugger
  #endif
  DEBUG_PRINTLN("\r\n");
}


// StartOTA
void startOTA(){ 
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPassword);

  ArduinoOTA.onStart([]() {
    stopTasks();
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //DBG_OUTPUT_PORT.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DBG_OUTPUT_PORT.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) DEBUG_PRINTLN("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) DEBUG_PRINTLN("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) DEBUG_PRINTLN("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) DEBUG_PRINTLN("Receive Failed");
    else if (error == OTA_END_ERROR) DEBUG_PRINTLN("End Failed");
    startTasks(); // Restart in case of failure
  });
  ArduinoOTA.begin();
  DEBUG_PRINTLN("OTA ready. Waiting for OTA 60 seconds.");

  // Allow for OTA during first 60 seconds startup even if our code is broken.
  //Serial.println("Waiting 60sec for OTA.");
  unsigned long start=millis();
  while (true) {
    if( (unsigned long)(millis()-start) >= 60*1000 ) break;
    ArduinoOTA.handle();  // listen for OTA events
    //delay(1);
    yield();
  }
  DEBUG_PRINTLN("Continue program.");
}

void startDebug(){
  #ifndef DEBUG_DISABLE
  // Initialize RemoteDebug
	Debug.begin(STASSID); // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command
	Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	Debug.showColors(true); // Colors
  #endif
}
#endif