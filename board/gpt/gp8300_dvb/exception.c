#include <common.h>
#include <config.h>
#include <linux/linkage.h>
#include <machine/gpt_kernel.h>
#include <asm/asm-debug.h>

#include "exception.h"

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
	volatile int *p = (volatile int *)0xf0017004;

	/* not support debug */
	if (h->magic != 0x1) {
		/* set MXB at the second page */
		//sprset_gpt(MXB, CONFIG_SYS_DDR_SDRAM_START + PAGE_SIZE);
		puts("Debug-monitor: disabled\n");
		return;
	}

	*p = 0x3fffffff;
	/* debug data area: 0x800 -> 0x8000*/
//	memset((void *)(DEBUG_DATA_TOP - DEBUG_DATA_SIZE + 0x20), 0, DEBUG_DATA_SIZE);
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
