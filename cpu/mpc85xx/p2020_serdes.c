/*
 * Copyright 2009 Freescale Semiconductor, Inc.
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

/* 
#include <common.h>
#include <asm/immap_85xx.h>
#include <asm/io.h>
#include <asm/fsl_serdes.h>

#define SRDS_MAX_LANES		4

static int serdes_cfg;
static int serdes_prtcl_map = 0;

u8 serdes_cfg_tbl[][SRDS_MAX_LANES] = {
	[0] = {PCIE1, NONE, NONE, NONE},
	[1] = {NONE, NONE, NONE, NONE},
	[2] = {PCIE1, PCIE2, PCIE3, PCIE3},
	[4] = {PCIE1, PCIE1, PCIE3, PCIE3},
	[6] = {PCIE1, PCIE1, PCIE1, PCIE1},
	[7] = {SRIO2, SRIO1, NONE, NONE},
	[8] = {SRIO2, SRIO2, SRIO2, SRIO2},
	[9] = {SRIO2, SRIO2, SRIO2, SRIO2},
	[10] = {SRIO2, SRIO2, SRIO2, SRIO2},
	[11] = {SRIO2, SRIO1, SGMII_TSEC2, SGMII_TSEC3},
	[12] = {SRIO2, SRIO1, SGMII_TSEC2, SGMII_TSEC3},
	[13] = {PCIE1, SRIO1, SGMII_TSEC2, SGMII_TSEC3},
	[14] = {PCIE1, PCIE2, SGMII_TSEC2, SGMII_TSEC3},
	[15] = {PCIE1, PCIE1, SGMII_TSEC2, SGMII_TSEC3},
};

static const char *serdes_prtcl_str[] = {
	[NONE] = "NA",
	[PCIE1] = "PCIE1",
	[PCIE2] = "PCIE2",
	[PCIE3] = "PCIE3",
	[SRIO1] = "SRIO1",
	[SRIO2] = "SRIO2",
	[SGMII_TSEC2] = "SGMII_TSEC2",
	[SGMII_TSEC3] = "SGMII_TSEC3",
};

static enum srds_prtcl serdes_get_prtcl(int lane)
{
	return serdes_cfg_tbl[serdes_cfg][lane];
}

int is_serdes_configured(enum srds_prtcl prtcl)
{
	return (1 << prtcl) & serdes_prtcl_map;
}

void fsl_serdes_init(void)
{
	ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	int lane;
	int i;
	enum srds_prtcl lane_prtcl;

	serdes_cfg = (in_be32(&gur->pordevsr) & MPC85xx_PORDEVSR_IO_SEL) >> 19;
	debug("Using SERDES configuration 0x%x, lane settings:\n",
			serdes_cfg);

	for (lane = 0; lane < SRDS_MAX_LANES; lane++) {
		lane_prtcl = serdes_get_prtcl(lane);

		serdes_prtcl_map |= (1 << lane_prtcl);
		debug("%s ", serdes_prtcl_str[lane_prtcl]);
	}
	debug("\n");
}
*/