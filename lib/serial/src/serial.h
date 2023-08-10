#ifndef DSERIAL_H
#define DSERIAL_H

// includes
#include <SoftwareSerial.h>

// instances
SoftwareSerial swSer(D2,D1); // rx, tx pin

// prototypes
void setupUART();

// function
void setupUART(){
  swSer.begin(9600);  // tx on swser
  Serial.begin(9600); // rx here
  //Serial.begin(115200); // rx here
}

#endif // DSERIAL_H