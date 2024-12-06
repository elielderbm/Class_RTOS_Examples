/*
 * Author: Elielder 
 */

#include "api_sync_manager.h"

// Task handles
static TaskHandle_t Task1Handle = NULL;
static TaskHandle_t Task2Handle = NULL;
static TaskHandle_t Task3Handle = NULL;

//Functions prototypes
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

// Semaphore handles
SemaphoreHandle_t SemTask1;
SemaphoreHandle_t SemTask2;
SemaphoreHandle_t SemTask3;

// Task creation function implementation
void CreateTasks(void)
{
    // Create Task1
    xTaskCreate(Task1, "Task1", 1024*4, NULL, 3, &Task1Handle);

    // Create Task2
    xTaskCreate(Task2, "Task2", 1024*4, NULL, 3, &Task2Handle);

    // Create Task3
    xTaskCreate(Task3, "Task3", 1024*4, NULL, 3, &Task3Handle);

    printf("\n\n*****Tasks created successfully!*****\n\n");
}

//Semaphore creation function implementation
void CreateSemaphores(void)
{
    // Create binary semaphores
    SemTask1 = xSemaphoreCreateBinary();
    SemTask2 = xSemaphoreCreateBinary();
    SemTask3 = xSemaphoreCreateBinary();

    if (SemTask1 == NULL || SemTask2 == NULL || SemTask3 == NULL)
    {
        printf("\n\n*****Semaphores creation failed!*****\n\n");
        for (;;); // Halt if semaphore creation fails
    }
    printf("\n\n*****Semaphores created successfully!*****\n\n");

    // Give the first semaphore to start the sequence
    xSemaphoreGive(SemTask1);
}

// Task1 function
void Task1(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {
        // Wait for semaphore
        if (xSemaphoreTake(SemTask1, portMAX_DELAY) == pdTRUE)
        {
            printf("\nTask One - Running - 1\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            // Signal Task2
            xSemaphoreGive(SemTask2);
        }
    }
}

// Task2 function
void Task2(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {
        // Wait for semaphore
        if (xSemaphoreTake(SemTask2, portMAX_DELAY) == pdTRUE)
        {
            printf("\nTask Two - Running - 2\n");
            vTaskDelay(500 / portTICK_PERIOD_MS);

            // Signal Task3
            xSemaphoreGive(SemTask3);
        }
    }
}

// Task3 function
void Task3(void *pvParameters)
{
    (void)pvParameters;
    for (;;)
    {
        // Wait for semaphore
        if (xSemaphoreTake(SemTask3, portMAX_DELAY) == pdTRUE)
        {
            printf("\nTask Three - Running - 3\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // Signal Task1 to restart the sequence
            xSemaphoreGive(SemTask1);
        }
    }
}

