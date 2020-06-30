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
 * 2019-08-23     corelink     the first version
 */


#include <stdio.h>
#include <string.h>
#include "proton.h"
#include "xprintf.h"

#include <fifo.h>
#include <tools.h>
#include <shell.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


int heap_free(void)
{
	size_t xPortGetTotalHeapSize( void );

	xprintf("total heap size: %d bytes\n", xPortGetTotalHeapSize());
	xprintf("free heap size: %d bytes\n", xPortGetFreeHeapSize());
	xprintf("minimum ever free heap size: %d bytes\n", xPortGetMinimumEverFreeHeapSize());

	return 0;
}
SHELL_FUNCTION_EXPORT(free, heap_free, 0)

int hello(void)
{
	xprintf("hello world\n");
	return 0;
}
SHELL_FUNCTION_EXPORT(hello, hello, 0)

int printk(uint8_t *arg1)
{
	xprintf("%s\n", arg1);
	return 0;
}

int read(uint8_t *arg1)
{
	long a;
	long v;
	char *ptr;
	
	ptr = (char *)arg1;
	xatoi(&ptr, &a);

	v = R32(a);
	xprintf("[%08X] = 0x%08X\n", a, v);
	
	return 0;
}
SHELL_FUNCTION_EXPORT(read, read, 1)

int write(uint8_t *arg1, uint8_t *arg2)
{
	long a;
	long v;
	char *ptr;
	
	ptr = (char *)arg1;
	xatoi(&ptr, &a);
	ptr = (char *)arg2;
	xatoi(&ptr, &v);

	R32(a) = (uint32_t)v;
	xprintf("[%08X] = 0x%08X\n", a, v);
	
	return 0;
}
SHELL_FUNCTION_EXPORT(write, write, 2)

int dump(uint8_t *arg1, uint8_t *arg2)
{
	int i;
	long a;
	long n;
	char *ptr;
	volatile uint32_t *p;
	
	ptr = (char *)arg1;
	xatoi(&ptr, &a);
	ptr = (char *)arg2;
	xatoi(&ptr, &n);

	p = ((volatile uint32_t *)a);

	for (i=0; i<n; i++)
	{
		xprintf("[0x%08X] = 0x%08X\n", (uint32_t)p, *p);
		p++;
	}

	return 0;
}
SHELL_FUNCTION_EXPORT(dump, dump, 2)

int set_func(uint8_t *arg1, uint8_t *arg2)
{
	long gpio;
	long func;
	char *ptr;
	
	ptr = (char *)arg1;
	xatoi(&ptr, &gpio);
	ptr = (char *)arg2;
	xatoi(&ptr, &func);

	gpio_func_set((uint32_t)gpio, (uint32_t)func);
	dump((uint8_t *)"0x1B005000", (uint8_t *)"15");

	return 0;
}

int set_mode(uint8_t *arg1, uint8_t *arg2)
{
	long gpio;
	long mode;
	char *ptr;
	
	ptr = (char *)arg1;
	xatoi(&ptr, &gpio);
	ptr = (char *)arg2;
	xatoi(&ptr, &mode);

	gpio_mode_set((uint32_t)gpio, (uint32_t)mode);
	dump((uint8_t *)"0x1B005000", (uint8_t *)"15");

	return 0;
}

int reboot(void)
{
	scu_reset_mcu();
	return 0;
}
SHELL_FUNCTION_EXPORT(reboot, reboot, 0)


/*
 * spi demo
 */
uint32_t tx_buf[8];
uint32_t buf[4];

#if (SPI_HW_CS_EN == 1)

int spi0_demo(void)
{
	uint32_t id = 0;
	char *ptr;

	gpio_direction_set(29, GPIO_MODE_OUT_PP);
	gpio_set_pin_value(29, 0);

    // xprintf("spi setup\n");
	spix_setup_master(SPI0); //sets direction for SPI master 
	spix_set_max_speed(SPI0, 2000000);
	spix_attach_chip_select(SPI0, SPI_CSN0);

	// xprintf("reset chip\n");
	spix_setup_cmd_addr(SPI0, 0xff, 8, 0, 0);	// reset chip
	spix_set_datalen(SPI0, 0);
	spix_setup_dummy(SPI0, 0, 0);
	spix_start_transaction(SPI0, SPI_CMD_WR, SPI_CSN0);
	while ((spix_get_status(SPI0) & 0xFFFF) != 1);

	// xprintf("write disable\n");
	spix_setup_cmd_addr(SPI0, 0x04, 8, 0, 0);	// write disable
	spix_set_datalen(SPI0, 0);
	spix_setup_dummy(SPI0, 0, 0);
	spix_start_transaction(SPI0, SPI_CMD_WR, SPI_CSN0);
	while ((spix_get_status(SPI0) & 0xFFFF) != 1);

	// xprintf("read flashid\n");
 	spix_setup_cmd_addr(SPI0, 0x9F, 8, 0, 0);
	spix_set_datalen(SPI0, 24);
	spix_setup_dummy(SPI0, 0, 0);
	spix_start_transaction(SPI0, SPI_CMD_RD, SPI_CSN0);
	spix_read_fifo(SPI0, &id, 24);
	while ((spix_get_status(SPI0) & 0xFFFF) != 1);

	id &= 0xffffff;
	xprintf("flashid = 0x%X\n", id);

	return 0;
}

#else

int spi0_demo(void)
{	
	uint32_t id;
	char *ptr = (char *)tx_buf;

    // xprintf("spi setup\n");
	spix_setup_master(SPI0); //sets direction for SPI master 
	spix_set_max_speed(SPI0, 100000);
	spix_attach_chip_select(SPI0, SPI_CSN0);

	// xprintf("reset chip\n");
	ptr[0] = 0xff;
	spix_write_direct(SPI0, 0, tx_buf, 1);

	// xprintf("write disable\n");
	ptr[0] = 0x04;
	spix_write_direct(SPI0, 0, tx_buf, 1);

	// xprintf("read flashid\n");
	ptr[0] = 0x9f;
	spix_write_then_read_direct(SPI0, 0, tx_buf, 1, &id, 3);

	id &= 0xffffff;
	xprintf("flashid = 0x%X\n", id);

	return 0;
}
SHELL_FUNCTION_EXPORT(spi0_demo, spi0_demo, 0)

#endif

/*
 * eflash demo
 */
int eflash_demo(void)
{
	xprintf("eflash test\n");

	eflash_erase_page(48);

	buf[0] = 0x12345678;
	eflash_program_word(48, 0, (uint32_t *)buf);
	
	buf[0] = 0;
	eflash_read_word(48, 0, buf);

	if (buf[0] == 0x12345678)
		xprintf("main page pass\n");
	else
		xprintf("main page error\n");

	return 0;
}
SHELL_FUNCTION_EXPORT(eflash_demo, eflash_demo, 0)

/*
 * uart demo
 */
int uart_demo(void)
{
	gpio_func_set(14, FUNC0_UART1_TX);
	gpio_func_set(15, FUNC0_UART1_RX);
	gpio_mode_set(14, GPIO_MODE_OUT_PP);
	gpio_mode_set(15, GPIO_MODE_IN_FLOATING);

	uartx_set_conf(UART1, 115200, UART_8BITS, UART_PARITY_NONE, UART_STOP_1);
	uartx_send(UART1, "uart1\n", 6);
	uartx_wait_tx_done(UART1);

	gpio_func_set(22, FUNC0_UART2_TX);
	gpio_func_set(23, FUNC0_UART2_RX);
	gpio_mode_set(22, GPIO_MODE_OUT_PP);
	gpio_mode_set(23, GPIO_MODE_IN_FLOATING);

	uartx_set_conf(UART2, 115200, UART_8BITS, UART_PARITY_NONE, UART_STOP_1);
	uartx_send(UART2, "uart2\n", 6);
	uartx_wait_tx_done(UART2);

	return 0;
}
SHELL_FUNCTION_EXPORT(uart_demo, uart_demo, 0)

/*
 * pwm demo
 */
int pwm_demo(void)
{
	gpio_func_set(7, FUNC6_PWM3);
	gpio_mode_set(7, GPIO_MODE_OUT_PP);

	pwm_init();
	pwm_disable_channel(3);

	pwm_set_channel(3, 4000, 2000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	pwm_set_channel(3, 6000, 3000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	pwm_set_channel(3, 8000, 4000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	pwm_set_channel(3, 10000, 5000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	pwm_set_channel(3, 12000, 6000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	pwm_set_channel(3, 14000, 7000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	pwm_set_channel(3, 16000, 8000, 1);
	pwm_enable_channel(3);
	vTaskDelay( 500 );
	pwm_disable_channel(3);
	vTaskDelay( 100 );

	return 0;
}
SHELL_FUNCTION_EXPORT(pwm_demo, pwm_demo, 0)

/*
 * adc demo
 */
#define SAMPLE_COUNT	(16)
uint16_t adc_buf[SAMPLE_COUNT];
adc_conf_struct_t adc_conf;

int adc_select1(void)
{
	gpio_func_set(28, FUNC1_ADC_IN1);
	gpio_mode_set(28, GPIO_MODE_AN);

	adc_clock_select(ADCCLK_FREQ_2M);
	adc_deinit();
	adc_init();

	adc_conf_struct_init(&adc_conf);
	adc_conf.conf.power_down = ADC_POWER_UP;
	adc_conf.conf.in_channel = ADC_IN1;
    adc_conf.conf.refp = ADC_VRP_VCC;
    adc_conf.conf.refm = ADC_VRM_AVSS;
    adc_conf.conf.clock_div = ADC_DIV32;
    adc_conf.conf.sequential = ADC_SEQUENTIAL;
    adc_conf.conf.one_shot = 0;
    adc_conf.conf.scan = 0;
    adc_conf.conf.select = ADC_SELECT;
	adc_conf.round = 1;
	adc_set_conf(&adc_conf);

	adc_start();
	while (adc_read_status() & ADC_EPMTY);
	adc_buf[0] = adc_read_result();

	adc_stop();

	printf("adc value = %d\n", adc_buf[0]&0x3ff);
	//printf("temperature = %dC\n", ((adc_buf[0] * 3300 / 1024) / 10));

	return 0;
}

int adc_select2(void)
{
	gpio_func_set(27, FUNC1_ADC_IN2);
	gpio_mode_set(27, GPIO_MODE_AN);

	adc_clock_select(ADCCLK_FREQ_2M);
	adc_deinit();
	adc_init();

	adc_conf_struct_init(&adc_conf);
	adc_conf.conf.power_down = ADC_POWER_UP;
	adc_conf.conf.in_channel = ADC_IN2;
    adc_conf.conf.refp = ADC_VRP_VCC;
    adc_conf.conf.refm = ADC_VRM_AVSS;
    adc_conf.conf.clock_div = ADC_DIV32;
    adc_conf.conf.sequential = ADC_SEQUENTIAL;
    adc_conf.conf.one_shot = 0;
    adc_conf.conf.scan = 0;
    adc_conf.conf.select = ADC_SELECT;
	adc_conf.round = 1;
	adc_set_conf(&adc_conf);

	adc_start();
	while (adc_read_status() & ADC_EPMTY);
	adc_buf[0] = adc_read_result();

	adc_stop();

	printf("adc value = %d\n", adc_buf[0]&0x3ff);
	//printf("temperature = %dC\n", ((adc_buf[0] * 3300 / 1024) / 10));

	return 0;
}

volatile int adc_done;
volatile int adi;
int adc_scan(void)
{
	adi = 0;
	adc_done = 0;

	gpio_func_set(28, FUNC1_ADC_IN1);
	gpio_func_set(27, FUNC1_ADC_IN2);

	gpio_mode_set(28, GPIO_MODE_AN);
	gpio_mode_set(27, GPIO_MODE_AN);

	adc_deinit();
	adc_init();

	adc_conf_struct_init(&adc_conf);
	adc_conf.conf.power_down = ADC_POWER_UP;
	adc_conf.conf.in_channel = ADC_IN2 | ADC_IN1;
    adc_conf.conf.refp = ADC_VRP_VCC;
    adc_conf.conf.refm = ADC_VRM_AVSS;
    adc_conf.conf.clock_div = ADC_DIV32;
    adc_conf.conf.sequential = ADC_SEQUENTIAL;
    adc_conf.conf.one_shot = 0;
    adc_conf.conf.scan = ADC_SCAN;
    adc_conf.conf.select = 0;
	adc_conf.round = 8;
	adc_set_conf(&adc_conf);

	adc_set_int_level(1);
	int_enable();
	adc_int_enable();

	adc_start();

	while (adc_done == 0);

	/*
	while (ADC_GET_FIFO_LEVEL() < 4);
	adc_buf[0] = adc_read_result();
	adc_buf[1] = adc_read_result();
	adc_buf[2] = adc_read_result();
	adc_buf[3] = adc_read_result();
	*/
	adc_stop();

	int i;
	for (i=0; i<SAMPLE_COUNT; i++)
	{
		printf("adc[%d] = %d, %d\n", i, (adc_buf[i]>>10)-1, adc_buf[i]&0x3ff);
	}

	printf("\n");

	return 0;
}

int adc_demo(void)
{
	printf("adc select mode: ch1\n");
	adc_select1();
	vTaskDelay( 5 );

	printf("adc select mode: ch2\n");
	adc_select2();
	vTaskDelay( 5 );

	printf("adc scan mode: ch1 & ch2\n");
	adc_scan();

	return 0;
}
SHELL_FUNCTION_EXPORT(adc_demo, adc_demo, 0)


void ISR_ADC(void)
{
	volatile uint32_t status;

    status = ADC->fifo_state;
    if (status & ADC_LVL1_INT)
    {
        // todo: read n ADC10
		if (adi < SAMPLE_COUNT)
		{
			adc_buf[adi++] = ADC->dout;
		}
    }

	if (adi >= SAMPLE_COUNT)
	{
		adc_done = 1;
	}

	ADC_INT_RESET();
}
DEFINE_IRQ_HANDLER(ADCH_IRQ, ISR_ADC)

/*
 * i2c demo
 */
unsigned char tx[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
unsigned char rx[18] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 0x00, 0x00};

/* write page: 64byte */
int eep_write(unsigned char addr, unsigned short oft, unsigned char *buf, int size)
{
	int i;
	i2c_msg_t msg[1];
	uint8_t _buf[72];

	//oft = oft - (oft & 0x3f);
	//size = ((size > 64) ? 64 : size);

	msg[0].addr = addr;
	msg[0].flag = I2C_FLAG_WR | I2C_FLAG_IGNORE_NACK;
	//msg[0].len = 2;
	msg[0].len = 1+size;
	msg[0].buf = _buf;
	//_buf[0] = (uint8_t)((oft & 0xFF00) >> 8);
	//_buf[1] = (uint8_t)((oft & 0xFF));
	_buf[0] = (uint8_t)(oft & 0xFF);
	for (i=0; i<size; i++)
	{
		//_buf[2+i] = buf[i];
		_buf[1+i] = buf[i];
	}
	i2cx_xfer(I2C0, msg, 1);

  	// acknowledge polling
	do
	{
    	i2cx_send_data(I2C0, addr << 1);
    	i2cx_send_command(I2C0, I2C_START_WRITE);
	} 
	while (!i2cx_get_ack(I2C0));

	i2cx_send_command(I2C0, I2C_STOP);	// do a stop bit, finish acknowledge polling transfer
	while (i2cx_busy(I2C0));

	return size;
}

/* read page: 64byte */
int eep_read(unsigned char addr, unsigned short oft, unsigned char *buf, int size)
{
	i2c_msg_t msg[2];
	uint8_t _buf[2];

	//size = ((size > 64) ? 64 : size);
	
	msg[0].addr = addr;
	msg[0].flag = I2C_FLAG_WR | I2C_FLAG_IGNORE_NACK | I2C_FLAG_NO_STOP;
	//msg[0].len = 2;
	msg[0].len = 1;
	msg[0].buf = _buf;
	//_buf[0] = (uint8_t)((oft & 0xFF00) >> 8);
	//_buf[1] = (uint8_t)((oft & 0xFF));
	_buf[0] = (uint8_t)(oft & 0xFF);

	msg[1].addr = addr;
	msg[1].flag = I2C_FLAG_RD | I2C_FLAG_IGNORE_NACK;
	msg[1].len = size;
	msg[1].buf = buf;

	i2cx_xfer(I2C0, msg, 2);

	return size;
}

int eep_demo(void) 
{
	/* at24c256 */
	i2cx_deinit(I2C0);
	i2cx_init(I2C0);
	i2cx_setup_adapter(I2C0, 100000);

	eep_write(0x54, 0, tx, 16);
	vTaskDelay( 10 );
	eep_read(0x54,  0, rx, 16);

	console_send("rx:", 3);
	console_send((char *)rx, 16);
	console_send("\n", 1);

	if (memcmp(tx, rx, 16) == 0)
	{
		xprintf("eeprom wr-rd test pass!\n");
	}
	else
	{
		printf("eeprom wr-rd test failed!\n");
	}

	return 0;
}
SHELL_FUNCTION_EXPORT(eep_demo, eep_demo, 0)

int set_freq(uint8_t *arg1)
{
	long freq;
	char *ptr;
	uint32_t f;
	
	ptr = (char *)arg1;
	xatoi(&ptr, &freq);

	taskENTER_CRITICAL();

	if (freq == 32000000)
	{
		f = 0x0;
		xprintf("cpu clock: 32MHz\n");
	}
	else if (freq == 16000000)
	{
		f = 0x1;
		xprintf("cpu clock: 16MHz\n");
	}
	else if (freq == 8000000)
	{
		f = 0x2;
		xprintf("cpu clock: 8MHz\n");
	}
	else
	{
		f = 0x3;
		xprintf("cpu clock: 4MHz\n");
	}
	
	system_clock_set(f);
	console_init(115200);

	taskEXIT_CRITICAL();
	
	return 0;
}
SHELL_FUNCTION_EXPORT(set_freq, set_freq, 1)


/*
 * shell get data
 */
void uart0_irq_handler(void)
{
	UART0_INT_RESET();
	
	while (UART0->rx_elm)
	{
		shell_getc(UART0->rbr);
	}
}
DEFINE_IRQ_HANDLER(UART0_IRQ, uart0_irq_handler)
