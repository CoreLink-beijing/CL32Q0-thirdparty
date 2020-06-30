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


#ifndef BOARD_H__
#define BOARD_H__

#include <stdint.h>
#include "proton.h"
#include "bsp_uart.h"

extern unsigned char __bss_start__;
extern unsigned char __bss_end__;

#define RT_HW_HEAP_BEGIN    (void*)&__bss_end__
#define RT_HW_HEAP_END      (void*)(0x00104000)


void rt_hw_board_init(void);

#endif
