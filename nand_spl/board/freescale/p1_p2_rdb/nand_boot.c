/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
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

#define SYSCLK_MASK     0x00200000

#define SYSCLK_66       66666666
#define SYSCLK_100      100000000

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_SYS_FSL_BOOT_DDR)
#define CONFIG_SYS_DDR_CS0_BNDS		0x0000003F
#define CONFIG_SYS_DDR_CS0_CONFIG	0x80014202
#define CONFIG_SYS_DDR_CS0_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_CONTROL          0x43000000      /* Type = DDR2*/
#define CONFIG_SYS_DDR_CONTROL_2        0x24401000

#define CONFIG_SYS_DDR_TIMING_3_667	0x00030000
#define CONFIG_SYS_DDR_TIMING_0_667	0x55770802
#define CONFIG_SYS_DDR_TIMING_1_667	0x5f599543
#define CONFIG_SYS_DDR_TIMING_2_667	0x0fa074d1
#define CONFIG_SYS_DDR_CLK_CTRL_667	0x03000000
#define CONFIG_SYS_DDR_MODE_1_667	0x00040852
#define CONFIG_SYS_DDR_MODE_2_667	0x00000000
#define CONFIG_SYS_DDR_INTERVAL_667	0x0a280100
#define SDRAM_CFG_32_BE			0x00080000

#define udelay(x) {int i, j; for (i = 0; i < x; i++) for (j = 0; j < 10000; j++); }

void initsdram(void)
{

	volatile ccsr_ddr_t *ddr= (ccsr_ddr_t *)CONFIG_SYS_MPC85xx_DDR_ADDR;
	int d_init, dbw;
	volatile ccsr_gpio_t *pgpio = (void *)(CONFIG_SYS_MPC85xx_GPIO_ADDR);
	unsigned int ddr_size;
	sys_info_t sysinfo;
	phys_size_t dram_size = 0;

	set_law(CONFIG_SYS_NAND_DDR_LAW, 0, LAW_SIZE_1G , LAW_TRGT_IF_DDR_1);

	out_be32(&ddr->cs0_bnds, CONFIG_SYS_DDR_CS0_BNDS);
	out_be32(&ddr->cs0_config, CONFIG_SYS_DDR_CS0_CONFIG);
	out_be32(&ddr->cs0_config_2, CONFIG_SYS_DDR_CS0_CONFIG_2);

	out_be32(&ddr->timing_cfg_3, CONFIG_SYS_DDR_TIMING_3_667);
	out_be32(&ddr->timing_cfg_0, CONFIG_SYS_DDR_TIMING_0_667);
	out_be32(&ddr->timing_cfg_1, CONFIG_SYS_DDR_TIMING_1_667);
	out_be32(&ddr->timing_cfg_2, CONFIG_SYS_DDR_TIMING_2_667);
	out_be32(&ddr->sdram_mode, CONFIG_SYS_DDR_MODE_1_667);
	out_be32(&ddr->sdram_mode_2, CONFIG_SYS_DDR_MODE_2_667);
	out_be32(&ddr->sdram_interval, CONFIG_SYS_DDR_INTERVAL_667);
	out_be32(&ddr->sdram_clk_cntl, CONFIG_SYS_DDR_CLK_CTRL_667);

#if defined(CONFIG_P2020) || defined(CONFIG_P2010)
	out_be32(&ddr->sdram_cfg, CONFIG_SYS_DDR_CONTROL);
#elif defined(CONFIG_P1020) || defined(CONFIG_P1011)
	out_be32(&ddr->sdram_cfg, CONFIG_SYS_DDR_CONTROL | SDRAM_CFG_32_BE);
#endif

	out_be32(&ddr->sdram_cfg_2, CONFIG_SYS_DDR_CONTROL_2);

	asm("sync;isync");

	udelay(500);

	ddr->sdram_cfg |= 0x80000000;

}
#endif

void board_init_f(ulong bootflag)
{
	uint plat_ratio, bus_clk, sys_clk = 0;
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	volatile ccsr_gpio_t *pgpio = (void *)(CONFIG_SYS_MPC85xx_GPIO_ADDR);
	uint val, sysclk_mask;

	val = pgpio->gpdat;
	sysclk_mask = val & SYSCLK_MASK;
	if(sysclk_mask == 0)
		sys_clk = SYSCLK_66;
	else
		sys_clk = SYSCLK_100;

	plat_ratio = gur->porpllsr & 0x0000003e;
	plat_ratio >>= 1;
	bus_clk = plat_ratio * sys_clk;
	NS16550_init((NS16550_t)CONFIG_SYS_NS16550_COM1,
			bus_clk / 16 / CONFIG_BAUDRATE);

	puts("\nNAND boot... ");

#ifdef CONFIG_SYS_FSL_BOOT_DDR
	/* board specific DDR initialization */
	initsdram();
#endif

	/* copy code to DDR and jump to it - this should not return */
	/* NOTE - code has to be copied out of NAND buffer before
	 * other blocks can be read.
	 */
	relocate_code(CONFIG_SYS_NAND_U_BOOT_RELOC_SP, 0,
			CONFIG_SYS_NAND_U_BOOT_RELOC);
}

void board_init_r(gd_t *gd, ulong dest_addr)
{
	nand_boot();
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
