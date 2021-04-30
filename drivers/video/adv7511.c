#include <common.h>
#include <command.h>
#include <i2c.h>
#include <asm/io.h>
#include <test/minitest.h>
#include <gpt_vdo.h>
#include <gpt_i2c_fix.h>
#include <asm/chip2/apiu.h>
#include <asm/chip2/sysctrl.h>

#define I2C_ADV7511_ADDR	0x72 //address for 0x72, bit0 indicate r/w
#if defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
#define I2C_ADV7511_BUS		0x02
#else
#ifdef CONFIG_TARGET_GP8300_EVK
#define I2C_ADV7511_BUS 	0x0  //adv7511 connected with i2c0 in fpga
#else
#define I2C_ADV7511_BUS 	0x1 //adv7511 connected with i2c1 in fpga
#endif
#endif

/*adv7511 register mainmap*/
#define ADV_REG_VIDEO_CTRL_15		0x15
#define ADV_REG_VIDEO_CTRL_16		0x16
#define ADV_REG_VIDEO_CTRL_17		0x17
#define ADV_REG_VIDEO_CTRL_30		0x30
#define ADV_REG_VIDEO_CTRL_31		0x31
#define ADV_REG_VIDEO_CTRL_32		0x32
#define ADV_REG_VIDEO_CTRL_33		0x33
#define ADV_REG_VIDEO_CTRL_34		0x34

#define ADV_REG_VIDEO_CTRL_35		0x35
#define ADV_REG_VIDEO_CTRL_36		0x36
#define ADV_REG_VIDEO_CTRL_37		0x37
#define ADV_REG_VIDEO_CTRL_38		0x38
#define ADV_REG_VIDEO_CTRL_39		0x39
#define ADV_REG_VIDEO_CTRL_3a		0x3a
#define ADV_REG_VIDEO_CTRL_3b		0x3b
#define ADV_REG_VIDEO_CTRL_3c		0x3c

#define ADV_REG_VIDEO_CTRL_d7		0xd7
#define ADV_REG_VIDEO_CTRL_d8		0xd8
#define ADV_REG_VIDEO_CTRL_d9		0xd9
#define ADV_REG_VIDEO_CTRL_da		0xda
#define ADV_REG_VIDEO_CTRL_db		0xdb
#define ADV_REG_VIDEO_CTRL_dc		0xdc

#define ADV_REG_VIDEO_CTRL_40		0x40
#define ADV_REG_ALL_PM_CTRL		0x41
#define ADV_REG_STATUS_0		0x42
#define ADV_REG_VIDEO_CTRL_48		0x48
#define ADV_REG_AVI_FRAME		0x55
#define ADV_REG_ACT_LINE_START_LSB	0x5a
#define ADV_REG_ACT_LINE_START_MSB	0x5b
#define ADV_REG_ACT_LINE_END_LSB	0x5c
#define ADV_REG_ACT_LINE_END_MSB	0x5d
#define ADV_REG_ACT_PIXEL_START_LSB	0x5e
#define ADV_REG_ACT_PIXEL_START_MSB	0x5f
#define ADV_REG_ACT_PIXEL_END_LSB	0x60
#define ADV_REG_ACT_PIXEL_END_MSB	0x61
#define ADV_REG_INTR_ENABLE_0		0x94
#define ADV_REG_INTR_ENABLE_1		0x95
#define ADV_REG_INTR_STATUS_0		0x96
#define ADV_REG_INTR_STATUS_1		0x97
#define ADV_REG_FIXED_BITS_98		0x98
#define ADV_REG_FIXED_BITS_9a		0x9a
#define ADV_REG_FIXED_BITS_9c		0x9c
#define ADV_REG_FIXED_BITS_a2		0xa2
#define ADV_REG_FIXED_BITS_a3		0xa3
#define ADV_REG_FIXED_I2C_ADDR		0xf9
#define ADV_REG_ALL_CTRL_af		0xaf
#define ADV_REG_VIDEO_CTRL_ba		0xba
#define ADV_REG_FIXED_BITS_e0		0xe0
#define ADV_REG_INPUT_CLK_DIV		0x9d
#define ADV_REG_TMDS_PM_CTRL		0xa1
#define ADV_REG_VIDEO_CTRL_d0		0xd0
#define ADV_REG_TMDS_CLK_SOFT 		0xd6
#define ADV_REG_CHIP_ID_HIGH		0xf5
#define ADV_REG_CHIP_ID_LOW 		0xf6

/**/
#define ADV_HPD_TYPE_POLL		(0x01 << 6)
#define ADV_HPD_TYPE_INTR		(0x01 << 7)
/**/
#define ADV_HPD_ACTIVE_OF		(0x01 << 7)
#define ADV_INPUT_CLK_DIV0		(0x00 << 2)
#define ADV_INPUT_CLK_DIV2		(0x01 << 2)
#define ADV_INPUT_CLK_DIV4		(0x02 << 2)
#define ADV_PM_POWER_DOWN		(0x01 << 6)

#define ADV_INPUT_ID_0			(0x00 << 0)	// 24 bit RGB 4:4:4 or YCbCr 4:4:4 (separte syncs)
#define ADV_INPUT_ID_1			(0x01 << 0)	//16,20,24 bit YCbCr 4:2:2(separate syncs);
#define ADV_INPUT_ID_2			(0x02 << 0)	//16,20,24 bit YCbCr 4:2:2(embedded syncs);
#define ADV_INPUT_ID_3			(0x03 << 0)	//8,10,12 bit YCbCr 4:2:2(2x pixel clock, separate syncs);
#define ADV_INPUT_ID_4			(0x04 << 0)	//8,10,12 bit YCbCr 4:2:2(2x pixel clock, dmbedded syncs);
#define ADV_INPUT_ID_5			(0x05 << 0)	//12,15,16 bit RGB 4:4:4 or YCbCr (DDR with separate syncs)
#define ADV_INPUT_ID_6			(0x06 << 0)	//8,10,12 bit RGB 4:2:2 or YCbCr (DDR with separate syncs)
#define ADV_INPUT_ID_7			(0x07 << 0)	//8,10,12 bit RGB 4:2:2 or YCbCr (DDR separate syncs)
#define ADV_INPUT_ID_8			(0x08 << 0)	//8,10,12 bit RGB 4:2:2 or YCbCr (DDR embedded syncs)
#define ADV_INPUT_DEFAULT_ID		ADV_INPUT_ID_1

#define ADV_INPUT_ALIGN_EVENN		(0x00 << 3)
#define ADV_INPUT_ALIGN_RIGTH		(0x01 << 3)
#define ADV_INPUT_ALIGN_LEFTT		(0x02 << 3)

#define ADV_INPUT_STYLE_1		(0x02 << 2)
#define ADV_INPUT_STYLE_2		(0x01 << 2)
#define ADV_INPUT_STYLE_3		(0x03 << 2)	//0x16 bit2-3

#define ADV_INPUT_COLOR_DP_08		(0x03 << 4)
#define ADV_INPUT_COLOR_DP_10		(0x01 << 4)
#define ADV_INPUT_COLOR_DP_12		(0x02 << 4)	//0x16 bit4-5

#define ADV_INPUT_DDR_RISING		(0x01 << 1)
#define ADV_INPUT_DDR_FALL		(0x00 << 1)	//0x16 bit1

#define ADV_INPUT_BLACK_RGB		(0x00 << 0)	//0x16 bit0	 RGB
#define ADV_INPUT_BLACK_YUV		(0x01 << 0)	//0x16 bit0 YCbCr

#define ADV_OUTPUT_FORMAT_422		(0x01 << 7)	//0x16 bit7
#define ADV_GC_PACKET_ENABLE		(0x01 << 7)
#define ADV_HDMI_MODE_SELECT		(0x01 << 1)

#define ADV_DE_GENERATE_EN		(0x01 << 0)
#define ADV_SYNC_GENERATE_EN		(0x01 << 1)

#define ADV_OUTPUT_AVI_RGB		(0x00 << 5)
#define ADV_OUTPUT_AVI_422		(0x01 << 5)
#define ADV_OUTPUT_AVI_444		(0x02 << 5)

#define ADV_SYNC_MODE_EXTERN		0x5a
#define ADV_SYNC_MODE_EMBEDED		0x5b
#define ADV_SYNC_MODE_ADJUST		0x5c

struct embedded_sync_param embeded_sync[15] = {
	[EM_SYNC_720P60] = {0x00 << 5, 0x1b, 0x82, 0x80, 0x14, 0x05, 0x40, 0xd9, 0x0a, 0x00, 0x2d, 0x00, 0x1b, 0x82, 0x80, 0x14, 0x05, 0x00}, //720p@60
	[EM_SYNC_480P] =     {0x03 << 5, 0x04, 0x03, 0xe0, 0x24, 0x06, 0x1e, 0x64, 0x05, 0xa0, 0x1e, 0x00, 0x04, 0x03, 0xe0, 0x24, 0x06, 0x00}, //480p
	[EM_SYNC_480I] =      {0x03 << 5, 0x04, 0xC3, 0xE0, 0x10, 0x03, 0x1d, 0x92, 0x05, 0xa0, 0x0f, 0x00, 0x04, 0xc3, 0xe0, 0x10, 0x03, 0x20}, //480i
	[EM_SYNC_576P] =     {0x03 << 5, 0x03, 0x04, 0x00, 0x14, 0x05, 0x20, 0xec, 0x05, 0xa0, 0x24, 0x00, 0x03, 0x04, 0x00, 0x14, 0x05, 0x00}, //576p
	[EM_SYNC_576I] =      {0x03 << 5, 0x03, 0x03, 0xF0, 0x08, 0x03, 0x20, 0xd6, 0x05, 0xa0, 0x12, 0x00, 0x03, 0x03, 0xf0, 0x08, 0x03, 0x20}, //576i
	[EM_SYNC_1080P50] = {0x00 << 5, 0x84, 0x02, 0xC0, 0x10, 0x05, 0x2f, 0xe9, 0x0f, 0x00, 0x43, 0x80, 0x84, 0x02, 0xc0, 0x10, 0x05, 0x00}, //1080p50
	[EM_SYNC_1080P60] = {0x00 << 5, 0x16, 0x02, 0xC0, 0x10, 0x05, 0x2f, 0xe9, 0x0f, 0x00, 0x43, 0x80, 0x16, 0x02, 0xc0, 0x10, 0x05, 0x00}, //1080p60
	[EM_SYNC_1080I30] = {0x00 << 5, 0x16, 0x02, 0xC0, 0x08, 0x05, 0x2f, 0xd4, 0x0f, 0x00, 0x21, 0xc0, 0x16, 0x02, 0xc0, 0x08, 0x05, 0x20}, //1080i30
	[EM_SYNC_1080P24] = {0x00 << 5, 0x9f, 0x82, 0xC0, 0x10, 0x05, 0x2f, 0xe9, 0x0f, 0x00, 0x89, 0xd0, 0x9f, 0x82, 0xc0, 0x10, 0x05, 0x00}, //1080p24
	[EM_SYNC_1080P30] = {0x00 << 5, 0x9f, 0x82, 0xC0, 0x10, 0x05, 0x2f, 0xe9, 0x0f, 0x00, 0x89, 0xd0, 0x9f, 0x82, 0xc0, 0x10, 0x05, 0x00}, //1080p30 same as 1080p@24
	[EM_SYNC_480P10B] = {0x03 << 5, 0x04, 0x03, 0xe0, 0x24, 0x06, 0x1e, 0x64, 0x05, 0xa0, 0x1e, 0x00, 0x04, 0x03, 0xe0, 0x24, 0x06, 0x00}, //480p
	[EM_SYNC_RESERVE] = {}
};

int adv7511_out_embeded_resolution(struct embedded_sync_param *resl);
int adv7511_set_sync_mode(int sync_mode);

void adv7511_write(unsigned int offset, unsigned int data)
{
	unsigned int ret = 0;
	ret += i2c_reg_bus_write(I2C_ADV7511_BUS, I2C_ADV7511_ADDR, offset, data);
	ADV7511_PDEBUG("I2C write data %s, *** 0x%x => 0x%x ***\n", (ret == 0 ? "SUCCESS!" : "FAILED!"), offset, data);
}

int adv7511_read(unsigned int offset)
{
	unsigned int data = i2c_reg_bus_read(I2C_ADV7511_BUS, I2C_ADV7511_ADDR, offset);
	ADV7511_PDEBUG("read data 0x%x => 0x%x \n", offset, data);
	return data;
}

#if 0
int adv7511_init2(void)
{
	int timeout;

	adv7511_read(0x96);
	while ((adv7511_read(0x96) & 0x80) != 0x80) {
		udelay(200*8000);
	}
	adv7511_read(0x96);

	debug("%s:%d hdmi init\n", __func__, __LINE__);

	adv7511_write(0x01, 0x00);
	adv7511_write(0x02, 0x18);
	adv7511_write(0x03, 0x00); /*0x1 ~ 0x3 audio clk generate*/

	adv7511_write(0x15, 0x00); /*input id: 0x1 seprate sync yuv422*/
	adv7511_write(0x16, 0x60); /*output 422, 8bit color dep, style2*/

	adv7511_write(0x17, 0x00); /*Aspect Ratio*/

	adv7511_write(0x18, 0x46); /*default value*/
	adv7511_write(0x40, 0x80); /*GC packet enable*/

	adv7511_read(0x41);

	for (timeout = 0x20; timeout > 0; timeout--) {
		adv7511_write(0x41, 0x10); /*power up all chip*/
		if ((adv7511_read(0x41) & 0x40) == 0) {
			debug("adv7511 power on\n");
			break;
		}
		udelay(1000*800);
	}

	if (timeout <= 0) {
		debug("adv7511 poweron timeout \n");
		return 0;
	}

	adv7511_write(0x48, 0x08); /*right justified*/

	adv7511_write(0x49, 0xa8); /*default value for bit timming mode*/
	adv7511_write(0x4c, 0x00); /*default valude*/
	adv7511_write(0x55, 0x40); /*avi output 422*/
	adv7511_write(0x56, 0x08); /*avi aspect ratio*/
	adv7511_write(0x96, 0x20);
	adv7511_write(0x98, 0x03); //Fixed
	adv7511_write(0x99, 0x02); //Fixed
//	adv7511_write(0x9a, 0xe0); //Fixed
	adv7511_write(0x9c, 0x3d); //Fixed
	adv7511_write(0x9d, 0x61); //Fixed & input clk divide 0
	adv7511_write(0xa2, 0xa4); //Fixed
	adv7511_write(0xa3, 0xa4); //Fixed
	adv7511_write(0xa5, 0x44); //Fixed
	adv7511_write(0xab, 0x40); //Fixed
	adv7511_write(0xaf, 0x16); //select HDMI mode
	adv7511_write(0xba, 0xa0); //
	adv7511_write(0xd0, 0x3c); //input clk no delay, no sync pulse
	adv7511_write(0xd1, 0xff); //Fixed
	adv7511_write(0xde, 0x9c); //????
//	adv7511_write(0xe0, 0xd0); //Fixed
	adv7511_write(0xe4, 0x60); //Fixed
//	adv7511_write(0xf9, 0x00); //Fixed i2c address
//	adv7511_write(0xfa, 0x00); //Hsync placement
#if 0
	/*for audio configure*/
	adv7511_write(0x0a, 0x10);
	adv7511_write(0x0b, 0x8e);
	adv7511_write(0x0c, 0x00);
	adv7511_write(0x73, 0x01);
	adv7511_write(0x14, 0x02);
#endif

	adv7511_read(0x0a);
	adv7511_read(0xc8);
	adv7511_read(0x9e);
	adv7511_read(0x96);
	adv7511_read(0x3e);
	adv7511_read(0x3d);
	adv7511_read(0x3c);

	return 0;
}

int adv7511_init1(void)
{
	int timeout;

//#define GPT_SYNC_MODE
//#define GPT_SYNC_16BIT
#define GPT_EMBEDED_8

#ifdef GPT_SYNC_MODE
	int data;
#else

#endif

	adv7511_read(0x96);
	while ((adv7511_read(0x96) & 0x80) != 0x80) {
		udelay(200*8000);
	}
	adv7511_read(0x96);

	debug("%s:%d hdmi init\n", __func__, __LINE__);

	adv7511_write(0x01, 0x00);
	adv7511_write(0x02, 0x18);
	adv7511_write(0x03, 0x00); /*0x1 ~ 0x3 audio clk generate*/
#ifdef GPT_SYNC_MODE
#ifdef GPT_SYNC_16BIT
	adv7511_write(0x15, 0x01); /*input id: 0x1 seprate sync yuv422*/
	adv7511_write(0x16, 0xb5); /*output 422, 8bit color dep, style2*/
#else
	/*for 8bit*/
	adv7511_write(0x15, 0x03); /*input id: 3 seprate sync yuv422*/
	adv7511_write(0x16, 0xb9); /*output 422, 8bit color dep, style3*/
	data = adv7511_read(0x3b);
	data &= ~0x18;
	adv7511_write(0x3b, data | (0x2 << 3)); /**/
#endif
#else
#ifdef GPT_EMBEDED_16
	adv7511_write(0x15, 0x02); /*input id: 0x1 eav/sav sync yuv422*/
//	adv7511_write(0x16, 0xbd); /*output 422, 8bit color dep, style3*/
	adv7511_write(0x16, 0xb5); /*output 422, 8bit color dep, style2*/
	adv7511_write(0x30, 0x1b); /* */
	adv7511_write(0x31, 0x82); /* */
	adv7511_write(0x32, 0x80); /* */
	adv7511_write(0x33, 0x14); /* */
	adv7511_write(0x34, 0x05); /* */
#endif

#ifdef GPT_EMBEDED_10
	adv7511_write(0x15, 0x04); /*input id: 0x1 eav/sav sync yuv422*/
	adv7511_write(0x16, 0x99); /*output 422, 8bit color dep, style2*/
	adv7511_write(0x30, 0x04); /* */
	adv7511_write(0x31, 0x03); /* */
	adv7511_write(0x32, 0xe0); /* */
	adv7511_write(0x33, 0x24); /* */
	adv7511_write(0x34, 0x06); /* */
	adv7511_write(0x35, 0x1e); /* */
	adv7511_write(0x36, 0x64); /* */
	adv7511_write(0x37, 0x05); /* */
	adv7511_write(0x38, 0xa0); /* */
	adv7511_write(0x39, 0x1e); /* */
	adv7511_write(0x3a, 0x00); /* */
	adv7511_write(0xd7, 0x04); /* */
	adv7511_write(0xd8, 0x03); /* */
	adv7511_write(0xd9, 0xe0); /* */
	adv7511_write(0xda, 0x24); /* */
	adv7511_write(0xdb, 0x06); /* */
	adv7511_write(0xdc, 0x00); /* */
	adv7511_write(0xdd, 0x00); /* */
#endif

#ifdef GPT_EMBEDED_8
	debug("lllll\n");
	adv7511_write(0x15, 0x04); /*input id: 0x1 eav/sav sync yuv422*/
	adv7511_write(0x16, 0xb9); /*output 422, 8bit color dep, style1*/

	adv7511_write(0x17, 0x63); /*Aspect Ratio*/
	adv7511_write(0x30, 0x04); /* */
	adv7511_write(0x31, 0xc3); /* */
	adv7511_write(0x32, 0xe0); /* */
	adv7511_write(0x33, 0x10); /* */
	adv7511_write(0x34, 0x03); /* */

	adv7511_write(0x35, 0x1d); /* */
	adv7511_write(0x36, 0x92); /* */
	adv7511_write(0x37, 0x05); /* */
	adv7511_write(0x38, 0xa0); /* */
	adv7511_write(0x39, 0x0f); /* */
	adv7511_write(0x3a, 0x00); /* */

	adv7511_write(0xd7, 0x04); /* */
	adv7511_write(0xd8, 0xc3); /* */
	adv7511_write(0xd9, 0xe0); /* */
	adv7511_write(0xda, 0x10); /* */
	adv7511_write(0xdb, 0x03); /* */
	adv7511_write(0xdc, 0x20); /* */

#endif

#endif

	adv7511_write(0x18, 0x46); /*default value*/
	adv7511_write(0x40, 0x80); /*GC packet enable*/

	adv7511_read(0x41);
	for (timeout = 0x20; timeout > 0; timeout--) {
		adv7511_write(0x41, 0x12); /*power up all chip*/
		if ((adv7511_read(0x41) & 0x40) == 0) {
			printf("adv7511 power on\n");
			break;
		}
		udelay(1000*800);
	}
	if (timeout <= 0) {
		debug("adv7511 poweron timeout \n");
		return 0;
	}

	debug("0x41: %x\n", adv7511_read(0x41));

//	adv7511_write(0x48, 0x08); /*right justified*/
	adv7511_write(0x48, 0x00); /*evenly justified*/
	adv7511_write(0x49, 0xa8); /*default value for bit timming mode*/
	adv7511_write(0x4c, 0x00); /*default valude*/
	adv7511_write(0x55, 0x20); /*avi output 422*/
	adv7511_write(0x56, 0x08); /*avi aspect ratio*/
	adv7511_write(0x96, 0x20);
	adv7511_write(0x98, 0x03); //Fixed
	adv7511_write(0x99, 0x02); //Fixed
	adv7511_write(0x9a, 0xe0); //Fixed
	adv7511_write(0x9c, 0x30); //Fixed
	adv7511_write(0x9d, 0x61); //Fixed & input clk divide 0
	adv7511_write(0xa2, 0xa4); //Fixed
	adv7511_write(0xa3, 0xa4); //Fixed
	adv7511_write(0xa5, 0x44); //Fixed
	adv7511_write(0xab, 0x40); //Fixed
	adv7511_write(0xaf, 0x06); //select HDMI mode
	adv7511_write(0xba, 0x00); //
	adv7511_write(0xd0, 0x3c); //input clk no delay, no sync pulse
	adv7511_write(0xd1, 0xff); //Fixed
	adv7511_write(0xde, 0x9c); //????
	adv7511_write(0xe0, 0xd0); //Fixed
	adv7511_write(0xe4, 0x60); //Fixed
	adv7511_write(0xf9, 0x00); //Fixed i2c address
	adv7511_write(0xfa, 0x00); //Hsync placement
	adv7511_write(0xfb, 0x00); //Hsync placement
//	adv7511_write(0x17, 0x02); /*Aspect Ratio*/

	/*for audio configure*/
	adv7511_write(0x0a, 0x10);
	adv7511_write(0x0b, 0x8e);
	adv7511_write(0x0c, 0x00);
	adv7511_write(0x73, 0x01);
	adv7511_write(0x14, 0x02);

	adv7511_read(0x0a);
	adv7511_read(0xc8);
	adv7511_read(0x9e);
	adv7511_read(0x96);
	adv7511_read(0x3e);
	adv7511_read(0x3d);
	adv7511_read(0x3c);

	return 0;
}
#endif

int adv7511_get_version(void)
{
	int id_h, id_l;
	id_h = adv7511_read(ADV_REG_CHIP_ID_HIGH);
	id_l = adv7511_read(ADV_REG_CHIP_ID_LOW);
	return ((id_h << 8) & 0xff00) | id_l;
}

int adv7511_set_avmute(void)
{
	return 0;
}

int adv7511_set_mute_audio(void)
{
	return 0;
}

int adv7511_set_mute_video(void)
{
	return 0;
}

int adv7511_get_hpd(int type)
{
	int ret = -1;
	switch (type) {
	case ADV_HPD_TYPE_POLL:
		ret = adv7511_read(ADV_REG_STATUS_0); //0x42
		break;
	case ADV_HPD_TYPE_INTR:
		ret = adv7511_read(ADV_REG_INTR_STATUS_0); //0x96
		break;
	default:
		debug("Unsupport type for hpd \n");
	}

	return ret;
}

int adv7511_enable_tmds(void)
{
	/*tmds powerup channel0 ~ 2, driver, monitor sense enable*/
	adv7511_write(ADV_REG_TMDS_PM_CTRL, 0x00);
	return 0;
}

int adv7511_input_pixel_format(int format,  int style, int align,
			int bitpercolor, int risingedge, int resl)
{
	int regs;
	debug("%s:%d --> id:%x, style:%x, align:%x, colordep:%x, edge:%x, resl:%x\n", __func__,
			__LINE__, format, style, align, bitpercolor, risingedge, resl);
	//id 1 style c, 0 30 0 b
	//id 3 style 8 0 30 0 0
	adv7511_write(ADV_REG_VIDEO_CTRL_15, format);	//
//	regs = adv7511_read(ADV_REG_VIDEO_CTRL_16);
//                                             8         0            30               1
	adv7511_write(ADV_REG_VIDEO_CTRL_16, style | risingedge | bitpercolor | ADV_INPUT_BLACK_YUV);
	adv7511_write(ADV_REG_VIDEO_CTRL_48, align);		//??
	regs = adv7511_read(ADV_REG_AVI_FRAME);
	regs &= ~0xc0;
	adv7511_write(ADV_REG_AVI_FRAME, regs | ADV_OUTPUT_AVI_422);

	adv7511_write(ADV_REG_VIDEO_CTRL_dc, 0x0);
#if 0
	switch (resl) {
	case EM_SYNC_480I:
	case EM_SYNC_576I:
	case EM_SYNC_1080I30:
		adv7511_set_sync_mode(ADV_SYNC_MODE_ADJUST);
		break;
	default:
		adv7511_set_sync_mode(ADV_SYNC_MODE_EMBEDED);
	}
#endif
	switch (format) {
	case ADV_INPUT_ID_2:
	case ADV_INPUT_ID_4:
	case ADV_INPUT_ID_8:
		adv7511_out_embeded_resolution(&embeded_sync[resl]);
		break;
	case ADV_INPUT_ID_0:
	case ADV_INPUT_ID_1:
	case ADV_INPUT_ID_3:
	case ADV_INPUT_ID_5:
	case ADV_INPUT_ID_6:
	case ADV_INPUT_ID_7:
		//adv7511_out_embeded_resolution(&embeded_sync[resl]); // add by lyy 2019-06-06
		break;
	default:
		debug("Unsupport ID code\n");
	}

	return 0;
}

int adv7511_out_pixel_format(int format, int gc_enable, int hdmi_en)
{
	int regs;
	regs = adv7511_read(ADV_REG_VIDEO_CTRL_16);
	adv7511_write(ADV_REG_VIDEO_CTRL_16, regs | format);

	regs = adv7511_read(ADV_REG_VIDEO_CTRL_40);
	adv7511_write(ADV_REG_VIDEO_CTRL_40, regs | gc_enable);

	regs = adv7511_read(ADV_REG_ALL_CTRL_af);
	adv7511_write(ADV_REG_ALL_CTRL_af, regs | hdmi_en);

	return 0;
}

int adv7511_set_resolution(int bits, int sync, int resl)
{
	int id = ADV_INPUT_ID_1;
	int style = ADV_INPUT_STYLE_1;
	int color_dp = ADV_INPUT_COLOR_DP_08;

#if defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	int align = ADV_INPUT_ALIGN_LEFTT;
#else
	int align = ADV_INPUT_ALIGN_EVENN;
#endif

	switch (bits) {
	case 8:
		if (sync)
			id = ADV_INPUT_ID_3;  
		else
			id = ADV_INPUT_ID_4;
		style = ADV_INPUT_STYLE_1;
		color_dp = ADV_INPUT_COLOR_DP_08;
		break;
	case 10:
		if (sync)
			id = ADV_INPUT_ID_3;
		else
			id = ADV_INPUT_ID_4;
		style = ADV_INPUT_STYLE_1;
		color_dp = ADV_INPUT_COLOR_DP_10;
		break;
	case 16:
		if (sync)
			id = ADV_INPUT_ID_1;
		else
			id = ADV_INPUT_ID_2;
		style = ADV_INPUT_STYLE_3;
		color_dp = ADV_INPUT_COLOR_DP_08;
		break;
	default:
		debug("Unsupport bit type\n");
		return -1;
	}

	adv7511_input_pixel_format(id, style, align, color_dp, ADV_INPUT_DDR_FALL, resl);
	adv7511_out_pixel_format(ADV_OUTPUT_FORMAT_422, ADV_GC_PACKET_ENABLE, ADV_HDMI_MODE_SELECT);

	return 0;
}

int adv7511_set_sync_mode(int sync_mode)
{
	int regs;

	switch (sync_mode) {

	case ADV_SYNC_MODE_EXTERN:
	case ADV_SYNC_MODE_EMBEDED:
		regs = adv7511_read(ADV_REG_ALL_PM_CTRL);
		adv7511_write(ADV_REG_ALL_PM_CTRL, regs & (~ADV_SYNC_GENERATE_EN));
		regs = adv7511_read(ADV_REG_VIDEO_CTRL_17);
		adv7511_write(ADV_REG_VIDEO_CTRL_17, regs & (~ADV_DE_GENERATE_EN));
		break;
#if 0
	case ADV_SYNC_MODE_EMBEDED:
		regs = adv7511_read(ADV_REG_ALL_PM_CTRL);
		adv7511_write(ADV_REG_ALL_PM_CTRL, regs | ADV_DE_GENERATE_EN);
		regs = adv7511_read(ADV_REG_VIDEO_CTRL_17);
		adv7511_write(ADV_REG_VIDEO_CTRL_17, regs & (~ADV_SYNC_GENERATE_EN));
		break;
#endif
	case ADV_SYNC_MODE_ADJUST:
		regs = adv7511_read(ADV_REG_ALL_PM_CTRL);
		adv7511_write(ADV_REG_ALL_PM_CTRL, regs | ADV_SYNC_GENERATE_EN);
		regs = adv7511_read(ADV_REG_VIDEO_CTRL_17);
		adv7511_write(ADV_REG_VIDEO_CTRL_17, regs | ADV_DE_GENERATE_EN);
		break;

	default:
		debug("sync mode is unsupported\n");
	}

	return 0;
}

int adv7511_out_embeded_resolution(struct embedded_sync_param *resl)
{
	debug("%s----------------\n", __func__);

	int regs;
	regs = adv7511_read(ADV_REG_VIDEO_CTRL_17);
	regs &= ~(0x3 << 5);
	adv7511_write(ADV_REG_VIDEO_CTRL_17, regs | resl->reg_17);

	adv7511_write(ADV_REG_VIDEO_CTRL_30, resl->reg_30);
	adv7511_write(ADV_REG_VIDEO_CTRL_31, resl->reg_31);
	adv7511_write(ADV_REG_VIDEO_CTRL_32, resl->reg_32);
	adv7511_write(ADV_REG_VIDEO_CTRL_33, resl->reg_33);
	adv7511_write(ADV_REG_VIDEO_CTRL_34, resl->reg_34);

	adv7511_write(ADV_REG_VIDEO_CTRL_35, resl->reg_35);
	adv7511_write(ADV_REG_VIDEO_CTRL_36, resl->reg_36);
	adv7511_write(ADV_REG_VIDEO_CTRL_37, resl->reg_37);
	adv7511_write(ADV_REG_VIDEO_CTRL_38, resl->reg_38);
	adv7511_write(ADV_REG_VIDEO_CTRL_39, resl->reg_39);
	adv7511_write(ADV_REG_VIDEO_CTRL_3a, resl->reg_3a);

	adv7511_write(ADV_REG_VIDEO_CTRL_d7, resl->reg_d7);
	adv7511_write(ADV_REG_VIDEO_CTRL_d8, resl->reg_d8);
	adv7511_write(ADV_REG_VIDEO_CTRL_d9, resl->reg_d9);
	adv7511_write(ADV_REG_VIDEO_CTRL_da, resl->reg_da);
	adv7511_write(ADV_REG_VIDEO_CTRL_db, resl->reg_db);
	adv7511_write(ADV_REG_VIDEO_CTRL_dc, resl->reg_dc);

	return 0;
}

int adv7511_set_csc(int incolorspace, int outcolorspace)
{
	return 0;
}

int adv7511_set_audio_interface(int inputformat, int outtype, int hbrstrmcount)
{
	return 0;
}

int adv7511_enable_events(int events, int enable)
{
	return 0;
}

int adv7511_int_pendding(void)
{
	return 0;
}

int adv7511_enable_packets(int packets, int enable)
{
	return 0;
}

int adv7511_edid_debug(int enable)
{
	return 0;
}

static void enable_i2c_ctrl(void * addr)
{
	writel(0x1fff5503, addr);
}

static void gpt_i2c_pinmux_reg(void)
{
	writel(readl(SYSCTRL_IOCNF) | 0x9 << 24, SYSCTRL_IOCNF);//pinmux enable i2c1 and i2c2
}

int adv7511_init(void)
{
	int ret;
	int timeout = 0x10;
	unsigned int reg;

	gpt_i2c_pinmux_reg();
#if defined(CONFIG_TARGET_GP8300_DVB) || defined(CONFIG_TARGET_GP8300_CHAOKONG)
	enable_i2c_ctrl((void *) (APIU_I2C3 | APIU_UNIT_UCR));
#else
	enable_i2c_ctrl((void *)(APIU_I2C1 | APIU_UNIT_UCR));
#endif

	debug("device version: %x \n", adv7511_get_version());
	do {
		ret = adv7511_get_hpd(ADV_HPD_TYPE_POLL);
	} while ((!(ret & ADV_HPD_TYPE_POLL)) && (timeout-- > 0));  // hdp detect state is high; return.

	if (timeout <= 0) {
		debug("No Slink connecte to adv7511\n");
		return -1;
	}
	/*power on module*/
	reg = adv7511_read(ADV_REG_ALL_PM_CTRL);
//	reg |= 0x40;
	adv7511_write(ADV_REG_ALL_PM_CTRL, reg & (~ADV_PM_POWER_DOWN)); //0x41 bit6 set value 1.(hdp pin=1, 0x41[6]=0, power down pin=0, now power up !)
	//udelay(0x100000);
	/*fixed register bits*/
	adv7511_write(ADV_REG_FIXED_BITS_98, 0x03);	//must be set 0x98=0x03
	adv7511_write(ADV_REG_FIXED_BITS_9a, 0xe0);	//must be set 0b1110 000x
	adv7511_write(ADV_REG_FIXED_BITS_9c, 0x30);	//must be set 0x30
	/*clk div 0, default clk from clkpin*/
	adv7511_write(ADV_REG_INPUT_CLK_DIV, ADV_INPUT_CLK_DIV0 | 0x61);	//0x9d = 0x61. input CLK not Divided.
	adv7511_write(ADV_REG_FIXED_BITS_a2, 0xa4);	//0xa2 = 0xa4
	adv7511_write(ADV_REG_FIXED_BITS_a3, 0xa4);	//0xa3 = 0xa4
	adv7511_write(ADV_REG_FIXED_BITS_e0, 0xd0);	//0xe0 = 0xd0

	adv7511_write(ADV_REG_FIXED_I2C_ADDR, 0x00); //fixed i2c address   //??? 0x00???

	reg = adv7511_read(ADV_REG_ALL_CTRL_af);
	adv7511_write(ADV_REG_ALL_CTRL_af, reg & (~0x10)); //disable hdcp frame encrypt


	adv7511_write(ADV_REG_VIDEO_CTRL_17, 0x2); //aspect ratio 0x17[1] => 0b1=16:9 0b0=4:3
/*
	adv7511_input_pixel_format(ADV_INPUT_DEFAULT_ID, ADV_INPUT_STYLE_2,
			ADV_INPUT_ALIGN_EVENN, ADV_INPUT_COLOR_DP_08, ADV_INPUT_DDR_FALL);

	adv7511_out_pixel_format(ADV_OUTPUT_FORMAT_422, ADV_GC_PACKET_ENABLE, ADV_HDMI_MODE_SELECT);
*/
//	adv7511_enable_tmds();
	return 0;
}

void adv7511_show_positon(void)
{
	debug("Active Line Start LSB : %x \n",
			adv7511_read(ADV_REG_ACT_LINE_START_LSB));
	debug("Active Line Start MSB : %x \n",
			adv7511_read(ADV_REG_ACT_LINE_START_MSB));
	debug("Active Line End LSB   : %x \n",
			adv7511_read(ADV_REG_ACT_LINE_END_LSB));
	debug("Active Line End MSB   : %x \n",
			adv7511_read(ADV_REG_ACT_LINE_END_MSB));
	debug("Active Pixel Start LSB: %x \n",
			adv7511_read(ADV_REG_ACT_PIXEL_START_LSB));
	debug("Active Pixel Start MSB: %x \n",
			adv7511_read(ADV_REG_ACT_PIXEL_START_MSB));
	debug("Active Pixel End  LSB : %x \n",
			adv7511_read(ADV_REG_ACT_PIXEL_END_LSB));
	debug("Active Pixel End  MSB : %x \n",
			adv7511_read(ADV_REG_ACT_PIXEL_END_MSB));
}
