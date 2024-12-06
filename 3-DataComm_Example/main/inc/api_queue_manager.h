/*
 * Author: Elielder 
 */

#ifndef _API_TASKS_H_
#define _API_TASKS_H_

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define THREE_BIN_SYNC  0
#define ONE_BIN_SYNC    1
#define MUTEX           2

#define QUEUE_TYPE        MUTEX

// Task creation function prototype
void CreateTasks(void);
// Queue creation function prototype
void CreateQueue(void);

#endif // _API_TASKS_H_
