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


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tools.h>
#include <shell.h>
#include <fifo.h>
#include <xprintf.h>


bool shell_get_cmd(uint8_t ** headp, uint8_t ** tailp, uint8_t c, uint8_t t) {
    if ((*headp)[0] == 0)
        return false;

    while ((*headp)[0] == c /*|| (*headp)[0] == t*/)
        (*headp)++;
    if ((*headp)[0] == 0)
        return false;

    (*tailp) = (*headp);
    while (/*(*tailp)[0] != t &&*/ (*tailp)[0] != 0) {
        (*tailp)++;
    }
    /*if ((*tailp)[0] != t)
        return false;*/

    (*tailp)[0] = 0;
    (*tailp)++;
    return true;
}

bool shell_get_args(uint8_t ** headp, uint8_t ** tailp, uint8_t ** endp, const uint8_t c) {
    while ((*headp)[0] == c && (*headp)[0] != 0)
        (*headp)++;
    if ((*headp)[0] == 0)
        return false;

    (*tailp) = (*headp);
    while ((*tailp)[0] != c && (*tailp)[0] != 0) {
        (*tailp)++;
    }

    if ((*tailp) >= (*endp))
        return false;

    (*tailp)[0] = 0;
    (*tailp)++;
    return true;
}

/*
int8_t shell(uint8_t * str, act_t * act, uint8_t act_count) {

    uint8_t semic = ';';
    uint8_t space = ' ';

    uint8_t *head = str;
    uint8_t *tail;

    while (shell_get_cmd(&head, &tail, space, semic)) {
        uint8_t *cmd_head = head;
        uint8_t *cmd_tail;

        cmd_t cmd = { };
        cmd.argc = 0;

        while (shell_get_args(&cmd_head, &cmd_tail, &tail, space) && (cmd.argc < MAX_ARGC)) {
            cmd.arg[cmd.argc] = cmd_head;
            cmd.argc++;
            cmd_head = cmd_tail;
        };

        uint8_t i = 0;

        int16_t ret_code = 0;
        bool act_found = false;

        while (i < act_count) {
            if (str_cmp((uint8_t *)act[i].name, cmd.arg[0])) {
                act_found = true;
                if (cmd.argc < act[i].argc) {
                    return SH_CMD_ARGCNT;
                    break;
                }
                switch (act[i].argc) {
                case 0:
					xprintf("%s\n", cmd.arg[0]);
                    ret_code = (act[i].func)();
                    break;
                case 1:
                    xprintf("%s %s\n", cmd.arg[0], cmd.arg[1]);
					ret_code = (act[i].func)(cmd.arg[1]);
                    break;
                case 2:
                    xprintf("%s %s %s\n", cmd.arg[0], cmd.arg[1], cmd.arg[2]);
					ret_code = (act[i].func)(cmd.arg[1], cmd.arg[2]);
                    break;
                case 3:
                    xprintf("%s %s %s %s\n", cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3]);
					ret_code = (act[i].func)(cmd.arg[1], cmd.arg[2], cmd.arg[3]);
                    break;
                case 4:
                    xprintf("%s %s %s %s %s\n", cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3], cmd.arg[4]);
					ret_code = (act[i].func)(cmd.arg[1], cmd.arg[2], cmd.arg[3], cmd.arg[4]);
                    break;
                default:
                    break;
                };
                break;
            }
            i++;
        }

        if (!act_found) {
            return SH_CMD_NOTFND;
        } else if (ret_code < 0) {
            return SH_CMD_ERROR;
        } else {
            return ret_code;
        }
        head = tail;
    }
    return SH_CMD_SUCCESS;
}
*/

struct shell_act *_shell_func;
struct shell_act *_shell_func_end;
extern const int __fsymtab_start;
extern const int __fsymtab_end;

int8_t shell(uint8_t *str) {

    uint8_t semic = ';';
    uint8_t space = ' ';

    uint8_t *head = str;
    uint8_t *tail;

	_shell_func = (struct shell_act *)&__fsymtab_start;
	_shell_func_end   = (struct shell_act *)&__fsymtab_end;

    while (shell_get_cmd(&head, &tail, space, semic)) {
        uint8_t *cmd_head = head;
        uint8_t *cmd_tail;

        cmd_t cmd = { };
        cmd.argc = 0;

        while (shell_get_args(&cmd_head, &cmd_tail, &tail, space) && (cmd.argc < MAX_ARGC)) {
            cmd.arg[cmd.argc] = cmd_head;
            cmd.argc++;
            cmd_head = cmd_tail;
        };

        int16_t ret_code = 0;
        bool act_found = false;

    	while (_shell_func < _shell_func_end)
    	{
    		if (str_cmp((uint8_t *)_shell_func->name, cmd.arg[0])) {
                act_found = true;
                if (cmd.argc < _shell_func->argc) {
                    return SH_CMD_ARGCNT;
                    break;
                }
                switch (_shell_func->argc) {
                case 0:
					xprintf("%s\n", cmd.arg[0]);
                    ret_code = (_shell_func->func)();
                    break;
                case 1:
                    xprintf("%s %s\n", cmd.arg[0], cmd.arg[1]);
					ret_code = (_shell_func->func)(cmd.arg[1]);
                    break;
                case 2:
                    xprintf("%s %s %s\n", cmd.arg[0], cmd.arg[1], cmd.arg[2]);
					ret_code = (_shell_func->func)(cmd.arg[1], cmd.arg[2]);
                    break;
                case 3:
                    xprintf("%s %s %s %s\n", cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3]);
					ret_code = (_shell_func->func)(cmd.arg[1], cmd.arg[2], cmd.arg[3]);
                    break;
                case 4:
                    xprintf("%s %s %s %s %s\n", cmd.arg[0], cmd.arg[1], cmd.arg[2], cmd.arg[3], cmd.arg[4]);
					ret_code = (_shell_func->func)(cmd.arg[1], cmd.arg[2], cmd.arg[3], cmd.arg[4]);
                    break;
                default:
                    break;
                };

                break;
            }

    		_shell_func++;
        }

        if (!act_found) {
            return SH_CMD_NOTFND;
        } else if (ret_code < 0) {
            return SH_CMD_ERROR;
        } else {
            return ret_code;
        }
        head = tail;
    }
    return SH_CMD_SUCCESS;
}

struct shell_var *_shell_var;
struct shell_var *_shell_var_end;
extern const int __vsymtab_start;
extern const int __vsymtab_end;

int get(uint8_t *arg1)
{
	_shell_var = (struct shell_var *)&__vsymtab_start;
	_shell_var_end   = (struct shell_var *)&__vsymtab_end;

	while (_shell_var < _shell_var_end)
	{
		if ( memcmp((void *)arg1, (const void *)_shell_var->name, strlen((char *)_shell_var->name)) == 0 )
		{
			switch (_shell_var->type)
			{
				case SHELL_VAR_STRING:
					printf("%s[0x%08X] = %s\n", _shell_var->name, (unsigned int)(_shell_var->addr), (char *)(_shell_var->addr));
					break ;

				case SHELL_VAR_PTR:
					printf("%s[0x%08X] = 0x%08X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(unsigned int *)(_shell_var->addr));
					break ;

				case SHELL_VAR_DOUBLE:
					printf("%s[0x%08X] = %lf\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(double *)(_shell_var->addr));
					break ;

				case SHELL_VAR_FLOAT:
					printf("%s[0x%08X] = %f\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(float *)(_shell_var->addr));
					break ;

				case SHELL_VAR_INT8:
				case SHELL_VAR_UINT8:
					printf("%s[0x%08X] = 0x%02X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(char *)(_shell_var->addr));
					break ;

				case SHELL_VAR_INT16:
				case SHELL_VAR_UINT16:
					printf("%s[0x%08X] = 0x%04X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(short *)(_shell_var->addr));
					break ;

				case SHELL_VAR_INT32:
				case SHELL_VAR_UINT32:
					printf("%s[0x%08X] = 0x%08X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(int *)(_shell_var->addr));
					break ;

				default:
					printf("invalid type\n");
					break ;
			}

			break ;
		}

		_shell_var++;
	}

	return 0;
}
SHELL_FUNCTION_EXPORT(get, get, 1)

int set(uint8_t *arg1, uint8_t *arg2)
{
	char *ptr;
	long v;

	_shell_var = (struct shell_var *)&__vsymtab_start;
	_shell_var_end   = (struct shell_var *)&__vsymtab_end;

	while (_shell_var < _shell_var_end)
	{
		if ( memcmp((void *)arg1, (const void *)_shell_var->name, strlen((char *)_shell_var->name)) == 0 )
		{
			switch (_shell_var->type)
			{
			case SHELL_VAR_STRING:
				memcpy(_shell_var->addr, arg2, strlen((char *)arg1));
				printf("%s[0x%08X] = %s\n", _shell_var->name, (unsigned int)(_shell_var->addr), (char *)(_shell_var->addr));
				break ;

			case SHELL_VAR_PTR:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(unsigned int *)(_shell_var->addr) = (unsigned int)v;
				printf("%s[0x%08X] = 0x%08X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(unsigned int *)(_shell_var->addr));
				break ;

			case SHELL_VAR_INT8:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(int8_t *)(_shell_var->addr) = (int8_t)v;
				printf("%s[0x%08X] = 0x%02X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(char *)(_shell_var->addr));
				break ;

			case SHELL_VAR_UINT8:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(uint8_t *)(_shell_var->addr) = (uint8_t)v;
				printf("%s[0x%08X] = 0x%02X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(char *)(_shell_var->addr));
				break ;

			case SHELL_VAR_INT16:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(int16_t *)(_shell_var->addr) = (int16_t)v;
				printf("%s[0x%08X] = 0x%04X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(short *)(_shell_var->addr));
				break ;

			case SHELL_VAR_UINT16:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(uint16_t *)(_shell_var->addr) = (uint16_t)v;
				printf("%s[0x%08X] = 0x%04X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(short *)(_shell_var->addr));
				break ;

			case SHELL_VAR_INT32:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(int32_t *)(_shell_var->addr) = (int32_t)v;
				printf("%s[0x%08X] = 0x%08X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(int *)(_shell_var->addr));
				break ;

			case SHELL_VAR_UINT32:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				*(uint32_t *)(_shell_var->addr) = (uint32_t)v;
				printf("%s[0x%08X] = 0x%08X\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(int *)(_shell_var->addr));
				break ;

			case SHELL_VAR_FLOAT:
				ptr = (char *)arg2;
				xatoi(&ptr, &v);
				memcpy(_shell_var->addr, &v, sizeof(long));
				printf("%s[0x%08X] = %f\n", _shell_var->name, (unsigned int)(_shell_var->addr), *(float *)(_shell_var->addr));
				break ;

			case SHELL_VAR_DOUBLE:
				printf("not support double type\n");
				break ;

			default:
				printf("invalid type\n");
				break ;
			}

			break ;
		}

		_shell_var++;
	}

	return 0;
}
SHELL_FUNCTION_EXPORT(set, set, 2)

int help(void)
{
	_shell_var = (struct shell_var *)&__vsymtab_start;
	_shell_var_end   = (struct shell_var *)&__vsymtab_end;

	if (_shell_var_end > _shell_var)
		printf("var:\n");

	while (_shell_var < _shell_var_end)
	{
		printf("    %s\n", _shell_var->name);
		_shell_var++;
	}

	_shell_func = (struct shell_act *)&__fsymtab_start;
	_shell_func_end   = (struct shell_act *)&__fsymtab_end;

	if (_shell_func_end > _shell_func)
		printf("cmd:\n");

	while (_shell_func < _shell_func_end)
	{
		printf("    %s\n", _shell_func->name);
		_shell_func++;
	}

	return 0;
}
SHELL_FUNCTION_EXPORT(help, help, 0)


static FIFO fifo_in;
static FIFO *in;
static uint8_t inbuf[FIFO_BUFFER_SIZE];

static void shell_io_init(void)
{
    in = &fifo_in;
    fifo_init(in, inbuf, sizeof(inbuf));
}

void shell_getc(uint8_t c)
{
	if (c == '\n')
	{
		fifo_putc(in, ' ');
	}
	else
	{
		fifo_putc(in, c);
	}
}


#ifdef __FreeRTOS__

#include "FreeRTOS.h"
#include "task.h"

__attribute__((weak)) void tiny_shell_run(void)
{
	#define MAX_CMD_LEN 128

	char prompt[] = "corelink>";
	uint8_t str[MAX_CMD_LEN];

	shell_io_init();
	xputs(prompt);

	while (1)
	{
		while (fifo_get_token(in, str, MAX_CMD_LEN, '\r') > 0)
		{
			/*
			int8_t ret_code = shell(str, shell_act, sizeof(shell_act) / sizeof(shell_act[0]));
			*/
			int8_t ret_code = shell(str);
            if (ret_code == SH_CMD_NOTFND)
            {
                xputs((char *) "COMMAND NOT FOUND\r\n");
            }
            xputs(prompt);
        }
		vTaskDelay(configTICK_RATE_HZ / 50);
	}
}

void shell_task (void *pvParameters)
{
	tiny_shell_run();

	return ;
}

#else	/* non-OS */

__attribute__((weak)) void tiny_shell_run(void)
{
	#define MAX_CMD_LEN 128

	char prompt[] = "proton>";
	uint8_t str[MAX_CMD_LEN];

	shell_io_init();
	xputs(prompt);

	while (1)
	{
		{
			/* todo: other task */
		}

		/* shell */
		while (fifo_get_token(in, str, MAX_CMD_LEN, '\r') > 0)
		{
            /*
			int8_t ret_code = shell(str, shell_act, sizeof(shell_act) / sizeof(shell_act[0]));
			*/
			int8_t ret_code = shell(str);
            if (ret_code == SH_CMD_NOTFND)
            {
                xputs((char *) "COMMAND NOT FOUND\r\n");
            }
            xputs(prompt);
        }
	}
}

#endif	/* #ifdef __FreeRTOS__ */

//EOF
