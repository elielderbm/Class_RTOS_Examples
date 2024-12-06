/*
 * Author: Elielder 
 */

#ifndef _API_TASKS_H_
#define _API_TASKS_H_

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Task creation function prototype
void CreateTasks(void);
// Semaphore creation function prototype
void CreateSemaphores(void);

#endif // _API_TASKS_H_
