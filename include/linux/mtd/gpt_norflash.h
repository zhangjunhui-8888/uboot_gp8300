#ifndef __GPT_NORFLASH__
#define __GPT_NORFLASH__
#include <common.h>
#include <asm/io.h>

/*spi sfc_ctl register*/
#define GPT_SPI_CLK_DIV02	(0x5 << 1)
#define GPT_SPI_CLK_DIV04	(0x4 << 1)
#define GPT_SPI_CLK_DIV08	(0x3 << 1)
#define GPT_SPI_CLK_DIV16	(0x2 << 1)
#define GPT_SPI_CLK_DIV32	(0x1 << 1)
#define GPT_SPI_CLK_DIV100	(0x0 << 1)
/*spi mode select*/
#define GPT_SPI_MODE0		(0x0 << 4)
#define GPT_SPI_MODE3		(0x3 << 4)
#define GPT_SPI_EN4BT_ADDR	(0x1 << 6)
#define GPT_SPI_EN3BT_ADDR	(0x0 << 6)
/*spi rd mode select*/
#define GPT_SPI_RD_MODE_SIG	(0x0 << 8)
#define GPT_SPI_RD_MODE_DUAL_OT	(0x1 << 8)
#define GPT_SPI_RD_MODE_QUAD_OT	(0x2 << 8)
#define GPT_SPI_RD_MODE_DUAL_IO	(0x3 << 8)
#define GPT_SPI_RD_MODE_QUAD_IO	(0x4 << 8)
/*spi wt mode select*/
#define GPT_SPI_WT_MODE_SIG	(0x0 << 12) 
#define GPT_SPI_WT_MODE_QUAD_IT	(0x2 << 12)
#define GPT_SPI_WT_MODE_QUAD_IO	(0x4 << 12)
/*spi rd delay*/
#define GPT_SPI_RD_TUNE_0	(0x0 << 16)
#define GPT_SPI_RD_TUNE_1	(0x1 << 16)
#define GPT_SPI_RD_TUNE_2	(0x2 << 16)
#define GPT_SPI_RD_TUNE_3	(0x3 << 16)

#define GPT_SPI_WP_VAL		(0x1 << 18)
#define GPT_SPI_HD_VAL		(0x1 << 19)
#define GPT_SPI_PREF_EN		(0x1 << 20)
#define GPT_SPI_WT_EXPR_MD	(0x1 << 21)
#define GPT_CS_DEASSERT		(GPT_CS_TIMING_VALUE << 24)

/*interrupt*/
#define GPT_SFO_INT_MASK	(0x1 << 16)
#define GPT_WR_PT_ADDR_MASK	(0x1 << 17)
#define GPT_PP_POL_DONE_MASK	(0x1 << 18)
#define GPT_SFO_INT_CLR		(0x1 << 24)
#define GPT_WR_PT_ADDR_CLR	(0x1 << 25)
#define GPT_PP_POL_DONE_CLR	(0x1 << 26)
#define GPT_SFO_INT_RAW		(0x1 << 8)
#define GPT_WR_PT_ADDR_RAW	(0x1 << 9)
#define GPT_PP_POL_DONE_RAW	(0x1 << 10)
#define GPT_SFO_INT_ST		(0x1 << 0)
#define GPT_WR_PT_ADDR_ST	(0x1 << 1)
#define GPT_PP_POL_ADDR_ST	(0x1 << 2)
/**/
#define GPT_SPI_WP_VAL		(0x1 << 18)
#define GPT_SPI_HD_VAL		(0x1 << 19)
#define GPT_SPI_PREF_EN		(0x1 << 20)
#define GPT_WRITE_EXPR_MODE	(0x1 << 21)

/*spi host reset*/
#define GPT_SPI_SW_RST		(0x1 << 0)

/*spi sfo cmd register*/
#define GPT_SFO_CMD_REQ_EN	(0x1 << 0)
#define GPT_SFO_CMD_TYPE_INST	(0x0 << 6)
#define GPT_SFO_CMD_TYPE_READ	(0x1 << 6)
#define GPT_SFO_CMD_TYPE_WRITE	(0x2 << 6)

#define GPT_SFO_POLLING_BSEL	(0x7 << 1)
#define GPT_SFO_POLLING_BPOL	(0x1 << 4)
#define GPT_SFO_POLLING_EN	(0x1 << 5)
#define GPT_SFO_ADDR_LEN_1	(0x1 << 16)
#define GPT_SFO_ADDR_LEN_2	(0x2 << 16)
#define GPT_SFO_ADDR_LEN_3	(0x3 << 16)
#define GPT_SFO_ADDR_LEN_4	(0x4 << 16)
#define GPT_SFO_DATA_LEN_1	(0x1 << 24)
#define GPT_SFO_DATA_LEN_2	(0x2 << 24)
#define GPT_SFO_DATA_LEN_3	(0x3 << 24)
#define GPT_SFO_DATA_LEN_4	(0x4 << 24)
#define GPT_SFO_DUMY_LEN_1	(0x1 << 20)
#define GPT_SFO_DUMY_LEN_2	(0x2 << 20)
#define GPT_SFO_DUMY_LEN_3	(0x3 << 20)
#define GPT_SFO_DUMY_LEN_4	(0x4 << 20)

/*s25fs command define*/
#define S25FS_COMMAND_WRSR		0x01
#define S25FS_COMMAND_PP		0x02
#define S25FS_COMMAND_READ		0x03
#define S25FS_COMMAND_WRDI		0x04
#define S25FS_COMMAND_RDSR1		0x05
#define S25FS_COMMAND_WREN		0x06
#define S25FS_COMMAND_RDSR2		0x07
#define S25FS_COMMAND_FST_RD		0x0B
#define S25FS_COMMAND_4FST_RD		0x0C
#define S25FS_COMMAND_4PP		0x12
#define S25FS_COMMAND_4READ		0x13
#define S25FS_COMMAND_P4E		0x20
#define S25FS_COMMAND_4P4E		0x21
#define S25FS_COMMAND_DUAL_OUT_RD	0x3B
#define S25FS_COMMAND_CHIPERASE		0x60
#define S25FS_COMMAND_RDAR		0x65
#define S25FS_COMMAND_RSTEN		0x66
#define S25FS_COMMAND_QUAD_OUT_RD	0x6B
#define S25FS_COMMAND_WRAR		0x71
#define S25FS_COMMAND_UNLOCK		0x98
#define S25FS_COMMAND_RST		0x99
#define S25FS_COMMAND_RDID		0x9F
#define S25FS_COMMAND_4BAM		0xB7
#define S25FS_COMMAND_DIOD		0xBB
#define S25FS_COMMAND_QIOD		0xEB
#define S25FS_COMMAND_SE		0xD8
#define S25FS_COMMAND_4SE		0xDC
/*w25q80 command define*/
#define W25Q80_COMMAND_RSR1		0x05
#define W25Q80_COMMAND_RSR2		0x35
#define W25Q80_COMMAND_RSR3		0x15
#define W25Q80_COMMAND_WSR		0x01
#define W25Q80_COMMAND_WSR1		0x31
#define W25Q80_COMMAND_WSR2		0x11
/*pn25f64 command define*/
#define PN25F64_COMMAND_EQPI		0x38

/*at25 command define*/
#define AT25SF_COMMAND_ST_WREN		0x50

/*mt25q command define*/
#define MT25Q_COMMAND_ENTER_4BYTE	0xB7
#define MT25Q_COMMAND_EXIT_4BYTE	0xE9
#define MT25Q_COMMAND_NONVOLATILE_CONF	0xB5
#define MT25Q_COMMAND_WR_EXTEND_ADDR	0xC5
#define MT25Q_COMMAND_RD_EXTEND_ADDR	0xC8

/*for options */
#define GPT_REGS(item)	(norflash_regs->item)

//#define GPT_NOR_DEBUG
#define GPT_CS_TIMING_VALUE 0x5e

#define GPT_WRITE_MEM_ADDR	(void *)0x200000000
#define GPT_READ_MEM_ADDR	(void *)0x200000000
struct gpt_norflash_reg {
	unsigned int sfc_ctl;
	unsigned int sw_rst;
	unsigned int sfc_int;
	unsigned int reserved1;
	unsigned int read_cmd;
	unsigned int reserved2;
	unsigned int reserved3;
	unsigned int reserved4;
	unsigned int pp_cmd;
	unsigned int pp_len;
	unsigned int pp_pol_init;
	unsigned int nxt_pol_time;
	unsigned int legal_wr_sp0_base;
	unsigned int legal_wr_sp0_end;
	unsigned int legal_wr_sp1_base;
	unsigned int legal_wr_sp1_end;
	unsigned int sfo_cmd;
	unsigned int sfo_addr;
	unsigned int sfo_rd;
	unsigned int sfo_wd;
	unsigned int sfo_polling_timing;
};

struct gpt_norflash_msg {
	unsigned char *buff;
	unsigned int len_buff;
	unsigned int addr;
	unsigned int opts;
	unsigned int dumy_len;
	unsigned int addr_len;
	unsigned int data_len;
	unsigned char  instr;
	unsigned char pollflag;
};

struct gpt_norflash_reg *norflash_regs;

void gpt_norflash_dumpregs(void);
int gpt_norflash_setconfig(unsigned int ctl,
		unsigned int intmask);
int gpt_norflash_init(void);
void gpt_norflash_addr_mode(unsigned int mode);
void gpt_norflash_reset(void);
int gpt_dev_probe(void);
int gpt_norflash_xfer(void);
int gpt_norflash_sfo_xfer(struct gpt_norflash_msg *msg);
void generate_random(unsigned char *buff, int length);
int gpt_norflash_checkdata(unsigned char *src, unsigned char *dst, int length);
int gpt_norflash_getid(void);


/*for device*/
#define GPT_AXI_TEST_LEN	0x1000
#define GPT_AXI_TEST_ADDR	0x1000
#define FLASH_PAGE_SIZE		0x1000
struct norflash_dev {
	int id;
	int (*enable_quad)(void);
	int (*disable_quad)(void);
	int (*dev_rst)(void);
	int (*dev_wren)(void);
	int (*protect_unlock)(void);
	int (*sec_erase)(unsigned int address);
	int (*sfo_read)(unsigned char *buff,
			unsigned int addr, unsigned int length);
	int (*sfo_write)(unsigned char *data,
			unsigned int addr, unsigned int length);
	int (*fast_read)(unsigned char *data, int from, int length);
	int (*fast_write)(unsigned char *data, int from, int length);
	int (*dual_read)(unsigned char *buff,
			unsigned int addr, unsigned int length);
	int (*dual_write)(unsigned char *buff,
			unsigned int addr, unsigned int length);
	int (*quad_read)(unsigned char *buff,
			unsigned int addr, unsigned int length);
	int (*quad_write)(unsigned char *buff,
			unsigned int addr, unsigned int length);
	int (*chip_erase)(void);
};

int s25fs_norflash_read(unsigned char *data, int from, int length);
int s25fs_norflash_fastread(void);
int s25fs_norflash_write(unsigned char *data, int from, int length);
int s25fs_norflash_pp(unsigned int address);
int s25fs_norflash_4p4e(void);
int s25fs_norflash_4se(void);
int s25fs_norflash_4ppbrd(void);
int s25fs_norflash_getid(void);
int s25fs_norflash_wren(void);

int s25fs_norflash_sfo_write(unsigned char *data,
		unsigned int addr, unsigned int length);
int s25fs_norflash_sfo_read(unsigned char *data,
		unsigned int addr, unsigned int length);

int s25fs_nor_flash_test(void);
int s25fs_norflash_getst(void);
int s25fs_norflash_sfo_write_4byte(unsigned char *data, unsigned int addr);
int s25fs_norflash_sfo_auto_se(unsigned int address);
int s25fs_norflash_sfo_auto_4se(unsigned int address);
int s25fs_norflash_sfo_rdar(int index);
int s25fs_norflash_axi_4byte_test(int flags);
int s25fs_norflash_4byte_en(void);
int s25fs_norflash_rst(void);
int s25fs_norflash_rst_en(void);
int s25fs_norflash_unlock(void);
int w25q80_norflash_quad_read(unsigned char *buff, unsigned int addr, unsigned int length);
int w25q80_norflash_quad_write(unsigned char *buff, unsigned int addr, unsigned int length);
int s25fs_norflash_dev_rst(void);
int w25q80_norflash_sfo_dual_read(unsigned char *buff, unsigned int addr, unsigned int length);
int w25q80_norflash_dual_write(unsigned char *buff, unsigned int addr, unsigned int length);
int sst26_quad_enable(void);
int s25fs_nor_flash_sfo_erase(void);
int mt25q_norflash_enter_4byte(void);
int mt25q_norflash_exit_4byte(void);
int mt25q_norflash_write_extend_addr(unsigned int addr);
int mt25q_norflash_read_extend_addr(void);
void gpt_norflash_dumpdata(unsigned char *data, int length);
int s25fs_norflash_sfo_auto_write(unsigned char *data, unsigned int addr, unsigned int length);
int read_buff(flash_info_t *info, uchar *dst, ulong addr, ulong cnt);
void gpt_test(void);

#endif
