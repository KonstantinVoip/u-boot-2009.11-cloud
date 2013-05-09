/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc.
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

#include <common.h>
#include <hwconfig.h>
#include <command.h>
#include <pci.h>
#include <asm/processor.h>
#include <asm/mmu.h>
#include <asm/cache.h>
#include <asm/immap_85xx.h>
#include <asm/io.h>
#include <miiphy.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <tsec.h>
#include <vsc7385.h>
#include <netdev.h>
#include <mmc.h>
#include <malloc.h>
#include <asm/fsl_serdes.h>
#include <asm/fsl_usb.h>

DECLARE_GLOBAL_DATA_PTR;
/*¬от она велика€ правда :))) дл€ подачи первоначального ресета мы должны 
 установить какие из портов GPIO будут у нас входами а какие выходами 
 делаем мы это путем выставление 0 или 1 значение регистра GPIO_DIR
 0 порт ->>signal input , 1 порт ->>signal output дл€ всех 15 GPIO
 после этого запол€ем регистр GPIO_DAT и выставл€ем в нашем случае это
 BOARD_PERI_RST_SET еденички там где они соответсвуют обозгаченным нами GPIO_DIR

 –егистр gpdat
 нумераци€ //Ёто соответсвенно подаЄм сигнал на GPIO 6 и GPIO 14 √де у нас вис€т 
 VCS8221 и VSC8641 и это как раз 0x00020000 и 0x02000000
 биты с 16 по 31 ->>Reserved
 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 0 0 0 0 0 0 1 0 0 0  0  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 
“оже самое с выстовлением портов на вход и выход
*/

///////////////////////////////////////MPC_P2020_GPIO_CONFIGURATION//////////////////////////////
#ifdef MPC_P2020
 //GPIO set for P2 020_mpc
 #define RMI1_RST   0x00010000            //gpio 13
 #define RMI2_RST   0x00020000            //gpio 14
 #define RMI3_RST   0x00040000            //gpio 15

 #define GPIO_DIR          0x00ff0000   //for p2020mpc  gpio 13,14,15
 #define BOARD_PERI_RST_SET	 RMI1_RST|RMI2_RST|RMI3_RST
#endif
 
///////////////////////////////////////RDB_P2020_GPIO_CONFIGURATION////////////////////////////// 
 #ifdef RDB_P2020 
  /*«десь нужно установить наши RESET  */
  #define VSC7385_RST_SET		0x00080000
  /*#define SLIC_RST_SET		0x00040000*/
  #define SGMII_PHY_RST_SET	0x00020000
  /*#define PCIE_RST_SET		0x00010000*/
  #define RGMII_PHY_RST_SET	0x02000000
  #define USB2_PORT_OUT_EN	0x01000000
  /*#define USB_RST_CLR		0x04000000*/
   /* ”станавливаем порты на вход выход регистр на новой плате нужно будет помен€ть GPDIR  0->signal_input  1->signal output*/ 
  #define GPIO_DIR		0x060f0000
  #define BOARD_PERI_RST_SET	SGMII_PHY_RST_SET |RGMII_PHY_RST_SET|VSC7385_RST_SET	
#endif





#define SYSCLK_MASK	    0x00200000
#define BOARDREV_MASK	0x10100000
#define BOARDREV_C	    0x00100000
#define BOARDREV_MPC	0x00000000

#define SYSCLK_66	    66666666
#define SYSCLK_100	    100000000

/*Cкорость SYSCLK дл€ нашей платы */
#define SYSCLK_83   833333333
/*–евизи€ дл€ нашей платы  */
#define BOARDREV_MSD 0x00000000

/*ѕолучаем значени€ clock  путем чтени€ с регистра GPIO_DAT и последующим битовым сдвигом */
unsigned long get_board_sys_clk(ulong dummy)
{
	volatile ccsr_gpio_t *pgpio = (void *)(CONFIG_SYS_MPC85xx_GPIO_ADDR);
	u32 val_gpdat, sysclk_gpio, board_rev_gpio;

	val_gpdat = pgpio->gpdat;
	sysclk_gpio = val_gpdat & SYSCLK_MASK;
	board_rev_gpio = val_gpdat & BOARDREV_MASK;
	if(sysclk_gpio == 0)
		return SYSCLK_66;
	else
		return SYSCLK_100;
	return 0;
}

#ifdef CONFIG_MMC
int board_early_init_f (void)
{
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	setbits_be32(&gur->pmuxcr,(MPC85xx_PMUXCR_SDHC_CD |MPC85xx_PMUXCR_SDHC_WP));
	return 0;
}
#endif

int checkboard (void)
{
	u32 val_gpdat, board_rev_gpio;
	volatile ccsr_gpio_t *pgpio = (void *)(CONFIG_SYS_MPC85xx_GPIO_ADDR);
	char board_rev = 0;
	struct cpu_type *cpu;

	val_gpdat = pgpio->gpdat;
	board_rev_gpio = val_gpdat & BOARDREV_MASK;
	if (board_rev_gpio == BOARDREV_C)
		board_rev = 'C';
	else if (board_rev_gpio == BOARDREV_MPC)
		board_rev = 'D';
	else
		panic ("Unexpected Board REV %x detected!!\n", board_rev_gpio);

	cpu = gd->cpu;

	#ifdef MPC_P2020
    printf ("Board: %s_MPC\n", cpu->name);
    #endif	
	
	#ifdef RDB_P2020 
	printf ("Board: %sRDB Rev%c\n", cpu->name, board_rev);
    #endif	
	
	#ifdef CONFIG_PHYS_64BIT
	puts ("(36-bit addrmap) \n");
#endif
	setbits_be32(&pgpio->gpdir, GPIO_DIR);

/*
 * Bringing the following peripherals out of reset via GPIOs
 * 0 = reset and 1 = out of reset
 * GPIO12 - Reset to Ethernet Switch
 * GPIO13 - Reset to SLIC/SLAC devices
 * GPIO14 - Reset to SGMII_PHY_N
 * GPIO15 - Reset to PCIe slots
 * GPIO6  - Reset to RGMII PHY
 * GPIO5  - Reset to USB3300 devices 1 = reset and 0 = out of reset
 */
  //printf ("No reset PANIC\n");
  /*≈сли правильным образом не подать resset на наши ethernet то в регистрах
  хлам получаетьс€ !!!!! нужно иметь ввиду это!!! при установке новых 
  внешних PHY “рансиверов*/
  /*clrsetbits_be32(&pgpio->gpdat, USB_RST_CLR, BOARD_PERI_RST_SET);*/
  setbits_be32(&pgpio->gpdat, BOARD_PERI_RST_SET);
  return 0;
}

#ifdef CONFIG_PCI
void pci_init_board(void)
{
	mpc85xx_serdes_board_init();
}
#endif

int board_early_init_r(void)
{
	u8 i2c_data;
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	const unsigned int flashbase = CONFIG_SYS_FLASH_BASE;
	const u8 flash_esel = 2;
	
	
	/*
	unsigned int orig_bus = i2c_get_bus_num();

	
	i2c_set_bus_num(1);
	if (i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR2, 0,1, &i2c_data, sizeof(i2c_data)) == 0) {
		if (i2c_data & 0x2)
			puts("NOR Flash Bank : Secondary\n");
		else
			puts("NOR Flash Bank : Primary\n");

		if (i2c_data & 0x1) 
		{
			setbits_be32(&gur->pmuxcr, MPC85xx_PMUXCR_SD_DATA);
			puts("SD/MMC : 8-bit Mode\n");
			puts("eSPI : Disabled\n");
		} else {
			puts("SD/MMC : 4-bit Mode\n");
			puts("eSPI : Enabled\n");
		}
	}
	else
		puts("Failed reading I2C Chip 0x18 on bus 1\n");

	i2c_set_bus_num(orig_bus); 
    */	
	/*
	 * Remap Boot flash region to caching-inhibited
	 * so that flash can be erased properly.
	 */

	/* Flush d-cache and invalidate i-cache of any FLASH data */
	flush_dcache();
	invalidate_icache();

	/* invalidate existing TLB entry for flash */
	disable_tlb(flash_esel);
    set_tlb(1, flashbase, CONFIG_SYS_FLASH_BASE_PHYS,MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,0, flash_esel, BOOKE_PAGESZ_16M, 1);
	//rtc_reset();
	return 0;
}


#ifdef CONFIG_TSEC_ENET
int board_eth_init(bd_t *bis)
{
	struct tsec_info_struct tsec_info[4];
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	int num = 0;
	char *tmp;
	unsigned int vscfw_addr;
/*«десь идет заполнение структуры дл€ tsec определение расположени€ регистров и адресов !!*/
/* где находить трансивер физического уровн€ */
#ifdef CONFIG_TSEC1
	SET_STD_TSEC_INFO(tsec_info[num], 1);
	num++;
#endif
#ifdef CONFIG_TSEC2
	SET_STD_TSEC_INFO(tsec_info[num], 2);
	num++;
#endif
#ifdef CONFIG_TSEC3
	SET_STD_TSEC_INFO(tsec_info[num], 3);
	/*
	if (!(gur->pordevsr & MPC85xx_PORDEVSR_SGMII3_DIS))
		tsec_info[num].flags |= TSEC_SGMII;
	*/
	num++;
#endif
	if (!num)
	{
		//printf("No TSECs initialized\n");
		  puts("No TSECs initialized\n\r");
		return 0;
	}
#ifdef CONFIG_VSC7385_ENET
/* If a VSC7385 microcode image is present, then upload it. */
	if ((tmp = getenv ("vscfw_addr")) != NULL) {
		vscfw_addr = simple_strtoul (tmp, NULL, 16);
		printf("uploading VSC7385 microcode from %x\n", vscfw_addr);
		if (vsc7385_upload_firmware((void *) vscfw_addr,
					CONFIG_VSC7385_IMAGE_SIZE))
			puts("Failure uploading VSC7385 microcode.\n");
	} else
		puts("No address specified for VSC7385 microcode.\n");
#endif
    puts("++++++++++++p1_p2_rdb.c/board_eth_init++++++++++++\n\r");
	tsec_eth_init(bis, tsec_info, num);

	return pci_eth_init(bis);  /*->>>идЄм в файл netdev.h*/
}
#endif

#if defined(CONFIG_OF_BOARD_SETUP)

void ft_board_setup(void *blob, bd_t *bd)
{
	/*
	volatile ccsr_gur_t *gur = (void *)CONFIG_SYS_MPC85xx_GUTS_ADDR;
	volatile ccsr_gpio_t *gpio = (void *)CONFIG_SYS_MPC85xx_GPIO_ADDR;
	phys_addr_t base;
	phys_size_t size;
	int off, usb_mode;
	const u32 *idx;
    */     
    phys_addr_t base;
	phys_size_t size;
	/*¬идиимо это та сама€ важна€ функци€ без которой всЄ плохо получаетьс€ */ 
	ft_cpu_setup(blob, bd);
	base = getenv_bootm_low();
	size = getenv_bootm_size();
	fdt_fixup_memory(blob, (u64)base, (u64)size);
	
	/*
	 * Configure mode for 1st usb.
	 */
	/* 
	if (hwconfig("usb1")) {
		const char *soc_compat = "fsl-usb2-dr";
		do {
			off = fdt_node_offset_by_compatible(blob, -1,
				soc_compat);
			if (off < 0) {
				printf("WARNING: could not find compatible"
				" node %s: %s.\n", soc_compat,
				fdt_strerror(off));
				break;
			} else{
				idx = fdt_getprop(blob, off, "cell-index",
					NULL);
			}
		} while (*idx != 0);

		if (hwconfig_subarg_cmp("usb1", "dr_mode", "host") == 1) {
			usb_mode = USB_HOST;
			printf("USB1 Controller configured as HOST\n");
		} else if (hwconfig_subarg_cmp("usb1", "dr_mode",
			"peripheral") == 1) {
			usb_mode = USB_PERIPHERAL;
			printf("USB1 Controller configured as PERIPHERAL\n");
		} else if (hwconfig_subarg_cmp("usb1", "dr_mode", "otg") == 1) {
			usb_mode = USB_OTG;
			printf("USB1 Controller configured as OTG\n");
		} else
			usb_mode = USB_HOST;
		fdt_fixup_usb_mode(blob, off, usb_mode);
	}
    */
	/*
	 * Add the 2nd usb node and enable it. eLBC will
	 * now be disabled since it is MUXed with USB2
	 */
	 /*
	if (hwconfig("usb2")) {
		if (hwconfig_subarg_cmp("usb2", "dr_mode", "host") == 1) {
			usb_mode = USB_HOST;
			printf("USB2 Controller configured as HOST. eLBC"
				" disabled!!\n");
		} else if (hwconfig_subarg_cmp("usb2", "dr_mode",
			"peripheral") == 1) {
			usb_mode = USB_PERIPHERAL;
			printf("USB2 Controller configured as PERIPHERAL. eLBC"
				" disabled!!\n");
		} else if (hwconfig_subarg_cmp("usb2", "dr_mode", "otg") == 1) {
			usb_mode = USB_OTG;
			printf("USB2 Controller configured as OTG. eLBC"
				" disabled!!\n");
		} else {
			usb_mode = USB_HOST;
		}
		fdt_fixup_add_2nd_usb(blob, usb_mode);
		setbits_be32(&gpio->gpdir, USB2_PORT_OUT_EN);
		setbits_be32(&gpio->gpdat, USB2_PORT_OUT_EN);
		setbits_be32(&gur->pmuxcr, MPC85xx_PMUXCR_ELBC_OFF_USB2_ON);
	}*/
	return;
}
#endif

#ifdef CONFIG_MP
extern void cpu_mp_lmb_reserve(struct lmb *lmb);

void board_lmb_reserve(struct lmb *lmb)
{
	cpu_mp_lmb_reserve(lmb);
}
#endif

#if defined(CONFIG_MMC)
/*
 * The environment variables are written to just after the u-boot image
 * on SDCard, so we must read the MBR to get the start address and code
 * length of the u-boot image, then calculate the address of the env.
 */
#define ESDHC_BOOT_IMAGE_SIZE	0x48
#define ESDHC_BOOT_IMAGE_ADDR	0x50

int mmc_get_env_addr(int dev, u32 *env_addr)
{
	int ret;
	u8 *tmp_buf;
	u32 blklen, code_offset, code_len;
	struct mmc *mmc = find_mmc_device(dev);

	mmc_init(mmc);

	blklen = mmc->read_bl_len;
	tmp_buf = malloc(blklen);
	if (!tmp_buf)
		return 1;

	/* read out the first block, get the config data information */
	ret = mmc_read(mmc, 0, tmp_buf, blklen);
	if (ret) {
		free(tmp_buf);
		return 1;
	}

	/* Get the Source Address, from offset 0x50 */
	code_offset = *(u32 *)(tmp_buf + ESDHC_BOOT_IMAGE_ADDR);

	/* Get the code size from offset 0x48 */
	code_len = *(u32 *)(tmp_buf + ESDHC_BOOT_IMAGE_SIZE);

	*env_addr = code_offset + code_len;

	free(tmp_buf);

	return 0;
}
#endif
