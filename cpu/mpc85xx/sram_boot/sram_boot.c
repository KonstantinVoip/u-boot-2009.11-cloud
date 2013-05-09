/*
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */
#include <common.h>
#include <mpc85xx.h>
#include <asm-ppc/io.h>
#include <ns16550.h>
#include <nand.h>
#include <asm/mmu.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_ddr_sdram.h>
#include <asm/fsl_law.h>

DECLARE_GLOBAL_DATA_PTR;

void board_init_f(ulong bootflag)
{
	uint plat_ratio, bus_clk, sys_clk = 0;
	ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);

	sys_clk = CONFIG_SYS_CLK_FREQ;

	plat_ratio = in_be32(&gur->porpllsr) & 0x0000003e;
	plat_ratio >>= 1;
	bus_clk = plat_ratio * sys_clk;
	get_clocks();

	NS16550_init((NS16550_t)CONFIG_SYS_NS16550_COM1,
			bus_clk / 16 / CONFIG_BAUDRATE);

	/* load environment */
#ifdef CONFIG_NAND_U_BOOT
	nand_load(CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE,
				(uchar *)CONFIG_ENV_ADDR);
#endif

#ifdef CONFIG_RAMBOOT_SPIFLASH
	espi_init();
	espi_load(CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE,
				(uchar *)CONFIG_ENV_ADDR);
#endif /* CONFIG_RAMBOOT_SPIFLASH */

#ifdef CONFIG_RAMBOOT_SDCARD
	esdhc_load();
#endif				/* CONFIG_RAMBOOT_SDCARD */

	gd->env_addr  = (ulong)(CONFIG_ENV_ADDR);
	gd->env_valid = 1;

	/* board specific DDR initialization */
	gd->ram_size = init_ddr_dram();
	puts("DRAM:");
	print_size (gd->ram_size, "");

	puts("\nSRAM boot (middle stage uboot in sram)... ");

	/*
	 * Load final image to DDR and let it run from there.
	 */
#ifdef CONFIG_NAND_U_BOOT
	nand_boot();
#endif

#ifdef CONFIG_RAMBOOT_SPIFLASH
       espi_boot();
#endif

#ifdef CONFIG_RAMBOOT_SDCARD
	esdhc_boot();
#endif
}

void putc(char c)
{
	if (c == '\n')
		NS16550_putc((NS16550_t)CONFIG_SYS_NS16550_COM1, '\r');

	NS16550_putc((NS16550_t)CONFIG_SYS_NS16550_COM1, c);
}

void puts(const char *str)
{
	while (*str)
		putc(*str++);
}
