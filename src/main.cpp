// includes
#include <Arduino.h>
#include <stdint.h>

// global
struct rst_info *rst_dump;      // store reset info

#include <configs.h>  // Config parameters
#include <dbgDefs.h>  // Debug definitions
#include <statemachine.h>
#include <nvm.h>
#include <influx.h> 
#include <tasks.h>    // Tasks
#include <wifi.h>     // WIFI settings
#include <serial.h>
#include <mower.h>    // mower definitions and structs


// functions
void setup() {
  rst_dump = system_get_rst_info();
  readNVM();
  setupUART();
  startWiFi();
  startOTA();      // Waits for 60 sec before starting program, allows OTA flash on power on. Even if code is broken further down.
  startWebServer();
  startTasks();
  startDebug();

  debugA("Run program");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true){
    // Send request to mower
    if(sendState.sendCmdFlag) blockingTasks();
    // Write data every 10th second
    if(sendInfluxDB) { 
      if(!client_v1.writePoint(sensor)) { 
      debugE("Write point to influxdb failed."); 
      } 
      sendInfluxDB=false; 
    }
    stateChanger(); // Update state-machine
    updateClock();  // Update our internal mirror clock.


    server.handleClient(); // Web traffic
    ArduinoOTA.handle();   // listen for OTA events
    Debug.handle();
    MDNS.update();

    if( millis()%20000 == 0) printState();  

    yield();
  }
}

