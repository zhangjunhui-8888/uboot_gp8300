#ifndef	CHASE_SPI_NAND_H
#define	CHASE_SPI_NAND_H

#include <spi_flash.h>

#define	GPT_SPI_NAND_SPI_IDLE		1 << 0
/*spi controller reg */
#define	GPT_SPI_NAND_SPI_DIV_OFFSET	1
#define	GPT_SPI_NAND_SPI_DIV100		0 << 1
#define	GPT_SPI_NAND_SPI_DIV32		1 << 1
#define	GPT_SPI_NAND_SPI_DIV16		2 << 1
#define	GPT_SPI_NAND_SPI_DIV8		3 << 1
#define	GPT_SPI_NAND_SPI_DIV4		4 << 1
#define	GPT_SPI_NAND_SPI_DIV2		5 << 1

#define	GPT_SPI_NAND_SPI_MODE_OFFSET	4
#define	GPT_SPI_NAND_SPI_MODE0		0 << 4
#define	GPT_SPI_NAND_SPI_MODE3		3 << 4

#define	GPT_SPI_NAND_SPI_RD_OFFSET	8
#define	GPT_SPI_NAND_SPI_RD_SIGNLE	0 << 8
#define	GPT_SPI_NAND_SPI_RD_DUAL	1 << 8
#define	GPT_SPI_NAND_SPI_RD_QUAD	2 << 8
#define	GPT_SPI_NAND_SPI_RD_IO_DUAL	3 << 8
#define	GPT_SPI_NAND_SPI_RD_IO_QUAD	4 << 8

#define	GPT_SPI_NAND_SPI_WT_OFFSET	12
#define	GPT_SPI_NAND_SPI_WT_SIGNLE	0 << 12
#define	GPT_SPI_NAND_SPI_WT_QUAD	2 << 12
#define	GPT_SPI_NAND_SPI_WT_IO_QUAD	4 << 12

#define	GPT_SPI_NAND_SPI_RD_TUNE_OFFSET	16
#define	GPT_SPI_NAND_SPI_RD_TUNE	0 << 16
#define	GPT_SPI_NAND_SPI_RD_TUNE_DELAY1	1 << 16
#define	GPT_SPI_NAND_SPI_RD_TUNE_DELAY2	2 << 16
#define	GPT_SPI_NAND_SPI_RD_TUNE_DELAY3	3 << 16
#define	GPT_SPI_NAND_SPI_WP_LOW		0 << 18
#define	GPT_SPI_NAND_SPI_WP_HIGH	1 << 18
#define	GPT_SPI_NAND_SPI_HD_LOW		0 << 19
#define	GPT_SPI_NAND_SPI_HD_HIGH	1 << 19

/* nfc controller reg */
#define	GPT_SPI_NAND_NFC_START		1 << 0
#define	GPT_SPI_NAND_NFC_STOP		1 << 1
#define	GPT_SPI_NAND_NFC_OP_READ	0 << 4
#define	GPT_SPI_NAND_NFC_OP_WRITE	1 << 4
#define	GPT_SPI_NAND_NFC_ECCERR_STOP	1 << 6
#define	GPT_SPI_NAND_NFC_DEBUG_EN	1 << 7
#define	GPT_SPI_NAND_CHECK_ERR_OFFSET	16
#define	GPT_SPI_NAND_CHECK_READY_OFFSET	24

/* nfc status reg */
#define	GPT_SPI_NAND_NFC_READY		1 << 0
#define	GPT_SPI_NAND_NFC_OK		1 << 1
#define	GPT_SPI_NAND_NFC_ERR		1 << 2
#define	GPT_SPI_NAND_CHECK_FAILED	1 << 8
#define	GPT_SPI_NAND_AXI_ERROR		1 << 10
#define	GPT_SPI_NAND_ECC_ERROR		1 << 11
#define	GPT_SPI_NAND_ECC_NOERROR	0 << 16
#define	GPT_SPI_NAND_ECC_ERR_CORRECTED	1 << 16
#define	GPT_SPI_NAND_ECC_MULTI		2 << 16
#define	GPT_SPI_NAND_ECC_ERR_CORRECTED8	3 << 16
#define	GPT_SPI_NAND_AXI_ERROR_RRESP_OFFSET	24
#define	GPT_SPI_NAND_AXI_ERROR_BRESP_OFFSET	26

/* interrupt status reg */
#define	GPT_SPI_NAND_NFC_READY_INT	1 << 0
#define	GPT_SPI_NAND_CHECK_FAILED_INT	1 << 8
#define	GPT_SPI_NAND_AXI_ERROR_INT	1 << 10
#define	GPT_SPI_NAND_ECC_ERROR_INT	1 << 11

/* interrupt mask reg */
#define	GPT_SPI_NAND_NFC_READY_MASK	1 << 0
#define	GPT_SPI_NAND_CHECK_FAILED_MASK	1 << 8
#define	GPT_SPI_NAND_AXI_ERROR_MASK	1 << 9
#define	GPT_SPI_NAND_ECC_ERROR_MASK	1 << 10

/* interrupt clear reg */
#define	GPT_SPI_NAND_NFC_READY_CLR	1 << 0
#define	GPT_SPI_NAND_CHECK_FAILED_CLR	1 << 8
#define	GPT_SPI_NAND_AXI_ERROR_CLR	1 << 10
#define	GPT_SPI_NAND_ECC_ERROR_CLR	1 << 11

/* pre-buf raddr reg */
#define	GPT_SPI_NAND_CMD_SET_BASE_OFFSET	0
#define	GPT_SPI_NAND_ADRR_SET_BASE_OFFSET	8
#define	GPT_SPI_NAND_INST_SET_BASE_OFFSET	16

/* sch instruction reg */
#define	GPT_SPI_NAND_SCH_CMD_OFFSET		0
#define	GPT_SPI_NAND_SCH_ADDR_OFFSET		8
#define	GPT_SPI_NAND_NAND_STATUS_OFFSET		16

/* transcation control reg */
#define	GPT_SPI_NAND_SECTOR_SIZE_OFFSET		0
#define	GPT_SPI_NAND_WRFIFO_THRESHOLD_OFFSET	16
#define	GPT_SPI_NAND_WRFIFO_THRESHOLD_DEFAULT	512

/* axi write addr reg */
/* axi read addr reg  */
/* axi rand write addr reg */
/* axi rand read addr reg */
/* axi error addr reg */
/* page number reg */
/* block number reg */
/* block base addr reg */
/* debug instruction reg */
/* current instruction reg */

/* current fsm reg */
#define	GPT_SPI_NAND_INSTR_EXE_STATE_OFFSET	0
#define	GPT_SPI_NAND_ADDR_INSTR_STATE_OFFSET	4
#define	GPT_SPI_NAND_CONNECT_STATE_OFFSET	8
#define	GPT_SPI_NAND_SPI_STATE_OFFSET		12


#define	GPT_CMD_SF_NAND_INSTRUCTION		100

struct gpt_sf_reg {
	unsigned int spi_ctrl;  // 0x800
	unsigned int reverse0[3];
	unsigned int nfc_ctrl;  //0x810
	unsigned int reverse1[3];
	unsigned int nfc_status;        //0x820
	unsigned int irq_status;        // after mask
	unsigned int irq_mask;
	unsigned int irq_clr;
	unsigned int pre_buf_raddr;
	unsigned int sch_instruction;   //0x834
	unsigned int reverse2[2];
	unsigned int transfer_ctrl;     //0x840
	unsigned int axi_waddr;
	unsigned int axi_raddr;
	unsigned int axi_rand_waddr;
	unsigned int axi_rand_raddr;
	unsigned int axi_err_addr;
	unsigned int page_num;  //
	unsigned int block_num;
	unsigned int block_base;        //0x860
	unsigned int reverse3[3];
	unsigned int debug_instruction; //0x870
	unsigned int cur_instruction;
	unsigned int cur_fsm;
	unsigned int rev;
	unsigned int axi_waddr_hi;
	unsigned int axi_raddr_hi;
	unsigned int axi_waddr_rand_hi;
	unsigned int axi_raddr_rand_hi;
	unsigned int axi_err_addr_hi;
};

struct gpt_sf_set_data {
	int spi_div;
	int spi_mode;
	int spi_rd_mode;
	int spi_wr_mode;
	int spi_rd_tune;
	int wp;
	int hold;
	int wrfifo;
};

struct nand_type {
	int id;
	char *name;
	int page_size;
	int page_per_block;
	int blocks;
	int erase_size;
	int total_size;
	char is_support_write_quad_io;
	char is_support_program_loadx4;
	char is_support_read_dual_io;
	char is_support_read_quad_io;
};


struct gpt_sf_nand {
	struct gpt_sf_reg *base;
	struct spi_flash flash;
	struct mtd_info *mtd;
	struct nand_chip nand;
	u16 rd_mode;
	u16 wt_mode;
	int flash_bank;
	void *cmd;
	void *column0;
	void *column1;
	void *column2;
	void *column3;
	void *row0;
	void *row1;
	void *row2;
	void *row3;
	void *instruction;
	int buf_ptr;
	u8 *buf;
};

extern struct gpt_sf_set_data priv;
extern struct nand_type gpt_nand_type[];
struct nand_type *g_nand_type;

/* This array is very danderous, Never change.
 * If you must change you must flow gpt spi
 * controller, and MUST change below  enum,
 * and KEEP the ORDER
 * */
enum {
	GPT_CMD_SF_NAND_WRITE_ENABLE = 0x00,	// 0x06
	GPT_CMD_SF_NAND_WRITE_DISABLE,		// 0x04
	GPT_CMD_SF_NAND_GET_FEATURE,		// 0x0f
	GPT_CMD_SF_NAND_SET_FEATURE,		// 0x1f
	GPT_CMD_SF_NAND_PAGE_READ,		// 0x13
	GPT_CMD_SF_NAND_READ_CACHE,		// 0x03
	GPT_CMD_SF_NAND_READ_CACHE2,		// 0x3b
	GPT_CMD_SF_NAND_READ_CACHE4,		// 0x6b
	GPT_CMD_SF_NAND_DUAL_IO,		// 0xbb
	GPT_CMD_SF_NAND_QUAD_IO,		// 0xeb
	GPT_CMD_SF_NAND_READ_ID,		// 0x9f
	GPT_CMD_SF_NAND_PROGRAM_LOAD,		// 0x02
	GPT_CMD_SF_NAND_PROGRAM_LOAD4,		// 0x32
	GPT_CMD_SF_NAND_PROGRAM_EXEC,		// 0x10
	GPT_CMD_SF_NAND_PROGRAM_LOAD_RANDOM,	// 0x84
	GPT_CMD_SF_NAND_PROGRAM_LOAD_RANDOM4,	// 0xc4
	GPT_CMD_SF_NAND_PROGRAM_LOAD_RANDOM_QUAD,// 0x72
	GPT_CMD_SF_NAND_BLOCK_ERASE,		// 0xd8
	GPT_CMD_SF_NAND_RESET,			// 0xff
};

#define	GPT_INSTR_SF_NAND_CLASS0		0x0 << 3
#define	GPT_INSTR_SF_NAND_CLASS1		0x1 << 3
#define	GPT_INSTR_SF_NAND_CLASS2		0x2 << 3
#define	GPT_INSTR_SF_NAND_CLASS3		0x3 << 3

#define GPT_INSTR_SF_NAND_BYTE1			0x0 << 0
#define	GPT_INSTR_SF_NAND_BYTE2			0x1 << 0
#define	GPT_INSTR_SF_NAND_BYTE3			0x2 << 0
#define	GPT_INSTR_SF_NAND_BYTE4			0x3 << 0

#define	GPT_SF_NAND_FEATURE_BRWD		0x1 << 7
#define	GPT_SF_NAND_FEATURE_BP2			0x1 << 5
#define	GPT_SF_NAND_FEATURE_BP1			0x1 << 4
#define	GPT_SF_NAND_FEATURE_BP0			0x1 << 3
#define	GPT_SF_NAND_FEATURE_INV			0x1 << 2
#define	GPT_SF_NAND_FEATURE_CMP			0x1 << 1

#define	GPT_SF_NAND_FEATURE_OTP_PRT		0x1 << 7
#define	GPT_SF_NAND_FEATURE_OTP_EN		0x1 << 6
#define	GPT_SF_NAND_FEATURE_ECC_EN		0x1 << 4
#define	GPT_SF_NAND_FEATURE_QE			0x1 << 0

#define	GPT_SF_NAND_FEATURE_ECCS1		0x1 << 5
#define	GPT_SF_NAND_FEATURE_ECCS0		0x1 << 4
#define	GPT_SF_NAND_FEATURE_P_FAIL		0x1 << 3
#define	GPT_SF_NAND_FEATURE_E_FAIL		0x1 << 2
#define	GPT_SF_NAND_FEATURE_WEL			0x1 << 1
#define	GPT_SF_NAND_FEATURE_OIP			0x1 << 0

#define	GPT_SF_NAND_FEATURE_DS_S1		0x1 << 6
#define	GPT_SF_NAND_FEATURE_DS_S0		0x1 << 5

#define	GPT_SF_NAND_FEATURE_ECCSE1		0x1 << 5
#define	GPT_SF_NAND_FEATURE_ECCSE0		0x1 << 4

enum {
	GPT_INSTR_SF_NAND_AWR = 0x0100, /* load row from pre-buffer */
	GPT_INSTR_SF_NAND_AWC = 0x0200, /* load column from pre-buffer */
	GPT_INSTR_SF_NAND_ACCL = 0x0300, /* clear column */
	GPT_INSTR_SF_NAND_ARCL = 0x0400, /* clear row */
	GPT_INSTR_SF_NAND_ARINC = 0x0500, /* increase page row */
	GPT_INSTR_SF_NAND_ARBINC = 0x0600, /* increase block row */
	GPT_INSTR_SF_NAND_WRDUMMY = 0x0700, /* dummy byte */
	GPT_INSTR_SF_NAND_CMDW = 0x1000, /* load cmd from pre-buffer */
	GPT_INSTR_SF_NAND_DTRANS = 0x2000, /* start transfer data, low 8 bit is number*/
	GPT_INSTR_SF_NAND_DRADM = 0x2200, /* */
	GPT_INSTR_SF_NAND_DRADM_INV = 0x2300, /* transfer random data */
	GPT_INSTR_SF_NAND_SCH = 0x3000, /* check status, low is status val */
	GPT_INSTR_SF_NAND_LOADSTART = 0x4000, /* start nandflash operation */
	GPT_INSTR_SF_NAND_JCKFD = 0x5000, /* jump when chack failed, low 8 bit is instruction index */
	GPT_INSTR_SF_NAND_JPCNTNZ = 0x5300, /* jump when transfer page counter is not zero */
	GPT_INSTR_SF_NAND_JBCNTNZ = 0x5400, /* jump when transfer bloack counter is not zero */
	GPT_INSTR_SF_NAND_JTPCNTNZ = 0x5500, /* jump when tmp counter is npt zero */
	GPT_INSTR_SF_NAND_JCKECCERR = 0x5600, /* jump when ecc error */
	GPT_INSTR_SF_NAND_JMP = 0x5700, /* jump not need any conditional */
	GPT_INSTR_SF_NAND_CKECKSTATUS = 0x5800, /**/
	GPT_INSTR_SF_NAND_MOVTPCNT = 0x6000, /* move initial value to tmp coumter */
	GPT_INSTR_SF_NAND_TPCNTDEC = 0x6100, /* tmp counter decrement */
	GPT_INSTR_SF_NAND_PCNTDEC = 0x6200, /* transfer page counter decrement*/
	GPT_INSTR_SF_NAND_BCNTDEC = 0x6300, /* transfer block counter decrement */
	GPT_INSTR_SF_NAND_NOP = 0x7000, /* number of cyscles insert */
	GPT_INSTR_SF_NAND_HALT = 0x8000, /* stop nfc */
	GPT_INSTR_SF_NAND_MCADD = 0xe000, /* modify column address of nand flash */
	GPT_INSTR_SF_NAND_MRADD = 0xe100, /* modify row address of nandflash */
};

struct gpt_sf_nand *gpt_sf_nand_init(void);
int gpt_sf_nand_cmd(struct gpt_sf_nand *sf, u8 cmd, void *response, size_t len);
int gpt_sf_nand_write_random(struct spi_flash *flash, int s_page, int d_page, int offset, size_t len, const void *buf);

#endif	/* CHASE_SPI_NAND_H */

