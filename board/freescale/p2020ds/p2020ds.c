/*
 * Copyright 2007-2009 Freescale Semiconductor, Inc.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <pci.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/cache.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_ddr_sdram.h>
#include <asm/io.h>
#include <miiphy.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <tsec.h>
#include <asm/fsl_law.h>
#include <asm/mp.h>
#include <netdev.h>
#include <mmc.h>
#include <malloc.h>
#include <asm/fsl_serdes.h>

#include "../common/pixis.h"
#include "../common/sgmii_riser.h"

DECLARE_GLOBAL_DATA_PTR;

phys_size_t fixed_sdram(void);

int board_early_init_f(void)
{
#ifdef CONFIG_MMC
	ccsr_gur_t *gur = (ccsr_gur_t *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);

	setbits_be32(&gur->pmuxcr,
			 (MPC85xx_PMUXCR_SDHC_CD |
			 MPC85xx_PMUXCR_SDHC_WP));
#endif

	return 0;
}

int checkboard(void)
{
	u8 sw7;
	u8 *pixis_base = (u8 *)PIXIS_BASE;

	puts("Board: P2020DS ");
#ifdef CONFIG_PHYS_64BIT
	puts("(36-bit addrmap) ");
#endif

	printf("Sys ID: 0x%02x, "
		"Sys Ver: 0x%02x, FPGA Ver: 0x%02x, ",
		in_8(pixis_base + PIXIS_ID), in_8(pixis_base + PIXIS_VER),
		in_8(pixis_base + PIXIS_PVER));

	sw7 = in_8(pixis_base + PIXIS_SW(7));
	switch ((sw7 & PIXIS_SW7_LBMAP) >> 6) {
		case 0:
		case 1:
			printf ("vBank: %d\n", ((sw7 & PIXIS_SW7_VBANK) >> 4));
			break;
		case 2:
		case 3:
			puts ("Promjet\n");
			break;
	}

	return 0;
}

phys_size_t initdram(int board_type)
{
	phys_size_t dram_size = 0;

	puts("Initializing....");

#ifdef CONFIG_SPD_EEPROM
	dram_size = fsl_ddr_sdram();
#else
	dram_size = fixed_sdram();

	if (set_ddr_laws(CONFIG_SYS_DDR_SDRAM_BASE,
			 dram_size,
			 LAW_TRGT_IF_DDR) < 0) {
		printf("ERROR setting Local Access Windows for DDR\n");
		return 0;
	};
#endif
	dram_size = setup_ddr_tlbs(dram_size / 0x100000);
	dram_size *= 0x100000;

	puts("    DDR: ");
	return dram_size;
}

#if !defined(CONFIG_SPD_EEPROM)
/*
 * Fixed sdram init -- doesn't use serial presence detect.
 */

phys_size_t fixed_sdram(void)
{
	volatile ccsr_ddr_t *ddr = (ccsr_ddr_t *)CONFIG_SYS_MPC85xx_DDR_ADDR;
	uint d_init;

	ddr->cs0_config = CONFIG_SYS_DDR_CS0_CONFIG;
	ddr->timing_cfg_3 = CONFIG_SYS_DDR_TIMING_3;
	ddr->timing_cfg_0 = CONFIG_SYS_DDR_TIMING_0;
	ddr->sdram_mode = CONFIG_SYS_DDR_MODE_1;
	ddr->sdram_mode_2 = CONFIG_SYS_DDR_MODE_2;
	ddr->sdram_md_cntl = CONFIG_SYS_DDR_MODE_CTRL;
	ddr->sdram_interval = CONFIG_SYS_DDR_INTERVAL;
	ddr->sdram_data_init = CONFIG_SYS_DDR_DATA_INIT;
	ddr->sdram_clk_cntl = CONFIG_SYS_DDR_CLK_CTRL;
	ddr->sdram_cfg_2 = CONFIG_SYS_DDR_CONTROL2;
	ddr->ddr_zq_cntl = CONFIG_SYS_DDR_ZQ_CNTL;
	ddr->ddr_wrlvl_cntl = CONFIG_SYS_DDR_WRLVL_CNTL;
	ddr->ddr_cdr1 = CONFIG_SYS_DDR_CDR1;
	ddr->timing_cfg_4 = CONFIG_SYS_DDR_TIMING_4;
	ddr->timing_cfg_5 = CONFIG_SYS_DDR_TIMING_5;

	if (!strcmp("performance", getenv("perf_mode"))) {
		/* Performance Mode Values */

		ddr->cs1_config = CONFIG_SYS_DDR_CS1_CONFIG_PERF;
		ddr->cs0_bnds = CONFIG_SYS_DDR_CS0_BNDS_PERF;
		ddr->cs1_bnds = CONFIG_SYS_DDR_CS1_BNDS_PERF;
		ddr->timing_cfg_1 = CONFIG_SYS_DDR_TIMING_1_PERF;
		ddr->timing_cfg_2 = CONFIG_SYS_DDR_TIMING_2_PERF;

		asm("sync;isync");

		udelay(500);

		ddr->sdram_cfg = CONFIG_SYS_DDR_CONTROL_PERF;
	} else {
		/* Stable Mode Values */

		ddr->cs1_config = CONFIG_SYS_DDR_CS1_CONFIG;
		ddr->cs0_bnds = CONFIG_SYS_DDR_CS0_BNDS;
		ddr->cs1_bnds = CONFIG_SYS_DDR_CS1_BNDS;
		ddr->timing_cfg_1 = CONFIG_SYS_DDR_TIMING_1;
		ddr->timing_cfg_2 = CONFIG_SYS_DDR_TIMING_2;

		/* ECC will be assumed in stable mode */
		ddr->err_int_en = CONFIG_SYS_DDR_ERR_INT_EN;
		ddr->err_disable = CONFIG_SYS_DDR_ERR_DIS;
		ddr->err_sbe = CONFIG_SYS_DDR_SBE;

		asm("sync;isync");

		udelay(500);

		ddr->sdram_cfg = CONFIG_SYS_DDR_CONTROL;
	}

#if defined(CONFIG_ECC_INIT_VIA_DDRCONTROLLER)
	d_init = 1;
	debug("DDR - 1st controller: memory initializing\n");
	/*
	 * Poll until memory is initialized.
	 * 512 Meg at 400 might hit this 200 times or so.
	 */
	while ((ddr->sdram_cfg_2 & (d_init << 4)) != 0)
		udelay(1000);
	debug("DDR: memory initialized\n\n");
	asm("sync; isync");
	udelay(500);
#endif

	return CONFIG_SYS_SDRAM_SIZE * 1024 * 1024;
}

#endif

#ifdef CONFIG_PCI
void pci_init_board(void)
{
	mpc85xx_serdes_board_init();
}
#endif

int board_early_init_r(void)
{
	const unsigned int flashbase = CONFIG_SYS_FLASH_BASE;
	const u8 flash_esel = 2;

	/*
	 * Remap Boot flash + PROMJET region to caching-inhibited
	 * so that flash can be erased properly.
	 */

	/* Flush d-cache and invalidate i-cache of any FLASH data */
	flush_dcache();
	invalidate_icache();

	/* invalidate existing TLB entry for flash + promjet */
	disable_tlb(flash_esel);

	set_tlb(1, flashbase, CONFIG_SYS_FLASH_BASE_PHYS,
			MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
			0, flash_esel, BOOKE_PAGESZ_256M, 1);

	return 0;
}

#ifdef CONFIG_GET_CLK_FROM_ICS307
/* decode S[0-2] to Output Divider (OD) */
static unsigned char ics307_S_to_OD[] = {
	10, 2, 8, 4, 5, 7, 3, 6
};

/* Calculate frequency being generated by ICS307-02 clock chip based upon
 * the control bytes being programmed into it. */
/* XXX: This function should probably go into a common library */
static unsigned long
ics307_clk_freq(unsigned char cw0, unsigned char cw1, unsigned char cw2)
{
	const unsigned long InputFrequency = CONFIG_ICS307_REFCLK_HZ;
	unsigned long VDW = ((cw1 << 1) & 0x1FE) + ((cw2 >> 7) & 1);
	unsigned long RDW = cw2 & 0x7F;
	unsigned long OD = ics307_S_to_OD[cw0 & 0x7];
	unsigned long freq;

	/* CLK1Frequency = InputFrequency * 2 * (VDW + 8) / ((RDW + 2) * OD) */

	/* cw0:  C1 C0 TTL F1 F0 S2 S1 S0
	 * cw1:  V8 V7 V6 V5 V4 V3 V2 V1
	 * cw2:  V0 R6 R5 R4 R3 R2 R1 R0
	 *
	 * R6:R0 = Reference Divider Word (RDW)
	 * V8:V0 = VCO Divider Word (VDW)
	 * S2:S0 = Output Divider Select (OD)
	 * F1:F0 = Function of CLK2 Output
	 * TTL = duty cycle
	 * C1:C0 = internal load capacitance for cyrstal
	 */

	/* Adding 1 to get a "nicely" rounded number, but this needs
	 * more tweaking to get a "properly" rounded number. */

	freq = 1 + (InputFrequency * 2 * (VDW + 8) / ((RDW + 2) * OD));

	debug("ICS307: CW[0-2]: %02X %02X %02X => %lu Hz\n", cw0, cw1, cw2,
			freq);
	return freq;
}

unsigned long get_board_sys_clk(ulong dummy)
{
	return gd->bus_clk;
}

unsigned long get_board_ddr_clk(ulong dummy)
{
	return gd->mem_clk;
}

unsigned long
calculate_board_sys_clk(ulong dummy)
{
	ulong val;
	u8 *pixis_base = (u8 *)PIXIS_BASE;

	val = ics307_clk_freq(
	    in_8(pixis_base + PIXIS_VSYSCLK0),
	    in_8(pixis_base + PIXIS_VSYSCLK1),
	    in_8(pixis_base + PIXIS_VSYSCLK2));
	debug("sysclk val = %lu\n", val);
	return val;
}

unsigned long
calculate_board_ddr_clk(ulong dummy)
{
	ulong val;
	u8 *pixis_base = (u8 *)PIXIS_BASE;

	val = ics307_clk_freq(
	    in_8(pixis_base + PIXIS_VDDRCLK0),
	    in_8(pixis_base + PIXIS_VDDRCLK1),
	    in_8(pixis_base + PIXIS_VDDRCLK2));
	debug("ddrclk val = %lu\n", val);
	return val;
}
#else
unsigned long get_board_sys_clk(ulong dummy)
{
	u8 i;
	ulong val = 0;
	u8 *pixis_base = (u8 *)PIXIS_BASE;

	i = in_8(pixis_base + PIXIS_SPD);
	i &= 0x07;

	switch (i) {
		case 0:
			val = 33333333;
			break;
		case 1:
			val = 40000000;
			break;
		case 2:
			val = 50000000;
			break;
		case 3:
			val = 66666666;
			break;
		case 4:
			val = 83333333;
			break;
		case 5:
			val = 100000000;
			break;
		case 6:
			val = 133333333;
			break;
		case 7:
			val = 166666666;
			break;
	}

	return val;
}

unsigned long get_board_ddr_clk(ulong dummy)
{
	u8 i;
	ulong val = 0;
	u8 *pixis_base = (u8 *)PIXIS_BASE;

	i = in_8(pixis_base + PIXIS_SPD);
	i &= 0x38;
	i >>= 3;

	switch (i) {
		case 0:
			val = 33333333;
			break;
		case 1:
			val = 40000000;
			break;
		case 2:
			val = 50000000;
			break;
		case 3:
			val = 66666666;
			break;
		case 4:
			val = 83333333;
			break;
		case 5:
			val = 100000000;
			break;
		case 6:
			val = 133333333;
			break;
		case 7:
			val = 166666666;
			break;
	}
	return val;
}
#endif

#ifdef CONFIG_TSEC_ENET
int board_eth_init(bd_t *bis)
{
	struct tsec_info_struct tsec_info[4];
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	int num = 0;

#ifdef CONFIG_TSEC1
	SET_STD_TSEC_INFO(tsec_info[num], 1);
	num++;
#endif
#ifdef CONFIG_TSEC2
	SET_STD_TSEC_INFO(tsec_info[num], 2);
	if (!(gur->pordevsr & MPC85xx_PORDEVSR_SGMII2_DIS))
		tsec_info[num].flags |= TSEC_SGMII;
	num++;
#endif
#ifdef CONFIG_TSEC3
	SET_STD_TSEC_INFO(tsec_info[num], 3);
	if (!(gur->pordevsr & MPC85xx_PORDEVSR_SGMII3_DIS))
		tsec_info[num].flags |= TSEC_SGMII;
	num++;
#endif

	if (!num) {
		printf("No TSECs initialized\n");

		return 0;
	}

#ifdef CONFIG_FSL_SGMII_RISER
	fsl_sgmii_riser_init(tsec_info, num);
#endif

	tsec_eth_init(bis, tsec_info, num);

	return pci_eth_init(bis);
}
#endif

#if defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup(void *blob, bd_t *bd)
{
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	u32 devdisr = in_be32(&gur->devdisr);
	phys_addr_t base;
	phys_size_t size;

	ft_cpu_setup(blob, bd);

	base = getenv_bootm_low();
	size = getenv_bootm_size();

	fdt_fixup_memory(blob, (u64)base, (u64)size);

#ifdef CONFIG_FSL_SERDES
	ft_mpc85xx_serdes_board_setup(blob);
#endif
#ifdef CONFIG_FSL_SGMII_RISER
	fsl_sgmii_riser_fdt_fixup(blob);
#endif
}
#endif

#ifdef CONFIG_MP
void board_lmb_reserve(struct lmb *lmb)
{
	cpu_mp_lmb_reserve(lmb);
}
#endif

#if defined(CONFIG_MMC)
/*
 * The environment variables are written to just after the u-boot image
 * on SDCard, so we must read the MBR to get the start address and code
 * length of the u-boot image, then calculate the address of the env.
 */
#define ESDHC_BOOT_IMAGE_SIZE	0x48
#define ESDHC_BOOT_IMAGE_ADDR	0x50

int mmc_get_env_addr(int dev, u32 *env_addr)
{
	int ret;
	u8 *tmp_buf;
	u32 blklen, code_offset, code_len;
	struct mmc *mmc = find_mmc_device(dev);

	mmc_init(mmc);

	blklen = mmc->read_bl_len;
	tmp_buf = malloc(blklen);
	if (!tmp_buf)
		return 1;

	/* read out the first block, get the config data information */
	ret = mmc_read(mmc, 0, tmp_buf, blklen);
	if (ret) {
		free(tmp_buf);
		return 1;
	}

	/* Get the Source Address, from offset 0x50 */
	code_offset = *(u32 *)(tmp_buf + ESDHC_BOOT_IMAGE_ADDR);

	/* Get the code size from offset 0x48 */
	code_len = *(u32 *)(tmp_buf + ESDHC_BOOT_IMAGE_SIZE);

	*env_addr = code_offset + code_len;

	free(tmp_buf);

	return 0;
}
#endif
