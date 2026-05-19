/*
 * Copyright (c) 2026 Chriesibaum GmbH
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include <zephyr/ztest.h>

#include "cb_info.h"

static bool starts_with(const char *value, const char *prefix)
{
    return (strncmp(value, prefix, strlen(prefix)) == 0);
}

ZTEST(cb_info, test_cb_info_get_device_info_format)
{
    size_t count = 0u;
    const char *const *lines = cb_info_get_device_info(&count);

    zassert_not_null(lines, "cb_info_get_device_info returned NULL");
    zassert_equal(count, DEVICE_INFO_LINES_COUNT,
                  "Expected %u lines, got %u",
                  (unsigned int)DEVICE_INFO_LINES_COUNT,
                  (unsigned int)count);

    for (size_t i = 0u; i < count; i++) {
        zassert_not_null(lines[i], "Line %u is NULL", (unsigned int)i);
    }

    zassert_equal(strcmp(lines[1], "Application and board information:"), 0,
                  "Unexpected title line");
    zassert_true(starts_with(lines[2], "  App release:         "), "Missing app release line");
    zassert_true(starts_with(lines[3], "  CB modules release:  "), "Missing module release line");
    zassert_true(starts_with(lines[4], "  Zephyr release:      "), "Missing Zephyr release line");
    zassert_true(starts_with(lines[5], "  Build date:          "), "Missing build date line");
    zassert_true(starts_with(lines[6], "  Build host:          "), "Missing build host line");
    zassert_true(starts_with(lines[7], "  Board name:          "), "Missing board line");
    zassert_true(starts_with(lines[8], "  SOC:                 "), "Missing SOC line");
    zassert_true(starts_with(lines[9], "  CPU type:            "), "Missing CPU line");
    zassert_true(starts_with(lines[10], "  Device ID:           "), "Missing device ID line");
}

ZTEST(cb_info, test_cb_info_get_device_info_stable_across_calls)
{
    size_t first_count = 0u;
    size_t second_count = 0u;
    const char *const *first = cb_info_get_device_info(&first_count);
    const char *const *second = cb_info_get_device_info(&second_count);

    zassert_not_null(first, "First call returned NULL");
    zassert_not_null(second, "Second call returned NULL");
    zassert_equal(first_count, DEVICE_INFO_LINES_COUNT, "Unexpected first count");
    zassert_equal(second_count, DEVICE_INFO_LINES_COUNT, "Unexpected second count");
    zassert_not_null(second[10], "Device ID line is NULL");
}

ZTEST(cb_info, test_cb_info_log_smoke)
{
    cb_info();
    zassert_true(true, "cb_info should not fail");
}

ZTEST_SUITE(cb_info, NULL, NULL, NULL, NULL, NULL);
