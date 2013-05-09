/*
 * Copyright 2007-2010 Freescale Semiconductor, Inc.
 * York Sun <yorksun@freescale.com>
 *
 * FSL DIU Framebuffer driver
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
#include <command.h>
#include <asm/io.h>

#ifdef CONFIG_FSL_DIU_FB

#include "../common/pixis.h"
#include "../common/fsl_diu_fb.h"

#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)
#include <stdio_dev.h>
#include <video_fb.h>
#endif

extern unsigned int FSL_Logo_BMP[];

static int xres, yres;

void diu_set_pixel_clock(unsigned int pixclock)
{
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	volatile unsigned int *guts_clkdvdr = &gur->clkdvdr;
	unsigned long speed_ccb, temp, pixval;

	speed_ccb = get_bus_freq(0);
	temp = 1000000000/pixclock;
	temp *= 1000;
	pixval = speed_ccb / temp;
	debug("DIU pixval = %lu\n", pixval);

	/* Modify PXCLK in GUTS CLKDVDR */
	debug("DIU: Current value of CLKDVDR = 0x%08x\n", *guts_clkdvdr);
	temp = *guts_clkdvdr & 0x2000FFFF;
	*guts_clkdvdr = temp;				/* turn off clock */
	*guts_clkdvdr = temp | 0x80000000 | ((pixval & 0x1F) << 16);
	debug("DIU: Modified value of CLKDVDR = 0x%08x\n", *guts_clkdvdr);
}

void p1022ds_diu_init(void)
{
	int gamma_fix;
	unsigned int pixel_format;
	unsigned char tmp_val;
	unsigned char pixis_arch;
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	u32 tmp, tmp_val_brdcfg0;
	u8 *pixis_base = (u8 *)PIXIS_BASE;

	tmp_val = in_8(pixis_base + PIXIS_BRDCFG1);
	pixis_arch = in_8(pixis_base + PIXIS_VER);

#ifdef CONFIG_MK_DIU1
		xres = 1024;
		yres = 768;
		pixel_format = 0x88883316;
		gamma_fix = 0;
		out_8(pixis_base + PIXIS_BRDCFG1, (tmp_val & 0xf7) | 0x40 | 0x20);
#else
		xres = 1280;
		yres = 1024;
		pixel_format = 0x88883316;
		gamma_fix = 0;
		out_8(pixis_base + PIXIS_BRDCFG1, tmp_val | 0x80);
#endif
	/* Set BRDCFG0[ELBC_DIU] */
	tmp_val_brdcfg0 = in_8(pixis_base + PIXIS_BRDCFG0);
	tmp_val_brdcfg0 = (tmp_val_brdcfg0 & 0x3D) | 0x02;
	out_8(pixis_base + PIXIS_BRDCFG0, tmp_val_brdcfg0);

	/* Setting PMUXCR to switch to DVI from ELBC */
	/* Set pmuxcr to allow both i2c1 and i2c2 */
	gur->pmuxcr |= 0x40000000;
	tmp = gur->pmuxcr;

	fsl_diu_init(xres, pixel_format, gamma_fix,
		     (unsigned char *)FSL_Logo_BMP);
}

int p1022diu_init_show_bmp(cmd_tbl_t *cmdtp,
			     int flag, int argc, char *argv[])
{
	unsigned int addr;

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	if (!strncmp(argv[1], "init", 4)) {
#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)
		fsl_diu_clear_screen();
		drv_video_init();
#else
		p1022ds_diu_init();
#endif
	} else {
		addr = simple_strtoul(argv[1], NULL, 16);
		fsl_diu_clear_screen();
		fsl_diu_display_bmp((unsigned char *)addr, 0, 0, 0);
	}

	return 0;
}

U_BOOT_CMD(
	diufb, CONFIG_SYS_MAXARGS, 1, p1022diu_init_show_bmp,
	"Init or Display BMP file",
	"init\n    - initialize DIU\n"
	"addr\n    - display bmp at address 'addr'"
);


#if defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE)

/*
 * The Graphic Device
 */
GraphicDevice ctfb;
void *video_hw_init(void)
{
	GraphicDevice *pGD = (GraphicDevice *) &ctfb;
	struct fb_info *info;
	printf("Calling p1022ds_diu_init from %s\n", __FUNCTION__);
	p1022ds_diu_init();

	/* fill in Graphic device struct */
	sprintf(pGD->modeIdent,
		"%dx%dx%d %ldkHz %ldHz",
		xres, yres, 32, 64, 60);

	pGD->frameAdrs = (unsigned int)fsl_fb_open(&info);
	pGD->winSizeX = xres;
	pGD->winSizeY = yres - info->logo_height;
	pGD->plnSizeX = pGD->winSizeX;
	pGD->plnSizeY = pGD->winSizeY;

	pGD->gdfBytesPP = 4;
	pGD->gdfIndex = GDF_32BIT_X888RGB;

	pGD->isaBase = 0;
	pGD->pciBase = 0;
	pGD->memSize = info->screen_size - info->logo_size;

	/* Cursor Start Address */
	pGD->dprBase = 0;
	pGD->vprBase = 0;
	pGD->cprBase = 0;

	return (void *)pGD;
}

void video_set_lut(unsigned int index,	/* color number */
		    unsigned char r,	/* red */
		    unsigned char g,	/* green */
		    unsigned char b	/* blue */
		    )
{
	return;
}

#endif /* defined(CONFIG_VIDEO) || defined(CONFIG_CFB_CONSOLE) */

#endif /* CONFIG_FSL_DIU_FB */
