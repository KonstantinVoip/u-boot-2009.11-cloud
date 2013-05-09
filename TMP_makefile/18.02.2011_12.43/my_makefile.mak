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
BASE_DIR = C:\u-boot-2009.11-cloud
MPC85xx_DIR = C:\u-boot-2009.11-cloud\cpu\mpc85xx


#/////////////////////////////List C source files here/////////////////////////////////
SRC = $(MPC85xx_DIR)\start.c $(MPC85xx_DIR)\resetvec.c $(MPC85xx_DIR)\release.c 



#////////////////////////////INCLUDE Directory////////////////////////////////////////
INCDIR = $(BASE_DIR)\include  


#////////////////////////////////////// DEFINE////////////////////////////////////
DEFS = -D__ASSEMBLY__

#//////////////////////////////////////OPTIMIZATION FLAGS/////////////////////////
OPT = 

#/////////////////////////////////////CPPFLAGS///////////////////////////////////
ARFLAGS = crv
CPFLAGS = $(OPT) -g  -Os -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE=0xeff80000 -DRESET_VECTOR_ADDRESS=0xeffffffc -fno-builtin -ffreestanding -nostdinc -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe


#Цель              Реквизит


libmpc85xx.a:$(MPC85xx_DIR)\start.o $(MPC85xx_DIR)\resetvec.o $(MPC85xx_DIR)\release.o
		$(AR) $(ARFLAGS) $(MPC85xx_DIR)\libmpc85xx.a  $(MPC85xx_DIR)\start.o $(MPC85xx_DIR)\resetvec.o $(MPC85xx_DIR)\release.o 
$(MPC85xx_DIR)\start.o $(MPC85xx_DIR)\resetvec.o $(MPC85xx_DIR)\release.o : $(SRC)   		
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\start.o $(MPC85xx_DIR)\start.c -c
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\resetvec.o $(MPC85xx_DIR)\resetvec.c -c
		$(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\release.o $(MPC85xx_DIR)\release.c -c
		
		