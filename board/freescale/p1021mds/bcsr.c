/*
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <common.h>
#include <asm/io.h>

#include "bcsr.h"

#define BCSR11_ENET_MICRST      0x20

void reset_p1021mds_micrel_phy(void)
{
	clrbits_8((u8 *)(CONFIG_SYS_BCSR_BASE + 11), BCSR11_ENET_MICRST);
	setbits_8((u8 *)(CONFIG_SYS_BCSR_BASE + 11), BCSR11_ENET_MICRST);
}
