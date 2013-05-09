#ifndef __FSL_SERDES_H
#define __FSL_SERDES_H

#include <config.h>

#ifdef CONFIG_MPC83xx
#define FSL_SERDES_CLK_100		(0 << 28)
#define FSL_SERDES_CLK_125		(1 << 28)
#define FSL_SERDES_CLK_150		(3 << 28)
#define FSL_SERDES_PROTO_SATA		0
#define FSL_SERDES_PROTO_PEX		1
#define FSL_SERDES_PROTO_PEX_X2		2
#define FSL_SERDES_PROTO_SGMII		3
#define FSL_SERDES_VDD_1V		1

#ifdef CONFIG_FSL_SERDES
extern void fsl_setup_serdes(u32 offset, char proto, u32 rfcks, char vdd);
#else
static void fsl_setup_serdes(u32 offset, char proto, u32 rfcks, char vdd) {}
#endif /* CONFIG_FSL_SERDES */
#endif /* CONFIG_MPC83xx */

#ifdef CONFIG_MPC85xx
enum srds_prtcl {
	NONE = 0,
	PCIE1,
	PCIE2,
	PCIE3,
	SRIO1,
	SRIO2,
	SGMII_TSEC1,
	SGMII_TSEC2,
	SGMII_TSEC3,
};

#if defined(CONFIG_MPC8548)
#define PCIE1_ALIAS_NAME	"pci2"
#elif defined(CONFIG_MPC8536)
#define PCIE1_ALIAS_NAME	"pci3"
#define PCIE2_ALIAS_NAME	"pci2"
#define PCIE3_ALIAS_NAME	"pci1"
#else
#define PCIE1_ALIAS_NAME	"pci2"
#define PCIE2_ALIAS_NAME	"pci1"
#define PCIE3_ALIAS_NAME	"pci0"
#endif

#define SRIO1_ALIAS_NAME	"rio0"
#define SRIO2_ALIAS_NAME	"rio1"

extern void mpc85xx_serdes_board_init(void);
extern void ft_mpc85xx_serdes_board_setup(void *blob);

#ifdef CONFIG_FSL_SERDES
extern void fsl_serdes_init(void);
extern int is_serdes_configured(enum srds_prtcl prtcl);
#else

/* XXX: MPC8536 should eventually use CONFIG_FSL_SERDES */
#ifndef CONFIG_MPC8536
static void fsl_serdes_init(void) {}
#endif

#define is_serdes_configured(prtcl)	(1)
#endif /* CONFIG_FSL_SERDES */
#endif /* CONFIG_MPC85xx */

#endif /* __FSL_SERDES_H */
