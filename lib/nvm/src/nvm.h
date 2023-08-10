#include <EEPROM.h>
void readNVM();

void readNVM(){
  EEPROM.begin(1);
  EEPROM.get(0, manMowOn);
}