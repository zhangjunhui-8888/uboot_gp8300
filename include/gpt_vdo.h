#ifndef __GPT_VIDEO_OUT_H__
#define __GPT_VIDEO_OUT_H__

#include <common.h>
#include <asm/io.h>

/*config extcr_lo_reg and extcr_hi_reg*/
#define GPT_APIU_ENABLE_MASK	0xffff0000
#define GPT_EXTCR_IO_REG			0xf0000100
#define GPT_EXTCR_HI_REG			0xf0000104
#define GPT_VOUT_ENABLE			(0x1 << 7)
#define GPT_VIDMEM_ENABLE		(0x1 << 8)
#define GPT_VID_ENABLE			(0x1 << 13)
#define GPT_VIDMEM_SLEEP			(0x1 << 8)
#define GPT_VIDMEM_SHUTDOWN	(0x1 << 9)

#define GPT_VDO_IRQNO		54
/*video dvo_enable register*/
#define GPT_DVO_ENABLE		(0x01 << 0)
#define GPT_DVO_FORMAT_8BIT	(0x00 << 4)
#define GPT_DVO_FORMAT_10BIT	(0x01 << 4)
#define GPT_DVO_FORMAT_16BIT	(0x02 << 4)
#define GPT_DVO_INTER_EN	(0x01 << 8)
#define GPT_DVO_PROGR_EN	(0x00 << 8)
#define GPT_DVO_EVEN		(0x01 << 9)
#define GPT_DVO_SYNC_EN		(0x01 << 10)
#define GPT_DVO_SYNC_DIS	(0x00 << 10)
#define GPT_DVO_DE_HI		(0x01 << 11)
#define GPT_DVO_HSYNC_HI	(0x01 << 12)
#define GPT_DVO_VSYNC_HI	(0x01 << 13)
#define GPT_VDO_CLK_DELAY(n)	(((n) & 0x1f) << 14)
#define GPT_VDO_CLK_DELAY_EN	(0x01 << 19)
#define GPT_VDO_CLK_GATE_EN	(0x01 << 20)
#define GPT_VDO_CLK_INV_EN	(0x01 << 21)
#define GPT_VDO_BW_OUTST_EN	(0x01 << 22)
#define GPT_VDO_BW_CMD_EN	(0x01 << 23)
#define GPT_VDO_AXI_CACHE	(((n) & 0xf) << 24)
#define GPT_VDO_AXI_PROT	(((n) & 0x7) << 28)

#define GPT_VDO_AXI_CACHE_B	(0x1 << 0)
#define GPT_VDO_AXI_CACHE_C	(0x1 << 1)
#define GPT_VDO_AXI_CACHE_R	(0x1 << 2)
#define GPT_VDO_AXI_CACHE_W	(0x1 << 3)
#define GPT_VDO_AXI_PROT_P	(0x1 << 0)
#define GPT_VDO_AXI_PROT_N	(0x1 << 1)
#define GPT_VDO_AXI_PROT_I	(0x1 << 2)

/*video dvo_int register*/
#define GPT_DVO_INT_DMA_CMP	(0x01 << 0)
#define GPT_DVO_INT_FIFO_UNDER	(0x01 << 1)
#define GPT_DVO_INT_SOF		(0x01 << 2)
#define GPT_DVO_INT_EOF		(0x01 << 3)
#define GPT_DVO_INT_LINE	(0x01 << 4)

#define GPT_DVO_RAW_DMA_CMP	(0x01 << 0)
#define GPT_DVO_RAW_FIFO_UNDER	(0x01 << 1)
#define GPT_DVO_RAW_SOF		(0x01 << 2)
#define GPT_DVO_RAW_EOF		(0x01 << 3)
#define GPT_DVO_RAW_LINE	(0x01 << 4)

#define GPT_DVO_MSK_DMA_CMP	(0x01 << 0)
#define GPT_DVO_MSK_FIFO_UNDER	(0x01 << 1)
#define GPT_DVO_MSK_SOF		(0x01 << 2)
#define GPT_DVO_MSK_EOF		(0x01 << 3)
#define GPT_DVO_MSK_LINE	(0x01 << 4)

#define GPT_DVO_CLR_DMA_CMP	(0x01 << 0)
#define GPT_DVO_CLR_FIFO_UNDER	(0x01 << 1)
#define GPT_DVO_CLR_SOF		(0x01 << 2)
#define GPT_DVO_CLR_EOF		(0x01 << 3)
#define GPT_DVO_CLR_LINE	(0x01 << 4)

#define GPT_VDO_IRQ_MASK	(GPT_DVO_CLR_DMA_CMP	\
				| GPT_DVO_CLR_SOF	\
				| GPT_DVO_CLR_EOF	\
				| GPT_DVO_CLR_LINE	\
				)
/*vedio dma_config register*/
#define GPT_DVO_DMA_SEMIP_EN	(0x01 << 9)
#define GPT_DVO_DMA_PACKE_EN	(0x00 << 9)

/* */
#define GPT_VDO_QOS(n)		(((n) & 0xf) << 8)
#define GPT_VDO_QOS_CL(n)	((n) & (~(0xf) << 8))

#define GPT_VDO_OUTST_NUM(n)	(((n) & 0x1f) << 0)
#define GPT_VDO_OUTST_CL(n)	((n) & (~(0x1f) << 0))

#define GPT_VDO_FIFO_DEP(n)	(((n) & 0x3ff) << 0)
#define GPT_VDO_FIFO_DEP_CL(n)	((n) & (~(0x3ff) << 0))

#define GPT_VDO_REGS(item)	(vdo_regs->item)

#define GPT_VDO_DEBUG    /*dump vdo regs*/

#ifdef CONFIG_TEST_ON_SOC
#if (defined(CONFIG_GPT_VIDMEM_TEST) || defined(CONFIG_GPT_VDO_VDMEM))
#define GPT_DATA_ADDR(n)	(0x20000000UL + n * 0x1000000) 	//mem addr
#define GPT_DATA_S_ADDR(n)	(0x20000000UL + n * 0x1000000)		//???
#define GPT_DATA_SIZE		0x01400000
#else
#define GPT_DATA_ADDR(n)	(0x220000000UL + n * 0x1000000) 	//mem addr
#define GPT_DATA_S_ADDR(n)	(0x230000000UL + n * 0x1000000)		//???
#define GPT_DATA_SIZE		0x01400000
#endif

#else
#define GPT_DATA_ADDR(n)	(0xa0000000UL + n * 0x1000000)
#define GPT_DATA_S_ADDR(n)	(0xb0000000UL + n * 0x1000000)
//#define GPT_DATA_ADDR_1		0xa1000000
//#define GPT_DATA_S_ADDR_1		0xb1000000
#define GPT_DATA_SIZE		0x01400000
#endif

#define GPT_PXI_BASE		0x0e0000000
#define GPT_ADDR_MASK		0xff000000
#define GPT_DATA_PXI_ADDR	0x20000000
#define GPT_DATA_S_PXI_ADDR	0x21000000
#define GPT_VDO_DMA_START	0x00010000


struct gpt_vdo_reg {
	unsigned int dvo_enable; //bit 1 read only.
	unsigned int dvo_dma_position;
	unsigned int dvo_background;
	unsigned int dvo_hactive;
	unsigned int dvo_hdisplay;
	unsigned int dvo_hsync;
	unsigned int dvo_vactive_odd;
	unsigned int dvo_bottom_blank_odd;
	unsigned int dvo_vdisplay_odd;
	unsigned int dvo_vsync_odd;
	unsigned int dvo_vsync_hnum_odd;
	unsigned int dvo_vactive_even;
	unsigned int dvo_bottom_blank_even;
	unsigned int dvo_vdisplay_even;
	unsigned int dvo_vsync_even;
	unsigned int dvo_vsync_hnum_even;
	unsigned int dvo_int; //read only
	unsigned int dvo_raw; //read only
	unsigned int dvo_int_mask;
	unsigned int dvo_int_clear; //write 1 clear.
	unsigned int dvo_int_line_num;

	unsigned int dvo_dma_base_0;
	unsigned int dvo_dma_base_1;
	unsigned int dvo_dma_hdat;
	unsigned int dvo_dma_high;
	unsigned int dvo_dma_stride;
	unsigned int dvo_dma_config;
	unsigned int dvo_dma_base_0_h;
	unsigned int dvo_dma_base_1_h;
	unsigned int dvo_dma_outstd_num;
	unsigned int dvo_dma_fifo_dep;
};

struct gpt_vdo_param_2 {
	unsigned int vdo_mode;
	unsigned int data_mode;
	unsigned int sync_mode;
	unsigned int disp_mode;
	unsigned int vdo_polarity;

	/*for horizontal sync*/
	unsigned int dvo_ha;
	unsigned int dvo_hd;
	unsigned int dvo_hb;
	unsigned int dvo_hc;
	unsigned int dvo_he;
	unsigned int dvo_hf;
	/*for odd field*/
	unsigned int hnum_odd;
	unsigned int dvo_va_o;
	unsigned int dvo_vd_o;
	unsigned int dvo_vb_o;
	unsigned int dvo_vc_o;
	unsigned int dvo_ve_o;
	unsigned int dvo_vf_o;
	unsigned int dvo_vg_o;
	/*for even field*/
	unsigned int hnum_even;
	unsigned int dvo_va_e;
	unsigned int dvo_vd_e;
	unsigned int dvo_vb_e;
	unsigned int dvo_vc_e;
	unsigned int dvo_ve_e;
	unsigned int dvo_vf_e;
	unsigned int dvo_vg_e;
};

struct gpt_vdo_param {
	unsigned int vdo_mode;
	unsigned int data_mode;
	unsigned int sync_mode;
	unsigned int disp_mode;
	unsigned int vdo_polarity;

	/*for horizontal sync*/
	unsigned int dvo_ha;
	unsigned int dvo_hd;
	unsigned int dvo_hb;
	unsigned int dvo_hc;
	unsigned int dvo_he;
	unsigned int dvo_hf;
	/*for odd field*/
	unsigned int dvo_va_o;
	unsigned int dvo_vd_o;
	unsigned int dvo_vb_o;
	unsigned int dvo_vc_o;
	unsigned int dvo_ve_o;
	unsigned int dvo_vf_o;
	unsigned int dvo_vg_o;
	/*for even field*/
	unsigned int dvo_va_e;
	unsigned int dvo_vd_e;
	unsigned int dvo_vb_e;
	unsigned int dvo_vc_e;
	unsigned int dvo_ve_e;
	unsigned int dvo_vf_e;
	unsigned int dvo_vg_e;
	unsigned int hnum_even;
	unsigned int hnum_odd;
};

int writel_debug(u32 val, unsigned int *addr);
u32 readl_debug(unsigned int *addr);
int gpt_vdo_init(void);
int gpt_vdo_run(int count);
int gpt_vdo_background(int y, int cb, int cr);
int gpt_vdo_set_resolution(struct gpt_vdo_param *param);
int gpt_vdo_set_configs(int dma_h, int dma_v, int dma_burst_len, int dma_simip_en);
int gpt_vdo_data_generate(int type, int mode);
void gpt_vdo_dumpregs(void);
void gpt_vdo_fill_regs(void);
void gpt_vdo_compare_regs(void);
int gpt_vdo_reg_read(int offset);
void gpt_vdo_reg_write(int offset, int data);
int gpt_vdo_test_480p(int type);
int gpt_vdo_mode(int mode);
void gpt_vdo_dma_init(unsigned int start, ulong dma_addr0, ulong dma_addr1, ulong dma_burst);
int gpt_vdo_yuvdata(int y, int cb, int cr);
void gpt_vdo_enable_clock_delay(int clk_delay);
void gpt_vdo_disable(void);
void gpt_vdo_x_int_show(void);
int gpt_vdo_color_change_loop(int mode);

/*for adv7511 device*/
//#define ADV7511_DEBUG 1
#ifdef DEBUG /*ADV7511_DEBUG*/
	#define ADV7511_PDEBUG(fmt, args...)			\
		do {						\
			printf("[%s:%d] " fmt, 			\
			__func__, __LINE__ , ## args);		\
		} while (0)

#else
	#define ADV7511_PDEBUG(level, fmt, args...)		\
		do {						\
		} while (0)
#endif

struct embedded_sync_param {
	int reg_17;
	int reg_30;
	int reg_31;
	int reg_32;
	int reg_33;
	int reg_34;

	int reg_35;
	int reg_36;
	int reg_37;
	int reg_38;
	int reg_39;
	int reg_3a;

	int reg_d7;
	int reg_d8;
	int reg_d9;
	int reg_da;
	int reg_db;
	int reg_dc;
};

int adv7511_init(void);
int adv7511_init1(void);
void adv7511_write(unsigned int offset, unsigned int data);
int adv7511_read(unsigned int offset);
void adv7511_show_positon(void);
int adv7511_set_resolution(int bits, int sync, int resl);

struct gpt_vdo_config {
	int bittype;
	int syncmode;
	int hostid;
	int resl;
};

enum {
	EM_SYNC_720P60,
	EM_SYNC_480P,
	EM_SYNC_480I,
	EM_SYNC_576P,
	EM_SYNC_576I,
	EM_SYNC_1080P50,
	EM_SYNC_1080P60,
	EM_SYNC_1080I30,
	EM_SYNC_1080P24,
	EM_SYNC_1080P30,  //add by lyy 2019-06-11
	EM_SYNC_480P10B,
	EM_SYNC_RESERVE,
};

void set_video_out_clk(uint8_t vcapn);
extern void open_vidmem(void);
extern void pxi_ctl(void);

#endif
