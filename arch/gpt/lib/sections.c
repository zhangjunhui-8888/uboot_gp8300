/*
 * Copyright 2016 scxie <scxie@hxgpt.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/**
 * refer to ARM
 */

char __bss_start[0] __attribute__((section(".__bss_start")));
char __bss_end[0] __attribute__((section(".__bss_end")));
char __image_copy_start[0] __attribute__((section(".__image_copy_start")));
char __image_copy_end[0] __attribute__((section(".__image_copy_end")));
char __rel_dyn_start[0] __attribute__((section(".__rela_dyn_start")));
char __rel_dyn_end[0] __attribute__((section(".__rela_dyn_end")));
char __data_load_start[0] __attribute__((section(".data_load_start")));
char __data_load_end[0] __attribute__((section(".data_load_end")));
char __data_start[0] __attribute__((section(".data")));
char __data_end[0] __attribute__((section(".data")));
char _end[0] __attribute__((section(".__end")));
