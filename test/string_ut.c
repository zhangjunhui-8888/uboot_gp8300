/*
 * Copyright (c) 2016 GPT, Inc
 * Written by Wu Feng <fwu@gpt.com>
 */

#include <common.h>
#include <command.h>
#include <errno.h>

static const char *s_refstr =
    "1234567890abcdefghicklmopqrstuvwxyz!@#$%^&*()\n"               \
    "1234567890abcdefghicklmopqrstuvwxyz!@#$%^&*()\n"               \
    "1234567890abcdefghicklmopqrstuvwxyz!@#$%^&*()\n"               \
    "1234567890abcdefghicklmopqrstuvwxyz!@#$%^&*()\n"               \
    ;

static int test_string_strcpy(void)
{
	ulong start, delta;
	int iter;
    int ret = -EINVAL;
    const char datastr[256];
    printf("%s start:\n", __func__);
    printf("Expect: size: %d\n[%s]\n", 188, s_refstr);
    strcpy(datastr, s_refstr);
    printf("Real  : size: %d\n[%s]\n", strlen(datastr), datastr);
    ret = !strcmp(datastr, s_refstr);
    printf("Check : %s\n", ret ? "OK" : "Invalid");
	start = get_timer(0);
	for (iter = 0; iter < 1000; iter++)
		strcpy(datastr, s_refstr);
	delta = get_timer(start);
	printf("%sTest do 1000 times, time use: %lu ms\n", __func__, delta);

	return ret;
}

static int test_string_strlen(void)
{
	ulong start, delta;
	int iter;
    int ret = -EINVAL;
    const char datastr[256];
    printf("%s start:\n", __func__);
    printf("Expect: size: %d\n[%s]\n", 188, s_refstr);
    printf("Real  : size: %d\n", strlen(s_refstr));
    ret = (188 == strlen(s_refstr));
    printf("Check : %s\n", ret ? "OK" : "Invalid");
	start = get_timer(0);
	for (iter = 0; iter < 1000; iter++)
		strlen(s_refstr);
	delta = get_timer(start);
	printf("%sTest do 1000 times, time use: %lu ms\n", __func__, delta);

	return ret;
}

static int test_string_strcmp(void)
{
	ulong start, delta;
	int iter;
    int ret = -EINVAL;
    const char datastr[256];
    printf("%s start:\n", __func__);
    printf("Expect: size: %d\n[%s]\n", 188, s_refstr);
    strcpy(datastr, s_refstr);
    printf("Real  : size: %d\n[%s]\n", strlen(datastr), datastr);
    ret = !strcmp(datastr, s_refstr);
    printf("Check : %s\n", ret ? "OK" : "Invalid");
	start = get_timer(0);
	for (iter = 0; iter < 1000; iter++)
		strcmp(datastr, s_refstr);
	delta = get_timer(start);
	printf("%sTest do 1000 times, time use: %lu ms\n", __func__, delta);

	return ret;
}

int do_ut_string(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;

    printf("String testing:\n");
	ret |= test_string_strcpy();
	ret |= test_string_strlen();
	ret |= test_string_strcmp();

	printf("Test %s\n", ret ? "failed" : "passed");

	return ret ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
}
