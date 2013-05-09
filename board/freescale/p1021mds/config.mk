#
# Copyright (C) 2010 Freescale Semiconductor, Inc.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# p1021mds board
#

ifdef SRAM_BOOT
TEXT_BASE := 0xf8f81000
PAD_TO := 0xf8f8c000
endif

ifndef NAND_SPL
ifeq ($(CONFIG_MK_NAND), y)
ifndef SRAM_BOOT
TEXT_BASE = $(CONFIG_RAMBOOT_TEXT_BASE)
LDSCRIPT := $(TOPDIR)/cpu/$(CPU)/u-boot-nand.lds
endif
endif
endif

ifeq ($(CONFIG_MK_SDCARD), y)
TEXT_BASE = $(CONFIG_RAMBOOT_TEXT_BASE)
ifdef CONFIG_SYS_FSL_BOOT_DDR
RESET_VECTOR_ADDRESS = 0x1107fffc
else
RESET_VECTOR_ADDRESS = 0xf8fffffc
endif
endif

ifeq ($(CONFIG_MK_SPIFLASH), y)
TEXT_BASE = $(CONFIG_RAMBOOT_TEXT_BASE)
ifdef CONFIG_SYS_FSL_BOOT_DDR
RESET_VECTOR_ADDRESS = 0x1107fffc
else
RESET_VECTOR_ADDRESS = 0xf8fffffc
endif
endif

ifndef TEXT_BASE
TEXT_BASE = 0xeff80000
endif

ifndef RESET_VECTOR_ADDRESS
RESET_VECTOR_ADDRESS = 0xeffffffc
endif
