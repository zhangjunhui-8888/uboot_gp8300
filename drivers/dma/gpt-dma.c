#include <gpt-dma.h>

#ifdef GPT_CDMA_USER
uint64_t *cdma_ctrl = (void *)GPT_CPU_DMA_BASE + 0x8000;
#else
uint64_t *cdma_ctrl = (void *)GPT_CPU_DMA_BASE;
#endif

void gpt_l2_flush(uint64_t addr, int size);
void gpt_l2_sync(uint64_t addr, int size);
int gpt_cdma_1dto1d_1set_data_ck(int nbase, uint8_t *src0, uint8_t *src1,
			uint8_t *src2, uint8_t *src3, uint8_t *dst, int len);
int gpt_cdma_mem_real_copy(uint8_t *dst, uint8_t *src, int len);
gpt_cdma_cfg cmd_mode[4] = {
{0, 0, 0x0, 0x0, 0x0, 0x0, 0x0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0, GPT_CDMA_RDATA3_BASE0, GPT_CDMA_WDATA0_BASE0, 0x0, 0x0},
{1, 0, 0x0, 0x0, 0x0, 0x0, 0x0, GPT_CDMA_RDATA0_BASE1, GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1, GPT_CDMA_RDATA3_BASE1, GPT_CDMA_WDATA0_BASE1, 0x0, 0x0},
{2, 0, 0x0, 0x0, 0x0, 0x0, 0x0, GPT_CDMA_RDATA0_BASE2, GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2, GPT_CDMA_RDATA3_BASE2, GPT_CDMA_WDATA0_BASE2, 0x0, 0x0},
{3, 0, 0x0, 0x0, 0x0, 0x0, 0x0, GPT_CDMA_RDATA0_BASE3, GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3, GPT_CDMA_RDATA3_BASE3, GPT_CDMA_WDATA0_BASE3, 0x0, 0x0},
};

#ifdef GPT_RW_32
static unsigned int gpt_cdma_read(unsigned int reg)
{
	unsigned int *base_addr;

	base_addr = (unsigned int *)cdma_ctrl;
	return readl(base_addr + reg);
}

static unsigned int gpt_cdma_write(unsigned int reg, unsigned int value)
{
	unsigned int *base_addr;

	base_addr = (unsigned int *)cdma_ctrl;
	return writel(value, base_addr + reg);
}
#else

int gpt_cdma_transfer128(uint64_t *addr, uint64_t *data)
{

	int size = 9;
#ifdef GPT_CDMA_DEBUG
	printf("read address: %p\n", addr);
#endif
	__asm__ volatile (
		"nsetr_h_u $n0, %2\n\r"
		"ldv_h   $v0, %1, $n0; barrier;\n\t"
		"stv_h   $v0, %0, $n0; barrier;\n\t"
		: "+a"(data)
		: "a"(addr), "r"(size)
		:
	);
	return 0;
}
uint64_t gpt_cdma_read64(uint64_t *addr)
{
	uint64_t value;

#ifdef GPT_CDMA_DEBUG
	printf("read address: %p\n", addr);
#endif
	__asm__ volatile (
		"ldl     %0,%1; barrier;\n\t"
		: "=r"(value)
		: "a"(addr)
		:
	);
	return value;
}

uint64_t gpt_cdma_write64(uint64_t *addr, uint64_t value)
{
#ifdef GPT_CDMA_DEBUG
	printf("write address: %p, value:%llx\n", addr, value);
#endif
	__asm__ volatile (
		"stl     %0,%1; barrier;\n\t"
		:
		: "r"(value), "a"(addr)
		:
	);

	return  value;
}
#endif

static uint64_t gpt_cdma_get_spr(void)
{
	uint64_t address = 0;
	uint64_t mask = 0;

	__asm__ volatile (
		"rsetspr   %0,%1\n\t"
		: "=r" (address)
		: "i"(DMATAG)
		);

	__asm__ volatile (
		"rsetspr   %0,%1\n\t"
		: "=r" (mask)
		: "i"(DMAMASK)
	);
#ifndef GPT_CDMA_DEBUG
	printf("DMATAG: %llx, DMAMSK: %llx\n", address, mask);
#endif
	if (address | mask)
		return 0;
	else
		return -1;

}

uint64_t gpt_cdma_get_psc_spr(void)
{
	uint64_t address = 0;
	uint64_t mask = 0;

	__asm__ volatile (
		"rsetspr   %0,%1\n\t"
		: "=r" (address)
		: "i"(CPUC)
		);

	__asm__ volatile (
		"rsetspr   %0,%1\n\t"
		: "=r" (mask)
		: "i"(PSC)
	);

	printf("CPUC: %llx, PSC: %llx\n", address, mask);
	return 0;
}

void gpt_cdma_dumpdata(unsigned char *data, int length)
{
	int i;
#ifdef GPT_CDMA_DEBUG
	printf("dump data  length %.8x\n", length);
#endif
	for (i = 0; i < length; i++) {
		if (i % 16 == 0)
			printf("%.8x: ", i);
		printf("%.2x%c", data[i], i % 16 == 15 ? '\n' : ' ');
	}
	printf("\n");
}

uint64_t *gpt_pxi_ctl_map(void)
{
	uint64_t address = GPT_PXI_TAG;
	uint64_t mask = GPT_PXI_MASK;

	__asm__ volatile (
		"sprsetr   %0,%1\n\t"
		:
		: "r"(address), "i"(PXITAG)
	);
	__asm__ volatile (
		"sprsetr   %0,%1\n\t"
		:
		: "r"(mask), "i"(PXIMASK)
	);

	return (uint64_t *)(GPT_PXI_BASE);
}

int gpt_pxi_read(unsigned char *dst, unsigned int offset, unsigned int size)
{

	gpt_cdma_transfer128((void *)GPT_PXI_BASE, (void *)dst);
	gpt_cdma_dumpdata(dst, size);
	return 0;
}

int gpt_pxi_write(unsigned char *src, unsigned int offset, unsigned int size)
{
	gpt_cdma_dumpdata(src, size);
	memcpy((void *)GPT_PXI_BASE + offset, src, size);
	gpt_cdma_dumpdata((void *)GPT_PXI_BASE + offset, size);
	return 0;
}


static uint64_t *gpt_cdma_ctl_map(void)
{
	/*
	 * write the address to the DMA register to
	 * set up the register control spaces
	*/
	uint64_t address = GPT_CPU_DMA_BASE + 0x1;
	uint64_t mask = GPT_CPU_DMA_MASK;

	__asm__ volatile (
		"sprsetr   %0,%1\n\t"
		:
		: "r"(address), "i"(DMATAG)
	);
	__asm__ volatile (
		"sprsetr   %0,%1\n\t"
		:
		: "r"(mask), "i"(DMAMASK)
	);

	return (uint64_t *)(GPT_CPU_DMA_BASE);
}

uint64_t gpt_cdma_status(void)
{
	return gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
}

uint64_t gpt_cdma_irq_clr(void)
{
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_ASYNC), GPT_ASYNC_CLEAR_OFF);
#if 0
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
	} while (status & GPT_STATUS_W);
#endif
	return 0;
}

void gpt_cdma_reset(void)
{
	uint64_t status;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_ASYNC), GPT_ASYNC_RESET_OFF);
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
	} while (!(status & GPT_STATUS_R));

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), 0x0);
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
	} while (status & GPT_STATUS_W);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_ASYNC), GPT_ASYNC_CLEAR_OFF);
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
	} while (status & GPT_STATUS_W);
}

int gpt_cdma_setn_op(int setn, uint64_t mode)
{

	if (setn > 4 || setn < 0) {
		printf("0 < nset < 4\n");
		return -1;
	}

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET(setn)), mode);

	return 0;
}

/*
 *setn: number of set
 *src0: base0 address
 *src1: base1 address
 *src2: base2 address
 *src3: base3 address
 *rows: if 1d to 1d rows is count0, if multiple d rows is number of rows
 *columns: number of columns
 */
void gpt_cdma_rd_setx(int setn, uint64_t src0, uint64_t src1,
		uint64_t src2, uint64_t src3, uint64_t columns, uint64_t rows, uint64_t rstr)
{
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE0_SET(setn)), src0);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE1_SET(setn)), src1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE2_SET(setn)), src2);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE3_SET(setn)), src3);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT0_SET(setn)), columns - 1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT1_SET(setn)), rows - 1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDSTRIDE1_SET(setn)), rstr);
}

void gpt_cdma_wr_setx(int setn, uint64_t columns,
		uint64_t rows, uint64_t stride, uint64_t dst)
{
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET(setn)), dst);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT0_SET(setn)), columns - 1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT1_SET(setn)), rows - 1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRSTRIDE1_SET(setn)), stride);
}

int gpt_cdma_setx_test(int setn, int nbase, int type, uint64_t src0,
			uint64_t src1, uint64_t src2, uint64_t src3,
			uint64_t dst, int len)
{
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE0_SET(setn)), src0);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE1_SET(setn)), src1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE2_SET(setn)), src2);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE3_SET(setn)), src3);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT0_SET(setn)), len - 1);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET(setn)), dst);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT0_SET(setn)), nbase * len - 1);

	gpt_cdma_setn_op(setn, type);

	return 0;
}

int gpt_cdma_setx_cfg(int setn, int nbase, int type, uint64_t src0,
			uint64_t src1, uint64_t src2, uint64_t src3,
			uint64_t dst, int rows)
{
	gpt_cdma_rd_setx(setn, src0, src1, src2, src3, rows, 0, 0);
	gpt_cdma_wr_setx(setn, rows * nbase, 0, 0, dst);
	return 0;
}

int gpt_cdma_setx_config(int setn, int nbase, int type, uint64_t src0,
			uint64_t src1, uint64_t src2, uint64_t src3,
			uint64_t dst, int len)
{

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE0_SET(setn)),
			src0);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE1_SET(setn)),
			src1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE2_SET(setn)),
			src2);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE3_SET(setn)),
			src3);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT0_SET(setn)), len - 1);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET(setn)),
			dst);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT0_SET(setn)),
			nbase * len - 1);
	switch (nbase) {
	case 1:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET(setn)),
				type | GPT_OP_RB_NUM0);
		break;
	case 2:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET(setn)),
				type | GPT_OP_RB_NUM1);
		break;
	case 3:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET(setn)),
				type | GPT_OP_RB_NUM2);
		break;
	case 4:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET(setn)),
				type | GPT_OP_RB_NUM3);
		break;
	}

	return 0;
}

int gpt_cdma_copy_mul_w2d(int nbase, uint64_t src0, uint64_t src1,
			uint64_t src2, uint64_t src3, int len,
			uint64_t dst, int row_num)
{
	int status;
	int timeout = 0x80000;
	int bytes_row = len * nbase / row_num;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE0_SET0), src0);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE1_SET0), src1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE2_SET0), src2);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE3_SET0), src3);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT0_SET0), len - 1);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET0), dst);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT0_SET0), bytes_row - 1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT1_SET0), row_num - 1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRSTRIDE1_SET0), 0x80);

	switch (nbase) {
	case 1:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
				GPT_OP_WR_MULTIPLE | (GPT_OP_RB_NUM0));
		break;
	case 2:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
				GPT_OP_WR_MULTIPLE | (GPT_OP_RB_NUM1));
		break;
	case 3:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
			GPT_OP_WR_MULTIPLE | (GPT_OP_RB_NUM2));
		break;
	case 4:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
			GPT_OP_WR_MULTIPLE | (GPT_OP_RB_NUM3));
		break;
	}

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD),
			GPT_CMD_SET0_EN | GPT_CMD_ACTIVE);

	printf("cdma wait for status\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0) {
		printf("cdma copy write timeout\n");
		return -1;
	}
	return 0;
}

int gpt_cdma_copy_mul(int nbase, uint64_t src0, uint64_t src1,
			uint64_t src2, uint64_t src3,
			uint64_t dst, int len)
{
	int timeout = 0xfffff;
	int status;
/*
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_CMD));
		timeout--;
	} while ((status & GPT_CMD_ACTIVE) && (timeout > 0));
	timeout = 0xfffff;
*/

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE0_SET0),
			src0);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE1_SET0),
			src1);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE2_SET0),
			src2);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE3_SET0),
			src3);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT0_SET0), len - 1);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET0),
			dst);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT0_SET0),
			nbase * len - 1);

//	printf("write base %llx\n", gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET0)));

	switch (nbase) {
	case 1:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
				GPT_OP_RB_NUM0);
		break;
	case 2:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
				GPT_OP_RB_NUM1);
		break;
	case 3:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
				GPT_OP_RB_NUM2);
		break;
	case 4:
		gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0),
				GPT_OP_RB_NUM3);
		break;
	}

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD),
			GPT_CMD_SET0_EN | GPT_CMD_ACTIVE);

//	printf("cdma wait for status\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0) {
		printf("cdma copy write timeout\n");
		return -1;
	}

	return 0;
}

inline int gpt_cdma_cmd_enable(int nset)
{
	uint64_t reg = 0;

	switch (nset) {
	case 3:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 2:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 1:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 0:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}

	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

	return 0;
}

int gpt_cdma_wait_done(void)
{
	int timeout = 0x100000;
	uint64_t status;

	udelay(0x1000);
#ifdef GPT_CDMA_DEBUG
	printf("wait for done flag\n");
#endif
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		if (status & GPT_STATUS_E) {
			printf("error status: %llx, mis address: %llx\n",
				status & GPT_STATUS_CODE, gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_MISS)));
		}
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0) {
		printf("gpt cdma wait timeout\n");
		return -1;
	}

#ifdef GPT_CDMA_DEBUG
	printf("gpt cdma transfer complete\n");
#endif
	return 0;
}

uint64_t gpt_cdma_len_cfg(int n)
{
	uint64_t len = 0;

	switch (n) {
	case 1:
		len = 0xffff;
		break;
	case 2:
		len = 0x8000;
		break;
	case 3:
		len = 0x5000;
		break;
	case 4:
		len = 0x4000;
		break;
	default:
		len = 0x4000;
		printf("use default 0 base\n");
	}
	printf("gpt CDMA copy %llx bytes\n", len);
	return len;
}

int gpt_cdma_1set_nbase_check(int nbase, uint8_t *src0,  uint8_t *src1,
			uint8_t *src2, uint8_t *src3, int cols, int rows,
			int str, uint8_t *dst)
{
	int i, j;
	int rindex = 0;
	int windex = 0;
	uint8_t *rbuff = NULL;
	uint8_t rd, wd;

	printf("nbase: %d, rows: %x, columns: %x aaaaaa\n", nbase, rows, cols);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < nbase * cols; j++) {
			switch (rindex % nbase) {
			case 0:
				rbuff = src0;
				break;

			case 1:
				rbuff = src1;
				break;

			case 2:
				rbuff = src2;
				break;

			case 3:
				rbuff = src3;
				break;
			default:
				return -1;
			}

			rd = rbuff[rindex++ / nbase];
			wd = dst[windex++];
#ifdef GPT_CDMA_DEBUG
			printf("address: %p, %.2x <----> %.2x \n", rbuff + (rindex -1) / nbase, rd, wd);
#endif
			if (rd != wd) {
				printf("check failed r-w: 0x%x-->0x%x, 0x%x-->0x%x\n",
					rindex, windex, rd, wd);
				return -1;
			}

		}
		rindex += str;
		windex += str;
	}
	printf("check successful\n");
	return 0;
}

uint64_t gpt_cdma_nbase_cfg(int n)
{
	uint64_t mode = 0;

	switch (n) {
	case 1:
		mode = GPT_OP_RB_NUM0;
		break;
	case 2:
		mode = GPT_OP_RB_NUM1;
		break;
	case 3:
		mode = GPT_OP_RB_NUM2;
		break;
	case 4:
		mode = GPT_OP_RB_NUM3;
		break;
	default:
		mode = GPT_OP_RB_NUM0;
		printf("use default 0 base\n");
	}

	return mode;
}

int gpt_cdma_xset_xbase(int setx, int basex, int mode, uint64_t r0,
			uint64_t r1, uint64_t r2, uint64_t r3, int rstr,
			uint64_t rcol, int rrow, uint64_t d0, int dstr,
			int dcol, int drow)
{
	gpt_cdma_rd_setx(setx, r0, r1, r2, r3, rcol, rrow, rstr);
	gpt_cdma_wr_setx(setx, dcol, drow, dstr, d0);
	gpt_cdma_setn_op(setx, mode);
	return 0;
}

int gpt_cdma_mul_2d_to_2d_virtual(int nset, int basen)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY |
		GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE | gpt_cdma_nbase_cfg(basen);

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_2d_to_2d_pad_virtual(int nset, uint64_t mode)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_2d_to_2d_fill_virtual(int nbase)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_FILL |
		GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(nbase - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_2d_to_2d_pad(int nset, int basen)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B |
		GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE | gpt_cdma_nbase_cfg(basen);

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_2d_to_2d_fill(int nset)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_FILL |
		GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_2d_to_2d(int nset, int nbase)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY |
		GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_xset_xbase(0, nbase, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * nbase, row);

	gpt_cdma_xset_xbase(1, nbase, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * nbase, row);

	gpt_cdma_xset_xbase(2, nbase, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * nbase, row);

	gpt_cdma_xset_xbase(3, nbase, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * nbase, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2d_to_2d(void)
{
	int len = 0x1000;
	int stride = 0x80;
	gpt_cdma_rd_setx(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA1_BASE0,
			GPT_CDMA_RDATA2_BASE0, GPT_CDMA_RDATA3_BASE0, len, 0x10, 0);
	gpt_cdma_wr_setx(0, len, 0x10, stride, GPT_CDMA_WDATA0_BASE0);
	gpt_cdma_setn_op(0, GPT_OP_RB_NUM0 |
		GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE);

	gpt_cdma_rd_setx(1, GPT_CDMA_RDATA0_BASE1, GPT_CDMA_RDATA1_BASE1,
			GPT_CDMA_RDATA2_BASE1, GPT_CDMA_RDATA3_BASE1, len, 0x10, 0);
	gpt_cdma_wr_setx(1, len, 0x10, stride, GPT_CDMA_WDATA0_BASE1);
	gpt_cdma_setn_op(1, GPT_OP_RB_NUM0 |
			GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE);

	gpt_cdma_rd_setx(2, GPT_CDMA_RDATA0_BASE2, GPT_CDMA_RDATA1_BASE2,
			GPT_CDMA_RDATA2_BASE2, GPT_CDMA_RDATA3_BASE2, len, 0x10, 0);
	gpt_cdma_wr_setx(2, len, 0x10, stride, GPT_CDMA_WDATA0_BASE2);
	gpt_cdma_setn_op(2, GPT_OP_RB_NUM0 |
			GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE);

	gpt_cdma_rd_setx(3, GPT_CDMA_RDATA0_BASE3, GPT_CDMA_RDATA1_BASE3,
			GPT_CDMA_RDATA2_BASE3, GPT_CDMA_RDATA3_BASE3, len, 0x10, 0);
	gpt_cdma_wr_setx(3, len, 0x10, stride, GPT_CDMA_WDATA0_BASE3);
	gpt_cdma_setn_op(3, GPT_OP_RB_NUM0 |
			GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1d_to_2d_pad_virtual(uint64_t mode)
{
	int len = 0x10000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len , row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1d_to_2d_fill_virtual(void)
{
	int len = 0x10000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_FILL | GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len , row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(1);
	gpt_cdma_wait_done();

	return 0;
}


int gpt_cdma_mul_1d_to_2d_virtual(int nset, int nbase)
{
	int len = 0x4000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_xset_xbase(0, nbase, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len , row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * nbase / row, row);

	gpt_cdma_xset_xbase(1, nbase, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * nbase / row, row);

	gpt_cdma_xset_xbase(2, nbase, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * nbase / row, row);

	gpt_cdma_xset_xbase(3, nbase, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * nbase / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1d_to_2d_pad(int nset)
{
	int len = 0x4000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_ZERO_PAD |
		GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0 | GPT_OP_SZ_1B | GPT_OP_CN_4B;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len , row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1d_to_2d_fill(int nset)
{
	int len = 0x8000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_FILL |
		GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1d_to_2d(int nset, int nbase)
{
	int len = 0x4000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;
/*
	int nbase = 0x4;
*/
	uint64_t mode = GPT_OP_TY_COPY |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_xset_xbase(0, nbase, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * nbase / row, row);

	gpt_cdma_xset_xbase(1, nbase, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * nbase / row, row);

	gpt_cdma_xset_xbase(2, nbase, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * nbase / row, row);

	gpt_cdma_xset_xbase(3, nbase, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * nbase / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1d_to_2d(void)
{
	int len = 0xf800;
	int stride = 0x80;
	gpt_cdma_rd_setx(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA1_BASE0,
			GPT_CDMA_RDATA2_BASE0, GPT_CDMA_RDATA3_BASE0, len, 0, 0);
	gpt_cdma_wr_setx(0, len / 0x10, 0x10, stride, GPT_CDMA_WDATA0_BASE0);
	gpt_cdma_setn_op(0, GPT_OP_RB_NUM0 | GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE);

	gpt_cdma_rd_setx(1, GPT_CDMA_RDATA0_BASE1, GPT_CDMA_RDATA1_BASE1,
			GPT_CDMA_RDATA2_BASE1, GPT_CDMA_RDATA3_BASE1, len, 0, 0);
	gpt_cdma_wr_setx(1, len / 0x10, 0x10, stride, GPT_CDMA_WDATA0_BASE1);
	gpt_cdma_setn_op(1, GPT_OP_RB_NUM0 | GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE);

	gpt_cdma_rd_setx(2, GPT_CDMA_RDATA0_BASE2, GPT_CDMA_RDATA1_BASE2,
			GPT_CDMA_RDATA2_BASE2, GPT_CDMA_RDATA3_BASE2, len, 0, 0);
	gpt_cdma_wr_setx(2, len / 0x10, 0x10, stride, GPT_CDMA_WDATA0_BASE2);
	gpt_cdma_setn_op(2, GPT_OP_RB_NUM0 | GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE);

	gpt_cdma_rd_setx(3, GPT_CDMA_RDATA0_BASE3, GPT_CDMA_RDATA1_BASE3,
			GPT_CDMA_RDATA2_BASE3, GPT_CDMA_RDATA3_BASE3, len, 0, 0);
	gpt_cdma_wr_setx(3, len / 0x10, 0x10, stride, GPT_CDMA_WDATA0_BASE3);
	gpt_cdma_setn_op(3, GPT_OP_RB_NUM0 | GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_set_fill_virtul(int nset, int type, int set1_len,
		int set2_len, int set3_len, int set4_len)
{
	int timeout = 0xfff;
	int status;
	uint64_t reg = 0;
	if (nset > 4) {
		printf("count of sets must less than 5\n");
		return -1;
	}

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_setx_config(0, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, GPT_CDMA_WDATA0_V_BASE0, set1_len);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_setx_config(1, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, GPT_CDMA_WDATA0_BASE1, set2_len);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_setx_config(2, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, GPT_CDMA_WDATA0_BASE2, set3_len);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_setx_config(3, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, GPT_CDMA_WDATA0_BASE3, set4_len);

	printf("wait for idle\n");
	switch (nset) {
	case 4:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 3:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 2:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 1:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}
	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		return -1;

	return 0;
}

int gpt_cdma_mul_set_fill(int nset, int type, int set1_len,
		int set2_len, int set3_len, int set4_len)
{
	int timeout = 0xfff;
	int status;
	uint64_t reg = 0;
	if (nset > 4) {
		printf("count of sets must less than 5\n");
		return -1;
	}

//	printf("wait for idle\n");

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_setx_config(0, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, GPT_CDMA_WDATA0_BASE0, set1_len);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_setx_config(1, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, GPT_CDMA_WDATA0_BASE1, set2_len);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_setx_config(2, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, GPT_CDMA_WDATA0_BASE2, set3_len);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_setx_config(3, 1, GPT_OP_TY_FILL, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, GPT_CDMA_WDATA0_BASE3, set4_len);

//	printf("wait for idle\n");
	switch (nset) {
	case 4:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 3:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 2:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 1:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}
	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

//	printf("wait for idle\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		return -1;

	return 0;
}

int gpt_cdma_mul_set_pad_virtual(int nset, int type, int set1_len,
		int set2_len, int set3_len, int set4_len)
{
	int timeout = 0xfffff;
	int status;
	uint64_t reg = 0;
	int nbase = 1;
	if (nset > 4) {
		printf("count of sets must less than 5\n");
		return -1;
	}

	gpt_cdma_setx_config(0, nbase, type, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, GPT_CDMA_WDATA0_V_BASE0, set1_len);

	gpt_cdma_setx_config(1, nbase, type, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, GPT_CDMA_WDATA0_V_BASE1, set2_len);

	gpt_cdma_setx_config(2, nbase, type, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, GPT_CDMA_WDATA0_V_BASE2, set3_len);

	gpt_cdma_setx_config(3, nbase, type, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, GPT_CDMA_WDATA0_V_BASE3, set4_len);

	switch (nset) {
	case 4:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 3:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 2:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 1:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}
	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

	printf("wait for idle\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		return -1;

	return 0;
}

int gpt_cdma_mul_set_pad(int nset, int type, int set1_len,
		int set2_len, int set3_len, int set4_len)
{
	int timeout = 0xfff;
	int status;
	uint64_t reg = 0;
	if (nset > 4) {
		printf("count of sets must less than 5\n");
		return -1;
	}
	gpt_cdma_setx_config(0, 1, type, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, GPT_CDMA_WDATA0_BASE0, set1_len);

	gpt_cdma_setx_config(1, 1, type, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, GPT_CDMA_WDATA0_BASE1, set2_len);

	gpt_cdma_setx_config(2, 1, type, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, GPT_CDMA_WDATA0_BASE2, set3_len);

	gpt_cdma_setx_config(3, 1, type, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, GPT_CDMA_WDATA0_BASE3, set4_len);

	switch (nset) {
	case 4:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 3:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 2:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 1:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}
	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

	printf("wait for idle\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		return -1;

	return 0;
}

int gpt_cdma_mul_set_tlb(int nset, int set1_len,
		int set2_len, int set3_len, int set4_len)
{
	int timeout = 0xfffff;
	int status;
	uint64_t reg = 0;
	int nbase = 0x1;
	if (nset > 4) {
		printf("count of sets must less than 5\n");
		return -1;
	}
	gpt_cdma_setx_config(0, nbase, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, GPT_CDMA_WDATA0_BASE0, set1_len);

	gpt_cdma_setx_config(1, nbase, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, GPT_CDMA_WDATA0_BASE1, set2_len);

	gpt_cdma_setx_config(2, nbase, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, GPT_CDMA_WDATA0_BASE2, set3_len);

	gpt_cdma_setx_config(3, nbase, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, GPT_CDMA_WDATA0_BASE3, set4_len);

	switch (nset) {
	case 4:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 3:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 2:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 1:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}
	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

	printf("wait for idle\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		return -1;

	return 0;
}


int gpt_cdma_mul_set(int nset, int set1_len,
		int set2_len, int set3_len, int set4_len)
{
	int timeout = 0xfff;
	int status;
	uint64_t reg = 0;
	if (nset > 4) {
		printf("count of sets must less than 5\n");
		return -1;
	}
	gpt_cdma_setx_config(0, 4, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, GPT_CDMA_WDATA0_BASE0, set1_len);

	gpt_cdma_setx_config(1, 4, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, GPT_CDMA_WDATA0_BASE1, set2_len);

	gpt_cdma_setx_config(2, 4, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, GPT_CDMA_WDATA0_BASE2, set3_len);

	gpt_cdma_setx_config(3, 4, GPT_OP_TY_COPY, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, GPT_CDMA_WDATA0_BASE3, set4_len);

	switch (nset) {
	case 4:
		reg |= (GPT_CMD_SET3_EN | GPT_CMD_SET3);
	case 3:
		reg |= (GPT_CMD_SET2_EN | GPT_CMD_SET2);
	case 2:
		reg |= (GPT_CMD_SET1_EN | GPT_CMD_SET1);
	case 1:
		reg |= (GPT_CMD_SET0_EN | GPT_CMD_SET0);
		break;
	}
	reg |= GPT_CMD_ACTIVE;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD), reg);

	printf("wait for idle\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		return -1;

	return 0;
}

void gpt_cdma_copy(unsigned char *src, unsigned char *dst, int len)
{
	int timeout = 0x10000;
	int status;
	uint64_t srcn = (uint64_t)src;
	uint64_t dstn = (uint64_t)dst;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDBASE0_SET0),
				srcn);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDCOUNT0_SET0), len - 1);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRBASE_SET0),
				dstn);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRCOUNT0_SET0), len - 1);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_OP_SET0), 0x0);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_CMD),
				GPT_CMD_SET0_EN | GPT_CMD_ACTIVE);
	printf("cdma wait for status\n");
	do {
		status = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_STATUS));
		timeout--;
	} while ((!(status & GPT_STATUS_D)) && (timeout > 0));

	if (timeout <= 0)
		printf("cdma copy write timeout\n");
}

static unsigned int gpt_data_random(unsigned char *buffer, int len)
{
	int i;
	unsigned int randr = get_timer(0);
	if (buffer != NULL) {
		for (i = 0; i < len; i++)
			buffer[i]  = (randr++) & 0xff;
	}
	printf("random data is %x\n", randr);
	return randr;
}

int gpt_cdma_data_prepare(int type)
{
	switch (type) {
	case 0:
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		break;
	case 1:
		break;
	default:
		printf("Unsupport data format for dma\n");
	}

	return 0;
}


int gpt_cdma_mul_data_check_1_2d( int nbase,  unsigned char *src0,
			unsigned char *src1, unsigned char *src2,
			unsigned char *src3, int wrows, int wcols,
			int wstr, unsigned char *dst)
{
	int i,j;
	int rindex = 0;
	int windex = 0;
	unsigned char *buff;

	for (i = 0; i < wrows; i++) {
		for (j = 0; j < wcols; j++) {
			switch(rindex % nbase) {
			case 0:
				buff =  src0;
				break;
			case 1:
				buff =  src1;
				break;
			case 2:
				buff =  src2;
				break;
			case 3:
				buff =  src3;
				break;
			default:
				buff = NULL;
				printf("Unsupport type for check mode\n");
				return -1;
			}

			if (buff[rindex++] != dst[windex++])
				return -1;
		}
		windex += wstr;
	}

	return 0;
}

int gpt_cdma_mul_data_check_nd( int nbase, int rrows, int rcols, int rstr,
			unsigned char *src0, unsigned char *src1, unsigned char *src2,
			unsigned char *src3, int wrows, int wcolumns, unsigned char *dst)
{
	return 0;
}

int gpt_cdma_mul_data_check(unsigned char *src, int channel,
		int length, unsigned char *dst)
{
	int i;
	int len =length *channel;

	for (i = 0; i < len; i++) {

		if (src[i % channel] != dst[i]) {
			printf("offset:%d, %x ---> %x\n", i, src[i], dst[i]);
			return -1;
		}

	}
	printf("data check success\n");
	return 0;
}

int gpt_cdma_data_check(unsigned char *src, unsigned char *dst, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		if (src[i] != dst[i]) {
			printf("offset:%d, %x ---> %x\n", i, src[i], dst[i]);
			return -1;
		}
	}
	printf("data check success\n");
	return 0;
}

void gpt_cdma_regs(void)
{
	int i;
	uint64_t *reg_t;

	reg_t = (uint64_t *)(GPT_CPU_DMA_BASE + 0x8000);
	if (reg_t == NULL) {
		printf("please init cmda host first");
		return;
	}

	printf("CDMA regs:\n");
	for (i = 0; i < 24; i++)
		printf("%.8llx%c", reg_t[i], i % 4 == 3 ? '\n' : ' ');
}

void gpt_cdma_virt_en(void)
{
	uint64_t regs;

	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	regs |= GPT_SCTL_VIRT;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_SCTL), regs);
	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	printf("enable  virtual address, status: %llx\n", regs);
}

void gpt_cdma_virt_dis(void)
{
	uint64_t regs;

	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	regs &= (~GPT_SCTL_VIRT);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_SCTL), regs);
}

/*
 * file read tlb items
 * real: real address for tlb
 * virt: virtual address for dma
 * mask: define offset of tlb
 * item: witch
 */
int gpt_cdma_tlb_r(int item, uint64_t real_base, uint64_t virt, uint64_t mask)
{
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDREAL_TLB(item)), real_base);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDMASK_TLB(item)),mask);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_RDTAG_TLB(item)), virt & (~mask));

	return 0;
}

/*
 * fill write tlb items
 * real: real base address for tlb
 * virt: virtual address for dma
 * mask: define offset of tlb
 * item: witch
 */
int gpt_cdma_tlb_w(int item, uint64_t real_base, uint64_t virt, uint64_t mask)
{
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRREAL_TLB(item)), real_base);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRMASK_TLB(item)), mask);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WRTAG_TLB(item)), virt & (~mask));
	return 0;
}

int gpt_cdma_init(void)
{
	cdma_ctrl = gpt_cdma_ctl_map();
	if (cdma_ctrl == 0) {
		printf("CDMA I/O address map failed\n");
		return -1;
	}
	gpt_cdma_get_spr();
	printf("CDMA I/O address is 0x%p\n", cdma_ctrl);
	gpt_cdma_reset();

	return 0;
}

void gpt_data_init(int type)
{
	switch (type) {
	case 0:
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x0, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE1, 0x0, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE2, 0x0, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE3, 0x0, 0x40000);
		break;

	case 1:
		memset((void *)GPT_CDMA_RDATA0_BASE0, 0x01, 0x10000);
		memset((void *)GPT_CDMA_RDATA1_BASE0, 0x02, 0x10000);
		memset((void *)GPT_CDMA_RDATA2_BASE0, 0x03, 0x10000);
		memset((void *)GPT_CDMA_RDATA3_BASE0, 0x04, 0x10000);

		memset((void *)GPT_CDMA_RDATA0_BASE1, 0x05, 0x10000);
		memset((void *)GPT_CDMA_RDATA1_BASE1, 0x06, 0x10000);
		memset((void *)GPT_CDMA_RDATA2_BASE1, 0x07, 0x10000);
		memset((void *)GPT_CDMA_RDATA3_BASE1, 0x08, 0x10000);

		memset((void *)GPT_CDMA_RDATA0_BASE2, 0x09, 0x10000);
		memset((void *)GPT_CDMA_RDATA1_BASE2, 0x0a, 0x10000);
		memset((void *)GPT_CDMA_RDATA2_BASE2, 0x0b, 0x10000);
		memset((void *)GPT_CDMA_RDATA3_BASE2, 0x0c, 0x10000);

		memset((void *)GPT_CDMA_RDATA0_BASE3, 0x0d, 0x10000);
		memset((void *)GPT_CDMA_RDATA1_BASE3, 0x0e, 0x10000);
		memset((void *)GPT_CDMA_RDATA2_BASE3, 0x0f, 0x10000);
		memset((void *)GPT_CDMA_RDATA3_BASE3, 0x10, 0x10000);

		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x00, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE1, 0x00, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE2, 0x00, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE3, 0x00, 0x40000);
		break;
	case 2:
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA1_BASE0, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA2_BASE0, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA3_BASE0, 0x100000);

		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE1, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA1_BASE1, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA2_BASE1, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA3_BASE1, 0x100000);

		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE2, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA1_BASE2, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA2_BASE2, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA3_BASE2, 0x100000);

		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE3, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA1_BASE3, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA2_BASE3, 0x100000);
		gpt_data_random((void *)GPT_CDMA_RDATA3_BASE3, 0x100000);

		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x0, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE1, 0x0, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE2, 0x0, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE3, 0x0, 0x40000);
		break;
	case 3:
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x5e, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE1, 0x5e, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE2, 0x5e, 0x40000);
		memset((void *)GPT_CDMA_WDATA0_BASE3, 0x5e, 0x40000);
		break;

	case 4:
		memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5a, 0x200000);
		memset((void *)GPT_CDMA_RDATA0_BASE1, 0x5b, 0x200000);
		memset((void *)GPT_CDMA_RDATA0_BASE2, 0x5c, 0x200000);
		memset((void *)GPT_CDMA_RDATA0_BASE3, 0x5d, 0x200000);
		break;

	case 5:
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x40000);
		break;

	default:
		printf("Unsupport data format\n");
	}

}

void gpt_cdma_rl2_en(void)
{
	uint64_t regs = 0;

	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	regs |= GPT_SCTL_RL2;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_SCTL), regs);
	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	printf("%s: sctl status: %llx\n", __func__, regs);

}

void gpt_cdma_rl2_dis(void)
{
	uint64_t regs = 0;

	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	regs &= ~GPT_SCTL_RL2;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_SCTL), regs);
	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	printf("%s: sctl status: %llx\n", __func__, regs);
}

void gpt_cdma_nosync_en(void)
{
	uint64_t regs = 0;

	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	regs |= GPT_SCTL_SYNC;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_SCTL), regs);
	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	printf("%s: sctl status: %llx\n", __func__, regs);

}

void gpt_cdma_nosync_dis(void)
{
	uint64_t regs = 0;

	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	regs &= ~GPT_SCTL_SYNC;
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_SCTL), regs);
	regs = gpt_cdma_read64(GPT_DMA_REG(GPT_CDMA_SCTL));
	printf("%s: sctl status: %llx\n", __func__, regs);
}

void gpt_cdma_pxi_test(int type)
{
	int loop_ra = 0;
	int loop_l;
	int loop_cnt = 0;
	int ret = 0;
	switch (type) {
	case 0: /*loop pxi address test case*/
		for (loop_ra = 0; loop_ra < 64; loop_ra++) {
			printf("pxi read offset: %x\n", loop_ra);
			for (loop_l = 1; loop_l <= 64; loop_l++) {
				gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0 + loop_ra, 0x100);
				gpt_cdma_copy_mul(1, GPT_CDMA_RDATA0_BASE0 + loop_ra,
					0, 0,
					0, GPT_PXI_BASE + loop_ra, loop_l);

				gpt_cdma_copy_mul(1, GPT_PXI_BASE + loop_ra,
					0, 0,
					0, GPT_CDMA_WDATA0_BASE0 + loop_ra, loop_l);
				ret = gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0 + loop_ra,
						(void *)GPT_CDMA_WDATA0_BASE0 + loop_ra, loop_l);
				if (ret)
					loop_cnt++;
			}
		}
		printf("extbus: total: %d, success: %d, failed: %d\n", 64*64, 64*64 - loop_cnt, loop_cnt);
		break;

	case 1: /* mem->pxi, pxi->pxi, pxi->mem */
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x0,  0x8000);
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x300);
		gpt_cdma_copy_mul(1, GPT_CDMA_RDATA0_BASE0,
			0, 0,
			0, GPT_PXI_BASE, 0x100);

		gpt_cdma_copy_mul(1, GPT_PXI_BASE,
			0, 0,
			0, GPT_PXI_BASE + 0x100, 0x100);

		gpt_cdma_copy_mul(1, GPT_PXI_BASE,
			0, 0,
			0, GPT_CDMA_WDATA0_BASE0, 0x200);
		ret = gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0,
					(void *)GPT_CDMA_WDATA0_BASE0, 0x100);

		ret = gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0,
					(void *)GPT_CDMA_WDATA0_BASE0 + 0x100, 0x100);
		break;

	case 2: /* mem->pxi, pxi->mem */
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x0,  0x4000);
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x300);
		gpt_cdma_copy_mul(1, GPT_CDMA_RDATA0_BASE0,
			0, 0,
			0, GPT_PXI_BASE, 0x200);

		gpt_cdma_copy_mul(1, GPT_PXI_BASE,
			0, 0,
			0, GPT_CDMA_WDATA0_BASE0, 0x200);
		ret = gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0,
					(void *)GPT_CDMA_WDATA0_BASE0, 0x200);
		break;
	}
}

void gpt_cdma_ext2axi_test(void)
{

	int loop_ra = 0;
	int loop_l;
	int loop_cnt = 0;
	int ret = 0;

	for (loop_ra = 0; loop_ra < 64; loop_ra++) {
		for (loop_l = 0; loop_l < 64; loop_l++) {
			gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
			gpt_cdma_copy_mul(1, GPT_CDMA_RDATA0_BASE0 + loop_ra,
				GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
				GPT_CDMA_RDATA3_BASE0, GPT_CDMA_WDATA0_BASE0 + loop_ra, 0x500 + loop_l);
			ret = gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0 + loop_ra,
					(void *)GPT_CDMA_WDATA0_BASE0 + loop_ra, 0x500 + loop_l);
			if (ret)
				loop_cnt++;
			udelay(100 * 1000);
		}
	}
	printf("extbus: total: %d, success: %d, failed: %d\n", 64*64*64, 64*64*64 - loop_cnt, loop_cnt);
}

int gpt_cdma_pxi_2dto2d_fill(void)
{

	int len = 0x800;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x8;
	int basen = 0x1;

	uint64_t mode = GPT_OP_TY_FILL | GPT_OP_RB_NUM0 |
			GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(0);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_pxi_2dto2d_copy_pad0(void)
{

	int len = 0x80;
	int rstr = 0x80;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;

	uint64_t mode = GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B |
			GPT_OP_RB_NUM0 | GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(1);
	gpt_cdma_wait_done();

	return 0;
}


int gpt_cdma_pxi_2dto2d_copy_pad(void)
{

	int len = 0x80;
	int rstr = 0x80;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x1;

	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B |
			GPT_OP_RB_NUM0 | GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_pxi_2dto2d_copy(void)
{

	int len = 0x80;
	int rstr = 0x80;
	int dstr = 0x80;
	int row = 0x10;
	int basen = 0x2;

	uint64_t mode = GPT_OP_TY_COPY | GPT_OP_RB_NUM1 |
			GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
		//	len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen, row);

	gpt_cdma_cmd_enable(1);
	gpt_cdma_wait_done();

	return 0;
}


int gpt_cdma_pxi_1dto2d_copy(void)
{
#if 1
	int len = 0x100;
	int rstr = 0x0;
	int dstr = 0x10;
	int row = 0x10;
	int basen = 0x4;
#else
	int len = 0x100;
	int rstr = 0x0;
	int dstr = 0x20;
	int row = 0x10;
	int basen = 0x4;

#endif
	uint64_t mode = GPT_OP_TY_COPY | GPT_OP_RB_NUM3 | GPT_OP_WR_MULTIPLE;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
//			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(1), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_pxi_1dto1d_copy_pad0(void)
{
	int len = 0x800;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B | GPT_OP_RB_NUM0;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(1), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_pxi_1dto1d_fill(void)
{
	int len = 0x1000;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_FILL | GPT_OP_SZ_1B | GPT_OP_CN_4B | GPT_OP_RB_NUM0;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(1), dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_pxi_1dto1d_copy_pad(void)
{
	int len = 0x800;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B | GPT_OP_RB_NUM0;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(1), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto1d_copy(void)
{
	int len = 0x100;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x4;
	uint64_t mode = GPT_OP_TY_COPY | GPT_OP_RB_NUM3;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_PXI_WDATA0_BASE(0), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_PXI_WDATA0_BASE(1), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_PXI_WDATA0_BASE(2), dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_PXI_WDATA0_BASE(3), dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(3);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto2d_m_copy(int nset, int basen)
{
	int len = 0x4000;
	int rstr = 0x0;
	int dstr = 0x80;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY | GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(basen);

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto1d_m_copy_pad(int nset)
{
	int len = 0x10000;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B | GPT_OP_RB_NUM0;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto1d_m_pad0(int nset)
{
	int len = 0x10000;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B | GPT_OP_RB_NUM0;

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto1d_m_fill(int nset)
{
	int len = 0x4000;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int basen = 0x1;
	uint64_t mode = GPT_OP_TY_FILL | GPT_OP_SZ_1B | GPT_OP_CN_4B | GPT_OP_RB_NUM0;

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);
	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto1d_m_copy(int nset, int basen)
{
	int len = 0x4000;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_COPY | gpt_cdma_nbase_cfg(basen);

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_mul_1dto1d_m_copy_v(int nset, int basen)
{
	int len = gpt_cdma_len_cfg(basen);
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_COPY | gpt_cdma_nbase_cfg(basen);

	gpt_cdma_xset_xbase(0, basen, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE0, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(1, basen, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE1, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(2, basen, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE2, dstr,
			len * basen / row, row);

	gpt_cdma_xset_xbase(3, basen, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			len, row, GPT_CDMA_WDATA0_V_BASE3, dstr,
			len * basen / row, row);

	gpt_cdma_cmd_enable(nset - 1);
	gpt_cdma_wait_done();

	return 0;
}

unsigned char *buff_l2[0x400];

void gpt_l2_sync(uint64_t addr, int size)
{
	int i;
	int count = size / 64;
	if (size % 64)
		count++;
	for (i = 0; i < count; i++) {
		__asm__ volatile (
			"dctl_sync_l2 %0\n\t"
			:
			: "a"(addr + i * 64)
			:
		);
	}
}

void gpt_l2_flush(uint64_t addr, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		__asm__ volatile (
			"dctl_flush_l2 %0\n\t"
			:
			: "a"(addr + i)
			:
		);
	}
}

void gpt_l2_invalid(uint64_t addr, int size)
{
	int i;
	int count = size / 64;
	if (size % 64)
		count++;

	for (i = 0; i < count; i++) {
		__asm__ volatile (
			"dctl_invalid_l2 %0\n\t"
			:
			: "a"(addr + i * 64)
			:
		);
	}
}

void gpt_l1_invalid(uint64_t addr, int size)
{
	int i;
	int count = size / 64;
	if (size % 64)
		count++;

	for (i = 0; i < count; i++) {
		__asm__ volatile (
			"dctl_invalid_l1 %0\n\t"
			:
			: "a"(addr + i * 64)
			:
		);
	}
}

void gpt_l1_sync(uint64_t addr, int size)
{
	int i;
	int count = size / 64;
	if (size % 64)
		count++;
	for (i = 0; i < count; i++) {
		__asm__ volatile (
			"dctl_sync_l1 %0\n\t"
			:
			: "a"(addr + i * 64)
			:
		);
	}
}

int gpt_cdma_pad_ck(int cnt, uint8_t *src, uint8_t *dst)
{
	int i;

	for (i = 0; i < cnt; i++)
		if (src[i] != dst[i]) {
			printf("pading check failed 0x%x: %x->%x \n",
						i, src[i], dst[i]);
//			return -1;
		}
	return 0;
}


int gpt_cdma_2dto2d_pad_ck(int nset, int nbase, int cols,
			int rows, int stride, int rstride, int type, int psize)
{
	int ret = 0;
	int i;
	uint8_t pdata[0x80];
	uint8_t *num;

	switch (nset) {
	case 4:
		printf("check set4 pad data....\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE3 + i * (nbase * cols + rstride),
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE3
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);
			if (type) {
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize);
#ifdef GPT_CDMA_DEBUG

				printf("waddress 0x%x, raddress: 0x%x\n", GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + stride + nbase * cols),
					GPT_CDMA_RDATA0_BASE3 + i * (nbase * cols + rstride));
				gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + stride + nbase * cols)
					, 0x100);
#endif

				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}
	case 3:
		printf("check set3 pad data....\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE2 + i * (nbase *cols + rstride),
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE2
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);

#ifdef GPT_CDMA_DEBUG
			printf("waddress 0x%x, raddress: 0x%x\n", GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + stride + nbase * cols),
					GPT_CDMA_RDATA0_BASE2 + i * (nbase * cols + rstride));
			gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + stride + nbase * cols)
					, 0x100);
#endif
			if (type) {

				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride) + psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}

	case 2:
		printf("check set2 pad data....\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE1 + i * (nbase *cols + rstride),
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE1
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);
#ifdef GPT_CDMA_DEBUG
			printf("waddress 0x%x, raddress: 0x%x\n", GPT_CDMA_WDATA0_BASE1
				+ i * (psize * 2 + stride + nbase * cols),
				GPT_CDMA_RDATA0_BASE1 + i * (nbase * cols + rstride));
			gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE1
				+ i * (psize * 2 + stride + nbase * cols)
				, 0x100);
#endif
			if (type) {
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride) + psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}

	case 1:
		printf("check set1 pad data....\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);

			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE0 + i * (nbase * cols + rstride),
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE0
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);

#ifdef GPT_CDMA_DEBUG
			printf("waddress 0x%x, raddress: 0x%x\n", GPT_CDMA_WDATA0_BASE0
				+ i * (psize * 2 + stride + nbase * cols),
				GPT_CDMA_RDATA0_BASE0 + i * (nbase * cols + rstride));
			gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0
				+ i * (psize * 2 + nbase * cols + stride)
				, 0x100);
#endif

			if (type) {
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE0
					+ i * (psize * 2 + nbase * cols + stride) + psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0 +
					i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE0 +
					i * (psize * 2 + nbase * cols + stride) +
					psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0 +
					i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}

		break;
	}

	return ret;
}

int gpt_cdma_1dto2d_pad_ck(int nset, int nbase, int cols,
			int rows, int stride, int type, int psize)
{
	int ret = 0;
	int i;
	uint8_t pdata[0x80];
	uint8_t *num;

	switch (nset) {
	case 4:
		printf("check set4 data...\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE3 + i * cols,
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE3
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);
			if (type) {
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE3
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}
	case 3:
		printf("check set3 data...\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE2 + i * cols,
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE2
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);
			if (type) {
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride) + psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(uint8_t *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE2
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}

	case 2:
		printf("check set2 data...\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE1 + i * cols,
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE1
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);
			if (type) {
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride) + psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE1
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}

	case 1:
		printf("check set1 data...\n");
		for (i = 0; i < rows; i++) {
			printf("row:%x\n", i);
			ret = gpt_cdma_1dto1d_1set_data_ck(nbase,
				(void *)GPT_CDMA_RDATA0_BASE0 + i * cols,
				NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE0
				+ i * (psize * 2 + stride + nbase * cols) + psize,
				cols * nbase);
			if (type) {
				num = (uint8_t *)((long)GPT_CDMA_WDATA0_BASE0
					+ i * (psize * 2 + nbase * cols + stride) + psize);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0 +
					i * (psize * 2 + nbase * cols + stride));
				num = (uint8_t *)((long)GPT_CDMA_WDATA0_BASE0 +
					i * (psize * 2 + nbase * cols + stride) +
					psize + cols * nbase);
				memset(pdata, *num, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0 +
					i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			} else {
				memset(pdata, 0, 0x50);
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0
					+ i * (psize * 2 + nbase * cols + stride));
				gpt_cdma_pad_ck(psize, pdata,
					(void *)GPT_CDMA_WDATA0_BASE0
					+ i * (psize * 2 + nbase * cols + stride)
					+ psize + cols * nbase);
			}
		}

		break;
	}

	return ret;
}

int gpt_cdma_1dto1d_pad_ck(int nset, int nbase, int len, int type, int psize)
{
	int ret = -1;
	uint8_t pdata[0x80];
	uint8_t *num;

	switch (nset) {
	case 4:
		printf("check set4 data....\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE3,
			NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE3 + psize,
			len * nbase);
		if (type) {
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE3 + psize);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE3);
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE3 + psize + len * nbase);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE3 + psize + len * nbase);
		} else {
			memset(pdata, 0, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE3);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE3 + psize + len * nbase);
		}
	case 3:
		printf("check set3 data....\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE2,
			NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE2 + psize,
			len * nbase);
		if (type) {
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE2 + psize);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE2);
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE2 + psize + len * nbase);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE2 + psize + len * nbase);
		} else {
			memset(pdata, 0, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE2);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE2 + psize + len * nbase);
		}
	case 2:
		printf("check set2 data....\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE1,
			NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE1 + psize,
			len * nbase);
		if (type) {
			num= (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE1 + psize);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE1);
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE1 + psize + len * nbase);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE1 + psize + len * nbase);

		} else {
			memset(pdata, 0, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE1);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE1 + psize + len * nbase);
		}
	case 1:
		printf("check set1 data....\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE0,
			NULL, NULL, NULL, (void *)GPT_CDMA_WDATA0_BASE0 + psize,
			len * nbase);
		if (type) {
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE0 + psize);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE0);
			num = (uint8_t *)((void *)GPT_CDMA_WDATA0_BASE0 + psize + len * nbase);
			memset(pdata, *num, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE0 + psize + len * nbase);
		} else {
			memset(pdata, 0, 0x50);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE0);
			gpt_cdma_pad_ck(psize, pdata, (void *)GPT_CDMA_WDATA0_BASE0 + psize + len * nbase);
		}
		break;
	}

	return ret;
}

int gpt_cdma_2dto2d_1set_data_ck(int nbase, uint8_t *src0,
		uint8_t *src1, uint8_t *src2, uint8_t *src3, int len,
		uint8_t *dst, int cols, int rows, int wstr, int rstr)
{
	int i, j;
	int rindex = 0;
	int windex = 0;

	uint8_t *addr[10] = {src0, src1, src2, src3};
	uint8_t *rbuff = NULL;
	uint8_t rd, wd;

#ifndef GPT_CDMA_DEBUG
	printf("nbase: %d, rows: %x, columns: %x\n", nbase, rows, cols);
#else
	printf("%c\n", '.');
#endif
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {

			rbuff = addr[rindex % nbase];
			rd = rbuff[rindex++ / nbase];
			wd = dst[windex++];
		#ifdef GPT_CDMA_DEBUG
			printf("address: %p, %.2x <----> %.2x\n", rbuff + (rindex -1) / nbase, rd, wd);
		#endif
			if (rd != wd) {
				printf("check failed r-w: 0x%x-->0x%x, 0x%x-->0x%x\n",
					rindex, windex, rd, wd);
				return -1;
			}
		}
		windex += wstr;
		rindex += rstr * nbase;
	}

	return 0;
}

int gpt_cdma_1dto2d_1set_data_ck(int nbase, uint8_t *src0,
		uint8_t *src1, uint8_t *src2, uint8_t *src3, int len,
		uint8_t *dst, int cols, int rows, int str)
{
	int i, j;
	int rindex = 0;
	int windex = 0;

	uint8_t *addr[10] = {src0, src1, src2, src3};
	uint8_t *rbuff = NULL;
	uint8_t rd, wd;

#ifndef GPT_CDMA_DEBUG
	printf("nbase: %d, rows: %x, columns: %x\n", nbase, rows, cols);
#else
	printf("%c\n", '.');
#endif
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {

			rbuff = addr[rindex % nbase];
			rd = rbuff[rindex++ / nbase];
			wd = dst[windex++];
		#ifdef GPT_CDMA_DEBUG
			printf("address: %p, %.2x <----> %.2x\n", rbuff + (rindex -1) / nbase, rd, wd);
		#endif
			if (rd != wd) {
				printf("check failed r-w: 0x%x-->0x%x, 0x%x-->0x%x\n",
					rindex, windex, rd, wd);
				return -1;
			}
		}
		windex += str;
	}

	return 0;
}

int gpt_cdma_1dto1d_1set_data_ck(int nbase, uint8_t *src0, uint8_t *src1,
			uint8_t *src2, uint8_t *src3, uint8_t *dst, int len)
{
	int j;
	int rindex = 0;
	int windex = 0;
	
	uint8_t *addr[10] = {src0, src1, src2, src3};
	uint8_t *rbuff = NULL;
	uint8_t rd, wd;

#ifdef GPT_CDMA_DEBUG
	printf("nbase: %d, rows: %x, columns\n", nbase, len);
#else
	printf("data checking...%c\n", '.');
#endif
	for (j = 0; j < len; j++) {
		rbuff = addr[rindex % nbase];

		rd = rbuff[rindex++ / nbase];
		wd = dst[windex++];
#ifdef GPT_CDMA_DEBUG
		printf("address: %p, %.2x <----> %.2x \n", rbuff + (rindex -1) / nbase, rd, wd);
#endif
		if (rd != wd) {
			printf("check failed r-w: 0x%x-->0x%x, 0x%x-->0x%x\n",
				rindex, windex, rd, wd);
			return -1;
		}

	}
	
	return 0;
}


int gpt_cdma_2dto2d_data_ck(int nset, int nbase, int len)
{
	int ret = 0;
	int rows = 0x10;
	int stride = 0x50;
	int rstr = 0x50;
	switch (nset) {
	case 4:
		printf("check set4 data...\n");
		ret = gpt_cdma_2dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE3,
			(void *)GPT_CDMA_RDATA1_BASE3, (void *)GPT_CDMA_RDATA2_BASE3,
			(void *)GPT_CDMA_RDATA3_BASE3, len, (void *)GPT_CDMA_WDATA0_BASE3,
			len * nbase, rows, stride, rstr);
	case 3:
		printf("check set3 data...\n");
		ret = gpt_cdma_2dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE2,
			(void *)GPT_CDMA_RDATA1_BASE2, (void *)GPT_CDMA_RDATA2_BASE2,
			(void *)GPT_CDMA_RDATA3_BASE2, len, (void *)GPT_CDMA_WDATA0_BASE2,
			len * nbase, rows, stride, rstr);
	case 2:
		printf("check set2 data...\n");
		ret = gpt_cdma_2dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE1,
			(void *)GPT_CDMA_RDATA1_BASE1, (void *)GPT_CDMA_RDATA2_BASE1,
			(void *)GPT_CDMA_RDATA3_BASE1, len, (void *)GPT_CDMA_WDATA0_BASE1,
			len * nbase, rows, stride, rstr);
	case 1:
		printf("check set1 data...\n");
		ret = gpt_cdma_2dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE0,
			(void *)GPT_CDMA_RDATA1_BASE0, (void *)GPT_CDMA_RDATA2_BASE0,
			(void *)GPT_CDMA_RDATA3_BASE0, len, (void *)GPT_CDMA_WDATA0_BASE0,
			len * nbase, rows, stride, rstr);
		break;
	}

	return ret;
}

int gpt_cdma_1dto2d_data_ck(int nset, int nbase, int len)
{
	int ret = 0;
	int rows = 0x10;
	int stride = 0x50;
	switch (nset) {
	case 4:
		printf("check set4 data...\n");
		ret = gpt_cdma_1dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE3,
			(void *)GPT_CDMA_RDATA1_BASE3, (void *)GPT_CDMA_RDATA2_BASE3,
			(void *)GPT_CDMA_RDATA3_BASE3, len, (void *)GPT_CDMA_WDATA0_BASE3,
			len * nbase / rows, rows, stride);
	case 3:
		printf("check set3 data...\n");
		ret = gpt_cdma_1dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE2,
			(void *)GPT_CDMA_RDATA1_BASE2, (void *)GPT_CDMA_RDATA2_BASE2,
			(void *)GPT_CDMA_RDATA3_BASE2, len, (void *)GPT_CDMA_WDATA0_BASE2,
			len * nbase / rows, rows, stride);
	case 2:
		printf("check set2 data...\n");
		ret = gpt_cdma_1dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE1,
			(void *)GPT_CDMA_RDATA1_BASE1, (void *)GPT_CDMA_RDATA2_BASE1,
			(void *)GPT_CDMA_RDATA3_BASE1, len, (void *)GPT_CDMA_WDATA0_BASE1,
			len * nbase / rows, rows, stride);
	case 1:
		printf("check set1 data...\n");
		ret = gpt_cdma_1dto2d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE0,
			(void *)GPT_CDMA_RDATA1_BASE0, (void *)GPT_CDMA_RDATA2_BASE0,
			(void *)GPT_CDMA_RDATA3_BASE0, len, (void *)GPT_CDMA_WDATA0_BASE0,
			len * nbase / rows, rows, stride);
		break;
	}

	return ret;
}



int gpt_cdma_1dto1d_data_ck(int nset, int nbase, int len)
{
	int ret = 0;
	switch (nset) {	
	case 4:
		printf("check 4 set data...\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE3,
			(void *)GPT_CDMA_RDATA1_BASE3, (void *)GPT_CDMA_RDATA2_BASE3,
			(void *)GPT_CDMA_RDATA3_BASE3, (void *)GPT_CDMA_WDATA0_BASE3,
			len * nbase);
	case 3:
		printf("check 3 set data...\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE2,
			(void *)GPT_CDMA_RDATA1_BASE2, (void *)GPT_CDMA_RDATA2_BASE2,
			(void *)GPT_CDMA_RDATA3_BASE2, (void *)GPT_CDMA_WDATA0_BASE2,
			len * nbase);
	case 2:
		printf("check 2 set data...\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE1,
			(void *)GPT_CDMA_RDATA1_BASE1, (void *)GPT_CDMA_RDATA2_BASE1,
			(void *)GPT_CDMA_RDATA3_BASE1, (void *)GPT_CDMA_WDATA0_BASE1,
			len * nbase);
	case 1:
		printf("check 1 set data...\n");
		ret = gpt_cdma_1dto1d_1set_data_ck(nbase, (void *)GPT_CDMA_RDATA0_BASE0,
			(void *)GPT_CDMA_RDATA1_BASE0, (void *)GPT_CDMA_RDATA2_BASE0,
			(void *)GPT_CDMA_RDATA3_BASE0, (void *)GPT_CDMA_WDATA0_BASE0,
			len * nbase);
		break;
	}

	return ret;
}

int gpt_cdma_base_virt_config(int nset, int nbase,
		uint64_t mode, int rcols, int rrows,
		int rstr, int wcols, int wrows, int dstr)
{

	gpt_cdma_xset_xbase(0, nbase, mode, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_V_BASE0, dstr,
			wcols, wrows);

	gpt_cdma_xset_xbase(1, nbase, mode, GPT_CDMA_RDATA0_V_BASE1,
			GPT_CDMA_RDATA1_V_BASE1, GPT_CDMA_RDATA2_V_BASE1,
			GPT_CDMA_RDATA3_V_BASE1, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_V_BASE1, dstr,
			wcols, wrows);

	gpt_cdma_xset_xbase(2, nbase, mode, GPT_CDMA_RDATA0_V_BASE2,
			GPT_CDMA_RDATA1_V_BASE2, GPT_CDMA_RDATA2_V_BASE2,
			GPT_CDMA_RDATA3_V_BASE2, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_V_BASE2, dstr,
			wcols, wrows);

	gpt_cdma_xset_xbase(3, nbase, mode, GPT_CDMA_RDATA0_V_BASE3,
			GPT_CDMA_RDATA1_V_BASE3, GPT_CDMA_RDATA2_V_BASE3,
			GPT_CDMA_RDATA3_V_BASE3, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_V_BASE3, dstr,
			wcols, wrows);

	gpt_cdma_cmd_enable(nset - 1);

	return 0;
}

int gpt_cdma_base_config(int nset, int nbase,
		uint64_t mode, int rcols, int rrows,
		int rstr, int wcols, int wrows, int dstr)
{

	gpt_cdma_xset_xbase(0, nbase, mode, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_BASE0, dstr,
			wcols, wrows);

	gpt_cdma_xset_xbase(1, nbase, mode, GPT_CDMA_RDATA0_BASE1,
			GPT_CDMA_RDATA1_BASE1, GPT_CDMA_RDATA2_BASE1,
			GPT_CDMA_RDATA3_BASE1, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_BASE1, dstr,
			wcols, wrows);

	gpt_cdma_xset_xbase(2, nbase, mode, GPT_CDMA_RDATA0_BASE2,
			GPT_CDMA_RDATA1_BASE2, GPT_CDMA_RDATA2_BASE2,
			GPT_CDMA_RDATA3_BASE2, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_BASE2, dstr,
			wcols, wrows);

	gpt_cdma_xset_xbase(3, nbase, mode, GPT_CDMA_RDATA0_BASE3,
			GPT_CDMA_RDATA1_BASE3, GPT_CDMA_RDATA2_BASE3,
			GPT_CDMA_RDATA3_BASE3, rstr,
			rcols, rrows, GPT_CDMA_WDATA0_BASE3, dstr,
			wcols, wrows);

	gpt_cdma_cmd_enable(nset - 1);

	return 0;
}

int gpt_cdma_1dto1d_mem_real_copy(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_COPY | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto1d_mem_real_fill(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_FILL | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);

	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto1d_mem_real_pad_c(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_COPY_PAD |  GPT_OP_SZ_1B
			| GPT_OP_CN_4B | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto1d_mem_real_pad_0(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_ZERO_PAD |  GPT_OP_SZ_1B
			| GPT_OP_CN_4B | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}



int gpt_cdma_1dto2d_mem_real_copy(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto2d_mem_real_fill(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_FILL |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto2d_mem_real_pad_c(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B
			| GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto2d_mem_real_pad_0(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B
			| GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_real_copy(int nset, int nbase, int len)
{
	int rstr = 0x50;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY | GPT_OP_RD_MULTIPLE |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_real_fill(int nset, int nbase, int len)
{
	int rstr = 0x50;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_FILL | GPT_OP_RD_MULTIPLE |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_real_pad_c(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B |
			GPT_OP_CN_4B | GPT_OP_RD_MULTIPLE |
			GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_real_pad_0(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_ZERO_PAD  | GPT_OP_SZ_1B |
			GPT_OP_CN_4B | GPT_OP_RD_MULTIPLE |
			GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}


int gpt_cdma_1dto1d_mem_virt_copy(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_COPY | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto1d_mem_virt_fill(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_FILL | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);

	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto1d_mem_virt_pad_c(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_COPY_PAD |  GPT_OP_SZ_1B
			| GPT_OP_CN_4B | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto1d_mem_virt_pad_0(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;

	uint64_t mode = GPT_OP_TY_ZERO_PAD |  GPT_OP_SZ_1B
			| GPT_OP_CN_4B | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}


int gpt_cdma_1dto2d_mem_virt_copy(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto2d_mem_virt_fill(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_FILL |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto2d_mem_virt_pad_c(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B
			| GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_1dto2d_mem_virt_pad_0(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B
			| GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len /  row, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_virt_copy(int nset, int nbase, int len)
{
	int rstr = 0x50;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY | GPT_OP_RD_MULTIPLE |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_virt_fill(int nset, int nbase, int len)
{
	int rstr = 0x50;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_FILL | GPT_OP_RD_MULTIPLE |
		GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(0)), 0x5a5a5a5a5a5a5a5a);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(1)), 0x5b5b5b5b5b5b5b5b);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(2)), 0x5c5c5c5c5c5c5c5c);
	gpt_cdma_write64(GPT_DMA_REG(GPT_CDMA_WITH_SET(3)), 0x5d5d5d5d5d5d5d5d);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_virt_pad_c(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_COPY_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B
		| GPT_OP_RD_MULTIPLE | GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

int gpt_cdma_2dto2d_mem_virt_pad_0(int nset, int nbase, int len)
{
	int rstr = 0x0;
	int dstr = 0x50;
	int row = 0x10;

	uint64_t mode = GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B
		| GPT_OP_RD_MULTIPLE | GPT_OP_WR_MULTIPLE | gpt_cdma_nbase_cfg(nbase);

	gpt_cdma_base_virt_config(nset, nbase, mode, len,
			row, rstr, nbase * len, row, dstr);
	gpt_cdma_wait_done();

	return 0;
}

void gpt_cdma_virt_tlb_l2en_config(void)
{
	gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
	gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);

}

void gpt_cdma_virt_tlb_l2en_sync_config(void)
{
	gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xffffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2 | GPT_TLBMASK_SYNC);
	gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xffffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2 | GPT_TLBMASK_SYNC);
}



void gpt_cdma_virt_tlb_config(void)
{
	gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);

	gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
}

int gpt_cdma_copy_test(int type)
{
	int length = 1;
	unsigned int randr;
	unsigned char cmp[5];

	switch (type) {

	case 100: /* cpu-nol2 dma-nol2 1d-1d real copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(1, 1, 0x10000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 101: /* cpu-nol2 dma-nol2 1d-1d real copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(1, 4, 0x4000);
		gpt_cdma_1dto1d_data_ck(1, 4, 0x4000);
		break;

	case 102: /* cpu-nol2 dma-nol2 1d-1d real copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(4, 1, 0x10000);
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 103: /* cpu-nol2 dma-nol2 1d-1d real copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(4, 4, 0x4000);
		gpt_cdma_1dto1d_data_ck(4, 4, 0x4000);
		break;

	case 104: /* cpu-nol2 dma-nol2 1d-1d real fill 4set 1base */
		gpt_data_init(0); //clear write data buffer
		gpt_data_init(4); //prepare read data buffer
		gpt_cdma_1dto1d_mem_real_fill(4, 1, 0x10000);
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 105: /* cpu-nol2 dma-nol2 1d-1d real fill 1set 1base */
		gpt_data_init(0); //clear write data buffer
		gpt_data_init(4); //prepare read data buffer
		gpt_cdma_1dto1d_mem_real_fill(1, 1, 0x10000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 106: /* cpu-nol2 dma-nol2 1d-1d real pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_pad_c(1, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(0x1, 0x1, 0x10000, 0x1, 0x4);
		break;

	case 107: /* cpu-nol2 dma-nol2 1d-1d real pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_pad_c(4, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(0x4, 0x1, 0x10000, 0x1, 0x4);
		break;

	case 108: /* cpu-nol2 dma-nol2 1d-1d real pad-0 1set 1base */
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_cdma_1dto1d_mem_real_pad_0(1, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(0x1, 0x1, 0x10000, 0x0, 0x4);
		break;

	case 109: /* cpu-nol2 dma-nol2 1d-1d real pad-0 4set 1base */
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_cdma_1dto1d_mem_real_pad_0(4, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(0x4, 0x1, 0x10000, 0x0, 0x4);
		break;

	case 110: /* cpu-nol2 dma-nol2 1d-2d real copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_copy(1, 1, 0x10000);
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 111: /* cpu-nol2 dma-nol2 1d-2d real copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_copy(1, 4, 0x4000);
		gpt_cdma_1dto2d_data_ck(1, 4, 0x4000);
		break;

	case 112: /* cpu-nol2 dma-nol2 1d-2d real copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_copy(4, 1, 0x10000);
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 113: /* cpu-nol2 dma-nol2 1d-2d real copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_copy(4, 4, 0x4000);
		gpt_cdma_1dto2d_data_ck(4, 4, 0x4000);
		break;
	/*.................need test.................*/
	case 114: /* cpu-nol2 dma-nol2 1d-2d real fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4); //prepare read data buffer
		memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5a, 0x40000);
		gpt_cdma_1dto2d_mem_real_fill(4, 1, 0x10000);
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 115: /* cpu-nol2 dma-nol2 1d-2d real fill 1set 1base */
		gpt_data_init(0);
		memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5a, 0x40000);
		gpt_cdma_1dto2d_mem_real_fill(1, 1, 0x10000);
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 116: /* cpu-nol2 dma-nol2 1d-2d real pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_pad_c(1, 1, 0x10000);

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
			0x10, 0x50, 1, 0x4);
		break;

	case 117: /* cpu-nol2 dma-nol2 1d-2d real pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_pad_c(4, 1, 0x10000);
		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
			0x10, 0x50, 1, 0x4);
		break;

	case 118: /* cpu-nol2 dma-nol2 1d-2d real pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_pad_0(1, 1, 0x10000);
		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
			0x10, 0x50, 0, 0x4);
		break;

	case 119: /* cpu-nol2 dma-nol2 1d-2d real pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto2d_mem_real_pad_0(4, 1, 0x10000);
		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
			0x10, 0x50, 0, 0x4);
		break;

	case 120: /* cpu-nol2 dma-nol2 2d-2d real copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_copy(1, 1, 0x10000);
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;

	case 121: /* cpu-nol2 dma-nol2 2d-2d real copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_copy(1, 4, 0x4000);
		gpt_cdma_2dto2d_data_ck(1, 4, 0x4000);
		break;

	case 122: /* cpu-nol2 dma-nol2 2d-2d real copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_copy(4, 1, 0x10000);
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;

	case 123: /* cpu-nol2 dma-nol2 2d-2d real copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_copy(4, 4, 0x4000);
		gpt_cdma_2dto2d_data_ck(4, 4, 0x4000);
		break;

	case 124: /* cpu-nol2 dma-nol2 2d-2d real fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4); //prepare read data buffer
		gpt_cdma_2dto2d_mem_real_fill(4, 1, 0x10000);
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;

	case 125: /* cpu-nol2 dma-nol2 2d-2d real fill 1set 1base */
		gpt_data_init(0);
		memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5a, 0x140000);
		gpt_cdma_2dto2d_mem_real_fill(1, 1, 0x10000);
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;

	case 126: /* cpu-nol2 dma-nol2 2d-2d real pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_pad_c(1, 1, 0x10000);
		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
			0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 127: /* cpu-nol2 dma-nol2 2d-2d real pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_pad_c(4, 1, 0x10000);
		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 128: /* cpu-nol2 dma-nol2 2d-2d real pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_pad_0(1, 1, 0x10000);
		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;
	case 129: /* cpu-nol2 dma-nol2 2d-2d real pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_2dto2d_mem_real_pad_0(4, 1, 0x10000);
		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;

	case 130: /* cpu-l2 dma-nol2 1d-1d real copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(1, 1, 0x10000);

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 131: /* cpu-l2 dma-nol2 1d-1d real copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(1, 4, 0x4000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(1, 4, 0x4000);
		break;

	case 132: /* cpu-l2 dma-nol2 1d-1d real copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(4, 1, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 133: /* cpu-l2 dma-nol2 1d-1d real copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_1dto1d_mem_real_copy(4, 4, 0x4000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(4, 4, 0x4000);
		break;

	case 134: /* cpu-l2 dma-nol2 1d-1d real fill 4set 1base */
		gpt_data_init(0); //clear write data buffer
		gpt_data_init(4); //prepare read data buffer
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto1d_mem_real_fill(4, 1, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 135: /* cpu-l2 dma-nol2 1d-1d real fill 1set 1base */
		gpt_data_init(0); //clear write data buffer
		gpt_data_init(4); //prepare read data buffer
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto1d_mem_real_fill(1, 1, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 136: /* cpu-l2 dma-nol2 1d-1d real pad-c 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto1d_mem_real_pad_c(1, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(1, 1, 0x10000, 1, 0x4);
		break;

	case 137: /* cpu-l2 dma-nol2 1d-1d real pad-c 4set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto1d_mem_real_pad_c(4, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(4, 1, 0x10000, 1, 0x4);
		break;

	case 138: /* cpu-l2 dma-nol2 1d-1d real pad-0 1set 1base */
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto1d_mem_real_pad_0(1, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(1, 1, 0x10000, 0, 0x4);
		break;

	case 139: /* cpu-l2 dma-nol2 1d-1d real pad-0 4set 1base */
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto1d_mem_real_pad_0(4, 1, 0x10000);
		gpt_cdma_1dto1d_pad_ck(4, 1, 0x10000, 0, 0x4);
		break;

	case 140: /* cpu-l2 dma-nol2 1d-2d real copy 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_copy(1, 1, 0x10000);
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 141: /* cpu-l2 dma-nol2 1d-2d real copy 1set 4base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_copy(1, 4, 0x4000);
		gpt_cdma_1dto2d_data_ck(1, 4, 0x4000);
		break;

	case 142: /* cpu-l2 dma-nol2 1d-2d real copy 4set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_copy(4, 1, 0x10000);
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 143: /* cpu-l2 dma-nol2 1d-2d real copy 4set 4base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_copy(4, 4, 0x4000);
		gpt_cdma_1dto2d_data_ck(4, 4, 0x4000);
		break;

	case 144: /* cpu-l2 dma-nol2 1d-2d real fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_fill(4, 1, 0x10000);

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 145: /* cpu-l2 dma-nol2 1d-2d real fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_fill(1, 1, 0x10000);

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 146: /* cpu-l2 dma-nol2 1d-2d real pad-c 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_pad_c(1, 1, 0x10000);

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
			0x10, 0x50, 1, 0x4);
		break;
	case 147: /* cpu-l2 dma-nol2 1d-2d real pad-c 4set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_pad_c(4, 1, 0x10000);

		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
			0x10, 0x50, 1, 0x4);
		break;

	case 148: /* cpu-l2 dma-nol2 1d-2d real pad-0 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_pad_0(1, 1, 0x10000);

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
			0x10, 0x50, 0, 0x4);
		break;

	case 149: /* cpu-l2 dma-nol2 1d-2d real pad-0 4set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_1dto2d_mem_real_pad_0(4, 1, 0x10000);

		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
			0x10, 0x50, 0, 0x4);
		break;

	case 150: /* cpu-l2 dma-nol2 2d-2d real copy 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_2dto2d_mem_real_copy(1, 1, 0x10000);
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;
	case 151: /* cpu-l2 dma-nol2 2d-2d real copy 1set 4base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_2dto2d_mem_real_copy(1, 4, 0x4000);
		gpt_cdma_2dto2d_data_ck(1, 4, 0x4000);
		break;
	case 152: /* cpu-l2 dma-nol2 2d-2d real copy 4set 1base */
		gpt_data_init(2);
		gpt_data_init(5);
		gpt_cdma_2dto2d_mem_real_copy(4, 1, 0x10000);
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;
	case 153: /* cpu-l2 dma-nol2 2d-2d real copy 4set 4base */
		gpt_data_init(2);
		gpt_data_init(5);
		gpt_cdma_2dto2d_mem_real_copy(4, 4, 0x4000);
		gpt_cdma_2dto2d_data_ck(4, 4, 0x4000);
		break;

	case 154: /* cpu-l2 dma-nol2 2d-2d real fill 4set 1base */

		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);

		gpt_cdma_2dto2d_mem_real_fill(4, 1, 0x10000);

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;

	case 155: /* cpu-l2 dma-nol2 2d-2d real fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_2dto2d_mem_real_fill(1, 1, 0x10000);

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;

	case 156: /* cpu-l2 dma-nol2 2d-2d real pad-c 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_2dto2d_mem_real_pad_c(1, 1, 0x10000);
		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
			0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 157: /* cpu-l2 dma-nol2 2d-2d real pad-c 4set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_2dto2d_mem_real_pad_c(4, 1, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x100000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x100000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x100000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x100000);

		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 1, 0x4);
		break;

	case 158: /* cpu-l2 dma-nol2 2d-2d real pad-0 1set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_2dto2d_mem_real_pad_0(1, 1, 0x10000);
		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;
	case 159: /* cpu-l2 dma-nol2 2d-2d real pad-0 4set 1base */
		gpt_data_init(2);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x100000);
		gpt_cdma_2dto2d_mem_real_pad_0(4, 1, 0x10000);

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x100000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x100000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x100000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x100000);
		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;

	case 160: /* cpu-l2 dma-l2 1d-1d real copy 1set 1base */
	case 161: /* cpu-l2 dma-l2 1d-1d real copy 1set 4base */
	case 162: /* cpu-l2 dma-l2 1d-1d real copy 4set 1base */
	case 163: /* cpu-l2 dma-l2 1d-1d real copy 4set 4base */
	case 164: /* cpu-l2 dma-l2 1d-1d real fill 4set 1base */
	case 165: /* cpu-l2 dma-l2 1d-1d real fill 1set 1base */
	case 166: /* cpu-l2 dma-l2 1d-1d real pad-c 1set 1base */
	case 167: /* cpu-l2 dma-l2 1d-1d real pad-c 4set 1base */
	case 168: /* cpu-l2 dma-l2 1d-1d real pad-0 1set 1base */
	case 169: /* cpu-l2 dma-l2 1d-1d real pad-0 4set 1base */
	case 170: /* cpu-l2 dma-l2 1d-2d real copy 1set 1base */
	case 171: /* cpu-l2 dma-l2 1d-2d real copy 1set 4base */
	case 172: /* cpu-l2 dma-l2 1d-2d real copy 4set 1base */
	case 173: /* cpu-l2 dma-l2 1d-2d real copy 4set 4base */
	case 174: /* cpu-l2 dma-l2 1d-2d real fill 4set 1base */
	case 175: /* cpu-l2 dma-l2 1d-2d real fill 1set 1base */
	case 176: /* cpu-l2 dma-l2 1d-2d real pad-c 1set 1base */
	case 177: /* cpu-l2 dma-l2 1d-2d real pad-c 4set 1base */
	case 178: /* cpu-l2 dma-l2 1d-2d real pad-0 1set 1base */
	case 179: /* cpu-l2 dma-l2 1d-2d real pad-0 4set 1base */
	case 180: /* cpu-l2 dma-l2 2d-2d real copy 1set 1base */
	case 181: /* cpu-l2 dma-l2 2d-2d real copy 1set 4base */
	case 182: /* cpu-l2 dma-l2 2d-2d real copy 4set 1base */
	case 183: /* cpu-l2 dma-l2 2d-2d real copy 4set 4base */
	case 184: /* cpu-l2 dma-l2 2d-2d real fill 4set 1base */
	case 185: /* cpu-l2 dma-l2 2d-2d real fill 1set 1base */
	case 186: /* cpu-l2 dma-l2 2d-2d real pad-c 1set 1base */
	case 187: /* cpu-l2 dma-l2 2d-2d real pad-c 4set 1base */
	case 188: /* cpu-l2 dma-l2 2d-2d real pad-0 1set 1base */
	case 189: /* cpu-l2 dma-l2 2d-2d real pad-0 4set 1base */
		printf("DMA controller not support curently\n");
		break;

	case 190: /* cpu-nol2 dma-nol2 1d-1d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 191: /* cpu-nol2 dma-nol2 1d-1d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_data_ck(1, 4, 0x4000);
		break;

	case 192: /* cpu-nol2 dma-nol2 1d-1d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 193: /* cpu-nol2 dma-nol2 1d-1d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_data_ck(4, 4, 0x4000);
		break;

	case 194: /* cpu-nol2 dma-nol2 1d-1d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4); //prepare read data buffer
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 195: /* cpu-nol2 dma-nol2 1d-1d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4); //prepare read data buffer
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 196: /* cpu-nol2 dma-nol2 1d-1d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_pad_ck(0x1, 0x1, 0x10000, 0x1, 0x4);
		break;

	case 197: /* cpu-nol2 dma-nol2 1d-1d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_pad_ck(0x4, 0x1, 0x10000, 0x1, 0x4);
		break;

	case 198: /* cpu-nol2 dma-nol2 1d-1d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_pad_ck(0x1, 0x1, 0x10000, 0x0, 0x4);
		break;

	case 199: /* cpu-nol2 dma-nol2 1d-1d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto1d_pad_ck(0x4, 0x1, 0x10000, 0x0, 0x4);
		break;

	case 200: /* cpu-nol2 dma-nol2 1d-2d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 201: /* cpu-nol2 dma-nol2 1d-2d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_data_ck(1, 4, 0x4000);
		break;

	case 202: /* cpu-nol2 dma-nol2 1d-2d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 203: /* cpu-nol2 dma-nol2 1d-2d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_data_ck(4, 4, 0x4000);
		break;

	case 204: /* cpu-nol2 dma-nol2 1d-2d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 205: /* cpu-nol2 dma-nol2 1d-2d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;
	case 206: /* cpu-nol2 dma-nol2 1d-2d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
			0x10, 0x50, 1, 0x4);
		break;
	case 207: /* cpu-nol2 dma-nol2 1d-2d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
			0x10, 0x50, 1, 0x4);
		break;

	case 208: /* cpu-nol2 dma-nol2 1d-2d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
			0x10, 0x50, 0, 0x4);
		break;

	case 209: /* cpu-nol2 dma-nol2 1d-2d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
			0x10, 0x50, 0, 0x4);
		break;

	case 210: /* cpu-nol2 dma-nol2 2d-2d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;

	case 211: /* cpu-nol2 dma-nol2 2d-2d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(1, 4, 0x4000);
		break;

	case 212: /* cpu-nol2 dma-nol2 2d-2d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;

	case 213: /* cpu-nol2 dma-nol2 2d-2d virt copy 4set 4base */
		gpt_data_init(0);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(4, 4, 0x4000);
		break;

	case 214: /* cpu-nol2 dma-nol2 2d-2d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;
	case 215: /* cpu-nol2 dma-nol2 2d-2d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;
	case 216: /* cpu-nol2 dma-nol2 2d-2d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
			0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 217: /* cpu-nol2 dma-nol2 2d-2d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 218: /* cpu-nol2 dma-nol2 2d-2d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;
	case 219: /* cpu-nol2 dma-nol2 2d-2d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;

	case 220: /* cpu-l2 dma-nol2 1d-1d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 221: /* cpu-l2 dma-nol2 1d-1d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(1, 4, 0x4000);
		break;
	case 222: /* cpu-l2 dma-nol2 1d-1d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;
	case 223: /* cpu-l2 dma-nol2 1d-1d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(4, 4, 0x4000);
		break;
	case 224: /* cpu-l2 dma-nol2 1d-1d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;
	case 225: /* cpu-l2 dma-nol2 1d-1d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;
	case 226: /* cpu-l2 dma-nol2 1d-1d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto1d_pad_ck(1, 1, 0x10000, 1, 0x4);
		break;
	case 227: /* cpu-l2 dma-nol2 1d-1d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_pad_ck(4, 1, 0x10000, 1, 0x4);
		break;
	case 228: /* cpu-l2 dma-nol2 1d-1d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_pad_ck(1, 1, 0x10000, 0, 0x4);
		break;
	case 229: /* cpu-l2 dma-nol2 1d-1d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto1d_pad_ck(4, 1, 0x10000, 0, 0x4);
		break;

	case 230: /* cpu-l2 dma-nol2 1d-2d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 231: /* cpu-l2 dma-nol2 1d-2d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto2d_data_ck(1, 4, 0x4000);
		break;
	case 232: /* cpu-l2 dma-nol2 1d-2d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;
	case 233: /* cpu-l2 dma-nol2 1d-2d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto2d_data_ck(4, 4, 0x4000);
		break;

	case 234: /* cpu-l2 dma-nol2 1d-2d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;
	case 235: /* cpu-l2 dma-nol2 1d-2d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;
	case 236: /* cpu-l2 dma-nol2 1d-2d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
				0x10, 0x50, 1, 0x4);

		break;
	case 237: /* cpu-l2 dma-nol2 1d-2d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
				0x10, 0x50, 1, 0x4);
		break;
	case 238: /* cpu-l2 dma-nol2 1d-2d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
				0x10, 0x50, 0, 0x4);
		break;

	case 239: /* cpu-l2 dma-nol2 1d-2d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
				0x10, 0x50, 0, 0x4);
		break;

	case 240: /* cpu-l2 dma-nol2 2d-2d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;
	case 241: /* cpu-l2 dma-nol2 2d-2d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(1, 4, 0x4000);
		break;
	case 242: /* cpu-l2 dma-nol2 2d-2d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;
	case 243: /* cpu-l2 dma-nol2 2d-2d virt copy 4set 4base */
		gpt_data_init(0);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(4, 4, 0x4000);
		break;
	case 244: /* cpu-l2 dma-nol2 2d-2d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;
	case 245: /* cpu-l2 dma-nol2 2d-2d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;

	case 246: /* cpu-l2 dma-nol2 2d-2d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
				0x10, 0x50, 0x0, 1, 0x4);
		break;

	case 247: /* cpu-l2 dma-nol2 2d-2d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
				0x10, 0x50, 0x0, 1, 0x4);
		break;

	case 248: /* cpu-l2 dma-nol2 2d-2d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
				0x10, 0x50, 0x0, 0, 0x4);

		break;

	case 249: /* cpu-l2 dma-nol2 2d-2d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x40000);

		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;

	case 250: /* cpu-l2 dma-l2 1d-1d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 251: /* cpu-l2 dma-l2 1d-1d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(1, 4, 0x4000);
		break;

	case 252: /* cpu-l2 dma-l2 1d-1d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 253: /* cpu-l2 dma-l2 1d-1d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(4, 4, 0x4000);
		break;

	case 254: /* cpu-l2 dma-l2 1d-1d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(4, 1, 0x10000);
		break;

	case 255: /* cpu-l2 dma-l2 1d-1d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_data_ck(1, 1, 0x10000);
		break;

	case 256: /* cpu-l2 dma-l2 1d-1d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_pad_ck(1, 1, 0x10000, 1, 0x4);
		break;

	case 257: /* cpu-l2 dma-l2 1d-1d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_pad_ck(4, 1, 0x10000, 1, 0x4);
		break;

	case 258: /* cpu-l2 dma-l2 1d-1d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_pad_ck(1, 1, 0x10000, 0, 0x4);
		break;

	case 259: /* cpu-l2 dma-l2 1d-1d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto1d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto1d_pad_ck(4, 1, 0x10000, 0, 0x4);
		break;

	case 260: /* cpu-l2 dma-l2 1d-2d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;
	case 261: /* cpu-l2 dma-l2 1d-2d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_data_ck(1, 4, 0x4000);
		break;
	case 262: /* cpu-l2 dma-l2 1d-2d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 263: /* cpu-l2 dma-l2 1d-2d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_data_ck(4, 4, 0x4000);
		break;

	case 264: /* cpu-l2 dma-l2 1d-2d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_data_ck(4, 1, 0x10000);
		break;

	case 265: /* cpu-l2 dma-l2 1d-2d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_data_ck(1, 1, 0x10000);
		break;

	case 266: /* cpu-l2 dma-l2 1d-2d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
				0x10, 0x50, 1, 0x4);
		break;

	case 267: /* cpu-l2 dma-l2 1d-2d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
				0x10, 0x50, 1, 0x4);
		break;

	case 268: /* cpu-l2 dma-l2 1d-2d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_pad_ck(1, 1, 0x1000,
				0x10, 0x50, 0, 0x4);
		break;

	case 269: /* cpu-l2 dma-l2 1d-2d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_1dto2d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_1dto2d_pad_ck(4, 1, 0x1000,
				0x10, 0x50, 0, 0x4);
		break;
	case 270: /* cpu-l2 dma-l2 2d-2d virt copy 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;
	case 271: /* cpu-l2 dma-l2 2d-2d virt copy 1set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(1, 4, 0x4000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_data_ck(1, 4, 0x4000);
		break;
	case 272: /* cpu-l2 dma-l2 2d-2d virt copy 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;

	case 273: /* cpu-l2 dma-l2 2d-2d virt copy 4set 4base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_copy(4, 4, 0x4000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(4, 4, 0x4000);
		break;

	case 274: /* cpu-l2 dma-l2 2d-2d virt fill 4set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_fill(4, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(4, 1, 0x10000);
		break;

	case 275: /* cpu-l2 dma-l2 2d-2d virt fill 1set 1base */
		gpt_data_init(0);
		gpt_data_init(4);
		gpt_l2_flush(GPT_CDMA_RDATA0_BASE0, 0x40000);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_fill(1, 1, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_2dto2d_data_ck(1, 1, 0x10000);
		break;

	case 276: /* cpu-l2 dma-l2 2d-2d virt pad-c 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_c(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
				0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 277: /* cpu-l2 dma-l2 2d-2d virt pad-c 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_c(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
				0x10, 0x50, 0x0, 1, 0x4);
		break;
	case 278: /* cpu-l2 dma-l2 2d-2d virt pad-0 1set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_0(1, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_pad_ck(1, 1, 0x10000,
				0x10, 0x50, 0x0, 0, 0x4);
		break;
	case 279: /* cpu-l2 dma-l2 2d-2d virt pad-0 4set 1base */
		gpt_data_init(2);
		gpt_cdma_virt_tlb_l2en_config();
		gpt_cdma_virt_en();
		gpt_cdma_2dto2d_mem_virt_pad_0(4, 1, 0x10000);
		gpt_cdma_virt_dis();

		gpt_cdma_2dto2d_pad_ck(4, 1, 0x10000,
			0x10, 0x50, 0x0, 0, 0x4);
		break;

	case 90: /* cpu-l2 dma-nol2 1d-1d real copy 1set 1base nosync*/
		length = 0x10000;
		gpt_data_init(2);

		gpt_cdma_nosync_en();
		gpt_cdma_copy((void *)GPT_CDMA_RDATA0_BASE0,
				(void *)GPT_CDMA_WDATA0_BASE0, length);
		gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0,
				(void *)GPT_CDMA_WDATA0_BASE0, length);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x5a, 0x10);
		gpt_cdma_copy((void *)GPT_CDMA_WDATA0_BASE0,
				(void *)GPT_CDMA_WDATA0_BASE1, length);
		gpt_cdma_data_check((void *)GPT_CDMA_WDATA0_BASE0,
				(void *)GPT_CDMA_WDATA0_BASE1, length);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE1, 0x100);
		break;

	case 0: /* . copy 1d to 1d with pad */
		gpt_data_init(2);
		gpt_cdma_mul_1dto1d_m_copy_pad(4);
		break;
	case 80:
		gpt_data_init(2);
		gpt_cdma_mul_1dto1d_m_pad0(1);
		break;
	case 81:
		gpt_data_init(2);
		gpt_cdma_mul_1dto2d_m_copy(4, 4);
		break;
	case 1:
		length = 0x10000;
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_cdma_copy((void *)GPT_CDMA_RDATA0_BASE0,
				(void *)GPT_CDMA_WDATA0_BASE0, length);
		gpt_cdma_data_check((void *)GPT_CDMA_RDATA0_BASE0,
				(void *)GPT_CDMA_WDATA0_BASE0, length);
		break;

	case 2: /* . 1 set 4 base w1d 41d*/
		gpt_data_init(1);
		gpt_cdma_copy_mul(4, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, GPT_CDMA_WDATA0_BASE0, 0x4000);
		break;

	case 3: /* . 4 set 4 base w1d r1d*/
		gpt_data_init(2);
		if(gpt_cdma_mul_set(4, 0x4000, 0x4000, 0x4000, 0x4000))
			printf("cdma timeout\n");
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 4: /* . 1 set 4 base w2d r1d*/
		randr = gpt_data_random(NULL, 0);
		cmp[0] = randr + 0;
		cmp[1] = randr + 1;
		cmp[2] = randr + 2;
		cmp[3] = randr + 3;
		memset((void *)GPT_CDMA_RDATA0_BASE0, randr + 0, 0x10000);
		memset((void *)GPT_CDMA_RDATA1_BASE0, randr + 1, 0x10000);
		memset((void *)GPT_CDMA_RDATA2_BASE0, randr + 2, 0x10000);
		memset((void *)GPT_CDMA_RDATA3_BASE0, randr + 3, 0x10000);
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x00, 0x40000);
		gpt_cdma_copy_mul_w2d(4, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA1_BASE0, GPT_CDMA_RDATA2_BASE0,
			GPT_CDMA_RDATA3_BASE0, 0x1000, GPT_CDMA_WDATA0_BASE0, 0x10);
		gpt_cdma_mul_data_check(cmp, 2, 0x100, (void *)GPT_CDMA_WDATA0_BASE0);
		break;

	case 5: /* 1d to 1d */
		gpt_data_init(2);
		gpt_cdma_mul_1dto1d_m_copy(4, 1);
		break;

	case 6: /* 4 set 4base w2d r2d */
		gpt_data_init(2);
		gpt_cdma_mul_1dto1d_m_fill(4);
		break;

	case 7: /* . 1 set 1 base w1d r1d tlb */
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		/*0xc000000 -> 0xa0010000*/
		gpt_cdma_tlb_r(0, 0xa0000000, 0xc0000000, 0xfffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_BASE0, 0xfffd);
		gpt_cdma_virt_en();
		gpt_cdma_copy_mul(1, 0xc0000000,
			0, 0, 0, GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_cdma_virt_dis();
		gpt_cdma_data_check((void *)0xa0000000,
				(void *)GPT_CDMA_WDATA0_BASE0, 0x10000);
		break;

	case 9: /* . 1d to 2d 4set 1 base */
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x00, 0x10000);
		memset((void *)GPT_CDMA_WDATA0_BASE1, 0x00, 0x10000);
		memset((void *)GPT_CDMA_WDATA0_BASE2, 0x00, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		memset((void *)GPT_CDMA_WDATA0_BASE3, 0x00, 0x10000);
		gpt_cdma_1d_to_2d();
		break;

	case 10: /* . copy 1d to 1d with zero pad */
		gpt_data_init(1);
		gpt_data_init(3);
		gpt_cdma_mul_set_pad(4, GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B, 0x10000, 0x10000, 0x10000, 0x10000);
		break;

	case 11: /* . copy 1d-1d 1set 1base with fill */
//		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x0, 0x10000);
		gpt_cdma_mul_set_fill(1, GPT_OP_TY_FILL, 0x10000, 0x100, 0x100, 0x100);
		break;

	case 12: /* . 1d-1d 1set 4base*/
		gpt_data_init(1);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		gpt_cdma_copy_mul(4, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0x4000);
		gpt_cdma_virt_dis();
		break;

	case 13: /* . 1d-1d 4set 4base vitrual */
		gpt_data_init(1);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		if(gpt_cdma_mul_set_tlb(4, 0x4000, 0x4000, 0x4000, 0x4000))
			printf("cdma timeout\n");
		gpt_cdma_virt_dis();
		break;

	case 14: /* . copy 1d-1d 1set 1base fill 0x5a virtual */
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		memset((void *)GPT_CDMA_WDATA0_BASE0, 0x0, 0x10000);
		gpt_cdma_mul_set_fill_virtul(4, GPT_OP_TY_FILL, 0x10000, 0x100, 0x100, 0x100);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x10000);
		break;

	case 15: /* . 1d to 2d 4set 4base */
		gpt_data_init(2);
		gpt_cdma_mul_1d_to_2d(1, 4);
		break;
	case 16: /*. 2d to 2d 4set 1base */
		gpt_data_init(2);
		gpt_cdma_2d_to_2d();
		break;

	case 17: /*. 2d to 2d 4set 4base */
		gpt_data_init(2);
		gpt_cdma_mul_2d_to_2d(1, 1);
		break;

	case 18: /* . 2d to 2d 4set 1base virtual */
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_data_init(3);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_virtual(4, 4);
		gpt_cdma_virt_dis();
		break;

	case 19: /* . 1d to 2d 4set 4base virtual*/
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE1, 0x10000);
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE2, 0x10000);
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE3, 0x10000);
		gpt_data_init(3);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_virtual(4, 4);
		gpt_cdma_virt_dis();
		break;

	case 20: /* . 1d to 2d 4set 4base pad x(1base ok)*/
		gpt_data_init(2);
		gpt_cdma_mul_1d_to_2d_pad(4);
		break;

	case 21: /* . 1d to 2d 4set 4base fill x*/
		gpt_data_init(2);
		gpt_cdma_mul_1d_to_2d_fill(1);
		break;

	case 22: /* . 2d to 2d 4set 4 base pad x*/
		gpt_data_init(2);
		gpt_cdma_mul_2d_to_2d_pad(1, 1);
		break;

	case 23: /* . 2d to 2d 4set 4base fill x*/
		gpt_data_init(2);
		gpt_cdma_mul_2d_to_2d_fill(4);
		break;

	case 24: /* . 1d to 2d 4set 4 base pad virtual x*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();

		gpt_cdma_mul_1d_to_2d_pad_virtual(GPT_OP_TY_ZERO_PAD |
			GPT_OP_SZ_4B | GPT_OP_CN_4B | GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();
		break;
	case 25: /* . 1d to 2d 4set 4base fill virtual x*/
		gpt_data_init(1);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_fill_virtual();
		gpt_cdma_virt_dis();
		break;

	case 26: /* . 2d to 2d 4set 4 base pad virtual x*/
		gpt_data_init(1);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_pad_virtual(4, GPT_OP_TY_ZERO_PAD | GPT_OP_SZ_1B | GPT_OP_CN_4B |
		GPT_OP_WR_MULTIPLE | GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();
		break;
	case 27: /* . 2d to 2d 4set 4base fill virtual x*/
		gpt_data_init(1);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0, GPT_CDMA_RDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0xffffffd);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_fill_virtual(4);
		gpt_cdma_virt_dis();
		break;

	case 28: /* . 1d-1d 1set 1base l2 coherence real(for test)*/
		memcpy(buff_l2, (void *)GPT_CDMA_WDATA0_BASE0, 0x10);
		gpt_cdma_dumpdata((void *)buff_l2, 0x10);

		gpt_cdma_rl2_en();
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_cdma_copy_mul(1, GPT_CDMA_RDATA0_BASE0,
			0, 0, 0, GPT_CDMA_WDATA0_BASE0, 0x10);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x10);

		gpt_cdma_copy_mul(1, GPT_CDMA_WDATA0_BASE0,
			0, 0, 0, GPT_CDMA_WDATA0_BASE1, 0x10);
		gpt_cdma_rl2_dis();
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE1, 0x10);
		break;

	case 29: /* . 1d-1d 1set 1base l2 coherence virtual (for test)*/
		memcpy(buff_l2, (void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		gpt_cdma_dumpdata((void *)buff_l2, 0x100);

		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_cdma_copy_mul(1, GPT_CDMA_RDATA0_BASE0,
			0, 0, 0, GPT_CDMA_WDATA0_BASE0, 0x100);
	//	gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x100);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_r(1, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_copy_mul(1, GPT_CDMA_WDATA0_V_BASE0,
			0, 0, 0, GPT_CDMA_WDATA0_V_BASE1, 0x100);
		gpt_cdma_virt_dis();
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x100);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE1, 0x100);
		break;

	case 30:
		gpt_data_init(2);
		gpt_cdma_mul_1dto1d_copy();
		break;

	case 31:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x0, 0x4000);
		gpt_cdma_pxi_1dto2d_copy();
		break;

	case 32:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x0, 0x4000);
		gpt_cdma_pxi_2dto2d_copy();
		break;

	case 33:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x0, 0x4000);
		gpt_cdma_pxi_2dto2d_copy_pad();
		break;

	case 34:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x63, 0x4000);
		gpt_cdma_pxi_2dto2d_copy_pad0();
		break;

	case 35:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x63, 0x4000);
		gpt_cdma_pxi_2dto2d_fill();
		break;

	case 36:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x0, 0x4000);
		gpt_cdma_pxi_1dto1d_copy_pad();
		break;

	case 37:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x5a, 0x4000);
		gpt_cdma_pxi_1dto1d_copy_pad0();
		break;

	case 38:
		gpt_data_init(2);
		memset((void *)GPT_PXI_WDATA0_BASE(0), 0x5a, 0x4000);
		gpt_cdma_pxi_1dto1d_fill();
		break;
	
	case 39:
		gpt_cdma_get_psc_spr();
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10);
		gpt_data_random((void *)GPT_CDMA_WDATA0_BASE0, 0x10);
		gpt_l2_sync(GPT_CDMA_RDATA0_BASE0, 0x100);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 40: /*1d to 1d 4 set 1base virtual copy with l2cache*/
		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x10000);
		gpt_data_random((void *)GPT_CDMA_RDATA1_BASE0, 0x10000);
		gpt_data_random((void *)GPT_CDMA_RDATA2_BASE0, 0x10000);
		gpt_data_random((void *)GPT_CDMA_RDATA3_BASE0, 0x10000);
//		gpt_l2_sync(GPT_CDMA_RDATA0_BASE0, 0x100);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_copy_mul(4, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0x4000);
		gpt_cdma_virt_dis();
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x10000);
		break;

	case 41:
		gpt_cdma_ext2axi_test();
		break;

	case 42:
		gpt_cdma_pxi_test(1);
		break;

	case 43: /*. 1d to 2d 4 base 4 set virutual copy with l2cache */
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_virtual(4, 4);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		gpt_cdma_virt_dis();

		gpt_cdma_1set_nbase_check(1, (void *)GPT_CDMA_RDATA0_BASE0, (void *)GPT_CDMA_RDATA1_BASE0, 
			(void *)GPT_CDMA_RDATA2_BASE0, (void *)GPT_CDMA_RDATA3_BASE0, 0x10000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE0);
		break;

	case 44: /*. 2d to 2d 4 base 1set virtual copy with l2cache (mulbase error)*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_virtual(4, 4);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		gpt_cdma_virt_dis();
		break;
	case 45: /*1d to 1d 4 set 1 base virtual fill with l2 (failed)*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_set_fill_virtul(4, GPT_OP_TY_FILL, 0x10000, 0x10000, 0x10000, 0x10000);
		gpt_cdma_virt_dis();

		memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5a, 0x20000);
		memset((void *)GPT_CDMA_RDATA0_BASE1, 0x5a, 0x20000);
		memset((void *)GPT_CDMA_RDATA0_BASE2, 0x5a, 0x20000);
		memset((void *)GPT_CDMA_RDATA0_BASE3, 0x5a, 0x20000);
		gpt_cdma_1set_nbase_check(1, (void *)GPT_CDMA_RDATA0_BASE0, (void *)GPT_CDMA_RDATA1_BASE0, 
			(void *)GPT_CDMA_RDATA2_BASE0, (void *)GPT_CDMA_RDATA3_BASE0, 0x10000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE0);

		gpt_cdma_1set_nbase_check(1, (void *)GPT_CDMA_RDATA0_BASE1, (void *)GPT_CDMA_RDATA1_BASE1, 
			(void *)GPT_CDMA_RDATA2_BASE1, (void *)GPT_CDMA_RDATA3_BASE1, 0x10000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE1);

		gpt_cdma_1set_nbase_check(1, (void *)GPT_CDMA_RDATA0_BASE2, (void *)GPT_CDMA_RDATA1_BASE2, 
			(void *)GPT_CDMA_RDATA2_BASE2, (void *)GPT_CDMA_RDATA3_BASE2, 0x10000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE2);

		gpt_cdma_1set_nbase_check(1, (void *)GPT_CDMA_RDATA0_BASE3, (void *)GPT_CDMA_RDATA1_BASE3, 
			(void *)GPT_CDMA_RDATA2_BASE3, (void *)GPT_CDMA_RDATA3_BASE3, 0x10000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE3);
		break;

	case 46: /* . 1d to 2d 4set 1base virtual fill with l2(other sets failed)*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_set_fill_virtul(1, GPT_OP_TY_FILL, 0x10000, 0x10000, 0x10000, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x1000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x1000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x1000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x1000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 47: /*. 2d to 2d 4set 1 base virtual fill with l2 (other sets failed)*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_fill_virtual(4);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 48: /* 1d to 1d 4 set 1 base virtual zero pad (copy failed)*/
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();

		gpt_cdma_mul_set_pad_virtual(4, GPT_OP_TY_ZERO_PAD |
				GPT_OP_SZ_1B | GPT_OP_CN_4B, 0x10000,
				0x10000, 0x10000, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 49: /* 1d to 1d 4 set 1 base virtual copy pad (copy failed)*/
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_set_pad_virtual(1, GPT_OP_TY_COPY_PAD |
				GPT_OP_SZ_1B | GPT_OP_CN_4B, 0x10000,
				0x10000, 0x10000, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 50: /*. 1d to 2d 4 set 1 base virtual zero pad (1base ok, mul base no pad data)*/
		gpt_data_init(2);
		gpt_data_init(3);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);

		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_pad_virtual(GPT_OP_TY_ZERO_PAD |
			GPT_OP_SZ_4B | GPT_OP_CN_4B | GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_flush(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_flush(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_flush(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_flush(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 51: /*. 1d to 2d 4 set 1 base virtual copy pad */
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);

		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_pad_virtual(GPT_OP_TY_COPY_PAD |
			GPT_OP_SZ_4B | GPT_OP_CN_4B | GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 52: /*. 2d to 2d 4 set 1 base virtual zero pad */
		gpt_data_init(2);
		gpt_data_init(3);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_pad_virtual(4, GPT_OP_TY_ZERO_PAD |
			GPT_OP_SZ_1B | GPT_OP_CN_8B | GPT_OP_WR_MULTIPLE |
			GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 53: /*. 2d to 2d 4 set 1 base virtual copy pad */
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_pad_virtual(4, GPT_OP_TY_COPY_PAD |
			GPT_OP_SZ_1B | GPT_OP_CN_8B | GPT_OP_WR_MULTIPLE |
			GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 54: /*. 1d to 1d 1 set 1base virtual copy soc-l2 dma-nol2*/
		memcpy(buff_l2, (void *)GPT_CDMA_RDATA0_BASE0, 0x100);
		gpt_cdma_dumpdata((void *)buff_l2, 0x100);

		gpt_data_random((void *)GPT_CDMA_RDATA0_BASE0, 0x1000);
		gpt_l2_sync(GPT_CDMA_RDATA0_BASE0, 0x100);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_copy_mul(4, GPT_CDMA_RDATA0_V_BASE0,
			GPT_CDMA_RDATA1_V_BASE0, GPT_CDMA_RDATA2_V_BASE0,
			GPT_CDMA_RDATA3_V_BASE0, GPT_CDMA_WDATA0_V_BASE0, 0x100);
		gpt_cdma_virt_dis();
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x180);
		break;

	case 55: /*. 1d to 1d 1 set 1base virtual copy soc-l2 dma-l2*/
		memcpy(buff_l2, (void *)GPT_CDMA_RDATA0_BASE0, 0x100);
		gpt_cdma_dumpdata((void *)buff_l2, 0x100);

		gpt_data_init(2);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN | GPT_TLBMASK_L2);
		gpt_cdma_virt_en();
		gpt_cdma_mul_1dto1d_m_copy_v(1, 4);
		gpt_cdma_virt_dis();

		gpt_cdma_1set_nbase_check(4, (void *)GPT_CDMA_RDATA0_BASE0, (void *)GPT_CDMA_RDATA1_BASE0,
			(void *)GPT_CDMA_RDATA2_BASE0, (void *)GPT_CDMA_RDATA3_BASE0, 0x4000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE0);
		break;

	case 56: /* 1d to 1d 4 set 1 base virtual zero pad (copy failed)*/
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();

		gpt_cdma_mul_set_pad_virtual(4, GPT_OP_TY_ZERO_PAD |
				GPT_OP_SZ_1B | GPT_OP_CN_4B, 0x10000,
				0x10000, 0x10000, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 57: /* . 1d to 1d 4set 1baes virtual copy pad */
		gpt_data_init(2);
		gpt_data_init(3);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();

		gpt_cdma_mul_set_pad_virtual(4, GPT_OP_TY_COPY_PAD |
				GPT_OP_SZ_1B | GPT_OP_CN_4B, 0x10000,
				0x10000, 0x10000, 0x10000);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 58: /*. 1d to 2d 4 base 4 set virutual copy */
		gpt_data_init(2);

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_virtual(1, 1);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x20000);

		gpt_cdma_1set_nbase_check(1, (void *)GPT_CDMA_RDATA0_BASE0, (void *)GPT_CDMA_RDATA1_BASE0, 
			(void *)GPT_CDMA_RDATA2_BASE0, (void *)GPT_CDMA_RDATA3_BASE0, 0x10000, 1,
			0, (void *)GPT_CDMA_WDATA0_BASE0);
		break;

	case 59: /*1d-2d 4set 1 base cpu-l2 dma-nol2 fill virtual */
		gpt_data_init(2);
#if 0
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x1000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x1000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x1000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x1000);
#endif
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_fill_virtual();
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x20000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 60: /*. 1d to 2d 4 set 1base virtual zero pad cpu-l2 dma-nol2 virtual*/ 
		gpt_data_init(2);
		gpt_data_init(3);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);

		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_pad_virtual(GPT_OP_TY_ZERO_PAD |
			GPT_OP_SZ_4B | GPT_OP_CN_4B | GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x20000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 61: /*. 1d to 2d 4 set 1 base virtual copy pad cpu-l2 dma-nol2 ,virtual*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);

		gpt_cdma_virt_en();
		gpt_cdma_mul_1d_to_2d_pad_virtual(GPT_OP_TY_COPY_PAD |
			GPT_OP_SZ_4B | GPT_OP_CN_4B | GPT_OP_WR_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x20000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 62: /* 2d to 2d cpu-l2 dma-nol2 virtual copy */
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_virtual(1, 1);
		gpt_cdma_virt_dis();

		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE0, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE1, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE2, 0x20000);
		gpt_l2_invalid(GPT_CDMA_WDATA0_BASE3, 0x20000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 63: /*. 2d to 2d 4set 1 base virtual fill with l2 (other sets failed)*/
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_fill_virtual(1);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x10000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x10000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 64: /*. 2d to 2d 4 set 1 base virtual zero pad */
		gpt_data_init(2);
		gpt_data_init(3);

		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_pad_virtual(4, GPT_OP_TY_ZERO_PAD |
			GPT_OP_SZ_1B | GPT_OP_CN_8B | GPT_OP_WR_MULTIPLE |
			GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 65: /*. 2d to 2d 4 set 1 base virtual copy pad */
		gpt_data_init(2);
		gpt_cdma_tlb_r(0, GPT_CDMA_RDATA0_BASE0,
			GPT_CDMA_RDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_tlb_w(0, GPT_CDMA_WDATA0_BASE0,
			GPT_CDMA_WDATA0_V_BASE0, GPT_TLBMASK_MK(0xfffffff)
			| GPT_TLBMASK_EN);
		gpt_cdma_virt_en();
		gpt_cdma_mul_2d_to_2d_pad_virtual(4, GPT_OP_TY_COPY_PAD |
			GPT_OP_SZ_1B | GPT_OP_CN_8B | GPT_OP_WR_MULTIPLE |
			GPT_OP_RD_MULTIPLE | GPT_OP_RB_NUM0);
		gpt_cdma_virt_dis();

		gpt_l2_sync(GPT_CDMA_WDATA0_BASE0, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE1, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE2, 0x40000);
		gpt_l2_sync(GPT_CDMA_WDATA0_BASE3, 0x40000);
		gpt_cdma_dumpdata((void *)GPT_CDMA_WDATA0_BASE0, 0x100);
		break;

	case 88:
		gpt_data_init(2);
		gpt_cdma_mem_real_copy((uint8_t *)GPT_CDMA_WDATA0_BASE0, (uint8_t *)GPT_CDMA_RDATA0_BASE0, 0x200000);
		gpt_cdma_1dto1d_data_ck(1, 1, 0x200000);
		break;
	default:
		printf("Unsupport mode %d\n", type);
		return -1;
	}

	return 0;
}

int gpt_cdma_mem_real_copy(uint8_t *dst, uint8_t *src, int length)
{
	int count;
	int nset = 1;
	int rstr = 0x0;
	int dstr = 0x0;
	int row = 0x1;
	int nbase = 0x1;
	#define GPT_CDMA_PAGE	0x10000
	int num = length / GPT_CDMA_PAGE;
	int offset = length % GPT_CDMA_PAGE;
	uint64_t mode = GPT_OP_TY_COPY | gpt_cdma_nbase_cfg(nbase);

	int len;

	num += (offset > 0) ? 1 : 0;

	len = GPT_CDMA_PAGE;

	for (count = 0; count < num; count++) {
		if ((offset > 0) && (count == (num - 1)))
			len = offset;

//		printf("count: %x, len: %x\n", count, len);
		gpt_cdma_xset_xbase(0, nbase, mode, (uint64_t)src + count * GPT_CDMA_PAGE,
				0, 0, 0, rstr, len,
				row, (uint64_t)dst + count * GPT_CDMA_PAGE, dstr,
				len, row);

		gpt_cdma_cmd_enable(nset - 1);
		gpt_cdma_wait_done();
	}

	return 0;
}

int gpt_cdma_mem_virt_copy(uint8_t *dst, uint8_t *src, int length)
{
#if 1
	gpt_cdma_virt_tlb_l2en_sync_config();
	gpt_cdma_virt_en();
	gpt_cdma_1dto1d_mem_virt_copy(1, 1, 0x10000);
	gpt_cdma_virt_dis();
#else
	gpt_cdma_virt_tlb_l2en_sync_config();
	gpt_cdma_virt_en();
	gpt_cdma_1dto1d_mem_virt_copy(1, 1, 0x10000);
	gpt_cdma_virt_dis();


#endif

	return 0;
}

#define GPT_CORE_CTL(n)		(0xf0000000 + 4 * (n))
#define GPT_CORE_EN		(0x1 << 4)
void gpt_core_en(int num)
{
	uint32_t regs;
	regs = readl(GPT_CORE_CTL(1));
	writel(regs | GPT_CORE_EN, GPT_CORE_CTL(1));
}

void gpt_lock_l1(uint64_t addr)
{
	__asm__ volatile (
		"dctl_lock_l1 %0; barrier;\n\t"
		:
		:"a"(addr)
		:
	);

}

void gpt_unlock_l1(uint64_t addr)
{
	__asm__ volatile (
		"dctl_unlock_l1 %0; barrier;\n\t"
		:
		:"a"(addr)
		:
	);
}

#define GPT_CDMA_REG_CMD_EN             0x8a000000
#define GPT_CDMA_REG_CMD                0x8a000004

#define GPT_CDMA_REG_DONE		0x8a100004
#define GPT_CDMA_REG_STATE              0x8a100008
#define GPT_CDMA_REG_STEP1              0x8a10000c
#define GPT_CDMA_REG_STEP2              0x8a100010

#define GPT_CDMA_CMD_EN                 0x5a
#define GPT_CDMA_CMD_IDEL               0x5c

#define GPT_MENU_CDMA_0                 0xa1
#define GPT_MENU_CDMA_1                 0xa2
#define GPT_MENU_CDMA_2                 0xa3

int gpt_cdma_multcore_cmd(uint32_t cmd)
{
	writel(0x0, GPT_CDMA_REG_DONE);
	writel(cmd, GPT_CDMA_REG_CMD);
	writel(GPT_CDMA_CMD_EN, GPT_CDMA_REG_CMD_EN);
	gpt_l1_sync(GPT_CDMA_REG_CMD_EN, 0x1);
	return 0;
}

int gpt_cdma_multcore_cmdump(uint64_t addr)
{
	gpt_l1_invalid(addr, 0x0);
	gpt_cdma_dumpdata((void *)addr, 0x20);
	return 0;
}

int gpt_cdma_multcore_sync(void)
{
	memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5a, 0x10);
	gpt_lock_l1(GPT_CDMA_RDATA0_BASE0);
	memset((void *)GPT_CDMA_RDATA0_BASE0, 0x5b, 0x10);
	gpt_unlock_l1(GPT_CDMA_RDATA0_BASE0);
	return 0;
}

int gpt_cdma_core_loop(void)
{
	int32_t state = 0;
	int32_t mode = 0;

	gpt_cdma_init();

	while (1) {
		state = readl(GPT_CDMA_REG_STATE);
		writel(state++, GPT_CDMA_REG_STATE);

		if (readl(GPT_CDMA_CMD_EN) == GPT_CDMA_CMD_EN) {

			mode = readl(GPT_CDMA_CMD_EN);
			switch(mode) {
			case GPT_MENU_CDMA_0:
				gpt_cdma_mem_real_copy((void *)0x230000000, (void *)0x220000000, 0x100);
				break;

			case GPT_MENU_CDMA_1:
				gpt_cdma_multcore_sync();
				break;

			case GPT_MENU_CDMA_2:
				break;
			}

			writel(GPT_CDMA_CMD_IDEL, GPT_CDMA_REG_CMD_EN);
		}
	}
}
