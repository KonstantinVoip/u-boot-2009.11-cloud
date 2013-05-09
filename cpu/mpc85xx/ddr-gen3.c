/*
 * Copyright 2008-2009 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/fsl_ddr_sdram.h>

#if (CONFIG_CHIP_SELECTS_PER_CTRL > 4)
#error Invalid setting for CONFIG_CHIP_SELECTS_PER_CTRL
#endif

static void fsl_ddr_set_workaround(volatile ccsr_ddr_t *ddr)
{
	volatile ccsr_local_ecm_t *ecm = (void *)CONFIG_SYS_MPC85xx_ECM_ADDR;
	unsigned int temp1, temp2, temp3;

	// 1. Set EEBACR[3] 
	setbits_be32(&ecm->eebacr, 0x10000000);
	debug("Setting EEBACR to 0x%08lx \n", in_be32(&ecm->eebacr));

	// 2. Set DINIT in SDRAM_CFG_2
	// Done 
	temp1 = in_be32(&ddr->sdram_cfg_2);
	debug("sdram_cfg_2 = 0x%x\n", temp1);

	// 3. Set DEBUG3[21] 
	setbits_be32(&ddr->debug_3, 0x400);
	debug("Setting D3[21] to 0x%08lx \n", in_be32(&ddr->debug_3));

	udelay(500);
	asm volatile("sync;isync");

	// Let the controller go 
	setbits_be32(&ddr->sdram_cfg, SDRAM_CFG_MEM_EN);
	debug("Enable DDR_SDRAM_CFG to 0x%x\n", in_be32(&ddr->sdram_cfg));

	// Poll DDR_SDRAM_CFG_2[D_INIT] bit until auto-data
	// init is done.
	
	while (in_be32(&ddr->sdram_cfg_2) & 0x10)
		udelay(10000);  // throttle polling rate //
	debug("Polling ing sdram_cfg_2 to %x\n", in_be32(&ddr->sdram_cfg_2));

	// 4. Clear DEBUG3[21] //
	clrbits_be32(&ddr->debug_3, 0x400);
	debug("Clearing D3[21] to 0x%08lx\n", in_be32(&ddr->debug_3));

	// CPO workaround starts //
	// A: Clear sdram_cfg_2[odt_cfg] //
	temp2 = in_be32(&ddr->sdram_cfg_2);
	clrbits_be32(&ddr->sdram_cfg_2, 0x600000);
	debug("Setting sdram_cfg_2 to 0x%08lx\n", in_be32(&ddr->sdram_cfg_2));

	// B: Set DEBUG1[15] //
	setbits_be32(&ddr->debug_1, 0x10000);
	debug("Setting D1[15] to 0x%08lx\n", in_be32(&ddr->debug_1));

	// C: Set timing_cfg_2[cpo] to 0b11111 //
	setbits_be32(&ddr->timing_cfg_2, 0xF800000);
	debug("Setting TMING_CFG_2[CPO] to 0x%08lx\n", in_be32(&ddr->timing_cfg_2));

	// D: Set D4 to 0x9f9f9f9f //
	temp3 = 0x9f9f9f9f;
	out_be32(&ddr->debug_4, temp3);
	debug("Setting D4 to 0x%08lx\n", in_be32(&ddr->debug_4));

	//E: Set D5 to 0x9f9f9f9f //
	temp3 = 0x9f9f9f9f;
	out_be32(&ddr->debug_5, temp3);
	debug("Setting D5 to 0x%08lx\n", in_be32(&ddr->debug_5));

	// F: Set D2[20] //
	setbits_be32(&ddr->debug_2, 0x800);
	debug("Setting D2[20] to 0x%08lx\n", in_be32(&ddr->debug_2));

	// G: Poll on D2[20] until cleared //
	while (in_be32(&ddr->debug_2) & 0x800)
		udelay(10000);          // throttle polling rate//

	// H: Clear D1[15] //
	clrbits_be32(&ddr->debug_1, 0x10000);
	debug("Setting D1[15] to 0x%08lx\n", in_be32(&ddr->debug_1));

	// I: Set sdram_cfg_2[odt_cfg] to 0b10 //
	out_be32(&ddr->sdram_cfg_2, temp2);
	debug("Setting sdram_cfg_2 to 0x%08lx\n", in_be32(&ddr->sdram_cfg_2));

	// Continuing with the MCKE workaround //
	// 4. Set D2[21] //
	setbits_be32(&ddr->debug_2, 0x400);
	debug("Setting D2[21] to 0x%08lx\n", in_be32(&ddr->debug_2));

	// 5. Poll D2[21] until its cleared //
	while (in_be32(&ddr->debug_2) & 0x400)
		udelay(10000);          //throttle polling rate //

	// 6. Wait for 400ms //
	udelay(400000);

	// 7. Set sdram_cfg_2[dinit] //
	out_be32(&ddr->sdram_cfg_2, temp1);
	debug("Setting sdram_cfg_2 to 0x%08lx \n", in_be32(&ddr->sdram_cfg_2));

	// 8. Poll until dinit is cleared //
	while (in_be32(&ddr->sdram_cfg_2) & 0x10)
		udelay(10000);

	// 9. Clear EEBACR[3] //
	clrbits_be32(&ecm->eebacr, 10000000);
	debug("Clearing EEBACR[3] to 0x%08lx \n", in_be32(&ecm->eebacr));
}

void fsl_ddr_set_memctl_regs(const fsl_ddr_cfg_regs_t *regs,unsigned int ctrl_num)
{
	unsigned int i;
	volatile ccsr_ddr_t *ddr;
	u32 temp_sdram_cfg;

	switch (ctrl_num) {
	case 0:
		ddr = (void *)CONFIG_SYS_MPC85xx_DDR_ADDR;
		break;
	case 1:
		ddr = (void *)CONFIG_SYS_MPC85xx_DDR2_ADDR;
		break;
	default:
		printf("%s unexpected ctrl_num = %u\n", __FUNCTION__, ctrl_num);
		return;
	}

	for (i = 0; i < CONFIG_CHIP_SELECTS_PER_CTRL; i++) {
		if (i == 0) {
			out_be32(&ddr->cs0_bnds, regs->cs[i].bnds);
			out_be32(&ddr->cs0_config, regs->cs[i].config);
			out_be32(&ddr->cs0_config_2, regs->cs[i].config_2);

		} else if (i == 1) {
			out_be32(&ddr->cs1_bnds, regs->cs[i].bnds);
			out_be32(&ddr->cs1_config, regs->cs[i].config);
			out_be32(&ddr->cs1_config_2, regs->cs[i].config_2);

		} else if (i == 2) {
			out_be32(&ddr->cs2_bnds, regs->cs[i].bnds);
			out_be32(&ddr->cs2_config, regs->cs[i].config);
			out_be32(&ddr->cs2_config_2, regs->cs[i].config_2);

		} else if (i == 3) {
			out_be32(&ddr->cs3_bnds, regs->cs[i].bnds);
			out_be32(&ddr->cs3_config, regs->cs[i].config);
			out_be32(&ddr->cs3_config_2, regs->cs[i].config_2);
		}
	}

	out_be32(&ddr->timing_cfg_3, regs->timing_cfg_3);
	out_be32(&ddr->timing_cfg_0, regs->timing_cfg_0);
	out_be32(&ddr->timing_cfg_1, regs->timing_cfg_1);
	out_be32(&ddr->timing_cfg_2, regs->timing_cfg_2);
	out_be32(&ddr->sdram_cfg_2, regs->ddr_sdram_cfg_2);
	out_be32(&ddr->sdram_mode, regs->ddr_sdram_mode);
	out_be32(&ddr->sdram_mode_2, regs->ddr_sdram_mode_2);
	out_be32(&ddr->sdram_md_cntl, regs->ddr_sdram_md_cntl);
	out_be32(&ddr->sdram_interval, regs->ddr_sdram_interval);
	out_be32(&ddr->sdram_data_init, regs->ddr_data_init);
	out_be32(&ddr->sdram_clk_cntl, regs->ddr_sdram_clk_cntl);
	out_be32(&ddr->init_addr, regs->ddr_init_addr);
	out_be32(&ddr->init_ext_addr, regs->ddr_init_ext_addr);

	out_be32(&ddr->timing_cfg_4, regs->timing_cfg_4);
	out_be32(&ddr->timing_cfg_5, regs->timing_cfg_5);
	out_be32(&ddr->ddr_zq_cntl, regs->ddr_zq_cntl);
	out_be32(&ddr->ddr_wrlvl_cntl, regs->ddr_wrlvl_cntl);
	out_be32(&ddr->ddr_sr_cntr, regs->ddr_sr_cntr);
	out_be32(&ddr->ddr_sdram_rcw_1, regs->ddr_sdram_rcw_1);
	out_be32(&ddr->ddr_sdram_rcw_2, regs->ddr_sdram_rcw_2);

	// Set, but do not enable the memory //
	temp_sdram_cfg = regs->ddr_sdram_cfg;
	temp_sdram_cfg &= ~(SDRAM_CFG_MEM_EN);
	out_be32(&ddr->sdram_cfg, temp_sdram_cfg);
	//
	 // For 8572 DDR1 erratum - DDR controller may enter illegal state
	// when operatiing in 32-bit bus mode with 4-beat bursts,
	 // This erratum does not affect DDR3 mode, only for DDR2 mode.
	 //
#ifdef CONFIG_MPC8572
	if ((((in_be32(&ddr->sdram_cfg) >> 24) & 0x7) == SDRAM_TYPE_DDR2)
	    && in_be32(&ddr->sdram_cfg) & 0x80000) {
		// set DEBUG_1[31] //
		u32 temp = in_be32(&ddr->debug_1);
		out_be32(&ddr->debug_1, temp | 1);
	}
#endif

#ifdef CONFIG_DDR_800MHZ
	// Set SBE //
	out_be32(&ddr->err_sbe, CONFIG_SYS_DDR_SBE);
	debug("Setting ERR_SBE to 0x%x\n", in_be32(&ddr->err_sbe));

	// Set ECC_EN //
	temp_sdram_cfg = in_be32(&ddr->sdram_cfg);
	temp_sdram_cfg |= CONFIG_SYS_ECC_EN;
	out_be32(&ddr->sdram_cfg, temp_sdram_cfg);
	debug("Enable ECC_EN to 0x%x\n", in_be32(&ddr->sdram_cfg));

#endif
	fsl_ddr_set_workaround(ddr);
}
