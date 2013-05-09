/*
 * Copyright 2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 */

#include <common.h>
#include <i2c.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/fsl_law.h>
#include <asm/fsl_ddr_sdram.h>
#include <asm/fsl_ddr_dimm_params.h>

DECLARE_GLOBAL_DATA_PTR;

unsigned int fsl_ddr_get_mem_data_rate(void)
{
	return get_ddr_freq(0);
}

void fsl_ddr_get_spd(ddr3_spd_eeprom_t *ctrl_dimms_spd, unsigned int ctrl_num)
{
	int ret;

	/*
	 * The P1021 only has one DDR controller, and the P1021MDS board has
	 * only one DIMM slot.
	 */

	ret = i2c_read(SPD_EEPROM_ADDRESS1, 0, 1, (u8 *)ctrl_dimms_spd,
			sizeof(ddr3_spd_eeprom_t));

	if (ret) {
		debug("DDR: failed to read SPD from address %u\n",
			SPD_EEPROM_ADDRESS1);
		memset(ctrl_dimms_spd, 0, sizeof(ddr3_spd_eeprom_t));
	}
}

void fsl_ddr_board_options(memctl_options_t *popts,
				dimm_params_t *pdimm,
				unsigned int ctrl_num)
{
	/*
	 * Factors to consider for clock adjust:
	 */
	popts->clk_adjust = 6;

	/*
	 * Factors to consider for CPO:
	 */
	popts->cpo_override = 0x1f;

	/*
	 * Factors to consider for write data delay:
	 */
	popts->write_data_delay = 2;

	/*
	 * Factors to consider for half-strength driver enable:
	 */
	popts->half_strength_driver_enable = 0;

	/*
	 * P1021 supports max 32-bit DDR width
	 */
	popts->data_bus_width = 1;

	/*
	 * disable on-the-fly burst chop mode for 32 bit data bus
	 */
	popts->OTF_burst_chop_en = 0;

	/*
	 * Set fixed 8 beat burst for 32 bit data bus
	 */
	popts->burst_length = DDR_BL8;
}

phys_size_t fixed_sdram(void)
{
	ccsr_ddr_t *ddr = (ccsr_ddr_t *)CONFIG_SYS_MPC85xx_DDR_ADDR;
	u32 temp_sdram_cfg;

	set_next_law(0 , LAW_SIZE_512M , LAW_TRGT_IF_DDR_1);

	out_be32(&ddr->cs0_bnds, CONFIG_SYS_DDR_CS0_BNDS);
	out_be32(&ddr->cs0_config, CONFIG_SYS_DDR_CS0_CONFIG);
	out_be32(&ddr->cs0_config_2, CONFIG_SYS_DDR_CS0_CONFIG_2);
	out_be32(&ddr->sdram_cfg_2, CONFIG_SYS_DDR_CONTROL_2);
	out_be32(&ddr->ddr_zq_cntl, CONFIG_SYS_DDR_ZQ_CNTL);
	out_be32(&ddr->ddr_wrlvl_cntl, CONFIG_SYS_DDR_WRLVL_CNTL);
	out_be32(&ddr->sdram_data_init, CONFIG_SYS_DDR_DATA_INIT);
	out_be32(&ddr->ddr_cdr1, CONFIG_SYS_DDR_CDR_1);
	out_be32(&ddr->ddr_cdr2, CONFIG_SYS_DDR_CDR_1);
	out_be32(&ddr->timing_cfg_3, CONFIG_SYS_DDR_TIMING_3);
	out_be32(&ddr->timing_cfg_0, CONFIG_SYS_DDR_TIMING_0);
	out_be32(&ddr->timing_cfg_1, CONFIG_SYS_DDR_TIMING_1);
	out_be32(&ddr->timing_cfg_2, CONFIG_SYS_DDR_TIMING_2);
	out_be32(&ddr->sdram_mode, CONFIG_SYS_DDR_SDRAM_MODE);
	out_be32(&ddr->sdram_mode_2, CONFIG_SYS_DDR_SDRAM_MODE_2);
	out_be32(&ddr->sdram_interval, CONFIG_SYS_DDR_SDRAM_INTERVAL);
	out_be32(&ddr->sdram_clk_cntl, CONFIG_SYS_DDR_SDRAM_CLK_CNTL);
	out_be32(&ddr->timing_cfg_4, CONFIG_SYS_DDR_TIMING_4);
	out_be32(&ddr->timing_cfg_5, CONFIG_SYS_DDR_TIMING_5);
	out_be32(&ddr->sdram_cfg, CONFIG_SYS_DDR_CONTROL | SDRAM_CFG_32_BE);

	sync();
	isync();

	udelay(500);

	/* Let the controller go */
	temp_sdram_cfg = in_be32(&ddr->sdram_cfg);
	out_be32(&ddr->sdram_cfg, temp_sdram_cfg | SDRAM_CFG_MEM_EN);

	return 512 * 1024 * 1024;
}

phys_size_t init_ddr_dram(void)
{
	phys_size_t dram_size = 0;
#ifdef CONFIG_SPD_EEPROM
	dram_size = fsl_ddr_sdram();
#else
	dram_size = fixed_sdram();
#endif
	dram_size = setup_ddr_tlbs(dram_size / 0x100000);
	dram_size *= 0x100000;

	puts("\n    DDR: ");
	return dram_size;
}
