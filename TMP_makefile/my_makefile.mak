##############################################################################################
# Start of default section
#
TRGT = powerpc-linux-gnu-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O srec 


#///////////////////////////////////////// Directory///////////////////////////////////
BASE_DIR = C:\u-boot-2009.11-cloud
MPC85xx_DIR = C:\u-boot-2009.11-cloud\cpu\mpc85xx


#/////////////////////////////List C source files here/////////////////////////////////
SRC = $(MPC85xx_DIR)\main.c  $(MPC85xx_DIR)\resetvec.c  



#////////////////////////////INCLUDE Directory////////////////////////////////////////
INCDIR = $(BASE_DIR)\include 


#////////////////////////////////////// DEFINE////////////////////////////////////
DEFS = -D__ASSEMBLY__

#//////////////////////////////////////OPTIMIZATION FLAGS/////////////////////////
OPT = 

#/////////////////////////////////////CPPFLAGS///////////////////////////////////
CPFLAGS = $(OPT) -g  -Os -mrelocatable -fPIC -ffixed-r14 -meabi -D__KERNEL__ -DTEXT_BASE=0xeff80000 -DRESET_VECTOR_ADDRESS=0xeffffffc -fno-builtin -ffreestanding -nostdinc -pipe  -DCONFIG_PPC -D__powerpc__ -ffixed-r2 -Wa,-me500 -msoft-float -mno-string -mspe=yes -mno-spe


#Цель  Реквизит
main.o resetvec.o : $(SRC)  
		 $(CC) -I$(INCDIR)  -o $(MPC85xx_DIR)\main.o $(MPC85xx_DIR)\main.c -c
	     $(AS) -I$(INCDIR) $(DEFS) $(CPFLAGS) -o $(MPC85xx_DIR)\resetvec.o $(MPC85xx_DIR)\resetvec.c -c



		
		