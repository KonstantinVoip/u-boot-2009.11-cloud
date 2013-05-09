/*
 * Copyright 2008-2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 */

#include <common.h>
#include <i2c.h>

#include <asm/fsl_ddr_sdram.h>
#include <asm/fsl_ddr_dimm_params.h>

static void get_spd(ddr3_spd_eeprom_t *spd, unsigned char i2c_address)
{
	i2c_read(i2c_address, 0, 1, (uchar *)spd, sizeof(ddr3_spd_eeprom_t));
}

unsigned int fsl_ddr_get_mem_data_rate(void)
{
	return get_ddr_freq(0);
}

void fsl_ddr_get_spd(ddr3_spd_eeprom_t *ctrl_dimms_spd,
		      unsigned int ctrl_num)
{
	unsigned int i;
	unsigned int i2c_address = 0;

	for (i = 0; i < CONFIG_DIMM_SLOTS_PER_CTLR; i++) {
		if (ctrl_num == 0 && i == 0)
			i2c_address = SPD_EEPROM_ADDRESS;
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
		{  0, 333,    2,    5,  31,    3,  0},
		{334, 400,    2,    5,  31,    3,  0},
		{401, 549,    2,    5,  31,    3,  0},
		{550, 680,    2,    5,  31,    5,  0},
		{681, 850,    2,    5,  31,    5,  0},
		{  0, 333,    1,    5,  31,    3,  0},
		{334, 400,    1,    5,  31,    3,  0},
		{401, 549,    1,    5,  31,    3,  0},
		{550, 680,    1,    5,  31,    5,  0},
		{681, 850,    1,    5,  31,    5,  0}
	},
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

	/* set odt_rd_cfg and odt_wr_cfg. */
	for (i = 0; i < CONFIG_CHIP_SELECTS_PER_CTRL; i++) {
			popts->cs_local_opts[i].odt_rd_cfg = 0;
			popts->cs_local_opts[i].odt_wr_cfg = 1;
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
	popts->half_strength_driver_enable = 1;

	/*
	 * For wake up arp feature, we need enable auto self refresh
	 */
	popts->auto_self_refresh_en = 1;
	popts->sr_it = 0xb;
}
