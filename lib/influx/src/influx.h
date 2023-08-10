#ifndef INFLUX_H 
#define INFLUX_H

#include <InfluxDbClient.h>

// InfluxDB  server url. Don't use localhost, always server name or ip address.
// E.g. http://192.168.1.48:8086 (In InfluxDB 2 UI -> Load Data -> Client Libraries), 
//#define INFLUXDB_URL "http://192.168.1.50:8086"
// InfluxDB v1 database name 
//#define INFLUXDB_DB_NAME "mower"

// InfluxDB client instance for InfluxDB 1
InfluxDBClient client_v1(INFLUXDB_URL, INFLUXDB_DB_NAME);

// Data point
Point sensor("amw");

#endif