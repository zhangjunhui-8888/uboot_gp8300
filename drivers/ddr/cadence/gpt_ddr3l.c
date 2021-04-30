#include <common.h>
#include <asm/io.h>

#include "asm/chip2/gpt_ddr3l.h"

#if defined(CONFIG_DDR3_2133MBPS)
#include "gpt_ddr3l_2133_config.h"
#define GPT_DDR_MODE			DDR_2133
#elif defined(CONFIG_DDR3_1600MBPS)
#include "gpt_ddr3l_1600_config.h"
#define GPT_DDR_MODE			DDR_1600
#else
#include "gpt_ddr3l_1600_config.h"
#define GPT_DDR_MODE			DDR_1600
#endif

#define MMIO_WRITE8(reg, val) 		writeb((val), gpt_ddr_base_addr + (reg))
#define MMIO_WRITE16(reg, val)		writew((val), gpt_ddr_base_addr + (reg))
#define MMIO_WRITE32(reg, val)		writel((val), gpt_ddr_base_addr + (reg))
#define MMIO_READ8(reg)			readb(gpt_ddr_base_addr + (reg))
#define MMIO_READ16(reg)		readw(gpt_ddr_base_addr + (reg))
#define MMIO_READ32(reg)		readl(gpt_ddr_base_addr + (reg))

#define TIMEOUT_INIT_VALUE		0x8ffff

static volatile unsigned char *const gpt_ddr_base_addr = (void*)GPT_DDR_BASE_ADDR;
static ddr_mode_e gpt_ddr_mode = GPT_DDR_MODE;

static void gpt_ddr_set_param(int addr, int offset, int size, int value)
{
	int param = 0, i;
	int data;

	for ( i = offset; i < (offset + size); i++)
		param |= (1 << i);
	data = MMIO_READ32(addr);
	data &= (~param);
	data |= ((value << offset) & param);
	MMIO_WRITE32(addr, data);
}

static inline void gpt_ddr_enable(void)
{
	u32 value;
	value = readl(GPT_EXTCR_LO_ADDR);
	value |= GPT_DDR_ENABLE_CONFIG;
	writel(value, GPT_EXTCR_LO_ADDR);
}

static int gpt_wait_ddr_complete(void)
{
	int timeout = TIMEOUT_INIT_VALUE;
	unsigned int status = 0;

	while(timeout--) {
		status = readl(GPT_DDR_CNTL_ADDR);
		if (status == GPT_DDR_INTR_STATUS2) {
			writel(GPT_DDR_INTR_CLEAR_CONFIG, GPT_DDR_INTR_CLEAR_ADDR);
			return 0;
		}
		udelay(1);
	};

	printf("DDR3L Init Failed, CNTL status is %x\n",status);
	return -1;
}

int gpt_ddr_config_set(void)
{
	int i;

	for ( i = 0; i < GPT_DDR3_CNTL_REG_CNT; i++) {
		MMIO_WRITE32(ctlArray[i][0], ctlArray[i][1]);
	}


	for ( i = 0; i < GPT_DDR3_PI_REG_CNT; i++) {
		MMIO_WRITE32(piArray[i][0], piArray[i][1]);
	}

	writel(0x00000100, GPT_DDR_BASE_ADDR + (3073 + 0x1000) * 4);
	for ( i = 0; i < GPT_DDR3_PHY_REG_CNT; i++) {
		MMIO_WRITE32(phyArray[i][0], phyArray[i][1]);
	}
	return 0;
}

int gpt_ddr_mode_config(ddr_mode_e mode)
{
unsigned long gpt_set_dpll(unsigned long rate);

	switch(mode)
	{
		case DDR_1333:
			gpt_set_dpll(333000000);
		break;

		case DDR_1600:
			gpt_set_dpll(400000000);
		break;

		case DDR_2133:
			gpt_set_dpll(533000000);
		break;

		default:
		break;
	}
	return 0;
}

static void gpt_ddr_start(void)
{
	gpt_ddr_enable();
	gpt_ddr_config_set();
	
	gpt_ddr_set_param(0x800 * 4, 0, 1, 0x1);
	gpt_ddr_set_param(0 * 4, 0, 1, 0x1);
}

int gpt_ddr_init(void)
{
	int i = 10000 * 10;

	while (i--); //DDR dealy reset when system reset from watchdog.
	
	gpt_ddr_mode_config(gpt_ddr_mode);
	gpt_ddr_start();

	if (gpt_wait_ddr_complete()){
		printf("GPT DDR Controller Init Failed\n");
		return -1;
	}
	
	return 0;
}
