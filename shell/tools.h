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
 * 2019-08-22     corelink		*
 */

/*
 * https://github.com/onborodin/mcu-tiny-shell
 */

#ifndef __TOOLS_H__
#define __TOOLS_H__

#define MAX_LINE_LEN 1024

uint16_t str_len(uint8_t * str);
uint8_t * str_replc(uint8_t ** str, uint8_t c, uint8_t r);
int str_cmp(uint8_t * str1, uint8_t * str2);
uint8_t * str_copy(uint8_t * src, uint8_t * dst, uint8_t len);
uint8_t * str_ltrim(uint8_t * str, uint8_t c);
uint8_t * str_trim(uint8_t * str, uint8_t c);
int32_t int_pow(uint8_t n, uint8_t s);
int32_t str2int(uint8_t * str);
uint8_t * int2str(int32_t num, uint8_t * dst, uint8_t dst_len, int16_t base);
uint8_t * uint2str(uint32_t num, uint8_t * dst, uint8_t dst_len, int16_t base);
uint8_t * int2str_r(int32_t num, uint8_t * dst, uint8_t dst_len, int16_t base);
uint8_t * uint2str_r(int32_t num, uint8_t * dst, uint8_t dst_len, int16_t base);
uint8_t * str_toright(uint8_t *src, uint8_t *dst, uint8_t len_dst);
uint8_t * tok_comp(uint8_t ** str, uint8_t c, uint8_t * end);

#endif
/* EOF */
