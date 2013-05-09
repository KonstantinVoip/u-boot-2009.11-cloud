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

/*
 * eeprom_array_tool/pre_init.c
 * I2C EEPROM boot loader
 * Output is an eeprom arra
 */


#include <config.h>
#include <linux/types.h>	/* for size_t */
#include <linux/stddef.h>	/* for NULL */
#include <asm/immap_85xx.h>
#include <asm/fsl_law.h>
#include <asm/mmu.h>

#include <stdio.h>


#define LAWAR_EN		0x80000000
#define ACS_BIT_SHIFT		0x7
#define BYTE_EN			0xF	/*indicates 4 byte write to register*/
#define BYTE_EN_BIT_SHIFT	0x3
#define CONT_BIT_SHIFT		0x2
#define DDR_TARGET_ADDR		0x7f000

#define CONFIG_FREQ_667

/*DDR settings*/
#define CONFIG_SYS_DDR_SDRAM_BASE	0x00000000
#define CONFIG_SYS_BOOT_LOADER_START	0x100

#define CONFIG_SYS_DDR_DATA_INIT	0xdeadbeef
#define CONFIG_SYS_DDR_CS0_BNDS		0x0000003F
#define CONFIG_SYS_DDR_CS0_CONFIG	0x80014202 /*Enable, no interleaving*/
#define CONFIG_SYS_DDR_CS0_CONFIG_2	0x00000000
#define CONFIG_SYS_DDR_CONTROL		0x43000000 /*Type = DDR2*/
#define CONFIG_SYS_DDR_CONTROL_2	0x24401000

#ifdef CONFIG_FREQ_400
#define CONFIG_SYS_DDR_TIMING_3		0x00010000
#define CONFIG_SYS_DDR_TIMING_0		0x00260802
#define CONFIG_SYS_DDR_TIMING_1		0x39355322
#define CONFIG_SYS_DDR_TIMING_2		0x1f9048ca
#define CONFIG_SYS_DDR_CLK_CTRL		0x02800000
#define CONFIG_SYS_DDR_MODE_1		0x00480432
#define CONFIG_SYS_DDR_MODE_2		0x00000000
#define CONFIG_SYS_DDR_INTERVAL		0x06180100
#endif

#ifdef CONFIG_FREQ_533
#define CONFIG_SYS_DDR_TIMING_4_533	0x00000000
#define CONFIG_SYS_DDR_TIMING_5_533	0x00000000
#define CONFIG_SYS_DDR_TIMING_3		0x00020000
#define CONFIG_SYS_DDR_TIMING_0		0x00260802
#define CONFIG_SYS_DDR_TIMING_1		0x4c47c432
#define CONFIG_SYS_DDR_TIMING_2		0x0f9848ce
#define CONFIG_SYS_DDR_CLK_CTRL		0x02800000
#define CONFIG_SYS_DDR_MODE_1		0x00040642
#define CONFIG_SYS_DDR_MODE_2		0x00000000
#define CONFIG_SYS_DDR_INTERVAL		0x08200100
#define CONFIG_SYS_DDR_CONTROL_533	0x43080000	/* Type = DDR2 */
#define CONFIG_SYS_DDR_CONTROL2_533	0x24401000
#endif

#ifdef CONFIG_FREQ_667
#define CONFIG_SYS_DDR_TIMING_3		0x00030000
#define CONFIG_SYS_DDR_TIMING_0		0x55770802
#define CONFIG_SYS_DDR_TIMING_1		0x5f599543
#define CONFIG_SYS_DDR_TIMING_2		0x0fa074d1
#define CONFIG_SYS_DDR_CLK_CTRL		0x03000000
#define CONFIG_SYS_DDR_MODE_1		0x00040852
#define CONFIG_SYS_DDR_MODE_2		0x00000000
#define CONFIG_SYS_DDR_INTERVAL		0x0a280100
#endif

#ifdef CONFIG_FREQ_800
#define CONFIG_SYS_DDR_TIMING_3		0x00040000
#define CONFIG_SYS_DDR_TIMING_0		0x55770802
#define CONFIG_SYS_DDR_TIMING_1		0x6f6b6543
#define CONFIG_SYS_DDR_TIMING_2		0x0fa074d1
#define CONFIG_SYS_DDR_CLK_CTRL		0x02800000
#define CONFIG_SYS_DDR_MODE_1		0x00040852
#define CONFIG_SYS_DDR_MODE_2		0x00000000
#define CONFIG_SYS_DDR_INTERVAL		0x0a280100
#endif

#define SDRAM_CFG_32_BE			0x00080000

#define BPTR_ENABLE			0x1
#define DEVDISR_REG_VAL			0x26002000

/*write to configuration space*/
#define WRITE_REG(a, v)\
	gen_code_array((u32 *)&a, v, 0)

/*write to alternate configuration space*/
#define WRITE_MEM(a, v)\
	gen_code_array((u32 *)a, v, 1)

/* globals */
u8 i2c_eeprom_array[0x100000];
static u32 array_index;


/* generat the code in the format understood by I2C. */
void gen_code_array(u32 *addr, u32 val, char flag)
{
	/* Ref to I2C chapter for the Format */
	u32 a = (u32) addr;
	if (flag == 0)
		i2c_eeprom_array[array_index++] =
			(BYTE_EN << BYTE_EN_BIT_SHIFT)|\
			(1 << CONT_BIT_SHIFT) | ((a >> 18) & 0x00000003);
	else
		i2c_eeprom_array[array_index++] =
			(1 << ACS_BIT_SHIFT) | (BYTE_EN << BYTE_EN_BIT_SHIFT)\
			| (1 << CONT_BIT_SHIFT) | ((a >> 18) & 0x00000003);

	i2c_eeprom_array[array_index++] = (a >> 10) & 0x000000FF;
	i2c_eeprom_array[array_index++] = (a >> 2) & 0x000000FF;
	i2c_eeprom_array[array_index++] = (val >> 24) & 0xFF;
	i2c_eeprom_array[array_index++] = (val >> 16) & 0xFF;
	i2c_eeprom_array[array_index++] = (val >> 8) & 0xFF;
	i2c_eeprom_array[array_index++] = (val) & 0xFF;
}

/* initialize the RCW, DDR, LBIU */
int ddr_init()
{
	struct ccsr_ddr *ddr = (struct ccsr_ddr *)CONFIG_SYS_MPC85xx_DDR_ADDR;
	struct ccsr_local_ecm *ecm = (struct ccsr_local_ecm *)\
					CONFIG_SYS_MPC85xx_ECM_ADDR;
	ccsr_lbc_t *lbc = (void *)(CONFIG_SYS_MPC85xx_LBC_ADDR);

	/* RCW values */
	WRITE_REG(ecm->lawbar0, (CONFIG_SYS_DDR_SDRAM_BASE >> 12) & 0xfffff);
#ifdef CONFIG_P1020
	WRITE_REG(ecm->lawar0, LAWAR_EN | ((u32)(LAW_TRGT_IF_DDR_1 << 20))\
		 | (u32)LAW_SIZE_512M);
#else
	WRITE_REG(ecm->lawar0, LAWAR_EN | ((u32)(LAW_TRGT_IF_DDR_1 << 20))\
		 | (u32)LAW_SIZE_1G);
#endif

	WRITE_REG(ecm->lawbar1, (CONFIG_SYS_FLASH_BASE_PHYS >> 12) & 0xfffff);
	WRITE_REG(ecm->lawar1, LAWAR_EN | ((u32)(LAW_TRGT_IF_LBC << 20))\
		| (u32)LAW_SIZE_16M);
	WRITE_REG(lbc->lcrr, LCRR_DBYP | LCRR_CLKDIV_8);
	WRITE_REG(lbc->br0, CONFIG_FLASH_BR_PRELIM);
	WRITE_REG(lbc->or0, CONFIG_FLASH_OR_PRELIM);

	WRITE_REG(ddr->sdram_clk_cntl, CONFIG_SYS_DDR_CLK_CTRL);
	WRITE_REG(ddr->cs0_bnds, CONFIG_SYS_DDR_CS0_BNDS);
	WRITE_REG(ddr->cs0_config, CONFIG_SYS_DDR_CS0_CONFIG);
	WRITE_REG(ddr->cs0_config_2, CONFIG_SYS_DDR_CS0_CONFIG_2);
	WRITE_REG(ddr->sdram_data_init, CONFIG_SYS_DDR_DATA_INIT);
	WRITE_REG(ddr->timing_cfg_3, CONFIG_SYS_DDR_TIMING_3);
	WRITE_REG(ddr->timing_cfg_1, CONFIG_SYS_DDR_TIMING_1);
	WRITE_REG(ddr->timing_cfg_2, CONFIG_SYS_DDR_TIMING_2);
	WRITE_REG(ddr->timing_cfg_0, CONFIG_SYS_DDR_TIMING_0);

	WRITE_REG(ddr->sdram_cfg_2, CONFIG_SYS_DDR_CONTROL_2);

	WRITE_REG(ddr->sdram_mode, CONFIG_SYS_DDR_MODE_1);
	WRITE_REG(ddr->sdram_mode_2, CONFIG_SYS_DDR_MODE_2);

	WRITE_REG(ddr->sdram_interval, CONFIG_SYS_DDR_INTERVAL);

#ifdef CONFIG_P1020
	WRITE_REG(ddr->sdram_cfg, CONFIG_SYS_DDR_CONTROL |\
		SDRAM_CFG_32_BE | 0x80000000);
#else
	WRITE_REG(ddr->sdram_cfg, CONFIG_SYS_DDR_CONTROL | 0x80000000);
#endif

	WRITE_REG(ecm->bptr, ((BPTR_ENABLE << 31) |\
		((DDR_TARGET_ADDR & 0xff000) >> 12)));
	return 0;
}

/* Initialize the preamble data */
void set_preamble()
{

	i2c_eeprom_array[array_index++] = 0xAA;
	i2c_eeprom_array[array_index++] = 0x55;
	i2c_eeprom_array[array_index++] = 0xAA;
}

/* read the binary file begining ar offset to EOF */
void readfile(FILE *f, int offset)
{
	int c;
	int i;
	u32 data = 0x0;

	/* it is assumed that the proper code was  generated here */
	/* loop till we reach the desired offset */

	i = DDR_TARGET_ADDR;

	while ((c = getc(f)) != EOF) {
		data = (data << 8) | c;
		if ((i % 4) == 3) {
			WRITE_MEM((u32 *) i, data);
			data = 0x0;
		}
		i++;
	}

}

/* Convert the bootloader code into I2C data format */
void boot_loader()
{
	FILE *f = fopen("bootloader.bin", "rb");
	if (f == NULL) {
		printf("cannot open file: ");
		perror("bootloader.bin");
		printf("\n");
	} else {
		readfile(f, CONFIG_SYS_BOOT_LOADER_START);
		fclose(f);
	}
}

/*
 enable the core by setting the CORE_DIS bit in ACR.
*/
void enable_core()
{
	struct ccsr_gur *guts = (struct ccsr_gur *)\
		CONFIG_SYS_MPC85xx_GUTS_ADDR;

	/* enable core */
	WRITE_REG(guts->devdisr, DEVDISR_REG_VAL);
}

/*  Generate the END sequence,CRC. */
void end_seq_crc()
{
	u32 crc = 0x0;

	/* end seq */
	i2c_eeprom_array[array_index++] = 0x00;
	i2c_eeprom_array[array_index++] = 0x00;
	i2c_eeprom_array[array_index++] = 0x00;

	/* calculate CRC32 up to this point */
	crc = crc32(i2c_eeprom_array, array_index);

	/* CRC */
	i2c_eeprom_array[array_index++] = (crc >> 24) & 0xFF;
	i2c_eeprom_array[array_index++] = (crc >> 16) & 0xFF;
	i2c_eeprom_array[array_index++] = (crc >> 8) & 0xFF;
	i2c_eeprom_array[array_index++] = (crc >> 0) & 0xFF;

	/* zero data */
	i2c_eeprom_array[array_index++] = 0x00;
	i2c_eeprom_array[array_index++] = 0x00;
	i2c_eeprom_array[array_index++] = 0x00;
}

/* Generate binary file of the code */
void gen_bin_file(u32 size)
{
	u32 ret = 0;
	FILE *f = fopen("i2c_prom.bin", "wb");	/* must open in binary mode */
	if (f == NULL) {
		perror("can not open file i2c_prom.bin\n");
	} else {
		ret = fwrite(i2c_eeprom_array, 1, array_index, f);
		if (ret == array_index) {
			printf("Success:: i2c_prom.bin generated.\n");
			printf("Use flash programmer to flash this image.\n");
		} else
			printf("Fail to generate i2c_prom.bin.\n");
		fclose(f);
	}
}

int main()
{
	int i = 0;

	/* Fill with 0's */
	for (i = 0; i < sizeof(i2c_eeprom_array); i++)
		i2c_eeprom_array[i] = 0x0;

	/* generate code for the following */
	set_preamble();
	ddr_init();
	boot_loader();
	WRITE_MEM(0x10000, 0xa1b2c3d4);
	WRITE_MEM(0xffff, 0xa1b2c3d4);
	WRITE_MEM(0x20000, 0xa1b2c3d4);
	enable_core();
	end_seq_crc();

	/* Copy to bin */
	gen_bin_file(array_index);
	return 0;
}
