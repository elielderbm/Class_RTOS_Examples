/*
 * Author: Elielder 
 */

#include "api_task_manager.h"

// Task handles
static TaskHandle_t Task1Handle = NULL;
static TaskHandle_t Task2Handle = NULL;
static TaskHandle_t Task3Handle = NULL;

//Functions prototypes
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

// Task creation function implementation
void CreateTasks(void)
{
    // Create Task1
    xTaskCreate(Task1, "Task1", 1024, NULL, 2, &Task1Handle);

    // Create Task2
    xTaskCreate(Task2, "Task2", 1024, NULL, 2, &Task2Handle);

    // Create Task3
    xTaskCreate(Task3, "Task3", 1024, NULL, 2, &Task3Handle);

    printf("\n\n*****Tasks created successfully!*****\n\n");
}

// Task1 function
void Task1(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("\nTask One - Running - 1\n");
    }
}

// Task2 function
void Task2(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {
        vTaskDelay(1500 / portTICK_PERIOD_MS);
        printf("\nTask Two - Running - 2\n");
    }
}

// Task3 function
void Task3(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        printf("\nTask Three - Running - 3\n");
    }
}

