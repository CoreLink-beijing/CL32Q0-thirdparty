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
 * 2019-09-02     corelink     the first version
 */


#include <rthw.h>
#include <rtthread.h>
#include "board.h"


int rt_hw_systick_init(void)
{
    unsigned int CompareMatch;

    CompareMatch = system_clock_get() / RT_TICK_PER_SECOND;

    /* Setup TIMER0 */
	timer_init(TIMER0);
	timer_set_prescaler(TIMER0, 0);
	timer_set_output_cmp(TIMER0, CompareMatch);
	timer_start(TIMER0);
	timer_int_enable(TIMER0);

    return 0;
}

void systick_handler(void)
{
	TIMER0_INT_RESET();

	rt_tick_increase();
}
DEFINE_IRQ_HANDLER(TIMER0_IRQ, systick_handler)

void rt_hw_board_init(void)
{
	system_clock_set(SYSCLK_FREQ_16M);

	/* initialize hardware interrupt */
    rt_hw_uart_init();
    rt_hw_systick_init();

#ifdef RT_USING_CONSOLE
    /* set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif /* RT_USING_CONSOLE */

#ifdef RT_USING_HEAP
    /* initialize memory system */
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}
