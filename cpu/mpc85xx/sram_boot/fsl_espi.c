/*
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 * Author: Haiying Wang <Haiying.Wang@freescale.com>
 *
 * Base on drivers/spi/fsl_espi.c
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
#include <spi.h>
#include <asm/mmu.h>
#include <asm/immap_85xx.h>

#define ESPI_MAX_CS_NUM	4
#define ESPI_MAX_RX_LEN	(1 << 16)

#define ESPI_EV_RNE	0x00000200
#define ESPI_EV_TNF     0x00000100

#define ESPI_MODE_EN	0x80000000	/* Enable interface */
#define ESPI_MODE_TXTHR(x)      ((x) << 8)      /* Tx FIFO threshold */
#define ESPI_MODE_RXTHR(x)      ((x) << 0)      /* Rx FIFO threshold */

#define ESPI_COM_CS(x)	((x) << 30)
#define ESPI_COM_RxSKIP(x)      ((x) << 16)
#define ESPI_COM_TRANLEN(x)     ((x) << 0)

#define ESPI_CSMODE_CI_INACTIVEHIGH     0x80000000
#define ESPI_CSMODE_CP_BEGIN_EDGCLK     0x40000000
#define ESPI_CSMODE_REV_MSB_FIRST       0x20000000
#define ESPI_CSMODE_DIV16               0x10000000
#define ESPI_CSMODE_PM(x)               ((x) << 24)
#define ESPI_CSMODE_POL_ASSERTED_LOW    0x00100000
#define ESPI_CSMODE_LEN(x)              ((x) << 16)
#define ESPI_CSMODE_CSBEF(x)            ((x) << 12)
#define ESPI_CSMODE_CSAFT(x)            ((x) << 8)
#define ESPI_CSMODE_CSCG(x)             ((x) << 3)
#define ESPI_CSMODE_INIT_VAL (ESPI_CSMODE_POL_ASSERTED_LOW | \
                ESPI_CSMODE_CSBEF(0) | ESPI_CSMODE_CSAFT(0) | \
                ESPI_CSMODE_CSCG(1))

void espi_init(void)
{
	ccsr_espi_t *espi = (void *)(CONFIG_SYS_MPC85xx_ESPI_ADDR);
	sys_info_t sysinfo;
	unsigned long spibrg = 0;
	unsigned char pm = 0;
	int i;

	out_be32(&espi->mode, ESPI_MODE_RXTHR(3) |
				 ESPI_MODE_TXTHR(4) | ESPI_MODE_EN);

	out_be32(&espi->event, 0xffffffff);	/* Clear all eSPI events */
	out_be32(&espi->mask, 0x00000000);	/* Mask all eSPI interrupts */

	for (i = 0; i < ESPI_MAX_CS_NUM; i++)
		out_be32(&espi->csmode[i], ESPI_CSMODE_INIT_VAL);

	clrbits_be32(&espi->csmode[0], (ESPI_CSMODE_PM(0xF) | ESPI_CSMODE_DIV16
		| ESPI_CSMODE_CI_INACTIVEHIGH | ESPI_CSMODE_CP_BEGIN_EDGCLK
		| ESPI_CSMODE_REV_MSB_FIRST | ESPI_CSMODE_LEN(0xF)));

	/* Set eSPI BRG clock source */
	get_sys_info(&sysinfo);
	spibrg = sysinfo.freqSystemBus / 2;
	if ((spibrg / CONFIG_SF_DEFAULT_SPEED) > 32) {
		setbits_be32(&espi->csmode[0], ESPI_CSMODE_DIV16);
		pm = spibrg / (CONFIG_SF_DEFAULT_SPEED * 16 * 2);
		if (pm > 16) {
			pm = 16;
			debug("Requested speed is too low: %d Hz %d Hz"
				"is used.\n", CONFIG_SF_DEFAULT_SPEED, spibrg / (32 * 16));
		}
	} else
		pm = spibrg / (CONFIG_SF_DEFAULT_SPEED * 2);

	if (pm)
		pm--;

	setbits_be32(&espi->csmode[0],
			ESPI_CSMODE_PM(pm) |
			ESPI_CSMODE_REV_MSB_FIRST |
			ESPI_CSMODE_LEN(7));
}

void espi_load(unsigned int offs, int data_size, uchar *dst)
{
        ccsr_espi_t *espi = (void *)(CONFIG_SYS_MPC85xx_ESPI_ADDR);

        u8 cmd[5];
        int i;
        unsigned int tranlen, num_fifo;
        void *dout, *din;

	setbits_be32(&espi->mode, ESPI_MODE_EN);
#define CMD_FAST_READ 0x0b
#define CMD_SIZE	5
	cmd[0] = CMD_FAST_READ;
	cmd[1] = (offs / CONFIG_SF_PAGE_SIZE) >> 8;
	cmd[2] = (offs / CONFIG_SF_PAGE_SIZE);
	cmd[3] = (offs % CONFIG_SF_PAGE_SIZE);
	cmd[4] = 0x00;

	tranlen = CMD_SIZE + data_size;

	/* We are using fast read data commad, so the cmd length is 1 byte
	 * for opcode(0x03), 3 byte for 24 bit address, 1 dummy byte.
	 */
	setbits_be32(&espi->com, ESPI_COM_CS(0) |
			ESPI_COM_TRANLEN(tranlen - 1));

	num_fifo = tranlen / 4 + (tranlen % 4 ? 1 : 0);

	out_be32(&espi->event, 0xffffffff);

	din = (u8 *)(dst - CMD_SIZE); /* skip the cmd bytes */

	for (i = 0; i < num_fifo; i++) {
		dout = (u8 *)(&cmd[0] + 4 * (i % 2));

		if (in_be32(&espi->event) & ESPI_EV_TNF) {
			espi->tx = *(u32 *)dout;
			setbits_be32(&espi->event, ESPI_EV_TNF);
			if (tranlen > 4)
				tranlen -= 4;
		}

		/* Wait for eSPI tx to get out */
		udelay(80);

		if (in_be32(&espi->event) & ESPI_EV_RNE) {
			*(u32 *) din = espi->rx;
			din += 4;
			setbits_be32(&espi->event, ESPI_EV_RNE);
		}

	}
	clrbits_be32(&espi->mode, ESPI_MODE_EN);
}

/* The main entry to load the final uboot image to SDRAM and start from there */
void espi_boot()
{
	__attribute__((noreturn)) void (*uboot)(void);
	unsigned int num_rx, i, j, rest_len;
	unsigned int offset;
	unsigned int max_size_per_load;
	uchar *buf;

	buf = 0x100000;	/* temp addr for read data */

	/* eSPI can receive maximum 64k(1 << 16) bytes at one time, so
	 * calculate the number of receive times to get the whole uboot image.
	 * Note that there are 5 bytes for CMD, we need to skip that 5 bytes
	 * for valid uboot images.
	 */
	max_size_per_load = ESPI_MAX_RX_LEN - CMD_SIZE;
	num_rx = CONFIG_SYS_SF_U_BOOT_SIZE / max_size_per_load;
	rest_len = CONFIG_SYS_SF_U_BOOT_SIZE % max_size_per_load;

	for (i = 0; i < num_rx; i++) {
		offset = CONFIG_SYS_SF_U_BOOT_OFFS +
			i * max_size_per_load;
		espi_load(offset, max_size_per_load, buf);

		for (j = 0; j < max_size_per_load; j++)
			*(uchar *)(CONFIG_SYS_SF_U_BOOT_DST +
				i * max_size_per_load + j) =
				*(uchar *)(buf + j);
	}

	if (rest_len) {
		offset = CONFIG_SYS_SF_U_BOOT_OFFS + num_rx *
			max_size_per_load;

		espi_load(offset, rest_len, buf);

		for (j = 0; j < rest_len; j++)
			*(uchar *)(CONFIG_SYS_SF_U_BOOT_DST + num_rx *
				max_size_per_load + j) =
				*(uchar *) (buf + j);
	}

	puts("transfering control\n");

	flush_cache(CONFIG_SYS_SF_U_BOOT_DST,
			CONFIG_SYS_SF_U_BOOT_SIZE);

	/* start address of the bootpage */
	uboot = (void *)(CONFIG_SYS_SF_U_BOOT_DST +
			CONFIG_SYS_SF_U_BOOT_SIZE - 0x1000);

	uboot();
}
