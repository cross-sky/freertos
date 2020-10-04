#include "FreeRTOS.h"
#include "task.h"

portCHAR flag1;
portCHAR flag2;

extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

TaskHandle_t    task1_handle;
#define         TASK1_STACK_SIZE    128
StackType_t     task1Stack[TASK1_STACK_SIZE];
TCB_t           Task1TCB;

TaskHandle_t    task2_handle;
#define         TASK2_STACK_SIZE    128
StackType_t     task2Stack[TASK2_STACK_SIZE];
TCB_t           Task2TCB;

void delay(uint32_t count);
void Task1_Entry(void* p_arg);
void Task2_Entry(void* p_arg);


int main(void)
{
    prvInitialiseTaskLists();

    task1_handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry,
                                        (char*) "Task1",
                                        (uint32_t)TASK1_STACK_SIZE,
                                        (void*)NULL,
                                        (StackType_t*)task1Stack,
                                        (TCB_t*)&Task1TCB);
    vListInsertEnd(&(pxReadyTasksLists[1]),
                    &(((TCB_t*)&Task1TCB)->xStateListItem));
    
    task2_handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry,
                                        (char*) "Task2",
                                        (uint32_t)TASK2_STACK_SIZE,
                                        (void*)NULL,
                                        (StackType_t*)task2Stack,
                                        (TCB_t*)&Task2TCB);
    vListInsertEnd(&(pxReadyTasksLists[2]),
                    &(((TCB_t*)&Task2TCB)->xStateListItem));

    vTaskStartScheduler();
    for(;;)
    {

    }
}


void delay(uint32_t count)
{
    for (; count!= 0; count--)
    {
        /* code */
    }
}

void Task1_Entry(void* p_arg)
{
    for(;;)
    {
        flag1 = 1;
        delay(100);
        flag1 = 0;
        delay(100);

        taskYIELD();
    }
}

void Task2_Entry(void* p_arg)
{
    for(;;)
    {
        flag2 = 1;
        delay(100);
        flag2 = 0;
        delay(100);

        taskYIELD();
    }
}


