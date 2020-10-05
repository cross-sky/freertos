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

uint16_t task1_delay = 200;
uint16_t task2_delay = 300;

int main(void)
{
    prvInitialiseTaskLists();

    task1_handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry,
                                        (char*) "Task1",
                                        (uint32_t)TASK1_STACK_SIZE,
                                        (void*)&task1_delay,
                                        (StackType_t*)task1Stack,
                                        (TCB_t*)&Task1TCB);
    vListInsertEnd(&(pxReadyTasksLists[1]),
                    &(((TCB_t*)&Task1TCB)->xStateListItem));
    
    task2_handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry,
                                        (char*) "Task2",
                                        (uint32_t)TASK2_STACK_SIZE,
                                        (void*)&task2_delay,
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

uint16_t task_nothing(uint16_t value1, uint16_t value2)
{
    uint16_t cc;
    cc = value1 + value2;
    return cc;
}
void Task1_Entry(void* p_arg)
{
    uint16_t max_delay = (uint16_t)(*((uint16_t*)p_arg));
    uint16_t count_delay = 100;
    for(;;)
    {
        flag1 = 1;
        delay(count_delay);
        flag1 = 0;
        delay(count_delay);

        if (count_delay++ >= max_delay)
        {
            count_delay = 100;
        }
        
        task_nothing(count_delay, count_delay+2);
        taskYIELD();
    }
}

void Task2_Entry(void* p_arg)
{
    uint16_t max_delay = (uint16_t)(*((uint16_t*)p_arg));
    uint16_t count_delay = 100;
    for(;;)
    {
        flag2 = 1;
        delay(count_delay);
        flag2 = 0;
        delay(count_delay);

        if (count_delay++ >= max_delay)
        {
            count_delay = 100;
        }
        
        task_nothing(count_delay, count_delay+2);
        taskYIELD();
    }
}


