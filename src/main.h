#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>

#include "hid_def.h"
#include "sed.h"

/**
 * HID Messages coming from the SED going to the PC 
 **/
#define MAX_SED_INPUT_MSG_QUEUE_SIZE       (5)
#define MAX_REMAPPED_INPUT_MSG_QUEUE_SIZE  (10)
extern QueueHandle_t sed_input_msg_queue;
extern QueueHandle_t remapped_input_msg_queue;

/**
 * HID Messages coming from the PC going to the SED
 **/
#define MAX_REMAPPED_OUT_MSG_QUEUE_SIZE  (5)
#define MAX_SED_OUT_MSG_QUEUE_SIZE       (5)
extern QueueHandle_t remapped_output_msg_queue;
extern QueueHandle_t sed_output_msg_queue;



#endif