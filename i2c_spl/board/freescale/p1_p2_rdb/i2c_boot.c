/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc.
 *
 * Authors:	Suresh PV <pala@freescale.com>,
 *		Kumar Gopalpet <Sandeep.Kumar@freescale.com>,
 *		Chandu Drona Pratap <D.P.Chandu@freescale.com>,
 *		Priyanka Jain <Priyanka.Jain@freescale.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <ns16550.h>

#define SYSCLK_MASK	0x00200000

#define SYSCLK_66	66666666
#define SYSCLK_100	100000000

extern flash_info_t flash_info[];       /* info for FLASH chips */

void board_init_f(ulong bootflag)
{
	uint plat_ratio, bus_clk, sys_clk = 0;
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	volatile ccsr_gpio_t *pgpio = (void *)(CONFIG_SYS_MPC85xx_GPIO_ADDR);
	uint val, sysclk_mask;

	val = pgpio->gpdat;
	sysclk_mask = val & SYSCLK_MASK;
	if (sysclk_mask == 0)
		sys_clk = SYSCLK_66;
	else
		sys_clk = SYSCLK_100;

	plat_ratio = gur->porpllsr & 0x0000003e;
	plat_ratio >>= 1;
	bus_clk = plat_ratio * sys_clk;
	NS16550_init((NS16550_t)CONFIG_SYS_NS16550_COM1,
			bus_clk / 16 / CONFIG_BAUDRATE);
	serial_puts("\n***Hello and welcome to I2C BOOTLOADER***\n");
}

int i2cboot_load_serial_bin(ulong offset)
{
	ulong addr;
	int rcode = 0;
	serial_puts("## Ready for binary (kermit) download ##\n");
	addr = do_load_serial_bin(offset);
	if (addr == offset)
		rcode = 1;
	serial_puts("\n## binary (kermit) download completed ##\n");
	return rcode;
}

void board_init_r(gd_t *gd, ulong dest_addr)
{
	ulong start_addr, sect_size;
	int i = 0, ret = 0;
	flash_info_t *info;

	i2cboot_load_serial_bin(CONFIG_SYS_LOAD_ADDR);
	serial_puts("Serial Download Done\n");

	flash_init();
	serial_puts("Flash Init Done\n");
	info = &flash_info[0];
	if (info->flash_id == FLASH_UNKNOWN) {
		serial_puts("unknown flash\n");
		return -1;
	} else
		serial_puts("flash detected\n");

	start_addr = info->start[0];
	sect_size = info->size/info->sector_count;

	for (i = 0; i < info->sector_count; i++)
		info->protect[i] = 0;

	/* erase the flash */
	ret = flash_erase(info, CONFIG_SYS_FLASH_UBOOT_OFFSET/sect_size,
			CONFIG_SYS_FLASH_UBOOT_OFFSET/sect_size + 4);
	if (ret != 0) {
		flash_perror(ret);
		serial_puts("The flash is bad and can't be erased\n");
		return -1;
	}
	serial_puts("\nflash erased\n");

	/* write to flash */
	ret = flash_write((char *)CONFIG_SYS_LOAD_ADDR,
			(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_UBOOT_OFFSET)\
			, 4*sect_size);
	if (ret != 0) {
		flash_perror(ret);
		serial_puts("Write error occured, flash cannot be written.\n");
		return -1;
	}
	serial_puts("\nu-boot written to flash\n");
	serial_puts("process completed, reboot the board in normal mode\n");
	while (1)
	;
}

ulong get_timer(ulong base)
{
	return 0;
}


int disable_interrupts(void)
{
	return 0;
}

void enable_interrupts(void)
{
	return ;
}

void putc(const char c)
{
	serial_putc(c);
}

void puts(const char *s)
{
	serial_puts(s);
}


int getc(void)
{
	/* Send directly to the handler */
	return serial_getc();
}

int tstc(void)
{
	/* Send directly to the handler */
	return serial_tstc();
}

inline int setenv(char *varname, char *varvalue)
{
	return;
}
