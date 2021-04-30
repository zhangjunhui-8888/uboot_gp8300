#ifndef __GPT_CPU_DMA_H__
#define __GPT_CPU_DMA_H__
#include <common.h>
#include <asm/io.h>

#ifdef CONFIG_TEST_ON_SOC
#define GPT_CDMA_D_BASE 0x220000000
#define GPT_PXI_BASE	0x0e0000000
#define GPT_PXI_TAG	0x0e0000001
#define GPT_PXI_MASK	0x000000000
#else
#define GPT_CDMA_D_BASE 0xa0000000
#define GPT_PXI_BASE	0x00e0000000
#define GPT_PXI_TAG	0x00e0000001
#define GPT_PXI_MASK	0x0000000000
#endif

#define GPT_PXI_EN	(0x1 << 0)
#define GPT_PXI_MK(x)	((x) << 15)

#define GPT_PXI_RDATA0_BASE(n)	(GPT_PXI_BASE + n * 0x20)
#define GPT_PXI_WDATA0_BASE(n)	(GPT_PXI_BASE + n * 0x1000)

#define GPT_CDMA_RDATA0_BASE0 	(GPT_CDMA_D_BASE + 0x00000000)
#define GPT_CDMA_RDATA1_BASE0	(GPT_CDMA_D_BASE + 0x01000000)
#define GPT_CDMA_RDATA2_BASE0	(GPT_CDMA_D_BASE + 0x02000000)
#define GPT_CDMA_RDATA3_BASE0	(GPT_CDMA_D_BASE + 0x03000000)
#define GPT_CDMA_WDATA0_BASE0	(GPT_CDMA_D_BASE + 0x10000000)

#define GPT_CDMA_RDATA0_BASE1 	(GPT_CDMA_D_BASE + 0x04000000)
#define GPT_CDMA_RDATA1_BASE1	(GPT_CDMA_D_BASE + 0x05000000)
#define GPT_CDMA_RDATA2_BASE1	(GPT_CDMA_D_BASE + 0x06000000)
#define GPT_CDMA_RDATA3_BASE1	(GPT_CDMA_D_BASE + 0x07000000)
#define GPT_CDMA_WDATA0_BASE1	(GPT_CDMA_D_BASE + 0x11000000)

#define GPT_CDMA_RDATA0_BASE2 	(GPT_CDMA_D_BASE + 0x08000000)
#define GPT_CDMA_RDATA1_BASE2	(GPT_CDMA_D_BASE + 0x09000000)
#define GPT_CDMA_RDATA2_BASE2	(GPT_CDMA_D_BASE + 0x0a000000)
#define GPT_CDMA_RDATA3_BASE2	(GPT_CDMA_D_BASE + 0x0b000000)
#define GPT_CDMA_WDATA0_BASE2	(GPT_CDMA_D_BASE + 0x12000000)

#define GPT_CDMA_RDATA0_BASE3 	(GPT_CDMA_D_BASE + 0x0c000000)
#define GPT_CDMA_RDATA1_BASE3	(GPT_CDMA_D_BASE + 0x0d000000)
#define GPT_CDMA_RDATA2_BASE3	(GPT_CDMA_D_BASE + 0x0e000000)
#define GPT_CDMA_RDATA3_BASE3	(GPT_CDMA_D_BASE + 0x0f000000)
#define GPT_CDMA_WDATA0_BASE3	(GPT_CDMA_D_BASE + 0x13000000)

#define GPT_CDMA_RDATA0_V_BASE0 (GPT_CDMA_D_BASE + 0x20000000)
#define GPT_CDMA_RDATA1_V_BASE0	(GPT_CDMA_D_BASE + 0x21000000)
#define GPT_CDMA_RDATA2_V_BASE0	(GPT_CDMA_D_BASE + 0x22000000)
#define GPT_CDMA_RDATA3_V_BASE0	(GPT_CDMA_D_BASE + 0x23000000)
#define GPT_CDMA_WDATA0_V_BASE0	(GPT_CDMA_D_BASE + 0x30000000)

#define GPT_CDMA_RDATA0_V_BASE1 (GPT_CDMA_D_BASE + 0x24000000)
#define GPT_CDMA_RDATA1_V_BASE1	(GPT_CDMA_D_BASE + 0x25000000)
#define GPT_CDMA_RDATA2_V_BASE1	(GPT_CDMA_D_BASE + 0x26000000)
#define GPT_CDMA_RDATA3_V_BASE1	(GPT_CDMA_D_BASE + 0x27000000)
#define GPT_CDMA_WDATA0_V_BASE1	(GPT_CDMA_D_BASE + 0x31000000)

#define GPT_CDMA_RDATA0_V_BASE2 (GPT_CDMA_D_BASE + 0x28000000)
#define GPT_CDMA_RDATA1_V_BASE2	(GPT_CDMA_D_BASE + 0x29000000)
#define GPT_CDMA_RDATA2_V_BASE2	(GPT_CDMA_D_BASE + 0x2a000000)
#define GPT_CDMA_RDATA3_V_BASE2	(GPT_CDMA_D_BASE + 0x2b000000)
#define GPT_CDMA_WDATA0_V_BASE2	(GPT_CDMA_D_BASE + 0x32000000)

#define GPT_CDMA_RDATA0_V_BASE3 (GPT_CDMA_D_BASE + 0x2c000000)
#define GPT_CDMA_RDATA1_V_BASE3	(GPT_CDMA_D_BASE + 0x2d000000)
#define GPT_CDMA_RDATA2_V_BASE3	(GPT_CDMA_D_BASE + 0x2e000000)
#define GPT_CDMA_RDATA3_V_BASE3	(GPT_CDMA_D_BASE + 0x2f000000)
#define GPT_CDMA_WDATA0_V_BASE3	(GPT_CDMA_D_BASE + 0x33000000)

#define GPT_CPU_DMA_BASE	0xe2000000
#define GPT_CPU_DMA_MASK	0x0000ffff

/* register */
#define GPT_CDMA_STATUS			0x000
#define GPT_CDMA_MISS			0x001
#define GPT_CDMA_SCTL			0x002
#define GPT_CDMA_ASYNC			0x003
#define GPT_CDMA_CMD			0x005

#define GPT_CDMA_OP_SET(n)		(0x006 + (n) * 0x20)
#define GPT_CDMA_WITH_SET(n)		(0x007 + (n) * 0x20)
#define GPT_CDMA_RDBASE0_SET(n)		(0x008 + (n) * 0x20)
#define GPT_CDMA_RDBASE1_SET(n)		(0x009 + (n) * 0x20)
#define GPT_CDMA_RDBASE2_SET(n)		(0x00a + (n) * 0x20)
#define GPT_CDMA_RDBASE3_SET(n)		(0x00b + (n) * 0x20)
#define GPT_CDMA_WRBASE_SET(n)		(0x00c + (n) * 0x20)
#define GPT_CDMA_RDCOUNT0_SET(n)	(0x010 + (n) * 0x20)
#define GPT_CDMA_RDCOUNT1_SET(n)	(0x012 + (n) * 0x20)
#define GPT_CDMA_RDSTRIDE1_SET(n)	(0x013 + (n) * 0x20)
#define GPT_CDMA_WRCOUNT0_SET(n)	(0x018 + (n) * 0x20)
#define GPT_CDMA_WRCOUNT1_SET(n)	(0x019 + (n) * 0x20)
#define GPT_CDMA_WRSTRIDE1_SET(n)	(0x01a + (n) * 0x20)
#define GPT_CDMA_RDMASK_TLB(n)		(0x080 + (n) * 0x08)
#define GPT_CDMA_RDTAG_TLB(n)		(0x081 + (n) * 0x08)
#define GPT_CDMA_RDREAL_TLB(n)		(0x082 + (n) * 0x08)
#define GPT_CDMA_WRMASK_TLB(n)		(0x084 + (n) * 0x08)
#define GPT_CDMA_WRTAG_TLB(n)		(0x085 + (n) * 0x08)
#define GPT_CDMA_WRREAL_TLB(n)		(0x086 + (n) * 0x08)

#define GPT_CDMA_OP_SET0		0x006
#define GPT_CDMA_WITH_SET0		0x007
#define GPT_CDMA_RDBASE0_SET0		0x008
#define GPT_CDMA_RDBASE1_SET0		0x009
#define GPT_CDMA_RDBASE2_SET0		0x00a
#define GPT_CDMA_RDBASE3_SET0		0x00b
#define GPT_CDMA_WRBASE_SET0		0x00c
#define GPT_CDMA_RDCOUNT0_SET0		0x010
#define GPT_CDMA_RDCOUNT1_SET0		0x012
#define GPT_CDMA_RDSTRIDE1_SET0		0x013
#define GPT_CDMA_WRCOUNT0_SET0		0x018
#define GPT_CDMA_WRCOUNT1_SET0		0x019
#define GPT_CDMA_WRSTRIDE1_SET0		0x01a

#define GPT_CDMA_OP_SET1		0x027
#define GPT_CDMA_RDBASE0_SET1		0x028
#define GPT_CDMA_RDBASE1_SET1		0x029
#define GPT_CDMA_RDBASE2_SET1		0x02a
#define GPT_CDMA_RDBASE3_SET1		0x02b
#define GPT_CDMA_WRBASE_SET1		0x02c
#define GPT_CDMA_RDCOUNT0_SET1		0x030
#define GPT_CDMA_RDCOUNT1_SET1		0x032
#define GPT_CDMA_RDSTRIDE1_SET1		0x033
#define GPT_CDMA_WRCOUNT0_SET1		0x038
#define GPT_CDMA_WRCOUNT1_SET1		0x039
#define GPT_CDMA_WRSTRIDE1_SET1		0x03a

#define GPT_CDMA_OP_SET2		0x047
#define GPT_CDMA_RDBASE0_SET2		0x048
#define GPT_CDMA_RDBASE1_SET2		0x049
#define GPT_CDMA_RDBASE2_SET2		0x04a
#define GPT_CDMA_RDBASE3_SET2		0x04b
#define GPT_CDMA_WRBASE_SET2		0x04c
#define GPT_CDMA_RDCOUNT0_SET2		0x050
#define GPT_CDMA_RDCOUNT1_SET2		0x052
#define GPT_CDMA_RDSTRIDE1_SET2		0x053
#define GPT_CDMA_WRCOUNT0_SET2		0x058
#define GPT_CDMA_WRCOUNT1_SET2		0x059
#define GPT_CDMA_WRSTRIDE1_SET2		0x05a

#define GPT_CDMA_OP_SET3		0x067
#define GPT_CDMA_RDBASE0_SET3		0x068
#define GPT_CDMA_RDBASE1_SET3		0x069
#define GPT_CDMA_RDBASE2_SET3		0x06a
#define GPT_CDMA_RDBASE3_SET3		0x06b
#define GPT_CDMA_WRBASE_SET3		0x06c
#define GPT_CDMA_RDCOUNT0_SET3		0x070
#define GPT_CDMA_RDCOUNT1_SET3		0x072
#define GPT_CDMA_RDSTRIDE1_SET3		0x073
#define GPT_CDMA_WRCOUNT0_SET3		0x078
#define GPT_CDMA_WRCOUNT1_SET3		0x079
#define GPT_CDMA_WRSTRIDE1_SET3		0x07a

#define GPT_CDMA_RDMASK_TLB0		0x080
#define GPT_CDMA_RDTAG_TLB0		0x081
#define GPT_CDMA_RDREAL_TLB0		0x082
#define GPT_CDMA_WRMASK_TLB0		0x084
#define GPT_CDMA_WRTAG_TLB0		0x085
#define GPT_CDMA_WRREAL_TLB0		0x086

#define GPT_CDMA_RDMASK_TLB1		0x088
#define GPT_CDMA_RDTAG_TLB1		0x089
#define GPT_CDMA_RDREAL_TLB1		0x08a
#define GPT_CDMA_WRMASK_TLB1		0x08c
#define GPT_CDMA_WRTAG_TLB1		0x08d
#define GPT_CDMA_WRREAL_TLB1		0x08e

#define GPT_CDMA_RDMASK_TLB2		0x090
#define GPT_CDMA_RDTAG_TLB2		0x091
#define GPT_CDMA_RDREAL_TLB2		0x092
#define GPT_CDMA_WRMASK_TLB2		0x094
#define GPT_CDMA_WRTAG_TLB2		0x095
#define GPT_CDMA_WRREAL_TLB2		0x096

#define GPT_CDMA_RDMASK_TLB3		0x0a8
#define GPT_CDMA_RDTAG_TLB3		0x0a9
#define GPT_CDMA_RDREAL_TLB3		0x0aa
#define GPT_CDMA_WRMASK_TLB3		0x0ac
#define GPT_CDMA_WRTAG_TLB3		0x0ad
#define GPT_CDMA_WRREAL_TLB3		0x0ae

#define GPT_CDMA_A_CMD			0x105
#define GPT_CDMA_A_OP_SET0		0x106
#define GPT_CDMA_A_WITH_SET0		0x107
#define GPT_CDMA_A_RDBASE0_SET0		0x108
#define GPT_CDMA_A_RDBASE1_SET0		0x109
#define GPT_CDMA_A_RDBASE2_SET0		0x10a
#define GPT_CDMA_A_RDBASE3_SET0		0x10b
#define GPT_CDMA_A_WRBASE_SET0		0x10c
#define GPT_CDMA_A_RDCOUNT0_SET0	0x110
#define GPT_CDMA_A_RDCOUNT1_SET0	0x112
#define GPT_CDMA_A_RDSTRIDE1_SET0	0x112
#define GPT_CDMA_A_WRCOUNT0_SET0	0x118
#define GPT_CDMA_A_WRCOUNT1_SET0	0x119
#define GPT_CDMA_A_WRSTRIDE1_SET0	0x11a

#define GPT_CDMA_A_OP_SET1		0x127
#define GPT_CDMA_A_RDBASE0_SET1		0x128
#define GPT_CDMA_A_RDBASE1_SET1		0x129
#define GPT_CDMA_A_RDBASE2_SET1		0x12a
#define GPT_CDMA_A_RDBASE3_SET1		0x12b
#define GPT_CDMA_A_WRBASE_SET1		0x12c
#define GPT_CDMA_A_RDCOUNT0_SET1	0x130
#define GPT_CDMA_A_RDCOUNT1_SET1	0x132
#define GPT_CDMA_A_RDSTRIDE1_SET1	0x133
#define GPT_CDMA_A_WRCOUNT0_SET1	0x138
#define GPT_CDMA_A_WRCOUNT1_SET1	0x139
#define GPT_CDMA_A_WRSTRIDE1_SET1	0x13a

#define GPT_CDMA_A_OP_SET2		0x147
#define GPT_CDMA_A_RDBASE0_SET2		0x148
#define GPT_CDMA_A_RDBASE1_SET2		0x149
#define GPT_CDMA_A_RDBASE2_SET2		0x14a
#define GPT_CDMA_A_RDBASE3_SET2		0x14b
#define GPT_CDMA_A_WRBASE_SET2		0x14c
#define GPT_CDMA_A_RDCOUNT0_SET2	0x150
#define GPT_CDMA_A_RDCOUNT1_SET2	0x152
#define GPT_CDMA_A_RDSTRIDE1_SET2	0x153
#define GPT_CDMA_A_WRCOUNT0_SET2	0x158
#define GPT_CDMA_A_WRCOUNT1_SET2	0x159
#define GPT_CDMA_A_WRSTRIDE1_SET2	0x15a

#define GPT_CDMA_A_OP_SET3		0x167
#define GPT_CDMA_A_RDBASE0_SET3		0x168
#define GPT_CDMA_A_RDBASE1_SET3		0x169
#define GPT_CDMA_A_RDBASE2_SET3		0x16a
#define GPT_CDMA_A_RDBASE3_SET3		0x16b
#define GPT_CDMA_A_WRBASE_SET3		0x16c
#define GPT_CDMA_A_RDCOUNT0_SET3	0x170
#define GPT_CDMA_A_RDCOUNT1_SET3	0x172
#define GPT_CDMA_A_RDSTRIDE1_SET3	0x173
#define GPT_CDMA_A_WRCOUNT0_SET3	0x178
#define GPT_CDMA_A_WRCOUNT1_SET3	0x179
#define GPT_CDMA_A_WRSTRIDE1_SET3	0x17a

/* status register offset */
#define GPT_STATUS_A			(0x1 << 0)
#define GPT_STATUS_D			(0x1 << 1)
#define GPT_STATUS_W			(0x1 << 2)
#define GPT_STATUS_P			(0x1 << 3)
#define GPT_STATUS_R			(0x1 << 4)
#define GPT_STATUS_E			(0x1 << 5)
#define GPT_STATUS_CODE			(0x3 << 6)
#define GPT_STATUS_OLD			(0x3 << 8)
/*sctl register offset*/
#define GPT_SCTL_VIRT			(0x01 << 0)
#define GPT_SCTL_RL2			(0x01 << 1)
#define GPT_SCTL_SYNC			(0x01 << 2)
/* async register offset */
#define GPT_ASYNC_RESET_OFF		(0x4)
#define GPT_ASYNC_PAUSE_OFF		(0x3)
#define GPT_ASYNC_CLEAR_OFF		(0x2)
#define GPT_ASYNC_RESUM_OFF		(0x1)
/* cmd register offset */
#define GPT_CMD_ACTIVE			(0x1 << 0)
#define GPT_CMD_SET0			(0x0 << 5)
#define GPT_CMD_SET0_EN			(0x1 << 4)
#define GPT_CMD_SET1			(0x1 << 9)
#define GPT_CMD_SET1_EN			(0x1 << 8)
#define GPT_CMD_SET2			(0x2 << 13)
#define GPT_CMD_SET2_EN			(0x1 << 12)
#define GPT_CMD_SET3			(0x3 << 17)
#define GPT_CMD_SET3_EN			(0x1 << 16)
/* tlb mask register offset */
#define GPT_TLBMASK_EN			(0x1 << 0)
#define GPT_TLBMASK_L2			(0x1 << 1)
#define GPT_TLBMASK_SYNC		(0x1 << 2)
#define GPT_TLBMASK_MK(n)		((n) & (~0x7))

#define NSETS_DMA   4
#define BASES_DMA   4
#define DEPTH_DMA   2
#define NRDREQS_DMA 4
#define NBUFS_DMA   5
#define WITH_DMA    8
#define RDTLBS_DMA  4
#define WRTLBS_DMA  4

#define ACTIVE_CTL			(1)
#define REGSET0_CTL(v__)		((((v__)&0x3)<<5)|1<<4)
#define REGSET1_CTL(v__)		((((v__)&0x3)<<9)|(1<<8))
#define REGSET2_CTL(v__)		((((v__)&0x3)<<13)|(1<<12))
#define REGSET3_CTL(v__)		((((v__)&0x3)<<17)|(1<<16))

/* op_set register offset */
#define GPT_OP_TY_COPY			((0x0 & 0x3) << 0)
#define GPT_OP_TY_FILL			((0x1 & 0x3) << 0)
#define GPT_OP_TY_ZERO_PAD		((0x2 & 0x3) << 0)
#define GPT_OP_TY_COPY_PAD		((0x3 & 0x3) << 0)
#define GPT_OP_RB_NUM0			((0x0 & 0x3) << 8)
#define GPT_OP_RB_NUM1			((0x1 & 0x3) << 8)
#define GPT_OP_RB_NUM2			((0x2 & 0x3) << 8)
#define GPT_OP_RB_NUM3			((0x3 & 0x3) << 8)
#define GPT_OP_SZ_1B			((0x0 & 0x3) << 12)
#define GPT_OP_SZ_2B			((0x1 & 0x3) << 12)
#define GPT_OP_SZ_4B			((0x2 & 0x3) << 12)
#define GPT_OP_SZ_8B			((0x3 & 0x3) << 12)
#define GPT_OP_CN_1B			((0x0 & 0xF) << 14)
#define GPT_OP_CN_2B			((0x1 & 0xF) << 14)
#define GPT_OP_CN_4B			((0x3 & 0xF) << 14)
#define GPT_OP_CN_8B			((0x7 & 0xF) << 14)
#define GPT_OP_RD_MULTIPLE		(0x1 << 4)
#define GPT_OP_WR_MULTIPLE		(0x1 << 6)

#define GPT_DMA_REG(offset)		(cdma_ctrl + offset)

typedef struct gpt_dma_cfg_s {
	int setx;
	int basex;
	int mode;
	int rcol;
	int rrow;
	int dcol;
	int drow;
	uint64_t r0;
	uint64_t r1;
	uint64_t r2;
	uint64_t r3;
	uint64_t d0;
	uint64_t rstr;
	uint64_t dstr;
} gpt_cdma_cfg;

int gpt_cdma_init(void);
int gpt_cdma_data_prepare(int type);
uint64_t gpt_cdma_read64(uint64_t *addr);
uint64_t gpt_cdma_write64(uint64_t *addr, uint64_t value);
int gpt_cdma_copy_test(int type);
void gpt_cdma_dumpdata(unsigned char *data, int length);
int gpt_cdma_copy_mul(int nbase, uint64_t src0, uint64_t src1,
			uint64_t src2, uint64_t src3,
			uint64_t dst, int len);
void gpt_data_init(int type);
int gpt_cdma_mem_real_copy(uint8_t *dst, uint8_t *src, int len);

uint64_t *gpt_pxi_ctl_map(void);
int gpt_pxi_read(unsigned char *dst, unsigned int offset, unsigned int size);
int gpt_pxi_write(unsigned char *src, unsigned int offset, unsigned int size);
uint64_t gpt_cdma_get_psc_spr(void);
static unsigned int gpt_data_random(unsigned char *buffer, int len);
int gpt_cdma_transfer128(uint64_t *addr, uint64_t *data);
void gpt_core_en(int num);
int gpt_cdma_multcore_cmd(uint32_t cmd);
int gpt_cdma_multcore_cmdump(uint64_t addr);
void gpt_cdma_nosync_en(void);
int gpt_cdma_mem_virt_copy(uint8_t *dst, uint8_t *src, int length);
#endif
