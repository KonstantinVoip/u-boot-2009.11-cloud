/*
 * Driver for NAND support, Rick Bronson
 * borrowed heavily from:
 * (c) 1999 Machine Vision Holdings, Inc.
 * (c) 1999, 2000 David Woodhouse <dwmw2@infradead.org>
 *
 * Added 16-bit nand support
 * (C) 2004 Texas Instruments
 */

#include <common.h>


/*
 *
 * New NAND support
 *
 */
#include <common.h>
#include <linux/mtd/mtd.h>

////////////////////////CYCLONE3 _Testing Include Add///////////////////////////////

#include <cyclone3/elbcTransnmitBuf.h>
#include <asm-ppc/io.h>
#include <tsec.h>


////////////////////////////////////////////////////////////////////////////////////

#if defined(CONFIG_CMD_NAND)

#include <command.h>
#include <watchdog.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>
#include <nand.h>

#if defined(CONFIG_CMD_MTDPARTS)

/* parition handling routines */
int mtdparts_init(void);
int id_parse(const char *id, const char **ret_id, u8 *dev_type, u8 *dev_num);
int find_dev_and_part(const char *id, struct mtd_device **dev,
		      u8 *part_num, struct part_info **part);
#endif

////////////////////////////Testig_Cyclone3_Functions/////////////////

//Testing_Menu_functions
void tes1_finctiond(u16 size,u16 parameter_test);
void test2_function(u16 size,u16 parameter_test);

//Packet_Functions
void assmble_packet(u16 length);

void write_toPLIS_success();

////////IO_READ_WRITE_FUNCTION's
static inline void * k_word_flash_map (u16 offset)
{
  unsigned int byte_offset = offset * 2;
  //puts("++++++++cfi_flash.c/flash_map+++++++\n\r");
	
  //return (void *)(0xff000000 + byte_offset);
    return (void *)(0xef000000 + byte_offset);     
}



static inline u16 k__flash_read16(void *addr)
{
	return __raw_readw(addr);
}


static inline void k__flash_write16(u16 value, void *addr)
{
	__raw_writew(value, addr);
}







void write_toPLIS_success()
{
u16 l_adress_write_success=30;
u16 data_write_success=0x0003;	


     *(volatile unsigned short *)PCI_FIX_ADDR((void *)(0xef00003c)) = 0x0003; 
   //*(volatile unsigned short *)PCI_FIX_ADDR((void *)(0xff00003c)) = 0x0003;
   //printf("Write_sucess Num=%d Waddress=0x%x -> Wdata= 0x%x|\n\r",l_adress_write_success,flash_map(l_adress_write_success),/*data_to_write*/ /*test_mas[m]*/data_write_success);	

}











////
void write_to_PLIS(u16 *inpacket_data,u16 data_legth)
{
//Conunters
u16 dannie1400=0;
u16 l_adress_read1400=1400;

u16 count_dannie30=0;


u16 dannie30=1;
u16 dannie120=0;

u16 l_adress_read120=120;
u16 l_adress_read30=30;

u16 m=0;
static u16 iteration;



/*

	for(i=0;i<508;i++)
	{
	pkt[i]=inpacket_data[i];
		
	}
*/	

printf("+++++++++++++++++Write_Iter->%d++++++++++++++++++++++++++++++++++++++\n\r",iteration++);




 while(dannie30)
    {  
       //data_in_memory800=*(volatile unsigned short *)(0xff000640);
        dannie30=k__flash_read16(k_word_flash_map(l_adress_read30));
        printf("Ctenie__30 =0x%x\n\r",dannie30);

     //ludelay(10000);
    }

	
	
	
	
//dannie120=__flash_read16(flash_map(l_adress_read120));
//printf("Read_Iter_120_After30_Success , Num=%d Raddress =0x%x ->Rdata=0x%x|\n\r",l_adress_read120,flash_map(l_adress_read120),dannie120);


// printf("length=%d\n\r",data_legth); 

 
 //#if 0
 
 	for(m=0;m<data_legth+1;m++)
 	{
   		// __raw_writew(inpacket_data[m], (void *)(0xff0000c8));
  	    //	*(volatile unsigned short *)(0xff000190) = test_full_packet_mas[m];//inpacket_data[m];
		
		  *(volatile unsigned short *)(0xef000190) =inpacket_data[m];
		//*(volatile unsigned short *)(0xff000190) =inpacket_data[m];
		//*(volatile unsigned short *)(0xff0000c8)/*PCI_FIX_ADDR((void *)*/ = inpacket_data[m];
  		// __flash_read16((void *)(0xff0000c8));
 		// printf("0x%x|",inpacket_data[m]);  
	
	//#if 0
 		if(m<4)
  		{
     	printf("Write_Iter =%d, Waddress=0x%x -> Wdata= 0x%04x|\n\r",m,0xef000190,inpacket_data[m] /*test_mas[m]*/ /*data_to_write*/);
     	}
    //#endif
        if(m>data_legth-4)
  		{
     	printf("Write_Iter =%d, Waddress=0x%x -> Wdata= 0x%04x|\n\r",m,0xef000190,inpacket_data[m] /*test_mas[m]*/ /*data_to_write*/);
     	}    

 	}

write_toPLIS_success();
//dannie120=__flash_read16(flash_map(l_adress_read120));
//printf("Read_Iter_120_After30_Success , Num=%d Raddress =0x%x ->Rdata=0x%x|\n\r",l_adress_read120,flash_map(l_adress_read120),dannie120);

dannie1400=k__flash_read16(k_word_flash_map(l_adress_read1400));
printf("Read_Iter_dannie1400_After30_Success  ->Rdata=0x%x|\n\r",dannie1400);



}








void assmble_packet(u16 length)
{
  u16 plis_raw_data_mas[length]; //massive_count = 0 to 759  element =760 word = 1520 bait
  u16 dannie0=0;
  u16 dannie2=0;
  u16 dannie4=0;
  u16 dannie6=0;
  u16 dannie8=0;
  u16 dannie10=0;
  u16 dannie12=0; 
  u16 dannie30=1;
  u16 dannie400=0;
  u16 dannie100=0;
  u16 dannie120=0;
  u16 dannie800=0;
  u16 dannie1000=0;
  u16 dannie1200=0;
  
  u16 l_adress_read0=0;
  u16 l_adress_read2=2;
  u16 l_adress_read4=4;
  u16 l_adress_read6=6;
  u16 l_adress_read8=8;
  u16 l_adress_read10=10;
  u16 l_adress_read12=12;
  u16 l_adress_read30=30;
  u16 l_adress_read100=100;
  u16 l_adress_read120=120;
  u16 l_adress_read400=400;
  u16 l_adress_read800=800;
  u16 l_adress_read1000=1000;
  u16 l_adress_read1200=1200;
  
  u16 count_dannie1000=0;
  static u16 iteration=0; 
  u16 t=0;
   //Clear Input Buffer packet buffer
  
  //memset(&plis_raw_data_mas, 0xffff, sizeof(plis_raw_data_mas));
  //memset(&ip_packet, 0, sizeof(ip_packet));
  
  
  printf("--------------------READ_Iter=%d|size_byte=%d------------------------------\n\r",iteration++,length*2);

 
  
 
    while(!dannie1000)
    {   	
		//data_in_memory800=*(volatile unsigned short *)(0xff000640);
		dannie1000=k__flash_read16(k_word_flash_map(l_adress_read1000));
		if(dannie1000==0xabc0)
		{
		dannie1000=0;
		}
     
	 //dannie800=__flash_read16(flash_map(l_adress_read800));       
      printf("WHILE_Ctenie__1000 =0x%x\n\r",dannie1000);
	  count_dannie1000++;
	  if(count_dannie1000==100)break;
	//printf("WHILE_Ctenie__800 =0x%x\n\r",dannie800);  
    }


  
  
  dannie800=k__flash_read16(k_word_flash_map(l_adress_read800)); 
  printf("Ctenie__800 =0x%x\n\r",dannie800);
 
  dannie1200=k__flash_read16(k_word_flash_map(l_adress_read1200));
  printf("Razmer_in1200->>0x%x ,Razmer_in1200+1->>0x%x-\n\r",dannie1200,dannie1200+1);
  

 

  do
  { 
       // plis_raw_data_mas[t]=__flash_read16((void *)(0xff0000cc));
       //plis_raw_data_mas[t]=*(volatile unsigned short *)(0xff0000cc);//__flash_read16((void *)(0xff0000cc));
       //NetTxPacket[t]=*(volatile unsigned short *)PCI_FIX_ADDR((void *)(0xff0000cc));
       
			plis_raw_data_mas[t]=*(volatile unsigned short *)(0xef000320);
        
			//#if 0
			if(t<4)
			{
			printf("Read_Iter =%d,Raddress=0x%x -> Rdata= 0x%04x|\n\r",t,0xef000320,plis_raw_data_mas[t] /*test_mas[m]*/ /*data_to_write*/);
			}
              
			//#if 0 
			if(t>length-4)
			{
			printf("Read_Iter =%d, Raddress=0x%x -> Rdata= 0x%x|\n\r",t,0xef000320,plis_raw_data_mas[t] /*test_mas[m]*/ /*data_to_write*/);
			}
			//#endif  
           
       t++;
       
        
  } while(t<length+1);

  
}




void tes1_finctiond( u16 size,u16 parameter_test)
{
//u16 input_packet_size=256;//256 elemnt for word =16 bit  =512bait;
u16 input_packet_size=size;
char ch;
u16 i;
printf("Start Test1 _Local_bus_Cylone3\n\r");

 do
 {  

	ch = getc();
	if(parameter_test==0)   
	{
	write_to_PLIS(test_full_packet_mas,input_packet_size);
	assmble_packet(input_packet_size);
	}
   
	else
	{
    
	   for(i=0;i<=150;i++)
       { 
	 
		write_to_PLIS(test_full_packet_mas,input_packet_size);
		assmble_packet(input_packet_size);
		udelay(100); 
	   }
      
	 printf("TEST COMPLETE\n\r");
	}
   
 } while (ch!='s');    

}


void test2_finctiond( u16 size,u16 parameter_test)
{
u16 input_packet_size=size;
char ch;
u16 i;
printf("Start Test1 _Local_bus_Cylone3\n\r");
 do
 {  
	ch = getc();
	if(parameter_test==0)   
	{
	write_to_PLIS(test_ethernet_preambula_inverse,input_packet_size);
	assmble_packet(input_packet_size);
	}
	else
	{
	   for(i=0;i<=150;i++)
       { 
		write_to_PLIS(test_full_packet_mas,input_packet_size);
		assmble_packet(input_packet_size);
		udelay(100); 
	   }
	 printf("TEST COMPLETE\n\r");
	}
   
 } while (ch!='s');

}
















static inline int str2long(char *p, ulong *num)
{
	char *endptr;

	*num = simple_strtoul(p, &endptr, 16);
	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

static int
arg_off_size(int argc, char *argv[], nand_info_t *nand, ulong *off, size_t *size)
{
	int idx = nand_curr_device;
#if defined(CONFIG_CMD_MTDPARTS)
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;

	if (argc >= 1 && !(str2long(argv[0], off))) {
		if ((mtdparts_init() == 0) &&
		    (find_dev_and_part(argv[0], &dev, &pnum, &part) == 0)) {
			if (dev->id->type != MTD_DEV_TYPE_NAND) {
				puts("not a NAND device\n");
				return -1;
			}
			*off = part->offset;
			if (argc >= 2) {
				if (!(str2long(argv[1], (ulong *)size))) {
					printf("'%s' is not a number\n", argv[1]);
					return -1;
				}
				if (*size > part->size)
					*size = part->size;
			} else {
				*size = part->size;
			}
			idx = dev->id->num;
			*nand = nand_info[idx];
			goto out;
		}
	}
#endif

	if (argc >= 1) {
		if (!(str2long(argv[0], off))) {
			printf("'%s' is not a number\n", argv[0]);
			return -1;
		}
	} else {
		*off = 0;
	}

	if (argc >= 2) {
		if (!(str2long(argv[1], (ulong *)size))) {
			printf("'%s' is not a number\n", argv[1]);
			return -1;
		}
	} else {
		*size = nand->size - *off;
	}

#if defined(CONFIG_CMD_MTDPARTS)
out:
#endif
	printf("device %d ", idx);
	if (*size == nand->size)
		puts("whole chip\n");
	else
		printf("offset 0x%lx, size 0x%zx\n", *off, *size);
	return 0;
}



static void nand_print_info(int idx)
{
	nand_info_t *nand = &nand_info[idx];
	struct nand_chip *chip = nand->priv;
	printf("Device %d: ", idx);
	if (chip->numchips > 1)
		printf("%dx ", chip->numchips);
	printf("%s, sector size %u KiB\n",
	       nand->name, nand->erasesize >> 10);
}

int do_nand(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int i, dev, ret = 0;
	ulong addr, off;
	size_t size;
	char *cmd, *s;
	nand_info_t *nand;
    static u16 enable_loop=0;
/////////////Start Kosta and Andrey Testing Functions
	/* at least two arguments please */
	if (argc < 2)
		goto usage;
	
	cmd = argv[1];
		
		if (strcmp(cmd, "test1_loop") == 0) 
		{
           if(enable_loop==1)
		   {
		   enable_loop=0;
		   printf("Loop disable\n\r");
		   }
		   else
		   {
		   printf("Loop enable\n\r");
		   enable_loop=1;
		   }
		  putc('\n');
		  return 0;   
		}
	
	
	
		if (strcmp(cmd, "test1_64") == 0) 
		{
		tes1_finctiond(32,enable_loop);
		putc('\n');
		return 0;   
		}
	
	    if (strcmp(cmd, "test1_512") == 0) 
		{
		tes1_finctiond(256,enable_loop);
		putc('\n');
		return 0;   
		}
		
		if (strcmp(cmd, "test1_1514") == 0) 
		{
		tes1_finctiond(757,enable_loop);
		putc('\n');
		return 0;   
		}
		if (strcmp(cmd, "test1_1518") == 0) 
		{
		tes1_finctiond(759,enable_loop);
		putc('\n');
		return 0;   
		}
	
	
	    if (strcmp(cmd, "test_pr") == 0) 
		{
		test2_finctiond(36,enable_loop);
		putc('\n');
		return 0;   
		}
	
	
	    if (strcmp(cmd, "test_pr_inverse") == 0) 
		{
		tes1_finctiond(36,enable_loop);
		putc('\n');
		return 0;   
		}
	
	
	    
	
	
	
	
	
	    if (strcmp(cmd, "info") == 0) 
		{
		flash_init ();
		putc('\n');
		return 0;
	    
		}

usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(cyc3, CONFIG_SYS_MAXARGS, 1, do_nand,
	"PLIS Cyclone 3 sub-system",
	"info  - show configuration\n"
	"test1_loop -enable/disable loop\n"
	"test1_64   -static =32*2= 64bait  ->lBC\n"
	"test1_512  -static =256*2=512bait ->lBC\n"
    "test1_1514 -static =757*2=1514bait->lBC\n"
	"test1_1518 -static =759*2=1518bait->lBC\n"
	"test_pr    -test preambula  ->lBC\n"
	"test_pr_inverse    -test preambula inverse  ->lBC\n"
	
	 
	
	#if 0
	"nand read - addr off|partition size\n"
	"nand write - addr off|partition size\n"
	"    read/write 'size' bytes starting at offset 'off'\n"
	"    to/from memory address 'addr', skipping bad blocks.\n"
	"nand erase [clean] [off size] - erase 'size' bytes from\n"
	"    offset 'off' (entire device if not specified)\n"
	"nand bad - show bad blocks\n"
	"nand dump[.oob] off - dump page\n"
	"nand scrub - really clean NAND erasing bad blocks (UNSAFE)\n"
	"nand markbad off [...] - mark bad block(s) at offset (UNSAFE)\n"
	"nand biterr off - make a bit error at offset (UNSAFE)"
#endif
#ifdef CONFIG_CMD_NAND_LOCK_UNLOCK
	"\n"
	"nand lock [tight] [status]\n"
	"    bring nand to lock state or display locked pages\n"
	"nand unlock [offset] [size] - unlock section"
#endif
);

static int nand_load_image(cmd_tbl_t *cmdtp, nand_info_t *nand,
			   ulong offset, ulong addr, char *cmd)
{
	int r;
	char *ep, *s;
	size_t cnt;
	image_header_t *hdr;
#if defined(CONFIG_FIT)
	const void *fit_hdr = NULL;
#endif

	s = strchr(cmd, '.');
	if (s != NULL &&
	    (strcmp(s, ".jffs2") && strcmp(s, ".e") && strcmp(s, ".i"))) {
		printf("Unknown nand load suffix '%s'\n", s);
		show_boot_progress(-53);
		return 1;
	}

	printf("\nLoading from %s, offset 0x%lx\n", nand->name, offset);

	cnt = nand->writesize;
	r = nand_read_skip_bad(nand, offset, &cnt, (u_char *) addr);
	if (r) {
		puts("** Read error\n");
		show_boot_progress (-56);
		return 1;
	}
	show_boot_progress (56);

	switch (genimg_get_format ((void *)addr)) {
	case IMAGE_FORMAT_LEGACY:
		hdr = (image_header_t *)addr;

		show_boot_progress (57);
		image_print_contents (hdr);

		cnt = image_get_image_size (hdr);
		break;
#if defined(CONFIG_FIT)
	case IMAGE_FORMAT_FIT:
		fit_hdr = (const void *)addr;
		puts ("Fit image detected...\n");

		cnt = fit_get_size (fit_hdr);
		break;
#endif
	default:
		show_boot_progress (-57);
		puts ("** Unknown image type\n");
		return 1;
	}
	show_boot_progress (57);

	r = nand_read_skip_bad(nand, offset, &cnt, (u_char *) addr);
	if (r) {
		puts("** Read error\n");
		show_boot_progress (-58);
		return 1;
	}
	show_boot_progress (58);

#if defined(CONFIG_FIT)
	/* This cannot be done earlier, we need complete FIT image in RAM first */
	if (genimg_get_format ((void *)addr) == IMAGE_FORMAT_FIT) {
		if (!fit_check_format (fit_hdr)) {
			show_boot_progress (-150);
			puts ("** Bad FIT image format\n");
			return 1;
		}
		show_boot_progress (151);
		fit_print_contents (fit_hdr);
	}
#endif

	/* Loading ok, update default load address */

	load_addr = addr;

	/* Check if we should attempt an auto-start */
	if (((ep = getenv("autostart")) != NULL) && (strcmp(ep, "yes") == 0)) {
		char *local_args[2];
		extern int do_bootm(cmd_tbl_t *, int, int, char *[]);

		local_args[0] = cmd;
		local_args[1] = NULL;

		printf("Automatic boot of image at addr 0x%08lx ...\n", addr);

		do_bootm(cmdtp, 0, 1, local_args);
		return 1;
	}
	return 0;
}

int do_nandboot(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char *boot_device = NULL;
	int idx;
	ulong addr, offset = 0;
#if defined(CONFIG_CMD_MTDPARTS)
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;

	if (argc >= 2) {
		char *p = (argc == 2) ? argv[1] : argv[2];
		if (!(str2long(p, &addr)) && (mtdparts_init() == 0) &&
		    (find_dev_and_part(p, &dev, &pnum, &part) == 0)) {
			if (dev->id->type != MTD_DEV_TYPE_NAND) {
				puts("Not a NAND device\n");
				return 1;
			}
			if (argc > 3)
				goto usage;
			if (argc == 3)
				addr = simple_strtoul(argv[1], NULL, 16);
			else
				addr = CONFIG_SYS_LOAD_ADDR;
			return nand_load_image(cmdtp, &nand_info[dev->id->num],
					       part->offset, addr, argv[0]);
		}
	}
#endif

	show_boot_progress(52);
	switch (argc) {
	case 1:
		addr = CONFIG_SYS_LOAD_ADDR;
		boot_device = getenv("bootdevice");
		break;
	case 2:
		addr = simple_strtoul(argv[1], NULL, 16);
		boot_device = getenv("bootdevice");
		break;
	case 3:
		addr = simple_strtoul(argv[1], NULL, 16);
		boot_device = argv[2];
		break;
	case 4:
		addr = simple_strtoul(argv[1], NULL, 16);
		boot_device = argv[2];
		offset = simple_strtoul(argv[3], NULL, 16);
		break;
	default:
#if defined(CONFIG_CMD_MTDPARTS)
usage:
#endif
		cmd_usage(cmdtp);
		show_boot_progress(-53);
		return 1;
	}

	show_boot_progress(53);
	if (!boot_device) {
		puts("\n** No boot device **\n");
		show_boot_progress(-54);
		return 1;
	}
	show_boot_progress(54);

	idx = simple_strtoul(boot_device, NULL, 16);

	if (idx < 0 || idx >= CONFIG_SYS_MAX_NAND_DEVICE || !nand_info[idx].name) {
		printf("\n** Device %d not available\n", idx);
		show_boot_progress(-55);
		return 1;
	}
	show_boot_progress(55);

	return nand_load_image(cmdtp, &nand_info[idx], offset, addr, argv[0]);
}

U_BOOT_CMD(nboot, 4, 1, do_nandboot,
	"boot from NAND device",
	"[partition] | [[[loadAddr] dev] offset]"
);
#endif
