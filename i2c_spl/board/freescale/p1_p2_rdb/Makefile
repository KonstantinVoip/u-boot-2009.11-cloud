#
# Copyright 2009-2010 Freescale Semiconductor, Inc.
#
# Authors:    Suresh PV <pala@freescale.com>,
#             Kumar Gopalpet <Sandeep.Kumar@freescale.com>,
#             Chandu Drona Pratap <D.P.Chandu@freescale.com>,
#             Priyanka Jain <Priyanka.Jain@freescale.com>
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#


#########################################################################
# load configuration

include $(SRCTREE)/config.mk

#########################################################################
# U-Boot objects....order is important (i.e. start must be first)

OBJS  = start.o ppcstring.o

COBJS	= cmd_load.o  ns16550.o serial.o i2c_boot.o

COBJS	+= flash.o cfi_flash.o

#########################################################################
# create symbolic links for common files

$(obj)flash.c:
	@rm -f $(obj)flash.c
	ln -sf $(SRCTREE)/common/flash.c $(obj)flash.c

$(obj)cfi_flash.c:
	@rm -f $(obj)cfi_flash.c
	ln -sf $(SRCTREE)/drivers/mtd/cfi_flash.c $(obj)cfi_flash.c

$(obj)start.S:
	@rm -f $(obj)start.S
	ln -sf $(SRCTREE)/cpu/$(CPU)/start.S $(obj)start.S

$(obj)ns16550.c:
	@rm -f $(obj)ns16550.c
	ln -sf $(SRCTREE)/drivers/serial/ns16550.c $(obj)ns16550.c

$(obj)serial.c:
	@rm -f $(obj)serial.c
	ln -sf $(SRCTREE)/drivers/serial/serial.c $(obj)serial.c

$(obj)cmd_load.c:
	@rm -f $(obj)cmd_load.c
	ln -sf $(SRCTREE)/common/cmd_load.c $(obj)cmd_load.c

$(obj)ppcstring.S:
	@rm -f $(obj)ppcstring.S
	ln -sf $(SRCTREE)/lib_$(ARCH)/ppcstring.S $(obj)ppcstring.S

#################################################################

ALL =   bootloader.bin bootloader.dis i2c_prom.bin

all:		$(ALL)

bootloader.hex:	bootloader
		$(OBJCOPY) ${OBJCFLAGS} -O ihex $< $@

bootloader.srec:	bootloader
		$(OBJCOPY) ${OBJCFLAGS} -O srec $< $@

bootloader.bin:	bootloader
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@

reset.bin:	bootloader
		$(OBJCOPY) ${OBJCFLAGS} -O binary --only-section=.resetvec $< $@

bootloader.dis:	bootloader
		$(OBJDUMP) -d $< > $@

bootloader:	$(SUBDIRS) $(COBJS) $(OBJS) $(LDSCRIPT)
		UNDEF_SYM=`$(OBJDUMP) -x |sed  -n -e 's/.*\(__u_boot_cmd_.*\)/-u\1/p'|sort|uniq`;\
		$(LD) $(LDFLAGS) $$UNDEF_SYM $(OBJS) $(COBJS)\
			--start-group --end-group $(PLATFORM_LIBS) \
			-Map bootloader.map -o bootloader -g

$(SUBDIRS):
		$(MAKE) -C $@ all


depend dep:
		@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir .depend ; done

System.map:	bootloader
		@$(NM) $< | \
		grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
		sort > System.map

#########################################################################

clean:
	find . -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'  -o -name '*.a'  \) -print \
		| xargs rm -f
	rm -f bootloader bootloader.map bootloader.hex bootloader.bin bootloader.srec bootloader.dis reset.bin
	rm -f System.map
	rm -f cpu/$(CPU)/.depend
	@for dir in eeprom_array_tool; do $(MAKE) -C $$dir clean; done
	rm -rf gen_eeprom_array i2c_prom.bin
	@rm -f $(obj)flash.c
	@rm -f $(obj)cfi_flash.c
	@rm -f $(obj)ns16550.c
	@rm -f $(obj)serial.c
	@rm -f $(obj)ppcstring.S
	@rm -f $(obj)start.S
	@rm -f $(obj)cmd_load.c

#########################################################################
#generates the i2c_prom.bin

gen_eeprom_array:	bootloader
	@for dir in eeprom_array_tool; do $(MAKE) -C $$dir ; cp $$dir/$@ .; done

i2c_prom.bin:	gen_eeprom_array
		@./$<>$@
		@echo "" >&2
		@echo "" >&2
		@echo "              i2c_prom.bin  ready Flash it using Flash Programmer " >&2
