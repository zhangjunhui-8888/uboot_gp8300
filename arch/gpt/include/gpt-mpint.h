#ifndef GPT_MPINT_H
#define GPT_MPINT_H

#define GPT_MPIC_BASE			0xf0007000UL
#define GPT_MPIC_ISR_OFF		0x100
#define GPT_MPIC_IEN_OFF		0x200
#define GPT_MPIC_ITYPE_OFF		0x220
#define GPT_MPIC_VADDR_OFF		0x240
#define GPT_MPIC_RIRQ_L_OFF		0x290
#define GPT_MPIC_RIRQ_H_OFF		0x294

#define GPT_MPIC_VADDR_ISRC(irq)	(GPT_MPIC_BASE + GPT_MPIC_ISR_OFF + ((irq) << 2))
#define GPT_MPIC_IEN_L(cpu)		(GPT_MPIC_BASE + GPT_MPIC_IEN_OFF + (cpu << 2))
#define GPT_MPIC_IEN_H(cpu)		(GPT_MPIC_BASE + GPT_MPIC_IEN_OFF + 4 + (cpu << 2))
#define GPT_MPIC_ITYPE_L(cpu)		(GPT_MPIC_BASE + GPT_MPIC_ITYPE_OFF + (cpu << 2))
#define GPT_MPIC_ITYPE_H(cpu)		(GPT_MPIC_BASE + GPT_MPIC_ITYPE_OFF + 4 + (cpu << 2))
#define GPT_MPIC_VADDR(cpu)		(GPT_MPIC_BASE + GPT_MPIC_VADDR_OFF + 4 * (cpu))

#define GPT_MPIC_RAW_IRQ_L		(GPT_MPIC_BASE + GPT_MPIC_RIRQ_L_OFF)
#define GPT_MPIC_RAW_IRQ_H		(GPT_MPIC_BASE + GPT_MPIC_RIRQ_H_OFF)


#ifdef CONFIG_TEST_ON_SOC
#define I2S1_HW_IRQN_RX			20
#define I2S1_HW_IRQN_TX			21

#define SPI1_HW_IRQN_RX			22
#define SPI2_HW_IRQN_RX			23
#define SPI3_HW_IRQN_RX			24
#define SPI1_HW_IRQN_TX			25
#define SPI2_HW_IRQN_TX			26
#define SPI3_HW_IRQN_TX			27
#define SPI1_HW_IRQN_IP			28
#define SPI2_HW_IRQN_IP			29
#define SPI3_HW_IRQN_IP			30
#define WDT_HW_IRQN				31
#else
#define I2S1_HW_IRQN_RX			20
#define I2S2_HW_IRQN_RX			21
#define I2S1_HW_IRQN_TX			22
#define I2S2_HW_IRQN_TX			23

#define SPI1_HW_IRQN_RX			24
#define SPI2_HW_IRQN_RX			25
#define SPI1_HW_IRQN_TX			26
#define SPI2_HW_IRQN_TX			27
#define SPI1_HW_IRQN_IP			28
#define SPI2_HW_IRQN_IP			29
/* 30 31 reserved*/
#endif

#define TIMER1_HW_IRQN_IP		32
#define TIMER2_HW_IRQN_IP		33
#define TIMER3_HW_IRQN_IP		34
#define TIMER4_HW_IRQN_IP		35

#define I2C1_HW_IRQN_RX			36
#define I2C2_HW_IRQN_RX			37
#define I2C3_HW_IRQN_RX			38
#define I2C4_HW_IRQN_RX			39

#define I2C1_HW_IRQN_TX			40
#define I2C2_HW_IRQN_TX			41
#define I2C3_HW_IRQN_TX			42
#define I2C4_HW_IRQN_TX			43

#define I2C1_HW_IRQN_IP			44
#define I2C2_HW_IRQN_IP			45
#define I2C3_HW_IRQN_IP			46
#define I2C4_HW_IRQN_IP			47

void gpt_mpic_clr_dis(int cpu, uint32_t irq);
void gpt_mpic_en(int cpu, uint32_t irq);
int gpt_mpic_irq_register(int irqno, int (*func)(void *data), void *data);
#endif
