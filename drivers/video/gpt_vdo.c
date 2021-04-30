#include <gpt_vdo.h>
#include <gpt-mpint.h>
#include <asm-generic/errno.h>

unsigned int vdo_reg_ro[] = 
{
    /*unsigned int dvo_enable*/             0x0, /*bit 1 read only.*/
	/*unsigned int dvo_dma_position*/       0x0,
	/*unsigned int dvo_background*/         0x0,
	/*unsigned int dvo_hactive*/            0x0,
	/*unsigned int dvo_hdisplay*/           0x0,
	/*unsigned int dvo_hsync*/              0x0,
	/*unsigned int dvo_vactive_odd*/        0x0,
	/*unsigned int dvo_bottom_blank_odd*/   0x0,
	/*unsigned int dvo_vdisplay_odd*/       0x0,
	/*unsigned int dvo_vsync_odd*/          0x0,
	/*unsigned int dvo_vsync_hnum_odd*/     0x0,
	/*unsigned int dvo_vactive_even*/       0x0,
	/*unsigned int dvo_bottom_blank_even*/  0x0,
	/*unsigned int dvo_vdisplay_even*/      0x0,
	/*unsigned int dvo_vsync_even*/         0x0,
	/*unsigned int dvo_vsync_hnum_even*/    0x0,
	/*unsigned const int dvo_int*/          0x0, /*read only*/
	/*unsigned const int dvo_raw*/          0x0, /*read only*/
	/*unsigned int dvo_int_mask*/           0x0,
	/*unsigned int dvo_int_clear*/          0x0, /*write 1 clear.*/
	/*unsigned int dvo_int_line_num*/       0x0,

	/*unsigned int dvo_dma_base_0*/         0x0,
	/*unsigned int dvo_dma_base_1*/         0x0,
	/*unsigned int dvo_dma_hdat*/           0x0,
	/*unsigned int dvo_dma_high*/           0x0,
	/*unsigned int dvo_dma_stride*/         0x0,
	/*unsigned int dvo_dma_config*/         0x10,
	/*unsigned int dvo_dma_base_0_h*/       0x0,
	/*unsigned int dvo_dma_base_1_h*/       0x0,
	/*unsigned int dvo_dma_outstd_num*/     0x72004,
	/*unsigned int dvo_dma_fifo_dep*/       0x200
};

#if 0
#define 1080P_odd    {0, 40, 1120, 40, 1120, 0, 5, 1124}
#define 1080P_even 1080P_odd
#define 1080P_dual_148 {280-1, 2199, 280-1, 2199, 87, 131}
#define 1080P_single_148 {559, 4399, 559, 4399, 175, 263}
#define 1080P25_dual {719, 2639, 719, }

#define 1080I_odd	{0, 19, 559, 19, 559, 0, 5, 561}

#define d_1080I_even_120_60 {1187, 20, 560, 20, 560, 0, 5, 562}
#define s_1080I_even_60[8]    = {2375, 20, 560, 20, 560, 0, 5, 562}
#define s_1080I_even_50[8]    = {3695, 20, 560, 20, 560, 0, 5, 562}
#define d_1080I_even_100_50[8]    = {1847, 20, 560, 20, 560, 0, 5, 562}
#define 72M_1080I_odd_50[8]     ={0, 61, 601, 61, 601, 0, 5, 623}
#define 72M_1080I_even_50[8]    ={1183, 62, 602, 62, 602, 0, 5, 625}

#define 720P_odd[8]     ={0, 24, 744, 24, 744, 0, 5, 749}
#define 720P_even[8]     ={0, 24, 744, 24, 744, 0, 5, 749}

#define 576P_odd[8]     ={0, 43, 619, 43, 619, 0, 5, 624}
#define 576P_even[8]    ={0, 43, 619, 43, 619, 0, 5, 624}

#define 576I_odd[8]     ={0, 21, 309, 21, 309, 0, 3, 311}
#define d_576I_even_200_100[8] ={443, 22, 310, 22, 310, 0, 3, 312}
#define s_576I_even_100[8] ={887, 22, 310, 22, 310, 0, 3, 312}

#define 480P_odd[8]     ={0, 41, 521, 41, 521, 6, 12, 524}
#define 480P_even[8]    ={0, 41, 521, 41, 521, 6, 12, 524}

#define 480I_odd[8]     ={0, 20, 260, 20, 260, 3, 6, 261}
#define s_480I_even_60_120_240[8] ={895, 21, 261, 21, 261, 3, 6, 262}
#define d_480I_even_120_240[8]  ={447, 21, 261, 21, 261, 3, 6, 262}

#define 640x480_odd = {0, 34, 514, 0, 2, 524}
#define 640x480_even = {0, 34, 514, 0, 2, 524}

#define 148_d_1080P60 {{280-1, 2199, 280-1, 2199, 87, 131},{0, 40, 1120, 40, 1120, 0, 5, 1124},{0, 40, 1120, 40, 1120, 0, 5, 1124}}
#define 148_s_1080P30[] {{559, 4399, 559, 4399, 175, 263},{0, 40, 1120, 40, 1120, 0, 5, 1124},{0, 40, 1120, 40, 1120, 0, 5, 1124}}
#define 74_d_1080P30[] {{280-1, 2199, 280-1, 2199, 87, 131},{0, 40, 1120, 40, 1120, 0, 5, 1124},{0, 40, 1120, 40, 1120, 0, 5, 1124}}

struct gpt_vdo_param_2 resl[] = {
/*index0, 16bit 1080p@30 74.25M, sync packet prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//p30
/*1080p sync plannar prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//p30
/*1080p embeded packet prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 830, 2750, 830, 2750, 638, 638 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p embeded plannar prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 830, 2750, 830, 2750, 638, 638 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index4, 16bit 720p@60, 74.25M, sync mode, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 370, 1650, 370, 1650, 110, 110 + 40, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, sync mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 370, 1650, 370, 1650, 110, 110 + 40, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, embeded mode, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 370, 1650, 370, 1650, 110, 110 + 40, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, embeded mode, packet, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 370, 1650, 370, 1650, 110, 110 + 40, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index8, 16bit 640x480p, 25.2M, sync mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, sync mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index12, 16bit 480p@60, 27M/25.2M, sync mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p, sync mode, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index16, 8bit 480i@60, 27M, sync, packet, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 27M, sync, semip, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 27M, embeded, packet, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) - 1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 27M, embeded, semip, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) - 1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},

/*index20,8bit, 576i@60, 27M, sync, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*8bit, 576i@60, 27M, sync, semi-plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*8bit, 576i@60, 27M, embeded, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*8bit, 576i@60, 27M, embeded, semi-plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},

/*index24, 8bit 720p@60, 148.5M, sync mode, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 8bit, 148.5M, sync mode, semi planar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 8bit, 148.5M, embeded mode, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 8bit, 148.5M, embeded mode, packet, semi plannar*/
                                                                                           /* 2*(hp+hd),2*(hp+hd+w),2*(hp+hd),2*(hp+hd+W),2*hp-1,2*(hp+hdu)-1,      vd,H+vd,vd,H+vd,of,vdu,H+vd+vdu */
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index 28, 16bit 1080p@30, 74.25M sync mode*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//										            /* 2*(hp+hd),2*(hp+hd+w),2*(hp+hd),   2*(hp+hd+W),   2*hp-1,2*(hp+hdu)-1,vd,H+vd,  vd,H+vd,of,vdu,H+vd+vdu */	
//1min{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88, 88+44, 41, 1080+41, 41, 1080+41, 4, 9, 1126, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88-1, 88+44-1, 41-1, 1080+41-1, 41-1, 1080+41-1, 2, 7, 1123, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88-1, 88+44-1, 41-1, 1080+41-1, 41-1, 1080+41-1, 3, 8, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88-1, 88+44-1, 41-1, 1080+41-1, 41-1, 1080+41-1, 1, 6, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2199, 280-1, 2199, 87, 87 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*16bit 1080p@60, 148.5M sync mode*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 4, 8, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},		//p30
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //p50
/*16bit 1080p@60, halt display sync mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*31 reserve*/
/*16bit 1080p@60, 148.5M sync mode*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 4, 8, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},		//p30
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //p50

/*index32, 1080i 74.25M, sync, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},
/*1080i 74.25M, sync,  semi-plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},
/*1080i 74.25M, embeded, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},
/*1080i 74.25M, embeded semi-plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},

/*index36, 480p@60, 8bit, 54M, sync, packet */
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p@60, 8bit, 54M, sync, semi-plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p@60p, 8bit, 54M, embeded , packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p@60, 8bit, 54M, sync mode*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index40, 640x480p, 8bit, 50.4M, sync mode*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, sync mode, semi planar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode, semi plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index44, 480p@60, 10bit, 54M, sync, packet */
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},
/*480p@60, 10bit, 54M, sync, semi-plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},
/*480p@60p, 10bit, 54M, embeded , packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},
/*480p@60, 10bit, 54M, sync mode*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},

/*index48, 1080p@24, 8bit, 148.5M, sync packet prog*/
//{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@24, 8bit, 148.5M, sync plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@24, 8bit, 148.5M, embeded packet prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@24, 8bit, 148.5M, embeded plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index52, 640x480p, 10bit, 50.4M, sync mode*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, 50.4M, sync mode, semi planar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, 50.4M, embeded mode*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 - 1, 2 * 800 - 1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 - 1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, 50.4M, embeded, semi plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index56, 720p@60, 10bit, 148.5M, sync mode, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 10bit, 148.5M, sync mode, semi planar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 10bit, 148.5M, embeded mode, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 10bit, 148.5M, embeded mode, packet, semi plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index60, 1080p@30, 10bit, 148.5M, sync packet prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 10bit, 148.5M, sync plannar prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 10bit, 148.5M, embeded packet prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 10bit, 148.5M, embeded plannar prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index64, 480i@60, 10bit, 27M, sync, packet, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 10bit, 27M, sync, semip, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 10bit, 27M, embeded, packet, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1 - 4, 1716 - 1 - 4, 276 - 1 - 4, 1716 - 1 - 4, 38 - 1 - 4, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 10bit, 27M, embeded, semip, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1 - 4, 1716 - 1 - 4, 276 - 1 - 4, 1716 - 1 - 4, 38 - 1, (38 + 124) - 1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},

/*index68, 10bit, 576i@60, 27M, sync, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*10bit, 576i@60, 27M, sync, semi-plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*10bit, 576i@60, 27M, embeded, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*10bit, 576i@60, 27M, embeded, semi-plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},

/*index72, 1080p@30, 8bit, 148.5M, sync packet prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 8bit, 148.5M, sync plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 8bit, 148.5M, embeded packet prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 8bit, 148.5M, embeded plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index76, 480p, 27M, sync mode halt windows*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a + 0x80, 0x35a - 0x80, 16, (16 + 62), 0x2a, 0x20a, 0x2a + 0x80, 0x20a - 0x80, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p, sync mode, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a - 0x26c, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a - 0x17c, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a + 0x80, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a + 0x80, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a + 0x80, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a + 0x80, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

#else
struct gpt_vdo_param resl[] = {
/*index0, 16bit 1080p@30 74.25M, sync packet prog*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 830, 2750, 830, 2750, 638, 638 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2199, 280-1, 2199, 87, 87 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0},	//p30
/*1080p sync plannar prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2199, 280-1, 2199, 87, 87 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0},	//p30
/*1080p embeded packet prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2199, 280-1, 2199, 87, 87 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0},
/*1080p embeded plannar prog*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2199, 280-1, 2199, 87, 87 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0},

/*index4, 16bit 720p@60, 74.25M, sync mode, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 369, 1649, 369, 1649, 109, 109 + 40, 24, 744, 24, 744, 0, 5, 750-1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0},
/*720p, sync mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 369, 1649, 369, 1649, 109, 109 + 40, 24, 744, 24, 744, 0, 5, 750-1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0},
/*720p, embeded mode, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 369, 1649, 369, 1649, 109, 109 + 40, 24, 744, 24, 744, 0, 5, 750-1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0},
/*720p, embeded mode, packet, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 369, 1649, 369, 1649, 109, 109 + 40, 24, 744, 24, 744, 0, 5, 750-1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0},

/*index8, 16bit 640x480p, 25.2M, sync mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, sync mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 160, 800, 160, 800, 16, (16 + 96), 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index12, 16bit 480p@60, 27M/25.2M, sync mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p, sync mode, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index16, 8bit 480i@60, 27M, sync, packet, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 27M, sync, semip, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 27M, embeded, packet, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) - 1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 27M, embeded, semip, interlace*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) - 1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},

/*index20,8bit, 576i@60, 27M, sync, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*8bit, 576i@60, 27M, sync, semi-plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*8bit, 576i@60, 27M, embeded, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*8bit, 576i@60, 27M, embeded, semi-plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},

/*index24, 8bit 720p@60, 148.5M, sync mode, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 8bit, 148.5M, sync mode, semi planar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 8bit, 148.5M, embeded mode, packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 8bit, 148.5M, embeded mode, packet, semi plannar*/
                                                                                           /* 2*(hp+hd),2*(hp+hd+w),2*(hp+hd),2*(hp+hd+W),2*hp-1,2*(hp+hdu)-1,      vd,H+vd,vd,H+vd,of,vdu,H+vd+vdu */
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index 28, 16bit 1080p@30, 74.25M sync mode*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//										            /* 2*(hp+hd),2*(hp+hd+w),2*(hp+hd),   2*(hp+hd+W),   2*hp-1,2*(hp+hdu)-1,vd,H+vd,  vd,H+vd,of,vdu,H+vd+vdu */	
//1min{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88, 88+44, 41, 1080+41, 41, 1080+41, 4, 9, 1126, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88-1, 88+44-1, 41-1, 1080+41-1, 41-1, 1080+41-1, 2, 7, 1123, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88-1, 88+44-1, 41-1, 1080+41-1, 41-1, 1080+41-1, 3, 8, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 88+191, 88+191+1920, 88+191, 1*(88+191+1920), 1*88-1, 88+44-1, 41-1, 1080+41-1, 41-1, 1080+41-1, 1, 6, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2199, 280-1, 2199, 87, 87 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*16bit 1080p@60, 148.5M sync mode*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 4, 8, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},		//p30
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //p50
/*16bit 1080p@60, halt display sync mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*31 reserve*/
/*16bit 1080p@60, 148.5M sync mode*/
//{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 40, 1120, 40, 1120, 4, 8, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},		//p30
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 720, 2640, 720, 2640, 528, 528 + 44, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //p50

/*index32, 1080i 74.25M, sync, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},
/*1080i 74.25M, sync,  semi-plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},
/*1080i 74.25M, embeded, packet*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},
/*1080i 74.25M, embeded semi-plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_INTER_EN, GPT_DVO_DE_HI, 280-1, 2200, 280-1, 2200, 88, 88 + 44, 21, 561, 21, 561, 2, 7, 561, 22, 562, 22, 562, 2, 7, 562, 1100, 0},

/*index36, 480p@60, 8bit, 54M, sync, packet */
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p@60, 8bit, 54M, sync, semi-plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p@60p, 8bit, 54M, embeded , packet*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p@60, 8bit, 54M, sync mode*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 522, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index40, 640x480p, 8bit, 50.4M, sync mode*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, sync mode, semi planar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, embeded mode, semi plannar*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index44, 480p@60, 10bit, 54M, sync, packet */
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},
/*480p@60, 10bit, 54M, sync, semi-plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},
/*480p@60p, 10bit, 54M, embeded , packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},
/*480p@60, 10bit, 54M, sync mode*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 138 - 1, 2 * 858 - 1, 2 * 138 - 1, 2 * 858 - 1, 2 * 16 - 1, 2 * (16 + 62) - 1, 42, 521, 42, 521, 6, 12, 524, 0, 0, 0, 0, 0, 0, 0, 0, 31},

/*index48, 1080p@24, 8bit, 148.5M, sync packet prog*/
//{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@24, 8bit, 148.5M, sync plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@24, 8bit, 148.5M, embeded packet prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@24, 8bit, 148.5M, embeded plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index52, 640x480p, 10bit, 50.4M, sync mode*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, 50.4M, sync mode, semi planar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, 50.4M, embeded mode*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 - 1, 2 * 800 - 1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 - 1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*640x480p, 50.4M, embeded, semi plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 2 * 160 -1, 2 * 800 -1, 2 * 160 - 1, 2 * 800 - 1, 2 * 16 -1, 2 * (16 + 96) - 1, 35, 515, 35, 515, 0, 1, 525, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index56, 720p@60, 10bit, 148.5M, sync mode, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 10bit, 148.5M, sync mode, semi planar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 10bit, 148.5M, embeded mode, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*720p, 10bit, 148.5M, embeded mode, packet, semi plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2 * 370 - 1, 2*1650-1, 2*370-1, 2*1650-1, 2*110-1, 2*(110 + 40) - 1, 24, 744, 24, 744, 0, 5, 750-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index60, 1080p@30, 10bit, 148.5M, sync packet prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 10bit, 148.5M, sync plannar prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 10bit, 148.5M, embeded packet prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 10bit, 148.5M, embeded plannar prog*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*830-1, 2*2750-1, 2*830-1, 2*2750-1, 2*638-1, 2*(638 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index64, 480i@60, 10bit, 27M, sync, packet, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 10bit, 27M, sync, semip, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1, 1716 - 1, 276 - 1, 1716 - 1, 38 - 1, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 10bit, 27M, embeded, packet, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1 - 4, 1716 - 1 - 4, 276 - 1 - 4, 1716 - 1 - 4, 38 - 1 - 4, (38 + 124) -  1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},
/*480i@60, 10bit, 27M, embeded, semip, interlace*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 276 - 1 - 4, 1716 - 1 - 4, 276 - 1 - 4, 1716 - 1 - 4, 38 - 1, (38 + 124) - 1, 21, 261, 21, 261, 4, 7, 261, 22, 262, 22, 262, 4, 7, 262, 897 - 1, 39 - 1},

/*index68, 10bit, 576i@60, 27M, sync, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*10bit, 576i@60, 27M, sync, semi-plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*10bit, 576i@60, 27M, embeded, packet*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},
/*10bit, 576i@60, 27M, embeded, semi-plannar*/
{GPT_DVO_FORMAT_10BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_INTER_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 288 - 1, 1728 - 1, 288 - 1, 1728 - 1, 24 - 1, (24 + 126) - 1, 23, 311, 23, 311, 2, 5, 311, 24, 312, 24, 312, 2, 5, 312, 864 + 23, 23},

/*index72, 1080p@30, 8bit, 148.5M, sync packet prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 8bit, 148.5M, sync plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 8bit, 148.5M, embeded packet prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*1080p@30, 8bit, 148.5M, embeded plannar prog*/
{GPT_DVO_FORMAT_8BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_DE_HI, 2*280-1, 2*2200-1, 2*280-1, 2*2200-1, 2*88-1, 2*(88 + 44) -1, 40, 1120, 40, 1120, 0, 5, 1125, 0, 0, 0, 0, 0, 0, 0, 0, 0},

/*index76, 480p, 27M, sync mode halt windows*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a + 0x80, 0x35a - 0x80, 16, (16 + 62), 0x2a, 0x20a, 0x2a + 0x80, 0x20a - 0x80, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p, sync mode, semi plannar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_EN, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a, 0x35a - 0x26c, 16, (16 + 62), 0x2a, 0x20a, 0x2a, 0x20a - 0x17c, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_PACKE_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a + 0x80, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a + 0x80, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*480p embeded mode, semi planar*/
{GPT_DVO_FORMAT_16BIT, GPT_DVO_DMA_SEMIP_EN, GPT_DVO_SYNC_DIS, GPT_DVO_PROGR_EN, GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI | GPT_DVO_DE_HI, 0x8a, 0x35a, 0x8a + 0x80, 0x35a, 16, (16 + 62), 0x2a, 0x20a, 0x2a + 0x80, 0x20a, 6, 12, 0x20d, 0, 0, 0, 0, 0, 0, 0, 0, 0},

};
#endif

int gpt_vdo_interrupt(void *data);
int gpt_cdma_mem_real_copy(uint8_t *dst, uint8_t *src, int length);

static struct gpt_vdo_reg *vdo_regs = (struct gpt_vdo_reg*)GPT_VDO_APB_BASE;
static ulong dma_addr0_of;
static ulong dma_addr1_of;

int writel_debug(u32 val, unsigned int *addr)
{
	debug("VDO write 0x%8p => 0x%x\n", (void *)addr, val);//add by lyy
	return writel(val, (void*)addr);
}

u32 readl_debug(unsigned int *addr)
{
	u32 val = readl((void*)addr);
	debug("VDO read  0x%8p => 0x%x\n", (void *)addr, val);//add by lyy
	return val;
}

#ifdef GPT_VDO_DEBUG
void gpt_vdo_dumpregs(void)
{
	int i;
	//vdo_regs = (struct gpt_vdo_reg*)GPT_VDO_APB_BASE;
	unsigned int *reg_t = (unsigned int *)vdo_regs;

	if (reg_t == NULL) {
		debug("please init vdo host first\n");
		return;
	}

	debug("vido out regs: \n");
	for (i = 0; i <= (0x78 / 4); i++) {
		if (i % 4 == 0){
			debug("%.8x: ", i* 4);
        }
		debug("%.8x%c", reg_t[i], i % 4 == 3?'\n':' ');
	}
	debug("\n");
}

void gpt_vdo_compare_regs(void)
{
    int i = 0;
    unsigned int *reg_t = (unsigned int *)vdo_regs;
    unsigned int *reg_ro = vdo_reg_ro;

    if(reg_t == NULL){
        debug("please init vdo host first.\n");
        return;
    }
    
    for(i = 0 ; i <= (0x78 / 4); i++){
        if(reg_ro[i] != reg_t[i]){
            debug("reg compare: 0x%x %.8x != %.8x\n", i * 4, reg_ro[i], reg_t[i]);
        }
    }
}

void gpt_vdo_fill_regs(void)
{
    int i = 0;
    unsigned int *reg_t = (unsigned int *)vdo_regs;

    if(reg_t == NULL){
        debug("please init vdo host first.\n");
        return;
    }
    
    for(i = 0 ; i <= (0x78 / 4); i++){
        reg_t[i] = 0xffffffff;
    }
}


void gpt_vdo_dumpdata(unsigned char *data, int length)
{
	int i;
#ifdef GPT_VDO_DEBUG
	debug("dump data  length %.8x\n", length);
#endif
	for (i = 0; i < length; i++) {
		if (i % 16 == 0)
			debug("%.8x: ", i);
		debug("%.2x%c", data[i], i % 16 == 15? '\n': ' ');
	}
	debug("\n");
}
#else
void gpt_vdo_dumpregs(void)
{

    debug("%s:%d\n", __func__, __LINE__);
}
void gpt_vdo_dumpdata(unsigned char *data, int length)
{

    debug("%s:%d\n", __func__, __LINE__);
}
#endif


int gpt_vdo_ycbcr_exchange_rgb(int mode, int *r, int *g,
				int *b, int *y, int *cb, int *cr)
{
	float yy, ccr, ccb;
	switch (mode) {
	case 2: //709
		yy  =  0.2126 * (*r) + 0.7154 * (*g) + 0.072 * (*b);
		ccb = -(0.1145 * (*r)) - 0.3855 * (*g) + 0.5 * (*b) + 128;
		ccr =  0.5 * (*r) - 0.4543 * (*g) - 0.0457 * (*b) + 128;
		if (yy > 0xff)
			yy = 0xff;
		if (yy < 0)
			yy = 0;

		if (ccb > 0xff)
			ccb = 0xff;
		if (ccb < 0)
			ccb = 0;

		if (ccr > 0xff)
			ccr = 0xff;
		if (ccr < 0)
			ccr = 0;

		*y = (uint32_t)yy;
		*cb = (uint32_t)ccb;
		*cr = (uint32_t)ccr;
		break;

	case 1: //601    //??
		yy  =  0.257 * (*r) + 0.564 * (*g) + 0.098 * (*b) + 16;
		ccb = -(0.148 * (*r)) - 0.291 * (*g) + 0.439 * (*b) + 128;
		ccr =  0.439 * (*r) - 0.368 * (*g) - 0.071 * (*b) + 128;
		if (yy > 0xff)
			yy = 0xff;
		if (yy < 0)
			yy = 0;

		if (ccb > 0xff)
			ccb = 0xff;
		if (ccb < 0)
			ccb = 0;

		if (ccr > 0xff)
			ccr = 0xff;
		if (ccr < 0)
			ccr = 0;

		*y = (uint32_t)yy;
		*cb = (uint32_t)ccb;
		*cr = (uint32_t)ccr;
		break;
	case 0: //601
		*r = 1.164 * (*y - 16) + 1.596 * (*cr -128);
		*g = 1.164 * (*y - 16) - 0.392 * (*cb -128) - 0.813 * (*cr -128);
		*b = 1.164 * (*y - 16) + 2.017 * (*cb -128);
		break;
	}

	return 0;
}

int gpt_vdo_data_prepare_10bit(unsigned short *buff, int y, int cb,
			int cr, int line, int colume, int colordep)
{
	int i,j;
#ifdef GPT_VDO_DEBUG
	debug("y:%x, cb:%x, cr:%x \n", y, cb, cr);
#endif
	for (i = 0; i < line; i++) {
		for (j = 0; j < colume * 2; j += 4) {

			buff[i * colume *2 + j + 0] = cb;
			buff[i * colume *2 + j + 1] = y;
			buff[i * colume *2 + j + 2] = cr;
			buff[i * colume *2 + j + 3] = y;

		}
	}

	return 0;
}

int gpt_vdo_data_prepare(unsigned char *buff, int y, int cb,
			int cr, int line, int colume, int colordep)
{
	int i,j;
#ifdef GPT_VDO_DEBUG
	debug("y:%x, cb:%x, cr:%x \n", y, cb, cr);
#endif
	for (i = 0; i < line; i++) {
		for (j = 0; j < colume * 2; j += 4) {

			buff[i * colume *2 + j + 0] = cb;	//why?
			buff[i * colume *2 + j + 1] = y;
			buff[i * colume *2 + j + 2] = cr;
			buff[i * colume *2 + j + 3] = y;

		}
	}

	return 0;
}

int gpt_vdo_interlace_data(unsigned char *buff, int line, int column)
{
	int i,j;

	for (i = 0; i < line; i++) {
		for (j = 0; j < column * 2; j += 4) {
			if (i % 2 == 0) {
				buff[i * column *2 + j + 0] = 0xa0;
				buff[i * column *2 + j + 2] = 0xa0;
			} else {
				buff[i * column *2 + j + 0] = 0x5a;
				buff[i * column *2 + j + 2] = 0x5a;
			}
			buff[i * column *2 + j + 1] = 0x90;
			buff[i * column *2 + j + 3] = 0x90;
		}
	}

	return 0;
}

int gpt_vdo_colorbar(unsigned char *buff, int line, int column)
{
	int i, j, width = 80;
	memset(buff, 128, line * column * 2);
	for (i = 0; i < line; i++) {
		for (j = 0; j < column * 2; j += 4) {
			buff[i * column *2 + j + 1] = j / width * 30;
			buff[i * column *2 + j + 3] = j / width * 30;
		}
	}
	return 0;
}

int gpt_vdo_rgb_colorbar(unsigned char *buff, int line, int column)
{
	int i, j;
	int index = 0;
	uint32_t *data = (uint32_t *)buff;
	debug("line: %d, column: %d\n", line, column);

	for (i = 0; i < line; i++) {
		for (j = 0; j < column; j++, index++) {

			if ((index % 720) < (160 * 2))
				data[index] = 0xffffff;
			else if ((index % 720) < 720)
				data[index] = 0x00ff;
			else
				debug("error\n");
		}
	}

	return 0;
}

int gpt_vdo_range_data(unsigned char *buff, int line, int colume)
{
	int i;
	unsigned int count = get_timer(0);
#ifdef GPT_VDO_DEBUG
	debug("random data is %x\n", count);
#endif
	for (i = 0; i < line * colume * 2; i++) {
		buff[i] = (count++) & 0xff;
	}
	return 0;
}

static int gpt_extcr_cfg(void)
{
	/*vout  pclk vidmem enable */
	writel((readl(GPT_EXTCR_IO_REG) | GPT_VOUT_ENABLE | GPT_VID_ENABLE)  | 
			GPT_VIDMEM_ENABLE | GPT_APIU_ENABLE_MASK, GPT_EXTCR_IO_REG); 
	
	/*write 0 set vidmem don't sleep and shutdown.*/
	writel((readl(GPT_EXTCR_HI_REG) | ((GPT_VIDMEM_SLEEP | GPT_VIDMEM_SHUTDOWN)  << 16)) & 
			~(GPT_VIDMEM_SLEEP | GPT_VIDMEM_SHUTDOWN), GPT_EXTCR_HI_REG);
	
	return 0;
}

int gpt_vdo_init(void)
{
#if 0
	uint32_t regs;
#endif
	debug("vdo controller init\n");

	gpt_extcr_cfg();

	//vdo_regs = (struct gpt_vdo_reg*)GPT_VDO_APB_BASE;
    	/* disable video out controller */
	gpt_vdo_disable();
	/* wait interrupt finish */
	/* init video out controller */
#ifdef CONFIG_GPT_VDO_RGB2YUV
	debug("dma_init: CONFIG_GPT_VDO_RGB2YUV\n");
	gpt_vdo_dma_init(GPT_VDO_DMA_START, GPT_DATA_ADDR(3), GPT_DATA_S_ADDR(3), 0x10);
#elif  CONFIG_GPT_VDO_VDMEM
	debug("dma_init: CONFIG_GPT_VDO_VDMEM\n");
	gpt_vdo_dma_init(GPT_VDO_DMA_START, GPT_DATA_PXI_ADDR, GPT_DATA_S_PXI_ADDR, 0x10);
#else
	debug("dma_init: other\n");
	gpt_vdo_dma_init(GPT_VDO_DMA_START, GPT_DATA_ADDR(0), GPT_DATA_S_ADDR(0), 0x10);
#endif

	/* init line num */
	writel_debug(0x80, &GPT_VDO_REGS(dvo_int_line_num)); // why 0x80 ??
	/* mask irq */
	writel_debug(GPT_VDO_IRQ_MASK, &GPT_VDO_REGS(dvo_int_mask));

	/* init mpint interrupt */
#ifdef CONFIG_GPT_IRQ_ENABLE
	debug("******** CONFIG_GPT_IRQ_ENABLE******* \n");
	gpt_mpic_irq_register(GPT_VDO_IRQNO, gpt_vdo_interrupt, NULL);
	gpt_mpic_en(0, GPT_VDO_IRQNO);
#endif

#if 0
	/*axi outstanding number*/
	regs = readl_debug(&GPT_VDO_REGS(dvo_dma_outstd_num));
	writel_debug((regs & ~0x4001f) | GPT_VDO_OUTST_NUM(0x1), &GPT_VDO_REGS(dvo_dma_outstd_num));
	/*vdo fifo depth */
	writel_debug(GPT_VDO_FIFO_DEP(0x200), &GPT_VDO_REGS(dvo_dma_fifo_dep));
#endif
	return 0;
}

int gpt_vdo_background(int y, int cb, int cr)
{
	unsigned int color = ((y << 0) | (cb << 10) | (cr << 20));
	debug("background color :%x\n", color);
	writel_debug(color, &GPT_VDO_REGS(dvo_background));
	return 0;
}

int gpt_vdo_set_configs(int dma_h, int dma_v, int dma_burst_len, int dma_simip_en)
{
	writel_debug(dma_h & (dma_v << 16), &GPT_VDO_REGS(dvo_dma_position));
	writel_debug(dma_burst_len & (0x1 << 9), &GPT_VDO_REGS(dvo_dma_config));
	return 0;
}

int gpt_wait_vdo_ready(unsigned int bits)
{
	unsigned int status;
	unsigned int timeout = 0x1000000;
	do {
		status = readl_debug(&GPT_VDO_REGS(dvo_raw));
	#ifdef GPT_VDO_DEBUG
		debug("status: %x, bits:%x\n", status, bits);
	#endif
		if (status & bits)
			return 1;
	} while ((timeout--) > 0);

	return 0;
}

int gpt_vdo_reg_read(int offset)
{
	unsigned int *address = (unsigned int *)(GPT_VDO_APB_BASE) + offset / 4;
	int data = readl_debug(address);
	debug("offset: %x, data: %x \n",offset, data);
	return data;
}

void gpt_vdo_reg_write(int offset, int data)
{
	unsigned int *address = (unsigned int *)(GPT_VDO_APB_BASE) + offset / 4;
	writel_debug(data, address);
}

#define GPT_VDO_BUFF_SIZE 0x100
#define GPT_VDO_BUFF_NUM  0x6
static int index_src = 0;
static int index_dst = 0;

int gpt_request_buff(void)
{
	index_src = (index_src > GPT_VDO_BUFF_NUM) ? 0 : index_src;
	index_dst = (index_dst > GPT_VDO_BUFF_NUM) ? 0 : index_dst;
	if (index_dst == index_src) return -1;
	return (index_dst++) * GPT_VDO_BUFF_SIZE + GPT_DATA_ADDR(0);
}

int gpt_release_buff(void)
{
	unsigned int tmp;
	index_src = (index_src > GPT_VDO_BUFF_NUM) ? 0 : index_src;
	index_dst = (index_dst > GPT_VDO_BUFF_NUM) ? 0 : index_dst;
	tmp = (index_dst + 1 > GPT_VDO_BUFF_NUM) ? 0 : index_dst + 1;
	if (tmp >= index_src) return -1;
	return (index_src++) * GPT_VDO_BUFF_SIZE + GPT_DATA_ADDR(0);
}

int gpt_vdo_run(int count)
{
	unsigned int reg = readl_debug(&GPT_VDO_REGS(dvo_enable));
	unsigned int address;
//	gpt_vdi_init();
	gpt_vdo_init();

	do {
	#if 0
		gpt_vdi_options();
		if (gpt_vdi_ready(GPT_DVO_RAW_DMA_CMP))
	#endif
		{
			address = gpt_release_buff();
			writel_debug(address, &GPT_VDO_REGS(dvo_dma_base_0));
			writel_debug(reg | GPT_DVO_ENABLE, &GPT_VDO_REGS(dvo_enable));
		}
		count--;
	} while (count > 0);

	return 0;
}

int gpt_vdo_yuvdata(int y, int cb, int cr)
{
	gpt_vdo_data_prepare((unsigned char *)GPT_DATA_ADDR(0),  y, cb, cr, 1920, 1080, 1);
	return 0;
}

int gpt_vdo_data_generate(int type, int mode )
{
	int i;
	int y = 81;
	int cb = 239;
	int cr = 90;
	int line = 480;
	int colume = 720;
	int colordep = 0;
	int r, g, b;

	if(resl[mode].vdo_mode == GPT_DVO_FORMAT_16BIT){
		line = resl[mode].dvo_hd - resl[mode].dvo_ha;
	} else {
		line = (resl[mode].dvo_hd - resl[mode].dvo_ha) / 2;
	}	
	colume = resl[mode].dvo_vd_o -resl[mode].dvo_va_o;

	switch (type) {
	case 0:
		r = 0xff;
		g = 0xff;
		b = 0xff;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare((unsigned char *)GPT_DATA_ADDR(0),  y, cb, cr, line, colume, colordep);
		break;
	case 1:
		r = 0x00;
		g = 0x00;
		b = 0x00;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare((unsigned char *)GPT_DATA_ADDR(0),  y, cb, cr, line, colume, colordep);
		break;

	case 2:
		/*0x51 0xef 0x5a  red*/
		r = 0xff;
		g = 0x00;
		b = 0;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare((unsigned char *)GPT_DATA_ADDR(0), y, cb, cr, line, colume, colordep);
//		gpt_vdo_dumpdata((unsigned char *)GPT_DATA_ADDR, line * colume * 2);
		break;

	case 3:
		/*green*/
		r = 0;
		g = 0xff;
		b = 0;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare((unsigned char *)GPT_DATA_ADDR(0), y, cb, cr, line, colume, colordep);
//		gpt_vdo_dumpdata((unsigned char *)GPT_DATA_ADDR, line * colume * 2);
		break;

	case 4:
		/*blue*/
		r = 0x00;
		g = 0x00;
		b = 0xff;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare((unsigned char *)GPT_DATA_ADDR(0), y, cb, cr, line, colume, colordep);
//		gpt_vdo_dumpdata((unsigned char *)GPT_DATA_ADDR, line * colume * 2);
		break;
	case 5:
	//	gpt_vdo_colorbar((unsigned char *)GPT_DATA_ADDR(0), line, colume);
		gpt_vdo_rgb_colorbar((unsigned char *)GPT_DATA_ADDR(0), line, colume);
		break;
	case 6:
		gpt_vdo_interlace_data((unsigned char *)GPT_DATA_ADDR(0), 480, 720*2);
		break;
	case 7:
//		debug("generate serial data \n");
		gpt_vdo_range_data((unsigned char *)GPT_DATA_ADDR(0), line, colume);
		break;
	case 8:
		for (i = 0; i < 32; i++) {
			if ( i % 2  == 0)
				memset((void*)GPT_DATA_ADDR(0) + i * 0x14000, 0x00, 0x14000);
			else
				memset((void*)GPT_DATA_ADDR(0) + i * 0x14000, 0xff, 0x14000);
		}
		break;
	case 9:
		/*blue*/
		r = 0x00;
		g = 0x00;
		b = 0xff;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), y << 2, cb << 2, cr << 2, line, colume, colordep);
		break;
	case 10:
		/*green*/
		r = 0x00;
		g = 0xff;
		b = 0x00;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), y << 2, cb << 2, cr << 2, line, colume, colordep);
		break;
	case 11:
		/*red*/
		r = 0xff;
		g = 0x00;
		b = 0x00;
		gpt_vdo_ycbcr_exchange_rgb(1, &r, &g, &b, &y, &cb, &cr);
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), y << 2, cb << 2, cr << 2, line, colume, colordep);
		break;
	case 12:
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), 0x3ff, 0x3ff, 0x3ff, line, colume, colordep);
		break;

	case 13:
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), 0x003, 0x003, 0x003, line, colume, colordep);
		break;

	case 14:
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), 0x154, 0x154, 0x154, line, colume, colordep);
		break;

	case 15:
		gpt_vdo_data_prepare_10bit((unsigned short *)GPT_DATA_ADDR(0), 0x2a8, 0x2a8, 0x2a8, line, colume, colordep);
		break;

	case 16:
		gpt_cdma_mem_real_copy((uint8_t *)GPT_PXI_BASE, (uint8_t *)GPT_DATA_ADDR(0), 1920 * 1080 * 2);
		break;
	case 17:
		y = 0xdf;
		cb = 0x00;
		cr = 0x96;
		debug("####y:%x, cr:%x, cb:%x\n", y, cb, cr);
		gpt_vdo_ycbcr_exchange_rgb(0, &r, &g, &b, &y, &cb, &cr);
		debug("####r:%x, g:%x, b:%x\n", r, g, b);
		break;

	case 18:
		y = 0xdf;
		cb = 0x00;
		cr = 0xa4;
		debug("####y:%x, cr:%x, cb:%x\n", y, cb, cr);
		gpt_vdo_ycbcr_exchange_rgb(0, &r, &g, &b, &y, &cb, &cr);
		debug("####r:%x, g:%x, b:%x\n", r, g, b);
		break;

	case 19:
		y = 0xa0;
		cb = 0xa1;
		cr = 0xa2;
		debug("####y:%x, cr:%x, cb:%x\n", y, cb, cr);
		gpt_vdo_ycbcr_exchange_rgb(0, &r, &g, &b, &y, &cb, &cr);
		debug("####r:%x, g:%x, b:%x\n", r, g, b);
		break;

	case 20:
		y = 0x98;
		cb = 0x00;
		cr = 0x00;
		debug("####y:%x, cr:%x, cb:%x\n", y, cb, cr);
		gpt_vdo_ycbcr_exchange_rgb(0, &r, &g, &b, &y, &cb, &cr);
		debug("####r:%x, g:%x, b:%x\n", r, g, b);
		break;
	case 21:
		y = 0xdf;
		cb = 0x00;
		cr = 0xa4;
		debug("####y:%x, cr:%x, cb:%x\n", y, cb, cr);
		gpt_vdo_ycbcr_exchange_rgb(0, &r, &g, &b, &y, &cb, &cr);
		debug("####r:%x, g:%x, b:%x\n", r, g, b);
		break;
	case 22:
		y = 0xfa;
		cb = 0x80;
		cr = 0x80;
		debug("####y:%x, cr:%x, cb:%x\n", y, cb, cr);
		gpt_vdo_ycbcr_exchange_rgb(0, &r, &g, &b, &y, &cb, &cr);
		debug("####r:%x, g:%x, b:%x\n", r, g, b);
		break;
	default:
		debug("unsupport data format\n");
	}

	return 0;
}

void gpt_vdo_disable(void)
{
	unsigned int regs;
	int timeout = 0x100000;
	regs = readl_debug(&GPT_VDO_REGS(dvo_enable));
	writel_debug(regs & (~GPT_DVO_ENABLE), &GPT_VDO_REGS(dvo_enable)); //0 disable
#if 1
	while (1) {
		regs = readl_debug(&GPT_VDO_REGS(dvo_enable));
		timeout--;
		if (!(regs & 0x2))	//10 & 00   check is disabled ?
			return;

		if (timeout < 0) {
			debug("wait underflow timeout\n");
			return;
		}
	}
#else
	writel_debug(0xff, &GPT_VDO_REGS(dvo_int_clear));
	while (1) {
		regs = readl_debug(&GPT_VDO_REGS(dvo_raw));
		timeout--;

		if (regs & 0x8)
			return;

		if (timeout < 0) {
			debug("wait underflow timeout\n");
			return;
		}
	}
#endif
}
void gpt_vdo_enable_clock_delay(int clk_delay){
	unsigned int regs;
	//regs = readl_debug(&GPT_VDO_REGS(dvo_enable)) & ~(GPT_VDO_CLK_DELAY(0x1f));
	regs = readl_debug(&GPT_VDO_REGS(dvo_enable)) & ~(GPT_VDO_CLK_INV_EN) & ~(GPT_VDO_CLK_DELAY(0x1f));
	writel_debug(regs | GPT_VDO_CLK_INV_EN | GPT_VDO_CLK_DELAY_EN |GPT_VDO_CLK_DELAY(clk_delay), &GPT_VDO_REGS(dvo_enable));
	//writel_debug(regs | GPT_VDO_CLK_DELAY_EN | GPT_VDO_CLK_DELAY(clk_delay), &GPT_VDO_REGS(dvo_enable));
	//writel_debug(regs | GPT_VDO_CLK_DELAY_EN | GPT_VDO_CLK_INV_EN, &GPT_VDO_REGS(dvo_enable));
	//debug("vdo clk delay %d \n", clk_delay);
}

void gpt_vdo_enable(void)
{
	unsigned int regs;
	/**/
	regs = readl_debug(&GPT_VDO_REGS(dvo_enable));
	writel_debug(regs | GPT_DVO_ENABLE | GPT_VDO_CLK_INV_EN, &GPT_VDO_REGS(dvo_enable));
}

void gpt_vdo_polarity(int polarity)
{
	unsigned int regs;
	regs = readl_debug(&GPT_VDO_REGS(dvo_enable));
	regs &= ~(GPT_DVO_DE_HI | GPT_DVO_HSYNC_HI | GPT_DVO_VSYNC_HI);
	writel_debug(regs | polarity, &GPT_VDO_REGS(dvo_enable));
}

void gpt_vdo_dma_setup(int dma_hdat, int dma_high, int dma_stride)
{
	/**/
	writel_debug(dma_high, &GPT_VDO_REGS(dvo_dma_high));
	/**/
	writel_debug(dma_hdat, &GPT_VDO_REGS(dvo_dma_hdat));
	/**/
	writel_debug(dma_stride, &GPT_VDO_REGS(dvo_dma_stride));
}

void gpt_vdo_dma_addr(ulong dma_addr0_of, ulong dma_addr1_of)
{
#if 0
	ulong dma_addr0 = GPT_DATA_ADDR + dma_addr0_of;
	ulong dma_addr1 = GPT_DATA_S_ADDR + dma_addr1_of;
#else
	ulong dma_addr0 = (readl_debug(&GPT_VDO_REGS(dvo_dma_base_0)) & GPT_ADDR_MASK);
	ulong dma_addr1 = (readl_debug(&GPT_VDO_REGS(dvo_dma_base_1)) & GPT_ADDR_MASK);
#endif

	debug("dma offset: %lx, address: %lx\n", dma_addr0, dma_addr1);
	/* config dma mem address*/
	writel_debug(dma_addr0 + dma_addr0_of, &GPT_VDO_REGS(dvo_dma_base_0));

	/*config dma mem address*/
	writel_debug(dma_addr1 + dma_addr1_of, &GPT_VDO_REGS(dvo_dma_base_1));
}

void gpt_vdo_dma_init(unsigned int start, ulong dma_addr0, ulong dma_addr1, ulong dma_burst)
{
	unsigned int regs;
	/* dma start position */
	writel_debug(start, &GPT_VDO_REGS(dvo_dma_position));

	/* config dma mem address*/
	writel_debug(dma_addr0, &GPT_VDO_REGS(dvo_dma_base_0));
	writel_debug(((dma_addr0 >> 32) & 0xff), &GPT_VDO_REGS(dvo_dma_base_0_h));

	/*config dma mem address*/
	writel_debug(dma_addr1, &GPT_VDO_REGS(dvo_dma_base_1));
	writel_debug(((dma_addr1 >> 32) & 0xff), &GPT_VDO_REGS(dvo_dma_base_1_h));

	/* dma configure */
	regs = readl_debug(&GPT_VDO_REGS(dvo_dma_config));
	regs &= ~(0x1f << 0); 	//semiplanar ???
	regs |= dma_burst;	//0x10 ?? max burst length is 0 ??
	writel_debug(regs, &GPT_VDO_REGS(dvo_dma_config));
#if 0
	/*dma qos config*/
	regs = readl_debug(&GPT_VDO_REGS(dvo_dma_base_1_h));
	regs = GPT_VDO_QOS_CL(regs);
	regs |= GPT_VDO_QOS(0);
	writel_debug(regs, &GPT_VDO_REGS(dvo_dma_base_1_h));
#endif
}

void gpt_vdo_win_sync(int he, int hf, int ve_o,
			int vf_o, int ve_e, int vf_e)
{
	unsigned int regs;
	/**/
	regs = ((hf << 16) | (he << 0));
	writel_debug(regs, &GPT_VDO_REGS(dvo_hsync));

	regs = (((vf_o) << 16) | (ve_o << 0));
	writel_debug(regs, &GPT_VDO_REGS(dvo_vsync_odd));

	regs = (((vf_e) << 16) | (ve_e << 0));
	writel_debug(regs, &GPT_VDO_REGS(dvo_vsync_even));
}

void gpt_vdo_hwin_setup(int ha, int hd, int hb, int hc)
{
	unsigned int regs;
	/**/
	regs = ((hd << 16) | (ha << 0));
	writel_debug(regs, &GPT_VDO_REGS(dvo_hactive));
	/**/
	regs = ((hc << 16) | (hb << 0));
	writel_debug(regs, &GPT_VDO_REGS(dvo_hdisplay));
}

int gpt_vdo_vwin_odd_setup(int va, int vd, int vb, int vc, int vg, int hnum)
{
	unsigned int regs;
	/**/
	regs = (vd << 16) | (va << 0);
	writel_debug(regs, &GPT_VDO_REGS(dvo_vactive_odd));
	/**/
	regs = (vc << 16) | (vb << 0);
	writel_debug(regs, &GPT_VDO_REGS(dvo_vdisplay_odd));
	/**/
	regs = vg;
	writel_debug(regs, &GPT_VDO_REGS(dvo_bottom_blank_odd));

	regs = hnum;
	writel_debug(regs, &GPT_VDO_REGS(dvo_vsync_hnum_odd));
	return 0;
}

int gpt_vdo_vwin_evn_setup(int va, int vd, int vb, int vc, int vg, int hnum)
{
	unsigned int regs;
	/**/
	regs = (vd << 16) | (va << 0);
	writel_debug(regs, &GPT_VDO_REGS(dvo_vactive_even));
	/**/
	regs = (vc << 16) | (vb << 0);
	writel_debug(regs, &GPT_VDO_REGS(dvo_vdisplay_even));
	/**/
	regs = vg;
	writel_debug(regs, &GPT_VDO_REGS(dvo_bottom_blank_even));

	regs = hnum;
	writel_debug(regs, &GPT_VDO_REGS(dvo_vsync_hnum_even));
	return 0;
}

int gpt_vdo_config_mode(int data_mode, int display_mode, int sync_mode, int dvo_format)
{
	unsigned int regs;
	regs = readl_debug(&GPT_VDO_REGS(dvo_enable));
	regs &= ~((0x03 << 4) | (0x01 << 8) | (0x01 << 10));
	regs |= (display_mode | dvo_format) | (sync_mode);
	writel_debug(regs, &GPT_VDO_REGS(dvo_enable));

	regs = readl_debug(&GPT_VDO_REGS(dvo_dma_config));
	regs &= ~(0x01 << 9);
	regs |= data_mode;
	writel_debug(regs, &GPT_VDO_REGS(dvo_dma_config));

	return 0;
}

int gpt_vdo_set_resolution(struct gpt_vdo_param *param)
{
	unsigned int h_pixel = param->dvo_hd - param->dvo_ha;
	unsigned int h_pixel_st = param->dvo_hb - param->dvo_ha;
	unsigned int h_pixel_ed = param->dvo_hd - param->dvo_hc;
	unsigned int v_pixel_o = param->dvo_vb_o - param->dvo_va_o;
	unsigned int v_pixel_e = param->dvo_vb_e - param->dvo_va_e;
	debug("a:%x, d: %x, b:%x, c:%x \n", param->dvo_ha, param->dvo_hd, param->dvo_hb, param->dvo_hc);
	gpt_vdo_hwin_setup(param->dvo_ha, param->dvo_hd, param->dvo_hb, param->dvo_hc);
	gpt_vdo_vwin_odd_setup(param->dvo_va_o, param->dvo_vd_o, param->dvo_vb_o, param->dvo_vc_o, param->dvo_vg_o, param->hnum_odd);
	gpt_vdo_vwin_evn_setup(param->dvo_va_e, param->dvo_vd_e, param->dvo_vb_e, param->dvo_vc_e, param->dvo_vg_e, param->hnum_even);
	gpt_vdo_win_sync(param->dvo_he, param->dvo_hf, param->dvo_ve_o, param->dvo_vf_o, param->dvo_ve_e, param->dvo_vf_e);

	switch (param->vdo_mode) {
	case GPT_DVO_FORMAT_8BIT:
		if (param->data_mode == GPT_DVO_DMA_SEMIP_EN) {
			gpt_vdo_dma_setup((param->dvo_hc - param->dvo_hb) / 2,
				param->dvo_vc_o - param->dvo_vb_o, (param->dvo_hc - param->dvo_hb) / 2);

			dma_addr0_of = h_pixel_st / 2 + v_pixel_o * h_pixel / 2;
			dma_addr1_of = h_pixel_st / 2 + v_pixel_e * h_pixel / 2;
			gpt_vdo_dma_addr(h_pixel_st / 2 + v_pixel_o * h_pixel / 2, h_pixel_st / 2 + v_pixel_e * h_pixel / 2);
		} else {
			gpt_vdo_dma_setup((param->dvo_hc - param->dvo_hb),
				param->dvo_vc_o - param->dvo_vb_o, (param->dvo_hc - param->dvo_hb));

			dma_addr0_of = h_pixel_st + v_pixel_o * h_pixel;
			dma_addr1_of = h_pixel_st + v_pixel_e * h_pixel;
			gpt_vdo_dma_addr(h_pixel_st + v_pixel_o * h_pixel, h_pixel_st + v_pixel_e * h_pixel);
		}
		break;

	case GPT_DVO_FORMAT_10BIT:
	case GPT_DVO_FORMAT_16BIT:

		if (param->data_mode == GPT_DVO_DMA_SEMIP_EN) {
			gpt_vdo_dma_setup((param->dvo_hc - param->dvo_hb),
				param->dvo_vc_o - param->dvo_vb_o, (param->dvo_hc - param->dvo_hb));

			dma_addr0_of = h_pixel_st + v_pixel_o * h_pixel;
			dma_addr1_of = h_pixel_st + v_pixel_e * h_pixel;
			gpt_vdo_dma_addr(h_pixel_st + v_pixel_o * h_pixel, h_pixel_st + v_pixel_e * h_pixel);
		} else {
		#if 0 //720p data display on 480p
			gpt_vdo_dma_setup((param->dvo_hc - param->dvo_hb) * 2,
				param->dvo_vc_o - param->dvo_vb_o,
				(param->dvo_hc - param->dvo_hb + h_pixel_st + h_pixel_ed) * 2 + 1120);
		#else // xx resolution data display on xx resolution
			gpt_vdo_dma_setup((param->dvo_hc - param->dvo_hb) * 2,
				param->dvo_vc_o - param->dvo_vb_o,
				(param->dvo_hc - param->dvo_hb + h_pixel_st + h_pixel_ed) * 2);
		#endif

			dma_addr0_of = h_pixel_st * 2 + v_pixel_o * h_pixel * 2;
			dma_addr1_of = h_pixel_st * 2 + v_pixel_e * h_pixel * 2;
			gpt_vdo_dma_addr(h_pixel_st * 2 + v_pixel_o * h_pixel * 2, h_pixel_st * 2 + v_pixel_e * h_pixel * 2);
		}
		break;
	}

	gpt_vdo_polarity(param->vdo_polarity);

	return 0;
}

void gpt_vdo_lyytest(void)
{	
	int i =0;
	int state0 =0;
	for(i=0 ;i <150; i++){
		state0 = readl_debug(&GPT_VDO_REGS(dvo_raw));
		state0 = state0 & 0x02;
		debug("0x44=%d  ", state0);
		udelay(1000 * 1000);
	}
}

int gpt_vdo_mode(int mode)
{
	struct gpt_vdo_param *param = &resl[mode];
	debug("resl size :%d \n", (int)(sizeof(resl) / sizeof(struct gpt_vdo_param)));

	if (mode < 0 || mode > (sizeof(resl) / sizeof(struct gpt_vdo_param))) {
		debug("unsupport test mode\n");
		return -1;
	}

	//gpt_vdo_disable();
	gpt_vdo_config_mode(param->data_mode, param->disp_mode, param->sync_mode, param->vdo_mode);
	gpt_vdo_set_resolution(param);
	gpt_vdo_enable();
	//gpt_vdo_lyytest();
	return 0;
}

void gpt_vdo_switch_buff(void)
{
	ulong dma_addr0;
	ulong dma_addr1;
	static int num = 0;

	dma_addr0 = GPT_DATA_ADDR((num++) % 2);
	dma_addr1 = GPT_DATA_S_ADDR((num++) % 2);
#if 0
	switch((num++) % 2) {
	case 0:
		dma_addr0 = GPT_DATA_ADDR;
		dma_addr1 = GPT_DATA_S_ADDR;
		break;
	case 1:
		dma_addr0 = GPT_DATA_ADDR_1;
		dma_addr1 = GPT_DATA_S_ADDR_1;
		break;
	default:
		debug("num is unsupported, use default address");
		dma_addr0 = GPT_DATA_ADDR;
		dma_addr1 = GPT_DATA_S_ADDR;
	}
#endif
	/* config dma mem address*/
	writel_debug(dma_addr0, &GPT_VDO_REGS(dvo_dma_base_0));
	writel_debug(((dma_addr0 >> 32) & 0xff), &GPT_VDO_REGS(dvo_dma_base_0_h));

	/*config dma mem address*/
	writel_debug(dma_addr1, &GPT_VDO_REGS(dvo_dma_base_1));
	writel_debug(((dma_addr1 >> 32) & 0xff), &GPT_VDO_REGS(dvo_dma_base_1_h));
}

int gpt_vdo_interrupt(void *data)
{
	int status = 0;
#if 1
	int state = readl_debug(&GPT_VDO_REGS(dvo_int));

	if (state & GPT_DVO_INT_LINE) {
//		debug("lqq\n");
		gpt_vdo_switch_buff();
	//	writel_debug(0x1f, &GPT_VDO_REGS(dvo_int_mask));
		writel_debug(GPT_DVO_INT_LINE, &GPT_VDO_REGS(dvo_int_clear));
		status = 1;
	} else if (state & GPT_DVO_INT_FIFO_UNDER) {
		debug("video out underflow\n");
		writel_debug(GPT_DVO_INT_FIFO_UNDER, &GPT_VDO_REGS(dvo_int_clear));
		status =1;
	}
#endif
	return status;
}

int gpt_vdo_color_change_loop(int mode)
{
	int column = 1280;
	int row = 720;
	uint8_t *src;
	uint8_t *dst;
	int i, j;
	int rc, gc, bc;
	int y, cb, cr, y1, cb1, cr1;
	int rindex = 0, yindex = 0;
	memset((void *)GPT_DATA_ADDR(3), 0x0, 0x100000);
	while (1) {
		src = (void *)GPT_DATA_ADDR(0);
		dst = (void *)GPT_DATA_ADDR(3);

		for (i = 0; i < row; i++) {
			for (j = 0; j < column / 2; j++, rindex += 2, yindex++) {
				rc = src[rindex * 4 + 0];
				gc = src[rindex * 4 + 1];
				bc = src[rindex * 4 + 2];
				gpt_vdo_ycbcr_exchange_rgb(mode, &rc, &gc, &bc, &y, &cb, &cr);

				rc = src[(rindex + 1) * 4 + 0];
				gc = src[(rindex + 1) * 4 + 1];
				bc = src[(rindex + 1) * 4 + 2];
				gpt_vdo_ycbcr_exchange_rgb(mode, &rc, &gc, &bc, &y1, &cb1, &cr1);

				dst[yindex * 4 + 0] = (cb + cb1) / 2;
				dst[yindex *4 + 1] = y;
				dst[yindex * 4 + 2] = (cr + cr1) / 2;
				dst[yindex * 4 + 3] = y1;
			}
		}
		break;
	}
	return 0;
}
