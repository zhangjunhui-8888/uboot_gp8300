#include <common.h>
#include <config.h>
#include <linux/linkage.h>
#include <machine/gpt_kernel.h>

#include "exception.h"

#define	MC_DBG_STACK	"$s15"
#define	MCS_DBG_FLAGS	"$s14"

#define PAGE_SHIFT      15
#define PAGE_SIZE       (1 << PAGE_SHIFT)
/*
 memory layout for debug:

                   |----------------|  -0
                   |   smp lunch    |
                   |----------------|  -2k
                   |                |
                   |30k for dbg data|
                   |                |
                   |----------------|  -32k
                   |                |
                   |                |
                   | 32k for MXB ex |
                   |                |
                   |                |
                   |----------------|  -64k(2 pages)
                   |       |        |
                   |       |        |
                   |       |        |
                   |       |        |
                   |       v        |
                   |  high memory   |
                   |                |
* */

#define	SMP_LUNCH_SIZE		0x800 /* 2k */
#define	DEBUG_DATA_SIZE		(PAGE_SIZE - SMP_LUNCH_SIZE) /* 32k - 2k */
#define DEBUG_DATA_TOP          (CONFIG_SYS_DDR_SDRAM_START + PAGE_SIZE)

#define IX_WDT_INT              0
#define IX_MC_DBG_RQ_IRQ        (_ONE << IX_WDT_INT)
#define IX_MONITOR_EXCEPTIONS   ((_ONE << IX_DBG_STEP) | (_ONE << IX_DBG_BRANCH) |\
		                        (_ONE << IX_DBG_INSN) | (_ONE << IX_DBG_DATA) | IX_MC_DBG_RQ_IRQ)
#define IX_MC_DBG_XLV           (IX_MONITOR_EXCEPTIONS)

#define PSCx_MC_DBG             ((_ONE << PSC_OFFSE_debug_dm ) | (_ONE << PSC_OFFSE_debug_im) | ((_ONE << PSC_OFFSE_interrupt_mc)))

#define MC_RESV_FUNCLIST_SIZE   0x30

#define MC_RESV_FUNCLIST_ADDR   (DEBUG_DATA_TOP - MC_RESV_FUNCLIST_SIZE)
#define	MC_DBG_STACK_TOP	(MC_RESV_FUNCLIST_ADDR - 8)

#define TRAP_NUMS               64

#define TRAP_MASK               ((_ONE << TRAPM_OFFSE_interrupt_0) | (_ONE << TRAPM_OFFSE_interrupt_x) |\
											(TRAP_NUMS - 1))
struct debug_header {
	uint64_t magic;
	uint64_t mc_base;
	uint64_t exec_addr_start;
	uint64_t exec_addr_end;
	/* debug handlers */
	uint64_t dbg_notifier;
	uint64_t dbg_step;
	uint64_t dbg_branch;
	uint64_t dbg_insn;
	uint64_t watch_data;
};

void gpt_debug_init(void)
{
	uint64_t spr;
	/* nor location: after 1M */
	struct debug_header *h = (void *)CONFIG_SYS_DEBUG_BASE;
	/* not support debug */
	if (h->magic != 0x1) {
		/* set MXB at the second page */
		sprset_gpt(MXB, CONFIG_SYS_DDR_SDRAM_START + PAGE_SIZE);
		return;
	}

	/* debug data area: 0x800 -> 0x8000*/
	memset((void *)(DEBUG_DATA_TOP - DEBUG_DATA_SIZE), 0, DEBUG_DATA_SIZE);

	memcpy((void *)h->exec_addr_start, (void *)(CONFIG_SYS_DEBUG_BASE + sizeof(struct debug_header)), h->exec_addr_end - h->exec_addr_start);

	sprset_gpt(MXB, h->mc_base);
	sprset_gpt(TXB, h->mc_base);

	sprset_gpt(TRAPM, TRAP_MASK);

	asm volatile("ssetr "MC_DBG_STACK", %0\n\t"::"r"(MC_DBG_STACK_TOP):);

	spr = sprget_gpt(XLV);
	spr = spr & (~IX_MC_DBG_XLV);
	sprset_gpt(XLV, spr);

	spr = sprget_gpt(XMS);
	spr |= IX_MC_DBG_XLV;
	sprset_gpt(XMS, spr);

	spr = sprget_gpt(PSC);
	sprset_gpt(PSC, spr | PSCx_MC_DBG);

	spr = sprget_gpt(PSCH);
	sprset_gpt(PSCH, spr | PSCx_MC_DBG);

	spr = sprget_gpt(PSCM);
	sprset_gpt(PSCM, spr | PSCx_MC_DBG);

	spr = sprget_gpt(XEN);
	sprset_gpt(XEN, spr | IX_MONITOR_EXCEPTIONS);

	memcpy((void *)MC_RESV_FUNCLIST_ADDR, (void *)(&h->dbg_notifier), 5 * sizeof(uint64_t));
	puts("Debugger monitor ready\n");
}
