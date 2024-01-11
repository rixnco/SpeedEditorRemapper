#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <Arduino.h>


#define DBG_INIT()        Serial1.begin(115200)
#define DBG_PRINT(...)    Serial1.print(__VA_ARGS__)
#define DBG_PRINTLN(...)  Serial1.println(__VA_ARGS__)
#define DBG_PRINTF(...)   Serial1.printf(__VA_ARGS__)
#define DBG_FLUSH()       Serial1.flush()

#endif