/*
 * Copyright 2004, 2007-2010 Freescale Semiconductor.
 * Copyright (C) 2003  Motorola,Inc.
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

/* U-Boot Startup Code for Motorola 85xx PowerPC based Embedded Boards
 *
 * The processor starts at 0xfffffffc and the code is first executed in the
 * last 4K page(0xfffff000-0xffffffff) in flash/rom.
 *
 */

#include <config.h>
#include <mpc85xx.h>
#include <timestamp.h>
#include <version.h>

#define _LINUX_CONFIG_H 1	/* avoid reading Linux autoconf.h file	*/

#include <ppc_asm.tmpl>
#include <ppc_defs.h>

#include <asm/cache.h>
#include <asm/mmu.h>

#ifndef	 CONFIG_IDENT_STRING
#define	 CONFIG_IDENT_STRING ""
#endif

#undef	MSR_KERNEL
#define MSR_KERNEL ( MSR_ME )	/* Machine Check */

/*
 * Set up GOT: Global Offset Table
 *
 * Use r14 to access the GOT
 */
	START_GOT
#ifndef CONFIG_SRAM_BOOT
	GOT_ENTRY(_GOT2_TABLE_)
	GOT_ENTRY(_FIXUP_TABLE_)

#if !defined(CONFIG_NAND_SPL) && !defined(CONFIG_I2C_U_BOOT)
	GOT_ENTRY(_start)
	GOT_ENTRY(_start_of_vectors)
	GOT_ENTRY(_end_of_vectors)
	GOT_ENTRY(transfer_to_handler)
#endif
#endif /* !CONFIG_SRAM_BOOT */
	GOT_ENTRY(__init_end)
	GOT_ENTRY(_end)
	GOT_ENTRY(__bss_start)
	END_GOT

/*
 * e500 Startup -- after reset only the last 4KB of the effective
 * address space is mapped in the MMU L2 TLB1 Entry0. The .bootpg
 * section is located at THIS LAST page and basically does three
 * things: clear some registers, set up exception tables and
 * add more TLB entries for 'larger spaces'(e.g. the boot rom) to
 * continue the boot procedure.

 * Once the boot rom is mapped by TLB entries we can proceed
 * with normal startup.
 *
 */

	