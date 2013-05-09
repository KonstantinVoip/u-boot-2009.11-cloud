##############################################################################################
# Start of default section
#
TRGT = powerpc-linux-gnu-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O srec 
AR   = $(TRGT)ar
LD   = $(TRGT)ld

#В дальнейшем должен делать мой парсер!!!!!
UNDEF_SYM = -u__u_boot_cmd_base  -u__u_boot_cmd_bdinfo  -u__u_boot_cmd_boot  -u__u_boot_cmd_bootd     \
-u__u_boot_cmd_bootelf  -u__u_boot_cmd_bootm  -u__u_boot_cmd_bootp  -u__u_boot_cmd_bootvx  -u__u_boot_cmd_clocks   \
-u__u_boot_cmd_cmp  -u__u_boot_cmd_coninfo  -u__u_boot_cmd_cp  -u__u_boot_cmd_cpu  -u__u_boot_cmd_crc32    \
-u__u_boot_cmd_date  -u__u_boot_cmd_echo  -u__u_boot_cmd_editenv  -u__u_boot_cmd_end  -u__u_boot_cmd_erase     \
-u__u_boot_cmd_exit  -u__u_boot_cmd_ext2load  -u__u_boot_cmd_ext2ls  -u__u_boot_cmd_fatinfo  -u__u_boot_cmd_fatload    \
-u__u_boot_cmd_fatls  -u__u_boot_cmd_fdt  -u__u_boot_cmd_flinfo  -u__u_boot_cmd_go  -u__u_boot_cmd_help  -u__u_boot_cmd_i2c      \
-u__u_boot_cmd_iminfo  -u__u_boot_cmd_imls  -u__u_boot_cmd_imxtract  -u__u_boot_cmd_interrupts  -u__u_boot_cmd_irqinfo         \
-u__u_boot_cmd_itest  -u__u_boot_cmd_loadb  -u__u_boot_cmd_loads  -u__u_boot_cmd_loady  -u__u_boot_cmd_loop  -u__u_boot_cmd_mac    \
-u__u_boot_cmd_md  -u__u_boot_cmd_mii  -u__u_boot_cmd_mm  -u__u_boot_cmd_mmc  -u__u_boot_cmd_mmcinfo  -u__u_boot_cmd_mtest      \
-u__u_boot_cmd_mw  -u__u_boot_cmd_nand  -u__u_boot_cmd_nboot  -u__u_boot_cmd_nfs  -u__u_boot_cmd_nm  -u__u_boot_cmd_pci        \
-u__u_boot_cmd_ping  -u__u_boot_cmd_printenv  -u__u_boot_cmd_protect  -u__u_boot_cmd_question_mark  -u__u_boot_cmd_rarpboot   \
-u__u_boot_cmd_reset  -u__u_boot_cmd_run  -u__u_boot_cmd_saveenv  -u__u_boot_cmd_setenv  -u__u_boot_cmd_setexpr        \
-u__u_boot_cmd_sf  -u__u_boot_cmd_showvar  -u__u_boot_cmd_sleep  -u__u_boot_cmd_source  -u__u_boot_cmd_start        \
-u__u_boot_cmd_test  -u__u_boot_cmd_tftpboot  -u__u_boot_cmd_usb  -u__u_boot_cmd_usbboot  -u__u_boot_cmd_version 



 
#///////////////////////////////////////// Directory///////////////////////////////////
#Общая директория где лежит U-Boot
BASE_DIR = C:\u-boot-2009.11-cloud

#Расположение cpu\mpc85xx
MPC85xx_DIR = C:\u-boot-2009.11-cloud\cpu\mpc85xx

#Расположение \lib_ppc
LIBPPC_DIR =  C:\u-boot-2009.11-cloud\lib_ppc

#Расположение \net
LIBNET_DIR =  C:\u-boot-2009.11-cloud\net
#Сегодня 22
#Расположение drivers\i2c
LIBI2C_DIR =  C:\u-boot-2009.11-cloud\drivers\i2c
#Расположение drivers\misc
LIBMISC_DIR = C:\u-boot-2009.11-cloud\drivers\misc
#Расположение drivers\mmc
LIBMMC_DIR =  C:\u-boot-2009.11-cloud\drivers\mmc
#Расположение drivers\mtd
LIBMTD_DIR =  C:\u-boot-2009.11-cloud\drivers\mtd
#Расположение drivers\mtd\nand
LIBNAND_DIR = C:\u-boot-2009.11-cloud\drivers\mtd\nand
#Расположение drivers\mtd\spi
LIBSPI_DIR  = C:\u-boot-2009.11-cloud\drivers\mtd\spi
#Расположение drivers\spi
LIBFSLSPI_DIR = C:\u-boot-2009.11-cloud\drivers\spi
#Расположение drivers\net
LIBDRNET_DIR = C:\u-boot-2009.11-cloud\drivers\net
#Расположение drivers\pci
LIBPCI_DIR = C:\u-boot-2009.11-cloud\drivers\pci
#Расположение drivers\rtc
LIBRTC_DIR = C:\u-boot-2009.11-cloud\drivers\rtc
#Расположение drivers\serial
LIBSERIAL_DIR = C:\u-boot-2009.11-cloud\drivers\serial
#Расположение drivers\usb\host
LIBUSB_DIR = C:\u-boot-2009.11-cloud\drivers\usb\host
#Расположение cpu\mpc8xxx
LIBCPU8xxx_DIR = C:\u-boot-2009.11-cloud\cpu\mpc8xxx
#Расположение \common
LIBCOMMON_DIR = C:\u-boot-2009.11-cloud\common
#Расположение \libfdt
LIBFDT_DIR = C:\u-boot-2009.11-cloud\libfdt
#Расположение \board\freescale\p1_p2_rdb
LIBP1P2RDB_DIR = C:\u-boot-2009.11-cloud\board\freescale\p1_p2_rdb
#Расположение \board\freescale\common
LIBRDBCOMMON_DIR = C:\u-boot-2009.11-cloud\board\freescale\common
#Расположение \lib_generic
LIBGENERIC_DIR = C:\u-boot-2009.11-cloud\lib_generic
#Расположение \fs\fat
LIBFAT_DIR = C:\u-boot-2009.11-cloud\fs\fat
#Расположение \fs\ext2
LIBEXT2_DIR = C:\u-boot-2009.11-cloud\fs\ext2
#Расположение \disk
LIBDISK_DIR = C:\u-boot-2009.11-cloud\disk


#//////////////////////////////ALL Assembler Librari///////////////////////////////
LIBS = $(MPC85xx_DIR)\libmpc85xx.a  $(LIBPPC_DIR)\libppc.a  $(LIBNET_DIR)\libnet.a      \
$(LIBI2C_DIR)\libi2c.a  $(LIBMISC_DIR)\libmisc.a  $(LIBMMC_DIR)\libmmc.a  $(LIBMTD_DIR)\libmtd.a   \
$(LIBNAND_DIR)\libnand.a  $(LIBSPI_DIR)\libspi_flash.a  $(LIBFSLSPI_DIR)\libspi.a  \
$(LIBDRNET_DIR)\libnet.a  $(LIBPCI_DIR)\libpci.a  $(LIBRTC_DIR)\librtc.a  $(LIBSERIAL_DIR)\libserial.a  \
$(LIBUSB_DIR)\libusb_host.a  $(LIBCPU8xxx_DIR)\lib8xxx.a   $(LIBCOMMON_DIR)\libcommon.a  $(LIBFDT_DIR)\libfdt.a  \
$(LIBP1P2RDB_DIR)\libp1_p2_rdb.a  $(LIBRDBCOMMON_DIR)\libfreescale.a  $(LIBGENERIC_DIR)\libgeneric.a      \
$(LIBFAT_DIR)\libfat.a  $(LIBEXT2_DIR)\libext2fs.a  $(LIBDISK_DIR)\libdisk.a
   



#/////////////////////////////List C source files here/////////////////////////////////
#Файлы СИ для сборки библиотеки  libmpc85xx.a     
SRC_MPC85xx = $(MPC85xx_DIR)\release.c  \
$(MPC85xx_DIR)\ddr-gen3.c  $(MPC85xx_DIR)\fdt.c  $(MPC85xx_DIR)\mp.c $(MPC85xx_DIR)\pci.c  \
$(MPC85xx_DIR)\serdes.c  $(MPC85xx_DIR)\p2020_serdes.c  $(MPC85xx_DIR)\cpu.c   \
$(MPC85xx_DIR)\cpu_init.c  $(MPC85xx_DIR)\cpu_init_early.c  $(MPC85xx_DIR)\interrupts.c   \
$(MPC85xx_DIR)\speed.c  $(MPC85xx_DIR)\tlb.c  $(MPC85xx_DIR)\traps.c   

#Файлы СИ для сборки библиотеки  libppc.a 
SRC_LIBPPC = $(LIBPPC_DIR)\ppccache.c  $(LIBPPC_DIR)\ppcstring.c  $(LIBPPC_DIR)\ticks.c   \
$(LIBPPC_DIR)\bat_rw.c  $(LIBPPC_DIR)\board.c  $(LIBPPC_DIR)\bootm.c     \
$(LIBPPC_DIR)\cache.c  $(LIBPPC_DIR)\extable.c $(LIBPPC_DIR)\interrupts.c    \
$(LIBPPC_DIR)\time.c

#Файлы СИ для сборки библиотеки  libnet.a
SRC_LIBNET = $(LIBNET_DIR)\bootp.c  $(LIBNET_DIR)\eth.c  $(LIBNET_DIR)\net.c  \
$(LIBNET_DIR)\nfs.c   $(LIBNET_DIR)\rarp.c  $(LIBNET_DIR)\tftp.c

#Файлы СИ для сборки библиотеки  libi2c.a
SRC_LIBI2C = $(LIBI2C_DIR)\fsl_i2c.c

#Файлы СИ для сборки библиотеки  libmisc.a
SRC_LIBMISC = $(LIBMISC_DIR)\fsl_law.c

#Файлы СИ для сборки библиотеки  libmmc.a
SRC_LIBMMC = $(LIBMMC_DIR)\mmc.c  $(LIBMMC_DIR)\fsl_esdhc.c

#Файлы СИ для сборки библиотеки  libmtd.a
SRC_LIBMTD = $(LIBMTD_DIR)\cfi_flash.c

#Файлы СИ для сборки библиотеки libnand.a
SRC_LIBNAND = $(LIBNAND_DIR)\nand.c  $(LIBNAND_DIR)\nand_base.c  $(LIBNAND_DIR)\nand_bbt.c     \
$(LIBNAND_DIR)\nand_ecc.c  $(LIBNAND_DIR)\nand_ids.c  $(LIBNAND_DIR)\nand_util.c     \
$(LIBNAND_DIR)\fsl_elbc_nand.c 

#Файлы СИ для сборки библиотеки libspi_flash.a
SRC_LIBSPI = $(LIBSPI_DIR)\spi_flash.c  $(LIBSPI_DIR)\spansion.c

#Файлы СИ для сборки библиотеки libspi.a
SRC_LIBFSLSPI = $(LIBFSLSPI_DIR)\fsl_espi.c

#Файлы СИ для сборки библиотеки libnet.a это другая библиотека!!!
SRC_LIBDRNET = $(LIBDRNET_DIR)\e1000.c  $(LIBDRNET_DIR)\tsec.c   $(LIBDRNET_DIR)\vsc7385.c

#Файлы СИ для сборки библиотеки libpci.a 
SRC_LIBPCI = $(LIBPCI_DIR)\fsl_pci_init.c  $(LIBPCI_DIR)\pci.c   $(LIBPCI_DIR)\pci_auto.c  $(LIBPCI_DIR)\pci_indirect.c

#Файлы СИ для сборки библиотеки librtc.a
SRC_LIBRTC = $(LIBRTC_DIR)\date.c  $(LIBRTC_DIR)\ds1337.c

#Файлы СИ для сборки библиотеки libserial.a
SRC_LIBSERIAL = $(LIBSERIAL_DIR)\ns16550.c  $(LIBSERIAL_DIR)\serial.c

#Файлы СИ для сборки библиотеки libusb_host.a
SRC_LIBUSB = $(LIBUSB_DIR)\ehci-hcd.c  $(LIBUSB_DIR)\ehci-fsl.c

#Файлы СИ для сборки библиотеки lib8xxx.a
SRC_LIBCPU8xxx = $(LIBCPU8xxx_DIR)\cpu.c  $(LIBCPU8xxx_DIR)\fdt.c  $(LIBCPU8xxx_DIR)\pci_cfg.c

#Файлы СИ для сборки библиотеки libcommon.a
SRC_LIBCOMMON = $(LIBCOMMON_DIR)\cmd_bdinfo.c  $(LIBCOMMON_DIR)\cmd_boot.c $(LIBCOMMON_DIR)\cmd_bootm.c    \
$(LIBCOMMON_DIR)\cmd_console.c  $(LIBCOMMON_DIR)\cmd_date.c  $(LIBCOMMON_DIR)\cmd_elf.c  $(LIBCOMMON_DIR)\cmd_ext2.c   \
$(LIBCOMMON_DIR)\cmd_fat.c  $(LIBCOMMON_DIR)\cmd_fdt.c  $(LIBCOMMON_DIR)\cmd_flash.c  $(LIBCOMMON_DIR)\cmd_i2c.c   \
$(LIBCOMMON_DIR)\cmd_irq.c  $(LIBCOMMON_DIR)\cmd_itest.c  $(LIBCOMMON_DIR)\cmd_load.c  $(LIBCOMMON_DIR)\cmd_mac.c   \
$(LIBCOMMON_DIR)\cmd_mem.c  $(LIBCOMMON_DIR)\cmd_mii.c  $(LIBCOMMON_DIR)\cmd_misc.c  $(LIBCOMMON_DIR)\cmd_mmc.c   \
$(LIBCOMMON_DIR)\cmd_mp.c  $(LIBCOMMON_DIR)\cmd_nand.c  $(LIBCOMMON_DIR)\cmd_net.c  $(LIBCOMMON_DIR)\cmd_nvedit.c   \
$(LIBCOMMON_DIR)\cmd_pci.c  $(LIBCOMMON_DIR)\cmd_pcmcia.c  $(LIBCOMMON_DIR)\cmd_setexpr.c  $(LIBCOMMON_DIR)\cmd_sf.c   \
$(LIBCOMMON_DIR)\cmd_source.c  $(LIBCOMMON_DIR)\cmd_usb.c  $(LIBCOMMON_DIR)\cmd_ximg.c  $(LIBCOMMON_DIR)\command.c   \
$(LIBCOMMON_DIR)\console.c  $(LIBCOMMON_DIR)\dlmalloc.c  $(LIBCOMMON_DIR)\env_common.c  $(LIBCOMMON_DIR)\env_sf.c   \
$(LIBCOMMON_DIR)\exports.c  $(LIBCOMMON_DIR)\fdt_support.c  $(LIBCOMMON_DIR)\flash.c  $(LIBCOMMON_DIR)\hush.c   \
$(LIBCOMMON_DIR)\hwconfig.c  $(LIBCOMMON_DIR)\image.c  $(LIBCOMMON_DIR)\main.c  $(LIBCOMMON_DIR)\memsize.c   \
$(LIBCOMMON_DIR)\miiphyutil.c  $(LIBCOMMON_DIR)\s_record.c  $(LIBCOMMON_DIR)\serial.c  $(LIBCOMMON_DIR)\stdio.c   \
$(LIBCOMMON_DIR)\usb.c  $(LIBCOMMON_DIR)\usb_storage.c  $(LIBCOMMON_DIR)\xyzModem.c


#Файлы СИ для сборки библиотеки libfdt.a
SRC_LIBFDT = $(LIBFDT_DIR)\fdt.c  $(LIBFDT_DIR)\fdt_ro.c  $(LIBFDT_DIR)\fdt_rw.c  $(LIBFDT_DIR)\fdt_strerror.c    \
$(LIBFDT_DIR)\fdt_sw.c  $(LIBFDT_DIR)\fdt_wip.c

#Файлы СИ для сборки библиотеки libp1_p2_rdb.a
SRC_LIBP1P2RDB = $(LIBP1P2RDB_DIR)\p1_p2_rdb.c  $(LIBP1P2RDB_DIR)\ddr.c   $(LIBP1P2RDB_DIR)\law.c     \
$(LIBP1P2RDB_DIR)\tlb.c  

#Файлы СИ для сборки библиотеки libfreescale.a
SRC_LIBRDBCOMMON = $(LIBRDBCOMMON_DIR)\sys_eeprom.c

#Файлы СИ для сборки библиотеки libgeneric.a
SRC_LIBGENERIC = $(LIBGENERIC_DIR)\crc16.c  $(LIBGENERIC_DIR)\crc32.c  $(LIBGENERIC_DIR)\ctype.c      \
$(LIBGENERIC_DIR)\display_options.c  $(LIBGENERIC_DIR)\div64.c  $(LIBGENERIC_DIR)\gunzip.c            \
$(LIBGENERIC_DIR)\lmb.c $(LIBGENERIC_DIR)\ldiv.c  $(LIBGENERIC_DIR)\md5.c  $(LIBGENERIC_DIR)\sha1.c  $(LIBGENERIC_DIR)\string.c  $(LIBGENERIC_DIR)\strmhz.c  $(LIBGENERIC_DIR)\vsprintf.c  $(LIBGENERIC_DIR)\zlib.c  

#Файлы СИ для сборки библиотеки libfat.a
SRC_LIBFAT = $(LIBFAT_DIR)\fat.c  $(LIBFAT_DIR)\file.c
 
#Файлы СИ для сборки библиотеки libext2fs.a
SRC_LIBEXT2 = $(LIBEXT2_DIR)\ext2fs.c  $(LIBEXT2_DIR)\dev.c


#Файлы СИ для сборки библиотеки libdisk.a
SRC_LIBDISK = $(LIBDISK_DIR)\part.c  $(LIBDISK_DIR)\part_dos.c



 
 

#/////////////////////////////List O Objects files here/////////////////////////////////
#Объектные файлы для сборки библиотеки libmpc85xx.a
SRO_MPC85xx = $(MPC85xx_DIR)\release.o   \
$(MPC85xx_DIR)\ddr-gen3.o  $(MPC85xx_DIR)\fdt.o $(MPC85xx_DIR)\mp.o $(MPC85xx_DIR)\pci.o  \
$(MPC85xx_DIR)\serdes.o  $(MPC85xx_DIR)\p2020_serdes.o  $(MPC85xx_DIR)\cpu.o   \
$(MPC85xx_DIR)\cpu_init.o  $(MPC85xx_DIR)\cpu_init_early.o  $(MPC85xx_DIR)\interrupts.o   \
$(MPC85xx_DIR)\speed.o  $(MPC85xx_DIR)\tlb.o  $(MPC85xx_DIR)\traps.o


#Объектные файлы для сборки библиотеки libppc.a
SRO_LIBPPC = $(LIBPPC_DIR)\ppccache.o  $(LIBPPC_DIR)\ppcstring.o  $(LIBPPC_DIR)\ticks.o   \
$(LIBPPC_DIR)\bat_rw.o  $(LIBPPC_DIR)\board.o  $(LIBPPC_DIR)\bootm.o     \
$(LIBPPC_DIR)\cache.o  $(LIBPPC_DIR)\extable.o $(LIBPPC_DIR)\interrupts.o    \
$(LIBPPC_DIR)\time.o

#Объектные файлы для сборки библиотеки  libnet.a
SRO_LIBNET = $(LIBNET_DIR)\bootp.o  $(LIBNET_DIR)\eth.o  $(LIBNET_DIR)\net.o  \
$(LIBNET_DIR)\nfs.o   $(LIBNET_DIR)\rarp.o  $(LIBNET_DIR)\tftp.o

#Объектные файлы для сборки библиотеки  libi2c.a
SRO_LIBI2C = $(LIBI2C_DIR)\fsl_i2c.o

#Объектные файлы для сборки библиотеки  libmisc.a
SRO_LIBMISC = $(LIBMISC_DIR)\fsl_law.o

#Объектные файлы для сборки библиотеки  libmmc.a
SRO_LIBMMC = $(LIBMMC_DIR)\mmc.o  $(LIBMMC_DIR)\fsl_esdhc.o

#Объектные файлы для сборки библиотеки  libmtd.a
SRO_LIBMTD = $(LIBMTD_DIR)\cfi_flash.o

#Объектные файлы для сборки библиотеки libnand.a
SRO_LIBNAND = $(LIBNAND_DIR)\nand.o  $(LIBNAND_DIR)\nand_base.o  $(LIBNAND_DIR)\nand_bbt.o     \
$(LIBNAND_DIR)\nand_ecc.o  $(LIBNAND_DIR)\nand_ids.o  $(LIBNAND_DIR)\nand_util.o     \
$(LIBNAND_DIR)\fsl_elbc_nand.o 

#Объектные файлы для сборки библиотеки libspi_flash.a
SRO_LIBSPI = $(LIBSPI_DIR)\spi_flash.o  $(LIBSPI_DIR)\spansion.o

#Объектные файлы для сборки библиотеки libspi.a
SRO_LIBFSLSPI = $(LIBFSLSPI_DIR)\fsl_espi.o

#Объектные файлы для сборки библиотеки libnet.a это другая библиотека!!!
SRO_LIBDRNET = $(LIBDRNET_DIR)\e1000.o  $(LIBDRNET_DIR)\tsec.o   $(LIBDRNET_DIR)\vsc7385.o

#Объектные файлы для сборки библиотеки libpci.a 
SRO_LIBPCI = $(LIBPCI_DIR)\fsl_pci_init.o  $(LIBPCI_DIR)\pci.o  $(LIBPCI_DIR)\pci_auto.o  $(LIBPCI_DIR)\pci_indirect.o

#Объектные файлы для сборки библиотеки librtc.a
SRO_LIBRTC = $(LIBRTC_DIR)\date.o  $(LIBRTC_DIR)\ds1337.o

#Объектные файлы для сборки библиотеки libserial.a
SRO_LIBSERIAL = $(LIBSERIAL_DIR)\ns16550.o  $(LIBSERIAL_DIR)\serial.o

#Объектные файлы для сборки библиотеки libusb_host.a
SRO_LIBUSB = $(LIBUSB_DIR)\ehci-hcd.o  $(LIBUSB_DIR)\ehci-fsl.o

#Объектные файлы для сборки библиотеки lib8xxx.a
SRO_LIBCPU8xxx = $(LIBCPU8xxx_DIR)\cpu.o  $(LIBCPU8xxx_DIR)\fdt.o  $(LIBCPU8xxx_DIR)\pci_cfg.o

#Объектные файлы для сборки библиотеки libcommon.a
SRO_LIBCOMMON = $(LIBCOMMON_DIR)\cmd_bdinfo.o  $(LIBCOMMON_DIR)\cmd_boot.o $(LIBCOMMON_DIR)\cmd_bootm.o    \
$(LIBCOMMON_DIR)\cmd_console.o  $(LIBCOMMON_DIR)\cmd_date.o  $(LIBCOMMON_DIR)\cmd_elf.o  $(LIBCOMMON_DIR)\cmd_ext2.o   \
$(LIBCOMMON_DIR)\cmd_fat.o  $(LIBCOMMON_DIR)\cmd_fdt.o  $(LIBCOMMON_DIR)\cmd_flash.o  $(LIBCOMMON_DIR)\cmd_i2c.o   \
$(LIBCOMMON_DIR)\cmd_irq.o  $(LIBCOMMON_DIR)\cmd_itest.o  $(LIBCOMMON_DIR)\cmd_load.o  $(LIBCOMMON_DIR)\cmd_mac.o   \
$(LIBCOMMON_DIR)\cmd_mem.o  $(LIBCOMMON_DIR)\cmd_mii.o  $(LIBCOMMON_DIR)\cmd_misc.o  $(LIBCOMMON_DIR)\cmd_mmc.o   \
$(LIBCOMMON_DIR)\cmd_mp.o  $(LIBCOMMON_DIR)\cmd_nand.o  $(LIBCOMMON_DIR)\cmd_net.o  $(LIBCOMMON_DIR)\cmd_nvedit.o   \
$(LIBCOMMON_DIR)\cmd_pci.o  $(LIBCOMMON_DIR)\cmd_pcmcia.o $(LIBCOMMON_DIR)\cmd_setexpr.o  $(LIBCOMMON_DIR)\cmd_sf.o   \
$(LIBCOMMON_DIR)\cmd_source.o  $(LIBCOMMON_DIR)\cmd_usb.o  $(LIBCOMMON_DIR)\cmd_ximg.o  $(LIBCOMMON_DIR)\command.o   \
$(LIBCOMMON_DIR)\console.o $(LIBCOMMON_DIR)\dlmalloc.o  $(LIBCOMMON_DIR)\env_common.o  $(LIBCOMMON_DIR)\env_sf.o   \
$(LIBCOMMON_DIR)\exports.o  $(LIBCOMMON_DIR)\fdt_support.o  $(LIBCOMMON_DIR)\flash.o  $(LIBCOMMON_DIR)\hush.o   \
$(LIBCOMMON_DIR)\hwconfig.o  $(LIBCOMMON_DIR)\image.o  $(LIBCOMMON_DIR)\main.o  $(LIBCOMMON_DIR)\memsize.o   \
$(LIBCOMMON_DIR)\miiphyutil.o  $(LIBCOMMON_DIR)\s_record.o  $(LIBCOMMON_DIR)\serial.o  $(LIBCOMMON_DIR)\stdio.o   \
$(LIBCOMMON_DIR)\usb.o  $(LIBCOMMON_DIR)\usb_storage.o  $(LIBCOMMON_DIR)\xyzModem.o

#Объектные файлы для сборки библиотеки libfdt.a
SRO_LIBFDT = $(LIBFDT_DIR)\fdt.o  $(LIBFDT_DIR)\fdt_ro.o  $(LIBFDT_DIR)\fdt_rw.o  $(LIBFDT_DIR)\fdt_strerror.o    \
$(LIBFDT_DIR)\fdt_sw.o  $(LIBFDT_DIR)\fdt_wip.o

#Объектные файлы для сборки библиотеки libp1_p2_rdb.a
SRO_LIBP1P2RDB = $(LIBP1P2RDB_DIR)\p1_p2_rdb.o  $(LIBP1P2RDB_DIR)\ddr.o   $(LIBP1P2RDB_DIR)\law.o     \
$(LIBP1P2RDB_DIR)\tlb.o 

#Объектные файлы для сборки библиотеки libfreescale.a
SRO_LIBRDBCOMMON = $(LIBRDBCOMMON_DIR)\sys_eeprom.o

#Объектные файлы для сборки библиотеки libgeneric.a
SRO_LIBGENERIC = $(LIBGENERIC_DIR)\crc16.o  $(LIBGENERIC_DIR)\crc32.o  $(LIBGENERIC_DIR)\ctype.o      \
$(LIBGENERIC_DIR)\display_options.o  $(LIBGENERIC_DIR)\div64.o  $(LIBGENERIC_DIR)\gunzip.o            \
$(LIBGENERIC_DIR)\lmb.o $(LIBGENERIC_DIR)\ldiv.o  $(LIBGENERIC_DIR)\md5.o  $(LIBGENERIC_DIR)\sha1.o  $(LIBGENERIC_DIR)\string.o  $(LIBGENERIC_DIR)\strmhz.o  $(LIBGENERIC_DIR)\vsprintf.o  $(LIBGENERIC_DIR)\zlib.o 

#Объектные файлы для сборки библиотеки libfat.a
SRO_LIBFAT = $(LIBFAT_DIR)\fat.o  $(LIBFAT_DIR)\file.o

#Объектные файлы для сборки библиотеки libext2fs.a
SRO_LIBEXT2 = $(LIBEXT2_DIR)\ext2fs.o  $(LIBEXT2_DIR)\dev.o


#Файлы СИ для сборки библиотеки libdisk.a
SRO_LIBDISK = $(LIBDISK_DIR)\part.o  $(LIBDISK_DIR)\part_dos.o

#////////////////////////////INCLUDE Directory////////////////////////////////////////
INCDIR = $(BASE_DIR)\include  
ISYSTEM = $(BASE_DIR)\new_lib

#////////////////////////////////////// DEFINE////////////////////////////////////
DEFS = -D__ASSEMBLY__
#NAND_TEXT_BASE = 0xeff80000 
#NAND_RESET_VEC = 0xeffffffc
TEXT_BASE = 0x11000000 
RESET_VEC = 0x1107fffc



#//////////////////////////////////////DEBUGGING FLAGS////////////////////////////
#DEBUG = -g2 -gdwarf-2 -DCONFIG_CW -ggdb

#//////////////////////////////////////OPTIMIZATION FLAGS/////////////////////////
#OPT = -O1
OPT = 
#/////////////////////////////////////CPPFLAGS///////////////////////////////////
ARFLAGS = crv
CPFLAGS = $(OPT) -g  -Os -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE="$(TEXT_BASE)" -DRESET_VECTOR_ADDRESS=$(RESET_VEC) -fno-builtin -ffreestanding -nostdinc -isystem$(ISYSTEM) -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe
C_CPFLAGS = $(OPT) -g  -Os -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE="$(TEXT_BASE)" -DRESET_VECTOR_ADDRESS=$(RESET_VEC) -fno-builtin -ffreestanding -nostdinc -isystem$(ISYSTEM) -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe -Wall -Wstrict-prototypes -fno-stack-protector

#Флаг для сборки u-boot.lds
CPLDFLAGS = -E -g  -Os  -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE="$(TEXT_BASE)" -DRESET_VECTOR_ADDRESS=$(RESET_VEC) -fno-builtin -ffreestanding -nostdinc -isystem$(ISYSTEM) -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe -includeC:\u-boot-2009.11-cloud\include\u-boot\u-boot.lds.h -ansi -D__ASSEMBLY__ -P  - <C:\u-boot-2009.11-cloud\cpu\mpc85xx\u-boot.lds > $(BASE_DIR)\u-boot.lds
#Флаг для сборки u-boot 
#$UNDEF_SYM
UBFLAGS1  = -Bstatic -T C:\u-boot-2009.11-cloud\u-boot.lds -n -Ttext "0x11000000" C:\u-boot-2009.11-cloud\cpu\mpc85xx\start.o C:\u-boot-2009.11-cloud\cpu\mpc85xx\resetvec.o --start-group $(LIBS) --end-group -LC:\Cross_Tools\freescale-4.4\lib\gcc\powerpc-linux-gnu\4.4.1 -lgcc -Map C:\u-boot-2009.11-cloud\u-boot.map -o C:\u-boot-2009.11-cloud\u-boot 
# Полная команда
#powerpc-linux-gnu-ld -Bstatic -T  C:\u-boot-2009.11-cloud\u-boot.lds -n -Ttext "0x11000000" $UNDEF_SYM C:\u-boot-2009.11-cloud\cpu\mpc85xx\start.o C:\u-boot-2009.11-cloud\cpu\mpc85xx\resetvec.o --start-group C:\u-boot-2009.11-cloud\lib_generic --end-group -LC:\Program Files\Freescale\Cross_Tools\freescale-4.4\lib\gcc\powerpc-linux-gnu\4.4.1 -lgcc -Map C:\u-boot-2009.11-cloud\u-boot.map -o C:\u-boot-2009.11-cloud\u-boot 




# Ещё один путь
#C:\Program Files\Freescale\Cross_Tools\freescale-4.4\lib\gcc\powerpc-linux-gnu\4.4.1


#Цель              Реквизит
$(BASE_DIR)\u-boot:$(LIBS)
		$(LD) $(UBFLAGS1)
$(MPC85xx_DIR)\libmpc85xx.a: $(SRO_MPC85xx)
		$(AR) $(ARFLAGS) $(MPC85xx_DIR)\libmpc85xx.a $(SRO_MPC85xx) 
		$(CC) -I$(INCDIR) $(CPLDFLAGS)
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\start.o $(MPC85xx_DIR)\start.c -c
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\resetvec.o $(MPC85xx_DIR)\resetvec.c -c		
$(SRO_MPC85xx) : $(SRC_MPC85xx)   		
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\release.o $(MPC85xx_DIR)\release.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\ddr-gen3.o $(MPC85xx_DIR)\ddr-gen3.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\fdt.o $(MPC85xx_DIR)\fdt.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\mp.o $(MPC85xx_DIR)\mp.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\pci.o $(MPC85xx_DIR)\pci.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\serdes.o $(MPC85xx_DIR)\serdes.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\p2020_serdes.o $(MPC85xx_DIR)\p2020_serdes.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\cpu.o $(MPC85xx_DIR)\cpu.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\cpu_init.o $(MPC85xx_DIR)\cpu_init.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\cpu_init_early.o $(MPC85xx_DIR)\cpu_init_early.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\interrupts.o $(MPC85xx_DIR)\interrupts.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\speed.o $(MPC85xx_DIR)\speed.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\tlb.o $(MPC85xx_DIR)\tlb.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(MPC85xx_DIR)\traps.o $(MPC85xx_DIR)\traps.c -c
$(LIBPPC_DIR)\libppc.a:$(SRO_LIBPPC)
		$(AR) $(ARFLAGS) $(LIBPPC_DIR)\libppc.a $(SRO_LIBPPC)
$(SRO_LIBPPC) : $(SRC_LIBPPC)
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(LIBPPC_DIR)\ppccache.o $(LIBPPC_DIR)\ppccache.c -c		
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(LIBPPC_DIR)\ppcstring.o $(LIBPPC_DIR)\ppcstring.c -c
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(LIBPPC_DIR)\ticks.o $(LIBPPC_DIR)\ticks.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\bat_rw.o $(LIBPPC_DIR)\bat_rw.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\board.o $(LIBPPC_DIR)\board.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\bootm.o $(LIBPPC_DIR)\bootm.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\cache.o $(LIBPPC_DIR)\cache.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\extable.o $(LIBPPC_DIR)\extable.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\interrupts.o $(LIBPPC_DIR)\interrupts.c -c	
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPPC_DIR)\time.o $(LIBPPC_DIR)\time.c -c
$(LIBNET_DIR)\libnet.a:$(SRO_LIBNET)		
		$(AR) $(ARFLAGS) $(LIBNET_DIR)\libnet.a $(SRO_LIBNET)		  
$(SRO_LIBNET) : $(SRC_LIBNET)
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNET_DIR)\bootp.o $(LIBNET_DIR)\bootp.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNET_DIR)\eth.o $(LIBNET_DIR)\eth.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNET_DIR)\net.o $(LIBNET_DIR)\net.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNET_DIR)\nfs.o $(LIBNET_DIR)\nfs.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNET_DIR)\rarp.o $(LIBNET_DIR)\rarp.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNET_DIR)\tftp.o $(LIBNET_DIR)\tftp.c -c
$(LIBI2C_DIR)\libi2c.a:$(SRO_LIBI2C)	
		$(AR) $(ARFLAGS) $(LIBI2C_DIR)\libi2c.a $(SRO_LIBI2C)		
$(SRO_LIBI2C) : $(SRC_LIBI2C)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBI2C_DIR)\fsl_i2c.o $(LIBI2C_DIR)\fsl_i2c.c -c	
$(LIBMISC_DIR)\libmisc.a:$(SRO_LIBMISC)	
		$(AR) $(ARFLAGS) $(LIBMISC_DIR)\libmisc.a $(SRO_LIBMISC)		
$(SRO_LIBMISC) : $(SRC_LIBMISC)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBMISC_DIR)\fsl_law.o $(LIBMISC_DIR)\fsl_law.c -c		
$(LIBMMC_DIR)\libmmc.a:$(SRO_LIBMMC)	
		$(AR) $(ARFLAGS) $(LIBMMC_DIR)\libmmc.a $(SRO_LIBMMC)		
$(SRO_LIBMMC) : $(SRC_LIBMMC)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBMMC_DIR)\mmc.o $(LIBMMC_DIR)\mmc.c -c		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBMMC_DIR)\fsl_esdhc.o $(LIBMMC_DIR)\fsl_esdhc.c -c
$(LIBMTD_DIR)\libmtd.a:$(SRO_LIBMTD)	
		$(AR) $(ARFLAGS) $(LIBMTD_DIR)\libmtd.a $(SRO_LIBMTD)		
$(SRO_LIBMTD) : $(SRC_LIBMTD)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBMTD_DIR)\cfi_flash.o $(LIBMTD_DIR)\cfi_flash.c -c
$(LIBNAND_DIR)\libnand.a:$(SRO_LIBNAND)	
		$(AR) $(ARFLAGS) $(LIBNAND_DIR)\libnand.a $(SRO_LIBNAND)		
$(SRO_LIBNAND) : $(SRC_LIBNAND)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\nand.o $(LIBNAND_DIR)\nand.c -c	
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\nand_base.o $(LIBNAND_DIR)\nand_base.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\nand_bbt.o $(LIBNAND_DIR)\nand_bbt.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\nand_ecc.o $(LIBNAND_DIR)\nand_ecc.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\nand_ids.o $(LIBNAND_DIR)\nand_ids.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\nand_util.o $(LIBNAND_DIR)\nand_util.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBNAND_DIR)\fsl_elbc_nand.o $(LIBNAND_DIR)\fsl_elbc_nand.c -c
$(LIBSPI_DIR)\libspi_flash.a:$(SRO_LIBSPI)	
		$(AR) $(ARFLAGS) $(LIBSPI_DIR)\libspi_flash.a $(SRO_LIBSPI)		
$(SRO_LIBSPI) : $(SRC_LIBSPI)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBSPI_DIR)\spi_flash.o $(LIBSPI_DIR)\spi_flash.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBSPI_DIR)\spansion.o $(LIBSPI_DIR)\spansion.c -c		
$(LIBFSLSPI_DIR)\libspi.a:$(SRO_LIBFSLSPI)	
		$(AR) $(ARFLAGS) $(LIBFSLSPI_DIR)\libspi.a $(SRO_LIBFSLSPI)		
$(SRO_LIBFSLSPI) : $(SRC_LIBFSLSPI)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFSLSPI_DIR)\fsl_espi.o $(LIBFSLSPI_DIR)\fsl_espi.c -c
$(LIBDRNET_DIR)\libnet.a:$(SRO_LIBDRNET)	
		$(AR) $(ARFLAGS) $(LIBDRNET_DIR)\libnet.a $(SRO_LIBDRNET)		
$(SRO_LIBDRNET) : $(SRC_LIBDRNET)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBDRNET_DIR)\e1000.o $(LIBDRNET_DIR)\e1000.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBDRNET_DIR)\tsec.o $(LIBDRNET_DIR)\tsec.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBDRNET_DIR)\vsc7385.o $(LIBDRNET_DIR)\vsc7385.c -c		
$(LIBPCI_DIR)\libpci.a:$(SRO_LIBPCI)	
		$(AR) $(ARFLAGS) $(LIBPCI_DIR)\libpci.a $(SRO_LIBPCI)		
$(SRO_LIBPCI) : $(SRC_LIBPCI)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPCI_DIR)\fsl_pci_init.o $(LIBPCI_DIR)\fsl_pci_init.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPCI_DIR)\pci.o $(LIBPCI_DIR)\pci.c -c	
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPCI_DIR)\pci_auto.o $(LIBPCI_DIR)\pci_auto.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBPCI_DIR)\pci_indirect.o $(LIBPCI_DIR)\pci_indirect.c -c		
$(LIBRTC_DIR)\librtc.a:$(SRO_LIBRTC)	
		$(AR) $(ARFLAGS) $(LIBRTC_DIR)\librtc.a $(SRO_LIBRTC)		
$(SRO_LIBRTC) : $(SRC_LIBRTC)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBRTC_DIR)\date.o $(LIBRTC_DIR)\date.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBRTC_DIR)\ds1337.o $(LIBRTC_DIR)\ds1337.c -c		
$(LIBSERIAL_DIR)\libserial.a:$(SRO_LIBSERIAL)	
		$(AR) $(ARFLAGS) $(LIBSERIAL_DIR)\libserial.a $(SRO_LIBSERIAL)		
$(SRO_LIBSERIAL) : $(SRC_LIBSERIAL)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBSERIAL_DIR)\ns16550.o $(LIBSERIAL_DIR)\ns16550.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBSERIAL_DIR)\serial.o $(LIBSERIAL_DIR)\serial.c -c			
$(LIBUSB_DIR)\libusb_host.a:$(SRO_LIBUSB)	
		$(AR) $(ARFLAGS) $(LIBUSB_DIR)\libusb_host.a $(SRO_LIBUSB)		
$(SRO_LIBUSB) : $(SRC_LIBUSB)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBUSB_DIR)\ehci-hcd.o $(LIBUSB_DIR)\ehci-hcd.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBUSB_DIR)\ehci-fsl.o $(LIBUSB_DIR)\ehci-fsl.c -c		
$(LIBCPU8xxx_DIR)\lib8xxx.a:$(SRO_LIBCPU8xxx)	
		$(AR) $(ARFLAGS) $(LIBCPU8xxx_DIR)\lib8xxx.a $(SRO_LIBCPU8xxx)		
$(SRO_LIBCPU8xxx) : $(SRC_LIBCPU8xxx)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCPU8xxx_DIR)\cpu.o $(LIBCPU8xxx_DIR)\cpu.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCPU8xxx_DIR)\fdt.o $(LIBCPU8xxx_DIR)\fdt.c -c	
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCPU8xxx_DIR)\pci_cfg.o $(LIBCPU8xxx_DIR)\pci_cfg.c -c		
$(LIBCOMMON_DIR)\libcommon.a:$(SRO_LIBCOMMON)	
		$(AR) $(ARFLAGS) $(LIBCOMMON_DIR)\libcommon.a $(SRO_LIBCOMMON)		
$(SRO_LIBCOMMON) : $(SRC_LIBCOMMON)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_bdinfo.o $(LIBCOMMON_DIR)\cmd_bdinfo.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_boot.o $(LIBCOMMON_DIR)\cmd_boot.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_bootm.o $(LIBCOMMON_DIR)\cmd_bootm.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_console.o $(LIBCOMMON_DIR)\cmd_console.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_date.o $(LIBCOMMON_DIR)\cmd_date.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_elf.o $(LIBCOMMON_DIR)\cmd_elf.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_ext2.o $(LIBCOMMON_DIR)\cmd_ext2.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_fat.o $(LIBCOMMON_DIR)\cmd_fat.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_fdt.o $(LIBCOMMON_DIR)\cmd_fdt.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_flash.o $(LIBCOMMON_DIR)\cmd_flash.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_i2c.o $(LIBCOMMON_DIR)\cmd_i2c.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_irq.o $(LIBCOMMON_DIR)\cmd_irq.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_itest.o $(LIBCOMMON_DIR)\cmd_itest.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_load.o $(LIBCOMMON_DIR)\cmd_load.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_mac.o $(LIBCOMMON_DIR)\cmd_mac.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_mem.o $(LIBCOMMON_DIR)\cmd_mem.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_mii.o $(LIBCOMMON_DIR)\cmd_mii.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_misc.o $(LIBCOMMON_DIR)\cmd_misc.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_mmc.o $(LIBCOMMON_DIR)\cmd_mmc.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_mp.o $(LIBCOMMON_DIR)\cmd_mp.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_nand.o $(LIBCOMMON_DIR)\cmd_nand.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_net.o $(LIBCOMMON_DIR)\cmd_net.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_nvedit.o $(LIBCOMMON_DIR)\cmd_nvedit.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_pci.o $(LIBCOMMON_DIR)\cmd_pci.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_pcmcia.o $(LIBCOMMON_DIR)\cmd_pcmcia.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_setexpr.o $(LIBCOMMON_DIR)\cmd_setexpr.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_sf.o $(LIBCOMMON_DIR)\cmd_sf.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_source.o $(LIBCOMMON_DIR)\cmd_source.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_usb.o $(LIBCOMMON_DIR)\cmd_usb.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\cmd_ximg.o $(LIBCOMMON_DIR)\cmd_ximg.c -c		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\command.o $(LIBCOMMON_DIR)\command.c -c	
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\console.o $(LIBCOMMON_DIR)\console.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\dlmalloc.o $(LIBCOMMON_DIR)\dlmalloc.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\env_common.o $(LIBCOMMON_DIR)\env_common.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\env_sf.o $(LIBCOMMON_DIR)\env_sf.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\exports.o $(LIBCOMMON_DIR)\exports.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\fdt_support.o $(LIBCOMMON_DIR)\fdt_support.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\flash.o $(LIBCOMMON_DIR)\flash.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\hush.o $(LIBCOMMON_DIR)\hush.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\hwconfig.o $(LIBCOMMON_DIR)\hwconfig.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\image.o $(LIBCOMMON_DIR)\image.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\main.o $(LIBCOMMON_DIR)\main.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\memsize.o $(LIBCOMMON_DIR)\memsize.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\miiphyutil.o $(LIBCOMMON_DIR)\miiphyutil.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\s_record.o $(LIBCOMMON_DIR)\s_record.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\serial.o $(LIBCOMMON_DIR)\serial.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\stdio.o $(LIBCOMMON_DIR)\stdio.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\usb.o $(LIBCOMMON_DIR)\usb.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\usb_storage.o $(LIBCOMMON_DIR)\usb_storage.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBCOMMON_DIR)\xyzModem.o $(LIBCOMMON_DIR)\hush.c -c	
$(LIBFDT_DIR)\libfdt.a:$(SRO_LIBFDT)	
		$(AR) $(ARFLAGS) $(LIBFDT_DIR)\libfdt.a $(SRO_LIBFDT)		
$(SRO_LIBFDT) : $(SRC_LIBFDT)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFDT_DIR)\fdt.o $(LIBFDT_DIR)\fdt.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFDT_DIR)\fdt_ro.o $(LIBFDT_DIR)\fdt_ro.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFDT_DIR)\fdt_rw.o $(LIBFDT_DIR)\fdt_rw.c -c 
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFDT_DIR)\fdt_strerror.o $(LIBFDT_DIR)\fdt_strerror.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFDT_DIR)\fdt_sw.o $(LIBFDT_DIR)\fdt_sw.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFDT_DIR)\fdt_wip.o $(LIBFDT_DIR)\fdt_wip.c -c		
$(LIBP1P2RDB_DIR)\libp1_p2_rdb.a:$(SRO_LIBP1P2RDB)	
		$(AR) $(ARFLAGS) $(LIBP1P2RDB_DIR)\libp1_p2_rdb.a $(SRO_LIBP1P2RDB)		
$(SRO_LIBP1P2RDB) : $(SRC_LIBP1P2RDB)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBP1P2RDB_DIR)\p1_p2_rdb.o $(LIBP1P2RDB_DIR)\p1_p2_rdb.c -c	
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBP1P2RDB_DIR)\ddr.o $(LIBP1P2RDB_DIR)\ddr.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBP1P2RDB_DIR)\law.o $(LIBP1P2RDB_DIR)\law.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBP1P2RDB_DIR)\tlb.o $(LIBP1P2RDB_DIR)\tlb.c -c		
$(LIBRDBCOMMON_DIR)\libfreescale.a:$(SRO_LIBRDBCOMMON)	
		$(AR) $(ARFLAGS) $(LIBRDBCOMMON_DIR)\libfreescale.a $(SRO_LIBRDBCOMMON)		
$(SRO_LIBRDBCOMMON) : $(SRC_LIBRDBCOMMON)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBRDBCOMMON_DIR)\sys_eeprom.o $(LIBRDBCOMMON_DIR)\sys_eeprom.c -c		
$(LIBGENERIC_DIR)\libgeneric.a:$(SRO_LIBGENERIC)	
		$(AR) $(ARFLAGS) $(LIBGENERIC_DIR)\libgeneric.a $(SRO_LIBGENERIC)		
$(SRO_LIBGENERIC) : $(SRC_LIBGENERIC)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\crc16.o $(LIBGENERIC_DIR)\crc16.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\crc32.o $(LIBGENERIC_DIR)\crc32.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\ctype.o $(LIBGENERIC_DIR)\ctype.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\display_options.o $(LIBGENERIC_DIR)\display_options.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\div64.o $(LIBGENERIC_DIR)\div64.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\gunzip.o $(LIBGENERIC_DIR)\gunzip.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\lmb.o $(LIBGENERIC_DIR)\lmb.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\ldiv.o $(LIBGENERIC_DIR)\ldiv.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\md5.o $(LIBGENERIC_DIR)\md5.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\sha1.o $(LIBGENERIC_DIR)\sha1.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\string.o $(LIBGENERIC_DIR)\string.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\strmhz.o $(LIBGENERIC_DIR)\strmhz.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\vsprintf.o $(LIBGENERIC_DIR)\vsprintf.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBGENERIC_DIR)\zlib.o $(LIBGENERIC_DIR)\zlib.c -c		
$(LIBFAT_DIR)\libfat.a:$(SRO_LIBFAT)	
		$(AR) $(ARFLAGS) $(LIBFAT_DIR)\libfat.a $(SRO_LIBFAT)		
$(SRO_LIBFAT) : $(SRC_LIBFAT)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFAT_DIR)\fat.o $(LIBFAT_DIR)\fat.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBFAT_DIR)\file.o $(LIBFAT_DIR)\file.c -c		
$(LIBEXT2_DIR)\libext2fs.a:$(SRO_LIBEXT2)	
		$(AR) $(ARFLAGS) $(LIBEXT2_DIR)\libext2fs.a $(SRO_LIBEXT2)		
$(SRO_LIBEXT2) : $(SRC_LIBEXT2)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBEXT2_DIR)\ext2fs.o $(LIBEXT2_DIR)\ext2fs.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBEXT2_DIR)\dev.o $(LIBEXT2_DIR)\dev.c -c		
$(LIBDISK_DIR)\libdisk.a:$(SRO_LIBDISK)	
		$(AR) $(ARFLAGS) $(LIBDISK_DIR)\libdisk.a $(SRO_LIBDISK)		
$(SRO_LIBDISK) : $(SRC_LIBDISK)		
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBDISK_DIR)\part.o $(LIBDISK_DIR)\part.c -c
		$(CC) -I$(INCDIR) $(C_CPFLAGS) -o $(LIBDISK_DIR)\part_dos.o $(LIBDISK_DIR)\part_dos.c -c


	
		
		
		
		
		
	
		
		
		