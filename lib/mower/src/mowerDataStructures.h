// structures

// Time struct
// Holds latest time and what value internal clock ( millis() )
// was latest synched at.
struct mowTime {
  u32_t t = 0;
  //u32_t lastClockSync = 0;
  int8_t hour = -1;
  int8_t minute = -1;
  int8_t seconds = -1;
} mowClock;

struct command {
  const char name[40];   // Text string description of command
  const uint8_t command[5];
  int32_t data[3];   // Three samples of data. Position 0 = newest data.
  uint8_t sent;    // = true -> waiting for respone, false = no request sent.
  u32_t t; // time latest sample arrived, decides if data is still valid.
} ;

// Software version specific data
#define INDEX_FW_VERSION 0
#define INDEX_LANG_VERSION 1
command read_sw_version[] = {
  {"FIRMWARE_VERSION", {0xf,0x33,0x90,0x0,0x0}},
  {"LANGUAGE_VERSION", {0xf,0x3a,0xc0,0x0,0x0}},
};
const size_t NUM_READ_SW_VERSION = sizeof(read_sw_version) / sizeof(command);

// Write commands and index to ease using.
// All keys have the correct index example to push key 5
// write_commands[5].command would achieve this.
#define INDEX_KEY_HOME 13
#define INDEX_KEY_MANAUTO 14
#define INDEX_KEY_C 15
#define INDEX_KEY_UP 16
#define INDEX_KEY_DOWN 17
#define INDEX_KEY_YES 18
#define INDEX_TIMER_ON 19
#define INDEX_TIMER_OFF 20
#define INDEX_SET_MODE_HOME 21
#define INDEX_SET_MODE_MAN 22
#define INDEX_SET_MODE_AUTO 23
#define INDEX_SET_MODE_DEMO 24

command write_commands[] = {
  {"KEY_0", {0xf,0x80,0x5f,0x0,0x0}},
  {"KEY_1", {0xf,0x80,0x5f,0x0,0x1}},
  {"KEY_2", {0xf,0x80,0x5f,0x0,0x2}},
  {"KEY_3", {0xf,0x80,0x5f,0x0,0x3}},
  {"KEY_4", {0xf,0x80,0x5f,0x0,0x4}},
  {"KEY_5", {0xf,0x80,0x5f,0x0,0x5}},
  {"KEY_6", {0xf,0x80,0x5f,0x0,0x6}},
  {"KEY_7", {0xf,0x80,0x5f,0x0,0x7}},
  {"KEY_8", {0xf,0x80,0x5f,0x0,0x8}},
  {"KEY_9", {0xf,0x80,0x5f,0x0,0x9}},
  {"PRG_A", {0xf,0x80,0x5f,0x0,0xa}},
  {"PRG_B", {0xf,0x80,0x5f,0x0,0xb}},
  {"PRG_C", {0xf,0x80,0x5f,0x0,0xc}},
  {"KEY_HOME", {0xf,0x80,0x5f,0x0,0xd}},
  {"KEY_MANAUTO", {0xf,0x80,0x5f,0x0,0xe}},
  {"KEY_C", {0xf,0x80,0x5f,0x0,0xf}},
  {"KEY_UP", {0xf,0x80,0x5f,0x0,0x10}},
  {"KEY_DOWN", {0xf,0x80,0x5f,0x0,0x11}},
  {"KEY_YES", {0xf,0x80,0x5f,0x0,0x12}},
  {"SET_TIMER_ON", {0xf,0xca,0x4e,0x0,0x0}},
  {"SET_TIMER_OFF", {0xf,0xca,0x4e,0x0,0x1}},
  {"SET_MODE_HOME", {0xf,0x81,0x2c,0x0,0x3}},
  {"SET_MODE_MAN", {0xf,0x81,0x2c,0x0,0x0}},
  {"SET_MODE_AUTO", {0xf,0x81,0x2c,0x0,0x1}},
  {"SET_MODE_DEMO", {0xf,0x81,0x2c,0x0,0x4}},
};
const size_t NUM_WRITE_COMMANDS = sizeof(write_commands) / sizeof(command);

// DATE & TIME commands
//#define INDEX_SECONDS 1
//#define INDEX_MINUTES 2
//#define INDEX_HOURS 3
command read_time_date_commands[] = {
  {"SECONDS", {0xf,0x36,0xb0,0x0,0x0}},
  {"MINUTES", {0xf,0x36,0xb1,0x0,0x0}},
  {"HOURS", {0xf,0x36,0xb2,0x0,0x0}},
  {"DAY", {0xf,0x36,0xb7,0x0,0x0}},
  {"MONTH", {0xf,0x36,0xb9,0x0,0x0}},
  {"YEAR", {0xf,0x36,0xbd,0x0,0x0}},  
};
const size_t NUM_READ_TIME_DATE_COMMANDS = sizeof(read_time_date_commands) / sizeof(command);

// DEFINE INDEXES to ease reading
#define INDEX_STATUS 0
#define INDEX_SECONDS 1
#define INDEX_MINUTES 2
#define INDEX_HOURS 3
#define INDEX_BATTERY_CAPACITY_USED 15
#define INDEX_BATTERY_TIME_SINCE_LAST_CHARGE 18
// All read commands
command read_commands[] = {
  {"STATUS", {0xf,0x1,0xf1,0x0,0x0}},
  {"SECONDS", {0xf,0x36,0xb0,0x0,0x0}},
  {"MINUTES", {0xf,0x36,0xb1,0x0,0x0}},
  {"HOURS", {0xf,0x36,0xb2,0x0,0x0}},
  {"DAY", {0xf,0x36,0xb7,0x0,0x0}},
  {"MONTH", {0xf,0x36,0xb9,0x0,0x0}},
  {"YEAR", {0xf,0x36,0xbd,0x0,0x0}},
  {"MOWER_TIME", {0xf,0x0,0x37,0x0,0x0}},
  {"PERIMETER_STATUS", {0xf,0x1,0x38,0x0,0x0}},
  {"PERIMETER_PERCENT", {0xf,0x1,0x34,0x0,0x0}},
  {"PERIMETER_REFERENCE", {0xf,0x1,0x37,0x0,0x0}},
  {"BATTERY_CHARGE_TIME", {0xf,0x1,0xec,0x0,0x0}},
  {"BATTERY_CAPACITY_MA", {0xf,0x1,0xeb,0x0,0x0}},
  {"BATTERY_CAPACITY_MAH", {0xf,0x1,0xef,0x0,0x0}},
  {"BATTERY_CAPACITY_SEARCH_START", {0xf,0x1,0xf0,0x0,0x0}},
  {"BATTERY_CAPACITY_USED", {0xf,0x2e,0xe0,0x0,0x0}},
  {"BATTERY_VOLTAGE", {0xf,0x2e,0xf4,0x0,0x0}},
  {"BATTERY_TEMPERATURE_ACTUAL", {0xf,0x2,0x33,0x0,0x0}},
  {"BATTERY_TIME_SINCE_LAST_CHARGE", {0xf,0x2,0x34,0x0,0x0}},
  {"BATTERY_TEMPERATURE_CHARGING", {0xf,0x2,0x35,0x0,0x0}},
  {"BATTERY_NEXT_TEMPERATURE_MEASUREMENT", {0xf,0x2,0x36,0x0,0x0}},
  {"CUTTINGMOTOR_SPEED", {0xf,0x2e,0xea,0x0,0x0}},
  {"RIGHT_WHEEL_SPEED", {0xf,0x24,0xbf,0x0,0x0}},
  {"LEFT_WHEEL_SPEED", {0xf,0x24,0xc0,0x0,0x0}},
};
const size_t NUM_READ_COMMANDS = sizeof(read_commands) / sizeof(command);

// Timer commands
#define INDEX_TIMER_STATUS 0
#define INDEX_TIMER1_START_HOUR 1
#define INDEX_TIMER1_START_MINUTE 2
#define INDEX_TIMER1_STOP_HOUR 3
#define INDEX_TIMER1_STOP_MINUTE 4
#define INDEX_TIMER1_END_START_HOUR 5
#define INDEX_TIMER1_END_START_MINUTE 6
#define INDEX_TIMER1_END_STOP_HOUR 7
#define INDEX_TIMER1_END_STOP_MINUTE 8
#define INDEX_TIMER2_START_HOUR 9
#define INDEX_TIMER2_START_MINUTE 10
#define INDEX_TIMER2_STOP_HOUR 11
#define INDEX_TIMER2_STOP_MINUTE 12
#define INDEX_TIMER2_END_START_HOUR 13
#define INDEX_TIMER2_END_START_MINUTE 14
#define INDEX_TIMER2_END_STOP_HOUR 15
#define INDEX_TIMER2_END_STOP_MINUTE 16

command read_timer_commands[] = {
  {"TIMER_STATUS", {0xf,0x4a,0x4e,0x0,0x0}},
  {"TIMER1_START_HOUR", {0xf,0x4a,0x38,0x0,0x0}},
  {"TIMER1_START_MINUTE", {0xf,0x4a,0x39,0x0,0x0}},
  {"TIMER1_STOP_HOUR", {0xf,0x4a,0x3a,0x0,0x0}},
  {"TIMER1_STOP_MINUTE", {0xf,0x4a,0x3b,0x0,0x0}},
  {"TIMER1_END_START_HOUR", {0xf,0x4a,0x3c,0x0,0x0}},
  {"TIMER1_END_START_MINUTE", {0xf,0x4a,0x3d,0x0,0x0}},
  {"TIMER1_END_STOP_HOUR", {0xf,0x4a,0x3e,0x0,0x0}},
  {"TIMER1_END_STOP_MINUTE", {0xf,0x4a,0x3f,0x0,0x0}},
  {"TIMER2_START_HOUR", {0xf,0x4a,0x40,0x0,0x0}},
  {"TIMER2_START_MINUTE", {0xf,0x4a,0x41,0x0,0x0}},
  {"TIMER2_STOP_HOUR", {0xf,0x4a,0x42,0x0,0x0}},
  {"TIMER2_STOP_MINUTE", {0xf,0x4a,0x43,0x0,0x0}},
  {"TIMER2_END_START_HOUR", {0xf,0x4a,0x44,0x0,0x0}},
  {"TIMER2_END_START_MINUTE", {0xf,0x4a,0x45,0x0,0x0}},
  {"TIMER2_END_STOP_HOUR", {0xf,0x4a,0x46,0x0,0x0}},
  {"TIMER2_END_STOP_MINUTE", {0xf,0x4a,0x47,0x0,0x0}},
};
const size_t NUM_READ_TIMER_COMMANDS = sizeof(read_timer_commands) / sizeof(command);


// Combine all list
typedef struct {
    command* cmds;
    size_t size;
} command_list;

command_list all_lists[] = {
    {read_sw_version, NUM_READ_SW_VERSION},
    {read_timer_commands, NUM_READ_TIMER_COMMANDS},
    {read_time_date_commands, NUM_READ_TIME_DATE_COMMANDS},
    {read_timer_commands, NUM_READ_TIME_DATE_COMMANDS},
    {read_commands, NUM_READ_COMMANDS},
};
const size_t NUM_ALL_LISTS = sizeof(all_lists) / sizeof(command_list);

/*
// All commands in a struct
// First byte is always 0xf
struct allcommands {
  uint8_t R_STATUS[5] = {0xf,0x1,0xf1,0x0,0x0};
  uint8_t R_SEKUNDE[5] = {0xf,0x36,0xb0,0x0,0x0};
  uint8_t R_MINUTE[5] = {0xf,0x36,0xb1,0x0,0x0};
  uint8_t R_STUNDE[5] = {0xf,0x36,0xb2,0x0,0x0};
  uint8_t R_TAG[5] = {0xf,0x36,0xb7,0x0,0x0};
  uint8_t R_MONAT[5] = {0xf,0x36,0xb9,0x0,0x0};
  uint8_t R_JAHR[5] = {0xf,0x36,0xbd,0x0,0x0};
  uint8_t R_TIMERSTATUS[5] = {0xf,0x4a,0x4e,0x0,0x0};
  uint8_t R_WOCHEN_TIMER1_START_STD[5] = {0xf,0x4a,0x38,0x0,0x0};
  uint8_t R_WOCHEN_TIMER1_START_MIN[5] = {0xf,0x4a,0x39,0x0,0x0};
  uint8_t R_WOCHEN_TIMER1_STOP_STD[5] = {0xf,0x4a,0x3a,0x0,0x0};
  uint8_t R_WOCHEN_TIMER1_STOP_MIN[5] = {0xf,0x4a,0x3b,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER1_START_STD[5] = {0xf,0x4a,0x3c,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER1_START_MIN[5] = {0xf,0x4a,0x3d,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER1_STOP_STD[5] = {0xf,0x4a,0x3e,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER1_STOP_MIN[5] = {0xf,0x4a,0x3f,0x0,0x0};
  uint8_t R_WOCHEN_TIMER2_START_STD[5] = {0xf,0x4a,0x40,0x0,0x0};
  uint8_t R_WOCHEN_TIMER2_START_MIN[5] = {0xf,0x4a,0x41,0x0,0x0};
  uint8_t R_WOCHEN_TIMER2_STOP_STD[5] = {0xf,0x4a,0x42,0x0,0x0};
  uint8_t R_WOCHEN_TIMER2_STOP_MIN[5] = {0xf,0x4a,0x43,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER2_START_STD[5] = {0xf,0x4a,0x44,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER2_START_MIN[5] = {0xf,0x4a,0x45,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER2_STOP_STD[5] = {0xf,0x4a,0x46,0x0,0x0};
  uint8_t R_WOCHENEND_TIMER2_STOP_MIN[5] = {0xf,0x4a,0x47,0x0,0x0};
  uint8_t R_TIMER_TAGE[5] = {0xf,0x4a,0x50,0x0,0x0};
  uint8_t R_MAEHZEIT[5] = {0xf,0x0,0x37,0x0,0x0};
  uint8_t R_VIERECKMODUS_STATUS[5] = {0xf,0x1,0x38,0x0,0x0};
  uint8_t R_VIERECKMODUS_PROZENT[5] = {0xf,0x1,0x34,0x0,0x0};
  uint8_t R_VIERECKMODUS_REFERENZ[5] = {0xf,0x1,0x37,0x0,0x0};
  uint8_t R_AKKU_LADEZEIT_MIN[5] = {0xf,0x1,0xec,0x0,0x0};
  uint8_t R_AKKU_KAPAZITAET_MA[5] = {0xf,0x1,0xeb,0x0,0x0};
  uint8_t R_AKKU_KAPAZITAET_MAH[5] = {0xf,0x1,0xef,0x0,0x0};
  uint8_t R_AKKU_KAPAZITAET_SUCHSTART_MAH[5] = {0xf,0x1,0xf0,0x0,0x0};
  uint8_t R_AKKU_KAPAZITAET_GENUTZT_MAH[5] = {0xf,0x2e,0xe0,0x0,0x0};
  uint8_t R_AKKU_SPANNUNG_MV[5] = {0xf,0x2e,0xf4,0x0,0x0};
  uint8_t R_AKKU_TEMPERATUR_AKTUELL[5] = {0xf,0x2,0x33,0x0,0x0};
  uint8_t R_AKKU_TEMPERATUR_LADEN[5] = {0xf,0x2,0x35,0x0,0x0};
  uint8_t R_AKKU_LETZTER_LADEVORGANG_MIN[5] = {0xf,0x2,0x34,0x0,0x0};
  uint8_t R_AKKU_NAECHSTE_TEMPERATURMESSUNG_SEK[5] = {0xf,0x2,0x36,0x0,0x0};
  uint8_t R_GESCHWINDIGKEIT_MESSERMOTOR[5] = {0xf,0x2e,0xea,0x0,0x0};
  uint8_t R_GESCHWINDIGKEIT_RECHTS[5] = {0xf,0x24,0xbf,0x0,0x0};
  uint8_t R_GESCHWINDIGKEIT_LINKS[5] = {0xf,0x24,0xc0,0x0,0x0};
  uint8_t R_FIRMWARE_VERSION[5] = {0xf,0x33,0x90,0x0,0x0};
  uint8_t R_SPRACHDATEI_VERSION[5] = {0xf,0x3a,0xc0,0x0,0x0};
  uint8_t W_TIMERAKTIV[5] = {0xf,0xca,0x4e,0x0,0x0};
  uint8_t W_TIMERINAKTIV[5] = {0xf,0xca,0x4e,0x0,0x1};
  uint8_t W_MODE_HOME[5] = {0xf,0x81,0x2c,0x0,0x3};
  uint8_t W_MODE_MAN[5] = {0xf,0x81,0x2c,0x0,0x0};
  uint8_t W_MODE_AUTO[5] = {0xf,0x81,0x2c,0x0,0x1};
  uint8_t W_MODE_DEMO[5] = {0xf,0x81,0x2c,0x0,0x4};
  uint8_t W_KEY_0[5] = {0xf,0x80,0x5f,0x0,0x0};
  uint8_t W_KEY_1[5] = {0xf,0x80,0x5f,0x0,0x1};
  uint8_t W_KEY_2[5] = {0xf,0x80,0x5f,0x0,0x2};
  uint8_t W_KEY_3[5] = {0xf,0x80,0x5f,0x0,0x3};
  uint8_t W_KEY_4[5] = {0xf,0x80,0x5f,0x0,0x4};
  uint8_t W_KEY_5[5] = {0xf,0x80,0x5f,0x0,0x5};
  uint8_t W_KEY_6[5] = {0xf,0x80,0x5f,0x0,0x6};
  uint8_t W_KEY_7[5] = {0xf,0x80,0x5f,0x0,0x7};
  uint8_t W_KEY_8[5] = {0xf,0x80,0x5f,0x0,0x8};
  uint8_t W_KEY_9[5] = {0xf,0x80,0x5f,0x0,0x9};
  uint8_t W_PRG_A[5] = {0xf,0x80,0x5f,0x0,0xa};
  uint8_t W_PRG_B[5] = {0xf,0x80,0x5f,0x0,0xb};
  uint8_t W_PRG_C[5] = {0xf,0x80,0x5f,0x0,0xc};
  uint8_t W_KEY_HOME[5] = {0xf,0x80,0x5f,0x0,0xd};
  uint8_t W_KEY_MANAUTO[5] = {0xf,0x80,0x5f,0x0,0xe};
  uint8_t W_KEY_C[5] = {0xf,0x80,0x5f,0x0,0xf};
  uint8_t W_KEY_UP[5] = {0xf,0x80,0x5f,0x0,0x10};
  uint8_t W_KEY_DOWN[5] = {0xf,0x80,0x5f,0x0,0x11};
  uint8_t W_KEY_YES[5] = {0xf,0x80,0x5f,0x0,0x12};
} commands;
*/