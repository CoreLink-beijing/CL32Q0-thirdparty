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
 * 2019-02-28     corelink     *
 */

/*
 * https://github.com/onborodin/mcu-tiny-shell
 */

#ifndef __FIFO_H__
#define __FIFO_H__

#ifndef FIFO_BUFFER_SIZE
#define FIFO_BUFFER_SIZE 256
#endif

typedef struct fifo {
    volatile uint32_t head;
    volatile uint32_t tail;
    volatile uint8_t *buffer;
    uint32_t buffer_len;
} FIFO;

void fifo_init(FIFO * b, uint8_t * buffer, uint32_t buffer_len);
uint8_t fifo_count(const FIFO * b);
int fifo_full(const FIFO * b);
int fifo_empty(const FIFO * b);
uint8_t fifo_peek(const FIFO * b);
uint8_t fifo_getc(FIFO * b);
int fifo_putc(FIFO * b, uint8_t data);
uint32_t fifo_puts(FIFO * b, uint8_t * str);
int fifo_scanc(FIFO * b, uint8_t c);
uint8_t fifo_get_token(FIFO * b, uint8_t * str, uint32_t len, uint8_t);
int fifo_back(FIFO * b);


#endif
/* EOF */
