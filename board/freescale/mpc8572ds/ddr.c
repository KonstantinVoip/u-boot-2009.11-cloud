/*
 * Copyright 2008-2009 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 */

#include <common.h>
#include <i2c.h>

#include <asm/fsl_ddr_sdram.h>
#include <asm/fsl_ddr_dimm_params.h>
#include <asm/fsl_law.h>

/* These are used when DDR doesn't use SPD.  */
#ifdef CONFIG_DDR_800MHZ
#define CONFIG_SYS_SDRAM_SIZE		0x200000000ull  /* DDR is 8GB */
#define CONFIG_SYS_DDR_CS0_0_BNDS	0x0000007f
#define CONFIG_SYS_DDR_CS0_1_BNDS	0x008000ff
#define CONFIG_SYS_DDR_CS0_0_CONFIG	0x80014302
#define CONFIG_SYS_DDR_CS0_1_CONFIG	0x80014302
#define CONFIG_SYS_DDR_CS0_0_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_CS0_1_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_CS1_0_BNDS	0x0100017f
#define CONFIG_SYS_DDR_CS1_1_BNDS	0x018001ff
#define CONFIG_SYS_DDR_CS1_0_CONFIG	0x80014302
#define CONFIG_SYS_DDR_CS1_1_CONFIG	0x80014302
#define CONFIG_SYS_DDR_CS1_0_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_CS1_1_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_TIMING_3		0x00040000
#define CONFIG_SYS_DDR_TIMING_0		0xff220a02
#define CONFIG_SYS_DDR_TIMING_1		0x626b7634
#define CONFIG_SYS_DDR_TIMING_2		0x06a874cF
#define CONFIG_SYS_DDR_SDRAM_CFG	0x43008000
#define CONFIG_SYS_DDR_SDRAM_CFG_2	0x24401010
#define CONFIG_SYS_DDR_MODE_1		0x00040a62
#define CONFIG_SYS_DDR_MODE_2		0x00000000
#define CONFIG_SYS_DDR_MD_CRTL		0x00000000
#define CONFIG_SYS_DDR_INTERVAL		0x0c300100
#define CONFIG_SYS_DDR_DATA_INIT	0xdeadbeef
#define CONFIG_SYS_DDR_CLK_CTRL		0x04000000
#define CONFIG_SYS_DDR_INIT_ADDR	0x00000000
#define CONFIG_SYS_DDR_INIT_EXT_ADDR	0x00000000
#define CONFIG_SYS_DDR_TIMING_4		0x88880000
#define CONFIG_SYS_DDR_TIMING_5		0x00000000
#define CONFIG_SYS_DDR_ZQ_CNTL		0x00000000
#define CONFIG_SYS_DDR_WRLVL_CNTL	0x00000000
#define CONFIG_SYS_DDR_PD_CNTL		0x00000000
#define CONFIG_SYS_DDR_SR_CNTL		0x00000000
#define CONFIG_SYS_DDR_SDRAM_RCW_1	0x00000000
#define CONFIG_SYS_DDR_SDRAM_RCW_2	0x00000000
#endif

static void get_spd(ddr2_spd_eeprom_t *spd, unsigned char i2c_address)
{
	i2c_read(i2c_address, 0, 1, (uchar *)spd, sizeof(ddr2_spd_eeprom_t));
}

unsigned int fsl_ddr_get_mem_data_rate(void)
{
	return get_ddr_freq(0);
}

void fsl_ddr_get_spd(ddr2_spd_eeprom_t *ctrl_dimms_spd,
		      unsigned int ctrl_num)
{
	unsigned int i;
	unsigned int i2c_address = 0;

	for (i = 0; i < CONFIG_DIMM_SLOTS_PER_CTLR; i++) {
		if (ctrl_num == 0 && i == 0) {
			i2c_address = SPD_EEPROM_ADDRESS1;
		}
		if (ctrl_num == 1 && i == 0) {
			i2c_address = SPD_EEPROM_ADDRESS2;
		}
		get_spd(&(ctrl_dimms_spd[i]), i2c_address);
	}
}

typedef struct {
	u32 datarate_mhz_low;
	u32 datarate_mhz_high;
	u32 n_ranks;
	u32 clk_adjust;
	u32 cpo;
	u32 write_data_delay;
	u32 force_2T;
} board_specific_parameters_t;

/* ranges for parameters:
 *  wr_data_delay = 0-6
 *  clk adjust = 0-8
 *  cpo 2-0x1E (30)
 */


/* XXX: these values need to be checked for all interleaving modes.  */
/* XXX: No reliable dual-rank 800 MHz setting has been found.  It may
 *      seem reliable, but errors will appear when memory intensive
 *      program is run. */
/* XXX: Single rank at 800 MHz is OK.  */
const board_specific_parameters_t board_specific_parameters[][20] = {
	{
	/* 	memory controller 0 			*/
	/*	  lo|  hi|  num|  clk| cpo|wrdata|2T	*/
	/*	 mhz| mhz|ranks|adjst|    | delay|	*/
		{  0, 333,    2,    6,   7,    3,  0},
		{334, 400,    2,    6,   9,    3,  0},
		{401, 549,    2,    6,  11,    3,  0},
		{550, 680,    2,    1,  10,    5,  0},
		{681, 850,    2,    8,  12,    5,  1},
		{  0, 333,    1,    6,   7,    3,  0},
		{334, 400,    1,    6,   9,    3,  0},
		{401, 549,    1,    6,  11,    3,  0},
		{550, 680,    1,    1,  10,    5,  0},
		{681, 850,    1,    1,  12,    5,  0}
	},

	{
	/*	memory controller 1			*/
	/*	  lo|  hi|  num|  clk| cpo|wrdata|2T	*/
	/*	 mhz| mhz|ranks|adjst|    | delay|	*/
		{  0, 333,    2,     6,  7,    3,  0},
		{334, 400,    2,     6,  9,    3,  0},
		{401, 549,    2,     6, 11,    3,  0},
		{550, 680,    2,     1, 11,    6,  0},
		{681, 850,    2,     8, 13,    5,  1},
		{  0, 333,    1,     6,  7,    3,  0},
		{334, 400,    1,     6,  9,    3,  0},
		{401, 549,    1,     6, 11,    3,  0},
		{550, 680,    1,     1, 11,    6,  0},
		{681, 850,    1,     1, 13,    6,  0}
	}
};

void fsl_ddr_board_options(memctl_options_t *popts,
				dimm_params_t *pdimm,
				unsigned int ctrl_num)
{
	const board_specific_parameters_t *pbsp =
				&(board_specific_parameters[ctrl_num][0]);
	u32 num_params = sizeof(board_specific_parameters[ctrl_num]) /
				sizeof(board_specific_parameters[0][0]);
	u32 i;
	ulong ddr_freq;

	/* set odt_rd_cfg and odt_wr_cfg. If the there is only one dimm in
	 * that controller, set odt_wr_cfg to 4 for CS0, and 0 to CS1. If
	 * there are two dimms in the controller, set odt_rd_cfg to 3 and
	 * odt_wr_cfg to 3 for the even CS, 0 for the odd CS.
	 */
	for (i = 0; i < CONFIG_CHIP_SELECTS_PER_CTRL; i++) {
		if (i&1) {	/* odd CS */
			popts->cs_local_opts[i].odt_rd_cfg = 0;
			popts->cs_local_opts[i].odt_wr_cfg = 0;
		} else {	/* even CS */
			if (CONFIG_DIMM_SLOTS_PER_CTLR == 1) {
				popts->cs_local_opts[i].odt_rd_cfg = 0;
				popts->cs_local_opts[i].odt_wr_cfg = 4;
			} else if (CONFIG_DIMM_SLOTS_PER_CTLR == 2) {
			popts->cs_local_opts[i].odt_rd_cfg = 3;
			popts->cs_local_opts[i].odt_wr_cfg = 3;
			}
		}
	}

	/* Get clk_adjust, cpo, write_data_delay,2T, according to the board ddr
	 * freqency and n_banks specified in board_specific_parameters table.
	 */
	ddr_freq = get_ddr_freq(0) / 1000000;
	for (i = 0; i < num_params; i++) {
		if (ddr_freq >= pbsp->datarate_mhz_low &&
		    ddr_freq <= pbsp->datarate_mhz_high &&
		    pdimm->n_ranks == pbsp->n_ranks) {
			popts->clk_adjust = pbsp->clk_adjust;
			popts->cpo_override = pbsp->cpo;
			popts->write_data_delay = pbsp->write_data_delay;
			popts->twoT_en = pbsp->force_2T;
		}
		pbsp++;
	}

	/*
	 * Factors to consider for half-strength driver enable:
	 *	- number of DIMMs installed
	 */
	popts->half_strength_driver_enable = 0;
}

#if !defined(CONFIG_SPD_EEPROM)
/*
 * Fixed sdram init -- doesn't use serial presence detect.
 */

phys_size_t fixed_sdram (void)
{
	fsl_ddr_cfg_regs_t regs;
	unsigned int i, ctrl_num;
	unsigned long long base, size;

	memset(&regs, 0, sizeof(fsl_ddr_cfg_regs_t));
	for (i = 0; i < CONFIG_NUM_DDR_CONTROLLERS; i++) {
		ctrl_num = i;
		if (ctrl_num == 0) {
			regs.cs[0].bnds = CONFIG_SYS_DDR_CS0_0_BNDS;
			regs.cs[0].config = CONFIG_SYS_DDR_CS0_0_CONFIG;
			regs.cs[0].config_2 = CONFIG_SYS_DDR_CS0_0_CONFIG_2;
			regs.cs[1].bnds = CONFIG_SYS_DDR_CS0_1_BNDS;
			regs.cs[1].config = CONFIG_SYS_DDR_CS0_1_CONFIG;
			regs.cs[1].config_2 = CONFIG_SYS_DDR_CS0_1_CONFIG_2;
		} else if (ctrl_num == 1) {
			regs.cs[0].bnds = CONFIG_SYS_DDR_CS1_0_BNDS;
			regs.cs[0].config = CONFIG_SYS_DDR_CS1_0_CONFIG;
			regs.cs[0].config_2 = CONFIG_SYS_DDR_CS1_0_CONFIG_2;
			regs.cs[1].bnds = CONFIG_SYS_DDR_CS1_1_BNDS;
			regs.cs[1].config = CONFIG_SYS_DDR_CS1_1_CONFIG;
			regs.cs[1].config_2 = CONFIG_SYS_DDR_CS1_1_CONFIG_2;
		}

		regs.timing_cfg_3 = CONFIG_SYS_DDR_TIMING_3;
		regs.timing_cfg_0 = CONFIG_SYS_DDR_TIMING_0;
		regs.timing_cfg_1 = CONFIG_SYS_DDR_TIMING_1;
		regs.timing_cfg_2 = CONFIG_SYS_DDR_TIMING_2;
		regs.ddr_sdram_cfg = CONFIG_SYS_DDR_SDRAM_CFG;
		regs.ddr_sdram_cfg_2 = CONFIG_SYS_DDR_SDRAM_CFG_2;
		regs.ddr_sdram_mode = CONFIG_SYS_DDR_MODE_1;
		regs.ddr_sdram_mode_2 = CONFIG_SYS_DDR_MODE_2;
		regs.ddr_sdram_md_cntl = CONFIG_SYS_DDR_MD_CRTL;
		regs.ddr_sdram_interval = CONFIG_SYS_DDR_INTERVAL;
		regs.ddr_data_init = CONFIG_SYS_DDR_DATA_INIT;
		regs.ddr_sdram_clk_cntl = CONFIG_SYS_DDR_CLK_CTRL;
		regs.ddr_init_addr = CONFIG_SYS_DDR_INIT_ADDR;
		regs.ddr_init_ext_addr = CONFIG_SYS_DDR_INIT_EXT_ADDR;
		regs.timing_cfg_4 = CONFIG_SYS_DDR_TIMING_4;
		regs.timing_cfg_5 = CONFIG_SYS_DDR_TIMING_5;
		regs.ddr_zq_cntl = CONFIG_SYS_DDR_ZQ_CNTL;
		regs.ddr_wrlvl_cntl = CONFIG_SYS_DDR_WRLVL_CNTL;
		regs.ddr_pd_cntl = CONFIG_SYS_DDR_PD_CNTL;
		regs.ddr_sr_cntr = CONFIG_SYS_DDR_SR_CNTL;
		regs.ddr_sdram_rcw_1 = CONFIG_SYS_DDR_SDRAM_RCW_1;
		regs.ddr_sdram_rcw_2 = CONFIG_SYS_DDR_SDRAM_RCW_2;

		fsl_ddr_set_memctl_regs(&regs, ctrl_num);
	}

	base = 0x0;
	size = CONFIG_SYS_SDRAM_SIZE/2;
	set_ddr_laws(base, size, LAW_TRGT_IF_DDR_1);
	base += size;
	set_ddr_laws(base, size, LAW_TRGT_IF_DDR_2);

	return CONFIG_SYS_SDRAM_SIZE;
}
#endif
