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


#ifndef __SHELL_H__
#define __SHELL_H__


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


#define MAX_ARGC        5
#define SH_CMD_NOTFND   -127
#define SH_CMD_ERROR    -126
#define SH_CMD_ARGCNT   -125
#define SH_CMD_SUCCESS  1

typedef struct cmd {
    uint8_t *arg[MAX_ARGC];
    uint8_t argc;
} cmd_t;

typedef int(*funcp_t) ();

struct shell_act
{
    char *name;
    funcp_t func;
    uint8_t argc;
};
typedef struct shell_act shell_act_t;

/*
int8_t shell(uint8_t * str, shell_act_t *cmd, uint8_t act_count);
*/
int8_t shell(uint8_t *str);
void shell_getc(uint8_t c);

/*
 * porting from rt-thread
 */
enum shell_var_type
{
	SHELL_VAR_INT8 		= 0,
	SHELL_VAR_UINT8 	= 1,
	SHELL_VAR_INT16 	= 2,
	SHELL_VAR_UINT16 	= 3,
	SHELL_VAR_INT32 	= 4,
	SHELL_VAR_UINT32 	= 5,
	SHELL_VAR_FLOAT 	= 6,
	SHELL_VAR_DOUBLE 	= 7,
	SHELL_VAR_STRING 	= 8,
	SHELL_VAR_PTR		= 9,
};
typedef enum shell_var_type shell_var_type_t;

struct shell_var
{
	char *name;
	void *addr;
	int type;
};
typedef struct shell_var shell_var_t;

#define SHELL_VAR_EXPORT(name, type)                              				\
    const char __vsym_##name##_name[] = #name;                          		\
    const struct shell_var __vsym_##name __attribute__((section("VSymTab"))) = 	\
    {                                                                   		\
    	(char *)__vsym_##name##_name,                                           \
		(void*)&name,															\
		type                                                          			\
    };

#define SHELL_FUNCTION_EXPORT(name, cmd, n)             						\
	const char __fsym_##cmd##_name[] = #name;                   				\
	const struct shell_act __fsym_##cmd __attribute__((section("FSymTab")))=	\
	{                                                     						\
		(char *)__fsym_##cmd##_name,                               				\
		(funcp_t)cmd,                                          					\
		n																		\
	};


#endif

/* EOF */
