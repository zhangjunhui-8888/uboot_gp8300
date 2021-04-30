#ifndef __GPT_SDHCI__
#define __GPT_SDHCI__
#include <common.h>
#include <asm/io.h>
#include "asm/chip2/apiu.h"

/*extcr_lo_reg*/
#define GPT_EXTCR_LO_REG 	0xf0000100
#define	GPT_SD_EN		(1UL << 10)
#define GPT_WR_MASK 		(1UL << 26)
 

/*core_ctrl_reg*/
#define	CORE_CTRL_REG		0x1000
#define CLK_0_EN		(0x01 << 8)
#define CLK_1_EN		(0x02 << 8)

#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
#define SD_GPIO1_DIR_MASK	0xFFFFFFFC

#define SD_VDD_33_ON		0x01
#define SD_VDD_18_ON		0x00
#endif

#endif
