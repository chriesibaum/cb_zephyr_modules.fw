#include <stdio.h>

#include <zephyr/kernel.h>

#include "cb_error.h"
#include "cb_sys.h"
#include "cb_info.h"

// ---- logging includes/defines ----------------------------------------------
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);


int32_t main(void)
{
    int ret;

    cb_info();

    ret = cb_sys_init();
    ERR_CHECK(ret < 0, ret, "cb_sys_init() failed (err %d)", ret);

    LOG_INF("   Chriesibaum's getting started sample is running fine!");
    LOG_INF("   Enjoy building on Zephyr with cb_zephyr_modules!");
    LOG_INF("   Let's have a nice cup of Café and get going with your project! :)");
    LOG_INF("   --------------------------------------------------------------------");

    while (true) {
        k_sleep(K_SECONDS(1));
    }
}
