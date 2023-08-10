#ifndef TASKS_H 
#define TASKS_H

#include <dbgDefs.h>
#include <Ticker.h>
#include <mower.h>

// instances
Ticker sendCommand, readBuffer, wrteLog, getTime, sendDB; // Tasks
bool sendInfluxDB=false;
const uint8_t fac = 1;

// sendState
struct sendTrack {
  bool sendCmdFlag = false;
  uint8_t sendId=0;
  u32_t t;
} sendState;

// Prototypes
void startTasks();
void stopTasks();
void blockingTasks();
void sendCmd();
void readBuf();
void sendToDb();

// Functions
void startTasks() {
  sendCommand.attach(fac*10, sendCmd);    // Task that sends commands periodically.
  readBuffer.attach(fac*0.1, readBuf);   // Takes care of data in serial buffer
  sendDB.attach(fac*10, sendToDb);
}

void stopTasks() {
  DEBUG_PRINTLN("stopTasks");
  sendCommand.detach();    // Task that sends commands periodically.
  readBuffer.detach();   // Takes care of data in serial buffer
  sendDB.detach();
}

// Send measurement data to influxdb
void sendToDb(){
  // Clear old data.
  sensor.clearFields();
  // Store measured value into point
  for( size_t i = 0; i < NUM_READ_COMMANDS; ++i ) {
    if ( INVALID_VALUE != get_data( i ) ) sensor.addField((String)read_commands[i].name, read_commands[i].data[0]);
  }
  sensor.addField("rstReason", rst_dump->reason);
  sensor.addField("excCause", rst_dump->exccause);
  sensor.addField("mowState", mowState);
  sensor.addField("mowStateDesired", mowStateDesired);
  sensor.addField("millis", millis());
  sensor.addField("manMowOn", manMowOn);
  
  // Print what are we exactly writing
  //debugV("%s", client_v1.pointToLineProtocol(sensor).c_str());
  // Cant be called inside task
  //client_v1.writePoint(sensor)

  sendInfluxDB = true;
}

void blockingTasks(){
  bool send_nok=false;

  // Send if timeout or response receieved.
  if((sendState.sendCmdFlag 
      && (unsigned long)(millis()-sendState.t) > 100) ) { // Wait 100ms between requests 
    
    uint8_t idx = sendState.sendId; // Index in read_commands to send. 
    send_nok = sendMowReq((uint8_t*)read_commands[idx].command, sizeof(read_commands[idx].command));
    
    if ( send_nok ){
      debugE("Failed to send command to autmower.");
    } else {
      read_commands[idx].sent = true;
    }
    sendState.t = millis(); // last time of execution
    if(sendState.sendId--==0) sendState.sendCmdFlag=false; // all messages sent.
  }
}

// set condition to send requests in blockingTasks()
void sendCmd(){
  sendState.sendId=NUM_READ_COMMANDS-1; // index starts from 0.
  sendState.sendCmdFlag = true;
}

// Read serial buffer in 5 bytes chunks and process response.
void readBuf(){
  uint8_t recvAutomower[6] = "";

  // Consume characters until 0x0f
  // A bad connection or interference could cause unexpected bytes.
  while( Serial.peek() != 0x0f 
         && Serial.available() ) {   
    debugE("Expected 0xf got 0x%x. Removing single byte from buffer.", Serial.peek());
    Serial.read();
  }
  
  // Read data in 5 bytes chunks.
  while (Serial.available() >= 5) {            
    int n = Serial.readBytes(recvAutomower, 5);
    if ( n == 5) processResp(recvAutomower, n);        // Process response
    memset(recvAutomower, 0, sizeof(recvAutomower));   // Clean array for next 5 byte chunk
    }
}

#endif // TASKS_H