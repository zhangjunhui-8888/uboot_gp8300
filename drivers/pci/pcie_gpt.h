#ifndef __PCIE_GPT_H__
#define __PCIE_GPT_H__

#define GPT_PCIE_PHY_BASE			0x0f0400000
#define GPT_PCIE_PHY_SIZE			0x000040000
#define GPT_PCIE_BASE				0x0f0440000
#define GPT_PCIE_SIZE				0x000010000
#define GPT_PCIE_EXT_BASE			0x0f0448000
#define GPT_PCIE_EXT_SIZE			0x000001000
#define GPT_PCIE_OTBND_BASE			0x100000000

#define GPT_PCIE_CFG0_BASE			0x00000000
#define GPT_PCIE_CFG0_SIZE			0x10000000
#define GPT_PCIE_CFG1_BASE			0x10000000
#define GPT_PCIE_CFG1_SIZE			0x10000000
#define GPT_PCIE_IO_BASE			0x20000000
#define GPT_PCIE_IO_SIZE			0x10000000
#define GPT_PCIE_DATA_BASE			0x30000000
#define GPT_PCIE_DATA_SIZE			0x10000000
#define GPT_PCIE_MSI_BASE			0x40000000
#define GPT_PCIE_MSI_SIZE			0x00100000
#define GPT_PCIE_RC_SYS_SIZE			0x100000000

#define GPT_PCIE_IO_OFFSET			(0x200000)
#define GPT_PCIE_RC_IOPHY_BASE			(GPT_PCIE_OTBND_BASE + GPT_PCIE_IO_OFFSET)
#define GPT_PCIE_RC_SYSMEM			(0x200000000UL)

#define GPT_PCIE_EPRC_OFFSET			0x0000
#define GPT_PCIE_MNG_OFFSET			0x1000
#define GPT_PCIE_RCW_OFFSET			0x2000
#define GPT_PCIE_AXI_OFFSET			0x4000
#define GPT_PCIE_DMA_OFFSET			0x6000
#define GPT_PCIE_EXT_REG			0x8000

/* register declare */
#define GPT_PCIE_CFG				(0x000)
#define GPT_PCIE_CTRL				(0x004)
#define GPT_PCIE_LPM_CTRL			(0x008)
#define GPT_PCIE_IRQ_STA			(0x00c)
#define GPT_PCIE_SDBND_STA0			(0x010)
#define GPT_PCIE_SDBND_STA1			(0x014)
#define GPT_PCIE_MSI_MSK_STA			(0x018)
#define GPT_PCIE_MSI_PEND_STA			(0x01c)
#define GPT_PCIE_STA				(0x020)
#define GPT_PCIE_IRQ_MASK			(0x024)
#define GPT_PCIE_RST_CTL			(0x028)
#define GPT_PCIE_DBG_CTL			(0x050)
#define GPT_PCIE_DBG_BUF			(0x054)
#define GPT_PCIE_DBG_BUFADDR			(0x058)
#define GPT_PCIE_LTSSM_ST_CTL			(0x080)
#define GPT_PCIE_LTSSM_ST_BUF			(0x084)
#define GPT_PCIE_MSI_EHADDR			(0x090)
#define GPT_PCIE_MSI_ELADDR			(0x094)
#define GPT_PCIE_MSI_EDATA			(0x098)
#define GPT_PCIE_MSI_IRQ_EN			(0x200)
#define GPT_PCIE_MSI_STA			(0x204)
#define GPT_PCIE_MSIX_IRQ_EN			(0x208)
#define GPT_PCIE_MSIX_STA			(0x20c)

/*ctl offset*/
#define GPT_PCIE_CTL_LINK_EN			(0x1 << 0)
#define GPT_PCIE_CTL_LTSSM_EN			(0x1 << 1)
#define GPT_PCIE_LINK				(0x3 << 7)
#define GPT_PCIE_LTSSM_ST_CLR			(0x1 << 30)
#define GPT_PCIE_LTSSM_ST_FULL			(0x1 << 31)
/* config offset */
#define GPT_PCIE_RCMD				(0x1 << 0)
#define GPT_PCIE_GEN2				(0x1 << 1)
#define GPT_PCIE_MACRO_VAUX_EN_OFF		(~(0x1 << 7))
#define GPT_PCIE_LKX1(num)			(((num) & (~(0x3 << 3))) | (0x00 << 3))
#define GPT_PCIE_LKX2(num)			(((num) & (~(0x3 << 3))) | (0x01 << 3))
#define GPT_PCIE_LKX4(num)			(((num) & (~(0x3 << 3))) | (0x02 << 3))
/* reset offset */
#define GPT_PCIE_RST_PHY_OF			(0x1 << 0)
#define GPT_PCIE_RST_CTL_OF			(0x1 << 1)
#define GPT_PCIE_RST_RESET_OF			(0x1 << 2)
#define GPT_PCIE_RST_MGMT_OF			(0x1 << 3)
#define GPT_PCIE_RST_MGMTST_OF			(0x1 << 4)
#define GPT_PCIE_RST_AXI_OF			(0x1 << 5)
#define GPT_PCIE_RST_PM_OF			(0x1 << 6)
#define GPT_PCIE_RST_CLK_STAB_OF		(0x1 << 31)
/* dbg ctrl offset */
#define GPT_PCIE_DBG_CAP_VEC(x, d)		(((x) & (~(0xffff << 0))) | (d))
#define GPT_PCIE_DBG_CAP_INTV(x, d)		(((x) & (~(0xfff << 16))) | (d))
#define GPT_PCIE_DBG_EN				(0x1 << 28)
#define GPT_PCIE_DBG_OVER			(0x1 << 29)
#define GPT_PCIE_PFM_EN				(0x1 << 30)
/* pcie state offset*/
#define GPT_PCIE_ST_SPEED5G			(0x1 << 21)

/* cadence pcie regster defination */
/* */
#define GPT_PCIE_RE_CMDST			(GPT_PCIE_EPRC_OFFSET + 0x004)

/* function register */

#define GPT_PCIE_FUNC_CLASS			(GPT_PCIE_EPRC_OFFSET + 0x008)
#define GPT_PCIE_BAR0				(GPT_PCIE_EPRC_OFFSET + 0x010)
#define GPT_PCIE_BAR1				(GPT_PCIE_EPRC_OFFSET + 0x014)
#define GPT_PCIE_MSI_CTL			(GPT_PCIE_EPRC_OFFSET + 0x090)
#define GPT_PCIE_MSI_LADDR			(GPT_PCIE_EPRC_OFFSET + 0x094)
#define GPT_PCIE_MSI_HADDR			(GPT_PCIE_EPRC_OFFSET + 0x098)
#define GPT_PCIE_MSI_DATA			(GPT_PCIE_EPRC_OFFSET + 0x09C)
#define GPT_PCIE_MSI_MASK			(GPT_PCIE_EPRC_OFFSET + 0x0a0)
#define GPT_PCIE_MSI_PEN			(GPT_PCIE_EPRC_OFFSET + 0x0a4)
#define GPT_PCIE_UNCORR_ERR_OF			(GPT_PCIE_EPRC_OFFSET + 0x104)
#define GPT_PCIE_CORR_ERR_OF			(GPT_PCIE_EPRC_OFFSET + 0x110)
#define GPT_PCIE_SERIAL_NUM0			(GPT_PCIE_EPRC_OFFSET + 0x154)
#define GPT_PCIE_SERIAL_NUM1			(GPT_PCIE_EPRC_OFFSET + 0x158)
/* local manager regsiter */
#define GPT_PCIE_TX_CNT				(GPT_PCIE_MNG_OFFSET + 0x028)
#define GPT_PCIE_TX_DWCNT			(GPT_PCIE_MNG_OFFSET + 0x02c)
#define GPT_PCIE_RX_CNT				(GPT_PCIE_MNG_OFFSET + 0x030)
#define GPT_PCIE_RX_DWCNT			(GPT_PCIE_MNG_OFFSET + 0x034)
#define GPT_PCIE_SRIS_CTL			(GPT_PCIE_MNG_OFFSET + 0x074)
#define GPT_PCIE_DBG_MUX_CTL			(GPT_PCIE_MNG_OFFSET + 0x208)
#define GPT_PCIE_LERR_ST			(GPT_PCIE_MNG_OFFSET + 0x20c)
#define GPT_PCIE_LCRC_CNT			(GPT_PCIE_MNG_OFFSET + 0x214)
#define GPT_PCIE_ECC_CNT			(GPT_PCIE_MNG_OFFSET + 0x218)
#define GPT_PCIE_BUSDEV				(GPT_PCIE_MNG_OFFSET + 0x22c)
#define GPT_PCIE_BAR_CFG0			(GPT_PCIE_MNG_OFFSET + 0x240)
#define GPT_PCIE_BAR_CFG1			(GPT_PCIE_MNG_OFFSET + 0x244)
#define GPT_PCIE_RC_BAR_CFG			(GPT_PCIE_MNG_OFFSET + 0x300)
/* axi register*/
#define GPT_OUT_RE_PCIE_ADDR0(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x00)
#define GPT_OUT_RE_PCIE_ADDR1(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x04)
#define GPT_OUT_RE_DESC_ADDR0(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x08)
#define GPT_OUT_RE_DESC_ADDR1(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x0c)
#define GPT_OUT_RE_DESC_ADDR2(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x10)
#define GPT_OUT_RE_DESC_ADDR3(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x14)
#define GPT_OUT_RE_AXI_ADDR0(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x18)
#define GPT_OUT_RE_AXI_ADDR1(region)		(GPT_PCIE_AXI_OFFSET + (0x20 * region) + 0x1c)
#define GPT_RC_IN_AXI_ADDR0(bar)		(GPT_PCIE_AXI_OFFSET + 0x800 + (0x08 * bar) + 0x0)
#define GPT_RC_IN_AXI_ADDR1(bar)		(GPT_PCIE_AXI_OFFSET + 0x800 + (0x08 * bar) + 0x4)
#define GPT_EP_IN_AXI_ADDR0(bar)		(GPT_PCIE_AXI_OFFSET + 0x840 + (0x08 * bar) + 0x0)
#define GPT_EP_IN_AXI_ADDR1(bar)		(GPT_PCIE_AXI_OFFSET + 0x840 + (0x08 * bar) + 0x4)
/* dma register */
#define GPT_PDMA_CTL(ch)			(GPT_PCIE_DMA_OFFSET + 0x14 * (ch) + 0x00)
#define GPT_PDMA_ADDR0(ch)			(GPT_PCIE_DMA_OFFSET + 0x14 * (ch) + 0x04)
#define GPT_PDMA_ADDR1(ch)			(GPT_PCIE_DMA_OFFSET + 0x14 * (ch) + 0x08)
#define GPT_PDMA_ATTR0(ch)			(GPT_PCIE_DMA_OFFSET + 0x14 * (ch) + 0x0c)
#define GPT_PDMA_ATTR1(ch)			(GPT_PCIE_DMA_OFFSET + 0x14 * (ch) + 0x10)
#define GPT_PDMA_IRQST				(GPT_PCIE_DMA_OFFSET + 0xa0)
#define GPT_PDMA_IRQEN				(GPT_PCIE_DMA_OFFSET + 0xa4)
#define GPT_PDMA_IRQDIS				(GPT_PCIE_DMA_OFFSET + 0xa8)
#define GPT_PDMA_IN_UNC_ECC			(GPT_PCIE_DMA_OFFSET + 0xac)
#define GPT_PDMA_IN_C_ECC			(GPT_PCIE_DMA_OFFSET + 0xb0)
#define GPT_PDMA_OUT_UNC_ECC			(GPT_PCIE_DMA_OFFSET + 0xb4)
#define GPT_PDMA_OUT_C_ECC			(GPT_PCIE_DMA_OFFSET + 0xb8)
#define GPT_PDMA_VERID				(GPT_PCIE_DMA_OFFSET + 0xf8)
#define GPT_PDMA_CONFIG				(GPT_PCIE_DMA_OFFSET + 0xfc)

/* type defination  of debug */
#define GPT_DEBUG_PHY_LTSSM0			0x00
#define GPT_DEBUG_PHY_LTSSM1			0x01
#define GPT_DEBUG_PHY_LTSSM2			0x04
#define GPT_DEBUG_PHY_RX			0x05
#define GPT_DEBUG_PHY_TX			0x06
#define GPT_DEBUG_DATALINK_TX			0x08
#define GPT_DEBUG_DATALINK_RX			0x09
#define GPT_DEBUG_DATALINK_RTX			0x0a
#define GPT_DEBUG_TRANSACT_TX0			0x10
#define GPT_DEBUG_TRANSACT_RX0			0x11
#define GPT_DEBUG_TRANSACT_TX1			0x13
#define GPT_DEBUG_TRANSACT_RX1			0x12
#define GPT_DEBUG_MODE(x,d)			(((x) & ~(0x3f)) | (d))
/* end point offset */
#define GPT_CTL_SERR_EN				(0x1 << 8)
#define GPT_CTL_INTX_DIS			(0x1 << 10)
#define GPT_CTL_MSI_EN				(0x1 << 16)
#define GPT_CTL_MM_EN(x, y)			(((x) & (~(7 << 20))) | y)
#define GPT_MSI_MASK				(0x1 << 0)
#define GPT_MSI_PEND				(0x1 << 0)

/* local manage offset */
#define GPT_RC_BAR_4B				(0x00)
#define GPT_RC_BAR_8B				(0x01)
#define GPT_RC_BAR_16B				(0x02)
#define GPT_RC_BAR_32B				(0x03)
#define GPT_RC_BAR_64B				(0x04)
#define GPT_RC_BAR_128B				(0x05)
#define GPT_RC_BAR_256B				(0x06)
#define GPT_RC_BAR_512B				(0x07)
#define GPT_RC_BAR_1KB				(0x08)
#define GPT_RC_BAR_2KB				(0x09)
#define GPT_RC_BAR_4KB				(0x0a)
#define GPT_RC_BAR_8KB				(0x0b)
#define GPT_RC_BAR_16KB				(0x0c)
#define GPT_RC_BAR_32KB				(0x0d)
#define GPT_RC_BAR_64KB				(0x0e)
#define GPT_RC_BAR_128K				(0x0f)
#define GPT_RC_BAR_256K				(0x10)
#define GPT_RC_BAR_512K				(0x11)
#define GPT_RC_BAR_1M				(0x12)
#define GPT_RC_BAR_2M				(0x13)
#define GPT_RC_BAR_4M				(0x14)
#define GPT_RC_BAR_8M				(0x15)
#define GPT_RC_BAR_16M				(0x16)
#define GPT_RC_BAR_32M				(0x17)
#define GPT_RC_BAR_64M				(0x18)
#define GPT_RC_BAR_128M				(0x19)
#define GPT_RC_BAR_256M				(0x1a)
#define GPT_RC_BAR_512M				(0x1b)
#define GPT_RC_BAR_1G				(0x1c)
#define GPT_RC_BAR_2G				(0x1d)
#define GPT_RC_BAR_4G				(0x1e)
#define GPT_RC_BAR_8G				(0x1f)
#define GPT_RC_BAR_16G				(0x20)
#define GPT_RC_BAR_32G				(0x21)
#define GPT_RC_BAR_64G				(0x22)
#define GPT_RC_BAR_128G				(0x23)
#define GPT_RC_BAR_256G				(0x24)
#define GPT_EP_BAR_128B				(0x00)
#define GPT_EP_BAR_256B				(0x00)
#define GPT_EP_BAR_512B				(0x02)
#define GPT_EP_BAR_1KB				(0x03)
#define GPT_EP_BAR_2KB				(0x04)
#define GPT_EP_BAR_4KB				(0x05)
#define GPT_EP_BAR_8KB				(0x06)
#define GPT_EP_BAR_16KB				(0x07)
#define GPT_EP_BAR_32KB				(0x08)
#define GPT_EP_BAR_64KB				(0x09)
#define GPT_EP_BAR_128K				(0x0a)
#define GPT_EP_BAR_256K				(0x0b)
#define GPT_EP_BAR_512K				(0x0c)
#define GPT_EP_BAR_1M				(0x0d)
#define GPT_EP_BAR_2M				(0x0e)
#define GPT_EP_BAR_4M				(0x0f)
#define GPT_EP_BAR_8M				(0x10)
#define GPT_EP_BAR_16M				(0x11)
#define GPT_EP_BAR_32M				(0x12)
#define GPT_EP_BAR_64M				(0x13)
#define GPT_EP_BAR_128M				(0x14)
#define GPT_EP_BAR_256M				(0x15)
#define GPT_EP_BAR_512M				(0x16)
#define GPT_EP_BAR_1G				(0x17)
#define GPT_EP_BAR_2G				(0x18)
#define GPT_EP_BAR_4G				(0x19)
#define GPT_EP_BAR_8G				(0x1a)
#define GPT_EP_BAR_16G				(0x1b)
#define GPT_EP_BAR_32G				(0x1c)
#define GPT_EP_BAR_64G				(0x1d)
#define GPT_EP_BAR_128G				(0x1e)
#define GPT_EP_BAR_256G				(0x1f)
#define GPT_EP_BAR_DISABLE			(0x0)
#define GPT_EP_BAR_IO32				(0x1)
#define GPT_EP_BAR_MEM32			(0x4)
#define GPT_EP_BAR_PREF_MEM32			(0x5)
#define GPT_EP_BAR_MEM64			(0x6)
#define GPT_EP_BAR_PREF_MEM64			(0x7)

#define GPT_EP_BAR0_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 0)
#define GPT_EP_BAR1_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 8)
#define GPT_EP_BAR2_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 16)
#define GPT_EP_BAR3_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 24)
#define GPT_EP_BAR4_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 0)
#define GPT_EP_BAR5_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 8)

#define GPT_RC_BAR_DISABLE			(0x0)
#define GPT_RC_BAR_IO32				(0x1)
#define GPT_RC_BAR_MEM32			(0x4)
#define GPT_RC_BAR_PREF_MEM32			(0x5)
#define GPT_RC_BAR_MEM64			(0x6)
#define GPT_RC_BAR_PREF_MEM64			(0x7)

#define GPT_RC_BAR0_ENABLE(type, size)		((((size) & 0x3f) | ((type & 0x7) << 6)) << 0)
#define GPT_RC_BAR1_ENABLE(type, size)		((((size) & 0x1f) | ((type & 0x7) << 5)) << 9)
#define GPT_RC_CHECK_EN				(0x1 << 31)
/* axi offset */
#define GPT_OUT_DESC_TP_MEM			(0x02)
#define GPT_OUT_DESC_TP_IO			(0x06)
#define GPT_OUT_DESC_TP_CFG0			(0x0a)
#define GPT_OUT_DESC_TP_CFG1			(0x0b)
#define GPT_OUT_DESC_TP_MSG			(0x0c)
#define GPT_OUT_DESC_TP_MSGV			(0x0d)
#define GPT_OUT_CAPT_WITH_RID			(0x1 << 23)
#define GPT_OUT_DEV_NUM(n)			(((n) & 0x1f) << 27)
#define GPT_OUT_BUS_NUM(n)			(((n) & 0xff) << 0)
/* dma offset */
#define GPT_PDMA_ENABLE				(0x1 << 0)
#define GPT_PDMA_OTBOUND			(0x1 << 1)
#define GPT_PDMA_INBOUND			(0x0 << 1)
#define GPT_PDMA_BULK				(0x0 << 25)
#define GPT_PDMA_SG				(0x1 << 25)
#define GPT_PDMA_PRE				(0x2 << 25)
#define GPT_PDMA_LEN(len)			((len) & 0xffffff)
#define GPT_PDMA_TYPE(data, type)		(((data) & (~(0x3 << 25))) | (type))
#define GPT_PDMA_CONTINUE			(0x1 << 29)
#define GPT_PDMA_INT				(0x1 << 24)
#define GPT_PDMA_ARBAR(n)			((0x2 | ((n) & 0x3)) << 22)
#define GPT_PDMA_ARDOMAIN(n)			(((n) & 0x3) << 20)
#define GPT_PDMA_ARSNOOP(n)			(((n) & 0x7) << 16)
#define GPT_PDMA_ARREGION(n)			(((n) & 0xf) << 12)
#define GPT_PDMA_ARQOS(n)			(((n) & 0xf) << 8)
#define GPT_PDMA_ARLOCK(n)			(((n) & 0x1) << 7)
#define GPT_PDMA_ARCACHE(n)			(((n) & 0xf) << 3)
#define GPT_PDMA_ARPROT(n)			(((n) & 0x7) << 0)
#define GPT_PDMA_VERMAJOR(id)			((id >> 8) & 0xf)
#define GPT_PDMA_VERMINOR(id)			((id) & 0xf)
#define GPT_PDMA_CHNUM(state)			((state) & 0xf)
#define GPT_PDMA_PARTNUM(state)			(((state) >> 0x4) & 0xf)
#define GPT_PDMA_PARTSIZE(state)		(((state) >> 0x8) & 0xf)

/* pcie phy offset */
#define SALVO_COM_LOCK_CFG1			0x3c002
#define SALVO_COM_LOCK_CFG2			0x3c003
#define SALVO_COM_LOCK_CFG3			0x3c004
#define SALVO_RCV_DET_INH			0x3c006

#define GPT_RC_MSI_ADDR				(0x000200000UL)
#define GPT_RC_BAR0_ADDR			(0x200000000UL)
#define GPT_EP_BAR0_ADDR			(0x220000000UL)
#define GPT_PDMA_DESC_ADDR			(0x221000000UL)
#define GPT_EP_BAR1_ADDR			(0x222000000UL)
#define GPT_EP_DMA_DBASE			(0x230000000UL)

#define GPT_PCIE_COMMAND			(0x04)
#define GPT_RC_MSI_DATA				(0x0000006a)
#define GPT_PCIE_MSI_CAP			(0x90)
#define GPT_PCIE_MSI_CCTL			(GPT_PCIE_MSI_CAP + 0x00)
#define GPT_PCIE_MSI_CLADDR			(GPT_PCIE_MSI_CAP + 0x04)
#define GPT_PCIE_MSI_CHADDR			(GPT_PCIE_MSI_CAP + 0x08)
#define GPT_PCIE_MSI_CDATA			(GPT_PCIE_MSI_CAP + 0x0c)
#define GPT_PCIE_MSI_CMASK			(GPT_PCIE_MSI_CAP + 0x10)
#define GPT_PCIE_MSI_CPEND			(GPT_PCIE_MSI_CAP + 0x14)

/* pcie func defination */
#define PCI_BASE_CLASS_MEMORY			0x05
#define PCI_CLASS_MEMORY_RAM			0x0500
#define PCI_CLASS_MEMORY_FLASH			0x0501
#define PCI_CLASS_MEMORY_OTHER			0x0580
#define GPT_PCIE_DEV_SN0			0xf45a9601
#define GPT_PCIE_DEV_SN1			0x9a156601

#define GPT_PCIERE_REGN_CFG			0x0
#define GPT_PCIERE_REGN_IO			0x1
#define GPT_PCIERE_REGN_DATA			0x2
#define GPT_PCIERE_REGN_MSI			0x3


#define GPT_PCIEDEV_REGN_MSI			0x0
#define GPT_PCIEDEV_OUTBD_MSI			0x00000000
#define GPT_PCIEDEV_OUTBD_MSI_AXI		0x100000000
#define GPT_PCIEDEV_MSI_PASS			20
#define GPT_PCIEDEV_REGN_DATA			0x1
#define GPT_PCIEDEV_OUTBD_DATA			0x00100000
#define GPT_PCIEDEV_OUTBD_DATA_AXI		0x100100000
#define GPT_PCIEDEV_DATA_PASS			20

/* pcie device register defination */
#define GPT_PCIEDEV_CTL				0x00
#define GPT_PCIEDEV_STATUS			0x04
#define GPT_PCIEDEV_DMA_TXDESC0			0x08
#define GPT_PCIEDEV_DMA_TXDESC1			0x0c
#define GPT_PCIEDEV_DMA_RXDESC0			0x10
#define GPT_PCIEDEV_DMA_RXDESC1			0x14
#define GPT_PCIEDEV_DMAADDR0			0x18
#define GPT_PCIEDEV_DMAADDR1			0x1c
#define GPT_PCIEDEV_DATAOF			0x20
#define GPT_PCIEDEV_DMALEN			0x24
#define GPT_PCIEDEV_DMACFG			0x28
#define GPT_PCIE_OB_ADDR0			0x2c
#define GPT_PCIE_OB_ADDR1			0x30
#define GPT_PCIE_OB_LEN				0x34
#define GPT_PCIE_IB_LEN				0x38
#define GPT_PCIE_XOR_KEY			0x3c

/* dma ctl offset */
#define GPT_PCIEDEV_EN_OF			(0x1 << 0)
#define GPT_PCIEDEV_CPY				(0x0 << 1)
#define GPT_PCIEDEV_XOR				(0x1 << 1)
#define GPT_PCIEDEV_RANDOM			(0x2 << 1)
#define GPT_PCIEDEV_SPEC			(0x3 << 1)
#define GPT_PCIE_IWIR				(0x1 << 16)
#define GPT_PCIE_IWOR				(0x1 << 17)
#define GPT_PCIE_IWDR				(0x1 << 18)
#define GPT_PCIE_OWIR				(0x1 << 19)
#define GPT_PCIE_OWOR				(0x1 << 20)
#define GPT_PCIE_OWDR				(0x1 << 21)
#define GPT_PCIE_DWIR				(0x1 << 22)
#define GPT_PCIE_DWOR				(0x1 << 23)
#define GPT_PCIE_DWDR				(0x1 << 24)
#define GPT_PCIE_XWDR				(0x1 << 25)
#define GPT_PCIE_DWXR				(0x1 << 26)
#define GPT_PCIE_XWIR				(0x1 << 27)
#define GPT_PCIE_IWXR				(0x1 << 28)
#define GPT_PCIE_XWOR				(0x1 << 29)
#define GPT_PCIE_OWXR				(0x1 << 30)
#define GPT_PCIEDEV_CTL_EXT			(0x1 << 31)

/* dma status offset */
#define GPT_PCIEDEV_DONE_OF			(0x1 << 0)
/* dmacfg offset */
#define GPT_PCIEDEV_DMAIN			(0x1 << 0)

/* pcie irq number */
#define GPT_PCIE_IRQNO				59

#define GPT_MASK_SET(b, s)              	(((-1UL) << (b)) &  (~((-1UL) << ((s) + 1))))

/*pcie apb enable*/
#define	GPT_PCIE_EN				(0x1 << 11) | ((0x1 << 11) << 16)	

/* phy register  defination */
#define CMN_DIAG_PLL_OVRD                       (0x01e4 * 4)
#define CMN_DIAG_PLL_ITRIM0                     (0x01e5 * 4)
#define CMN_DIAG_PLL_ITRIM1                     (0x01e6 * 4)
#define RX_DIAG_DEF_AMP_TUNE_2(m, n)            ((0x81e5 | ((m) << 13) | (n) << 9) * 4)
#define RX_REE_PEAK_THR(m, n)            	((0x80c3 | ((m) << 13) | (n) << 9) * 4)
#define RX_CDRLF_CNFG(m, n)            		((0x8080 | ((m) << 13) | (n) << 9) * 4)
#define TX_DIAG_TX_BOOST(m, n)            	((0x81e0 | ((m) << 13) | (n) << 9) * 4)
#define TX_HYDC_EMPH_TRVAL_LOW_XX(m, n, x)      (((0x4080 | ((m) << 13) | (n) << 9) | (x)) * 4)
#define TX_HYDC_EMPH_TRVAL_HIGH_XX(m, n, x)     (((0x40a0 | ((m) << 13) | (n) << 9) | (x)) * 4)
#define PHY_PCIE_ISO_RX_CTRL(n)			(((0xd008 | ((n) << 8))) * 4)
#define PHY_PCIE_ISO_TX_CTRL(n)			(((0xd000 | ((n) << 8))) * 4)
#define PHY_PCIE_ISO_TX_DATA_LO(n)		(((0xd006 | ((n) << 8))) * 4)
#define PHY_PCIE_RX_DETECT(m, n)		((0x4020 | ((m) << 13) | (n) << 9) * 4)
#define PHY_PCIE_ISO_CMN_CTRL			(0xc010 * 4)
#define PHY_PMA_CMN_CTRL			(0xe000 * 4)
#define CMN_PSC_A3				(0x0043 * 4)
#define PHY_PMA_PSM_STATE_LO			(0xe002 * 4)

#pragma pack(push, 1)
struct gpt_pdma_desc {
	u64 bus_addr;
	u32 axi_ctrl;
	u64 pci_addr;
	u32 tlp_header;
	u32 len_ctl;
	u8  axibus_st;
	u8  pcibus_st;
	u8  ch_stat;
	u8  reserved;
	struct gpt_pdma_desc *next;
};
#pragma pack(pop)

struct gpt_pcie_data {
	unsigned long regs;
	int pci_num;
	u64 phys_base;
	u64 cfg0_phys;
	u64 cfg0_size;
	u64 cfg1_phys;
	u64 cfg1_size;
};

struct gpt_pcie {
	int idx;
	void __iomem *regs;
	void __iomem *dbi;
	void __iomem *phy;
	void __iomem *cfg0_va;
	void __iomem *cfg1_va;
	u64 msi_addr;
	struct pci_controller hose;
};

//#define GPT_PCIE_DEBUG
#ifdef GPT_PCIE_DEBUG
	#define PDEBUG(fmt, args...) \
		printf("[%s:%d] " fmt, \
		__PRETTY_FUNCTION__, __LINE__ , ## args)
#else
	#define PDEBUG(fmt, args...) do {} while(0)
#endif
#endif
