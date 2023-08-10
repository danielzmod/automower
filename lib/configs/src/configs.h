#ifndef D_CONFIGS_H
#define D_CONFIGS_H

// Global configuration and variables.
// More configuration in wifi/src/secrets.h and influx/src/influx.h
uint8_t manMowOn = 0;                 // Override auto mode, On Off. Read from flash memory on startup --> See nvm.h
const uint8_t maxMowTime = 180;       // [minutes] Maximum mowing time
const int32_t minBatCapUsed = -4000;  // [mAh] lowest allowed battery capacity before setting auto mode. 
// TODO: add maxMowTime & minBatCapUsed in EEPROM for easy user configuration.

// InfluxDB  server url. Don't use localhost, always server name or ip address.
// E.g. http://192.168.1.48:8086 (In InfluxDB 2 UI -> Load Data -> Client Libraries), 
#define INFLUXDB_URL "http://192.168.1.50:8086"
// InfluxDB v1 database name 
#define INFLUXDB_DB_NAME "mower"

#endif