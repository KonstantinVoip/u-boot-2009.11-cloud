CONFIG_PPC=y
CONFIG_L2_CACHE=y
CONFIG_SYS_I2C_EEPROM_NXID=y
CONFIG_CMD_FAT=y
CONFIG_SYS_EEPROM_BUS_NUM=y
CONFIG_BOARD_EARLY_INIT_F=y
CONFIG_BOARD_EARLY_INIT_R=y
CONFIG_HARD_I2C=y
CONFIG_SYS_CLK_FREQ="get_board_sys_clk(0)"
CONFIG_SYS_PCIE2_MEM_PHYS="0xa0000000"
CONFIG_CMD_ITEST=y
CONFIG_SYS_MPC85xx_USB_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_USB_OFFSET)"
CONFIG_CMD_EDITENV=y
CONFIG_CMD_SETEXPR=y
CONFIG_SYS_MAX_NAND_DEVICE=y
CONFIG_SYS_NAND_U_BOOT_DST="(0x01000000 - CONFIG_SYS_NAND_SPL_SIZE)"
CONFIG_MTD_NAND_VERIFY_WRITE=y
CONFIG_SYS_MPC85xx_ESPI_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_ESPI_OFFSET)"
CONFIG_SYS_GBL_DATA_SIZE=128
CONFIG_SYS_MPC85xx_CPM_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_CPM_OFFSET)"
CONFIG_SF_DEFAULT_MODE=0
CONFIG_SYS_LONGHELP=y
CONFIG_SYS_I2C_NOPROBES="{{0,0x29}}"
CONFIG_SYS_MPC85xx_USB_OFFSET="0x22000"
CONFIG_SYS_LOAD_ADDR="0x2000000"
CONFIG_CMD_XIMG=y
CONFIG_BOOTDELAY=10
CONFIG_SYS_NAND_BASE="0xffa00000"
CONFIG_SYS_MPC85xx_ESPI_OFFSET="0x7000"
CONFIG_SYS_INIT_RAM_END="0x00004000"
CONFIG_SPI_FLASH=y
CONFIG_SYS_HELP_CMD_WIDTH=8
CONFIG_SYS_MPC85xx_GUTS_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_GUTS_OFFSET)"
CONFIG_SYS_MPC85xx_DMA_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_DMA_OFFSET)"
CONFIG_FSL_LAW=y
CONFIG_SYS_CBSIZE=256
CONFIG_SYS_MPC85xx_PCIX2_OFFSET="0x9000"
CONFIG_SYS_MONITOR_LEN="(256 * 1024)"
CONFIG_MD5=y
CONFIG_SYS_64BIT_STRTOUL=y
CONFIG_SYS_PCIE1_IO_PHYS="0xffc30000"
CONFIG_CMD_CONSOLE=y
CONFIG_SYS_FLASH_BASE_PHYS="CONFIG_SYS_FLASH_BASE"
CONFIG_SYS_PCIE1_MEM_BUS="0xc0000000"
CONFIG_SYS_MPC85xx_PIC_OFFSET="0x40000"
CONFIG_MII=y
CONFIG_SYS_MPC85xx_PIC_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_PIC_OFFSET)"
CONFIG_SYS_MPC85xx_DMA_OFFSET="0x21000"
CONFIG_SYS_MPC85xx_PCIX2_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_PCIX2_OFFSET)"
CONFIG_SYS_MPC85xx_SERDES1_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_SERDES2_OFFSET)"
CONFIG_DDR_CLK_FREQ=66666666
CONFIG_MMC=y
CONFIG_SYS_PCIE1_MEM_PHYS="0xc0000000"
CONFIG_SYS_CCSRBAR_PHYS="CONFIG_SYS_CCSRBAR"
CONFIG_ROOTPATH="/opt/nfsroot"
CONFIG_BTB=y
CONFIG_MEM_INIT_VALUE="0xDeadBeef"
CONFIG_CMD_MISC=y
CONFIG_SYS_RAMBOOT=y
CONFIG_FIT=y
CONFIG_SYS_MPC85xx_ECM_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_ECM_OFFSET)"
CONFIG_SYS_GBL_DATA_OFFSET="(CONFIG_SYS_INIT_RAM_END - CONFIG_SYS_GBL_DATA_SIZE)"
CONFIG_SYS_MPC85xx_PCIX_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_PCIX_OFFSET)"
CONFIG_PHY_GIGE=y
CONFIG_ENV_OFFSET="0x100000"
CONFIG_BPTR_VIRT_ADDR="0xfffff000"
CONFIG_ENV_SPI_CS=0
CONFIG_SYS_NAND_BLOCK_SIZE="(16 * 1024)"
CONFIG_SYS_MPC85xx_LBC_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_LBC_OFFSET)"
CONFIG_ENV_OVERWRITE=y
CONFIG_CMD_NET=y
CONFIG_SYS_BR2_PRELIM="(BR_PHYS_ADDR(CONFIG_SYS_VSC7385_BASE) | BR_PS_8 | BR_V)"
CONFIG_CMD_NFS=y
CONFIG_FSL_ESPI=y
CONFIG_ENV_SIZE="0x2000"
CONFIG_FSL_I2C=y
CONFIG_SYS_PCIE2_IO_SIZE="0x00010000"
CONFIG_SYS_MPC85xx_ECM_OFFSET="0x0000"
CONFIG_SYS_IMMR="CONFIG_SYS_CCSRBAR"
CONFIG_CMD_PING=y
CONFIG_SYS_MPC85xx_SATA2_OFFSET="0x19000"
CONFIG_FSL_ELBC=y
CONFIG_BOOKE=y
CONFIG_SYS_MPC85xx_L2_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_L2_OFFSET)"
CONFIG_SYS_MALLOC_LEN="(1024 * 1024)"
CONFIG_SYS_MPC85xx_GPIO_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_GPIO_OFFSET)"
CONFIG_SYS_MPC85xx_SERDES2_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_SERDES2_OFFSET)"
CONFIG_FSL_SERDES=y
CONFIG_NAND_BR_PRELIM="(BR_PHYS_ADDR(CONFIG_SYS_NAND_BASE_PHYS) | (2<<BR_DECC_SHIFT) | BR_PS_8 | BR_MS_FCM | BR_V)"
CONFIG_ID_EEPROM=y
CONFIG_SYS_I2C_SPEED=400000
CONFIG_PCIE1=y
CONFIG_PCIE2=y
CONFIG_P2020=y
CONFIG_SPI_FLASH_SPANSION=y
CONFIG_SYS_PCIE2_IO_VIRT="0xffc20000"
CONFIG_SYS_FLASH_BANKS_LIST="{CONFIG_SYS_FLASH_BASE_PHYS}"
CONFIG_SYS_MPC85xx_PCIX_OFFSET="0x8000"
CONFIG_CMD_FLASH=y
CONFIG_CMD_SAVEENV=y
CONFIG_ENV_SECT_SIZE="0x10000"
CONFIG_SYS_NS16550_SERIAL=y
CONFIG_I2C_MULTI_BUS=y
CONFIG_SYS_RTC_DS1337_NOOSC=y
CONFIG_SYS_MPC85xx_CPM_OFFSET="0x80000"
CONFIG_SYS_FSL_CORENET_CCM_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_FSL_CORENET_CCM_OFFSET)"
CONFIG_CMD_NAND=y
CONFIG_SYS_PCIE1_MEM_SIZE="0x20000000"
CONFIG_SYS_MPC85xx_SATA1_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_SATA1_OFFSET)"
CONFIG_SYS_MPC85xx_DDR_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_DDR_OFFSET)"
CONFIG_SYS_DDR_SDRAM_BASE="0x00000000"
CONFIG_SYS_MPC85xx_DDR_OFFSET="0x2000"
CONFIG_SYS_I2C_OFFSET="0x3000"
CONFIG_CMD_MEMORY=y
CONFIG_SYS_MAXARGS=16
CONFIG_CMD_RUN=y
CONFIG_SYS_I2C_RTC_ADDR="0x68"
CONFIG_SYS_PBSIZE="(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)"
CONFIG_SYS_PCIE2_MEM_SIZE="0x20000000"
CONFIG_CHIP_SELECTS_PER_CTRL=y
CONFIG_EHCI_HCD_INIT_AFTER_RESET=y
CONFIG_SYS_BR1_PRELIM="CONFIG_NAND_BR_PRELIM"
CONFIG_OF_LIBFDT=y
CONFIG_SYS_MPC85xx_GUTS_OFFSET="0xE0000"
CONFIG_SYS_OR2_PRELIM="(OR_AM_128KB | OR_GPCM_CSNT | OR_GPCM_XACS | OR_GPCM_SCY_15 | OR_GPCM_SETA | OR_GPCM_TRLX | OR_GPCM_EHTR | OR_GPCM_EAD)"
CONFIG_SYS_PCIE2_MEM_VIRT="0xa0000000"
CONFIG_FSL_PCIE_RESET=y
CONFIG_LOADS_ECHO=y
CONFIG_SYS_MPC85xx_USB2_OFFSET="0x23000"
CONFIG_CMDLINE_EDITING=y
CONFIG_CMD_USB=y
CONFIG_FLASH_CFI_DRIVER=y
CONFIG_SYS_NAND_U_BOOT_SIZE="((512 << 10) + CONFIG_SYS_NAND_SPL_SIZE)"
CONFIG_SYS_MPC85xx_SATA2_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_SATA2_OFFSET)"
CONFIG_SYS_CONSOLE_IS_IN_ENV=y
CONFIG_CMD_EXT2=y
CONFIG_USB_EHCI=y
CONFIG_BOOTCOMMAND="CONFIG_HDBOOT"
CONFIG_OF_STDOUT_VIA_ALIAS=y
CONFIG_TSEC1_NAME="eTSEC1"
CONFIG_CMD_SETGETDCR=y
CONFIG_FSL_PCI_INIT=y
CONFIG_SYS_NAND_U_BOOT_OFFS="(0)"
CONFIG_MK_P2020RDB=y
CONFIG_SYS_PCIE1_IO_SIZE="0x00010000"
CONFIG_LOADADDR=1000000
CONFIG_SYS_MPC85xx_LBC_OFFSET="0x5000"
CONFIG_VSC7385_IMAGE_SIZE=8192
CONFIG_ETHPRIME="eTSEC1"
CONFIG_SYS_FLASH_WRITE_TOUT=500
CONFIG_CMD_BOOTD=y
CONFIG_NET_MULTI=y
CONFIG_SYS_NS16550=y
CONFIG_SYS_MPC85xx_ESDHC_OFFSET="0x2e000"
CONFIG_SYS_BARGSIZE="CONFIG_SYS_CBSIZE"
CONFIG_MAX_MEM_MAPPED="((phys_size_t)2 << 30)"
CONFIG_PANIC_HANG=y
CONFIG_MP=y
CONFIG_SYS_PCIE1_IO_VIRT="0xffc30000"
CONFIG_SYS_HZ=1000
CONFIG_SYS_I2C2_OFFSET="0x3100"
CONFIG_RAMBOOT_TEXT_BASE="0x11000000"
CONFIG_SYS_NS16550_CLK="get_bus_freq(0)"
CONFIG_DOS_PARTITION=y
CONFIG_CMD_SF=y
CONFIG_SYS_FSL_CORENET_CLK_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_FSL_CORENET_CLK_OFFSET)"
CONFIG_SYS_LOADS_BAUD_CHANGE=y
CONFIG_RAMBOOT_SPIFLASH=y
CONFIG_CMD_FPGA=y
CONFIG_SYS_FSL_ESDHC_ADDR="CONFIG_SYS_MPC85xx_ESDHC_ADDR"
CONFIG_SYS_PCIE1_MEM_VIRT="0xc0000000"
CONFIG_TSEC2_NAME="eTSEC2"
CONFIG_SYS_VSC7385_BASE="0xffb00000"
CONFIG_SYS_BAUDRATE_TABLE="{300, 600, 1200, 2400, 4800, 9600, 19200, 38400,115200}"
CONFIG_HDBOOT="setenv bootargs root=/dev/$bdev rw rootdelay=30 console=$consoledev,$baudrate $othbootargs;usb start;ext2load usb 0:1 $loadaddr /boot/$bootfile;ext2load usb 0:1 $fdtaddr /boot/$fdtfile;bootm $loadaddr - $fdtaddr"
CONFIG_SYS_NS16550_COM1="(CONFIG_SYS_CCSRBAR+0x4500)"
CONFIG_SYS_NS16550_COM2="(CONFIG_SYS_CCSRBAR+0x4600)"
CONFIG_SYS_VSC7385_BASE_PHYS="CONFIG_SYS_VSC7385_BASE"
CONFIG_SYS_PCIE1_ADDR="(CONFIG_SYS_CCSRBAR+0xa000)"
CONFIG_E500=y
CONFIG_PCI_SCAN_SHOW=y
CONFIG_SYS_BR0_PRELIM="CONFIG_FLASH_BR_PRELIM"
CONFIG_TSEC1=y
CONFIG_TSEC2=y
CONFIG_TSEC3=y
CONFIG_SYS_CCSRBAR_DEFAULT="0xff700000"
CONFIG_SYS_HUSH_PARSER=y
CONFIG_SYS_OR1_PRELIM="CONFIG_NAND_OR_PRELIM"
CONFIG_SYS_INIT_RAM_LOCK=y
CONFIG_SYS_SDRAM_BASE="CONFIG_SYS_DDR_SDRAM_BASE"
CONFIG_RAMBOOTCOMMAND="setenv bootargs root=/dev/ram rw console=$consoledev,$baudrate $othbootargs; tftp $ramdiskaddr $ramdiskfile;tftp $loadaddr $bootfile;tftp $fdtaddr $fdtfile;bootm $loadaddr $ramdiskaddr $fdtaddr"
CONFIG_SYS_PCI_64BIT=y
CONFIG_FLASH_OR_PRELIM="0xff000ff7"
CONFIG_SYS_PROMPT_HUSH_PS2="> "
CONFIG_CMD_ECHO=y
CONFIG_GENERIC_MMC=y
CONFIG_SYS_INIT_SP_OFFSET="CONFIG_SYS_GBL_DATA_OFFSET"
CONFIG_HOSTNAME="P2020RDB"
CONFIG_ENV_SPI_MODE=0
CONFIG_SYS_DDR_SBE="0x00FF0000"
CONFIG_TSEC3_NAME="eTSEC3"
CONFIG_SYS_FLASH_EMPTY_INFO=y
CONFIG_VSC7385_ENET=y
CONFIG_SYS_INIT_RAM_ADDR="0xffd00000"
CONFIG_SYS_PCIE2_ADDR="(CONFIG_SYS_CCSRBAR+0x9000)"
CONFIG_SYS_NAND_DDR_LAW=11
CONFIG_SYS_MONITOR_BASE="TEXT_BASE"
CONFIG_I2C_CMD_TREE=y
CONFIG_ENV_SPI_MAX_HZ=10000000
CONFIG_EXTRA_ENV_SETTINGS="netdev=eth0\0uboot=" MK_STR(CONFIG_UBOOTPATH) "\0loadaddr=1000000\0tftpflash=tftpboot $loadaddr $uboot; protect off " MK_STR(TEXT_BASE) " +$filesize; erase " MK_STR(TEXT_BASE) " +$filesize; cp.b $loadaddr " MK_STR(TEXT_BASE) " $filesize; protect on " MK_STR(TEXT_BASE) " +$filesize; cmp.b $loadaddr " MK_STR(TEXT_BASE) " $filesize\0consoledev=ttyS0\0ramdiskaddr=2000000\0ramdiskfile=rootfs.ext2.gz.uboot\0fdtaddr=c00000\0fdtfile="PLATFORM_DTB"\0bdev=sda1\0hwconfig=esdhc\0jffs2nor=mtdblock3\0norbootaddr=ef080000\0norfdtaddr=ef040000\0jffs2nand=mtdblock9\0nandbootaddr=200000\0nandfdtaddr=100000\0nandimgsize=400000\0nandfdtsize=100000\0usb_phy_type=ulpi\0vscfw_addr=ef000000\0othbootargs=ramdisk_size="MK_STR(RAMDISK_SIZE)" cache-sram-size=0x10000\0usbfatboot=setenv bootargs root=/dev/ram rw console=$consoledev,$baudrate $othbootargs; usb start;fatload usb 0:2 $loadaddr $bootfile;fatload usb 0:2 $fdtaddr $fdtfile;fatload usb 0:2 $ramdiskaddr $ramdiskfile;bootm $loadaddr $ramdiskaddr $fdtaddr\0usbext2boot=setenv bootargs root=/dev/ram rw console=$consoledev,$baudrate $othbootargs; usb start;ext2load usb 0:4 $loadaddr $bootfile;ext2load usb 0:4 $fdtaddr $fdtfile;ext2load usb 0:4 $ramdiskaddr $ramdiskfile;bootm $loadaddr $ramdiskaddr $fdtaddr\0norboot=setenv bootargs root=/dev/$jffs2nor rw console=$consoledev,$baudrate rootfstype=jffs2 $othbootargs;bootm $norbootaddr - $norfdtaddr\0nandboot=setenv bootargs root=/dev/$jffs2nand rw rootfstype=jffs2 console=$consoledev,$baudrate $othbootargs;nand read 2000000 $nandbootaddr $nandimgsize;nand read 3000000 $nandfdtaddr $nandfdtsize;bootm 2000000 - 3000000;\0sdboot=setenv bdev mmcblk0p2;setenv bootargs root=/dev/$bdev rw rootdelay=30 console=$consoledev,$baudrate $othbootargs;mmcinfo;ext2load mmc 0:2 $loadaddr /boot/$bootfile;ext2load mmc 0:2 $fdtaddr /boot/$fdtfile;bootm $loadaddr - $fdtaddr;\0"
CONFIG_SYS_CCSRBAR="0xffe00000"
CONFIG_BOOTFILE="uImage"
CONFIG_SYS_TSEC1_OFFSET="0x24000"
CONFIG_SYS_NAND_U_BOOT_RELOC="0x00010000"
CONFIG_FSL_ESDHC=y
CONFIG_SYS_PCIE2_IO_BUS="0x00000000"
CONFIG_E1000=y
CONFIG_SYS_NAND_BASE_LIST="{CONFIG_SYS_NAND_BASE}"
CONFIG_BAUDRATE=115200
CONFIG_SYS_I2C_EEPROM_ADDR_LEN=y
CONFIG_NUM_DDR_CONTROLLERS=y
CONFIG_SYS_BOOTMAPSZ="(16 << 20)"
CONFIG_ENV_IS_IN_SPI_FLASH=y
CONFIG_SYS_MPC85xx_GPIO_OFFSET="0xF000"
CONFIG_SYS_64BIT_VSPRINTF=y
CONFIG_RELOC_FIXUP_WORKS=y
CONFIG_HWCONFIG=y
CONFIG_SYS_MPC85xx_DDR2_OFFSET="0x6000"
CONFIG_OF_BOARD_SETUP=y
CONFIG_SYS_MPC85xx_SERDES2_OFFSET="0xE3100"
CONFIG_ADDR_STREAMING=y
CONFIG_SYS_NAND_SPL_SIZE="0x1000"
CONFIG_SYS_FLASH_CFI=y
CONFIG_CMD_PCI=y
CONFIG_SERIAL_MULTI=y
CONFIG_SYS_NS16550_REG_SIZE=y
CONFIG_FSL_DDR2=y
CONFIG_SYS_MEMTEST_END="0x1fffffff"
CONFIG_CMD_I2C=y
CONFIG_FIT_VERBOSE=y
CONFIG_SYS_FSL_CORENET_RCPM_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_FSL_CORENET_RCPM_OFFSET)"
CONFIG_SYS_MPC85xx_ESDHC_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_ESDHC_OFFSET)"
CONFIG_MAX_CPUS=2
CONFIG_CMD_ELF=y
CONFIG_SYS_FSL_CORENET_BMAN_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_FSL_CORENET_BMAN_OFFSET)"
CONFIG_SYS_OR0_PRELIM="CONFIG_FLASH_OR_PRELIM"
CONFIG_CMD_DATE=y
CONFIG_SYS_MPC85xx_SATA1_OFFSET="0x18000"
CONFIG_SYS_DEF_EEPROM_ADDR="CONFIG_SYS_I2C_EEPROM_ADDR"
CONFIG_TSEC_ENET=y
CONFIG_SYS_FLASH_BASE="0xef000000"
CONFIG_SYS_I2C_SLAVE="0x7F"
CONFIG_MII_DEFAULT_TSEC=y
CONFIG_SYS_MPC85xx_DDR2_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_MPC85xx_DDR2_OFFSET)"
CONFIG_SYS_MAX_FLASH_BANKS=y
CONFIG_CMD_IMLS=y
CONFIG_CMD_SOURCE=y
CONFIG_PCI_PNP=y
CONFIG_SYS_PROMPT="=> "
CONFIG_USB_STORAGE=y
CONFIG_SYS_DDR_ERR_INT_EN="0x0000000d"
CONFIG_SYS_NAND_U_BOOT_RELOC_SP="(CONFIG_SYS_NAND_U_BOOT_RELOC + 0x10000)"
CONFIG_SYS_I2C_EEPROM_ADDR="0x52"
CONFIG_SYS_MEMTEST_START="0x00000000"
CONFIG_CMD_LOADB=y
CONFIG_CMD_LOADS=y
CONFIG_SYS_FSL_BOOT_DDR=y
CONFIG_SYS_DDR_ERR_DIS="0x00000000"
CONFIG_CMD_IMI=y
CONFIG_HARD_SPI=y
CONFIG_NFSBOOTCOMMAND="setenv bootargs root=/dev/nfs rw nfsroot=$serverip:$rootpath ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:$netdev:off console=$consoledev,$baudrate $othbootargs;tftp $loadaddr $bootfile;tftp $fdtaddr $fdtfile;bootm $loadaddr - $fdtaddr"
CONFIG_SYS_PCIE2_IO_PHYS="0xffc20000"
CONFIG_SF_DEFAULT_SPEED=10000000
CONFIG_CONS_INDEX=y
CONFIG_SYS_PCIE2_MEM_BUS="0xa0000000"
CONFIG_SYS_PCIE1_IO_BUS="0x00000000"
CONFIG_SYS_NAND_BASE_PHYS="CONFIG_SYS_NAND_BASE"
CONFIG_CMD_IRQ=y
CONFIG_ENV_SPI_BUS=0
CONFIG_SYS_SDRAM_SIZE=1024
CONFIG_USB_EHCI_FSL=y
CONFIG_MPC85xx=y
CONFIG_SYS_MAX_FLASH_SECT=128
CONFIG_ENABLE_36BIT_PHYS=y
CONFIG_SYS_FSL_ESDHC_BROKEN_TIMEOUT=y
CONFIG_SYS_I2C_EEPROM_ADDR2="0x18"
CONFIG_SYS_FLASH_QUIET_TEST=y
CONFIG_CMD_BDI=y
CONFIG_SYS_FLASH_AMD_CHECK_DQ7=y
CONFIG_NAND_FSL_ELBC=y
CONFIG_NAND_OR_PRELIM="(0xFFF80000 | OR_FCM_CSCT | OR_FCM_CST | OR_FCM_CHT | OR_FCM_SCY_1 | OR_FCM_TRLX | OR_FCM_EHTR)"
CONFIG_SYS_MPC85xx_SERDES1_OFFSET="0xE3000"
CONFIG_SYS_DDR_TLB_START=9
CONFIG_FLASH_SHOW_PROGRESS=45
CONFIG_SYS_FSL_CORENET_QMAN_ADDR="(CONFIG_SYS_IMMR + CONFIG_SYS_FSL_CORENET_QMAN_OFFSET)"
CONFIG_DIMM_SLOTS_PER_CTLR=y
CONFIG_PCI=y
CONFIG_UBOOTPATH="u-boot.bin"
CONFIG_FLASH_BR_PRELIM="(BR_PHYS_ADDR(CONFIG_SYS_FLASH_BASE_PHYS) | BR_PS_16 | BR_V)"
CONFIG_CMD_MII=y
CONFIG_HAS_ETH0=y
CONFIG_HAS_ETH1=y
CONFIG_HAS_ETH2=y
CONFIG_SYS_FLASH_ERASE_TOUT=60000
CONFIG_SYS_MPC85xx_L2_OFFSET="0x20000"
CONFIG_CMD_MMC=y
CONFIG_SYS_NAND_U_BOOT_START="0x01000000"
CONFIG_MK_SPIFLASH=y
CONFIG_RTC_DS1337=y
CONFIG_SYS_MDIO1_OFFSET="0x24000"
