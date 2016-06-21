#include "util.h"
#include "user_interface.h"
#include "osapi.h"

void printFloat(float val, char *buff) {
   char smallBuff[16];
   int val1 = (int) val;
   unsigned int val2;
   if (val < 0) {
      val2 = (int) (-100.0 * val) % 100;
   } else {
      val2 = (int) (100.0 * val) % 100;
   }
   if (val2 < 10) {
      os_sprintf(smallBuff, "%i.0%u", val1, val2);
   } else {
      os_sprintf(smallBuff, "%i.%u", val1, val2);
   }

   strcat(buff, smallBuff);
} // See more at: http://www.esp8266.com/viewtopic.php?p=14265#sthash.yUjsnuYP.dpuf
