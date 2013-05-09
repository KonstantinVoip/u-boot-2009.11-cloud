/*
 * Copyright (C) 2008-2009 Freescale Semiconductor, Inc.
 *
 * Changelog:
 * July 2008, Intial version.
 * June 2009, align to the MMC framework.
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
#include <mmc.h>

#include <environment.h>

DECLARE_GLOBAL_DATA_PTR;

/* references to names in env_common.c */
extern uchar default_environment[];
extern int mmc_get_env_addr(int dev, u32 *env_addr);

char *env_name_spec = "SD CARD";
env_t *env_ptr;

uchar env_get_char_spec(int index)
{
	return *((uchar *)(gd->env_addr + index));
}

static int readenv(int dev, size_t offset, u_char *buf)
{
	int ret;
	struct mmc *mmc = find_mmc_device(dev);

	mmc_init(mmc);

	ret = mmc_read(mmc, offset, buf, CONFIG_ENV_SIZE);
	if (ret)
		return 1;

	return 0;
}

static int writeenv(int dev, size_t offset, u_char *buf)
{
	int env_blknr, env_blkcnt, n;
	uint blklen;
	struct mmc *mmc = find_mmc_device(dev);

	mmc_init(mmc);

	blklen = mmc->write_bl_len;
	env_blknr = offset / blklen;
	env_blkcnt = CONFIG_ENV_SIZE / blklen;

	n = mmc->block_dev.block_write(dev, env_blknr, env_blkcnt, buf);
	if (n != env_blkcnt)
		return 1;

	return 0;
}

int saveenv(void)
{
	int ret;
	int dev = 0;
	u32 env_addr;

	ret = mmc_get_env_addr(dev, &env_addr);
	if (ret) {
		puts("FAILED!\n");
		return 1;
	}

	ret = writeenv(dev, env_addr, (u_char *) env_ptr);
	if (ret)
		return 1;

	puts("done\n");
	gd->env_valid = 1;

	return ret;
}

void env_relocate_spec(void)
{
	int ret;
	int dev = 0;
	u32 env_addr;

	ret = mmc_get_env_addr(dev, &env_addr);
	if (ret)
		goto err_read;

	ret = readenv(dev, env_addr, (u_char *) env_ptr);
	if (ret)
		goto err_read;

	if (crc32(0, env_ptr->data, ENV_SIZE) != env_ptr->crc)
		goto err_crc;

	gd->env_valid = 1;

	return;

err_read:
err_crc:
	puts("*** Warning - bad CRC, using default environment\n\n");

	set_default_env();
}

int env_init(void)
{
	/* eSDHC isn't usable before relocation */
	gd->env_addr  = (ulong)&default_environment[0];
	gd->env_valid = 1;

	return 0;
}
