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
		   		
	
		


	
		
		
		
		
		
	
		
		
		