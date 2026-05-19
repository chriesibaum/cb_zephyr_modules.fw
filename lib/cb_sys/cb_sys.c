/*
 * Copyright (c) 2026 Chriesibaum GmbH
 * SPDX-License-Identifier: Apache-2.0
 */


// ---- system includes -------------------------------------------------------
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/util.h>
#include <errno.h>

// ---- logging includes/defines ----------------------------------------------
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(CB_SYS, CONFIG_LOG_DEFAULT_LEVEL);

// application includes
#include "cb_sys.h"
#include "cb_error.h"


#define LED_SYS_NODE DT_ALIAS(sys_led)
#define LED_SYS_PWM_NODE DT_ALIAS(sys_led_pwm)

#define LED_BLINK_DURATION_MS  50   // LED on duration in milliseconds
#define LED_BLINK_PERIOD_MS \
    MAX(1U, (60000U + (CONFIG_CB_SYS_LED_BPM / 2U)) / CONFIG_CB_SYS_LED_BPM)


#if defined(CONFIG_CB_SYS_LED_HEARTBEAT) && DT_NODE_HAS_STATUS(LED_SYS_PWM_NODE, okay)
static const struct pwm_dt_spec led_sys_pwm = PWM_DT_SPEC_GET(LED_SYS_PWM_NODE);

static const uint8_t led_heartbeat_pattern_keyframes_pct[] = {
    0, 10, 30, 60, 100, 80, 50, 20, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define LED_HEARTBEAT_INTERP_STEPS 4U
#define LED_HEARTBEAT_KEYFRAME_COUNT ARRAY_SIZE(led_heartbeat_pattern_keyframes_pct)
#define LED_HEARTBEAT_STEP_COUNT (LED_HEARTBEAT_KEYFRAME_COUNT * LED_HEARTBEAT_INTERP_STEPS)
#define LED_HEARTBEAT_STEP_PERIOD_MS \
    MAX(1U, (60000U + ((CONFIG_CB_SYS_LED_BPM * LED_HEARTBEAT_STEP_COUNT) / 2U)) / \
            (CONFIG_CB_SYS_LED_BPM * LED_HEARTBEAT_STEP_COUNT))

static uint8_t led_heartbeat_idx;

static void sys_led_pwm_timer_handler(struct k_timer *timer);
K_TIMER_DEFINE(sys_led_pwm_timer, sys_led_pwm_timer_handler, NULL);

static void sys_led_pwm_timer_handler(struct k_timer *timer)
{
    int32_t ret;
    uint32_t pulse_ns;
    uint32_t frame_idx;
    uint32_t next_frame_idx;
    uint32_t interp_idx;
    int32_t pct_a;
    int32_t pct_b;
    uint32_t pct;

    ARG_UNUSED(timer);

    frame_idx = led_heartbeat_idx / LED_HEARTBEAT_INTERP_STEPS;
    next_frame_idx = (frame_idx + 1U) % LED_HEARTBEAT_KEYFRAME_COUNT;
    interp_idx = led_heartbeat_idx % LED_HEARTBEAT_INTERP_STEPS;

    pct_a = led_heartbeat_pattern_keyframes_pct[frame_idx];
    pct_b = led_heartbeat_pattern_keyframes_pct[next_frame_idx];
    pct = (uint32_t)(pct_a + (((pct_b - pct_a) * (int32_t)interp_idx) /
                              (int32_t)LED_HEARTBEAT_INTERP_STEPS));
    pct = MIN(pct, 100U);

    pulse_ns = (uint32_t)(((uint64_t)led_sys_pwm.period * pct) / 100U);

    led_heartbeat_idx++;
    if (led_heartbeat_idx >= LED_HEARTBEAT_STEP_COUNT) {
        led_heartbeat_idx = 0;
    }

    ret = pwm_set_pulse_dt(&led_sys_pwm, pulse_ns);
    if (ret < 0) {
        LOG_ERR("Failed to update PWM pulse (%d)", ret);
    }
}

#elif defined(CONFIG_CB_SYS_LED_BLINK) && DT_NODE_HAS_STATUS(LED_SYS_NODE, okay)
static const struct gpio_dt_spec led_sys = GPIO_DT_SPEC_GET(LED_SYS_NODE, gpios);

// cb sys led timer
static void cb_sys_led_timer_handler(struct k_timer *timer);
K_TIMER_DEFINE(cb_sys_led_timer, cb_sys_led_timer_handler, NULL);

static void cb_sys_led_timer_off_handler(struct k_timer *timer);
K_TIMER_DEFINE(cb_sys_led_timer_off, cb_sys_led_timer_off_handler, NULL);

static void cb_sys_led_timer_handler(struct k_timer *timer)
{
    gpio_pin_set_dt(&led_sys, 1);
    k_timer_start(&cb_sys_led_timer_off, K_MSEC(LED_BLINK_DURATION_MS), K_NO_WAIT);
}

static void cb_sys_led_timer_off_handler(struct k_timer *timer)
{
    gpio_pin_set_dt(&led_sys, 0);
}
#endif

int32_t cb_sys_init(void)
{
#if defined(CONFIG_CB_SYS_LED_HEARTBEAT) && DT_NODE_HAS_STATUS(LED_SYS_PWM_NODE, okay)
    int32_t ret;

    ERR_CHECK(!pwm_is_ready_dt(&led_sys_pwm), -ENODEV, "sys-led-pwm not ready");

    led_heartbeat_idx = 0;

    ret = pwm_set_pulse_dt(&led_sys_pwm, 0);
    ERR_CHECK(ret < 0, ret, "Failed to set initial PWM pulse (%d)", ret);

    k_timer_start(&sys_led_pwm_timer,
                  K_MSEC(LED_HEARTBEAT_STEP_PERIOD_MS),
                  K_MSEC(LED_HEARTBEAT_STEP_PERIOD_MS));
    return 0;

#elif defined(CONFIG_CB_SYS_LED_BLINK) && DT_NODE_HAS_STATUS(LED_SYS_NODE, okay)
    ERR_CHECK(!gpio_is_ready_dt(&led_sys), -ENODEV, "sys-led GPIO not ready");

    int32_t ret = gpio_pin_configure_dt(&led_sys, GPIO_OUTPUT_INACTIVE);
    ERR_CHECK(ret < 0, ret, "Failed to configure sys-led GPIO (%d)", ret);

    /* Start periodic blink timer derived from configured BPM. */
    k_timer_start(&cb_sys_led_timer, K_MSEC(LED_BLINK_PERIOD_MS), K_MSEC(LED_BLINK_PERIOD_MS));

    return 0;

#else
    return -ENODEV;
#endif
}
