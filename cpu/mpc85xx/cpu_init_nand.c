/*
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

void cpu_init_f(void)
{
	ccsr_lbc_t *lbc = (void *)(CONFIG_SYS_MPC85xx_LBC_ADDR);

	/*
	 * LCRR - Clock Ratio Register - set up local bus timing
	 * when needed
	 */
	out_be32(&lbc->lcrr, LCRR_DBYP | LCRR_CLKDIV_8);

#if defined(CONFIG_NAND_BR_PRELIM) && defined(CONFIG_NAND_OR_PRELIM)
	out_be32(&lbc->br0, CONFIG_NAND_BR_PRELIM);
	out_be32(&lbc->or0, CONFIG_NAND_OR_PRELIM);
#else
#error  CONFIG_NAND_BR_PRELIM, CONFIG_NAND_OR_PRELIM must be defined
#endif

#if defined(CONFIG_SYS_RAMBOOT) && defined(CONFIG_SYS_INIT_L2_ADDR) \
	&& !defined(CONFIG_SRAM_BOOT)
	ccsr_l2cache_t *l2cache = (void *)CONFIG_SYS_MPC85xx_L2_ADDR;
	char *l2srbar;
	int i;

	out_be32(&l2cache->l2srbar0, CONFIG_SYS_INIT_L2_ADDR);

	/* set MBECCDIS=1, SBECCDIS=1 */
	out_be32(&l2cache->l2errdis,
		(MPC85xx_L2ERRDIS_MBECC | MPC85xx_L2ERRDIS_SBECC));

	/* set L2E=1 & L2SRAM=001 */
	out_be32(&l2cache->l2ctl,
		(MPC85xx_L2CTL_L2E | MPC85xx_L2CTL_L2SRAM_ENTIRE));

	/* Initialize L2 SRAM to zero */
	l2srbar = (char *)CONFIG_SYS_INIT_L2_ADDR;
	for (i = 0; i < CONFIG_SYS_L2_SIZE; i++)
		l2srbar[i] = 0;
#endif
}

/* Because the primary cpu's info is enough in NAND SPL and NAND SRAM
 * uboot, we define the cpu number to 1 so as to keep code size for spl
 * and sram stage uboot as small as possible.
 */
int cpu_numcores()
{
	return 1;
}

/*
 * Get timebase clock frequency
 */
unsigned long get_tbclk (void)
{
#ifdef CONFIG_FSL_CORENET
	return (gd->bus_clk + 8) / 16;
#else
	return (gd->bus_clk + 4UL)/8UL;
#endif
}
