#ifndef __GPT_DDR3L__
#define __GPT_DDR3L__

#define GPT_DDR_PHY_SLICE_CNT		1
#define GPT_DDR_PHY_SLICE_INTERVAL	0x400

#define GPT_DDR_INIT_CONFIG2		0X600
#define GPT_DDR_ENABLE_CONFIG		0X10001000
#define GPT_DDR_INTR_CLEAR_CONFIG	0x800
#define GPT_DDR_32BIT_CONFIG		(0x1 << 8)
#define GPT_DDR_DFS_REQ_CONFIG		0x0145
#define GPT_DDR_DFS_ACK_CONFIG		0x2145
#define GPT_DDR_CLEAR_LP_REQ_CONFIG	0x0000
#define GPT_DDR_FREQ_SELECT_CONFIG	0x01
#define GPT_DDR_PHY_CTRL_UPD_DIS_CONFIG	0x1
#define GPT_DDR_PHY_TRIGGER_UPDATE 	0x1
#define GPT_DDR_CTRL_TRIGGER_MRW	(0x1 << 25)
#define GPT_DDR_PHY_CTRL_LPBK_EN	(0x1 << 8)
#define GPT_DDR_PHY_EXTE_LB_EN          (0X1 << 9)
#define GPT_DDR_PHY_LBPK_DATA_TYPE	(0x1 << 10)
#define GPT_DDR_PHY_LBPK_CNT		(0x01 << 13)
#define GPT_DDR_PHY_CTRL_LPBK_GO	(0x1 << 15)
#define GPT_DDR_PHY_DDL_DELAY_EN	0x1
#define GPT_DDR_PHY_DDL_GROUP_MODE	(0x2 << 9)
#define GPT_DDR_PHY_DDL_MASK_CUSTOM	(0x2 << 3)
#define GPT_DDR_PHY_DDL_MASK_WHOLE	0x2
#define GPT_DDR_CTL_SWLVL_START		0x1
#define GPT_DDR_CTL_SWLVL_EXIT		(0x1 << 8)
#define GPT_DDR_CTL_WRLVL_MODE		(0x1 << 16)
#define GPT_DDR_CTL_DATA_EYE_MODE	(0x2 << 16)
#define GPT_DDR_CTL_GATE_MODE		(0x3 << 16)

#define GPT_DDR_INIT_STATUS		0x20400000
#define GPT_DDR_INIT_STATUS2		0X20400600
#define GPT_DDR_INTR_STATUS		(0x1 << 28)
#define GPT_DDR_INTR_STATUS2		0x800
#define CNTL_FREQ_CHANGE_REQ_STS	(0x1 << 12)
#define GPT_DDR_LP_EXT_DONE_STS		(0x1 << 23)
#define GPT_DDR_LP_EXT_RESP_STS		(~(0x1 << 14))
#define GPT_DDR_LP_EXT_ACK_STS		(~(0x1 << 1))
#define GPT_DDR_DDL_DELAY_DONE_STS	0x1
#define GPT_DDR_DDL_BIST_DONE_STS	0x1
#define GPT_DDR_SWLVL_OP_DONE_STS	(0x1 << 16)

#define GPT_DPLL_CNTL_ADDR		0xf0000038
#define GPT_EXTCR_LO_ADDR		0xf0000100
#define GPT_DDR_BASE_ADDR		0xf0200000
#define GPT_PHY_BASE_ADDR		0x1000
#define GPT_PI_BASE_ADDR		0x800
#define GPT_DDR_DFS_CNTL_ADDR		0xf0200f00
#define GPT_DDR_INTR_ADDR		0xf0007294
#define GPT_DDR_CNTL_ADDR		0xf0200658
#define GPT_DDR_INTR_CLEAR_ADDR		0xf0200660
#define GPT_DDR_32BIT_ADDR		0xf0200614
#define GPT_DDR_FREQ_SELE_ADDR		0xf0207000
#define GPT_DDR_WRITE_MODE_ADDR		(0xf0200000 + 195 * 4)
#define GPT_DDR_SWLVL_MODE_ADDR		(0xf0200000 + 449 * 4)
#define GPT_DDR_SWLVL_OP_ADDR		(0xf0200000 + 450 * 4)
#define GPT_DDR_PHY_UPDATE_ADDR		(0xf0200000 + (4096 + 3170) * 4)
#define GPT_DDR_PHY_MANUAL_ADDR		(0xf0200000 + (4096 + 3081) * 4)
#define GPT_DDR_PHY_LPBK_CONTROL_ADDR	(0xf0200000 + (4096 + 8) * 4)
#define GPT_DDR_PHY_DDL_TEST_ADDR	(0xf0200000 + (4096 + 61) * 4)
#define GPT_DDR_PHY_DDL_MASK_ADDR	(0xf0200000 + (4096 + 62) * 4)
#define GPT_DDR_PHY_DDL_TEST_OBS_ADDR	(0xf0200000 + (4096 + 63) * 4)

typedef enum ddr_mode {
	DDR_1066 = 1066000000,
	DDR_1333 = 1333000000,
	DDR_1600 = 1600000000,
	DDR_2133 = 2133000000,
}ddr_mode_e;
#define CONFIG_DDR_1066			0
#define CONFIG_DDR_1333			1
#define CONFIG_DDR_1600			2
#define CONFIG_DDR_2133			3

int gpt_ddr_init(void);

#endif