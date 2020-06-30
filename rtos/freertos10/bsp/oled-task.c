#include "stdlib.h"

#include "oled.h"
#include "oledfont.h" 
#include "bmp.h" 	

#include <drv_uart.h>
#include <drv_i2c.h>
#include <utils.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   


#define I2Cx	I2C0


#if 0
void Write_IIC_Command(unsigned char cmd)
{
	i2cx_send_data(I2Cx, 0x78); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn
	i2cx_send_command(I2Cx, I2C_START_WRITE); //do a start bit and send data
	i2cx_get_ack(I2Cx);

	i2cx_send_data(I2Cx, 0x00);            //write i-th byte into fifo
    i2cx_send_command(I2Cx, I2C_WRITE);
	i2cx_get_ack(I2Cx);  

	i2cx_send_data(I2Cx, cmd);            //write i-th byte into fifo
    i2cx_send_command(I2Cx, I2C_WRITE);
	i2cx_get_ack(I2Cx); 

	i2cx_send_command(I2Cx, I2C_STOP);
	while (i2cx_busy(I2Cx));
}


void Write_IIC_Data(unsigned char dat)
{
	i2cx_send_data(I2Cx, 0x78); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn
	i2cx_send_command(I2Cx, I2C_START_WRITE); //do a start bit and send data
	i2cx_get_ack(I2Cx);

	i2cx_send_data(I2Cx, 0x40);            //write i-th byte into fifo
    i2cx_send_command(I2Cx, I2C_WRITE);
	i2cx_get_ack(I2Cx);  

	i2cx_send_data(I2Cx, dat);            //write i-th byte into fifo
    i2cx_send_command(I2Cx, I2C_WRITE);
	i2cx_get_ack(I2Cx); 

	i2cx_send_command(I2Cx, I2C_STOP); 
	while (i2cx_busy(I2Cx));
}
#else

void Write_IIC_Command(unsigned char cmd)
{
	i2c_msg_t msg;
	uint8_t buf[4];

	msg.addr = 0x3c;
	msg.flag = I2C_FLAG_WR | I2C_FLAG_IGNORE_NACK;
	msg.len  = 2;
	msg.buf  = buf;
	buf[0] = 0x00;
	buf[1] = cmd;

	i2cx_xfer(I2Cx, &msg, 1);
}

void Write_IIC_Data(unsigned char dat)
{
	i2c_msg_t msg;
	uint8_t buf[4];

	msg.addr = 0x3c;
	msg.flag = I2C_FLAG_WR | I2C_FLAG_IGNORE_NACK;
	msg.len  = 2;
	msg.buf  = buf;
	buf[0] = 0x40;
	buf[1] = dat;

	i2cx_xfer(I2Cx, &msg, 1);
}
#endif


void OLED_WR_Byte(unsigned char dat, unsigned char cmd)
{
	if (cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0; m<8; m++)
	{
		OLED_WR_Byte(0xb0+m, 0);		//page0-page1
		OLED_WR_Byte(0x00, 0);		//low column start address
		OLED_WR_Byte(0x10, 0);		//high column start address
		for(n=0; n<128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}


void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}  
 	  

void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC閿熸枻鎷烽敓鏂ゆ嫹
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}


void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC閿熸枻鎷烽敓鏂ゆ嫹
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
		   			 

void OLED_Clear(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);
		OLED_WR_Byte (0x00,OLED_CMD);
		OLED_WR_Byte (0x10,OLED_CMD);
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}
}

void OLED_On(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);
		OLED_WR_Byte (0x00,OLED_CMD);
		OLED_WR_Byte (0x10,OLED_CMD);
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	}
}


void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char Char_Size)
{      	
	unsigned char c=0,i=0;	

	c=chr-' ';
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else
	{
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
	}
}


unsigned int oled_pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}	
			  

void OLED_ShowNum(unsigned char x,unsigned char y,unsigned char num,unsigned char len,unsigned char size2)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else
				enshow=1;
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 


void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120)
		{
			x=0;
			y+=2;
		}
		j++;
	}
}

void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no)
{      			    
	unsigned char t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
    	OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
    	adder+=1;
	}
	OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
	{
    	OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
    	adder+=1;
	}
}

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;
  
	if(y1%8==0)
		y=y1/8;
	else
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//SD1306
void OLED_Init(void)
{ 	
	Delay_50ms(16);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

void delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245*2;m>0;m--);
}


#include <proton.h>
void oled_task (void *pvParameters) 
{	
	i2cx_deinit(I2Cx);
	i2cx_init(I2Cx);

	i2cx_setup_adapter(I2Cx, 100000);

	OLED_Init();

	while(1) 
	{	
		OLED_Clear();

		OLED_ShowString(32, 1, (unsigned char *)"CORELINK", 16);
		OLED_ShowString(0,  3, (unsigned char *)"www.corelink.vip", 16);
		OLED_ShowString(0,  5, (unsigned char *)"www.FreeRTOS.org", 16);

		vTaskDelay(2000);

		OLED_DrawBMP(0, 0, 128, 8, (unsigned char *)BMP1);

		vTaskDelay(2000);
	}	  
}
