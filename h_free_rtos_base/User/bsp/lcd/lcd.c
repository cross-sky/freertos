#include "lcd.h"
#include "stdlib.h"
//#include "usart.h"
//#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RBT6,��Ƶ72M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:QDtech2008@gmail.com 
//Skype:QDtech2008
//��������QQȺ:324828016
//��������:2013/5/13
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	
/****************************************************************************************************
//=======================================Һ���������߽���==========================================//
//DB0       ��PD14 
//DB1       ��PD15 
//DB2       ��PD0 
//DB3       ��PD1 
//DB4~DB12  ���ν�PE7~PE15
//DB13      ��PD8 
//DB14      ��PD9
//DB15      ��PD10  
//=======================================Һ���������߽���==========================================//
//LCD_CS	��PG12	//Ƭѡ�ź�
//LCD_RS	��PG0	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PD5	//д�ź�
//LCD_RD	��PD4	//���ź�
//LCD_RST	��PC5	//��λ�ź�
//LCD_LED	��PB0	//��������ź�(�ߵ�ƽ����)
//=========================================������������=========================================//
//��ʹ�ô�������ģ�鱾������������ɲ�����
//MO(MISO)	��PF8	//SPI�������
//MI(MOSI)	��PF9	//SPI��������
//PEN		��PF10	//�������ж��ź�
//TCS		��PB2	//����ICƬѡ
//CLK		��PB1	//SPI����ʱ��
**************************************************************************************************/	
	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(u16 data)
{ 
	LCD->LCD_REG=data;//д��Ҫд�ļĴ������	 	
}

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_DATA(u16 data)
{
	LCD->LCD_RAM=data;

}
//******************************************************************
//��������  LCD_DrawPoint_16Bit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д��һ��16λ����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
	LCD_WR_DATA(color); 
}

//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;	
	return ram;	 
}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD->LCD_RAM = LCD_RegValue;//д������	     		 
}		   
	 
//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//��������  LCD_DrawPoint
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	LCD_WR_DATA(POINT_COLOR); 
}

//******************************************************************
//��������  LCD_Clear
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{	
	//u32 index=0; 
	   u32 index=0,i=0;   
	LCD_SetWindows(0,0,lcddev.width,lcddev.height);	

	for(i=0;i<lcddev.width;i++)
	{

	for(index=0;index<lcddev.height;index++)
	{
		LCD->LCD_RAM=Color;		
	}
	//delay_ms(100);	
	}
} 

//******************************************************************
//��������  LCD_GPIOInit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Һ����IO��ʼ����FSMC���߳�ʼ����Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5��Һ����λ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12����������� A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 
 
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //0x01 ��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////0x03 ���ݱ���ʱ��Ϊ4��HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 
}

//******************************************************************
//��������  LCD_Reset
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}
 	 
//******************************************************************
//��������  LCD_Init
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();

	//Һ������ʼ��
	// LCD_WR_REG(0XF1);
	// LCD_WR_DATA(0x36);
	// LCD_WR_DATA(0x04);
	// LCD_WR_DATA(0x00);
	// LCD_WR_DATA(0x3C);
	// LCD_WR_DATA(0X0F);
	// LCD_WR_DATA(0x8F);
	// LCD_WR_REG(0XF2);
	// LCD_WR_DATA(0x18);
	// LCD_WR_DATA(0xA3);
	// LCD_WR_DATA(0x12);
	// LCD_WR_DATA(0x02);
	// LCD_WR_DATA(0XB2);
	// LCD_WR_DATA(0x12);
	// LCD_WR_DATA(0xFF);
	// LCD_WR_DATA(0x10);
	// LCD_WR_DATA(0x00);
	// LCD_WR_REG(0XF8);
	// LCD_WR_DATA(0x21);
	// LCD_WR_DATA(0x04);
	// LCD_WR_REG(0XF9);
	// LCD_WR_DATA(0x00);
	// LCD_WR_DATA(0x08);
	// LCD_WR_REG(0x36);
	// LCD_WR_DATA(0x08);
	// LCD_WR_REG(0xB4);
	// LCD_WR_DATA(0x00);
	// LCD_WR_REG(0xB6);
	// LCD_WR_DATA(0x02);
	// LCD_WR_DATA(0x22);
	// LCD_WR_REG(0xC1);
	// LCD_WR_DATA(0x41);
	// LCD_WR_REG(0xC5);
	// LCD_WR_DATA(0x00);
	// LCD_WR_DATA(0x18);
	// LCD_WR_REG(0xE0);
	// LCD_WR_DATA(0x0F);
	// LCD_WR_DATA(0x1F);
	// LCD_WR_DATA(0x1C);
	// LCD_WR_DATA(0x0C);
	// LCD_WR_DATA(0x0F);
	// LCD_WR_DATA(0x08);
	// LCD_WR_DATA(0x48);
	// LCD_WR_DATA(0x98);
	// LCD_WR_DATA(0x37);
	// LCD_WR_DATA(0x0A);
	// LCD_WR_DATA(0x13);
	// LCD_WR_DATA(0x04);
	// LCD_WR_DATA(0x11);
	// LCD_WR_DATA(0x0D);
	// LCD_WR_DATA(0x00);
	// LCD_WR_REG(0xE1);
	// LCD_WR_DATA(0x0F);
	// LCD_WR_DATA(0x32);
	// LCD_WR_DATA(0x2E);
	// LCD_WR_DATA(0x0B);
	// LCD_WR_DATA(0x0D);
	// LCD_WR_DATA(0x05);
	// LCD_WR_DATA(0x47);
	// LCD_WR_DATA(0x75);
	// LCD_WR_DATA(0x37);
	// LCD_WR_DATA(0x06);
	// LCD_WR_DATA(0x10);
	// LCD_WR_DATA(0x03);
	// LCD_WR_DATA(0x24);
	// LCD_WR_DATA(0x20);
	// LCD_WR_DATA(0x00);
	// LCD_WR_REG(0x3A);
	// LCD_WR_DATA(0x55);
	// LCD_WR_REG(0x11);
	// LCD_WR_REG(0x36);
	// LCD_WR_DATA(0xC8);
	// delay_ms(120);
	// LCD_WR_REG(0x29);

	//����9341 ID�Ķ�ȡ		
	LCD_WR_REG(0XD3);				   
	lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();	//����0X00
	lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//��ȡ41 

	printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID   


	//********
	LCD_WR_REG(0xB0);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x11);
	delay_ms(20);
 
	LCD_WR_REG(0xB3);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
 
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x10);//13
	LCD_WR_DATA(0x3B);//480
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x00);//NW
	LCD_WR_DATA(0x43);
 
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x10);//LCD_WriteData(0x08);
	LCD_WR_DATA(0x10);//LCD_WriteData(0x16);//CLOCK
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x08);
 
	LCD_WR_REG(0xC4);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x03);
 
	LCD_WR_REG(0xC6);
	LCD_WR_DATA(0x00);
 
	LCD_WR_REG(0xC8);//GAMMA
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x5C);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x53);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x00);
 
	LCD_WR_REG(0x35);
	LCD_WR_DATA(0x00);
 
	LCD_WR_REG(0x36);  
	LCD_WR_DATA(0x00);
 
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
 
	LCD_WR_REG(0x44);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
 
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x22);//0 GS SS SM ISC[3:0];����GS SS������ʾ����ͬʱ�޸�R36
	LCD_WR_DATA(0x3B);
 
	LCD_WR_REG(0xD0);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x07);//VCI1
	LCD_WR_DATA(0x1D);//VRH
 
	LCD_WR_REG(0xD1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x03);//VCM
	LCD_WR_DATA(0x00);//VDV
 
	LCD_WR_REG(0xD2);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x04);
 
	LCD_WR_REG(0x29);
	delay_ms(20);
 
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x00);
	delay_ms(20);
	LCD_WR_REG(0x2C);
	//*****


	    // Interface Mode Control
    // LCD_WR_REG(0xb0);
    // LCD_WR_DATA(0x00);
    // // Interface Pixel Format, 16 bits / pixel
    // LCD_WR_REG(0x3A);
    // LCD_WR_DATA(0x55); // 5D
    // // PGAMCTRL(Positive Gamma Control)
    // LCD_WR_REG(0xE0);
    // LCD_WR_DATA(0x0F);
    // LCD_WR_DATA(0x1F);
    // LCD_WR_DATA(0x1C);
    // LCD_WR_DATA(0x0C);
    // LCD_WR_DATA(0x0F);
    // LCD_WR_DATA(0x08);
    // LCD_WR_DATA(0x48);
    // LCD_WR_DATA(0x98);
    // LCD_WR_DATA(0x37);
    // LCD_WR_DATA(0x0A);
    // LCD_WR_DATA(0x13);
    // LCD_WR_DATA(0x04);
    // LCD_WR_DATA(0x11);
    // LCD_WR_DATA(0x0D);
    // LCD_WR_DATA(0x00);
    // // NGAMCTRL (Negative Gamma Correction)
    // LCD_WR_REG(0xE1);
    // LCD_WR_DATA(0x0F);
    // LCD_WR_DATA(0x32);
    // LCD_WR_DATA(0x2E);
    // LCD_WR_DATA(0x0B);
    // LCD_WR_DATA(0x0D);
    // LCD_WR_DATA(0x05);
    // LCD_WR_DATA(0x47);
    // LCD_WR_DATA(0x75);
    // LCD_WR_DATA(0x37);
    // LCD_WR_DATA(0x06);
    // LCD_WR_DATA(0x10);
    // LCD_WR_DATA(0x03);
    // LCD_WR_DATA(0x24);
    // LCD_WR_DATA(0x20);
    // LCD_WR_DATA(0x00);
    // // Digital Gamma Control 1
    // LCD_WR_REG(0xE2);
    // LCD_WR_DATA(0x0F);
    // LCD_WR_DATA(0x32);
    // LCD_WR_DATA(0x2E);
    // LCD_WR_DATA(0x0B);
    // LCD_WR_DATA(0x0D);
    // LCD_WR_DATA(0x05);
    // LCD_WR_DATA(0x47);
    // LCD_WR_DATA(0x75);
    // LCD_WR_DATA(0x37);
    // LCD_WR_DATA(0x06);
    // LCD_WR_DATA(0x10);
    // LCD_WR_DATA(0x03);
    // LCD_WR_DATA(0x24);
    // LCD_WR_DATA(0x20);
    // LCD_WR_DATA(0x00);
    // // Set rotation
    // //setRotation(_rotation);
    // // Idle mode control + Power +  Frame rate ctrl
    // //idleModeOff();
	// LCD_WR_REG(0x38);
    // // frame rate ctrl
    // LCD_WR_REG(0xB1);
    // // Frame rate(Hz) (default: 70kHz) /-/ Division Ratio (default: fosc)
    // LCD_WR_DATA(0xB0);
    // // Clock per Line (default: 17 clk cycles)
    // LCD_WR_DATA(0x11);
    // // Power Control 3 (For Normal Mode)
    // LCD_WR_REG(0xC2);
    // LCD_WR_DATA(0x55); // 44
    // // Display Inversion Control
    // LCD_WR_REG(0xB4);
    // LCD_WR_DATA(0x02); // 2 dot invercion /-/ disabled | 0x12 to enable
    // // Display Function Control
    // LCD_WR_REG(0xB6);
    // LCD_WR_DATA(0x02);
    // LCD_WR_DATA(0x22);
    // LCD_WR_DATA(0x3B);
    // // # Sleep OUT
    // LCD_WR_REG(0x11);
    // delay_ms(150);
    // // Display ON
    // LCD_WR_REG(0x29);
	//****

	LCD_SetParam();//����LCD����	 
	LCD_LED=0;//��������	 
	LCD_Clear(WHITE);
}
  		  
//******************************************************************
//��������  LCD_SetWindows
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ������ʾ����
//���������(xStar,yStar):�������Ͻ���ʼ����
//	 	   	(xEnd,yEnd):�������½ǽ�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM			
}   

//******************************************************************
//��������  LCD_SetCursor
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ���ù��λ��
//���������Xpos:������
//			Ypos:������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		
	LCD_WR_DATA((Xpos+1)>>8);
	LCD_WR_DATA((Xpos+1));
	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		
	LCD_WR_DATA((Ypos+1)>>8);
	LCD_WR_DATA((Ypos+1));
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM		
} 

//******************************************************************
//��������  LCD_SetParam
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ����LCD������������к�����ģʽ�л�
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_SetParam(void)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=480;
	lcddev.height=320;			
	LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//����
	lcddev.dir=0;//����				 	 		
	lcddev.width=320;
	lcddev.height=480;	
	LCD_WriteReg(0x36,(1<<3)|(1<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0 (1<<3)|(1<<6)|(1<<7)
#endif
}

void LCD_direction(u8 direction )
{ 
	switch(direction){
		case 3:
		    // |������������������(X)
			// |
			// |
			// (Y)
			lcddev.dir=1;//����	
			lcddev.width=480;
			lcddev.height=320;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY7==1,MX6==0,MV5==1
		break;
		case 1:
			//           (Y)
			//            |
			//            |
			// (X)________|
			
			lcddev.dir=1;//����
			lcddev.width=480;
			lcddev.height=320;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:
		// (X)
		// |
		// |
		// |________(Y)		
			lcddev.dir=0;//����					 	 		
			lcddev.width=320;
			lcddev.height=480;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 0:	
		    // (Y) ��������������|
			//            |
			//            |
			//           (X)
			lcddev.dir=0;//����						 	 		
			lcddev.width=320;
			lcddev.height=480;	
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		default:break;
	}
}	 




 

