#ifndef D_RTC_H
#define D_RTC_H

// includes

// RTC MEMORY
extern "C" {
#include "user_interface.h"
}

typedef struct {
  unsigned long mowTime_b;    // Current cutting session in milliseconds [ms].     
  uint32_t mowState_b;        // 
  uint32_t mowStateDesired_b; // 
  uint32_t reset_counter;   // Count number of resets during/last mowing.
} rtcStore;

rtcStore rtcMem;

const uint32_t buckets=(sizeof(rtcMem)/4);

#define RTCMEMORYSTART 65
#define RTCMEMORYLEN 128

void readRTCMemory() {
  system_rtc_mem_read(RTCMEMORYSTART, &rtcMem, sizeof(rtcMem));
  yield();
}

void writeRTCMemory() {
  system_rtc_mem_write(RTCMEMORYSTART, &rtcMem, buckets*4);
  yield();
}

#endif // D_RTC_H