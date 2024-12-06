/*
 * Author: Elielder 
 */

#include "api_queue_manager.h"

// Task handles
static TaskHandle_t Task1Handle = NULL;
static TaskHandle_t Task2Handle = NULL;
static TaskHandle_t Task3Handle = NULL;

//Functions prototypes
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

// Queue handle
QueueHandle_t DataQueue;

// Task creation function implementation
void CreateTasks(void)
{
    // Create Task1
    xTaskCreate(Task1, "Task1", 1024*4, NULL, 3, &Task1Handle);

    // Create Task2
    xTaskCreate(Task2, "Task2", 1024*4, NULL, 3, &Task2Handle);

    // Create Task3
    xTaskCreate(Task3, "Task3", 1024*4, NULL, 4, &Task3Handle);

    printf("\n\n*****Tasks created successfully!*****\n\n");
}

//Queue creation function implementation
void CreateQueue(void)
{
    DataQueue = xQueueCreate(5, sizeof(int));
    if (DataQueue == NULL)
    {
        printf("\n\n*****Queue creation failed!*****\n\n");
        for (;;); // Halt if queue creation fails
    }
    printf("\n\n*****Queue created successfully!*****\n\n");
}

// Task1 function
void Task1(void *pvParameters)
{
    (void)pvParameters;
    int valueToSend = 100;
    vTaskDelay(100 / portTICK_PERIOD_MS);
    for (;;)
    {
        printf("SenderTask1: Sending %d to queue\n", valueToSend);
        if (xQueueSend(DataQueue, &valueToSend, pdMS_TO_TICKS(1000)) == pdPASS)
        {
            printf("SenderTask1: Data sent successfully\n\n");
        }
        else
        {
            printf("SenderTask1: Failed to send data\n\n");
        }
        valueToSend += 10;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

}

// Task2 function
void Task2(void *pvParameters)
{
    (void)pvParameters;
    int valueToSend = 200;
    vTaskDelay(100 / portTICK_PERIOD_MS);
    for (;;)
    {
        printf("SenderTask2: Sending %d to queue\n", valueToSend);
        if (xQueueSend(DataQueue, &valueToSend, pdMS_TO_TICKS(1000)) == pdPASS)
        {
            printf("SenderTask2: Data sent successfully\n\n");
        }
        else
        {
            printf("SenderTask2: Failed to send data\n\n");
        }
        valueToSend += 20;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }   
}

// Task3 function
void Task3(void *pvParameters)
{
    (void)pvParameters;
    int receivedValue;
    for (;;)
    {
        // Wait indefinitely for data to arrive
        if (xQueueReceive(DataQueue, &receivedValue, portMAX_DELAY) == pdPASS)
        {
            printf("ReceiverTask: Received value %d from queue\n\n", receivedValue);
        }
    }
}

