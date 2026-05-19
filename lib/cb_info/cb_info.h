/*
 * Copyright (c) 2026 Chriesibaum GmbH
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CB_ZEPHYR_MODULE_CB_INFO_H_
#define CB_ZEPHYR_MODULE_CB_INFO_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEVICE_INFO_LINES_COUNT 12u

/*
* Returns a pointer to a static array of informational text lines.
* The returned pointer remains valid until the next call.
*/
const char *const *cb_info_get_device_info(size_t *count);

/**
 * @brief Print build, system and version info via logging.
 */
void cb_info(void);

#ifdef __cplusplus
}
#endif

#endif /* CB_ZEPHYR_MODULE_CB_INFO_H_ */
