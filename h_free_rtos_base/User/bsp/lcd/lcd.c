#include "lcd.h"
#include "stdlib.h"
//#include "usart.h"
//#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RBT6,主频72M  单片机工作电压3.3V
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtech.net
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	
/****************************************************************************************************
//=======================================液晶屏数据线接线==========================================//
//DB0       接PD14 
//DB1       接PD15 
//DB2       接PD0 
//DB3       接PD1 
//DB4~DB12  依次接PE7~PE15
//DB13      接PD8 
//DB14      接PD9
//DB15      接PD10  
//=======================================液晶屏控制线接线==========================================//
//LCD_CS	接PG12	//片选信号
//LCD_RS	接PG0	//寄存器/数据选择信号
//LCD_WR	接PD5	//写信号
//LCD_RD	接PD4	//读信号
//LCD_RST	接PC5	//复位信号
//LCD_LED	接PB0	//背光控制信号(高电平点亮)
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//MO(MISO)	接PF8	//SPI总线输出
//MI(MOSI)	接PF9	//SPI总线输入
//PEN		接PF10	//触摸屏中断信号
//TCS		接PB2	//触摸IC片选
//CLK		接PB1	//SPI总线时钟
**************************************************************************************************/	
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


//******************************************************************
//函数名：  LCD_WR_REG
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(u16 data)
{ 
	LCD->LCD_REG=data;//写入要写的寄存器序号	 	
}

//******************************************************************
//函数名：  LCD_WR_DATA
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA(u16 data)
{
	LCD->LCD_RAM=data;

}
//******************************************************************
//函数名：  LCD_DrawPoint_16Bit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
	LCD_WR_DATA(color); 
}

//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}

//******************************************************************
//函数名：  LCD_WriteReg
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	     		 
}		   
	 
//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//函数名：  LCD_DrawPoint
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	LCD_WR_DATA(POINT_COLOR); 
}

//******************************************************************
//函数名：  LCD_Clear
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
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
//函数名：  LCD_GPIOInit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    液晶屏IO初始化，FSMC总线初始化，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5复液晶屏位
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12复用推挽输出 A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 
 
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //0x01 地址建立时间（ADDSET）为1个HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////0x03 数据保存时间为4个HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 
}

//******************************************************************
//函数名：  LCD_Reset
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}
 	 
//******************************************************************
//函数名：  LCD_Init
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();

	//液晶屏初始化
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

	//尝试9341 ID的读取		
	LCD_WR_REG(0XD3);				   
	lcddev.id=LCD_RD_DATA();	//dummy read 	
	lcddev.id=LCD_RD_DATA();	//读到0X00
	lcddev.id=LCD_RD_DATA();   	//读取93								   
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();  	//读取41 

	printf(" LCD ID:%x\r\n",lcddev.id); //打印LCD ID   


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
	LCD_WR_DATA(0x22);//0 GS SS SM ISC[3:0];其中GS SS控制显示方向，同时修改R36
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

	LCD_SetParam();//设置LCD参数	 
	LCD_LED=0;//点亮背光	 
	LCD_Clear(WHITE);
}
  		  
//******************************************************************
//函数名：  LCD_SetWindows
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    设置显示窗口
//输入参数：(xStar,yStar):窗口左上角起始坐标
//	 	   	(xEnd,yEnd):窗口右下角结束坐标
//返回值：  无
//修改记录：无
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

	LCD_WriteRAM_Prepare();	//开始写入GRAM			
}   

//******************************************************************
//函数名：  LCD_SetCursor
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    设置光标位置
//输入参数：Xpos:横坐标
//			Ypos:纵坐标
//返回值：  无
//修改记录：无
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
	LCD_WriteRAM_Prepare();	//开始写入GRAM		
} 

//******************************************************************
//函数名：  LCD_SetParam
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    设置LCD参数，方便进行横竖屏模式切换
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_SetParam(void)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=480;
	lcddev.height=320;			
	LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=320;
	lcddev.height=480;	
	LCD_WriteReg(0x36,(1<<3)|(1<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0 (1<<3)|(1<<6)|(1<<7)
#endif
}

void LCD_direction(u8 direction )
{ 
	switch(direction){
		case 3:
		    // |ˉˉˉˉˉˉˉˉˉ(X)
			// |
			// |
			// (Y)
			lcddev.dir=1;//横屏	
			lcddev.width=480;
			lcddev.height=320;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY7==1,MX6==0,MV5==1
		break;
		case 1:
			//           (Y)
			//            |
			//            |
			// (X)________|
			
			lcddev.dir=1;//横屏
			lcddev.width=480;
			lcddev.height=320;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:
		// (X)
		// |
		// |
		// |________(Y)		
			lcddev.dir=0;//竖屏					 	 		
			lcddev.width=320;
			lcddev.height=480;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 0:	
		    // (Y) ˉˉˉˉˉˉˉ|
			//            |
			//            |
			//           (X)
			lcddev.dir=0;//竖屏						 	 		
			lcddev.width=320;
			lcddev.height=480;	
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		default:break;
	}
}	 




 

