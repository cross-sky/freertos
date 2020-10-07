/**
  ************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��FreeRTOS�ں�ʵ����Ӧ�ÿ���ʵսָ�ϡ��鼮����
  *           �½�FreeRTOS���̡���������
  ************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 ϵ�� ������
  * 
  * ����    :www.embedfire.com  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ************************************************************************
  */
  
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"

/*
*************************************************************************
*                              ȫ�ֱ���
*************************************************************************
*/
portCHAR flag1;
portCHAR flag2;
portCHAR flag3;

extern List_t pxReadyTasksLists[ configMAX_PRIORITIES ];

/*
*************************************************************************
*                        ������ƿ� & STACK 
*************************************************************************
*/
TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE                    128
StackType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t Task1TCB;

TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE                    128
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t Task2TCB;

TaskHandle_t Task3_Handle;
#define TASK3_STACK_SIZE                    128
StackType_t Task3Stack[TASK3_STACK_SIZE];
TCB_t Task3TCB;


/*
*************************************************************************
*                               �������� 
*************************************************************************
*/
void delay (uint32_t count);
void Task1_Entry( void *p_arg );
void Task2_Entry( void *p_arg );
void Task3_Entry( void *p_arg );


/*
************************************************************************
*                                main����
************************************************************************
*/
int main(void)
{	
    /* Ӳ����ʼ�� */
	/* ��Ӳ����صĳ�ʼ������������������������û����س�ʼ������ */ 

    
    /* �������� */
    Task1_Handle = xTaskCreateStatic( (TaskFunction_t)Task1_Entry,   /* ������� */
					                  (char *)"Task1",               /* �������ƣ��ַ�����ʽ */
					                  (uint32_t)TASK1_STACK_SIZE ,   /* ����ջ��С����λΪ�� */
					                  (void *) NULL,                 /* �����β� */
                                      (UBaseType_t) 1,               /* �������ȼ�����ֵԽ�����ȼ�Խ�� */
					                  (StackType_t *)Task1Stack,     /* ����ջ��ʼ��ַ */
					                  (TCB_t *)&Task1TCB );          /* ������ƿ� */
    /* ���������ӵ������б� */                                 
    //vListInsertEnd( &( pxReadyTasksLists[1] ), &( ((TCB_t *)(&Task1TCB))->xStateListItem ) );
                                
    Task2_Handle = xTaskCreateStatic( (TaskFunction_t)Task2_Entry,   /* ������� */
					                  (char *)"Task2",               /* �������ƣ��ַ�����ʽ */
					                  (uint32_t)TASK2_STACK_SIZE ,   /* ����ջ��С����λΪ�� */
					                  (void *) NULL,                 /* �����β� */
                                      (UBaseType_t) 2,               /* �������ȼ�����ֵԽ�����ȼ�Խ�� */                                          
					                  (StackType_t *)Task2Stack,     /* ����ջ��ʼ��ַ */
					                  (TCB_t *)&Task2TCB );          /* ������ƿ� */ 
    
    //��������е�С���⣬ͬһ�����ȼ��Ĳ����ж������......10.07
    // Task3_Handle = xTaskCreateStatic( (TaskFunction_t)Task3_Entry,   /* ������� */
    //                                 (char *)"Task3",               /* �������ƣ��ַ�����ʽ */
    //                                 (uint32_t)TASK3_STACK_SIZE ,   /* ����ջ��С����λΪ�� */
    //                                 (void *) NULL,                 /* �����β� */
    //                                 (UBaseType_t) 1,               /* �������ȼ�����ֵԽ�����ȼ�Խ�� */                                          
    //                                 (StackType_t *)Task3Stack,     /* ����ջ��ʼ��ַ */
    //                                 (TCB_t *)&Task3TCB );          /* ������ƿ� */ 

    /* ���������ӵ������б� */                                 
    //vListInsertEnd( &( pxReadyTasksLists[2] ), &( ((TCB_t *)(&Task2TCB))->xStateListItem ) );
                                      
    /* ��������������ʼ��������ȣ������ɹ��򲻷��� */
    vTaskStartScheduler();                                      
    
    for(;;)
	{
		/* ϵͳ�����ɹ����ᵽ������ */
	}
}

/*
*************************************************************************
*                               ����ʵ��
*************************************************************************
*/
/* ������ʱ */
void delay (uint32_t count)
{
	for(; count!=0; count--);
}
/* ����1 */
void Task1_Entry( void *p_arg )
{
	for( ;; )
	{
		flag1 = 1;
        vTaskDelay( 2 );		
		flag1 = 0;
        vTaskDelay( 2 );       
	}
}

/* ����2 */
void Task2_Entry( void *p_arg )
{
	for( ;; )
	{
		flag2 = 1;
        vTaskDelay( 2 );		
		flag2 = 0;
        vTaskDelay( 2 );        
	}
}

/* ����3 */
void Task3_Entry( void *p_arg )
{
	for( ;; )
	{
		flag3 = 1;
        vTaskDelay( 2 );		
		flag3 = 0;
        vTaskDelay( 2 );        
	}
}

/* ��ȡ����������ڴ� */
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;
void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer, 
                                    StackType_t **ppxIdleTaskStackBuffer, 
                                    uint32_t *pulIdleTaskStackSize )
{
		*ppxIdleTaskTCBBuffer=&IdleTaskTCB;
		*ppxIdleTaskStackBuffer=IdleTaskStack; 
		*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}






















