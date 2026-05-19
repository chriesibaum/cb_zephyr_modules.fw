# SPDX-License-Identifier: Apache-2.0

set(CB_INFO_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CB_INFO_CMAKE_DIR}")

# --- 1. App repo: generate fw_version.h (GIT_WORKING_DIR = app = CMAKE_SOURCE_DIR) ---
set(PRE_CONFIGURE_FILE        "${CB_INFO_CMAKE_DIR}/fw_version.h.in")
set(POST_CONFIGURE_FILE       "${CMAKE_BINARY_DIR}/cb_info/fw_version.h")
set(GIT_STATE_FILE            "${CMAKE_BINARY_DIR}/cb_info/git-state-app")
set(GIT_WATCHER_TARGET_NAME   "check_git_app")
unset(GIT_WORKING_DIR)
include(git_watcher)

# --- 2. Module repo: generate cb_modules_version.h ---
set(PRE_CONFIGURE_FILE        "${CB_INFO_CMAKE_DIR}/cb_modules_version.h.in")
set(POST_CONFIGURE_FILE       "${CMAKE_BINARY_DIR}/cb_info/cb_modules_version.h")
set(GIT_STATE_FILE            "${CMAKE_BINARY_DIR}/cb_info/git-state-modules")
set(GIT_WORKING_DIR           "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(GIT_WATCHER_TARGET_NAME   "check_git_modules")
include(git_watcher)

zephyr_library()
zephyr_library_sources(cb_info.c)
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
zephyr_include_directories(${CMAKE_BINARY_DIR}/cb_info)
cmake_host_system_information(RESULT _build_hostname QUERY FQDN)
string(TIMESTAMP _build_timezone "%Z")
zephyr_library_compile_definitions(CB_INFO_BUILD_HOSTNAME="${_build_hostname}")
zephyr_library_compile_definitions(CB_INFO_BUILD_TIMEZONE="${_build_timezone}")
add_dependencies(${ZEPHYR_CURRENT_LIBRARY} check_git_app check_git_modules)
