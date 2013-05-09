##############################################################################################
# Start of default section
#
TRGT = powerpc-linux-gnu-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O srec 
AR   = $(TRGT)ar
OBJ  = $(TRGT)objdump
  
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



#Общая директория где лежит U-Boot
BASE_DIR = C:\u-boot-2009.11-cloud

#//////////////////////////////ALL LDS Librari///////////////////////////////
LIBS = $(BASE_DIR)\u-boot.lds 
 
#////////////////////////////INCLUDE Directory////////////////////////////////////////
INCDIR = $(BASE_DIR)\include  
ISYSTEM = $(BASE_DIR)\new_lib

UBOOTLDS = $(INCDIR)\u-boot
#////////////////////////////////////// DEFINE////////////////////////////////////
DEFS = -D__ASSEMBLY__
TEXT_BASE = 0xeff80000 
RESET_VEC = 0xeffffffc

#//////////////////////////////////////DEBUGGING FLAGS////////////////////////////
#DEBUG = -g2 -gdwarf-2 -DCONFIG_CW -ggdb
#//////////////////////////////////////OPTIMIZATION FLAGS/////////////////////////
#OPT = -O1
OPT = 
#/////////////////////////////////////CPPFLAGS///////////////////////////////////
CPLDFLAGS = -E -g  -Os  -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE) -DRESET_VECTOR_ADDRESS=$(RESET_VEC) -fno-builtin -ffreestanding -nostdinc -isystem$(ISYSTEM) -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe -includeC:\u-boot-2009.11-cloud\include\u-boot\u-boot.lds.h -ansi -D__ASSEMBLY__ -P  - <C:\u-boot-2009.11-cloud\cpu\mpc85xx\u-boot.lds > $(BASE_DIR)\u-boot.lds                                             
$(BASE_DIR)\u-boot.lds: C:\u-boot-2009.11-cloud\cpu\mpc85xx\u-boot.lds  C:\u-boot-2009.11-cloud\include\u-boot\u-boot.lds.h
		$(CC) -I$(INCDIR) $(CPLDFLAGS) 
		   		
	
		


	
		
		
		
		
		
	
		
		
		