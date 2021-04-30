#ifndef __GPT_APIU_H__
#define __GPT_APIU_H__

/*operations for peripherals
*/
#define APIU_RQST_FIFO_WRITE 	0
#define APIU_RQST_FIFO_READ 	1

#define APB_CTRL_REG_GPIO2	0x80000000
#define APB_CTRL_REG_GPIO1	0x40000000
#define APB_CTRL_REG_UART2	0x20000000
#define APB_CTRL_REG_UART1	0x10000000

enum EXT_BUS_CLK_DIV{
	EXT_BUS_CLK_DIV4	=0x0,
	EXT_BUS_CLK_DIV8 	=0x1,
	EXT_BUS_CLK_DIV16	=0x2,
	EXT_BUS_CLK_DIV32	=0x3,
	EXT_BUS_CLK_DIV64	=0x4
};

enum APB_BUS_CLK_DIV{
	APB_BUS_CLK_DIV4	=0x0,
	APB_BUS_CLK_DIV8	=0x1,
	APB_BUS_CLK_DIV16	=0x2,
	APB_BUS_CLK_DIV32	=0x3,
	APB_BUS_CLK_DIV64	=0x4
};

enum APIU_DEV{
	APIU_UART1	=0xf0008000,
	APIU_UART2	=0xf0009000,
	APIU_GPIO1	=0xf000a000,
	APIU_GPIO2	=0xf000b000,
	APIU_SYS1 	=0xf000e000,
	APIU_SYS2 	=0xf000e004,
	APIU_CTRL 	=0xf000f000
};


struct apiu_ctrl_reg{
	uint32_t rqst_fifo_empty		:1;
	uint32_t rqst_fifo_aobmid		:1;
	uint32_t rqst_fifo_full			:1;
	uint32_t reserv_0			:1;
	uint32_t resp_fifo_not_empty		:1;
	uint32_t resp_fifo_aoamid		:1;
	uint32_t resp_fifo_full			:1;
	uint32_t reserv_1			:1;
	uint32_t uart1_burst_tx_empty 		:1;
	uint32_t uart1_burst_tx_aob1q 		:1;
	uint32_t uart1_burst_tx_full 		:1;
	uint32_t reserv_2		 	:1;
	uint32_t uart1_burst_rx_not_empty	:1;
	uint32_t uart1_burst_rx_aoa3q		:1;
	uint32_t uart1_burst_rx_full		:1;
	uint32_t uart1_peripheral_int		:1;
	uint32_t uart2_burst_tx_empty		:1;
	uint32_t uart2_burst_tx_aob1q		:1;
	uint32_t uart2_burst_tx_full		:1;
	uint32_t reserv_3			:1;
	uint32_t uart2_burst_rx_not_empty       :1;
	uint32_t uart2_burst_rx_aoa3q           :1;
	uint32_t uart2_burst_rx_full            :1;
	uint32_t uart2_peripheral_int           :1;
	uint32_t gpio1_peripheral_int           :1;
	uint32_t gpio2_peripheral_int           :1;
	uint32_t reserv_4			:2;
	uint32_t uart1_periph_cfg_int           :1;
	uint32_t uart2_periph_cfg_int           :1;
	uint32_t gpio1_periph_cfg_int		:1;
	uint32_t gpio2_periph_cfg_int		:1;
};

struct apiu_sys_reg1{
	uint32_t ext_bus_clk	:3;
	uint32_t reserv_0	:1;
	uint32_t apb_clk	:3;
	uint32_t reserv_1	:25;
};

void gpt_apiu_enable(uint32_t dev_en);
void gpt_apiu_uart_init(uint32_t uart_en, uint32_t div);
uint32_t gpt_apiu_read(enum APIU_DEV dev,void* reg);
void gpt_apiu_write(enum APIU_DEV dev, void* reg, uint32_t val);

#endif
