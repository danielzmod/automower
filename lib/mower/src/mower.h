#ifndef MOWER_H
#define MOWER_H

// includes
#include "mowerDataStructures.h"
#include <serial.h> 

// definitions
#define INVALID_VALUE -99999
#define MOWER_STATUS get_data( INDEX_STATUS )
#define IS_CHARGING(x)  x >= 1014 && x <= 1024 // 1024>=x>=1014 => mower in charger
#define IS_MOWING(x)  x >= 1002 && x <= 1008  // 1008>=x>=1002 => mower is mowing

// First byte should always be 0x0f omitting that
			// 	"6" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Linker Radmotor blockiert"}
			// 	"12" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Kein Schleifensignal"}
			// 	"16" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Außerhalb"}
			// 	"18" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Niedrige Batteriespannung"}
			// 	"26" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Ladestation blockiert"}
      //  24 something wrong
      //  28 behöver laddas manuellt.
      // 30 some error code.
			// 	"34" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Mäher hochgehoben"}
			// 	"52" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Kein Kontakt zur Ladestation"}
			// 	"54" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Pin abgelaufen"}
			// 	"1000" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Aus LS ausfahren"}
			// 	"1002" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Mähen"}
			// 	"1006" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Mähwerk starten"}
			// 	"1008" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Mähwerk gestartet"}
			// 	"1012" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Signal starte Mähwerk"}
			// 	"1014" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Laden"}
			// 	"1016" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: in LS wartend"}
			// 	"1024" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: aus LS einfahren"}
			// 	"1036" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Viereckmodus"}
			// 	"1038" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Festgefahren"}
			// 	"1040" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Kollision"}
			// 	"1042" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Suchen"}
			// 	"1044" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Stop"}
			// 	"1048" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Andocken"}
			// 	"1050" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: aus LS ausfahren"}
			// 	"1052" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Fehler"}
			// 	"1056" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Wartet (Modus Manuell/Home)"}
			// 	"1058" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Begrenzung folgen"}
			// 	"1060" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: N-Signal gefunden"}
			// 	"1062" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Festgefahren"}
			// 	"1064" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Suchen"}
			// 	"1070" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Suchschleife folgen"}
			// 	"1072" {set automower_status "[clock format [clock sec] -format %H:%M:%S]: Schleife folgen"}
			// }


// function prototypes
int sendMowReq(const uint8_t *request, uint8_t len);
int processResp(uint8_t *data, uint8_t len);
int32_t get_data(uint8_t index); // Check data is valid before getting it. 
int time_valid( u32_t t); 
void updateClock(); // Maintain and sync clock our mirror clock of that in automower.
void printState();
command* find_command_in_list(command commands[], size_t num_commands, uint8_t search_command[5]); // Find command in a known list
command* find_command_in_all_lists(uint8_t search_command[5]);

// variables
unsigned long lastClockSync=0; // time when internal Clock was synced with mower clock.

// functions
// Function to find a command based on the command array
command* find_command_in_list(command commands[], size_t num_commands, uint8_t search_command[5]) {

    for (size_t i = 0; i < num_commands; ++i) {
        if (memcmp(commands[i].command, search_command, 3) == 0) {
            return &commands[i];
        }
    }
    return NULL;  // return NULL if command not found
}

// Find command in all lists
command* find_command_in_all_lists(uint8_t search_command[5]) {
    for (size_t i = 0; i < NUM_ALL_LISTS; ++i) {
        command* cmd = find_command_in_list(all_lists[i].cmds, all_lists[i].size, search_command);
        if (cmd != NULL) {
            return cmd;
        }
    }
    return NULL;
}

void printState(){
  debugD("reason: %u, exccause: %u", rst_dump->reason, rst_dump->exccause);
  debugI("mowMode: %u, State: %u, mowStateDesired: %u, RSSI: %i", mowMode, mowState, mowStateDesired, WiFi.RSSI());
  debugI("mowClock: %d:%d:%d", mowClock.hour, mowClock.minute, mowClock.seconds);
  debugI("mowStatus: %i, manMowOn: %u", MOWER_STATUS, manMowOn);
}

int sendMowReq(const uint8_t *request, uint8_t len){
  if(len < 5 || len > 5) return 1;
  swSer.write(request, 5);
  return 0;
}

int processResp(uint8_t *data, uint8_t len){
  debugV("checkResp 0x%0*x 0x%0*x 0x%0*x 0x%0*x 0x%0*x to autmower", 2, data[0], 2, data[1], 2, data[2], 2, data[3], 2, data[4]); 

  if(data[0] != 0x0f || len != 5){
    debugE("Unexpected data at first byte in response or incorrect length.\r");
    return 1;
  }

  command* cmd = find_command_in_list(read_commands, NUM_READ_COMMANDS, data);
  //command* cmd = find_command_in_all_lists(data);
  if( NULL == cmd){
    debugE("no command match, response 0x%0*x 0x%0*x 0x%0*x 0x%0*x 0x%0*x.", 2, data[0], 2, data[1], 2, data[2], 2, data[3], 2, data[4]); 
    return 1;
  } else {
    u32_t t = millis();
    uint32_t respData = static_cast<int16_t>(data[4] << 8 | data[3]);
    //uint32_t respCode = data[1] << 8 | data[2];

    // Store data and update tracking status.
    cmd->data[2] = cmd->data[1];
    cmd->data[1] = cmd->data[0]; 
    cmd->data[0] = respData;
    cmd->t = t;
    debugD("%s = %d, sent=%1u", cmd->name, cmd->data[0], cmd->sent);
    cmd->sent = false;
  }

  return 0;
}

int32_t get_data(uint8_t index) {
  if ( time_valid( read_commands[index].t ) ) {
    return read_commands[index].data[0];
  }
  return INVALID_VALUE;
} 

int time_valid(u32_t t){
  return (u32_t)(millis()-t) <= 60*1e3;
}

void updateClock(){
  // Update our mirror clock of that in automower.
  if ( (unsigned long)(millis()-mowClock.t) >= 1000 ) {
    //debugV("Update clock. %u:%u:%u", mowClock.hour, mowClock.minute, mowClock.seconds);
    if ( ++mowClock.seconds == 60) {
      mowClock.minute++;
      mowClock.seconds = 0;
    }
    if ( mowClock.minute == 60 ) {
      mowClock.hour++;
      mowClock.minute = 0;
    }
    if ( mowClock.hour == 24) mowClock.hour = 0;

    mowClock.t = millis(); // Update last time updated.
    debugV("New clock. %d:%d:%d", mowClock.hour, mowClock.minute, mowClock.seconds);
  }
  
  // Synchronize internal clock with mower every half an hour.
  if( (unsigned long)(millis()-lastClockSync) >= 30*60*1000 || lastClockSync==0 ) {
    int32_t hours = get_data( INDEX_HOURS );
    int32_t minutes = get_data( INDEX_MINUTES );
    int32_t seconds = get_data( INDEX_SECONDS );
    
    debugD("Sync clock. lastClockSync=%lu", lastClockSync);

    if (   INVALID_VALUE != hours 
        && INVALID_VALUE != minutes
        && INVALID_VALUE != seconds ) {

      mowClock.hour = hours; 
      mowClock.minute = minutes;
      mowClock.seconds = seconds; 
      lastClockSync = millis(); 
    }          
  }
}

#endif // MOWER_H