#include <common.h>
#include <pci.h>
#include <asm/io.h>
#include <errno.h>
#include <malloc.h>

#include "asm/chip2/apiu.h"
#include "asm/chip2/sysctrl.h"
#include "pcie_gpt.h"

struct gpt_pcie *pcie;

int gpt_get_bus(struct gpt_pcie *pcie);
int gpt_get_dev(struct gpt_pcie *pcie);

static inline void pcie_phy_write(unsigned long long addr, unsigned int data)
{
        writel(data, GPT_PCIE_PHY_BASE + addr);
}

static inline unsigned int pcie_phy_read(unsigned long long addr)
{
        return readl(GPT_PCIE_PHY_BASE + addr);
}

void gpt_data_random(unsigned char * data, int len)
{
}

void gpt_pcie_dumpreg(unsigned int *regs, int length)
{
	int i;

	if (regs == NULL) {
		printf("please init norflash host first");
		return;
	}
	printf("regs: \n");
	for (i = 0; i < length; i++) {
		if (i % 4 == 0)
			printf("%.8x: ", i);
		printf("%.8x%c", regs[i], i % 4 == 3?'\n':' ');
	}
}

void gpt_pcie_dump1reg(unsigned char *addr, int count)
{
	int i;
	unsigned int data;

	for (i = 0; i < count; i++) {
		data = readl(addr);
		if (i % 4 == 0)
			printf("%.8x: ", i);
		printf("%.8x%c", data, i % 4 == 3?'\n':' ');
	}
}

static int gpt_pcie_rd_cfg(struct pci_controller *hose,
				pci_dev_t d, int where, u32 *val)
{
	struct gpt_pcie *pcie = hose->priv_data;
	u32 *addr = NULL;
	u32 addr0, desc0;

	addr0 = ((12 -1) & GENMASK(5, 0)) | ((PCI_DEV(d) << 12) & GENMASK(19, 12)) |
        	((PCI_BUS(d) << 20) & GENMASK(27, 20));

	switch (PCI_BUS(d) - hose->first_busno) {
        case 0:
                addr = pcie->dbi + (where & ~0x3);
                break;
        case 1:
       		writel(addr0, pcie->dbi + GPT_OUT_RE_PCIE_ADDR0(0));
        	desc0 = GPT_OUT_DESC_TP_CFG0;
		writel(desc0, pcie->dbi + GPT_OUT_RE_DESC_ADDR0(0));
        	addr = pcie->cfg0_va + (where & ~0x3);
                break;
        default:
		writel(addr0, pcie->dbi + GPT_OUT_RE_PCIE_ADDR0(0));
        	desc0 = GPT_OUT_DESC_TP_CFG1;
		writel(desc0, pcie->dbi + GPT_OUT_RE_DESC_ADDR0(0));
        	addr = pcie->cfg0_va + (where & ~0x3);
	}

	*val = readl(addr);
	PDEBUG("%s:%d mode:%d, addr:%p, val:%x\n", __func__,
			__LINE__, PCI_BUS(d) - hose->first_busno, addr, *val);

	return 0;
}

static int gpt_pcie_wr_cfg(struct pci_controller *hose,
				pci_dev_t d,int where, u32 val)
{
	struct gpt_pcie *pcie = hose->priv_data;
	u32 *addr = NULL;
	u32 addr0, desc0;

	addr0 = ((12 -1) & GENMASK(5, 0)) | ((PCI_DEV(d) << 12) & GENMASK(19, 12)) |
        	((PCI_BUS(d) << 20) & GENMASK(27, 20));

	switch (PCI_BUS(d) - hose->first_busno) {
        case 0:
                addr = pcie->dbi + (where & ~0x3);
                break;
        case 1:
       		writel(addr0, pcie->dbi + GPT_OUT_RE_PCIE_ADDR0(0));
        	desc0 = GPT_OUT_DESC_TP_CFG0;
		writel(desc0, pcie->dbi + GPT_OUT_RE_DESC_ADDR0(0));
        	addr = pcie->cfg0_va + (where & ~0x3);
                break;
        default:
		writel(addr0, pcie->dbi + GPT_OUT_RE_PCIE_ADDR0(0));
        	desc0 = GPT_OUT_DESC_TP_CFG1;
		writel(desc0, pcie->dbi + GPT_OUT_RE_DESC_ADDR0(0));
        	addr = pcie->cfg0_va + (where & ~0x3);
	}

	writel(val, addr);
	PDEBUG("%s:%d bus:%d, addr:%p, val:%x\n", __func__,
			__LINE__, PCI_BUS(d) - hose->first_busno, addr, val);

	return 0;
}

void gpt_pcie_state(struct gpt_pcie *pcie)
{
	unsigned int state;
	unsigned int data;
	unsigned int corr;
	unsigned int uncorr;

	printf("bus(%d) dev(%d)\n", gpt_get_bus(pcie), gpt_get_dev(pcie));

	/*local manage error state */
	state = readl(pcie->dbi + GPT_PCIE_TX_CNT);
	data = readl(pcie->dbi + GPT_PCIE_TX_DWCNT);
	corr = readl(pcie->dbi + GPT_PCIE_RX_CNT);
	uncorr = readl(pcie->dbi + GPT_PCIE_RX_DWCNT);
	printf("TX  : 0x%x, TXDW: 0x%x\n", state, data);
	printf("RX  : 0x%x, RXDW: 0x%x\n", corr, uncorr);
	state = readl(pcie->dbi + GPT_PCIE_LERR_ST);
	data = readl(pcie->dbi + GPT_PCIE_LCRC_CNT);
	corr = readl(pcie->dbi + GPT_PCIE_ECC_CNT);
	printf("local error: 0x%x, LCRC counter: 0x%x\n", state, data);
	printf("ECC errr  : 0x%x\n", corr);

	/*function error state */
	corr = readl(pcie->dbi + GPT_PCIE_CORR_ERR_OF);
	uncorr = readl(pcie->dbi + GPT_PCIE_UNCORR_ERR_OF);
	printf("corr: %x, uncorr:%x\n", corr, uncorr);
}

void gpt_pcie_debug(struct gpt_pcie *pcie, int type, int triger)
{
	unsigned int data;
	int timeout = 0x1000;
	data = readl(pcie->dbi + GPT_PCIE_DBG_MUX_CTL);
	data = GPT_DEBUG_MODE(data, type);
	writel(data , pcie->dbi + GPT_PCIE_DBG_MUX_CTL);

	/* */
	data = readl(pcie->regs + GPT_PCIE_DBG_CTL);
	writel(GPT_PCIE_DBG_CAP_VEC(data, triger),
			pcie->regs + GPT_PCIE_DBG_CTL);

	data = readl(pcie->regs + GPT_PCIE_DBG_CTL);
	writel(GPT_PCIE_DBG_CAP_INTV(data, 0),
			pcie->regs + GPT_PCIE_DBG_CTL);

	data = readl(pcie->regs + GPT_PCIE_DBG_CTL);
	writel(data  | GPT_PCIE_DBG_EN,
			pcie->regs + GPT_PCIE_DBG_CTL);
	do {
		data = readl(pcie->regs + GPT_PCIE_DBG_CTL);
	} while ((!(data & GPT_PCIE_DBG_OVER)) && timeout--);
	if (timeout <= 0) {
		printf("timeout .....\n");
	}

	gpt_pcie_dump1reg(pcie->regs + GPT_PCIE_DBG_BUF, 1024);

	data = readl(pcie->regs + GPT_PCIE_DBG_CTL);
	writel(data | GPT_PCIE_DBG_OVER,
			pcie->regs + GPT_PCIE_DBG_CTL);
}

void gpt_pcie_ltssm_st(struct gpt_pcie *pcie)
{
	int i;
	unsigned int data;

	for (i = 0; i < 32; i++) {

		data = readl(pcie->regs + GPT_PCIE_LTSSM_ST_BUF);
		if (i % 4 == 0)
			printf("%.8x: ", i);
		printf("%.8x%c", data, i % 4 == 3?'\n':' ');
		data = readl(pcie->regs + GPT_PCIE_LTSSM_ST_CTL);
		if (data & GPT_PCIE_LTSSM_ST_FULL) {
			writel(data | GPT_PCIE_LTSSM_ST_CLR, pcie->regs + GPT_PCIE_LTSSM_ST_CTL);
			data = readl(pcie->regs + GPT_PCIE_LTSSM_ST_CTL);
			printf("laflaflafa: %x\n", data);
		}
	}

}

int gpt_get_dev(struct gpt_pcie *pcie)
{
	u32 state;
	state = readl(pcie->dbi + GPT_PCIE_BUSDEV);
	return state & 0x1f;
}

int gpt_get_bus(struct gpt_pcie *pcie)
{
	u32 state;
	state = readl(pcie->dbi + GPT_PCIE_BUSDEV);
	return (state >> 8) & 0xff;
}

int gpt_pcie_reset(struct pci_controller *hose)
{
	struct gpt_pcie *pcie = hose->priv_data;
	u32 state;
	u32 timeout = 0x10000;

#if defined(CONFIG_TARGET_GP8300_EVK) || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	state = readl(pcie->regs + GPT_PCIE_CFG);
	/* set bit7 as 0 for PMA_MACRO_VAUX_EN  to close VAUX, run Gen1 only */
	writel(state & GPT_PCIE_MACRO_VAUX_EN_OFF, pcie->regs + GPT_PCIE_CFG);

	/* phy initial */
	pcie_phy_write(CMN_PSC_A3,  0x2022);
        pcie_phy_write(CMN_DIAG_PLL_ITRIM0, 0x1089);
        pcie_phy_write(CMN_DIAG_PLL_ITRIM1, 0x50f0);
	pcie_phy_write(CMN_DIAG_PLL_OVRD, 0x0360);  	/* for LVDS */

       for (int cnt = 0; cnt < 4; cnt++) {
                pcie_phy_write(RX_REE_PEAK_THR(0, cnt), 0x04fc);
                pcie_phy_write(RX_DIAG_DEF_AMP_TUNE_2(0, cnt), 0x0201);
                pcie_phy_write(RX_CDRLF_CNFG(0, cnt), 0x018c);
                pcie_phy_write(TX_DIAG_TX_BOOST(0, cnt), 0x000b);

                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 0), 0x0000);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 1), 0x0002);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 2), 0x0004);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 3), 0x0006);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 4), 0x0008);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 5), 0x000b);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 6), 0x000d);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 7), 0x0010);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 8), 0x0012);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 9), 0x0015);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 10), 0x0018);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 11), 0x001b);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 12), 0x001e);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 13), 0x0021);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 14), 0x0024);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 15), 0x0027);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 16), 0x002a);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 17), 0x002d);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 18), 0x0030);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 19), 0x0033);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 20), 0x0036);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 21), 0x003a);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 22), 0x003e);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 23), 0x0041);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 24), 0x0045);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 25), 0x0049);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 26), 0x004c);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 27), 0x0050);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 28), 0x0053);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 29), 0x0057);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 30), 0x005a);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_LOW_XX(0, cnt, 31), 0x005e);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 32), 0x0062);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 33), 0x0066);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 34), 0x006a);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 35), 0x006e);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 36), 0x0072);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 37), 0x0076);
                pcie_phy_write(TX_HYDC_EMPH_TRVAL_HIGH_XX(0, cnt, 38), 0x007a);
        }
		/* PCIE X1/2/4 */
		state = readl(pcie->regs + GPT_PCIE_CFG);
		writel(GPT_PCIE_LKX4(state), pcie->regs + GPT_PCIE_CFG);
		debug("GPT_PCIE_CFG reg set by 0x%x\n", readl(pcie->regs + GPT_PCIE_CFG));

#endif

	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_PHY_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);

	do {
		state = readl(pcie->regs + GPT_PCIE_RST_CTL);
		timeout--;
	} while (!(state & GPT_PCIE_RST_CLK_STAB_OF) && (timeout));
	if (!timeout) {
		printf("gpt pcie phy clock stable timeout .....\n");
		return -1;
	}

#if defined(CONFIG_TARGET_GP8300_EVK)  || defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG) 
	timeout = 0x10000;

	do {
		state = pcie_phy_read(PHY_PMA_PSM_STATE_LO);
		timeout--;
	}while((state != 0x0104) && (timeout));
	if (!timeout) {
		printf("gpt pcie phy l0 state timeout .....\n");
		return -1;
	}

	udelay(0x100000);  /* delay 80ms */
#endif
	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_CTL_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);

	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_RESET_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);


	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_MGMTST_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);

	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_MGMT_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);

	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_AXI_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);

	state = readl(pcie->regs + GPT_PCIE_RST_CTL) | GPT_PCIE_RST_PM_OF;
	writel(state, pcie->regs + GPT_PCIE_RST_CTL);

	state = readl(pcie->regs + GPT_PCIE_CTRL)
				| GPT_PCIE_CTL_LTSSM_EN
				| GPT_PCIE_CTL_LINK_EN;
	writel(state, pcie->regs + GPT_PCIE_CTRL);

	timeout = 0x20000;
	do {
		state = readl(pcie->regs +  GPT_PCIE_STA);
		if ((state & GPT_PCIE_LINK) == GPT_PCIE_LINK) {
			debug("reset state: %x, register: %x, timeout: 0x%x\n", state,
				readl(pcie->regs + GPT_PCIE_RST_CTL), timeout);
			break;
		}

		timeout--;
		debug("reset state: %x, timeout: %d \n", state, timeout);

	} while(timeout);
	if (!timeout) {
		printf("gpt pcie phy link timeout .....\n");
		return -1;
	}

	return 0;
}

static int gpt_pcie_rp_inbound_set(struct gpt_pcie *pcie, int bars, u64 bus_addr, int pass)
{
	u32 regs;

	if (bars > 2)
		return -1;

	regs = (bus_addr & (~0x3f)) | (pass - 1);
	writel(regs, pcie->dbi + GPT_RC_IN_AXI_ADDR0(bars));
	writel(bus_addr >> 32, pcie->dbi + GPT_RC_IN_AXI_ADDR1(bars));

	return 0;
}

static int gpt_pcie_ep_inbound_set(struct gpt_pcie *pcie, int bars, u64 bus_addr)
{
	if (bars > 6)
		return -1;

	writel(bus_addr, pcie->dbi + GPT_EP_IN_AXI_ADDR0(bars));
	writel(bus_addr >> 32, pcie->dbi + GPT_EP_IN_AXI_ADDR1(bars));

	return 0;
}

static int gpt_pcie_outbound_set(struct gpt_pcie *pcie, int func, int region,
				u64 desc, u64 phys, u64 bus_addr, u32 pass)
{
	u32 regs;
	int bus = gpt_get_bus(pcie);
	int dev = gpt_get_dev(pcie);
	u32 desc0 = desc;
	u32 desc1 = desc >> 32;

	if (pass > 64 || pass < 8) {
		printf("8 < pass  < 64\n");
		return -1;
	}

	if (region > 31) {
		printf("region must less 32\n");
		return -1;
	}

	regs = (phys & (~0x3f)) | (pass - 1);
	writel(regs, pcie->dbi + GPT_OUT_RE_PCIE_ADDR0(region));
	writel(phys >> 32, pcie->dbi + GPT_OUT_RE_PCIE_ADDR1(region));

	bus = 3;
	dev = 0;
	if (desc0 & GPT_OUT_CAPT_WITH_RID) {
		desc0 |= GPT_OUT_DEV_NUM(dev);
		desc1 |= GPT_OUT_BUS_NUM(bus);
	}

	writel(desc0, pcie->dbi + GPT_OUT_RE_DESC_ADDR0(region));
	writel(desc1, pcie->dbi + GPT_OUT_RE_DESC_ADDR1(region));

	regs = (bus_addr & (~0x3f)) | (pass - 1);
	writel(regs, pcie->dbi + GPT_OUT_RE_AXI_ADDR0(region));
	writel(bus_addr >> 32, pcie->dbi + GPT_OUT_RE_AXI_ADDR1(region));

	return 0;
}

static int gpt_pcie_dma(struct gpt_pcie *pcie, int ch, int dir, u64 bus_addr, u64 pci_addr, int len)
{
	unsigned int flags = GPT_PDMA_INBOUND;
	struct gpt_pdma_desc *desc = (struct gpt_pdma_desc *)GPT_PDMA_DESC_ADDR;
	int timeout = 0xfffff;

	int dmacfg = readl(pcie->dbi + GPT_PDMA_CONFIG);

	printf("%s  (%llx) --> (%llx), len(%d)\n",
		dir == GPT_PDMA_OTBOUND ? "outbond": "inbound", bus_addr, pci_addr, len);

	memset((void *)desc, 0x0, 0x1000);
	desc->bus_addr = bus_addr;
	desc->pci_addr = pci_addr;

	desc->len_ctl = GPT_PDMA_TYPE(GPT_PDMA_LEN(len), (GPT_PDMA_BULK | GPT_PDMA_INT));
	desc->next = NULL;
//	printf("%p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", &desc->bus_addr,
//		&desc->axi_ctrl, &desc->pci_addr, &desc->tlp_header,
//		&desc->len_ctl, &desc->axibus_st, &desc->pcibus_st,
//		&desc->ch_stat, &desc->reserved, &desc->next);

	writel((u64)desc, pcie->dbi + GPT_PDMA_ADDR0(ch));
	writel(((u64)desc >> 32), pcie->dbi + GPT_PDMA_ADDR1(ch));
//	writel(0x0, pcie->dbi + GPT_PDMA_ATTR0(ch));
//	writel(0x0, pcie->dbi + GPT_PDMA_ATTR1(ch));

	if (dir == GPT_PDMA_OTBOUND)
		flags = GPT_PDMA_OTBOUND;

	writel(0x01, pcie->dbi + GPT_PDMA_IRQEN);
	writel(0xf0, pcie->dbi + GPT_PDMA_IRQDIS);

	writel(flags | GPT_PDMA_ENABLE, pcie->dbi + GPT_PDMA_CTL(ch));

	do {
		dmacfg = readl(pcie->dbi + GPT_PDMA_IRQST);
	} while((!(dmacfg & 0x1)) && timeout-- > 0);

	writel(0x1, pcie->dbi + GPT_PDMA_IRQST);

	if (timeout <= 0)
		printf("dma timeout\n");
	else
		printf("dma finish\n");

	return 0;
}

static void gpt_pcie_intx(struct pci_controller *hose,
				int bus, int dev, int func, int enable)
{
	uint32_t data;
	pci_dev_t bdf = PCI_BDF(bus, dev, func);
	pci_hose_read_config_dword(hose, bdf, GPT_PCIE_COMMAND, &data);
	data |= (GPT_CTL_INTX_DIS | GPT_CTL_SERR_EN);
	pci_hose_write_config_dword(hose, bdf, GPT_PCIE_COMMAND, data);
	debug("pcie command ctl:0x%x\n", data);
}

static void gpt_pcie_msien(struct pci_controller *hose, int bus, int dev, int func)
{
	uint32_t data;
	u32 addr1 = (u32)GPT_RC_MSI_ADDR;
	u32 addr2 = GPT_RC_MSI_ADDR >> 32;

	pci_dev_t bdf = PCI_BDF(bus, dev, func);

	gpt_pcie_intx(hose, bus, dev, func, 0);
	pci_hose_write_config_dword(hose, bdf, GPT_PCIE_MSI_CLADDR, addr1);
	pci_hose_read_config_dword(hose, bdf, GPT_PCIE_MSI_CLADDR, &data);
	debug("pcie cap low:0x%x\n", data);
	pci_hose_write_config_dword(hose, bdf, GPT_PCIE_MSI_CHADDR, addr2);
	pci_hose_read_config_dword(hose, bdf, GPT_PCIE_MSI_CHADDR, &data);
	debug("pcie cap hi:0x%x\n", data);

	pci_hose_write_config_dword(hose, bdf, GPT_PCIE_MSI_CDATA, GPT_RC_MSI_DATA);
	pci_hose_read_config_dword(hose, bdf, GPT_PCIE_MSI_CDATA, &data);
	debug("pcie cap data:0x%x\n", data);

	pci_hose_read_config_dword(hose, bdf, GPT_PCIE_MSI_CCTL, &data);
	data |= GPT_CTL_MSI_EN;
	pci_hose_write_config_dword(hose, bdf, GPT_PCIE_MSI_CCTL, data);
	debug("pcie cap ctl:0x%x\n", data);
}

static void gpt_pcie_msi(struct gpt_pcie *pcie)
{
	u32 data;
	u64 addr0, addr1;

	addr0 = readl(pcie->dbi + GPT_PCIE_MSI_LADDR);
	addr1 = readl(pcie->dbi + GPT_PCIE_MSI_HADDR);

	addr1 = (addr0 | (addr1 << 32)) & (~0xfffff);
	addr0 = addr0 & 0xfffff;

	if (pcie->msi_addr != GPT_PCIEDEV_OUTBD_MSI_AXI) {
		gpt_pcie_outbound_set(pcie, 0, GPT_PCIEDEV_REGN_MSI,
				GPT_OUT_DESC_TP_MEM, addr1,
				GPT_PCIEDEV_OUTBD_MSI, GPT_PCIEDEV_MSI_PASS);

		pcie->msi_addr = GPT_PCIEDEV_OUTBD_MSI_AXI;
	}

	data  = readl(pcie->dbi + GPT_PCIE_MSI_DATA);
	printf("msi addr(%llx) --> addr(%llx), offset(%llx), data(%x)\n",
				pcie->msi_addr, addr1, addr0, data);
	writel(data, pcie->msi_addr + addr0);
}

void gpt_pcie_epcfg(struct pci_controller *hose)
{
	u32 data;
	u32 reg;
	u32 addr1 = (u32)GPT_RC_BAR0_ADDR;
	u32 addr2 = GPT_RC_BAR0_ADDR >> 32;
	struct gpt_pcie *pcie = hose->priv_data;

	data = readl(pcie->regs + GPT_PCIE_STA);
	if (data & GPT_PCIE_ST_SPEED5G)
		printf("pcie negotiate speed to 5G/s\n");
	else
		printf("pcie negotiate speed to 2.5G/s\n");

	/* enable rc mem/io/master */
	data = readl(pcie->dbi + GPT_PCIE_RE_CMDST);
	writel(data | 0x7, pcie->dbi + GPT_PCIE_RE_CMDST);

        reg = readl(pcie->dbi + 0x2000 + GPT_PCIE_FUNC_CLASS);
        writel(reg | (PCI_CLASS_MULTIMEDIA_VIDEO << 16), pcie->dbi + 0x2000 + GPT_PCIE_FUNC_CLASS);
        reg = readl(pcie->dbi + 0x2000 +  GPT_PCIE_FUNC_CLASS);

	gpt_pcie_outbound_set(pcie, 0, GPT_PCIERE_REGN_DATA,
			GPT_OUT_DESC_TP_MEM, GPT_PCIE_DATA_BASE,
			GPT_PCIE_DATA_BASE, 28);

	/*msi mem outbound*/
	gpt_pcie_outbound_set(pcie, 0, GPT_PCIERE_REGN_MSI,
			GPT_OUT_DESC_TP_MEM, GPT_PCIE_MSI_BASE,
			GPT_PCIE_MSI_BASE, 28);

	data = GPT_EP_BAR0_ENABLE(GPT_EP_BAR_IO32, GPT_EP_BAR_8KB)
		| GPT_EP_BAR2_ENABLE(GPT_EP_BAR_MEM64, GPT_EP_BAR_1M);
	writel(data, pcie->dbi + GPT_PCIE_BAR_CFG0);

	data = GPT_EP_BAR4_ENABLE(GPT_EP_BAR_DISABLE, GPT_EP_BAR_1M)
		| GPT_EP_BAR5_ENABLE(GPT_EP_BAR_DISABLE, GPT_EP_BAR_8M);
	writel(data, pcie->dbi + GPT_PCIE_BAR_CFG1);

	gpt_pcie_ep_inbound_set(pcie, 0, GPT_EP_BAR0_ADDR);
	gpt_pcie_ep_inbound_set(pcie, 2, GPT_EP_BAR1_ADDR);
}

static void gpt_pcie_phy_cfg(struct gpt_pcie *pcie)
{
	printf("phy: %p\n", pcie->phy);;

	writel(0x4010, pcie->phy + SALVO_COM_LOCK_CFG1);
	printf("SALVO_COM_LOCK_CFG1: %x\n", readl(pcie->phy + SALVO_COM_LOCK_CFG1));

	writel(0x810, pcie->phy + SALVO_COM_LOCK_CFG2);
	printf("SALVO_COM_LOCK_CFG2: %x\n", readl(pcie->phy + SALVO_COM_LOCK_CFG2));

	writel(0x101, pcie->phy + SALVO_COM_LOCK_CFG3);
	printf("SALVO_COM_LOCK_CFG3: %x\n", readl(pcie->phy + SALVO_COM_LOCK_CFG3));

	writel(0xa, pcie->phy + SALVO_RCV_DET_INH);
	printf("SALVO_RCV_DET_INH: %x\n", readl(pcie->phy + SALVO_RCV_DET_INH));
}

#ifdef CONFIG_GPT_IRQ_ENABLE
int gpt_pcie_interrupt(void *data)
{
	int status = 0;

	printf("pcie dma interrupt\n");
	gpt_pcie_msi(pcie);
	writel(0xffffffff, pcie->regs + GPT_PCIE_IRQ_STA);

	return status;
}

int gpt_pcie_request_irq(struct gpt_pcie *pcie)
{
	static int init = 0;

	if (init)
		return 1;

	init++;
	writel(0x0, pcie->regs + GPT_PCIE_IRQ_MASK);

        /* init mpint interrupt */
        gpt_mpic_irq_register(GPT_PCIE_IRQNO, gpt_pcie_interrupt, NULL);
        gpt_mpic_en(0, GPT_PCIE_IRQNO);

	return 0;
}
#else
int gpt_pcie_request_irq(struct gpt_pcie *pcie){ return -1;}
#endif

void gpt_pcie_vendinfo(struct gpt_pcie *pcie)
{

/*
	int version = readl(pcie->dbi + GPT_PDMA_VERID);
	int dmacfg = readl(pcie->dbi + GPT_PDMA_CONFIG);
*/

	/* class & subclass */
	writel(PCI_CLASS_MEMORY_RAM << 16, pcie->dbi + GPT_PCIE_FUNC_CLASS);
	/* SN */
	writel(GPT_PCIE_DEV_SN0, pcie->dbi + GPT_PCIE_SERIAL_NUM0);
	writel(GPT_PCIE_DEV_SN1, pcie->dbi + GPT_PCIE_SERIAL_NUM1);
	/* DMA */

/*	printf("major: %d, minor: %d channel(%d), partition(%d), partsize(%d)\n",
			GPT_PDMA_VERMAJOR(version), GPT_PDMA_VERMINOR(version),
			GPT_PDMA_CHNUM(dmacfg), GPT_PDMA_PARTNUM(dmacfg),
			GPT_PDMA_PARTSIZE(dmacfg));
*/
}

void gpt_pcie_phycfg(void)
{
	gpt_pcie_phy_cfg(pcie);
}

void gpt_pcie_dma_transfer(void)
{

	int type = GPT_PDMA_INBOUND;

	u64 pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
	u64 pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
	int data_len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
	int data_of  = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DATAOF);
	int opcode = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMACFG);

	if (gpt_pcie_request_irq(pcie) < 0) {
		printf("interrupt not supported\n");
	}

	if (opcode & GPT_PCIEDEV_DMAIN)
		type = GPT_PDMA_INBOUND;
	else
		type = GPT_PDMA_OTBOUND;

	gpt_pcie_dma(pcie, 0, type, GPT_EP_DMA_DBASE + data_of,
				pci_addr0 | (pci_addr1 << 32), data_len);
}

void gpt_pcie_rcdma_transfer(int mode)
{

	int type;
	u64 pci_addr0 = 0x05800000;
	u64 pci_addr1 = 0x0;
	int data_len = 0x100;
	int data_of  = 0x0;
	int opcode = 0x0;

	if (gpt_pcie_request_irq(pcie) < 0) {
		printf("interrupt not supported\n");
	}

	if (mode) {
		opcode |= GPT_PCIEDEV_DMAIN;
	}

	if (opcode & GPT_PCIEDEV_DMAIN)
		type = GPT_PDMA_INBOUND;
	else
		type = GPT_PDMA_OTBOUND;

	gpt_pcie_dma(pcie, 0, type, GPT_EP_DMA_DBASE + data_of,
				pci_addr0 | (pci_addr1 << 32), data_len);
}

void gpt_pcie_rand(void)
{
	int data_len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
	int data_of  = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DATAOF);

	gpt_data_random((void *)GPT_EP_DMA_DBASE + data_of, data_len);

	gpt_pcie_dma_transfer();
}

void gpt_pcie_obsetup(void)
{
	u64 pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_ADDR0);
	u64 pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_ADDR1);

	printf("addr0:%llx, addr1:%llx, full:%llx\n",
			pci_addr0, pci_addr1, pci_addr0 | (pci_addr1 << 32));

	gpt_pcie_outbound_set(pcie, 0, GPT_PCIEDEV_REGN_DATA,
			GPT_OUT_DESC_TP_MEM, pci_addr0 | (pci_addr1 << 32),
			GPT_PCIEDEV_OUTBD_DATA, 20);
}

int data_xor(unsigned char *src, unsigned char *dst, unsigned char key, int len)
{
	int i;

	if (src == NULL || dst == NULL)
		return -1;

	for(i = 0; i < len; i++)
		*dst++ = (*src++) ^ key;

	return 0;
}

int gpt_pcie_process(struct gpt_pcie *pcie)
{
	u64 ctl;
	int stat;
	int len = 0;
	unsigned int channel;
	unsigned int command;
	unsigned char *indata, *outdata;
	uint64_t pci_addr0;
	uint64_t pci_addr1;
	int xorkey;

	do {
		ctl = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_CTL);

		if (ctl & GPT_PCIEDEV_CTL_EXT)
			break;

		if (!(ctl & GPT_PCIEDEV_EN_OF))
			continue;

		/* Clear state register */
		stat = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_STATUS);
		writel(stat & (~GPT_PCIEDEV_DONE_OF),
				GPT_EP_BAR0_ADDR + GPT_PCIEDEV_STATUS);


		command = ctl & GPT_MASK_SET(1, 3);
		channel = ctl & GPT_MASK_SET(16, 30);
		switch (command) {
		case GPT_PCIEDEV_RANDOM:
			switch (channel) {
			case GPT_PCIE_XWIR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				outdata = (void *)GPT_EP_BAR1_ADDR;
				gpt_data_random(outdata, len);
				break;

			case GPT_PCIE_XWOR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				gpt_data_random(outdata, len);
				break;

			case GPT_PCIE_XWDR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				outdata = (void *)GPT_EP_DMA_DBASE;
				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_data_random(outdata, len);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			default:
				outdata = (void *)GPT_EP_BAR1_ADDR;
				printf("random: unsupport channel(%x)\n", channel);
			}

			printf("random outaddr: %p, len: 0x%x\n", outdata, len);
			break;

		case GPT_PCIEDEV_XOR:
			xorkey = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_XOR_KEY);
			switch (channel) {
			case GPT_PCIE_IWIR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				data_xor(indata, outdata, xorkey, len);
				break;

			case GPT_PCIE_IWOR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				gpt_pcie_obsetup();
				data_xor(indata, outdata, xorkey, len);
				break;

			case GPT_PCIE_IWDR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_DMA_DBASE;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				data_xor(indata, outdata, xorkey, len);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);

				break;

			case GPT_PCIE_OWIR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				indata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				data_xor(indata, outdata, xorkey, len);
				break;

			case GPT_PCIE_OWOR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				indata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				data_xor(indata, outdata, xorkey, len);
				break;

			case GPT_PCIE_OWDR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				indata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				outdata = (void *)GPT_EP_DMA_DBASE;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);

				data_xor(indata, outdata, xorkey, len);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			case GPT_PCIE_DWIR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				indata = (void *)GPT_EP_DMA_DBASE;
				outdata = (void *)GPT_EP_BAR1_ADDR;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				data_xor(indata, outdata, xorkey, len);
				break;

			case GPT_PCIE_DWOR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				indata = (void *)GPT_EP_DMA_DBASE;
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				data_xor(indata, outdata, xorkey, len);
				break;

			case GPT_PCIE_DWDR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				indata = (void *)GPT_EP_DMA_DBASE;
				outdata = (void *)GPT_EP_DMA_DBASE;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);

				data_xor(indata, outdata, xorkey, len);

				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			default:
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				printf("xor unsupport channel(%x)\n", channel);
			}

			printf("xor %p -> %p, len:0x%x\n", indata, outdata, len);
			break;

		case GPT_PCIEDEV_CPY:
			switch (channel) {
			case GPT_PCIE_IWIR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				memcpy(outdata, indata, len);
				break;

			case GPT_PCIE_IWOR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				gpt_pcie_obsetup();
				memcpy(outdata, indata, len);
				break;

			case GPT_PCIE_IWDR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_IB_LEN);
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_DMA_DBASE;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				memcpy(outdata, indata, len);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);

				break;

			case GPT_PCIE_OWIR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				indata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				memcpy(outdata, indata, len);
				break;

			case GPT_PCIE_OWOR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				indata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				memcpy(outdata, indata, len);
				break;

			case GPT_PCIE_OWDR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIE_OB_LEN);
				indata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;
				outdata = (void *)GPT_EP_DMA_DBASE;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);

				memcpy(outdata, indata, len);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			case GPT_PCIE_DWIR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				indata = (void *)GPT_EP_DMA_DBASE;
				outdata = (void *)GPT_EP_BAR1_ADDR;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				memcpy(outdata, indata, len);
				break;

			case GPT_PCIE_DWOR:
				gpt_pcie_obsetup();
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				indata = (void *)GPT_EP_DMA_DBASE;
				outdata = (void *)GPT_PCIEDEV_OUTBD_DATA_AXI;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				memcpy(outdata, indata, len);
				break;

			case GPT_PCIE_DWDR:
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				indata = (void *)GPT_EP_DMA_DBASE;
				outdata = (void *)GPT_EP_DMA_DBASE;

				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);

				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			default:
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				printf("copy unsupport channel(%x)\n", channel);
			}

			printf("copy %p -> %p, len:0x%x\n", indata, outdata, len);
			break;

		case GPT_PCIEDEV_SPEC:
			switch (channel) {
			case GPT_PCIE_DWXR:
				indata = (void *)GPT_EP_BAR1_ADDR; //no use
				outdata = (void *)GPT_EP_BAR1_ADDR; // no use

				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_INBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			case GPT_PCIE_XWDR:
				indata = (void *)GPT_EP_BAR1_ADDR; //no use
				outdata = (void *)GPT_EP_BAR1_ADDR; //no use
				len = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMALEN);
				pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
				pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);
				gpt_pcie_dma(pcie, 0, GPT_PDMA_OTBOUND, GPT_EP_DMA_DBASE,
						pci_addr0 | (pci_addr1 << 32), len);
				break;

			default:
				indata = (void *)GPT_EP_BAR1_ADDR;
				outdata = (void *)GPT_EP_BAR1_ADDR;
				printf("spec unsupport channel(%x)\n", channel);
			}
			break;

		default:
			printf("unsupport function\n");
		}

		memset((void *)GPT_EP_BAR0_ADDR, 0x0, 0x100);
		/* Update state register */
		writel(stat | GPT_PCIEDEV_DONE_OF,
				GPT_EP_BAR0_ADDR + GPT_PCIEDEV_STATUS);
		writel(ctl & (~GPT_PCIEDEV_EN_OF),
				GPT_EP_BAR0_ADDR + GPT_PCIEDEV_CTL);

	#ifndef CONFIG_GPT_IRQ_ENABLE
		gpt_pcie_msi(pcie);
	#endif

	} while (1);

	return 0;
}

void gpt_process_t(void)
{
	gpt_pcie_process(pcie);
}

void gpt_pcie_data_t(int type)
{
	switch (type) {
	case 0:
		gpt_data_random((void *)GPT_EP_DMA_DBASE, 0x100000);
		break;
	case 1:
		gpt_data_random((void *)0x100100000, 0x100000);
		break;
	case 2:
		gpt_data_random((void *)GPT_EP_BAR1_ADDR, 0x100000);
		break;
	case 3:
		gpt_data_random((void *)GPT_EP_BAR0_ADDR, 0x100000);
		break;
	default:
		printf("Unsupported data format\n");
	}
}

void gpt_state_t(void)
{
	gpt_pcie_state(pcie);
}

void gpt_debug_t(int type, int triger)
{
	gpt_pcie_debug(pcie, type, triger);
}

void gpt_msien_t(void)
{
	gpt_pcie_msien(&pcie->hose, 1, 0, 0);
}
void gpt_msi_t(void)
{
	gpt_pcie_msi(pcie);
}

void gpt_rcdma_t(int mode)
{
	gpt_pcie_rcdma_transfer(mode);
}

void gpt_irqen_t(void)
{
	if (gpt_pcie_request_irq(pcie) < 0) {
		printf("interrupt not supported\n");
	}
}

void gpt_dma_t(int mode)
{

	int type;
	u64 pci_addr0 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR0);
	u64 pci_addr1 = readl(GPT_EP_BAR0_ADDR + GPT_PCIEDEV_DMAADDR1);

	if (gpt_pcie_request_irq(pcie) < 0) {
		printf("interrupt not supported\n");
	}
	switch (mode) {
	case 0:
		type = GPT_PDMA_INBOUND;
		break;
	case 1:
		type = GPT_PDMA_OTBOUND;
		break;
	default:
		type = GPT_PDMA_OTBOUND;
	}

	gpt_pcie_dma(pcie, 0, type, GPT_EP_DMA_DBASE,
				pci_addr0 | (pci_addr1 << 32), 0x100);
}

void gpt_outbd_t(void)
{
	gpt_pcie_obsetup();
}

unsigned int gpt_rdcfg_t(int offset)
{
	unsigned int data;
	pci_dev_t pdev = PCI_BDF(1, 0, 0);
	pci_hose_read_config_dword(&pcie->hose, pdev, offset, &data);
	return data;
}

void gpt_pcie_apb_reset(void)
{
	u32 val;

	val = readl(SYSCTRL_EXTCR_LO);
	writel(val | GPT_PCIE_EN, SYSCTRL_EXTCR_LO);
}


#if defined(CONFIG_TARGET_GP8300_FPGA)
#define PCIE_GPIO1_DIR_MASK	0xFFFFFFCF

void gpt_pcie_release_dev(void)
{
	ulong val = 0;

	val = readl(APIU_GPIO1_DIR);
	writel(val & PCIE_GPIO1_DIR_MASK, APIU_GPIO1_DIR);	/*set gpio4/5 output*/

	val = readl(APIU_GPIO1_DATA);
	writel(val | 0x30, APIU_GPIO1_DATA);			/*set gpio4/5 high*/
}

#elif defined(CONFIG_TARGET_GP8300_CHAOKONG)
#define PCIE_GPIO1_DIR_MASK      0xFFFFFF27
#define PCIE_GPIO1_4G_PWRKEY    (0x01 << 3) /*4G powerkey*/
#define PCIE_GPIO1_4G_RESET     (0x01 << 4) /*4G reset*/
#define PCIE_GPIO1_PERSTB_UPD   (0x01 << 6)
#define PCIE_GPIO1_PERSTB_ISO   (0x01 << 7) /*rtl iosolate pin for upd power_on_reset*/

void gpt_pcie_release_dev(void)
{
	ulong val = 0;

	val = readl(APIU_GPIO1_DIR);
	writel(val & PCIE_GPIO1_DIR_MASK, APIU_GPIO1_DIR);		/*set gpio4 5 6 output*/

	udelay(1000);   //1ms
	val = readl(APIU_GPIO1_DATA);
	writel((val & (~PCIE_GPIO1_PERSTB_UPD)), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val & (~PCIE_GPIO1_4G_PWRKEY)), APIU_GPIO1_DATA);
	udelay(110000); //110ms 

	val = readl(APIU_GPIO1_DATA);
	writel((val & (~ PCIE_GPIO1_PERSTB_ISO )), APIU_GPIO1_DATA);

	udelay(110000); //105ms(delay) + 5ms(porst)

	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_4G_PWRKEY), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_4G_RESET), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_PERSTB_ISO), APIU_GPIO1_DATA);
	udelay(100000); //100ms(delay)
	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_PERSTB_UPD), APIU_GPIO1_DATA);
}
#elif defined(CONFIG_TARGET_GP8300_DVB)
#define PCIE_GPIO1_DIR_MASK      0xFFFFFF0F
#define PCIE_GPIO1_PERSTB_RTL   (0x01 << 4) /*upd poweron reset*/
#define PCIE_GPIO1_PERSTB_PEX   (0x01 << 5)
#define PCIE_GPIO1_PERSTB_UPD   (0x01 << 6)
#define PCIE_GPIO1_PERSTB_ISO   (0x01 << 7) /*rtl iosolate pin for upd power_on_reset*/

void gpt_pcie_release_dev(void)
{
	ulong val = 0;

	val = readl(APIU_GPIO1_DIR);
	writel(val & PCIE_GPIO1_DIR_MASK, APIU_GPIO1_DIR);		/*set gpio4 5 6 output*/

	udelay(1000);   //1ms
	val = readl(APIU_GPIO1_DATA);
	writel((val & (~PCIE_GPIO1_PERSTB_PEX)), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val & (~PCIE_GPIO1_PERSTB_UPD)), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val & (~ PCIE_GPIO1_PERSTB_RTL )), APIU_GPIO1_DATA);
	udelay(110000); //110ms 

	val = readl(APIU_GPIO1_DATA);
	writel((val & (~ PCIE_GPIO1_PERSTB_ISO )), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_PERSTB_RTL ), APIU_GPIO1_DATA);

	udelay(110000); //105ms(delay) + 5ms(porst)
	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_PERSTB_ISO), APIU_GPIO1_DATA);
	udelay(100000); //100ms
	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_PERSTB_UPD), APIU_GPIO1_DATA);

	val = readl(APIU_GPIO1_DATA);
	writel((val | PCIE_GPIO1_PERSTB_PEX), APIU_GPIO1_DATA);

}

#elif defined(CONFIG_TARGET_GP8300_EVK)
#define PCIE_GPIO1_DIR_MASK	0xFFFFFFF3
#define PCIE_GPIO1_ISOLATEB_OFF	(0x01 << 3)
#define PCIE_GPIO1_PERSTB_ON	(0x01 << 2)
#define PCIE_GPIO1_PERSTB_OFF	~(0x01 << 2)

void gpt_pcie_release_dev(void)
{
	ulong val = 0;

	udelay(0x80000);  								/*260ms for FPGA power on*/	
	val = readl(APIU_GPIO1_DIR);
	writel(val & PCIE_GPIO1_DIR_MASK, APIU_GPIO1_DIR);				/*set gpio2/3 output*/

	val = readl(APIU_GPIO1_DATA);
	writel(((val | PCIE_GPIO1_ISOLATEB_OFF) & PCIE_GPIO1_PERSTB_OFF), APIU_GPIO1_DATA);
	udelay(0x80000);  								/*260ms*/
	writel((val | PCIE_GPIO1_ISOLATEB_OFF | PCIE_GPIO1_PERSTB_ON), APIU_GPIO1_DATA);
}
#endif

void gpt_pcie_setnx(struct gpt_pcie *pcie, int xn)
{
	unsigned int state;

	state = readl(pcie->regs + GPT_PCIE_CFG);
	writel(state | GPT_PCIE_RCMD, pcie->regs + GPT_PCIE_CFG);
}

void gpt_pcie_epmode(struct gpt_pcie *pcie)
{
	unsigned int state;
	state = readl(pcie->regs + GPT_PCIE_CFG);
	state &= (~GPT_PCIE_RCMD),
	writel(state, pcie->regs + GPT_PCIE_CFG);
}

int pci_skip_dev(struct pci_controller *hose, pci_dev_t dev)
{
	/* Do not skip controller */
	return 0;
}

int gpt_pcie_init(void)
{
	pcie = malloc(sizeof(struct gpt_pcie));
	if (!pcie)
		return -1;

	memset(pcie, 0, sizeof(struct gpt_pcie));

	pcie->hose.priv_data = pcie;
	pcie->hose.first_busno = 0;
	pcie->idx = 0;
	pcie->regs = map_physmem(GPT_PCIE_EXT_BASE, GPT_PCIE_EXT_SIZE, MAP_NOCACHE);
	pcie->dbi = map_physmem(GPT_PCIE_BASE, GPT_PCIE_SIZE, MAP_NOCACHE);
	pcie->phy = map_physmem(GPT_PCIE_PHY_BASE, GPT_PCIE_PHY_SIZE, MAP_NOCACHE);
	pcie->cfg0_va = map_physmem(GPT_PCIE_OTBND_BASE + GPT_PCIE_CFG0_BASE, GPT_PCIE_CFG0_SIZE, MAP_NOCACHE);
	pcie->cfg1_va = map_physmem(GPT_PCIE_OTBND_BASE + GPT_PCIE_CFG1_BASE, GPT_PCIE_CFG1_SIZE, MAP_NOCACHE);

	/* outbound memory */
	pci_set_region(&(pcie->hose.regions[2]),
			(pci_size_t)GPT_PCIE_DATA_BASE,
			(phys_size_t)GPT_PCIE_OTBND_BASE + GPT_PCIE_DATA_BASE,
			(pci_size_t)GPT_PCIE_DATA_SIZE,
			PCI_REGION_MEM);

	pci_set_region(&(pcie->hose.regions[3]),
			(pci_size_t)GPT_PCIE_MSI_BASE,
			(phys_size_t)GPT_PCIE_OTBND_BASE + GPT_PCIE_MSI_BASE,
			(pci_size_t)GPT_PCIE_MSI_SIZE,
			PCI_REGION_MEM);

	pcie->hose.region_count = 4;

	pci_set_ops(&pcie->hose,
		    pci_hose_read_config_byte_via_dword,
		    pci_hose_read_config_word_via_dword,
		    gpt_pcie_rd_cfg,
		    pci_hose_write_config_byte_via_dword,
		    pci_hose_write_config_word_via_dword,
		    gpt_pcie_wr_cfg);

	gpt_pcie_epmode(pcie);

	gpt_pcie_release_dev();
	if (gpt_pcie_reset(&pcie->hose)) {
		printf("gpt pcie reset failed!!\n");
		return -1;
	}

	//memset((void *)GPT_EP_BAR0_ADDR, 0x0, 0x1000);
	//memset((void *)GPT_EP_BAR1_ADDR, 0x0, 0x1000);

	/* do something after reset */

	gpt_pcie_epcfg(&pcie->hose);
	//gpt_pcie_msien(&pcie->hose, 0, 0, 0);
	//
	//gpt_pcie_vendinfo(pcie);

	return 0;
}

void pci_init_board(void)
{
	gpt_pcie_apb_reset();
	gpt_pcie_init();
}
