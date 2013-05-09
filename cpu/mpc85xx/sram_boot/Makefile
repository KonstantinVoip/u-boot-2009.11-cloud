#
# Copyright (C) 2010 Freescale Semiconductor, Inc.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#

SRAM_BOOT := y

include $(TOPDIR)/config.mk

LDSCRIPT= $(TOPDIR)/cpu/$(CPU)/sram_boot/u-boot-sram-boot.lds
LDFLAGS	= -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE) $(PLATFORM_LDFLAGS)
AFLAGS	+= -DCONFIG_SRAM_BOOT
CFLAGS	+= -DCONFIG_SRAM_BOOT

SOBJS	= start.o ticks.o ppcstring.o
COBJS	= cache.o cpu_init_early.o cpu_init_nand.o fsl_law.o law.o speed.o \
	  sram_boot.o ns16550.o tlb.o tlb_table.o string.o \
	  ddr.o time.o ddr-gen3.o ddr_spd.o ctype.o div64.o console.o \
	  cmd_nvedit.o env_common.o env_nand.o vsprintf.o display_options.o

ifdef CONFIG_RAMBOOT_NAND
COBJS += nand_boot_fsl_elbc.o
endif

ifdef CONFIG_RAMBOOT_SPIFLASH
COBJS += fsl_espi.o
endif

ifdef CONFIG_RAMBOOT_SDCARD
COBJS += fsl_esdhc.o
endif

LIBS = $(TOPDIR)/cpu/mpc8xxx/ddr/libddr.a
LIBS += $(TOPDIR)/drivers/i2c/libi2c.a


SRCS	:= $(addprefix $(obj),$(SOBJS:.o=.S) $(COBJS:.o=.c))
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
__OBJS	:= $(SOBJS) $(COBJS)
LNDIR	:= $(OBJTREE)/cpu/$(CPU)/sram_boot

sramobj	:= $(OBJTREE)/cpu/$(CPU)/

ALL	= $(sramobj)u-boot-sram $(sramobj)u-boot-sram.bin

all:	$(obj).depend $(ALL)

$(sramobj)u-boot-sram.bin: $(sramobj)u-boot-sram
	$(OBJCOPY) ${OBJCFLAGS} --pad-to=$(PAD_TO) -O binary $< $@

$(sramobj)u-boot-sram:	$(OBJS) $(LIBS)
	cd $(LNDIR) && $(LD) $(LDFLAGS) $(__OBJS) $(LIBS) $(PLATFORM_LIBS) \
		-Map $(sramobj)u-boot-sram.map \
		-o $(sramobj)u-boot-sram

# create symbolic links for common files

$(obj)cache.c:
	@rm -f $(obj)cache.c
	ln -sf $(SRCTREE)/lib_ppc/cache.c $(obj)cache.c

$(obj)cpu_init_early.c:
	@rm -f $(obj)cpu_init_early.c
	ln -sf $(SRCTREE)/cpu/mpc85xx/cpu_init_early.c $(obj)cpu_init_early.c

$(obj)cpu_init_nand.c:
	@rm -f $(obj)cpu_init_nand.c
	ln -sf $(SRCTREE)/cpu/mpc85xx/cpu_init_nand.c $(obj)cpu_init_nand.c

$(obj)speed.c:
	@rm -f $(obj)speed.c
	ln -sf $(SRCTREE)/cpu/mpc85xx/speed.c $(obj)speed.c

$(obj)fsl_law.c:
	@rm -f $(obj)fsl_law.c
	ln -sf $(SRCTREE)/drivers/misc/fsl_law.c $(obj)fsl_law.c

$(obj)law.c:
	@rm -f $(obj)law.c
	ln -sf $(SRCTREE)/board/$(BOARDDIR)/law.c $(obj)law.c

ifdef  CONFIG_RAMBOOT_NAND
$(obj)nand_boot_fsl_elbc.c:
	@rm -f $(obj)nand_boot_fsl_elbc.c
	ln -sf $(SRCTREE)/nand_spl/nand_boot_fsl_elbc.c \
	       $(obj)nand_boot_fsl_elbc.c
endif

$(obj)ns16550.c:
	@rm -f $(obj)ns16550.c
	ln -sf $(SRCTREE)/drivers/serial/ns16550.c $(obj)ns16550.c

$(obj)fixed_ivor.S:
	@rm -f $(obj)fixed_ivor.S
	ln -sf $(SRCTREE)/cpu/mpc85xx/fixed_ivor.S $(obj)fixed_ivor.S

$(obj)start.S: $(obj)fixed_ivor.S
	@rm -f $(obj)start.S
	ln -sf $(SRCTREE)/cpu/mpc85xx/start.S $(obj)start.S

$(obj)tlb.c:
	@rm -f $(obj)tlb.c
	ln -sf $(SRCTREE)/cpu/mpc85xx/tlb.c $(obj)tlb.c

$(obj)tlb_table.c:
	@rm -f $(obj)tlb_table.c
	ln -sf $(SRCTREE)/board/$(BOARDDIR)/tlb.c $(obj)tlb_table.c

$(obj)ddr.c:
	@rm -f $(obj)ddr.c
	ln -sf $(SRCTREE)/board/$(BOARDDIR)/ddr.c $(obj)ddr.c

$(obj)ticks.S:
	@rm -f $(obj)ticks.S
	ln -sf $(SRCTREE)/lib_ppc/ticks.S $(obj)ticks.S

$(obj)time.c:
	@rm -f $(obj)time.o
	ln -sf $(SRCTREE)/lib_ppc/time.c $(obj)time.c

$(obj)ppcstring.S:
	@rm -f $(obj)ppcstring.S
	ln -sf $(SRCTREE)/lib_ppc/ppcstring.S $(obj)ppcstring.S

$(obj)ddr-gen3.c:
	@rm -f $(obj)ddr-gen3.c
	ln -sf $(SRCTREE)/cpu/mpc85xx/ddr-gen3.c $(obj)ddr-gen3.c

$(obj)ddr_spd.c:
	@rm -f $(obj)ddr_spd.c
	ln -sf $(SRCTREE)/common/ddr_spd.c $(obj)ddr_spd.c

$(obj)ctype.c:
	@rm -f $(obj)ctype.c
	ln -sf $(SRCTREE)/lib_generic/ctype.c $(obj)ctype.c

$(obj)div64.c:
	@rm -f $(obj)div64.c
	ln -sf $(SRCTREE)/lib_generic/div64.c $(obj)div64.c

$(obj)env_common.c:
	@rm -f $(obj)env_common.c
	ln -sf $(SRCTREE)/common/env_common.c $(obj)env_common.c

$(obj)env_nand.c:
	@rm -f $(obj)env_nand.c
	ln -sf $(SRCTREE)/common/env_nand.c $(obj)env_nand.c

$(obj)cmd_nvedit.c:
	@rm -f $(obj)cmd_nvedit.c
	ln -sf $(SRCTREE)/common/cmd_nvedit.c $(obj)cmd_nvedit.c

$(obj)console.c:
	@rm -f $(obj)console.c
	ln -sf $(SRCTREE)/common/console.c $(obj)console.c

$(obj)string.c:
	@rm -f $(obj)string.c
	ln -sf $(SRCTREE)/lib_generic/string.c $(obj)string.c

$(obj)vsprintf.c:
	@rm -f $(obj)vsprintf.c
	ln -sf $(SRCTREE)/lib_generic/vsprintf.c $(obj)vsprintf.c

$(obj)display_options.c:
	@rm -f $(obj)display_options.c
	ln -sf $(SRCTREE)/lib_generic/display_options.c $(obj)display_options.c

ifneq ($(OBJTREE), $(SRCTREE))
$(obj)sram_boot.c:
	@rm -f $(obj)sram_boot.c
	ln -s $(SRCTREE)/cpu/$(CPU)/sram_boot/sram_boot.c $(obj)sram_boot.c
endif

#########################################################################

$(obj)%.o:	$(obj)%.S
	$(CC) $(AFLAGS) -c -o $@ $<

$(obj)%.o:	$(obj)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
