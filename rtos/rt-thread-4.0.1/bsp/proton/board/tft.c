/* Copyright (c) 2019-2020, corelink inc., www.corelink.vip
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-15     corelink     the first version
 */


#include <string.h>
#include "rtthread.h"
#include "proton.h"


#define bl		(25)
#define scl		(16)
#define sda		(19)
#define rs		(23)
#define cs		(24)
#define reset	(22)


#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  	0xFFE0
#define GRAY0   	0xEF7D
#define GRAY1   	0x8410
#define GRAY2   	0x4208


const uint8_t Zk_ASCII8X16[];

void Display_ASCII8X16(uint16_t x0, uint16_t y0, uint8_t *s, uint16_t color);


static void time_delay(uint32_t delay)
{
	volatile uint32_t d = delay / 8;

	if (d == 0) d = 1;
	while (--d);
}

extern gpio_dev_t* const PORT[2];

void SPI_WriteData(uint8_t Data)
{
	unsigned char i=0;

	for(i=8; i>0; i--)
	{
		if(Data&0x80)	
			PORT[sda >> 5]->padout |= ((uint32_t)0x1 << (sda & (uint32_t)0x1f));
		else
			PORT[sda >> 5]->padout &= ~((uint32_t)0x1 << (sda & (uint32_t)0x1f));
		PORT[scl >> 5]->padout &= ~((uint32_t)0x1 << (scl & (uint32_t)0x1f));
		PORT[scl >> 5]->padout |= ((uint32_t)0x1 << (scl & (uint32_t)0x1f));
		Data<<=1;
	}
}

void SPI_WriteData16(uint16_t Data)
{
	unsigned char i=0;

	for(i=8; i>0; i--)
	{
		if(Data&0x8000)
			PORT[sda >> 5]->padout |= ((uint32_t)0x1 << (sda & (uint32_t)0x1f));
		else
			PORT[sda >> 5]->padout &= ~((uint32_t)0x1 << (sda & (uint32_t)0x1f));
		PORT[scl >> 5]->padout &= ~((uint32_t)0x1 << (scl & (uint32_t)0x1f));
		PORT[scl >> 5]->padout |= ((uint32_t)0x1 << (scl & (uint32_t)0x1f));
		Data<<=1;
	}

	for(i=8; i>0; i--)
	{
		if(Data&0x8000)
			PORT[sda >> 5]->padout |= ((uint32_t)0x1 << (sda & (uint32_t)0x1f));
		else
			PORT[sda >> 5]->padout &= ~((uint32_t)0x1 << (sda & (uint32_t)0x1f));
		PORT[scl >> 5]->padout &= ~((uint32_t)0x1 << (scl & (uint32_t)0x1f));
		PORT[scl >> 5]->padout |= ((uint32_t)0x1 << (scl & (uint32_t)0x1f));
		Data<<=1;
	}
}

void Lcd_WriteIndex(uint8_t Data)
{
	PORT[cs >> 5]->padout &= ~((uint32_t)0x1 << (cs & (uint32_t)0x1f));
	PORT[rs >> 5]->padout &= ~((uint32_t)0x1 << (rs & (uint32_t)0x1f));
	SPI_WriteData(Data);
	PORT[cs >> 5]->padout |= ((uint32_t)0x1 << (cs & (uint32_t)0x1f));
}

void Lcd_WriteData(uint8_t Data)
{	
	PORT[cs >> 5]->padout &= ~((uint32_t)0x1 << (cs & (uint32_t)0x1f));
	PORT[rs >> 5]->padout |= ((uint32_t)0x1 << (rs & (uint32_t)0x1f));
	SPI_WriteData(Data);
	PORT[cs >> 5]->padout |= ((uint32_t)0x1 << (cs & (uint32_t)0x1f));
}

void LCD_WriteData_16Bit(uint16_t Data)
{
	PORT[cs >> 5]->padout &= ~((uint32_t)0x1 << (cs & (uint32_t)0x1f));
	PORT[rs >> 5]->padout |= ((uint32_t)0x1 << (rs & (uint32_t)0x1f));
	SPI_WriteData16(Data);
	PORT[cs >> 5]->padout |= ((uint32_t)0x1 << (cs & (uint32_t)0x1f));
}

void Reset(void)
{
	gpio_set_pin_value(reset, 0);
	time_delay(system_clock_get() / 10);
    gpio_set_pin_value(reset, 1);
    time_delay(system_clock_get() / 10);
}

void tft_hw_if_init(void)
{
	gpio_func_set(bl, FUNC2_GPIO);
	gpio_mode_set(bl, GPIO_MODE_OUT_PP);
	gpio_func_set(scl, FUNC2_GPIO);
	gpio_mode_set(scl, GPIO_MODE_OUT_PP);
	gpio_func_set(sda, FUNC2_GPIO);
	gpio_mode_set(sda, GPIO_MODE_OUT_PP);
	gpio_func_set(rs, FUNC2_GPIO);
	gpio_mode_set(rs, GPIO_MODE_OUT_PP);
	gpio_func_set(cs, FUNC2_GPIO);
	gpio_mode_set(cs, GPIO_MODE_OUT_PP);
	gpio_func_set(reset, FUNC2_GPIO);
	gpio_mode_set(reset, GPIO_MODE_OUT_PP);
}

void lcd_initial(void)
{
	tft_hw_if_init();

	Reset();//Reset before LCD Init.
		
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	time_delay(system_clock_get() / 8);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
	Lcd_WriteData(0xC0); 
#ifdef USE_LANDSCAPE
	Lcd_WriteData(0xA8); //����C8 ����08 A8
#else
	Lcd_WriteData(0xC8); //����C8 ����08 A8
#endif		
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+3);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	Lcd_WriteIndex(0x29);//Display on
}

void Lcd_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{	
#ifdef USE_LANDSCAPE//ʹ�ú���ģʽ
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_end+1);

#else//����ģʽ	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_end+1);	
#endif
	Lcd_WriteIndex(0x2c);
}

void PutPixel(uint16_t x_start, uint16_t y_start, uint16_t color)
{
	Lcd_SetRegion(x_start,y_start,x_start+1,y_start+1);
	LCD_WriteData_16Bit(color);
}

void Display_Clear(uint16_t color)
{
	volatile uint16_t i = 128*160+1;
	Lcd_SetRegion(0,0,128-1,160-1);
	while (--i)
	{
		PORT[cs >> 5]->padout &= ~((uint32_t)0x1 << (cs & (uint32_t)0x1f));
		PORT[rs >> 5]->padout |= ((uint32_t)0x1 << (rs & (uint32_t)0x1f));
		SPI_WriteData16(color);
		PORT[cs >> 5]->padout |= ((uint32_t)0x1 << (cs & (uint32_t)0x1f));
	}
}

void Display_Panel1(void)
{ 
    Display_ASCII8X16(0, 10, (uint8_t *)"WELCOME", RED);
    Display_ASCII8X16(0, 26, (uint8_t *)"ELEXCON 2019", RED);
    Display_ASCII8X16(0, 42, (uint8_t *)"EMBEDDED EXPO", RED);

    Display_ASCII8X16(0, 58+10, (uint8_t *)"www.corelink.vip", GREEN);
    Display_ASCII8X16(0, 74+10, (uint8_t *)"CL-Q0 32-bit MCU", GREEN);
    Display_ASCII8X16(0, 90+10, (uint8_t *)"RISC-V", GREEN);

    Display_ASCII8X16(0, 106+20, (uint8_t *)"RT-Thread Inside", BLUE);
}

void Display_Panel2(void)
{
    Display_ASCII8X16(0, 10, (uint8_t *)"WELCOME", RED);
    Display_ASCII8X16(0, 26, (uint8_t *)"ELEXCON 2019", RED);
    Display_ASCII8X16(0, 42, (uint8_t *)"EMBEDDED EXPO", RED);

    Display_ASCII8X16(0, 58+10, (uint8_t *)"www.corelink.vip", GREEN);
    Display_ASCII8X16(0, 74+10, (uint8_t *)"CL-Q0 32-bit MCU", GREEN);
    Display_ASCII8X16(0, 90+10, (uint8_t *)"RISC-V", GREEN);

    Display_ASCII8X16(0, 106+20, (uint8_t *)"RT-Thread Inside", WHITE);
}

void Display_ASCII8X16(uint16_t x0, uint16_t y0, uint8_t *s, uint16_t color)
{
	int i,j,k,x,y,xx;
	unsigned char qm;
	long int ulOffset;
	char  ywbuf[32];

	for(i = 0; i<strlen((char*)s);i++)
	{
		if(((unsigned char)(*(s+i))) >= 161)
		{
			return;
		}
		else
		{
			qm = *(s+i);

			ulOffset = (long int)(qm) * 16;
			
			for (j = 0; j < 16; j ++)
			{
				ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
			}
             
			for (y = 0;y < 16;y++)
			{
				for (x=0;x<8;x++)
				{
					k = x % 8;
                	
				  	if (ywbuf[y]&(0x80 >> k))
				   	{
				  		xx=x0+x+i*8;
				    	PutPixel(xx,y+y0,color);
				  	}
			   	}
			}
		}
	}     	
}

void tft_task(void *pvParameters)
{
	lcd_initial();

	gpio_set_pin_value(bl, 1);

	while(1)
	{
		Display_Clear(WHITE);
		Display_Panel1();
		rt_thread_delay(10 * RT_TICK_PER_SECOND);

		Display_Clear(BLACK);
		Display_Panel2();
		rt_thread_delay(10 * RT_TICK_PER_SECOND);
    }
}

const uint8_t Zk_ASCII8X16[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x18,0x08,0xc8,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x7f,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xe0,
	0x60,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x10,
	0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x0c,0x0c,0x0c,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x6c,0x38,0x10,0x00,0x7c,0xc6,0x70,0x1c,
	0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x6c,0x00,0x78,
	0x0c,0x7c,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x7c,0xc6,0x70,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x6c,0x96,0x7c,0xd0,0xd6,0x6c,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0x06,0x06,0xc6,
	0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x6c,0xc6,0xc6,0xfe,
	0xc6,0xc6,0x6c,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x10,0x38,0x28,0x6c,0x44,0xc6,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7c,0xc6,0x06,0xfe,0xc6,0x7c,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x66,0x66,0x66,0xf6,0x06,
	0x06,0x66,0x3c,0x00,0x00,0x00,0x00,0x6c,0x38,0xf8,0x0c,0x7e,0xc6,
	0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x1b,0x18,
	0x18,0x18,0x18,0x18,0x18,0x18,0xd8,0x70,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xfe,0x86,0x0c,0x18,0x0c,0x06,0x06,0xc6,0x7c,0x00,0x00,
	0x00,0x00,0x76,0xdc,0x00,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0x76,0x00,
	0x00,0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x00,0x7c,0xc6,0x06,0x06,
	0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x00,0xdc,
	0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x76,0x99,0x9f,0x98,0x99,0x6f,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x06,0x06,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,
	0x66,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3c,
	0x3c,0x3c,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
	0xc6,0xc6,0xc6,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x6c,0x6c,0xfe,0x6c,0x6c,0x6c,0xfe,0x6c,0x6c,
	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7c,0xc6,0xc2,0xc0,0x7c,0x06,
	0x86,0xc6,0x7c,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc3,
	0xc6,0x0c,0x18,0x30,0x63,0xc3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x38,0x6c,0x6c,0x38,0x76,0xdc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,
	0x00,0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x30,0x60,0x60,0x60,0x60,0x60,
	0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x0c,0x06,0x06,
	0x06,0x06,0x06,0x0c,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x6c,0x38,0xfe,0x38,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x18,0x18,0x18,0x7e,0x18,0x18,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,
	0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x02,0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7c,0xc6,0xce,0xde,0xf6,0xe6,0xc6,0xc6,0x7c,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,
	0x18,0x18,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0x06,
	0x0c,0x18,0x30,0x60,0xc6,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x7c,0xc6,0x06,0x06,0x3c,0x06,0x06,0xc6,0x7c,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x0c,0x1c,0x3c,0x6c,0xcc,0xfe,0x0c,0x0c,0x1e,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xc0,0xc0,0xc0,0xfc,0x06,0x06,
	0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x60,0xc0,0xc0,
	0xfc,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,
	0xc6,0x06,0x0c,0x18,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7c,0xc6,0xc6,0xc6,0x7c,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7e,0x06,0x06,0x0c,
	0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,
	0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,
	0x18,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x06,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x06,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x0c,
	0x18,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,
	0x0c,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x7c,0xc6,0xc6,0xde,0xde,0xde,0xdc,0xc0,0x7c,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x10,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x66,0x66,
	0x66,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0xc2,0xc0,
	0xc0,0xc0,0xc2,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,
	0x6c,0x66,0x66,0x66,0x66,0x66,0x6c,0xf8,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xfe,0x66,0x62,0x68,0x78,0x68,0x62,0x66,0xfe,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xfe,0x66,0x62,0x68,0x78,0x68,0x60,0x60,
	0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0xc2,0xc0,0xc0,
	0xde,0xc6,0x66,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,
	0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1e,0x0c,0x0c,0x0c,0x0c,0x0c,0xcc,0xcc,0x78,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe6,0x66,0x6c,0x6c,0x78,0x6c,
	0x6c,0x66,0xe6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x60,0x60,
	0x60,0x60,0x60,0x62,0x66,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xc6,0xee,0xfe,0xd6,0xc6,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0xc6,0xe6,0xf6,0xfe,0xde,0xce,0xc6,0xc6,0xc6,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x6c,0xc6,0xc6,0xc6,0xc6,0xc6,
	0x6c,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x66,0x66,0x66,
	0x7c,0x60,0x60,0x60,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,
	0xc6,0xc6,0xc6,0xd6,0xde,0x7c,0x0c,0x0e,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x6c,0x66,0x66,0xe6,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,0x60,0x38,0x0c,0xc6,0xc6,
	0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x5a,0x18,0x18,0x18,
	0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,
	0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x6c,0x38,0x10,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0x6c,0x38,0x6c,
	0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,
	0x6c,0x38,0x38,0x38,0x38,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xfe,0xc6,0x8c,0x18,0x30,0x60,0xc2,0xc6,0xfe,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3c,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3c,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0x70,0x38,0x1c,0x0e,
	0x06,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x0c,0x0c,0x0c,
	0x0c,0x0c,0x0c,0x0c,0x3c,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x6c,
	0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,
	0x00,0x00,0x00,0x00,0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x0c,
	0x7c,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x60,
	0x60,0x78,0x6c,0x66,0x66,0x66,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7c,0xc6,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1c,0x0c,0x0c,0x3c,0x6c,0xcc,0xcc,0xcc,0x76,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xfe,
	0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x36,0x32,
	0x30,0x7c,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x76,0xcc,0xcc,0xcc,0x7c,0x0c,0xcc,0x78,0x00,0x00,
	0x00,0x00,0x00,0xe0,0x60,0x60,0x6c,0x76,0x66,0x66,0x66,0xe6,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,0x18,
	0x18,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x0e,
	0x06,0x06,0x06,0x06,0x66,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0xe0,
	0x60,0x60,0x66,0x6c,0x78,0x6c,0x66,0xe6,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0xfe,0xd6,0xd6,0xd6,
	0xd6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x66,
	0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xdc,0x66,0x66,0x66,0x7c,0x60,0x60,0xf0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xcc,0xcc,0xcc,0x7c,0x0c,
	0x0c,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x76,0x66,
	0x60,0x60,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x7c,0xc6,0x70,0x1c,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x10,0x30,0x30,0xfc,0x30,0x30,0x30,0x36,0x1c,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0x6c,
	0x38,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,
	0xc6,0xc6,0xd6,0xfe,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xc6,0x6c,0x38,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0xc6,0x7e,0x06,0x0c,0x78,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xcc,0x18,0x30,0x66,
	0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x18,0x18,0x18,0x70,
	0x18,0x18,0x18,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,
	0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x70,0x18,0x18,0x18,0x0e,0x18,0x18,0x18,0x70,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};



