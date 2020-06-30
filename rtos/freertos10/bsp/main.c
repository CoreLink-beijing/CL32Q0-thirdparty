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


#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "proton.h"
#include "xprintf.h"
#include "shell.h"


void prvSetupHardware(void)
{
	system_clock_set(SYSCLK_FREQ_16M);

	//R32_BITS_SET(0x1b00500c, 0x0f000000, 0x01000000);
	//gpio_func_set(30, FUNC2_GPIO);
	//gpio_mode_set(30, GPIO_MODE_OUT_PP);

	gpio_func_set(30, FUNC1_TOP_TP);
	gpio_mode_set(30, GPIO_MODE_AN);

	console_init(115200);

	xdev_out(console_send_char);
	xdev_in(console_get_char);

	printf("************************\n");
	printf("    www.corelink.vip\n");
	printf("    www.freertos.org\n");
	printf("    sdk: v%d.%d.%d\n", CL_VERSION, CL_SUBVERSION, CL_REVISION);
	printf("************************\n\n");
}

void led_task(void *pvParameters) 
{
	#define LED1 	(10)

	gpio_func_set(LED1, FUNC2_GPIO);
	gpio_mode_set(LED1, GPIO_MODE_OUT_PP);

	while (1)
	{
		gpio_set_pin_value(LED1, 1);
		vTaskDelay( 3 * configTICK_RATE_HZ );

		gpio_set_pin_value(LED1, 0);
		vTaskDelay( 3 * configTICK_RATE_HZ );
	}

	vTaskDelete(NULL);
}

void pwm_task(void *pvParameters) 
{
	int i;

	gpio_func_set(4, FUNC6_PWM0);
	gpio_mode_set(4, GPIO_MODE_OUT_PP);
	gpio_func_set(7, FUNC6_PWM3);
	gpio_mode_set(7, GPIO_MODE_OUT_PP);

	pwm_init();

	pwm_disable_channel(0);
	pwm_disable_channel(3);
	pwm_set_channel(0, 32000000, 1000000 * 1, 0);
	pwm_set_channel(3, 32000000, 1000000 * 1, 1);
	pwm_enable_channel(0);
	pwm_enable_channel(3);
	
	while (1) 
	{
		for (i=1; i<16; i++)
		{
			vTaskDelay( 5 * configTICK_RATE_HZ );
			
			pwm_set_channel(0, 32000000, 2000000 * i, 0);
			pwm_set_channel(3, 32000000, 2000000 * i, 1);
		}
	}

	vTaskDelete(NULL);
}

void rtc_task(void *pvParameters)
{
	int year, month, day, hour, min, sec;

	gpio_func_set(35, FUNC0_XTAL_IN);
	gpio_func_set(36, FUNC0_XTAL_OUT);
	gpio_mode_set(35, GPIO_MODE_AN);
	gpio_mode_set(36, GPIO_MODE_AN);

	ext_clock_enable(EXT_CLOCK_SOURCE_XTAL, 1);
	vTaskDelay( 2 * configTICK_RATE_HZ );
	lf_clock_select(LF_CLOCK_SOURCE_EXT);

	rtc_deinit();
	rtc_init();
	rtc_set_time(2019, 9, 26, 10, 10, 10);
	rtc_enable();

	while (1)
	{
		rtc_get_time((uint32_t *)&year, (uint32_t *)&month, (uint32_t *)&day, (uint32_t *)&hour, (uint32_t *)&min, (uint32_t *)&sec);
		printf("time: %04d-%02d-%02d, %02d:%02d:%02d\n", year, month, day, hour, min, sec);
		vTaskDelay( 10 * configTICK_RATE_HZ );
	}

	vTaskDelete(NULL);
}

void wdt_task(void *pvParameters)
{
	wdt_init();
	vTaskDelay(100);
	wdt_start(10 * 32768);

	while (1)
	{
		vTaskDelay( 1 * configTICK_RATE_HZ );
		wdt_keep_alive();
	}

	vTaskDelete(NULL);
}

void shell_task(void *pvParameters);
void oled_task(void *pvParameters);
void tft_task(void *pvParameters);

xTaskHandle handle_oled = NULL;
xTaskHandle handle_shell = NULL;
xTaskHandle handle_led = NULL;
xTaskHandle handle_pwm = NULL;
xTaskHandle handle_rtc = NULL;
xTaskHandle handle_wdt = NULL;

int stack_check(void)
{
	printf("stack free:\n");

	if (handle_oled != NULL) 	printf("oled_task:  %d/800\n", (int)uxTaskGetStackHighWaterMark(handle_oled)*4);
	if (handle_shell != NULL) 	printf("shell_task: %d/800\n", (int)uxTaskGetStackHighWaterMark(handle_shell)*4);
	if (handle_led != NULL) 	printf("led_task:   %d/480\n", (int)uxTaskGetStackHighWaterMark(handle_led)*4);
	if (handle_pwm != NULL) 	printf("pwm_task:   %d/480\n", (int)uxTaskGetStackHighWaterMark(handle_pwm)*4);
	if (handle_rtc != NULL) 	printf("rtc_task:   %d/480\n", (int)uxTaskGetStackHighWaterMark(handle_rtc)*4);
	if (handle_wdt != NULL) 	printf("wdt_task:   %d/480\n", (int)uxTaskGetStackHighWaterMark(handle_wdt)*4);

	return 0;
}
SHELL_FUNCTION_EXPORT(check, stack_check, 0)

void init_task(void *pvParameters)
{
	//xTaskCreate(oled_task, "Task 2", 200, NULL, 2, &handle_oled);
	xTaskCreate(tft_task, "Task 2", 200, NULL, 2, &handle_oled);
	xTaskCreate(shell_task, "Task 3", 200, NULL, 3, &handle_shell);

 	xTaskCreate(led_task, "Task 4", 120, NULL, 4, NULL);
	//xTaskCreate(pwm_task, "Task 5", 120, NULL, 5, &handle_pwm);
	//xTaskCreate(rtc_task, "Task 6", 120, NULL, 6, &handle_rtc);
	xTaskCreate(wdt_task, "Task 7", 120, NULL, 6, &handle_wdt);

	vTaskDelete(NULL);
}

int main(void)
{
	prvSetupHardware();

	xTaskCreate(init_task, "init", 120, NULL, 5, NULL);

	vTaskStartScheduler();

	return 0;
}
