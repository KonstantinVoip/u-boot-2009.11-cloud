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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
#include <common.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_law.h>
#include <asm/fsl_serdes.h>
#include <asm/fsl_pci.h>

#ifdef CONFIG_PCIE1
static struct pci_controller pcie1_hose;
#endif

#ifdef CONFIG_PCIE2
static struct pci_controller pcie2_hose;
#endif

#ifdef CONFIG_PCIE3
static struct pci_controller pcie3_hose;
#endif

void mpc85xx_serdes_board_init(void)
{
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	struct fsl_pci_info pci_info[3];
	u32 devdisr, host_agent;
	int first_free_busno = 0;
	int num = 0;
	int rio = 0;

	int pcie_ep;

	devdisr = in_be32(&gur->devdisr);
	host_agent = (in_be32(&gur->porbmsr) & MPC85xx_PORBMSR_HA) >> 16;

	debug("   mpc85xx_serdes_board_init: devdisr=%x, host_agent=%x\n",
			devdisr, host_agent);

	if (!(gur->pordevsr & MPC85xx_PORDEVSR_SGMII1_DIS)) {
		if (is_serdes_configured(SGMII_TSEC1))
			puts("    eTSEC1 is in sgmii mode.\n");
	}
	if (!(gur->pordevsr & MPC85xx_PORDEVSR_SGMII2_DIS)) {
		if (is_serdes_configured(SGMII_TSEC2))
			puts("    eTSEC2 is in sgmii mode.\n");
	}
	if (!(gur->pordevsr & MPC85xx_PORDEVSR_SGMII3_DIS)) {
		if (is_serdes_configured(SGMII_TSEC3))
			puts("    eTSEC3 is in sgmii mode.\n");
	}

	puts("\n");
#ifdef CONFIG_PCIE1
	pcie_ep = is_fsl_pci_agent(LAW_TRGT_IF_PCIE_1, host_agent);

	if (is_serdes_configured(PCIE1) && !(devdisr & MPC85xx_DEVDISR_PCIE)) {
		set_next_law(CONFIG_SYS_PCIE1_MEM_PHYS,
				law_size_bits(CONFIG_SYS_PCIE1_MEM_SIZE),
				LAW_TRGT_IF_PCIE_1);
		set_next_law(CONFIG_SYS_PCIE1_IO_PHYS,
				law_size_bits(CONFIG_SYS_PCIE1_IO_SIZE),
				LAW_TRGT_IF_PCIE_1);

		SET_STD_PCIE_INFO(pci_info[num], 1);
		printf("    PCIE1 used as %s (base addr %lx)\n",
				pcie_ep ? "End Point" : "Root Complex",
				pci_info[num].regs);
		first_free_busno = fsl_pci_init_port(&pci_info[num++],
					&pcie1_hose, first_free_busno, pcie_ep);
	} else {
		setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE);
		printf("    PCIE1: disabled\n");
	}
	puts("\n");
#else
	setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE); // disable 
#endif

#ifdef CONFIG_PCIE2
	pcie_ep = is_fsl_pci_agent(LAW_TRGT_IF_PCIE_2, host_agent);

	if (is_serdes_configured(PCIE2) && !(devdisr & MPC85xx_DEVDISR_PCIE2)){
		set_next_law(CONFIG_SYS_PCIE2_MEM_PHYS,
				law_size_bits(CONFIG_SYS_PCIE2_MEM_SIZE),
				LAW_TRGT_IF_PCIE_2);
		set_next_law(CONFIG_SYS_PCIE2_IO_PHYS,
				law_size_bits(CONFIG_SYS_PCIE2_IO_SIZE),
				LAW_TRGT_IF_PCIE_2);

		SET_STD_PCIE_INFO(pci_info[num], 2);
		printf("    PCIE2 used as %s (base addr %lx)\n",
				pcie_ep ? "End Point" : "Root Complex",
				pci_info[num].regs);
		first_free_busno = fsl_pci_init_port(&pci_info[num++],
					&pcie2_hose, first_free_busno, pcie_ep);

	} else {
		setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE2);
		printf("    PCIE2: disabled\n");
	}
	puts("\n");
#else
	setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE2); // disable 
#endif

#ifdef CONFIG_PCIE3
	pcie_ep = is_fsl_pci_agent(LAW_TRGT_IF_PCIE_3, host_agent);

	if (is_serdes_configured(PCIE3) && !(devdisr & MPC85xx_DEVDISR_PCIE3)){
		set_next_law(CONFIG_SYS_PCIE3_MEM_PHYS,
				law_size_bits(CONFIG_SYS_PCIE3_MEM_SIZE),
				LAW_TRGT_IF_PCIE_3);
		set_next_law(CONFIG_SYS_PCIE3_IO_PHYS,
				law_size_bits(CONFIG_SYS_PCIE3_IO_SIZE),
				LAW_TRGT_IF_PCIE_3);

		SET_STD_PCIE_INFO(pci_info[num], 3);
		printf("    PCIE3 used as %s (base addr %lx)\n",
				pcie_ep ? "End Point" : "Root Complex",
				pci_info[num].regs);
		first_free_busno = fsl_pci_init_port(&pci_info[num++],
					&pcie3_hose, first_free_busno, pcie_ep);
	} else {
		setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE3);
		printf("    PCIE3: disabled\n");
	}
	puts("\n");
#else
	setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_PCIE3); // disable 
#endif

#ifdef CONFIG_SRIO
	if (is_serdes_configured(SRIO1) && !(devdisr & MPC85xx_DEVDISR_SRIO)){
		set_next_law(CONFIG_SYS_SRIO1_MEM_PHYS,
				law_size_bits(CONFIG_SYS_SRIO1_MEM_SIZE),
				LAW_TRGT_IF_RIO);

		rio = 1;
		printf("    SRIO1: enabled\n");
	} else {
		printf("    SRIO1: disabled\n");
	}

	if (is_serdes_configured(SRIO2) && !(devdisr & MPC85xx_DEVDISR_SRIO)){
		set_next_law(CONFIG_SYS_SRIO2_MEM_PHYS,
				law_size_bits(CONFIG_SYS_SRIO2_MEM_SIZE),
				LAW_TRGT_IF_RIO_2);

		printf("    SRIO2: enabled\n");
	} else {
		printf("    SRIO2: disabled\n");
		if (rio == 0)
			setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_SRIO);

	}
#else
	setbits_be32(&gur->devdisr, MPC85xx_DEVDISR_SRIO); // disable 
#endif
}

void ft_mpc85xx_serdes_board_setup(void *blob)
{
	volatile ccsr_gur_t *gur = (void *)(CONFIG_SYS_MPC85xx_GUTS_ADDR);
	u32 devdisr = in_be32(&gur->devdisr);

#ifdef CONFIG_PCIE1
	if (devdisr & MPC85xx_DEVDISR_PCIE)
		fdt_del_node_by_path(blob, PCIE1_ALIAS_NAME);
	else
		ft_fsl_pci_setup(blob, PCIE1_ALIAS_NAME, &pcie1_hose);
#else
	fdt_del_node_by_path(blob, PCIE1_ALIAS_NAME);
#endif
#ifdef CONFIG_PCIE2
	if (devdisr & MPC85xx_DEVDISR_PCIE2)
		fdt_del_node_by_path(blob, PCIE2_ALIAS_NAME);
	else
		ft_fsl_pci_setup(blob, PCIE2_ALIAS_NAME, &pcie2_hose);
#else
	fdt_del_node_by_path(blob, PCIE2_ALIAS_NAME);
#endif
#ifdef CONFIG_PCIE3
	if (devdisr & MPC85xx_DEVDISR_PCIE3)
		fdt_del_node_by_path(blob, PCIE3_ALIAS_NAME);
	else
		ft_fsl_pci_setup(blob, PCIE3_ALIAS_NAME, &pcie3_hose);
#else
	fdt_del_node_by_path(blob, PCIE3_ALIAS_NAME);
#endif
#ifdef CONFIG_SRIO
	if (devdisr & MPC85xx_DEVDISR_SRIO) {
		fdt_del_node_by_path(blob, SRIO1_ALIAS_NAME);
		fdt_del_node_by_path(blob, SRIO2_ALIAS_NAME);
	} else {
		if (!is_serdes_configured(SRIO1))
			fdt_del_node_by_path(blob, SRIO1_ALIAS_NAME);
		if (!is_serdes_configured(SRIO2))
			fdt_del_node_by_path(blob, SRIO2_ALIAS_NAME);
	}
#else
	fdt_del_node_by_path(blob, SRIO1_ALIAS_NAME);
	fdt_del_node_by_path(blob, SRIO2_ALIAS_NAME);
#endif
}
*/

