/*
 * (C) Copyright 2015 - 2018 GPT, Inc.
 *
 * GPT SD Host Controller Interface
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <fdtdec.h>
#include <libfdt.h>
#include <malloc.h>
#include <sdhci.h>
#include "gpt_sdhci.h"

#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
void gpt_sdhci_voltage_init(void)
{
	int val = 0;

	val = readl(APIU_GPIO1_DIR);
	writel(val & SD_GPIO1_DIR_MASK, APIU_GPIO1_DIR);		/*set gpio1_0(signal)/gpio1_1(power) output */

	val = readl(APIU_GPIO1_DATA);
	writel((val | 0x03), APIU_GPIO1_DATA);				/*set signal/power output high */
}
#endif

void gpt_sdhci_mmc_rst(bd_t *bis)
{
	u32 stat = 0;
	
	stat = readl(GPT_EXTCR_LO_REG);
	stat |= GPT_WR_MASK | GPT_SD_EN; 
	writel(stat, GPT_EXTCR_LO_REG);	
}


int gpt_sdhci_mmc_init(bd_t *bis)
{
	struct sdhci_host *host = malloc(sizeof(struct sdhci_host));
	if (!host){
		printf("%s: buffer alloc failed!!!\n", __func__);
		return -1;
	}

	host->cfg.part_type = PART_TYPE_DOS;
	host->name = "gpt_sdhci";
	host->ioaddr = (void *)CONFIG_GPT_SDHCI_BASE;
	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);
	host->quirks = SDHCI_QUIRK_NO_CD;

	add_sdhci(host, CONFIG_GPT_SDHCI_MAX_FREQ, 0);
#if defined(CONFIG_TARGET_GP8300_EVK)||defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	gpt_sdhci_voltage_init();
#endif
	return 0;
}


/*
 * Initializes on-chip MMC controllers.
 * to override, implement board_mmc_init()
 */
int cpu_mmc_init(bd_t *bis)
{
	gpt_sdhci_mmc_rst(bis);

	return gpt_sdhci_mmc_init(bis);
}

