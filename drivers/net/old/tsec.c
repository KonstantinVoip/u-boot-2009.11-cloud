/*
 * Freescale Three Speed Ethernet Controller driver
 *
 * This software may be used and distributed according to the
 * terms of the GNU Public License, Version 2, incorporated
 * herein by reference.
 *
 * Copyright 2004-2010 Freescale Semiconductor, Inc.
 * (C) Copyright 2003, Motorola, Inc.
 * author Andy Fleming
 *
 */

#include <config.h>
#include <common.h>
#include <malloc.h>
#include <net.h>
#include <command.h>
#include <tsec.h>
#include <asm/errno.h>
#include <asm/io.h>

#include "miiphy.h"

DECLARE_GLOBAL_DATA_PTR;


#define  RX_buffer_descriptors_per_ring  128 // RX_RING_SIZE
#define  TX_buffer_descriptors_per_ring  128 // TX_RING_SIZE



//#define TX_BUF_CNT		2

static uint tsec1_rxIdx;		/* index of the current RX buffer_TSEC1 */
static uint tsec1_txIdx;		/* index of the current TX buffer_TSEC1 */

static uint tsec2_rxIdx;		/* index of the current RX buffer_TSEC2 */
static uint tsec2_txIdx;		/* index of the current TX buffer_TSEC2 */






typedef volatile struct tsec1 {
	txbd8_t txbd[TX_buffer_descriptors_per_ring];
	rxbd8_t rxbd[RX_buffer_descriptors_per_ring];
} TSEC1;


typedef volatile struct tsec2 {
	txbd8_t txbd[TX_buffer_descriptors_per_ring];
	rxbd8_t rxbd[RX_buffer_descriptors_per_ring];
} TSEC2;





static TSEC1 tsec1 __attribute__ ((aligned(64)));
static TSEC2 tsec2 __attribute__ ((aligned(64)));



volatile uint *RxPacet[RX_buffer_descriptors_per_ring];/* Receive packets		*/


//uint *R [PKTBUFSRX];/* Receive packets		*/



#define MAXCONTROLLERS	(8)

static struct tsec_private *privlist[MAXCONTROLLERS];
static int num_tsecs = 0;


//

#ifdef __GNUC__
//static RTXBD rtx __attribute__ ((aligned(8)));
#else
#error "rtx must be 64-bit aligned"
#endif

//void tsec_send(struct eth_device *dev,volatile void *packet, int length);
void tsec_send(struct eth_device *dev,struct eth_device *dev2);  


void tsec_recv(struct eth_device *dev,struct eth_device *dev2,struct eth_device *dev3);
static int tsec_init(struct eth_device *dev);
static void tsec_halt(struct eth_device *dev);
static void init_registers(volatile tsec_t * regs);
static void startup_tsec(struct eth_device *dev);
static int init_phy(struct eth_device *dev);
void write_phy_reg(struct tsec_private *priv, uint regnum, uint value);
uint read_phy_reg(struct tsec_private *priv, uint regnum);
struct phy_info *get_phy_info(struct eth_device *dev);
void phy_run_commands(struct tsec_private *priv, struct phy_cmd *cmd);
static void adjust_link(struct eth_device *dev);
#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII) \
	&& !defined(BITBANGMII)
static int tsec_miiphy_write(char *devname, unsigned char addr,unsigned char reg, unsigned short value);
static int tsec_miiphy_read(char *devname, unsigned char addr,unsigned char reg, unsigned short *value);
#endif
#ifdef CONFIG_MCAST_TFTP
static int tsec_mcast_addr (struct eth_device *dev, u8 mcast_mac, u8 set);
#endif

static struct tsec_info_struct tsec_info[] = {
#ifdef CONFIG_TSEC1
	STD_TSEC_INFO(1),	/* TSEC1 */
#endif
#ifdef CONFIG_TSEC2
	STD_TSEC_INFO(2),	/* TSEC2 */
#endif
#ifdef CONFIG_MPC85XX_FEC
	{
		.regs = (tsec_t *)(TSEC_BASE_ADDR + 0x2000),
		.miiregs = (tsec_mdio_t *)(MDIO_BASE_ADDR),
		.devname = CONFIG_MPC85XX_FEC_NAME,
		.phyaddr = FEC_PHY_ADDR,
		.flags = FEC_FLAGS
	},			/* FEC */
#endif
#ifdef CONFIG_TSEC3
	STD_TSEC_INFO(3),	/* TSEC3 */
#endif
#ifdef CONFIG_TSEC4
	STD_TSEC_INFO(4),	/* TSEC4 */
#endif
};


#define TEST_DATA_SIZE          0x200
u32 mas[TEST_DATA_SIZE] =
{
    0x02608c87, 0x65400000, 0x00006364, 0x01eeffff, 0x11111111, 0xffffffff,
    0x22222222, 0xffffffff, 0x33333333, 0xffffffff, 0x44444444, 0xffffffff,
    0x55555555, 0xffffffff, 0x66666666, 0xffffffff, 0x77777777, 0xffffffff,
    0x88888888, 0xffffffff, 0x99999999, 0xffffffff, 0xaaaaaaaa, 0xffffffff,
    0xbbbbbbbb, 0xffffffff, 0xcccccccc, 0xffffffff, 0xdddddddd, 0xffffffff,
 
    0xbbbbbbbb, 0xffffffff, 0xcccccccc, 0xffffffff, 0xdddddddd, 0xffffffff,
    0xbbbbbbbb, 0xffffffff, 0xcccccccc, 0xffffffff, 0xdddddddd, 0xffffffff,
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x11223344,
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x11223344,
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x11223344,
 
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x11223344,
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x11223344,
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x99999999,
    0x02608c87, 0x65400000, 0x00006364, 0x012affff, 0x11111111, 0xffffffff,
    0x22222222, 0xffffffff, 0x33333333, 0xffffffff, 0x44444444, 0xffffffff,
 
    0x55555555, 0xffffffff, 0x66666666, 0xffffffff, 0x77777777, 0xffffffff,
    0x88888888, 0xffffffff, 0x99999999, 0xffffffff, 0xaaaaaaaa, 0xffffffff,
    0xbbbbbbbb, 0xffffffff, 0xcccccccc, 0xffffffff, 0xdddddddd, 0xffffffff,
    0xbbbbbbbb, 0xffffffff, 0xcccccccc, 0xffffffff, 0xdddddddd, 0xffffffff,
    0xbbbbbbbb, 0xffffffff, 0xcccccccc, 0xffffffff, 0xdddddddd, 0xffffffff,
 
    0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x12345678, 0x11223344,
    0x11111111 };


///////////////////////////////////TSEC_INITIALIZE_FUNCTIONS/////////////////////////////	
int tsec_eth_init(bd_t *bis, struct tsec_info_struct *tsecs, int num)
{
   int i;
   struct eth_device dev[3];
   struct tsec_private priv[3];
   
   
   memset(&dev, 0, sizeof(dev));
   memset(&priv, 0, sizeof(priv));
   

   for(i=1;i<4;i++)
   {
   	privlist[i] = &priv[i];
	priv[i].regs =          tsecs[i-1].regs;
	priv[i].phyregs =       tsecs[i-1].miiregs;
	priv[i].phyregs_sgmii = tsecs[i-1].miiregs_sgmii;
    priv[i].phyaddr =       tsecs[i-1].phyaddr;
    
	switch(i-1)
	    {case 0:puts("###############INITIALIZATION_TSEC1-->");break;
	     case 1:puts("###############INITIALIZATION_TSEC2-->");break;
	     case 2:puts("###############INITIALIZATION_TSEC3-->");break;
	     case 3:puts("*************?????****************");break;	
		default:break;}
	
	
	
	switch(priv[i].phyaddr)
	    {case 0:puts("MDIO PHYADDR=0###########\n\r");break;
	     case 1:puts("MDIO PHYADDR=1###########\n\r");break;
	     case 2:puts("MDIO PHYADDR=2###########\n\r");break;
	     case 3:puts("MDIO PHYADDR=3###########\n\r");break;	
		default:break;}
	
	
	
    priv[i].flags = tsecs[i-1].flags;
	dev[i].iobase = 0;
	dev[i].priv = &priv[i];
	dev[i].init = tsec_init;
	dev[i].halt = tsec_halt;
	dev[i].send = tsec_send;
	dev[i].recv = tsec_recv;
	
	// Reset the MAC 
	priv[i].regs->maccfg1 |= MACCFG1_SOFT_RESET;
	udelay(2);  // Soft Reset must be asserted for 3 TX clocks 
	priv[i].regs->maccfg1 &= ~(MACCFG1_SOFT_RESET);
	
	     if(0==init_phy(&dev[i]))
	     {
	        tsec_halt(&dev[i]); 
			puts("0==init_phy(&dev[i]) error init phy no match\n\r");
	        puts("STOP TSEC DEVICE\n\r");
	     }
	     else
	     {
	        if(tsec_init(&dev[i])<0)
			{
			   
			tsec_halt(&dev[i]);  //Stop tsec! if no Link fo external Device PHY. 
			puts("STOP TSEC DEVICE\n\r");
			}
			 
    
	    }
   
    }

	
///////dev1 ->TSEC1->2eth
//////dev2 ->TSEC2->3eth	
//////dev3 ->TSEC3->4eth	
	
	
	
	///////////////////////////////////////////////MAIN NET LOOP/////////////////////////////////
     puts("##########################Start Ethernet CYCLE##############################\n\r");
	 tsec_recv(&dev[1],&dev[2],&dev[3]);
	
	 //tsec_send(&dev[2],&dev[1]);
	 
	 /*
	 for(;;)
	 {
	 tsec_recv(&dev[1],&dev[2]);
	 //tsec_send(&dev[2]);
	 }
	 */
	//tsec_recv(&dev[2]);
	 /*
    	  for(;;)
		  {
	       tsec_send(&dev[3], (void*)mas, 0x200);    
		  } 
	 */  
	   
return 0;
}

int tsec_standard_init(bd_t *bis)
{
	return tsec_eth_init(bis, tsec_info, ARRAY_SIZE(tsec_info));
}


/* Initializes data structures and registers for the controller,
 * and brings the interface up.	 Returns the link status, meaning
 * that it returns success if the link is up, failure otherwise.
 * This allows u-boot to find the first active controller.
 */
int tsec_init(struct eth_device *dev)
{
	uint tempval;
	char tmpbuf[MAC_ADDR_LEN];
	int i;
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	volatile tsec_t *regs = priv->regs;

	 
	puts("+++++++++tsec.c/tsec_init+++++++\n\r");
	/* Make sure the controller is stopped */
	tsec_halt(dev);

	/*Init MACCFG2.Defaults to GMII */
	//regs->maccfg2 = MACCFG2_INIT_SETTINGS;  // p.733 
      regs->maccfg2 = MACCFG2_INIT_SETTINGS_MPC;  /*Init MACCFG2.Defaults to RMII */
	  
	
	/* Init ECNTRL */
	//regs->ecntrl = ECNTRL_INIT_SETTINGS;
      regs->ecntrl = ECNTRL_INIT_SETTINGS_MPC;   //p.693
	/* Copy the station address into the address registers.
	 * Backwards, because little endian MACS are dumb */
	
	/*
	for (i = 0; i < MAC_ADDR_LEN; i++) 
	{
		tmpbuf[MAC_ADDR_LEN - 1 - i] = dev->enetaddr[i];
	}
	tempval = (tmpbuf[0] << 24) | (tmpbuf[1] << 16) | (tmpbuf[2] << 8) |tmpbuf[3];
	regs->macstnaddr1 = tempval;
	tempval = *((uint *) (tmpbuf + 4));
	regs->macstnaddr2 = tempval;
    */
	//MAC address 0x12345678ABCD
	
	regs->macstnaddr1=0xCDAB7856;   //p.741 MAC stantion address
	regs->macstnaddr2=0x34120000;
	/* reset the indices to zero */
	tsec1_rxIdx = 0;
	tsec1_txIdx = 0;

	tsec2_rxIdx = 0;
	tsec2_txIdx = 0;
	
	/* Clear out (for the most part) the other registers */
	init_registers(regs);

	/* Ready the device for tx/rx */
	startup_tsec(dev);

	/* If there's no link, fail */
	return (priv->link ? 0 : -1);
}
/* Initialized required registers to appropriate values, zeroing
 * those we don't care about (unless zero is bad, in which case,
 * choose a more appropriate value)
 */
static void init_registers(volatile tsec_t * regs)
{
	/* Clear IEVENT */
	regs->ievent = IEVENT_INIT_CLEAR;

	regs->imask = IMASK_INIT_CLEAR;

	regs->hash.iaddr0 = 0;
	regs->hash.iaddr1 = 0;
	regs->hash.iaddr2 = 0;
	regs->hash.iaddr3 = 0;
	regs->hash.iaddr4 = 0;
	regs->hash.iaddr5 = 0;
	regs->hash.iaddr6 = 0;
	regs->hash.iaddr7 = 0;

	regs->hash.gaddr0 = 0;
	regs->hash.gaddr1 = 0;
	regs->hash.gaddr2 = 0;
	regs->hash.gaddr3 = 0;
	regs->hash.gaddr4 = 0;
	regs->hash.gaddr5 = 0;
	regs->hash.gaddr6 = 0;
	regs->hash.gaddr7 = 0;

	//RCTRL_regs///////////////
	 //regs->rctrl = 0x00000000;               /////////////p.713
     regs->rctrl = RCTRL_PromiscEN;
	 puts("++++++++PromiscMode_Enable+++++\n\r");
	
	/* Init RMON mib registers */
	memset((void *)&(regs->rmon), 0, sizeof(rmon_mib_t));

	regs->rmon.cam1 = 0xffffffff;
	regs->rmon.cam2 = 0xffffffff;

	
	
	regs->mrblr =  MRBLR_INIT_SETTINGS;   //Maximum Receive Buffer Length Register (MRBLR) p.725
	regs->minflr = MINFLR_INIT_SETTINGS;

	regs->attr    = ATTR_INIT_SETTINGS_MPC;  //Allocate in L2 cache_memory
	//regs->attr    = ATTR_INIT_SETTINGS;       //Attribute Register (ATTR)  p.773
	regs->attreli = ATTRELI_INIT_SETTINGS; //Attribute Extract Length and Extract Index Register (ATTRELI) p.774

}



/* Writes the given phy's reg with value, using the specified MDIO regs */
static void tsec_local_mdio_write(volatile tsec_mdio_t *phyregs, uint addr,uint reg, uint value)
{
	int timeout = 1000000;

	phyregs->miimadd = (addr << 8) | reg;
	phyregs->miimcon = value;
	asm("sync");

	timeout = 1000000;
	while ((phyregs->miimind & MIIMIND_BUSY) && timeout--) ;
}


/* Provide the default behavior of writing the PHY of this ethernet device */
#define write_phy_reg(priv, regnum, value) tsec_local_mdio_write(priv->phyregs,priv->phyaddr,regnum,value)

/* Reads register regnum on the device's PHY through the
 * specified registers.	 It lowers and raises the read
 * command, and waits for the data to become valid (miimind
 * notvalid bit cleared), and the bus to cease activity (miimind
 * busy bit cleared), and then returns the value
 */
uint tsec_local_mdio_read(volatile tsec_mdio_t *phyregs, uint phyid, uint regnum)
{
	uint value;

	/* Put the address of the phy, and the register
	 * number into MIIMADD */
	phyregs->miimadd = (phyid << 8) | regnum;

	/* Clear the command register, and wait */
	phyregs->miimcom = 0;
	asm("sync");

	/* Initiate a read command, and wait */
	phyregs->miimcom = MIIM_READ_COMMAND;
	asm("sync");

	/* Wait for the the indication that the read is done */
	while ((phyregs->miimind & (MIIMIND_NOTVALID | MIIMIND_BUSY))) ;

	/* Grab the value read from the PHY */
	value = phyregs->miimstat;

	return value;
}

/* #define to provide old read_phy_reg functionality without duplicating code */
#define read_phy_reg(priv,regnum) tsec_local_mdio_read(priv->phyregs,priv->phyaddr,regnum)

#define TBIANA_SETTINGS ( \
		TBIANA_ASYMMETRIC_PAUSE \
		| TBIANA_SYMMETRIC_PAUSE \
		| TBIANA_FULL_DUPLEX \
		)

#define TBICR_SETTINGS ( \
		TBICR_PHY_RESET \
		| TBICR_ANEG_ENABLE \
		| TBICR_FULL_DUPLEX \
		| TBICR_SPEED1_SET \
		)
/* Configure the TBI for SGMII operation */
static void tsec_configure_serdes(struct tsec_private *priv)
{
	/* Access TBI PHY registers at given TSEC register offset as opposed to the
	 * register offset used for external PHY accesses */
	tsec_local_mdio_write(priv->phyregs_sgmii, priv->regs->tbipa, TBI_ANA,TBIANA_SETTINGS);
	tsec_local_mdio_write(priv->phyregs_sgmii, priv->regs->tbipa, TBI_TBICON,TBICON_CLK_SELECT);
	tsec_local_mdio_write(priv->phyregs_sgmii, priv->regs->tbipa, TBI_CR,TBICR_SETTINGS);
}

/* Discover which PHY is attached to the device, and configure it
 * properly.  If the PHY is not recognized, then return 0
 * (failure).  Otherwise, return 1
 */
static int init_phy(struct eth_device *dev)
{
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	struct phy_info *curphy;
	volatile tsec_mdio_t *phyregs = priv->phyregs;
	volatile tsec_t *regs = priv->regs;

	/* Assign a Physical address to the TBI */
	regs->tbipa = CONFIG_SYS_TBIPA_VALUE;
	asm("sync");
    //printf("+++++++++tsec.c/init_phy+++++++\n\r");
	puts("+++++++++tsec.c/init_phy+++++++\n\r");
	/* Reset MII (due to new addresses) */
	priv->phyregs->miimcfg = MIIMCFG_RESET;
	asm("sync");
	priv->phyregs->miimcfg = MIIMCFG_INIT_VALUE;
	asm("sync");
	while (priv->phyregs->miimind & MIIMIND_BUSY) ;

	
	
	/* Get the cmd structure corresponding to the attached
	 * PHY */
	curphy = get_phy_info(dev);
    
	if (curphy == NULL) 
	{
		priv->phyinfo = NULL;
		//printf("%s: No PHY found\n", dev->name);
		puts("No PHY found\n\r");
		return 0;
	}

	/*
	if (regs->ecntrl & ECNTRL_SGMII_MODE)
		tsec_configure_serdes(priv);*/

	priv->phyinfo = curphy;
	phy_run_commands(priv, priv->phyinfo->config);

	return 1;
}

/*
 * Returns which value to write to the control register.
 * For 10/100, the value is slightly different
 */
uint mii_cr_init(uint mii_reg, struct tsec_private * priv)
{
	if (priv->flags & TSEC_GIGABIT)
		return MIIM_CONTROL_INIT;
	else
		return MIIM_CR_INIT;
}

/*
 * Wait for auto-negotiation to complete, then determine link
 */
uint mii_parse_sr(uint mii_reg, struct tsec_private * priv)
{
	/*
	 * Wait if the link is up, and autonegotiation is in progress
	 * (ie - we're capable and it's not done)
	 */
	mii_reg = read_phy_reg(priv, MIIM_STATUS);
	//printf ("+++++++++tsec.c/mii_parse_sr+++++++++\n\r"); 
	  puts ("+++++++++tsec.c/mii_parse_sr+++++++++\n\r"); 
	
	
	if ((mii_reg & PHY_BMSR_AUTN_ABLE) && !(mii_reg & PHY_BMSR_AUTN_COMP)) 
	{
		int i = 0;   
		//puts("Waiting for PHY auto negotiation to complete");
		//printf("Waiting for PHY auto negotiation to complete");  
		  //puts("+++++++++tsec.c/mii_parse_sr+++++++++\n\r"); 
		
		while (!(mii_reg & PHY_BMSR_AUTN_COMP)) {
			/*
			 * Timeout reached ?
			 */
			if (i > PHY_AUTONEGOTIATE_TIMEOUT) {
				  puts(" TIMEOUT !\n\r");
				//printf(" TIMEOUT !\n");  
				priv->link = 0;
				return 0;
			}

			if (ctrlc())
			{
				 puts("user interrupt!\n\r");
				//printf("user interrupt!\n");
				priv->link = 0;
				return -EINTR;
			}

			if ((i++ % 1000) == 0)
			{
				//printf('.');
				  puts(".");
			 
			}
			udelay(1000);	/* 1 ms */
			mii_reg = read_phy_reg(priv, MIIM_STATUS);
		}
		    puts("done\n\r");
         // printf(" done\n");
		
		/* Link status bit is latched low, read it again */
		mii_reg = read_phy_reg(priv, MIIM_STATUS);

		udelay(500000);	/* another 500 ms (results in faster booting) */
	}

	priv->link = mii_reg & MIIM_STATUS_LINK ? 1 : 0;

	return 0;
}

/* Generic function which updates the speed and duplex.  If
 * autonegotiation is enabled, it uses the AND of the link
 * partner's advertised capabilities and our advertised
 * capabilities.  If autonegotiation is disabled, we use the
 * appropriate bits in the control register.
 *
 * Stolen from Linux's mii.c and phy_device.c
 */
uint mii_parse_link(uint mii_reg, struct tsec_private *priv)
{
	/* We're using autonegotiation */
	if (mii_reg & PHY_BMSR_AUTN_ABLE) {
		uint lpa = 0;
		uint gblpa = 0;

		/* Check for gigabit capability */
		if (mii_reg & PHY_BMSR_EXT) {
			/* We want a list of states supported by
			 * both PHYs in the link
			 */
			gblpa = read_phy_reg(priv, PHY_1000BTSR);
			gblpa &= read_phy_reg(priv, PHY_1000BTCR) << 2;
		}

		/* Set the baseline so we only have to set them
		 * if they're different
		 */
		priv->speed = 10;
		priv->duplexity = 0;

		/* Check the gigabit fields */
		if (gblpa & (PHY_1000BTSR_1000FD | PHY_1000BTSR_1000HD)) {
			priv->speed = 1000;

			if (gblpa & PHY_1000BTSR_1000FD)
				priv->duplexity = 1;

			/* We're done! */
			return 0;
		}

		lpa = read_phy_reg(priv, PHY_ANAR);
		lpa &= read_phy_reg(priv, PHY_ANLPAR);

		if (lpa & (PHY_ANLPAR_TXFD | PHY_ANLPAR_TX)) {
			priv->speed = 100;

			if (lpa & PHY_ANLPAR_TXFD)
				priv->duplexity = 1;

		} else if (lpa & PHY_ANLPAR_10FD)
			priv->duplexity = 1;
	} else {
		uint bmcr = read_phy_reg(priv, PHY_BMCR);

		priv->speed = 10;
		priv->duplexity = 0;

		if (bmcr & PHY_BMCR_DPLX)
			priv->duplexity = 1;

		if (bmcr & PHY_BMCR_1000_MBPS)
			priv->speed = 1000;
		else if (bmcr & PHY_BMCR_100_MBPS)
			priv->speed = 100;
	}

	return 0;
}






/* Configure maccfg2 based on negotiated speed and duplex
 * reported by PHY handling code
 */
static void adjust_link(struct eth_device *dev)
{
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	volatile tsec_t *regs = priv->regs;
	//printf("+++++++++tsec.c/adjust_link+++++++++\n\r"); 
	puts("+++++++++tsec.c/adjust_link+++++++++\n\r");
	//eth_get_dev_index();
	if (priv->link) 
	{
		if (priv->duplexity != 0)
			regs->maccfg2 |= MACCFG2_FULL_DUPLEX;
		else
			regs->maccfg2 &= ~(MACCFG2_FULL_DUPLEX);

		switch (priv->speed)
		{
		    case 1000:
			         regs->maccfg2 = ((regs->maccfg2 & ~(MACCFG2_IF))| MACCFG2_GMII);
			break;
		
		
		    case 100:
		    case 10:
			       regs->maccfg2 = ((regs->maccfg2 & ~(MACCFG2_IF))| MACCFG2_MII);
				   puts ("case 10: regs->maccfg2 \n\r");
                   /* Set R100 bit in all modes although
			        * it is only used in RGMII mode
			        */
			       if (priv->speed == 100)
				   regs->ecntrl |= ECNTRL_R100; 
				   else
				   regs->ecntrl &= ~(ECNTRL_R100);
			       break;
		
		    default:
			//printf("%s: Speed was bad\n", dev->name);
			  puts ("Tsec_Speed was bad\n\r");
			 break;
		}

		//printf("Speed: %d, %s duplex\n", priv->speed,(priv->duplexity) ? "full" : "half");
        //puts ("Speed:\n\r");
	} 
	else 
	{
		//printf("%s: No link.\n", dev->name);
	      puts ("Tsec_Link is down\n\r");
	};
}

/* Set up the buffers and their descriptors, and bring up the
 * interface
 */
static void startup_tsec(struct eth_device *dev)
{
	int i,in_phyaddr;
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	volatile tsec_t *regs = priv->regs;
	puts("+++++++++tsec.c/startup_tsec+++++++\n\r");
    in_phyaddr=priv->phyaddr;
	

	
	if(in_phyaddr==1)
	{
	     puts("buffer_descriptor_point_TSEC1!!!!!!!!\n\r");
	     // Point to the buffer descriptors 
	     regs->tbase = (unsigned int)(&tsec1.txbd[tsec1_txIdx]);
	     regs->rbase = (unsigned int)(&tsec1.rxbd[tsec1_rxIdx]);

	     //Initialize the Rx Buffer descriptors
	     for (i = 0; i < RX_buffer_descriptors_per_ring; i++)
	     {
		 tsec1.rxbd[i].status = RXBD_EMPTY;
		 tsec1.rxbd[i].length = 0;
		 tsec1.rxbd[i].bufPtr =0;//RxPacet[i];
	     }
	     tsec1.rxbd[RX_buffer_descriptors_per_ring - 1].status |= RXBD_WRAP;

	
	    //Initialize the TX Buffer Descriptors
	    for (i = 0; i < TX_buffer_descriptors_per_ring; i++) 
	    {
		tsec1.txbd[i].status = 0;
		tsec1.txbd[i].length = 0;
		tsec1.txbd[i].bufPtr = 0;
	    }
	    tsec1.txbd[TX_buffer_descriptors_per_ring - 1].status |= TXBD_WRAP;

	}
	
	if((in_phyaddr==2)||(in_phyaddr==3))
	{
	     puts("buffer_descriptor_point_TSEC2!!!!!!!!\n\r");
	     // Point to the buffer descriptors 
	     regs->tbase = (unsigned int)(&tsec2.txbd[tsec2_txIdx]);
	     regs->rbase = (unsigned int)(&tsec2.rxbd[tsec2_rxIdx]);

	     //Initialize the Rx Buffer descriptors
	     for (i = 0; i < RX_buffer_descriptors_per_ring; i++)
	     {
		 tsec2.rxbd[i].status = RXBD_EMPTY;
		 tsec2.rxbd[i].length = 0;
		 tsec2.rxbd[i].bufPtr = 0;
	     }
	     tsec2.rxbd[RX_buffer_descriptors_per_ring - 1].status |= RXBD_WRAP;

	
	    //Initialize the TX Buffer Descriptors
	    for (i = 0; i < TX_buffer_descriptors_per_ring; i++) 
	    {
		tsec2.txbd[i].status = 0;
		tsec2.txbd[i].length = 0;
		tsec2.txbd[i].bufPtr = 0;
	    }
	    tsec2.txbd[TX_buffer_descriptors_per_ring - 1].status |= TXBD_WRAP;

	}
	
	
	
	
	// Start up the PHY
	if(priv->phyinfo)
		phy_run_commands(priv, priv->phyinfo->startup);

	adjust_link(dev);

	// Enable Transmit and Receive 
	regs->maccfg1 |= (MACCFG1_RX_EN | MACCFG1_TX_EN);
     
	  //Set Internal Tsec Loopback Mode    tx->rx
	 // regs->maccfg1 |= (MACCFG1_RX_EN | MACCFG1_TX_EN|MACCFG1_LOOPBACK);
	
	// Tell the DMA it is clear to go
	regs->dmactrl |= DMACTRL_INIT_SETTINGS;
	regs->tstat = TSTAT_CLEAR_THALT;
	regs->rstat = RSTAT_CLEAR_RHALT;
	regs->dmactrl &= ~(DMACTRL_GRS | DMACTRL_GTS);
}

/* This returns the status bits of the device.	The return value
 * is never checked, and this is what the 8260 driver did, so we
 * do the same.	 Presumably, this would be zero if there were no
 * errors
 */

void tsec_send(struct eth_device *dev,struct eth_device *dev2)
//void tsec_send(struct eth_device *dev, volatile void *packet, int length)
{
	int i;
	int in_phyaddr;
	//int result = 0;
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	volatile tsec_t *regs = priv->regs;
	in_phyaddr=priv->phyaddr;
    //printf("+++++++++tsec.c/tsec_send+++++++\n\r"); 
	//puts("+++++++++tsec.c/tsec_send+++++++\n\r"); 
	// Find an empty buffer descriptor */
 	 
	//tsec2.txbd[txIdx].bufPtr = RxPacet;
    //tsec2.txbd[txIdx].length =0x200; //length;
	
	/*  
	
	tsec2.txbd[txIdx].status |=(TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT);
	// Tell the DMA to go
	regs->tstat = TSTAT_CLEAR_THALT;

	// Wait for buffer to be transmitted
	for (i = 0; tsec2.txbd[txIdx].status & TXBD_READY; i++) 
	{
		if (i >= TOUT_LOOP) 
		{
			//debug("%s: tsec: tx error\n", dev->name);
			puts("tsec: tx error\n\r");
			//return result;
		}
	}
	
	txIdx = (txIdx + 1) % TX_buffer_descriptors_per_ring;
	//result = rtx.txbd[txIdx].status & TXBD_STATS;
	//return result;
    */
	
}

void tsec_recv(struct eth_device *dev,struct eth_device *dev2,struct eth_device *dev3)
{
	int in_phyaddr,i;
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	volatile tsec_t *regs = priv->regs;
	
	struct tsec_private *priv2 = (struct tsec_private *)dev2->priv;
	volatile tsec_t *regs2 = priv2->regs;
	
	struct tsec_private *priv3 = (struct tsec_private *)dev3->priv;
	volatile tsec_t *regs3 = priv3->regs;
	
	
	
	for(;;)
	{
	
	     while (!(tsec1.rxbd[tsec1_rxIdx].status & RXBD_EMPTY))
	       {
		        	
				 
				/**********Transmit TSEC1 rx  =>>> to TSEC2 tx*/ 
			    tsec2.txbd[tsec2_txIdx].bufPtr = tsec1.rxbd[tsec1_rxIdx].bufPtr;
	            tsec2.txbd[tsec2_txIdx].length = tsec1.rxbd[tsec1_rxIdx].length-4; 
		    	tsec2.txbd[tsec2_txIdx].status |=(TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT);
	            
								//Transmit TSEC2 rx -------->TSEC1 tx 
				tsec1.txbd[tsec1_txIdx].bufPtr = tsec1.rxbd[tsec1_rxIdx].bufPtr;
	            tsec1.txbd[tsec1_txIdx].length =  tsec1.rxbd[tsec1_rxIdx].length-4; 
		    	tsec1.txbd[tsec1_txIdx].status |=(TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT);
				
				
				
				puts("Frame Recieve\n\r");
				
				// Tell the DMA to go
	            regs->tstat = TSTAT_CLEAR_THALT;
				tsec1_txIdx = (tsec1_txIdx + 1) % TX_buffer_descriptors_per_ring;
				
				
				/* Tell the DMA to go */
	            regs2->tstat = TSTAT_CLEAR_THALT;
				tsec2_txIdx = (tsec2_txIdx + 1) % TX_buffer_descriptors_per_ring; 
				
				
				/* Tell the DMA to go */
	            regs3->tstat = TSTAT_CLEAR_THALT;
				tsec2_txIdx = (tsec2_txIdx + 1) % TX_buffer_descriptors_per_ring; 
				
                /**********Transmit TSEC1 rx  =>>> to TSEC2 tx*/
				eieio();
				 
				  
				tsec1.rxbd[tsec1_rxIdx].length = 0;
		        /* Set the wrap bit if this is the last element in the list */
		        tsec1.rxbd[tsec1_rxIdx].status = RXBD_EMPTY | (((tsec1_rxIdx + 1) == RX_buffer_descriptors_per_ring) ? RXBD_WRAP : 0);
 	            tsec1_rxIdx = (tsec1_rxIdx + 1) % RX_buffer_descriptors_per_ring;	
			}	 
		    if (regs->ievent & IEVENT_BSY) 
	        {regs->ievent = IEVENT_BSY;regs->rstat = RSTAT_CLEAR_RHALT;}  	 
				 
			 	 
			/*	 
			while(!(tsec2.rxbd[tsec2_rxIdx].status & RXBD_EMPTY))
			{
				 
				//Transmit TSEC2 rx -------->TSEC1 tx 
				tsec1.txbd[tsec1_txIdx].bufPtr = tsec2.rxbd[tsec2_rxIdx].bufPtr;
	            tsec1.txbd[tsec1_txIdx].length = tsec2.rxbd[tsec2_rxIdx].length-4; 
		    	tsec1.txbd[tsec1_txIdx].status |=(TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT);
	            // Tell the DMA to go
	            regs->tstat = TSTAT_CLEAR_THALT;
				tsec1_txIdx = (tsec1_txIdx + 1) % TX_buffer_descriptors_per_ring;
				//Transmit TSEC2 rx -------->TSEC1 tx 
				eieio();
				 tsec2.rxbd[tsec2_rxIdx].length = 0;
		        // Set the wrap bit if this is the last element in the list 
		        tsec2.rxbd[tsec2_rxIdx].status = RXBD_EMPTY | (((tsec2_rxIdx + 1) == RX_buffer_descriptors_per_ring) ? RXBD_WRAP : 0);
 	            tsec2_rxIdx = (tsec2_rxIdx + 1) % RX_buffer_descriptors_per_ring;
					
				} 	
            if (regs2->ievent & IEVENT_BSY) 
	        {regs2->ievent = IEVENT_BSY;regs2->rstat = RSTAT_CLEAR_RHALT;} 
            */

   }
  
}






/*//////////////// Stop the eTSEC interface ////////////////////*/
static void tsec_halt(struct eth_device *dev)
{
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	volatile tsec_t *regs = priv->regs;
 
	regs->dmactrl &= ~(DMACTRL_GRS | DMACTRL_GTS);
	regs->dmactrl |= (DMACTRL_GRS | DMACTRL_GTS);
     //printf("+++++++++tsec.c/tsec_halt+++++++\n\r"); 
	 puts("+++++++++tsec.c/tsec_halt+++++++\n\r");
	while ((regs->ievent & (IEVENT_GRSC | IEVENT_GTSC))
			!= (IEVENT_GRSC | IEVENT_GTSC)) ;

	regs->maccfg1 &= ~(MACCFG1_TX_EN | MACCFG1_RX_EN);

	/* Shut down the PHY, as needed */
	if(priv->phyinfo)
		phy_run_commands(priv, priv->phyinfo->shutdown);
}









/* Parse the vsc8244's status register for speed and duplex
 * information
 */
uint mii_parse_vsc8244(uint mii_reg, struct tsec_private * priv)
{
	uint speed;

	if (mii_reg & MIIM_VSC8244_AUXCONSTAT_DUPLEX)
		priv->duplexity = 1;
	else
		priv->duplexity = 0;

	speed = mii_reg & MIIM_VSC8244_AUXCONSTAT_SPEED;
	switch (speed) 
	{
	case MIIM_VSC8244_AUXCONSTAT_GBIT:
		priv->speed = 1000;
		puts("vsc8244_SPEED=1000M/bit\n\r");
		break;
	case MIIM_VSC8244_AUXCONSTAT_100:
		priv->speed = 100;
		puts("vsc8244_SPEED=100M/bit\n\r");
		break;
	default:
		priv->speed = 10;
		puts("vsc8244_SPEED=10M/bit\n\r");
		break;
	}

	return 0;
}


struct phy_info phy_info_VSC8641 = {
	0x7043,
	"Vitesse VSC8641",
	4,
	(struct phy_cmd[]){	/* config */
			   /* Configure some basic stuff */
			   {MIIM_CONTROL, MIIM_CONTROL_INIT, &mii_cr_init},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	/* startup */
			   /* Read the Status (2x to make sure link is right) */
			   {MIIM_STATUS, miim_read, NULL},
			   /* Auto-negotiate */
			   {MIIM_STATUS, miim_read, &mii_parse_sr},
			   /* Read the status */
			   {MIIM_VSC8244_AUX_CONSTAT, miim_read,
			    &mii_parse_vsc8244},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	/* shutdown */
			   {miim_end,}
			   },
};

struct phy_info phy_info_VSC8221 = {
	0xfc55,
	"Vitesse VSC8221",
	4,
	(struct phy_cmd[]){	/* config */
			   /* Configure some basic stuff */
			   {MIIM_CONTROL, MIIM_CONTROL_INIT, &mii_cr_init},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	/* startup */
			   /* Read the Status (2x to make sure link is right) */
			   {MIIM_STATUS, miim_read, NULL},
			   /* Auto-negotiate */
			   {MIIM_STATUS, miim_read, &mii_parse_sr},
			   /* Read the status */
			   {MIIM_VSC8244_AUX_CONSTAT, miim_read,
			    &mii_parse_vsc8244},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	/* shutdown */
			   {miim_end,}
			   },
};



/* a generic flavor.  */
struct phy_info phy_info_generic =  {
	0,
	"Unknown/Generic PHY",
	32,
	(struct phy_cmd[]) { /* config */
		{PHY_BMCR, PHY_BMCR_RESET, NULL},
		{PHY_BMCR, PHY_BMCR_AUTON|PHY_BMCR_RST_NEG, NULL},
		{miim_end,}
	},
	(struct phy_cmd[]) { /* startup */
		{PHY_BMSR, miim_read, NULL},
		{PHY_BMSR, miim_read, &mii_parse_sr},
		{PHY_BMSR, miim_read, &mii_parse_link},
		{miim_end,}
	},
	(struct phy_cmd[]) { /* shutdown */
		{miim_end,}
	}
};


/* Parse the DP83848's link and auto-neg status register for speed and duplex
 * information
 */
uint mii_parse_dp83848_lanr(uint mii_reg, struct tsec_private *priv)
{
  //printf("DP83848 PHY_STATUS_REGISTER= %x\n",mii_reg);
  //printf("+++++++++tsec.c/mii_parse_dp83848_lanr+++++++\n\r");
    puts ("+++++++++tsec.c/mii_parse_dp83848_lanr+++++++\n\r");
	////////////////////////////Link/////////////////////////////////
	if (mii_reg & MIIM_DP83848_LINK_MASK_MASK)
	{
	puts("dp83848_Link is UP\n\r"); 
	}
	else	
	{
	puts("dp83848_Link is down\n\r");  
	return 0;
	}
	
    ////////////////////////////////////Speed////////////////////////////////////////////
	switch (mii_reg & MIIM_DP83848_SPD_MASK ) {
	case MIIM_DP83848_SPD_100:
		priv->speed = 100;
		puts("dp83848_SPEED=100M/bit\n\r");
		break;

	case MIIM_DP83848_SPD_10:
		priv->speed = 10;
		puts("dp83848_SPEED=10M/bit\n\r");
		break;

	default:
		priv->speed = 10;
		puts("dp83848_Error SPEED=default\n\r");
		break;

	}

    ///////////////////////////////DUplex//////////////////////////
	if (mii_reg & MIIM_DP83848_DPX_MASK)
	{
		priv->duplexity = 1; //FULL DUPLEX
	    puts("dp83848_FULL DUPLEX\n\r"); 
	}
	else
	{
		priv->duplexity = 0; //HALF DUPLEX
        puts("dp83848_HALF DUPLEX\n\r"); 
    }

	    ////////////////////////////LoopBaCK MODE/////////////////////////////////
	if (mii_reg & MIIM_DP83848_LOOPBACK_MASK)
	{puts("dp83848_Loopback enabled.\n\r"); }
	else
	{puts("dp83848_Normal operation.\n\r"); }
 
 return 0;
}

struct phy_info phy_info_dp83848 = {
	0x20005c90,
	"NatSemi DP83848",
	0,
	(struct phy_cmd[]){	// config 
			   {MIIM_CONTROL, MIIM_DP83848_CR_INIT, NULL},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	// startup
			   // Status is read once to clear old link state 
			   {MIIM_STATUS, miim_read, NULL},
			   // Auto-negotiate 
			   {MIIM_STATUS, miim_read, &mii_parse_sr},
			   // Read the link and auto-neg status 
			   {PHY_DP83848_STATUS_REGISTER , miim_read,&mii_parse_dp83848_lanr},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	// shutdown 
			   {miim_end,}
			   },
};




/* Parse the DP83865's link and auto-neg status register for speed and duplex
 * information
 */
uint mii_parse_dp83865_lanr(uint mii_reg, struct tsec_private *priv)
{
	switch (mii_reg & MIIM_DP83865_SPD_MASK) {

	case MIIM_DP83865_SPD_1000:
		priv->speed = 1000;
		break;

	case MIIM_DP83865_SPD_100:
		priv->speed = 100;
		break;

	default:
		priv->speed = 10;
		break;

	}

	if (mii_reg & MIIM_DP83865_DPX_FULL)
		priv->duplexity = 1;
	else
		priv->duplexity = 0;

	return 0;
}

struct phy_info phy_info_dp83865 = {
	0x20005c7,
	"NatSemi DP83865",
	4,
	(struct phy_cmd[]){	/* config */
			   {MIIM_CONTROL, MIIM_DP83865_CR_INIT, NULL},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	/* startup */
			   /* Status is read once to clear old link state */
			   {MIIM_STATUS, miim_read, NULL},
			   /* Auto-negotiate */
			   {MIIM_STATUS, miim_read, &mii_parse_sr},
			   /* Read the link and auto-neg status */
			   {MIIM_DP83865_LANR, miim_read,
			    &mii_parse_dp83865_lanr},
			   {miim_end,}
			   },
	(struct phy_cmd[]){	/* shutdown */
			   {miim_end,}
			   },
};


struct phy_info *phy_info[] = {
	&phy_info_VSC8641,
	&phy_info_VSC8221,
	&phy_info_dp83865,
	&phy_info_dp83848,
	&phy_info_generic,	/* must be last; has ID 0 and 32 bit mask */
	NULL
};



/* Grab the identifier of the device's PHY, and search through
 * all of the known PHYs to see if one matches.	 If so, return
 * it, if not, return NULL
 */
struct phy_info *get_phy_info(struct eth_device *dev)
{
	struct tsec_private *priv = (struct tsec_private *)dev->priv;
	uint phy_reg, phy_ID;
	int i;
	struct phy_info *theInfo = NULL;
    //printf("+++++++++tsec.c/get_phy_info+++++++\n\r");
	  puts("+++++++++tsec.c/get_phy_info+++++++\n\r");
	/* Grab the bits from PHYIR1, and put them in the upper half */
	phy_reg = read_phy_reg(priv, MIIM_PHYIR1);
	phy_ID = (phy_reg & 0xffff) << 16;
    //printf("PHY_ID_REGISTER1=%x\n",phy_reg);
	/* Grab the bits from PHYIR2, and put them in the lower half */
	phy_reg = read_phy_reg(priv, MIIM_PHYIR2);
	phy_ID |= (phy_reg & 0xffff);
    //printf("PHY_ID_REGISTER2=%x\n",phy_reg);
	/* loop through all the known PHY types, and find one that */
	/* matches the ID we read from the PHY. */
	for (i = 0; phy_info[i]; i++) 
	{
		if (phy_info[i]->id == (phy_ID >> phy_info[i]->shift)) 
		{
			theInfo = phy_info[i];
			break;
		}
	}

	if (theInfo == &phy_info_generic) 
	{
		//printf("%s: No support for PHY id %x; assuming generic\n", dev->name, phy_ID);
	    puts("No support for PHY id\n\r"); 
	} else 
	{
		//debug("%s: PHY is %s (%x)\n", dev->name, theInfo->name, phy_ID);
	      //printf("%s: PHY is %s (%x)\n", dev->name, theInfo->name, phy_ID);
	     puts("----------------------PHY is Complete initialize------------------\n\r");
	}

	return theInfo;
}

/* Execute the given series of commands on the given device's
 * PHY, running functions as necessary
 */
void phy_run_commands(struct tsec_private *priv, struct phy_cmd *cmd)
{
	int i;
	uint result;
	volatile tsec_mdio_t *phyregs = priv->phyregs;

	phyregs->miimcfg = MIIMCFG_RESET;
	phyregs->miimcfg = MIIMCFG_INIT_VALUE;

	puts("++++++++tsec.c/phy_run_commands+++++++++++\n\r"); 
	 
	while (phyregs->miimind & MIIMIND_BUSY) ;

	for (i = 0; cmd->mii_reg != miim_end; i++) {
		if (cmd->mii_data == miim_read) {
			result = read_phy_reg(priv, cmd->mii_reg);

			if (cmd->funct != NULL)
				(*(cmd->funct)) (result, priv);

		} else {
			if (cmd->funct != NULL)
				result = (*(cmd->funct)) (cmd->mii_reg, priv);
			else
				result = cmd->mii_data;

			write_phy_reg(priv, cmd->mii_reg, result);

		}
		cmd++;
	}
}

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII) \
	&& !defined(BITBANGMII)

/*
 * Read a MII PHY register.
 *
 * Returns:
 *  0 on success
 */
static int tsec_miiphy_read(char *devname, unsigned char addr,unsigned char reg, unsigned short *value)
{
	unsigned short ret;
	struct tsec_private *priv = privlist[0];
    //printf("+++++++++tsec.c/tsec_miiphy_read+++++++\n\r");  
	  puts("+++++++++tsec.c/tsec_miiphy_read+++++++\n\r");
	if (NULL == priv) {
		//printf("Can't read PHY at address %d\n", addr);
		  puts("Can't read PHY at address\n\r");
		return -1;
	}

	ret = (unsigned short)tsec_local_mdio_read(priv->phyregs, addr, reg);
	*value = ret;

	return 0;
}

/*
 * Write a MII PHY register.
 *
 * Returns:
 *  0 on success
 */
static int tsec_miiphy_write(char *devname, unsigned char addr,unsigned char reg, unsigned short value)
{
	struct tsec_private *priv = privlist[0];
    //printf("+++++++++tsec.c/tsec_miiphy_write+++++++\n\r"); 
	  puts("+++++++++tsec.c/tsec_miiphy_write+++++++\n\r");
	if (NULL == priv) {
		//printf("Can't write PHY at address %d\n", addr);
		   puts("Can't write PHY at address\n\r");
		return -1;
	}

	tsec_local_mdio_write(priv->phyregs, addr, reg, value);

	return 0;
}

#endif

#ifdef CONFIG_MCAST_TFTP

/* CREDITS: linux gianfar driver, slightly adjusted... thanx. */

/* Set the appropriate hash bit for the given addr */

/* The algorithm works like so:
 * 1) Take the Destination Address (ie the multicast address), and
 * do a CRC on it (little endian), and reverse the bits of the
 * result.
 * 2) Use the 8 most significant bits as a hash into a 256-entry
 * table.  The table is controlled through 8 32-bit registers:
 * gaddr0-7.  gaddr0's MSB is entry 0, and gaddr7's LSB is
 * gaddr7.  This means that the 3 most significant bits in the
 * hash index which gaddr register to use, and the 5 other bits
 * indicate which bit (assuming an IBM numbering scheme, which
 * for PowerPC (tm) is usually the case) in the tregister holds
 * the entry. */
static int tsec_mcast_addr (struct eth_device *dev, u8 mcast_mac, u8 set)
{
 struct tsec_private *priv = privlist[1];
 volatile tsec_t *regs = priv->regs;
 volatile u32  *reg_array, value;
 u8 result, whichbit, whichreg;

	result = (u8)((ether_crc(MAC_ADDR_LEN,mcast_mac) >> 24) & 0xff);
	whichbit = result & 0x1f;	/* the 5 LSB = which bit to set */
	whichreg = result >> 5;		/* the 3 MSB = which reg to set it in */
	value = (1 << (31-whichbit));

	reg_array = &(regs->hash.gaddr0);

	if (set) {
		reg_array[whichreg] |= value;
	} else {
		reg_array[whichreg] &= ~value;
	}
	return 0;
}
#endif /* Multicast TFTP ? */
