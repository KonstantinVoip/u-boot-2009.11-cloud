/*
 * Copyright 2007-2010 Freescale Semiconductor, Inc.
 *
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <libfdt.h>
#include <fdt_support.h>
#include <asm/processor.h>
#include <asm/fsl_usb.h>
#include <linux/ctype.h>
#ifdef CONFIG_FSL_ESDHC
#include <fsl_esdhc.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

extern void ft_qe_setup(void *blob);
extern void ft_fixup_num_cores(void *blob);

#ifdef CONFIG_MP
#include "mp.h"

void ft_fixup_cpu(void *blob, u64 memory_limit)
{
	int off;
	ulong spin_tbl_addr = get_spin_phys_addr();
	u32 bootpg = determine_mp_bootpg();
	u32 id = get_my_id();

	off = fdt_node_offset_by_prop_value(blob, -1, "device_type", "cpu", 4);
	while (off != -FDT_ERR_NOTFOUND) {
		u32 *reg = (u32 *)fdt_getprop(blob, off, "reg", 0);

		if (reg) {
			if (*reg == id) {
				fdt_setprop_string(blob, off, "status", "okay");
			} else {
				u64 val = *reg * SIZE_BOOT_ENTRY + spin_tbl_addr;
				val = cpu_to_fdt32(val);
				fdt_setprop_string(blob, off, "status",
								"disabled");
				fdt_setprop_string(blob, off, "enable-method",
								"spin-table");
				fdt_setprop(blob, off, "cpu-release-addr",
						&val, sizeof(val));
			}
		} else {
			printf ("cpu NULL\n");
		}
		off = fdt_node_offset_by_prop_value(blob, off,
				"device_type", "cpu", 4);
	}

	/* Reserve the boot page so OSes dont use it */
	if ((u64)bootpg < memory_limit) {
		off = fdt_add_mem_rsv(blob, bootpg, (u64)4096);
		if (off < 0)
			printf("%s: %s\n", __FUNCTION__, fdt_strerror(off));
	}
}
#endif

#define ft_fixup_l3cache(x, y)

#if defined(CONFIG_L2_CACHE)
/* return size in kilobytes */
static inline u32 l2cache_size(void)
{
	volatile ccsr_l2cache_t *l2cache = (void *)CONFIG_SYS_MPC85xx_L2_ADDR;
	volatile u32 l2siz_field = (l2cache->l2ctl >> 28) & 0x3;
	u32 ver = SVR_SOC_VER(get_svr());

	switch (l2siz_field) {
	case 0x0:
		break;
	case 0x1:
		if (ver == SVR_8540 || ver == SVR_8560   ||
		    ver == SVR_8541 || ver == SVR_8541_E ||
		    ver == SVR_8555 || ver == SVR_8555_E)
			return 128;
		else
			return 256;
		break;
	case 0x2:
		if (ver == SVR_8540 || ver == SVR_8560   ||
		    ver == SVR_8541 || ver == SVR_8541_E ||
		    ver == SVR_8555 || ver == SVR_8555_E)
			return 256;
		else
			return 512;
		break;
	case 0x3:
		return 1024;
		break;
	}

	return 0;
}

static inline void ft_fixup_l2cache(void *blob)
{
	int len, off;
	u32 *ph;
	struct cpu_type *cpu = identify_cpu(SVR_SOC_VER(get_svr()));
	char compat_buf[38];

	const u32 line_size = 32;
	const u32 num_ways = 8;
	const u32 size = l2cache_size() * 1024;
	const u32 num_sets = size / (line_size * num_ways);

	off = fdt_node_offset_by_prop_value(blob, -1, "device_type", "cpu", 4);
	if (off < 0) {
		debug("no cpu node fount\n");
		return;
	}

	ph = (u32 *)fdt_getprop(blob, off, "next-level-cache", 0);

	if (ph == NULL) {
		debug("no next-level-cache property\n");
		return ;
	}

	off = fdt_node_offset_by_phandle(blob, *ph);
	if (off < 0) {
		printf("%s: %s\n", __func__, fdt_strerror(off));
		return ;
	}

	if (cpu) {
		if (isdigit(cpu->name[0]))
			len = sprintf(compat_buf,
				"fsl,mpc%s-l2-cache-controller", cpu->name);
		else
			len = sprintf(compat_buf,
				"fsl,%c%s-l2-cache-controller",
				tolower(cpu->name[0]), cpu->name + 1);

		sprintf(&compat_buf[len + 1], "cache");
	}
	fdt_setprop(blob, off, "cache-unified", NULL, 0);
	fdt_setprop_cell(blob, off, "cache-block-size", line_size);
	fdt_setprop_cell(blob, off, "cache-size", size);
	fdt_setprop_cell(blob, off, "cache-sets", num_sets);
	fdt_setprop_cell(blob, off, "cache-level", 2);
	fdt_setprop(blob, off, "compatible", compat_buf, sizeof(compat_buf));

	/* we dont bother w/L3 since no platform of this type has one */
}
#elif defined(CONFIG_BACKSIDE_L2_CACHE)
static inline void ft_fixup_l2cache(void *blob)
{
	int off, l2_off, l3_off = -1;
	u32 *ph;
	u32 l2cfg0 = mfspr(SPRN_L2CFG0);
	u32 size, line_size, num_ways, num_sets;

	size = (l2cfg0 & 0x3fff) * 64 * 1024;
	num_ways = ((l2cfg0 >> 14) & 0x1f) + 1;
	line_size = (((l2cfg0 >> 23) & 0x3) + 1) * 32;
	num_sets = size / (line_size * num_ways);

	off = fdt_node_offset_by_prop_value(blob, -1, "device_type", "cpu", 4);

	while (off != -FDT_ERR_NOTFOUND) {
		ph = (u32 *)fdt_getprop(blob, off, "next-level-cache", 0);

		if (ph == NULL) {
			debug("no next-level-cache property\n");
			goto next;
		}

		l2_off = fdt_node_offset_by_phandle(blob, *ph);
		if (l2_off < 0) {
			printf("%s: %s\n", __func__, fdt_strerror(off));
			goto next;
		}

		fdt_setprop(blob, l2_off, "cache-unified", NULL, 0);
		fdt_setprop_cell(blob, l2_off, "cache-block-size", line_size);
		fdt_setprop_cell(blob, l2_off, "cache-size", size);
		fdt_setprop_cell(blob, l2_off, "cache-sets", num_sets);
		fdt_setprop_cell(blob, l2_off, "cache-level", 2);
		fdt_setprop(blob, l2_off, "compatible", "cache", 6);

		if (l3_off < 0) {
			ph = (u32 *)fdt_getprop(blob, l2_off, "next-level-cache", 0);

			if (ph == NULL) {
				debug("no next-level-cache property\n");
				goto next;
			}
			l3_off = *ph;
		}
next:
		off = fdt_node_offset_by_prop_value(blob, off,
				"device_type", "cpu", 4);
	}
	if (l3_off > 0) {
		l3_off = fdt_node_offset_by_phandle(blob, l3_off);
		if (l3_off < 0) {
			printf("%s: %s\n", __func__, fdt_strerror(off));
			return ;
		}
		ft_fixup_l3cache(blob, l3_off);
	}
}
#else
#define ft_fixup_l2cache(x)
#endif

static inline void ft_fixup_cache(void *blob)
{
	int off;

	off = fdt_node_offset_by_prop_value(blob, -1, "device_type", "cpu", 4);

	while (off != -FDT_ERR_NOTFOUND) {
		u32 l1cfg0 = mfspr(SPRN_L1CFG0);
		u32 l1cfg1 = mfspr(SPRN_L1CFG1);
		u32 isize, iline_size, inum_sets, inum_ways;
		u32 dsize, dline_size, dnum_sets, dnum_ways;

		/* d-side config */
		dsize = (l1cfg0 & 0x7ff) * 1024;
		dnum_ways = ((l1cfg0 >> 11) & 0xff) + 1;
		dline_size = (((l1cfg0 >> 23) & 0x3) + 1) * 32;
		dnum_sets = dsize / (dline_size * dnum_ways);

		fdt_setprop_cell(blob, off, "d-cache-block-size", dline_size);
		fdt_setprop_cell(blob, off, "d-cache-size", dsize);
		fdt_setprop_cell(blob, off, "d-cache-sets", dnum_sets);

		/* i-side config */
		isize = (l1cfg1 & 0x7ff) * 1024;
		inum_ways = ((l1cfg1 >> 11) & 0xff) + 1;
		iline_size = (((l1cfg1 >> 23) & 0x3) + 1) * 32;
		inum_sets = isize / (iline_size * inum_ways);

		fdt_setprop_cell(blob, off, "i-cache-block-size", iline_size);
		fdt_setprop_cell(blob, off, "i-cache-size", isize);
		fdt_setprop_cell(blob, off, "i-cache-sets", inum_sets);

		off = fdt_node_offset_by_prop_value(blob, off,
				"device_type", "cpu", 4);
	}

	ft_fixup_l2cache(blob);
}


void fdt_add_enet_stashing(void *fdt)
{
	do_fixup_by_compat(fdt, "gianfar", "bd-stash", NULL, 0, 1);

	do_fixup_by_compat_u32(fdt, "gianfar", "rx-stash-len", 96, 1);

	do_fixup_by_compat_u32(fdt, "gianfar", "rx-stash-idx", 0, 1);

	do_fixup_by_compat(fdt, "fsl,etsec2", "bd-stash", NULL, 0, 1);

	do_fixup_by_compat_u32(fdt, "fsl,etsec2", "rx-stash-len", 96, 1);

	do_fixup_by_compat_u32(fdt, "fsl,etsec2", "rx-stash-idx", 0, 1);
}

void ft_cpu_setup(void *blob, bd_t *bd)
{
	int off;
	int val;
	sys_info_t sysinfo;

	/* delete crypto node if not on an E-processor */
	if (!IS_E_PROCESSOR(get_svr()))
		fdt_fixup_crypto_node(blob, 0);

	fdt_fixup_ethernet(blob);

	fdt_add_enet_stashing(blob);

	do_fixup_by_prop_u32(blob, "device_type", "cpu", 4,
		"timebase-frequency", get_tbclk(), 1);
	do_fixup_by_prop_u32(blob, "device_type", "cpu", 4,
		"bus-frequency", bd->bi_busfreq, 1);
	get_sys_info(&sysinfo);
	off = fdt_node_offset_by_prop_value(blob, -1, "device_type", "cpu", 4);
	while (off != -FDT_ERR_NOTFOUND) {
		u32 *reg = (u32 *)fdt_getprop(blob, off, "reg", 0);
		val = cpu_to_fdt32(sysinfo.freqProcessor[*reg]);
		fdt_setprop(blob, off, "clock-frequency", &val, 4);
		off = fdt_node_offset_by_prop_value(blob, off, "device_type",
							"cpu", 4);
	}
	do_fixup_by_prop_u32(blob, "device_type", "soc", 4,
		"bus-frequency", bd->bi_busfreq, 1);

	do_fixup_by_compat_u32(blob, "fsl,pq3-localbus",
		"bus-frequency", gd->lbc_clk, 1);
	do_fixup_by_compat_u32(blob, "fsl,elbc",
		"bus-frequency", gd->lbc_clk, 1);
#ifdef CONFIG_QE
	ft_qe_setup(blob);
#endif

#ifdef CONFIG_SYS_NS16550
	do_fixup_by_compat_u32(blob, "ns16550",
		"clock-frequency", CONFIG_SYS_NS16550_CLK, 1);
#endif

#ifdef CONFIG_CPM2
	do_fixup_by_compat_u32(blob, "fsl,cpm2-scc-uart",
		"current-speed", bd->bi_baudrate, 1);

	do_fixup_by_compat_u32(blob, "fsl,cpm2-brg",
		"clock-frequency", bd->bi_brgfreq, 1);
#endif

	fdt_fixup_memory(blob, (u64)bd->bi_memstart, (u64)bd->bi_memsize);

#ifdef CONFIG_MP
	ft_fixup_cpu(blob, (u64)bd->bi_memstart + (u64)bd->bi_memsize);
#endif
	ft_fixup_num_cores(blob);

	ft_fixup_cache(blob);

#if defined(CONFIG_FSL_ESDHC)
	fdt_fixup_esdhc(blob, bd);
#endif
	fdt_fixup_1588(blob);
}

void fdt_fixup_1588(void *blob)
{
	const char *ptp_compat = "fsl,gianfar-ptp-timer";
	int off, err;
	unsigned long tmr_freq;
       
	tmr_freq = (gd->bus_clk >> 3);
	tmr_freq = (tmr_freq * 5);
	off = fdt_node_offset_by_compatible(blob, -1, ptp_compat);
	if (off < 0) {
		printf("WARNING: could not find 1588 compatible node %s: %s.\n",
			ptp_compat, fdt_strerror(off));
		return;
	}

	err = fdt_setprop_cell(blob, off, "timer-frequency", tmr_freq);
	if (err < 0)
	   printf("WARNING: could not set 1588 timer-frequency property: %s\n",
		fdt_strerror(err));
}

#if defined(CONFIG_OF_BOARD_SETUP)
void fdt_fixup_usb_mode(void *blob, int off, int usb_mode)
{
	const char *usb_compat = "fsl-usb2-dr";
	int err;

	switch (usb_mode) {
	case USB_PERIPHERAL:{
			err = fdt_setprop_string(blob, off, "dr_mode",
				"peripheral");
			if (err < 0)
				printf("WARNING: could not set dr_mode"
					" property: %s\n",
					fdt_strerror(err));
			}
			break;
	case USB_OTG:{
			err = fdt_setprop_string(blob, off, "dr_mode", "otg");
			if (err < 0)
				printf("WARNING: could not set dr_mode"
					" property: %s\n",
					fdt_strerror(err));
			}
			break;
	case USB_HOST:
	default:{
			err = fdt_setprop_string(blob, off, "dr_mode", "host");
			if (err < 0)
				printf("WARNING: could not set dr_modae"
					" property: %s\n",
					fdt_strerror(err));
			}
			break;
		}
	return;
}

void fdt_fixup_add_2nd_usb(void *blob, int usb_mode)
{
	const char *soc_compat = "fsl,p1020-immr";
	const char *lbc_compat = "fsl,p1020-elbc";
	const u32 *addrcell, *sizecell, *ph;
	const u32 idx = (u32)1;
	int off, lbcoff, len, err;
	u32 *regbuf = NULL;
	u32 *irqbuf = NULL;

	regbuf = (u32 *)malloc(MAXBUFSIZE * sizeof(u32));
	if (regbuf == NULL) {
		printf("%s: WARNING: failed to allocate memory\n", __func__);
		return;
	}
	irqbuf = (u32 *)malloc(MAXBUFSIZE * sizeof(u32));
	if (irqbuf == NULL) {
		printf("%s: WARNING: failed to allocate memory\n", __func__);
		return;
	}

	off = fdt_node_offset_by_compatible(blob, -1, soc_compat);
	if (off < 0) {
		printf("WARNING: could not find compatible node %s: %s.\n",
			soc_compat, fdt_strerror(off));
		return;
	}

	lbcoff = fdt_node_offset_by_compatible(blob, -1, lbc_compat);
	if (lbcoff < 0) {
		printf("WARNING: could not find compatible node %s: %s.\n",
			lbc_compat, fdt_strerror(lbcoff));
		return;
	}

	addrcell = fdt_getprop(blob, off, "#address-cells", NULL);
	sizecell = fdt_getprop(blob, off, "#size-cells", NULL);

	off = fdt_add_subnode(blob, off, "usb@23000");
	if (off < 0) {
		printf("WARNING: could not add 2nd usb node %s.\n",
				fdt_strerror(off));
		return;
	}

	err = fdt_setprop_cell(blob, off, "#address-cells", 1);
	if (err < 0)
		printf("WARNING: could not set #address-cell property: %s\n",
			fdt_strerror(err));

	err = fdt_setprop_cell(blob, off, "#size-cells", 0);
	if (err < 0)
		printf("WARNING: could not set #size-cells property: %s\n",
			fdt_strerror(err));

	err = fdt_setprop_string(blob, off, "compatible", "fsl-usb2-dr");
	if (err < 0)
		printf("WARNING: could not set compatible property: %s\n",
			fdt_strerror(err));

	err = fdt_setprop(blob, off, "cell-index", (const void *)&idx,
		sizeof(u32));
	if (err < 0)
		printf("WARNING: could not set <%s> %s\n",
					"cell-index", fdt_strerror(err));

	err = fdt_setprop_string(blob, off, "phy_type", "ulpi");
	if (err < 0)
		printf("WARNING: could not set phy_type property: %s\n",
			fdt_strerror(err));

	fdt_fixup_usb_mode(blob, off, usb_mode);

	if (addrcell && *addrcell == 2) {
		regbuf[0] = 0;
		regbuf[1] = CONFIG_SYS_MPC85xx_USB2_OFFSET;
		len = 2;
	} else {
		regbuf[0] = CONFIG_SYS_MPC85xx_USB2_OFFSET;
		len = 1;
	}

	if (sizecell && *sizecell == 2) {
		regbuf[len] = 0;
		regbuf[len + 1] = 0x1000;
		len = 2;
	} else {
		regbuf[len] = 0x1000;
		len++;
	}

	err = fdt_setprop(blob, off, "reg", regbuf, len * sizeof(u32));
	if (err < 0)
		printf("WARNING: could not set <%s> %s\n",
					"reg", fdt_strerror(err));

	irqbuf[0] = 0x2e;
	irqbuf[1] = 0x2;

	err = fdt_setprop(blob, off, "interrupts", irqbuf, 2 * sizeof(u32));
	if (err < 0)
		printf("WARNING: could not set %s %s\n",
				"interrupts", fdt_strerror(err));

	ph = fdt_getprop(blob, lbcoff, "interrupt-parent", 0);
	if (!ph) {
		printf("WARNING: could not read interrupt-parent property\n");
		return;
	}

	err = fdt_setprop(blob, off, "interrupt-parent", ph, sizeof(u32));
	if (err < 0)
		printf("WARNING: could not set %s %s\n",
				"interrupt-parent", fdt_strerror(err));

	free(regbuf);
	free(irqbuf);
}
#endif
