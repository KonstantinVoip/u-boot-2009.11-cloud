/*
 * Copyright 2010 Freescale Semiconductor, Inc
 * Jin Yanjiang(B30464@freescale.com)
 *
 * Based on the Freescale common mmc code:
 * (drivers/mmc/mmc.c & drivers/mmc/fsl_esdhc.c)
 * (C) Copyright  2007, 2009-2010 Freescale Semiconductor, Inc
 * Andy Fleming
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

#include <config.h>
#include <common.h>
#include <command.h>
#include <mmc.h>
#include <asm/io.h>
#include <fsl_esdhc.h>
#include <div64.h>
#include <environment.h>

DECLARE_GLOBAL_DATA_PTR;

struct fsl_esdhc {
	uint dsaddr;
	uint blkattr;
	uint cmdarg;
	uint xfertyp;
	uint cmdrsp0;
	uint cmdrsp1;
	uint cmdrsp2;
	uint cmdrsp3;
	uint datport;
	uint prsstat;
	uint proctl;
	uint sysctl;
	uint irqstat;
	uint irqstaten;
	uint irqsigen;
	uint autoc12err;
	uint hostcapblt;
	uint wml;
	char reserved1[8];
	uint fevt;
	char reserved2[168];
	uint hostver;
	char reserved3[780];
	uint scr;
};

static struct fsl_esdhc *regs = (struct fsl_esdhc *)CONFIG_SYS_FSL_ESDHC_ADDR;
/*All loading stage variables and environment variables are stored in sram.*/
static env_t *env_ptr = (env_t *) CONFIG_ENV_ADDR;/*0xf8fa0000*/
#define LOAD_BUFFER 0xf8fa2000
#define LOAD_MMC_PTR 0xf8fa4000
#define LOAD_TMP_BUF 0xf8fa4c00


/*All booting stage variables are stored in dram*/
#define BOOT_CP_TMP_BUF	 0x200000;
#define BOOT_BUFFER 0x210000
#define BOOT_MMC_PTR 0x220000

#define BLK_LEN 512

/* Return the XFERTYP flags for a given command and data packet */
static uint esdhc_xfertyp(struct mmc_cmd *cmd, struct mmc_data *data)
{
	uint xfertyp = 0;

	if (data) {
		xfertyp |= XFERTYP_DPSEL;	/* Disable internal DMA */
		if (data->blocks > 1) {
			xfertyp |= XFERTYP_MSBSEL;
			xfertyp |= XFERTYP_BCEN;
			xfertyp |= XFERTYP_AC12EN;
		}

		if (data->flags & MMC_DATA_READ)
			xfertyp |= XFERTYP_DTDSEL;
	}

	if (cmd->resp_type & MMC_RSP_CRC)
		xfertyp |= XFERTYP_CCCEN;
	if (cmd->resp_type & MMC_RSP_OPCODE)
		xfertyp |= XFERTYP_CICEN;
	if (cmd->resp_type & MMC_RSP_136)
		xfertyp |= XFERTYP_RSPTYP_136;
	else if (cmd->resp_type & MMC_RSP_BUSY)
		xfertyp |= XFERTYP_RSPTYP_48_BUSY;
	else if (cmd->resp_type & MMC_RSP_PRESENT)
		xfertyp |= XFERTYP_RSPTYP_48;

	return XFERTYP_CMD(cmd->cmdidx) | xfertyp;
}

/*
 * PIO Read/Write Mode reduce the performace as DMA is not used in this mode.
 */
static void esdhc_pio_read_write(struct mmc_data *data)
{
	uint blocks;
	uchar *buffer;
	uint databuf;
	uint size;
	uint irqstat = 0;

	if (data->flags & MMC_DATA_READ) {
		blocks = data->blocks;
		buffer = (uchar *) data->dest;
		while (blocks) {
			size = data->blocksize;
			while (size && (!(irqstat & IRQSTAT_TC))) {
				irqstat = in_be32(&regs->irqstat);
				databuf = in_le32(&regs->datport);
				*((uint *) buffer) = databuf;
				buffer += 4;
				size -= 4;
			}
			blocks--;
		}
	} else {
		blocks = data->blocks;
		buffer = (uchar *) data->src;
		while (blocks) {
			size = data->blocksize;
			irqstat = in_be32(&regs->irqstat);
			while (!(in_be32(&regs->prsstat) & PRSSTAT_BWEN))
				;
			while (size && (!(irqstat & IRQSTAT_TC))) {
				databuf = *((uint *) buffer);
				buffer += 4;
				size -= 4;
				irqstat = in_be32(&regs->irqstat);
				out_le32(&regs->datport, databuf);
			}
			blocks--;
		}
	}
}

static int esdhc_setup_data(struct mmc_data *data)
{
	int timeout;


	if (data->flags & MMC_DATA_READ) {

		out_be32(&regs->dsaddr, (u32) data->dest);
	} else {
		if ((in_be32(&regs->prsstat) & PRSSTAT_WPSPL) == 0) {
			puts("\nThe SD card is locked."
				" Can not write to a locked card.\n\n");
			return TIMEOUT;
		}
		out_be32(&regs->dsaddr, (u32) data->src);
	}


	out_be32(&regs->blkattr, data->blocks << 16 | data->blocksize);

	/* Calculate the timeout period for data transactions */
	timeout = 14;

	clrsetbits_be32(&regs->sysctl, SYSCTL_TIMEOUT_MASK, timeout << 16);

	return 0;
}

/*
 * Sends a command out on the bus.  Takes the mmc pointer,
 * a command pointer, and an optional data pointer.
 */
static int esdhc_send_cmd(struct mmc_cmd *cmd, struct mmc_data *data)
{
	uint xfertyp;
	uint irqstat;

	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION)
		return 0;

	out_be32(&regs->irqstat, -1);

	sync();

	/* Wait for the bus to be idle */
	while ((in_be32(&regs->prsstat) & PRSSTAT_CICHB) ||
	       (in_be32(&regs->prsstat) & PRSSTAT_CIDHB))
		;

	while (in_be32(&regs->prsstat) & PRSSTAT_DLA)
		;

	/* Wait at least 8 SD clock cycles before the next command */
	/*
	 * Note: This is way more than 8 cycles, but 1ms seems to
	 * resolve timing issues with some cards
	 */
	udelay(1000);

	/* Set up for a data transfer if we have one */
	if (data) {
		int err;

		err = esdhc_setup_data(data);
		if (err)
			return err;
	}

	/* Figure out the transfer arguments */
	xfertyp = esdhc_xfertyp(cmd, data);

	/* Send the command */
	out_be32(&regs->cmdarg, cmd->cmdarg);
	out_be32(&regs->xfertyp, xfertyp);

	/* Wait for the command to complete */
	while (!(in_be32(&regs->irqstat) & IRQSTAT_CC))
		;

	irqstat = in_be32(&regs->irqstat);
	out_be32(&regs->irqstat, irqstat);

	if (irqstat & CMD_ERR)
		return COMM_ERR;

	if (irqstat & IRQSTAT_CTOE)
		return TIMEOUT;

	/* Copy the response to the response buffer */
	udelay(5000);

	if (cmd->resp_type & MMC_RSP_136) {
		u32 cmdrsp3, cmdrsp2, cmdrsp1, cmdrsp0;

		cmdrsp3 = in_be32(&regs->cmdrsp3);
		cmdrsp2 = in_be32(&regs->cmdrsp2);
		cmdrsp1 = in_be32(&regs->cmdrsp1);
		cmdrsp0 = in_be32(&regs->cmdrsp0);
		cmd->response[0] = (cmdrsp3 << 8) | (cmdrsp2 >> 24);
		cmd->response[1] = (cmdrsp2 << 8) | (cmdrsp1 >> 24);
		cmd->response[2] = (cmdrsp1 << 8) | (cmdrsp0 >> 24);
		cmd->response[3] = (cmdrsp0 << 8);
	} else
		cmd->response[0] = in_be32(&regs->cmdrsp0);

	/* Wait until all of the blocks are transferred */
	if (data)
		esdhc_pio_read_write(data);

	out_be32(&regs->irqstat, -1);

	return 0;
}

static void set_sysctl(uint clock)
{
	int sdhc_clk = gd->sdhc_clk;
	int div, pre_div;
	uint clk;

	if (sdhc_clk / 16 > clock) {
		for (pre_div = 2; pre_div < 256; pre_div *= 2)
			if ((sdhc_clk / pre_div) <= (clock * 16))
				break;
	} else
		pre_div = 2;

	for (div = 1; div <= 16; div++)
		if ((sdhc_clk / (div * pre_div)) <= clock)
			break;

	pre_div >>= 1;
	div -= 1;

	clk = (pre_div << 8) | (div << 4);

	clrsetbits_be32(&regs->sysctl, SYSCTL_CLOCK_MASK, clk);

	udelay(10000);

	setbits_be32(&regs->sysctl, SYSCTL_PEREN);
}

static void esdhc_set_ios(void)
{
	/* Set the clock speed */
	set_sysctl(25000000);

	/* Set the bus width */
	clrbits_be32(&regs->proctl, PROCTL_DTW_4 | PROCTL_DTW_8);
	setbits_be32(&regs->proctl, PROCTL_DTW_4);
}

static void esdhc_reset(void)
{
	unsigned long timeout;

	/* reset the controller */
	out_be32(&regs->sysctl, SYSCTL_RSTA);

	/* wait max 100 ms */
	timeout = 100;
	/* hardware clears the bit when it is done */
	while ((in_be32(&regs->sysctl) & SYSCTL_RSTA) && timeout--)
		udelay(1000);
	if (!timeout)
		puts("MMC/SD: Reset never completed.\n");
	udelay(1000);
}

static int mmc_set_blocklen(void)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = BLK_LEN;
	cmd.flags = 0;

	return esdhc_send_cmd(&cmd, NULL);
}

static int mmc_read_block(void *dst, uint start, int blkcnt)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err, stoperr = 0;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	cmd.cmdarg = start * BLK_LEN;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = BLK_LEN;
	data.flags = MMC_DATA_READ;

	err = esdhc_send_cmd(&cmd, &data);

	if (blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		stoperr = esdhc_send_cmd(&cmd, NULL);
	}

	if (err)
		return err;

	return stoperr;
}

static int sram_boot_mmc_read(u64 src, uchar *dst, int size)
{
	char *buffer;
	int startblock = lldiv(src, BLK_LEN);
	int endblock = lldiv(src + size - 1, BLK_LEN);
	int err = 0;

	if ((s32) dst > CONFIG_SYS_INIT_L2_ADDR)
		buffer = (char *)LOAD_BUFFER;
	else
		buffer = (char *)BOOT_BUFFER;

	/* We always do full block reads from the card */
	err = mmc_set_blocklen();

	if (err)
		return err;

	err =
	    mmc_read_block(buffer, startblock, endblock - startblock + 1);
	if (err)
		return err;

	memcpy(dst, buffer, size);

	return 0;
}

static int mmc_go_idle(void)
{
	struct mmc_cmd cmd;
	int err;

	udelay(1000);

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	err = esdhc_send_cmd(&cmd, NULL);

	if (err)
		return err;

	udelay(2000);

	return 0;
}

static int sd_send_op_cond(void)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;

	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = esdhc_send_cmd(&cmd, NULL);

		if (err)
			return err;

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = MMC_VDD_32_33 | MMC_VDD_33_34;
		cmd.cmdarg |= OCR_HCS;

		err = esdhc_send_cmd(&cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	return 0;
}

static int mmc_startup(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;

	/* Put the Card in Identify Mode */
	cmd.cmdidx = MMC_CMD_ALL_SEND_CID;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = esdhc_send_cmd(&cmd, NULL);

	if (err)
		return err;

	mmc->rca = 0;
	/*
	 * For MMC cards, set the Relative Address.
	 * For SD cards, get the Relatvie Address.
	 * This also puts the cards into Standby State
	 */
	cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
	cmd.cmdarg = mmc->rca << 16;
	cmd.resp_type = MMC_RSP_R6;
	cmd.flags = 0;

	err = esdhc_send_cmd(&cmd, NULL);

	if (err)
		return err;

	mmc->rca = (cmd.response[0] >> 16) & 0xffff;

	/* Get the Card-Specific Data */
	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = esdhc_send_cmd(&cmd, NULL);

	if (err)
		return err;

	/* Select the card, and put it into Transfer Mode */
	cmd.cmdidx = MMC_CMD_SELECT_CARD;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;
	err = esdhc_send_cmd(&cmd, NULL);

	if (err)
		return err;

	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = esdhc_send_cmd(&cmd, NULL);
	if (err)
		return err;

	cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 2;
	cmd.flags = 0;
	err = esdhc_send_cmd(&cmd, NULL);
	if (err)
		return err;

	return 0;
}

/*
 * The final uboot are written to just after the middle u-boot image
 * on SDCard, so we must read the MBR to get the start address and code
 * length of the u-boot image, then calculate the address of the final uboot.
 */
#define ESDHC_BOOT_IMAGE_SIZE	0x48
#define ESDHC_BOOT_IMAGE_ADDR	0x50

static void mmc_cp_final(void)
{
	int ret;
	u8 *dst, *tmp_buf;
	u32 code_offset, code_len, code_end;

	tmp_buf = (u8 *) BOOT_CP_TMP_BUF;

	/* read out the first block, get the config data information */
	ret = sram_boot_mmc_read(0, tmp_buf, BLK_LEN);
	if (ret)
		return;

	/* Get the Source Address, from offset 0x50 */
	code_offset = *(u32 *) (tmp_buf + ESDHC_BOOT_IMAGE_ADDR);

	/* Get the code size from offset 0x48 */
	code_len = *(u32 *) (tmp_buf + ESDHC_BOOT_IMAGE_SIZE);

	code_offset += CONFIG_SYS_SD_U_BOOT_OFFS;
	dst = (u8 *) CONFIG_SYS_SD_U_BOOT_DST;

	code_end = code_offset + code_len;

	while (code_offset < code_end) {
		ret = sram_boot_mmc_read(code_offset, dst, BLK_LEN);
		if (ret)
			return;
		code_offset += BLK_LEN;
		dst += BLK_LEN;
	}

	return;
}

static int mmc_get_env_addr(void)
{
	int ret = 0;
	u8 *tmp_buf;
	u32 env_addr;
	u32 code_offset, code_len;

	tmp_buf = (u8 *) (LOAD_TMP_BUF);

	/* read out the first block, get the config data information */
	ret = sram_boot_mmc_read(0, tmp_buf, BLK_LEN);
	if (ret)
		return 1;

	/* Get the Source Address, from offset 0x50 */
	code_offset = *(u32 *) (tmp_buf + ESDHC_BOOT_IMAGE_ADDR);

	/* Get the code size from offset 0x48 */
	code_len = *(u32 *) (tmp_buf + ESDHC_BOOT_IMAGE_SIZE);

	env_addr = code_offset + code_len;

	ret = sram_boot_mmc_read(env_addr, (u_char *) env_ptr, CONFIG_ENV_SIZE);

	return ret;
}


void esdhc_load(void)
{
	struct mmc *mmc = (struct mmc *)(LOAD_MMC_PTR);

	esdhc_reset();
	mmc_go_idle();
	sd_send_op_cond();
	esdhc_set_ios();
	mmc_startup(mmc);

	mmc_get_env_addr();

	return;
}

/* The main entry to load the final uboot image to SDRAM and start from there */
void esdhc_boot(void)
{
	struct mmc *mmc = (struct mmc *)(BOOT_MMC_PTR);

	__attribute__ ((noreturn)) void (*uboot) (void);

	puts("\nloading final stage uboot ...\n");

	esdhc_reset();
	mmc_go_idle();
	sd_send_op_cond();
	esdhc_set_ios();
	mmc_startup(mmc);

	mmc_cp_final();

	puts("transfering control\n");

	flush_cache(CONFIG_SYS_SD_U_BOOT_DST, CONFIG_SYS_SD_U_BOOT_SIZE);

	/* start address of the bootpage */
	uboot = (void *)(CONFIG_SYS_SD_U_BOOT_DST +
			 CONFIG_SYS_SD_U_BOOT_SIZE - 0x1000);

	uboot();
}
