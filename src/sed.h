
#ifndef __SED_H__
#define __SED_H__

#include "stdint.h"
#include "stdbool.h"

#define MAX_SED_REPORT_PAYLOAD 32

/** 
 * Leds report
 * Output Report ID: 2
 **/
typedef struct __attribute__((__packed__)) {
  uint32_t leds;
} led_output_report_t;

/** 
 * Jog mode report
 * Input/Output Report ID: 3
 **/
typedef struct __attribute__((__packed__)) {
  uint8_t  mode;
  int32_t  value;
  uint8_t  unknown;
} jog_inout_report_t;

/** 
 * Keys report
 * Input Report ID: 4
 **/
typedef struct __attribute__((__packed__)) {
  uint16_t keys[6];
} keys_input_report_t;

/** 
 * Jog leds report
 * Output Report ID: 4
 **/
typedef struct __attribute__((__packed__)) {
  uint8_t jog_leds;
} jogleds_output_report_t;


/** 
 * Battery report
 * Input Report ID: 7
 **/
typedef struct __attribute__((__packed__)) {
  uint8_t charging;
  uint8_t batt_level;
} battery_input_report_t;


typedef struct {
  uint8_t id;
  uint8_t type;
  uint8_t len;
  union {
    uint8_t                 raw[MAX_SED_REPORT_PAYLOAD];
    led_output_report_t     led_out;
    jog_inout_report_t      jog_inout;
    keys_input_report_t     keys_in;
    jog_inout_report_t      jogled_out;
    battery_input_report_t  battery_in;
  } payload;
} sed_report_t;



#endif