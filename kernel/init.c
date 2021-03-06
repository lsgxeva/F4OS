/*
 * Copyright (C) 2013, 2014 F4OS Authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include <linker_array.h>
#include <mm/mm.h>
#include <kernel/sched.h>
#include <kernel/fault.h>
#include <kernel/init.h>

#include <dev/arch.h>
#include <dev/char.h>
#include <dev/device.h>
#include <dev/hw/perfcounter.h>
#include <dev/hw/led.h>
#include <dev/hw/usart.h>
#include <dev/hw/usbdev.h>

const char banner[] = "Welcome to...\r\n"
                "\r\n"
                "88888888888      ,d8      ,ad8888ba,     ad88888ba   \r\n"
                "88             ,d888     d8\"\'    `\"8b   d8\"     \"8b  \r\n"
                "88           ,d8\" 88    d8\'        `8b  Y8,          \r\n"
                "88aaaaa    ,d8\"   88    88          88  `Y8aaaaa,    \r\n"
                "88\"\"\"\"\"  ,d8\"     88    88          88    `\"\"\"\"\"8b,  \r\n"
                "88       8888888888888  Y8,        ,8P          `8b  \r\n"
                "88                88     Y8a.    .a8P   Y8a     a8P  \r\n"
                "88                88      `\"Y8888Y\"\'     \"Y88888P\"   \r\n"
                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n";

void os_start(void) __attribute__((section(".kernel")));

void os_start(void) {
    init_arch();
    do_early_initializers();

#ifdef CONFIG_PERFCOUNTER
    init_perfcounter();
#endif

    init_heap();

#ifdef CONFIG_HAVE_LED
    init_power_led();
#endif

    do_core_initializers();

    device_driver_fdt_register();

    init_io();

    printf("\r\n%s\r\n", banner);

    if (!char_device_base_equal(stdout, stderr)) {
        fprintf(stderr, "\r\n%sStandard error terminal.\r\n", banner);
    }

    do_late_initializers();
    start_sched();
    panic_print("Task switching ended.");
}

LINKER_ARRAY_DECLARE(initializers_early)
LINKER_ARRAY_DECLARE(initializers_core)
LINKER_ARRAY_DECLARE(initializers_late)

/* TODO: Something nicer when an initializer fails... */

void do_early_initializers(void) {
    initializer_t *initializer;

    LINKER_ARRAY_FOR_EACH(initializers_early, initializer) {
        if (initializer->func()) {
            panic();
        }
    }
}

void do_core_initializers(void) {
    initializer_t *initializer;

    LINKER_ARRAY_FOR_EACH(initializers_core, initializer) {
        if (initializer->func()) {
            panic();
        }
    }
}

void do_late_initializers(void) {
    initializer_t *initializer;

    LINKER_ARRAY_FOR_EACH(initializers_late, initializer) {
        if (initializer->func()) {
            panic();
        }
    }
}
