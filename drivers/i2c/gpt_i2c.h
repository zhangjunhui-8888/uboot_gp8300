/*
 * (C) Copyright 2009
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __DW_I2C_H_
#define __DW_I2C_H_

#include <gpt-mpint.h>
struct i2c_regs {
	u32 ic_con;
	u32 ic_tar;
	u32 ic_sar;
	u32 ic_hs_maddr;
	u32 ic_cmd_data;
	u32 ic_ss_scl_hcnt;
	u32 ic_ss_scl_lcnt;
	u32 ic_fs_scl_hcnt;
	u32 ic_fs_scl_lcnt;
	u32 ic_hs_scl_hcnt;
	u32 ic_hs_scl_lcnt;
	u32 ic_intr_stat;
	u32 ic_intr_mask;
	u32 ic_raw_intr_stat;
	u32 ic_rx_tl;
	u32 ic_tx_tl;
	u32 ic_clr_intr;
	u32 ic_clr_rx_under;
	u32 ic_clr_rx_over;
	u32 ic_clr_tx_over;
	u32 ic_clr_rd_req;
	u32 ic_clr_tx_abrt;
	u32 ic_clr_rx_done;
	u32 ic_clr_activity;
	u32 ic_clr_stop_det;
	u32 ic_clr_start_det;
	u32 ic_clr_gen_call;
	u32 ic_enable;
	u32 ic_status;
	u32 ic_txflr;
	u32 ix_rxflr;
	u32 reserved_1;
	u32 ic_tx_abrt_source;
};

#ifdef CONFIG_GPT_IRQ_ENABLE
int I2C_TX_IRQ[] = {
    I2C1_HW_IRQN_TX,
    I2C2_HW_IRQN_TX,
    I2C3_HW_IRQN_TX,
#ifndef CONFIG_TEST_ON_SOC
    I2C4_HW_IRQN_TX
#endif
};
int I2C_RX_IRQ[] = {
	I2C1_HW_IRQN_RX,
    I2C2_HW_IRQN_RX,
    I2C3_HW_IRQN_RX,
#ifndef CONFIG_TEST_ON_SOC
    I2C4_HW_IRQN_RX
#endif
};
#endif

#if !defined(IC_CLK)
#define IC_CLK	 166
#endif
#define NANO_TO_MICRO		1000

/* High and low times in different speed modes (in ns) */
#define MIN_SS_SCL_HIGHTIME	4000
#define MIN_SS_SCL_LOWTIME	4700
#define MIN_FS_SCL_HIGHTIME	600
#define MIN_FS_SCL_LOWTIME	1300
#define MIN_HS_SCL_HIGHTIME	60
#define MIN_HS_SCL_LOWTIME	160

/* Worst case timeout for 1 byte is kept as 2ms */
//  i2c bus status reg can not being polling if I2C_BYTE_TO value is too small!!!
#define I2C_BYTE_TO		(CONFIG_SYS_HZ/200)	// WARNING!!! POLARIS FPGA 
#define I2C_STOPDET_TO		(CONFIG_SYS_HZ/200)
#define I2C_BYTE_TO_BB		(I2C_BYTE_TO * 16)

/* i2c control register definitions */
#define IC_CON_SD		0x0040
#define IC_CON_RE		0x0020
#define IC_CON_10BITADDRMASTER	0x0010
#define IC_CON_10BITADDR_SLAVE	0x0008
#define IC_CON_SPD_MSK		0x0006
#define IC_CON_SPD_SS		0x0002
#define IC_CON_SPD_FS		0x0004
#define IC_CON_SPD_HS		0x0006
#define IC_CON_MM		0x0001

/* i2c target address register definitions */
#define TAR_ADDR		0x0050

/* i2c slave address register definitions */
#define IC_SLAVE_ADDR		0x0002

/* i2c data buffer and command register definitions */
#define IC_CMD			0x0100
#define IC_STOP			0x0200

/* i2c interrupt status register definitions */
#define IC_GEN_CALL		0x0800
#define IC_START_DET		0x0400
#define IC_STOP_DET		0x0200
#define IC_ACTIVITY		0x0100
#define IC_RX_DONE		0x0080
#define IC_TX_ABRT		0x0040
#define IC_RD_REQ		0x0020
#define IC_TX_EMPTY		0x0010
#define IC_TX_OVER		0x0008
#define IC_RX_FULL		0x0004
#define IC_RX_OVER 		0x0002
#define IC_RX_UNDER		0x0001

/* fifo threshold register definitions */
#define IC_TL0			0x00
#define IC_TL1			0x01
#define IC_TL2			0x02
#define IC_TL3			0x03
#define IC_TL4			0x04
#define IC_TL5			0x05
#define IC_TL6			0x06
#define IC_TL7			0x07
#define IC_RX_TL		IC_TL0
#define IC_TX_TL		IC_TL0

/* i2c enable register definitions */
#define IC_ENABLE_0B		0x0001

/* i2c status register  definitions */
#define IC_STATUS_SA		0x0040
#define IC_STATUS_MA		0x0020
#define IC_STATUS_RFF		0x0010
#define IC_STATUS_RFNE		0x0008
#define IC_STATUS_TFE		0x0004
#define IC_STATUS_TFNF		0x0002
#define IC_STATUS_ACT		0x0001

/* Speed Selection */
#define IC_SPEED_MODE_STANDARD	1
#define IC_SPEED_MODE_FAST	2
#define IC_SPEED_MODE_MAX	3

#define I2C_MAX_SPEED		3400000
#define I2C_FAST_SPEED		400000
#define I2C_STANDARD_SPEED	100000

#endif /* __DW_I2C_H_ */
