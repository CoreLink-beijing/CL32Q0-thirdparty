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
 * 2019-09-23     corelink     the first version
 */


#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "board.h"


void rt_tft_init(void);

int main(int argc, char** argv)
{
	#define LED1	(10)

	rt_tft_init();

	rt_kprintf("www.corelink.vip\r\n");
    rt_kprintf("www.rt-thread.org\r\n");

	gpio_func_set(LED1, FUNC2_GPIO);
	gpio_mode_set(LED1, GPIO_MODE_OUT_PP);

	while (1)
	{
		gpio_set_pin_value(LED1, 1);
		rt_thread_delay(RT_TICK_PER_SECOND * 3);

		gpio_set_pin_value(LED1, 0);
		rt_thread_delay(RT_TICK_PER_SECOND * 3);
	}

    return 0;
}


void tft_task(void *pvParameters);

void rt_tft_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("tft", tft_task, RT_NULL,
                           2048, 25, 2);
    RT_ASSERT(tid != RT_NULL);

    rt_thread_startup(tid);
}
