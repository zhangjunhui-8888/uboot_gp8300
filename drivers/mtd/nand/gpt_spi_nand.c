#include <common.h>
#include <malloc.h>
#include <nand.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <linux/sizes.h>

#include <asm/chip2/gpt_spi_nand.h>

static struct gpt_sf_nand *g_sf;

static u8 cmd_array[20] = {0x06, 0x04, 0x0f, 0x1f,
			   0x13, 0x03, 0x3b, 0x6b,
			   0xbb, 0xeb, 0x9f, 0x02,
			   0x32, 0x10, 0x84, 0xc4,
			   0x72, 0xd8, 0xff};

static u16 instr[128] = {0};

#define	PRE_BUFF_CMD_BASE	(CONFIG_SYS_NAND_BASE + 0x000)
#define	PRE_BUFF_COLUMN0_BASE	(CONFIG_SYS_NAND_BASE + 0x014)
#define	PRE_BUFF_COLUMN1_BASE	(CONFIG_SYS_NAND_BASE + 0x018)
#define	PRE_BUFF_COLUMN2_BASE	(CONFIG_SYS_NAND_BASE + 0x01c)
#define	PRE_BUFF_COLUMN3_BASE	(CONFIG_SYS_NAND_BASE + 0x020)
#define	PRE_BUFF_ROW0_BASE	(CONFIG_SYS_NAND_BASE + 0x024)
#define	PRE_BUFF_ROW1_BASE	(CONFIG_SYS_NAND_BASE + 0x028)
#define	PRE_BUFF_ROW2_BASE	(CONFIG_SYS_NAND_BASE + 0x02c)
#define	PRE_BUFF_ROW3_BASE	(CONFIG_SYS_NAND_BASE + 0x030)
#define	PRE_BUFF_INSTR_BASE	(CONFIG_SYS_NAND_BASE + 0x034)
#define GPT_SPI_NAND_PRE_BUF    0x000
#define GPT_SPI_REG_BASE        0x800

#define mtd_to_gpt(m) container_of(m->priv, struct gpt_sf_nand, nand)

static int gpt_sf_nand_pre_buf_cmd_set(struct gpt_sf_nand *sf)
{
	volatile int *cmd_base = sf->cmd;
	volatile int *p = (volatile int *)cmd_array;
	int len = sizeof(cmd_array) / 4;
	for(; len >= 0; len--) {
		writel(*p++, cmd_base++);
	}
	return 0;
}

static int gpt_sf_nand_check_status(struct gpt_sf_nand *sf)
{
        int val = 0;
        int status = readl(&sf->base->irq_status);
        u8 nfc = readl(&sf->base->nfc_status);

        if (nfc >> 3) {
                printf("OP failed ================================nfc_status: %d\n", nfc);
        }
        if (status & GPT_SPI_NAND_NFC_READY_INT) {
//              printf("GPT_SPI_NAND_NFC_READY_INT ...\n");
                val |= GPT_SPI_NAND_NFC_READY_CLR;
        } else {
                writel(0, &sf->base->irq_mask);
        }
        if (status & GPT_SPI_NAND_CHECK_FAILED_INT) {
                val |= GPT_SPI_NAND_CHECK_FAILED_CLR;
        }
        if (status & GPT_SPI_NAND_AXI_ERROR_INT) {
                val |= GPT_SPI_NAND_AXI_ERROR_CLR;
        }
        if (status & GPT_SPI_NAND_ECC_ERROR_INT) {
                val |= GPT_SPI_NAND_ECC_ERROR_CLR;
        }

        writel(val, &sf->base->irq_clr);

        return 0;
}

static int gpt_sf_start(struct gpt_sf_nand *sf, int page, int len)
{
	int val;
	if (len <= 0)
		return -1;

	val = GPT_SPI_NAND_SPI_DIV8 | GPT_SPI_NAND_SPI_MODE0 | GPT_SPI_NAND_SPI_RD_SIGNLE | GPT_SPI_NAND_SPI_RD_TUNE |\
	      GPT_SPI_NAND_SPI_WT_SIGNLE | GPT_SPI_NAND_SPI_WP_HIGH | GPT_SPI_NAND_SPI_HD_HIGH | GPT_SPI_NAND_SPI_IDLE;
	writel(val, &sf->base->spi_ctrl);
	val = GPT_SPI_NAND_WRFIFO_THRESHOLD_DEFAULT << GPT_SPI_NAND_WRFIFO_THRESHOLD_OFFSET | len;
	writel(val, &sf->base->transfer_ctrl);

	writel(page, &sf->base->page_num);

	val = GPT_SPI_NAND_NFC_START | GPT_SPI_NAND_NFC_OP_READ | GPT_SPI_NAND_NFC_ECCERR_STOP | \
				1 << GPT_SPI_NAND_CHECK_READY_OFFSET | 0xc << GPT_SPI_NAND_CHECK_ERR_OFFSET;
	writel(val, &sf->base->nfc_ctrl);

	return 0;
}

static int gpt_sf_nand_reset(struct gpt_sf_nand *sf)
{
	int index = 0, i, val;
	volatile int *instr_base = g_sf->instruction;
	volatile int *p;

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_RESET;
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_SCH;
	instr[index++] = GPT_INSTR_SF_NAND_CKECKSTATUS | 2;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;

	p = (volatile int *)instr;
	if (index & 0x1)
		index++;
	for (i = 0; i < index / 2; i++ ) {
		writel(*p++, instr_base++);
	}

	val = GPT_SPI_NAND_SPI_DIV8 | GPT_SPI_NAND_SPI_MODE0 | GPT_SPI_NAND_SPI_RD_SIGNLE |\
	      GPT_SPI_NAND_SPI_WT_SIGNLE | GPT_SPI_NAND_SPI_WP_HIGH | GPT_SPI_NAND_SPI_HD_HIGH |\
										GPT_SPI_NAND_SPI_IDLE;
	writel(val, &g_sf->base->spi_ctrl);
	writel(0x000d0500, &g_sf->base->pre_buf_raddr);
	writel(0x010c0021, &g_sf->base->nfc_ctrl);

	val = readl(&sf->base->irq_status);
	int timeout = 200000;
	while(!(val & 0x1)) {
		if (!timeout--)
			break;
		val = readl(&sf->base->irq_status);
	}
	gpt_sf_nand_check_status(sf);

	return 0;
}

static int gpt_sf_set_feature(struct gpt_sf_nand *sf, u8 addr, u8 *buf)
{
	u16 instr[128] = {0};
	int index = 0, i;
	volatile int *p;
	volatile int *instr_base = sf->instruction;
	int val;

	val = (addr << 8) | buf[0];
	writel(val, sf->row0);
	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_SET_FEATURE;
	instr[index++] = GPT_INSTR_SF_NAND_AWR | GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE2;
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;

	p = (volatile int *)instr;
	for (i = 0; i < index / 2; i++ ) {
		writel(*p++, instr_base++);
	}

	val = GPT_SPI_NAND_SPI_DIV8 | GPT_SPI_NAND_SPI_MODE0 | GPT_SPI_NAND_SPI_RD_SIGNLE |\
	      GPT_SPI_NAND_SPI_WT_SIGNLE | GPT_SPI_NAND_SPI_WP_HIGH | GPT_SPI_NAND_SPI_HD_HIGH | GPT_SPI_NAND_SPI_IDLE;
	writel(val, &sf->base->spi_ctrl);// spi config
	writel(((long)buf & 0xffffffff), &g_sf->base->axi_rand_raddr);
	writel(((long)buf >> 32), &g_sf->base->axi_raddr_rand_hi);
	val = GPT_SPI_NAND_NFC_START | GPT_SPI_NAND_NFC_OP_READ | GPT_SPI_NAND_NFC_ECCERR_STOP |\
			1 << GPT_SPI_NAND_CHECK_READY_OFFSET | 0x3c << GPT_SPI_NAND_CHECK_ERR_OFFSET;
	writel(val, &sf->base->nfc_ctrl);// start nfc

	val = readl(&sf->base->irq_status);
	int timeout = 2000000;
	while (!(val & 0x1)) {
		while (!timeout--) {
			printf("Set feature timeout....\n");
			return -1;
		}
		val = readl(&sf->base->irq_status);
	}
	gpt_sf_nand_check_status(g_sf);

	return 0;
}

#if 0
static int gpt_sf_get_feature(struct gpt_sf_nand *sf, u8 addr, u8 *buf)
{
	int index = 0, i;
	volatile int *p;
	volatile int *instr_base = sf->instruction;
	int val;

	buf[0] = 0;
	writel(addr, sf->row0);

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_GET_FEATURE;
	instr[index++] = GPT_INSTR_SF_NAND_AWR | GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE1;
	instr[index++] = GPT_INSTR_SF_NAND_DRADM | 1;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;

	p = (volatile int *)instr;
	for (i = 0; i < index / 2 + 1; i++ ) {
		writel(*p++, instr_base++);
	}

	val = GPT_SPI_NAND_SPI_DIV100 | GPT_SPI_NAND_SPI_MODE0 | GPT_SPI_NAND_SPI_RD_SIGNLE |\
	      GPT_SPI_NAND_SPI_WT_SIGNLE | GPT_SPI_NAND_SPI_WP_HIGH | GPT_SPI_NAND_SPI_HD_HIGH | GPT_SPI_NAND_SPI_IDLE;
	writel(val, &sf->base->spi_ctrl);// spi config
	writel((long)buf, &sf->base->axi_rand_waddr);// set axi_waddr
	val = GPT_SPI_NAND_NFC_START | GPT_SPI_NAND_NFC_OP_READ | GPT_SPI_NAND_NFC_ECCERR_STOP | 1 << GPT_SPI_NAND_CHECK_READY_OFFSET | 0x3c << GPT_SPI_NAND_CHECK_ERR_OFFSET;
	writel(val, &sf->base->nfc_ctrl);// start nfc

	int timeout = 2000000;
	val = readl(&sf->base->irq_status);
	while (!(val & GPT_SPI_NAND_NFC_READY)) {
		val = readl(&sf->base->irq_status);
		if (!timeout--) {
			printf("read timeout.\n");
			break;
		}
	}
	gpt_sf_nand_check_status(g_sf);

	return 0;
}

static void dump_buf(char *buf, int len)
{
	int i = 0;
	int *p = (int *)buf;
//	char *p = buf;
	for (i = 0; i < len; i++)
		printf("%.8x%c",p[i], i % 4 == 3 ?'\n':' ');
	printf("\n");
}
#endif

static int gpt_spinand_read_op(loff_t from, int page, size_t len, uint8_t *buf)
{
	int index = 0, i;
	volatile int *p;
	int val;
	int row, column;
	int dummy;
	int rd_cmd;
	int addr_len = 0;
	int loop = 0;
	u8 tmp[8] = {1};

	volatile int *instr_base = g_sf->instruction;
	g_sf->nand.page_shift = 11;
	row = from >> g_sf->nand.page_shift;
	column = from % (1 << g_sf->nand.page_shift);

	g_sf->rd_mode = GPT_SPI_NAND_SPI_RD_SIGNLE;
	switch (g_sf->rd_mode) {
		case GPT_SPI_NAND_SPI_RD_SIGNLE:
			rd_cmd = GPT_CMD_SF_NAND_READ_CACHE;
			dummy = 0;
			addr_len = GPT_INSTR_SF_NAND_BYTE3;
			writel(column, g_sf->column0);
			break;
		case GPT_SPI_NAND_SPI_RD_DUAL:
			dummy = 1;
			addr_len = GPT_INSTR_SF_NAND_BYTE2;
			if ((g_nand_type->id & 0xffff00) == (0xc848b1 & 0xffff00))
				addr_len = GPT_INSTR_SF_NAND_BYTE3;
			else
				addr_len = GPT_INSTR_SF_NAND_BYTE2;

			rd_cmd = GPT_CMD_SF_NAND_READ_CACHE2;
			writel(column, g_sf->column0);
			break;
		case GPT_SPI_NAND_SPI_RD_QUAD:
			tmp[0] = GPT_SF_NAND_FEATURE_ECC_EN | GPT_SF_NAND_FEATURE_QE;
			gpt_sf_set_feature(g_sf, 0xb0, tmp);
			dummy = 1;
			addr_len = GPT_INSTR_SF_NAND_BYTE2;
			if ((g_nand_type->id & 0xffff00) == (0xc848b1 & 0xffff00) || g_nand_type->id == 0x21abef)
				addr_len = GPT_INSTR_SF_NAND_BYTE3;
			else
				addr_len = GPT_INSTR_SF_NAND_BYTE2;

			rd_cmd = GPT_CMD_SF_NAND_READ_CACHE4;
			writel(column, g_sf->column0);
			break;
		case GPT_SPI_NAND_SPI_RD_IO_DUAL:
			dummy = 1;
			addr_len = GPT_INSTR_SF_NAND_BYTE2;
			tmp[0] = GPT_SF_NAND_FEATURE_ECC_EN | 0x8;
			gpt_sf_set_feature(g_sf, 0xb0, tmp);
			rd_cmd = GPT_CMD_SF_NAND_DUAL_IO;
			writel(column, g_sf->column0);
			break;
		case GPT_SPI_NAND_SPI_RD_IO_QUAD:
			if ((g_nand_type->id & 0xffff00) == (0xc848b1 & 0xffff00) || g_nand_type->id == 0xc1a1 || g_nand_type->id == 0xe1c8 ||  g_nand_type->id == 0xe1a1)
				dummy = 1;
			else
				dummy = 2;
			tmp[0] = GPT_SF_NAND_FEATURE_ECC_EN | GPT_SF_NAND_FEATURE_QE | 0x8;
			gpt_sf_set_feature(g_sf, 0xb0, tmp);
			addr_len = GPT_INSTR_SF_NAND_BYTE2;
			rd_cmd = GPT_CMD_SF_NAND_QUAD_IO;
			writel(column, g_sf->column0);
			break;
		default:
			printf("spi read mode error: mode: %d ...\n", g_sf->rd_mode);
			return -1;
	}
	writel(row, g_sf->row0);

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_PAGE_READ;
	instr[index++] = GPT_INSTR_SF_NAND_AWR | (GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE3);//0x2;
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_MOVTPCNT | 200;
	loop = index;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_TPCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_JTPCNTNZ | loop;
	instr[index++] = GPT_INSTR_SF_NAND_SCH;
	instr[index++] = GPT_INSTR_SF_NAND_CKECKSTATUS | 0x4;
	loop = index;
	instr[index++] = GPT_INSTR_SF_NAND_JCKECCERR | (loop + 8);

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | rd_cmd;
	instr[index++] = GPT_INSTR_SF_NAND_AWC | (GPT_INSTR_SF_NAND_CLASS0 | addr_len);//0x1;
	if (dummy) {
		instr[index++] = GPT_INSTR_SF_NAND_WRDUMMY;
		if (dummy > 1)
			instr[index++] = GPT_INSTR_SF_NAND_WRDUMMY | (dummy - 1);
	}

	instr[index++] = GPT_INSTR_SF_NAND_DTRANS;
	instr[index++] = GPT_INSTR_SF_NAND_ARINC | 1;
	instr[index++] = GPT_INSTR_SF_NAND_PCNTDEC | 1;
	instr[index++] = GPT_INSTR_SF_NAND_JPCNTNZ | 0;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;
	p = (volatile int *)instr;
	if (index & 0x1)
		index++;
	for (i = 0; i < index / 2; i++ ) {
		writel(*p++, instr_base++);
	}

	writel((long)buf & 0xffffffff, &g_sf->base->axi_waddr);
	writel((long)buf >> 32, &g_sf->base->axi_waddr_hi);
	gpt_sf_start(g_sf, page, len);

	val = readl(&g_sf->base->irq_status);
	int timeout = 20000000;
	while(!(val & 0x1)) {
		if (!timeout--) {
			printf("Cur: %x \n", readl(&g_sf->base->cur_instruction));
			printf("read time out ................\n");
			writel(GPT_SPI_NAND_NFC_STOP, &g_sf->base->nfc_ctrl);
			break;
		}
		val = readl(&g_sf->base->irq_status);
	}

	if (val >> 2) {
		printf("Read: SF Nand Controller have some errors. nfc status is: 0x%x\n", val);
	}
	gpt_sf_nand_check_status(g_sf);

	return 0;
}

#ifndef	CONFIG_SPL_BUILD
static int gpt_spinand_write_op(loff_t to, int page, size_t len, const uint8_t *buf)
{
	int index = 0, i;
	volatile int *p;
	volatile int *instr_base = g_sf->instruction;
	int val, row, column;
	int wr_cmd = 0;
	u8 tmp[8] = {0};
	int loop;

	val = readl(&g_sf->base->nfc_status);
	row = to >> g_sf->nand.page_shift;
	column = to % (1 << g_sf->nand.page_shift);

	switch (g_sf->wt_mode) {
		case GPT_SPI_NAND_SPI_WT_SIGNLE:
			tmp[0] = GPT_SF_NAND_FEATURE_ECC_EN | 0x8;
			gpt_sf_set_feature(g_sf, 0xb0, tmp);
			wr_cmd = GPT_CMD_SF_NAND_PROGRAM_LOAD;
			break;
		case GPT_SPI_NAND_SPI_WT_QUAD:
			tmp[0] = GPT_SF_NAND_FEATURE_ECC_EN | GPT_SF_NAND_FEATURE_QE | 0x8;
			gpt_sf_set_feature(g_sf, 0xb0, tmp);
			wr_cmd = GPT_CMD_SF_NAND_PROGRAM_LOAD4;
			break;
		case GPT_SPI_NAND_SPI_WT_IO_QUAD:
			tmp[0] = GPT_SF_NAND_FEATURE_ECC_EN | GPT_SF_NAND_FEATURE_QE | 0x8;
			gpt_sf_set_feature(g_sf, 0xb0, tmp);
			wr_cmd = GPT_CMD_SF_NAND_PROGRAM_LOAD_RANDOM_QUAD;
			break;
		default:
			printf("not support write mode \n");
			break;
	}

	writel(column, g_sf->column0);
	writel(row, g_sf->row0);

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_WRITE_ENABLE;
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_MOVTPCNT | 250;
	loop = index;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_TPCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_JTPCNTNZ | loop;

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | wr_cmd;
	instr[index++] = GPT_INSTR_SF_NAND_AWC | (GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE2);//0x1;
	instr[index++] = GPT_INSTR_SF_NAND_DTRANS | 4;
	instr[index++] = GPT_INSTR_SF_NAND_MOVTPCNT | 250;
	loop = index;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_TPCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_JTPCNTNZ | loop;

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_PROGRAM_EXEC;
	instr[index++] = GPT_INSTR_SF_NAND_AWR | (GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE3);//0x2;
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_MOVTPCNT | 250;
	loop = index;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_TPCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_JTPCNTNZ | loop;
	instr[index++] = GPT_INSTR_SF_NAND_SCH;

	instr[index++] = GPT_INSTR_SF_NAND_CKECKSTATUS | (loop + 5);
	instr[index++] = GPT_INSTR_SF_NAND_JCKFD | 0x10;
	instr[index++] = GPT_INSTR_SF_NAND_ARINC | 0x1;
	instr[index++] = GPT_INSTR_SF_NAND_PCNTDEC | 1;
	instr[index++] = GPT_INSTR_SF_NAND_JPCNTNZ;// | 3;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;

	p = (volatile int *)instr;
	if (index & 0x1)
		index++;
	for (i = 0; i < index / 2; i++ ) {
		writel(*p++, instr_base++);
	}
	writel(((long)buf & 0xffffffff), &g_sf->base->axi_raddr);
	writel(((long)buf >> 32), &g_sf->base->axi_raddr_hi);

	gpt_sf_start(g_sf, page, len);
	int timeout = 20000000;
	val = readl(&g_sf->base->irq_status);
	while (!(val & GPT_SPI_NAND_NFC_READY)) {
		val = readl(&g_sf->base->irq_status);
		if (timeout-- <= 0) {
			writel(GPT_SPI_NAND_NFC_STOP, &g_sf->base->nfc_ctrl);
			printf("nfc transfer time out ...\n");
			break;
		}
	}
	if (val >> 2) {
		printf("Write: SF Nand Controller have some errors. nfc status is: 0x%x\n", val);
	}

	gpt_sf_nand_check_status(g_sf);
	return 0;
}

static int gpt_spinand_read_id(struct gpt_sf_nand *sf, char *buf)
{
	int i;
	volatile int *p;
	int index = 0;
	volatile int *instr_base = sf->instruction;
	int val;

	writel(0, sf->row0);
	instr[index++] = (GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_READ_ID);
	instr[index++] = GPT_INSTR_SF_NAND_AWR | GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE1;
	instr[index++] = GPT_INSTR_SF_NAND_DRADM | 3;
	instr[index++] = GPT_INSTR_SF_NAND_SCH;
	instr[index] = GPT_INSTR_SF_NAND_HALT;

	p = (volatile int *)instr;
	for (i = 0; i < index / 2 + 1; i++ ) {
		writel(*p++, instr_base++);
	}

	val = GPT_SPI_NAND_SPI_WP_HIGH | GPT_SPI_NAND_SPI_HD_HIGH | GPT_SPI_NAND_SPI_DIV8 | GPT_SPI_NAND_SPI_IDLE;
	writel(val, &sf->base->spi_ctrl);// spi config
	writel(((long)buf & 0xffffffff), &g_sf->base->axi_rand_waddr);
	writel(((long)buf >> 32), &g_sf->base->axi_waddr_rand_hi);
	val = GPT_SPI_NAND_NFC_START | GPT_SPI_NAND_NFC_OP_READ | GPT_SPI_NAND_NFC_ECCERR_STOP | \
			1 << GPT_SPI_NAND_CHECK_READY_OFFSET | 0xc << GPT_SPI_NAND_CHECK_ERR_OFFSET;
	writel(val, &sf->base->nfc_ctrl);// start nfc

	int timeout = 2000000;
	val = readl(&sf->base->irq_status);
	while (!(val & GPT_SPI_NAND_NFC_READY)) {
		val = readl(&sf->base->irq_status);
		if (!timeout--) {
			printf("read timeout.\n");
			return -1;
		}
	}

	gpt_sf_nand_check_status(sf);
	return 0;
}

static int gpt_spinand_read(struct mtd_info *mtd, loff_t from, size_t len,
					size_t *retlen, uint8_t *buf)
{
	int ret;
	int page_num = len >> g_sf->nand.page_shift;

	*retlen = 0;
	if (page_num > 1) {
		ret = gpt_spinand_read_op(from, page_num, 1 << g_sf->nand.page_shift, buf);
		if (ret)
			return ret;
		*retlen = page_num << g_sf->nand.page_shift;
		buf = buf + *retlen;
	}
	len &= ((1 << g_sf->nand.page_shift) - 1);
	if (len) {
		if(page_num)
			page_num++;
		ret = gpt_spinand_read_op(from + (page_num << g_sf->nand.page_shift), 1, len, buf);
		if (ret)
			return ret;
		*retlen += len;
	}

	return ret;
}

static int gpt_spinand_write(struct mtd_info *mtd, loff_t to, size_t len,
		                                size_t *retlen, const u_char *buf)
{
	int ret;
	int page_num = len >> g_sf->nand.page_shift;
	size_t size = 0;

//	*retlen = 0;
	if (page_num) {
		while (page_num--) {
			ret = gpt_spinand_write_op(to, 1, 1 << g_sf->nand.page_shift, buf);
			if (ret)
				return -1;
			to += 1 << g_sf->nand.page_shift;
			buf += 1 << g_sf->nand.page_shift;
			size += 1 << g_sf->nand.page_shift;
		}
	}

	len &= ((1 << g_sf->nand.page_shift) - 1);
	if (len) {
		ret = gpt_spinand_write_op(to, 1, len, buf);
		if (ret)
			return -1;
		size += len;
	}
#if 0
	if (page_num > 1) {
		ret = gpt_spinand_write_op(to, page_num, 1 << g_sf->nand.page_shift, buf);
		if (ret)
			return ret;
		buf = buf + (page_num << g_sf->nand.page_shift);
	}
	len &= ((1 << g_sf->nand.page_shift) - 1);
	if (len) {
		if(page_num)
			page_num++;
		ret = gpt_spinand_write_op(to + (page_num << g_sf->nand.page_shift), 1, len, buf);
		if (ret)
			return ret;
	}
#endif

//	*retlen = size;
	return 0;
}

static int gpt_spinand_erase(struct mtd_info *mtd, struct erase_info *erase)
{
	int index = 0, i;
	volatile int *p;
	volatile int *instr_base = g_sf->instruction;
	int val;

	val = readl(&g_sf->base->nfc_status);
	writel(erase->addr / (1 << g_sf->nand.page_shift), g_sf->row0);
	writel(0x0000a000, g_sf->row1);

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_WRITE_ENABLE;
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_MOVTPCNT | 250;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_TPCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_JTPCNTNZ | 3;

	instr[index++] = GPT_INSTR_SF_NAND_CMDW | GPT_CMD_SF_NAND_BLOCK_ERASE;
	instr[index++] = GPT_INSTR_SF_NAND_AWR | (GPT_INSTR_SF_NAND_CLASS0 | GPT_INSTR_SF_NAND_BYTE3);
	instr[index++] = GPT_INSTR_SF_NAND_LOADSTART;
	instr[index++] = GPT_INSTR_SF_NAND_MOVTPCNT | 250;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_TPCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_JTPCNTNZ | 10;
	instr[index++] = GPT_INSTR_SF_NAND_SCH;
	instr[index++] = GPT_INSTR_SF_NAND_CKECKSTATUS | 10;

	instr[index++] = GPT_INSTR_SF_NAND_ARBINC;
	instr[index++] = GPT_INSTR_SF_NAND_BCNTDEC;
	instr[index++] = GPT_INSTR_SF_NAND_NOP | 0xff;
	instr[index++] = GPT_INSTR_SF_NAND_JBCNTNZ | 0;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;
	instr[index++] = GPT_INSTR_SF_NAND_HALT;

	p = (volatile int *)instr;
	if (index & 0x1)
		index++;
	for (i = 0; i < index / 2; i++ ) {
		writel(*p++, instr_base++);
	}

	writel(2, &g_sf->base->block_num);
	val = GPT_SPI_NAND_SPI_DIV8 | GPT_SPI_NAND_SPI_MODE0 | GPT_SPI_NAND_SPI_RD_SIGNLE | GPT_SPI_NAND_SPI_RD_TUNE | \
	      GPT_SPI_NAND_SPI_WT_SIGNLE | GPT_SPI_NAND_SPI_WP_HIGH | GPT_SPI_NAND_SPI_HD_HIGH | GPT_SPI_NAND_SPI_IDLE;
	writel(val, &g_sf->base->spi_ctrl);
	val = GPT_SPI_NAND_NFC_START | GPT_SPI_NAND_NFC_OP_READ | GPT_SPI_NAND_NFC_ECCERR_STOP |\
			1 << GPT_SPI_NAND_CHECK_READY_OFFSET | 0x3c << GPT_SPI_NAND_CHECK_ERR_OFFSET;
	writel(val,  &g_sf->base->nfc_ctrl);

	val = readl(&g_sf->base->irq_status);
	int timeout = 2000000;
	while(!(val & 0x1)) {
		if (!timeout--) {
			writel(GPT_SPI_NAND_NFC_STOP, &g_sf->base->nfc_ctrl);
			printf("cur: %x\n", readl(&g_sf->base->cur_instruction));
			printf("Erase timeout...\n");
			break;
		}
		val = readl(&g_sf->base->irq_status);
	}

	gpt_sf_nand_check_status(g_sf);

	return 0;
}

static void gpt_spinand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct gpt_sf_nand *gpt = mtd_to_gpt(mtd);

	memcpy(buf, gpt->buf + gpt->buf_ptr, len);
	gpt->buf_ptr += len;
}

static uint8_t gpt_spinand_read_byte(struct mtd_info *mtd)
{
	struct gpt_sf_nand *gpt = mtd_to_gpt(mtd);
	u8 data;

	data = gpt->buf[gpt->buf_ptr];
	gpt->buf_ptr++;
	return data;
}

static void gpt_spiand_cmdfunc(struct mtd_info *mtd, unsigned int command,
		int column, int page)
{
	struct gpt_sf_nand *g_sf = mtd_to_gpt(mtd);

	switch (command) {
		case NAND_CMD_READ1:
		case NAND_CMD_READ0:
			g_sf->buf_ptr = 0;
//			spinand_read_page(info->spi, page, 0x0, 0x840, state->buf);
			break;
			/* READOOB reads only the OOB because no ECC is performed. */
		case NAND_CMD_READOOB:
			g_sf->buf_ptr = 0;
//			spinand_read_page(info->spi, page, 0x800, 0x40, state->buf);
			break;
		case NAND_CMD_RNDOUT:
			g_sf->buf_ptr = column;
			break;
		case NAND_CMD_READID:
			g_sf->buf_ptr = 0;
			gpt_spinand_read_id(g_sf, (char *)g_sf->buf);
			break;
		case NAND_CMD_PARAM:
			printf("NAND_CMD_PARAM: \n");
			g_sf->buf_ptr = 0;
			break;
			/* ERASE1 stores the block and page address */
		case NAND_CMD_ERASE1:
//			spinand_erase_block(info->spi, page);
			break;
			/* ERASE2 uses the block and page address from ERASE1 */
		case NAND_CMD_ERASE2:
			break;
			/* SEQIN sets up the addr buffer and all registers except the length */
		case NAND_CMD_SEQIN:
//			state->col = column;
//			state->row = page;
//			state->buf_ptr = 0;
			break;
			/* PAGEPROG reuses all of the setup from SEQIN and adds the length */
		case NAND_CMD_PAGEPROG:
			printf("NAND_CMD_PAGEPROG: \n");
//			spinand_program_page(info->spi, state->row, state->col,
//					state->buf_ptr, state->buf);
			break;
		case NAND_CMD_STATUS:
			printf("NAND_CMD_STATUS: \n");
//			spinand_get_otp(info->spi, state->buf);
//			if (!(state->buf[0] & 0x80))
//				state->buf[0] = 0x80;
//			state->buf_ptr = 0;
//			break;
			/* RESET command */
		case NAND_CMD_RESET:
//			if (wait_till_ready(info->spi))
//				dev_err(&info->spi->dev, "WAIT timedout!!!\n");
//			/* a minimum of 250us must elapse before issuing RESET cmd*/
//			udelay(250);
//			spinand_reset(info->spi);
			break;
		default:
			dev_err(&mtd->dev, "Unknown CMD: 0x%x\n", command);
	}
}

static void gpt_spinand_select_chip(struct mtd_info *mtd, int dev)
{
}
#endif

#ifdef	CONFIG_SPL_BUILD
static int spl_read(uint32_t from, uint32_t size, void *dst)
{
	int ret;
	int page_shift = 11;

	int page_num = size >> page_shift;

	while (page_num--) {
		ret = gpt_spinand_read_op(from, 1, 1 << page_shift, g_sf->buf);
		if (ret)
			return ret;
		memcpy(dst, g_sf->buf, 1 << page_shift);
		dst += (1 << page_shift);
		from += (1 << page_shift);
	}

	size &= ((1 << page_shift) - 1);
	if (size) {
		if(page_num)
			page_num++;
		ret = gpt_spinand_read_op(from, 1, size, g_sf->buf);
		if (ret)
			return ret;
		memcpy(dst, g_sf->buf, size);
	}
#if 0
	if (page_num > 1) {
		ret = gpt_spinand_read_op(from, page_num, 1 << page_shift, dst);
		if (ret)
			return ret;
	}
	size &= ((1 << page_shift) - 1);
	if (size) {
		if(page_num)
			page_num++;
		ret = gpt_spinand_read_op(from + (page_num << page_shift), 1, size, dst);
		if (ret)
			return ret;
	}
#endif

	return 0;
}

int nand_spl_load_image(uint32_t offs, unsigned int size, void *dst)
{
	spl_read(offs, size, dst);
	return 0;

}
void nand_deselect(void) {}

int nand_default_bbt(struct mtd_info *mtd)
{
	        return 0;
}

void nand_init(void)
{
	board_nand_init();
}
#endif

static int gpt_nand_hw_init(void)
{
	int val;
	gpt_sf_nand_pre_buf_cmd_set(g_sf);

	gpt_sf_nand_reset(g_sf);

	val = GPT_SF_NAND_FEATURE_ECC_EN | 1 << 3;
	gpt_sf_set_feature(g_sf, 0xb0, (u8 *)&val);

	val = 0;
	gpt_sf_set_feature(g_sf, 0xa0, (u8 *)&val);
	writel(1, &g_sf->base->irq_clr);

	return 0;
}

static int gpt_nand_board_init(void)
{
	int ret;
#ifdef	CONFIG_SPL_BUILD
	g_sf = (void *)0x220000000;//(void *)(CONFIG_SPL_NAND_BUF + 0x1000);//kzalloc(sizeof(struct gpt_sf_nand), GFP_KERNEL);
#else
	g_sf = kzalloc(sizeof(struct gpt_sf_nand), GFP_KERNEL);
#endif
	if (!g_sf)
		return -ENOMEM;

#ifdef	CONFIG_SPL_BUILD
	g_sf->buf = (void *)CONFIG_SPL_NAND_BUF;
#else
	g_sf->buf = kzalloc(4096, GFP_KERNEL);
#endif
	if(!g_sf->buf) {
		printf("Kzalloc failed...\n");
		free(g_sf);
		return -1;
	}

	/*
	 * In the future, these base addresses should be taken from
	 * Device Tree or platform data.
	 */
	printf("gpt_nand_board_init \n");
	g_sf->base = (void *)(CONFIG_SYS_NAND_BASE + GPT_SPI_REG_BASE);
	g_sf->cmd = (void*)PRE_BUFF_CMD_BASE;
	g_sf->column0 = (void *)PRE_BUFF_COLUMN0_BASE;
	g_sf->column1 = (void *)PRE_BUFF_COLUMN1_BASE;
	g_sf->column2 = (void *)PRE_BUFF_COLUMN2_BASE;
	g_sf->column3 = (void *)PRE_BUFF_COLUMN3_BASE;
	g_sf->row0 = (void *)PRE_BUFF_ROW0_BASE;
	g_sf->row1 = (void *)PRE_BUFF_ROW1_BASE;
	g_sf->row2 = (void *)PRE_BUFF_ROW2_BASE;
	g_sf->row3 = (void *)PRE_BUFF_ROW3_BASE;
	g_sf->instruction = (void *)PRE_BUFF_INSTR_BASE;
	g_sf->wt_mode = GPT_SPI_NAND_SPI_WT_SIGNLE;

	ret = gpt_nand_hw_init();

#ifndef	CONFIG_SPL_BUILD
	/*
	 * If CONFIG_SYS_NAND_SELF_INIT is defined, each driver is responsible
	 * for instantiating struct nand_chip, while drivers/mtd/nand/nand.c
	 * still provides a "struct mtd_info nand_info" instance.
	 */
	g_sf->mtd = &nand_info[0];

	g_sf->mtd->name = "gpt-nand";
	g_sf->mtd->owner = THIS_MODULE;
	g_sf->mtd->priv = &g_sf->nand;

	/* register the driver with the NAND core subsystem */
	g_sf->nand.cmdfunc = gpt_spiand_cmdfunc;
	g_sf->nand.read_byte = gpt_spinand_read_byte;
	g_sf->nand.read_buf = gpt_spinand_read_buf;
	g_sf->nand.waitfunc = NULL;
	g_sf->nand.select_chip = gpt_spinand_select_chip;

	g_sf->nand.options   |= NAND_NO_SUBPAGE_WRITE;
	g_sf->nand.ecc.mode = NAND_ECC_HW;
	g_sf->nand.ecc.size = 512;//CONFIG_NAND_DENALI_ECC_SIZE;

	g_sf->mtd->_read = gpt_spinand_read;
	g_sf->mtd->_erase = gpt_spinand_erase;//nand_erase;
	g_sf->mtd->_write = gpt_spinand_write;//nand_write;
	g_sf->mtd->writebufsize = 2048;//mtd->writesize;
	g_sf->mtd->_block_isbad = NULL;
	g_sf->mtd->flags =  MTD_CAP_RAM;

	/*
	 * scan for NAND devices attached to the controller
	 * this is the first stage in a two step process to register
	 * with the nand subsystem
	 */
	if (nand_scan_ident(g_sf->mtd, 1, NULL)) {
		free(g_sf);
		return -1;
	}

	// tmp set
	g_sf->nand.page_shift = 11;
	g_sf->mtd->writesize = 2048;
	ret = nand_register(0);
	printf("gpt spi nand init done ...\n");
#else
	g_sf->nand.page_shift = 11;
	g_sf->mtd->writesize = 2048;
#endif

	return ret;
}

void board_nand_init(void)
{
	if (gpt_nand_board_init() < 0) {

	}
}
