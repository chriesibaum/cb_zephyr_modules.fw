# SPDX-License-Identifier: GPL-3.0
# Copyright (c) 2023 Chriesibaum GmbH


# select the BOARD type here:

BOARD := nucleo_u385rg_q
# BOARD := rpi_pico
# BOARD := rpi_pico2/rp2350a/m33
# BOARD := rpi_pico2/rp2350a/hazard3		# INFO/Warning: not working yet as there are no led's configured!



.phony: all clean flash print release copy_uf2_PRI-PR2 copy_uf2_RP2350 flash.stm32cubeprogrammer


all:
	@echo get the build process working...
	west build -p auto -b $(BOARD) ./ \
		-DEXTRA_CONF_FILE="prj_log.conf"



clean:
	rm -rf ./build/
	rm -rf ./$(RELEASE_DIR)/

flash:
	west flash -r jlink

flash.stm32cubeprogrammer:
	west flash -r stm32cubeprogrammer

flash.pyocd:
	west flash -r pyocd


RELEASE_DIR=release

BOARD_NAME=$(subst /,_,$(BOARD))
FW_VERSION=$(shell sed -n 's/^#define[[:space:]]\+FW_VERSION_STRING[[:space:]]\+"\([^"]*\)"/\1/p' ./build/fw_version.h)
RELEASE_FILE_NAME=cb_jtag_probe_$(FW_VERSION)_$(BOARD_NAME).uf2

release_bin:
	@echo "Creating release package $(RELEASE_FILE_NAME)..."
	@mkdir -p $(RELEASE_DIR)
	@cp ./build/zephyr/zephyr.uf2 $(RELEASE_DIR)/$(RELEASE_FILE_NAME)



copy_uf2_PRI-PR2:
	# copy the generated .uf2 file to the mounted RPI Pico
	cp ./build/zephyr/zephyr.uf2 /media/$(USER)/RPI-RP2/

copy_uf2_RP2350:
	# copy the generated .uf2 file to the mounted RPI Pico
	cp ./build/zephyr/zephyr.uf2 /media/$(USER)/RP2350/

