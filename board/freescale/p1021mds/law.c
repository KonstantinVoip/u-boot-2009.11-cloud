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
#include <asm/fsl_law.h>
#include <asm/mmu.h>

struct law_entry law_table[] = {
	SET_LAW(CONFIG_SYS_PCIE1_MEM_PHYS, LAW_SIZE_512M, LAW_TRGT_IF_PCIE_1),
	SET_LAW(CONFIG_SYS_PCIE1_IO_PHYS, LAW_SIZE_64K, LAW_TRGT_IF_PCIE_1),
	SET_LAW(CONFIG_SYS_PCIE2_MEM_PHYS, LAW_SIZE_512M, LAW_TRGT_IF_PCIE_2),
	SET_LAW(CONFIG_SYS_PCIE2_IO_PHYS, LAW_SIZE_64K, LAW_TRGT_IF_PCIE_2),
	SET_LAW(CONFIG_SYS_NAND_BASE_PHYS, LAW_SIZE_1M, LAW_TRGT_IF_LBC),
	SET_LAW(CONFIG_SYS_BCSR_BASE_PHYS, LAW_SIZE_256K, LAW_TRGT_IF_LBC),
};

int num_law_entries = ARRAY_SIZE(law_table);
