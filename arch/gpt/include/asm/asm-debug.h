
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
