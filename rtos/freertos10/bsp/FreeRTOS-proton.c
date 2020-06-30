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
 * 2019-08-28     corelink     the first version
 */


#include "FreeRTOS.h"
#include "task.h"
#include "proton.h"


void vPortSetupTimerInterrupt(void)
{
    unsigned int CompareMatch;

    CompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

    /* Setup TIMER0 */
	timer_init(TIMER0);
	timer_set_prescaler(TIMER0, 0);
	timer_set_output_cmp(TIMER0, CompareMatch);
	timer_start(TIMER0);
	timer_int_enable(TIMER0);
}

void timer0_irq_handler(void)
{
	TIMER0_INT_RESET();

	/* Increment the RTOS tick. */
	if ( xTaskIncrementTick() != pdFALSE )
	{
		vTaskSwitchContext();
	}
}
DEFINE_IRQ_HANDLER(TIMER0_IRQ, timer0_irq_handler)

void vApplicationIdleHook( void )
{
	sleep();
}

