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

#if SEM_TYPE == THREE_BIN_SYNC   
// Semaphore handles
SemaphoreHandle_t SemTask1;
SemaphoreHandle_t SemTask2;
SemaphoreHandle_t SemTask3;
#elif SEM_TYPE == ONE_BIN_SYNC 
// Semaphore handle
SemaphoreHandle_t syncSemaphore;
#elif SEM_TYPE == MUTEX 
// Shared buffer and mutex
char sharedBuffer[100];
SemaphoreHandle_t bufferMutex;
#endif

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
    #if SEM_TYPE == THREE_BIN_SYNC 
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
    #elif SEM_TYPE == ONE_BIN_SYNC 
    // Create a binary semaphore
    syncSemaphore = xSemaphoreCreateBinary();
    if (syncSemaphore == NULL)
    {
        printf("\n\n*****Semaphore creation failed!*****\n\n");
        for (;;); // Halt if semaphore creation fails
    }
    printf("\n\n*****Semaphore created successfully!*****\n\n");
    
    // Initially give the semaphore to start Task1
    xSemaphoreGive(syncSemaphore);
    #elif SEM_TYPE == MUTEX 
    // Create the mutex
    bufferMutex = xSemaphoreCreateMutex();
    if (bufferMutex == NULL)
    {
        printf("\n\n*****Mutex creation failed!*****\n\n");
        for (;;); // Halt if semaphore creation fails
    }
    printf("\n\n*****Mutex created successfully!*****\n\n"); 
    #endif
}

// Task1 function
void Task1(void *pvParameters)
{
    #if SEM_TYPE == THREE_BIN_SYNC
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
    #elif SEM_TYPE == ONE_BIN_SYNC 
    (void)pvParameters;
    for (;;)
    {
        // Wait for semaphore
        if (xSemaphoreTake(syncSemaphore, portMAX_DELAY) == pdTRUE)
        {
            //Do an operation
            printf("\nTask One - Running - 1\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // Give semaphore to Task2
            xSemaphoreGive(syncSemaphore);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    #elif SEM_TYPE == MUTEX 
    (void)pvParameters;
    int counter = 0;
    for (;;)
    {
        // Lock the mutex
        if (xSemaphoreTake(bufferMutex, portMAX_DELAY) == pdTRUE)
        {
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // Modify the shared buffer
            printf("Task One - Running - 1\n");
            snprintf(sharedBuffer, sizeof(sharedBuffer), "Message %d", counter++);
            printf("Producer: Updated buffer to '%s'\n\n", sharedBuffer);

            // Unlock the mutex
            xSemaphoreGive(bufferMutex);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    #endif

}

// Task2 function
void Task2(void *pvParameters)
{
    #if SEM_TYPE == THREE_BIN_SYNC 
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
    #elif SEM_TYPE == ONE_BIN_SYNC 
    (void)pvParameters;
    for (;;)
    {
        // Wait for semaphore
        if (xSemaphoreTake(syncSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Do an operation
            printf("\nTask Two - Running - 2\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS); 

            // Give semaphore to Task3
            xSemaphoreGive(syncSemaphore);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
    #elif SEM_TYPE == MUTEX 
    (void)pvParameters;
    for (;;)
    {
        // Lock the mutex
        if (xSemaphoreTake(bufferMutex, portMAX_DELAY) == pdTRUE)
        {
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // Access the shared buffer
            printf("Task Two - Running - 2\n");
            printf("Consumer1: Read buffer as '%s'\n\n", sharedBuffer);

            // Unlock the mutex
            xSemaphoreGive(bufferMutex);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    #endif
}

// Task3 function
void Task3(void *pvParameters)
{
    #if SEM_TYPE == THREE_BIN_SYNC 
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
    #elif SEM_TYPE == ONE_BIN_SYNC 
    (void)pvParameters;
    for (;;)
    {
        // Wait for semaphore
        if (xSemaphoreTake(syncSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Do an operation
            printf("\nTask Three - Running - 3\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // Give semaphore to Task1 to restart the sequence
            xSemaphoreGive(syncSemaphore);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    #elif SEM_TYPE == MUTEX 
    (void)pvParameters;
    for (;;)
    {
        // Lock the mutex
        if (xSemaphoreTake(bufferMutex, portMAX_DELAY) == pdTRUE)
        {
            vTaskDelay(2000 / portTICK_PERIOD_MS);

            // Access the shared buffer
            printf("Task Three - Running - 3\n");
            printf("Consumer2: Read buffer as '%s'\n\n", sharedBuffer);

            // Unlock the mutex
            xSemaphoreGive(bufferMutex);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    #endif
}

