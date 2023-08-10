#ifndef D_STATEMACHINE_H
#define D_STATEMACHINE_H

// global stuff
#define S_CHARGING 0
#define S_MOWING_TO_MAN 1
#define S_MOWING_NOW 2
#define S_STOP_MOWING 3 // Set to AUTO
#define S_ERROR 4
#define S_UNKOWN 5
#define AUTO 0
#define MAN 1

uint8_t mowState=S_UNKOWN, mowStateDesired=S_UNKOWN, mowMode=AUTO; // Statemachine actual and desired state.

#include <mower.h>

// function prototypes
void stateChanger();

void stateChanger(){
  int32_t mower_status = get_data( INDEX_STATUS ); 
  int32_t time_since_last_charge = get_data(INDEX_BATTERY_TIME_SINCE_LAST_CHARGE);
  int32_t battery_capacity_used = get_data(INDEX_BATTERY_CAPACITY_USED);
  bool valid_value_time = time_since_last_charge != INVALID_VALUE;
  bool valid_value_capacity_used = battery_capacity_used != INVALID_VALUE;

  // Run actions in state
  switch(mowState) {
    case S_UNKOWN:
      if ( IS_CHARGING(mower_status) ) mowStateDesired = S_CHARGING;
      else if ( IS_MOWING(mower_status) ) mowStateDesired = S_MOWING_NOW;
      break;

    case S_CHARGING:
      if ( IS_MOWING(mower_status) ) mowStateDesired = S_MOWING_NOW;
      break;

    case S_MOWING_NOW:
      if( ( valid_value_capacity_used ) && ( battery_capacity_used < minBatCapUsed) ) mowStateDesired = S_STOP_MOWING;
      if( ( valid_value_time ) && ( time_since_last_charge >= maxMowTime ) ) mowStateDesired = S_STOP_MOWING;
          //|| minBatVolt > get_mow_batVolt() ) mowStateDesired = S_STOP_MOWING;
          // Target battery cap used -4600 mAh (means we used 6800mAh)
      break;

    case S_STOP_MOWING:
      // TODO: Add extra safety to make sure mower is searching for docking station
      //if( !IS_SEARCHING )
      if( IS_CHARGING(mower_status) ) mowStateDesired = S_CHARGING;
      break;
  }
  // Only print when a state change is requested.
  if( mowStateDesired != mowState ) printState();

  if( mowState == mowStateDesired ){
    return; // Exit if no state change is requested.
  }
  
  // Execute state change code
  if( ( mowState == S_UNKOWN || mowState == S_ERROR || mowState == S_STOP_MOWING ) 
        && ( mowStateDesired == S_CHARGING || mowStateDesired == S_UNKOWN ) ){
    debugI("mowState changed from S_UNKOWN || S_ERROR || S_STOP_MOWING -> S_CHARGING || S_UNKOWN");
  }

  else if ( mowState == S_UNKOWN 
            && mowStateDesired == S_MOWING_NOW ) {
    debugI("Mowstate changed from S_UNKOWN to S_MOWING. No action performed.");
  }

  else if ( mowState == S_CHARGING 
            && mowStateDesired == S_MOWING_NOW ) {
    debugI("mowState changed from S_CHARGING to S_MOWING_NOW.");
    // Don't send command if override is turned off.
    if(manMowOn) {
      debugI("Switching to manual mode.");
      sendMowReq(write_commands[INDEX_SET_MODE_MAN].command, sizeof(write_commands[INDEX_SET_MODE_MAN].command));
      mowMode=MAN;
    }
  }

  else if ( ( mowState==S_MOWING_NOW || mowState == S_UNKOWN )
              && mowStateDesired==S_STOP_MOWING ) {
    debugI("mowState changed from S_MOWING_NOW || S_UNKOWN to S_STOP_MOWING.");
    // Don't send command if override is turned off.
    if(manMowOn) {
      debugI("Switching to automatic mode.");
      sendMowReq(write_commands[INDEX_SET_MODE_AUTO].command, sizeof(write_commands[INDEX_SET_MODE_AUTO].command));
      mowMode=AUTO;
    }
  }

  // Update mowState
  mowState = mowStateDesired;

}


#endif // D_STATEMACHINE_H