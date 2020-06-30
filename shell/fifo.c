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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <fifo.h>
#include <tools.h>

void fifo_init(FIFO *fifo, uint8_t * buffer, uint32_t buffer_len) {
    if (fifo && buffer) {
        memset((void **)buffer, 0, buffer_len);
        fifo->buffer_len = buffer_len;
        fifo->buffer = buffer;
        fifo->head = 0;
        fifo->tail = 0;
    }
}

uint8_t fifo_count(const FIFO *fifo) {
    if (fifo) {
        return (fifo->head - fifo->tail);
        /*
        if (fifo->head >= fifo->tail)
           return (fifo->head - fifo->tail);
        else
            return (fifo->tail - fifo->head);
         */
    }
    return 0;
}

int fifo_full(const FIFO *fifo) {
    if (fifo) {
        return (fifo_count(fifo) == fifo->buffer_len);
    }
    return 1;
}

int fifo_empty(const FIFO *fifo) {
    if (fifo) {
        return (fifo_count(fifo) == 0);
    }
    return 1;
}

uint8_t fifo_peek(const FIFO *fifo) {
    uint8_t data = 0;

    if (!fifo_empty(fifo)) {
        data = fifo->buffer[fifo->tail % fifo->buffer_len];
    }
    return data;
}

int fifo_back(FIFO *fifo) {
    if (!fifo_empty(fifo)) {
        fifo->head--;
        return 1;
    }
    return 0;
}

uint8_t fifo_getc(FIFO *fifo) {
    uint8_t data = 0;

    if (!fifo_empty(fifo)) {
        data = fifo->buffer[fifo->tail % fifo->buffer_len];
        fifo->tail++;
    }
    return data;
}

int fifo_putc(FIFO *fifo, uint8_t data) {
    int status = 0;

    if (fifo) {
        if (!fifo_full(fifo)) {
            fifo->buffer[fifo->head % fifo->buffer_len] = data;
            fifo->head++;
            status = 1;
        }
    }
    return status;
}

uint32_t fifo_puts(FIFO *fifo, uint8_t * string) {
    if (fifo) {
        for (uint8_t i = 0; i < str_len(string); i++) {
            if (!fifo_putc(fifo, string[i]))
                return i;
        }
    }
    return 0;
}

int fifo_scanc(FIFO *fifo, uint8_t c) {
    if (fifo) {
        if (!fifo_empty(fifo)) {
            uint8_t tail = fifo->tail;

            for (uint8_t i = 0; i < fifo_count(fifo); i++) {
                uint8_t data = fifo->buffer[tail % fifo->buffer_len];

                if (data == c) {
                    return 1;
                }
                tail++;
            }
        }
        return 0;
    }
    return 0;
}

uint8_t fifo_get_token(FIFO *fifo, uint8_t * str, uint32_t len, uint8_t term) {
    if (fifo) {
        memset((void *)str, 0, len);

        if (fifo_scanc(fifo, term) && str) {
            uint8_t i = 0, c = 0;

            while ((c = fifo_getc(fifo)) != 0 && c != term && i < len) {
                str[i] = c;
                i++;
            }
            return i;
        }
        return 0;
    }
    return 0;
}

/* EOF */
