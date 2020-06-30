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
 * 2019-08-30     corelink     the first version
 */


#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"


struct corelink_uart
{
	uart_dev_t *uart;
	int irqn;
	struct rt_serial_device *serial;
	char *device_name;
};

static rt_err_t  uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg);
static rt_err_t  uart_control(struct rt_serial_device *serial, int cmd, void *arg);
static int       uart_putc(struct rt_serial_device *serial, char c);
static int       uart_getc(struct rt_serial_device *serial);
static rt_size_t uart_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction);

const struct rt_uart_ops _uart_ops =
{
    uart_configure,
    uart_control,
    uart_putc,
    uart_getc,
    uart_dma_transmit
};

static void uart_isr(struct rt_serial_device *serial);

#if defined(BSP_USING_UART0)

struct rt_serial_device serial0;

void uart0_irq_handler(void)
{
	UART0_INT_RESET();
	uart_isr(&serial0);
}
DEFINE_IRQ_HANDLER(UART0_IRQ, uart0_irq_handler)

#endif

static const struct corelink_uart uarts[] =
{
#ifdef BSP_USING_UART0
    {
        (uart_dev_t *)UART0_BASE_ADDR,
        18,
        &serial0,
        "uart0",
    },
#endif
};

/*
 * UART Initiation
 */
int rt_hw_uart_init(void)
{
    int i;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

#if defined(BSP_USING_UART0)
	gpio_func_set(2, FUNC0_UART0_TX);
	gpio_func_set(3, FUNC0_UART0_RX);
	gpio_mode_set(2, GPIO_MODE_OUT_PP);
	gpio_mode_set(3, GPIO_MODE_IN_FLOATING);
#endif

    for (i = 0; i < sizeof(uarts) / sizeof(uarts[0]); i++)
    {
        uarts[i].serial->ops    = &_uart_ops;
        uarts[i].serial->config = config;

        /* register UART device */
        rt_hw_serial_register(uarts[i].serial,
                              uarts[i].device_name,
                              RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                              (void *)&uarts[i]);
    }

    return 0;
}

/*
 * UART interface
 */
static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct corelink_uart *uart;
    uint32_t baudrate;
    uart_databits_t databits;
    uart_stopbits_t stopbits;
    uart_parity_t parity;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (struct corelink_uart *)serial->parent.user_data;

    baudrate = cfg->baud_rate;

    switch (cfg->data_bits)
    {
    case DATA_BITS_7:
    	databits = UART_7BITS;
        break;

    default:
    	databits = UART_8BITS;
        break;
    }

    switch (cfg->stop_bits)
    {
    case STOP_BITS_2:
    	stopbits = UART_STOP_2;
        break;

    default:
    	stopbits = UART_STOP_1;
        break;
    }

    switch (cfg->parity)
    {
    case PARITY_ODD:
    	parity = UART_PARITY_ODD;
        break;

    default:
    	parity = UART_PARITY_NONE;
        break;
    }

    uartx_set_conf(uart->uart, baudrate, databits, stopbits, parity);
    uartx_int_enable(uart->uart);

    return RT_EOK;
}

static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct corelink_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct corelink_uart *)serial->parent.user_data;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
    	uartx_int_disable(uart->uart);

        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
    	uartx_int_enable(uart->uart);
        break;
    }

    return RT_EOK;
}

static int uart_putc(struct rt_serial_device *serial, char c)
{
    struct corelink_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct corelink_uart *)serial->parent.user_data;
    uartx_send_char(uart->uart, c);

    return (1);
}

static int uart_getc(struct rt_serial_device *serial)
{
    int ch = -1;
    struct corelink_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct corelink_uart *)serial->parent.user_data;
    if ((uart->uart)->rx_elm)
    	ch = uartx_get_char(uart->uart);

    return ch;
}

static rt_size_t uart_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    return (0);
}

/* UART ISR */
/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    struct corelink_uart *uart;

    RT_ASSERT(serial != RT_NULL);

    uart = (struct corelink_uart *) serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

    if ((uart->uart)->rx_elm)
    	rt_hw_serial_isr(uart->serial, RT_SERIAL_EVENT_RX_IND);
}

