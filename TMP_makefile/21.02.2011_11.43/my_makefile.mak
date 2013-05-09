##############################################################################################
# Start of default section
#
TRGT = powerpc-linux-gnu-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O srec 
AR   = $(TRGT)ar


#///////////////////////////////////////// Directory///////////////////////////////////
#Общая директория где лежит U-Boot
BASE_DIR = C:\u-boot-2009.11-cloud

#Расположение cpu\mpc85xx
MPC85xx_DIR = C:\u-boot-2009.11-cloud\cpu\mpc85xx

#Расположение \lib_ppc
LIBPPC_DIR =  C:\u-boot-2009.11-cloud\lib_ppc

#/////////////////////////////List C source files here/////////////////////////////////

#Файлы с для сборки библиотеки  libmpc85xx.a     
SRC_MPC85xx = $(MPC85xx_DIR)\release.c  \
$(MPC85xx_DIR)\ddr-gen3.c  $(MPC85xx_DIR)\fdt.c  $(MPC85xx_DIR)\mp.c $(MPC85xx_DIR)\pci.c  \
$(MPC85xx_DIR)\serdes.c  $(MPC85xx_DIR)\p2020_serdes.c  $(MPC85xx_DIR)\cpu.c   \
$(MPC85xx_DIR)\cpu_init.c  $(MPC85xx_DIR)\cpu_init_early.c  $(MPC85xx_DIR)\interrupts.c   \
$(MPC85xx_DIR)\speed.c  $(MPC85xx_DIR)\tlb.c  $(MPC85xx_DIR)\traps.c 

#Файлы с для сборки библиотеки  libppc.a 
SRC_LIBPPC = $(LIBPPC_DIR)\ppccache.c  $(LIBPPC_DIR)\ppcstring.c  $(LIBPPC_DIR)\ticks.c   \
$(LIBPPC_DIR)\bat_rw.c  $(LIBPPC_DIR)\board.c  $(LIBPPC_DIR)\bootm.c     \
$(LIBPPC_DIR)\cache.c  $(LIBPPC_DIR)\extable.c $(LIBPPC_DIR)\interrupts.c    \
$(LIBPPC_DIR)\time.c




#/////////////////////////////List O Objects files here/////////////////////////////////
#Объектные файлы для сборки библиотеки libmpc85xx.a
SRO_MPC85xx = $(MPC85xx_DIR)\release.o   \
$(MPC85xx_DIR)\ddr-gen3.o  $(MPC85xx_DIR)\fdt.o $(MPC85xx_DIR)\mp.o $(MPC85xx_DIR)\pci.o  \
$(MPC85xx_DIR)\serdes.o  $(MPC85xx_DIR)\p2020_serdes.o  $(MPC85xx_DIR)\cpu.o   \
$(MPC85xx_DIR)\cpu_init.o  $(MPC85xx_DIR)\cpu_init_early.o  $(MPC85xx_DIR)\interrupts.o   \
$(MPC85xx_DIR)\speed.o  $(MPC85xx_DIR)\tlb.o  $(MPC85xx_DIR)\traps.o


#Объектные файлы для сборки библиотеки libppc.a
SRC_LIBPPC = $(LIBPPC_DIR)\ppccache.o  $(LIBPPC_DIR)\ppcstring.o  $(LIBPPC_DIR)\ticks.o   \
$(LIBPPC_DIR)\bat_rw.o  $(LIBPPC_DIR)\board.o  $(LIBPPC_DIR)\bootm.o     \
$(LIBPPC_DIR)\cache.o  $(LIBPPC_DIR)\extable.o $(LIBPPC_DIR)\interrupts.o    \
$(LIBPPC_DIR)\time.o



#////////////////////////////INCLUDE Directory////////////////////////////////////////
INCDIR = $(BASE_DIR)\include  
ISYSTEM = $(BASE_DIR)\new_lib

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
ARFLAGS = crv
CPFLAGS = $(OPT) -g  -Os -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE) -DRESET_VECTOR_ADDRESS=$(RESET_VEC) -fno-builtin -ffreestanding -nostdinc -isystem$(ISYSTEM) -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe
C_CPFLAGS = $(OPT) -g  -Os -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE) -DRESET_VECTOR_ADDRESS=$(RESET_VEC) -fno-builtin -ffreestanding -nostdinc -isystem$(ISYSTEM) -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe -Wall -Wstrict-prototypes -fno-stack-protector

#Цель              Реквизит
libmpc85xx.a: $(SRO_MPC85xx)
		$(AR) $(ARFLAGS) $(MPC85xx_DIR)\libmpc85xx.a $(SRO_MPC85xx)
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


		
			
		
		
		
		