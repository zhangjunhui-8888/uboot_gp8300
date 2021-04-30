#include <common.h>
#include <linux/ctype.h>
#include <command.h>
#include <test/minitest.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/polaris/apiu.h>
#include <machine/gpt_io_addrs.h>
#include <asm/spr.h>

/* GP Timer control */
#define TIMER_PWM_EN	BIT(3)
#define TIMER_TOG_EN	BIT(2)
#define TIMER_CONT		BIT(1)
#define TIMER_START		BIT(0)
#define TIMER_INTCLR	BIT(0)
#define TIMER_INTEN		0
#define ONESHOT_MODE	0
#define CONTINUE_MODE	1
/* WD Timer control */
#define WDT_START		BIT(0)
#define WDT_STOP		!(IT(0))
#define CPU0_WDT_ACTIVE		BIT(4)
#define WDT_FEED		0x1ACCE770

struct gpt_timer_regs {
	u32	clkdiv;			/* Timer clock devider */
	u32	timer_val;		/* Timer value  */
	u32	current_val;	/* Timer current value */
	u32	pwm_val;		/* Timer PWM value */
	u32	control;		/* Timer Control register */
	u32	int_rawst;		/* Interrupt raw status */
	u32	int_mask;		/* Interrupt mask for */
	u32	int_status;		/* Interrupt status */
	u32	int_clr;		/* Interrupt clear */
	u32	pulse_mode;		/* pulse_mode control register */
	u32	reload_val;		/* reload value for the timer */
};

struct gpt_wdt_regs {
	u32	clkdiv;			/* clock divider of the wdt */
	u32	ini_val;		/* initial value which counter starts to count_down */
	u32	cur_val;	/* current counter value of the counter */
	u32	wdt_rcc;		/* re-count control of the WDT */
	u32	wdt_ctrl;		/* control register */
	u32 resa;
	u32 resb;
	u32	intr_st;		/* status of interrupt */
};

struct timer_config {
	/* GP Timer config */
	u8 id;
	u32 clkdiv;
	u32 clkout;
	u32 mode;
	u32 timer_val;
	u32 pwm_val;
	u32 reload_val;
	/* WD Timer config */
	u32 ini_val;
	u32 feed;
	u32 wdt_disable;
};
struct gpt_timer_regs *regs;
struct gpt_wdt_regs *wdt_regs;

static void timer_regdump(void)
{
	printf("\t============Timer regs dump=============\n\
	timer_val;      :  %08X,\n\
	current_val1;   :  %08X,\n\
	pwm_val;        :  %08X,\n\
	contorl;        :  %08X,\n\
	int_rawst;      :  %08X,\n\
	int_mask;      :  %08X,\n\
	int_sta;      :  %08X,\n\
	reload_val;     :  %08X,\n\
	=======================================\n",
			regs->timer_val,
			regs->current_val,
			regs->pwm_val,
			regs->control,
			regs->int_rawst,
			regs->int_mask,
			regs->int_status,
			regs->reload_val);
}

static void wdt_regdump(void)
{
	printf("\t============WDT regs dump=============\n\
	clkdiv;      : %08X,\n\
	ini_val;     : %08X,\n\
	cur_val;     : %08X,\n\
	wdt_rcc;     : %08X,\n\
	wdt_ctrl;    : %08X,\n\
	intr_st;     : %08X,\n\
	=======================================\n",
			wdt_regs->clkdiv,
			wdt_regs->ini_val,
			wdt_regs->cur_val,
			wdt_regs->wdt_rcc,
			wdt_regs->wdt_ctrl,
			wdt_regs->intr_st);
		printf("PSC =%016lX\n", sprget_gpt(PSC));
		printf("PSCH=%016lX\n", sprget_gpt(PSCH));
		printf("PSCM=%016lX\n", sprget_gpt(PSCM));
		printf("CPUC=%016lX\n", sprget_gpt(CPUC));
		printf("XEN =%016lX\n", sprget_gpt(XEN));
		printf("XPEN=%016lX\n", sprget_gpt(XPEN));
		printf("XLV =%016lX\n", sprget_gpt(XLV));
		printf("XMS =%016lX\n", sprget_gpt(XMS));
		printf("XHS =%016lX\n", sprget_gpt(XHS));
		printf("TIMC=%016lX\n", sprget_gpt(TIMC));
		printf("TCD0C=%016lX\n", sprget_gpt(TCD0C));
}

static int gpt_timer_get_count(int count)
{
	u32 val, st, rw;

	while(count--){
		val = readl(&regs->current_val);
		st = readl(&regs->int_status);
		rw = readl(&regs->int_rawst);
		printf("timer current value: %x int_status: %x int_raw: %x \n", val, st, rw);
		if(st)
			writel(0x1, &regs->int_clr);
	}
	return val;
}
void wdt_interrupt_63(void)
{
	printf("\n=========================\n");	
	printf("The WDT 63 interrupt occurs!\n");	
	printf("=========================\n");	

	wdt_regdump();
	writel(WDT_FEED, &wdt_regs->wdt_rcc);
	printf("63 The feeding WDT DONE!\n");	
	wdt_regdump();
	while(!(readl(&wdt_regs->intr_st))){
		printf("current value: %x intr_status: %x \n", readl(&wdt_regs->cur_val), readl(&wdt_regs->intr_st));
		udelay(10 * 1000);
	}
}
void wdt_interrupt(void)
{
	static int index = 0;
	printf("\n=========================\n");	
	printf("The WDT interrupt occurs!\n");	
	printf("=========================\n");	

	wdt_regdump();
	if(index <=3)
	writel(WDT_FEED, &wdt_regs->wdt_rcc);
	printf("The feeding WDT DONE! %d\n", ++index);	
	wdt_regdump();
	while(!(readl(&wdt_regs->intr_st))){
		printf("current value: %x intr_status: %x \n", readl(&wdt_regs->cur_val), readl(&wdt_regs->intr_st));
		udelay(10 * 1000);
	}
	writel(0, &wdt_regs->wdt_ctrl);
}


static int gpt_wdt_test(struct timer_config *config)
{		
	u32 st, ini;
	
	wdt_regdump();
	
	if(config->wdt_disable == 1) {
		writel(~WDT_FEED, &wdt_regs->wdt_rcc);
		writel(0, &wdt_regs->wdt_ctrl);
		
		return 0;
	}

	writel(config->clkdiv, &wdt_regs->clkdiv);
	writel(config->ini_val, &wdt_regs->ini_val);
	writel(WDT_START | CPU0_WDT_ACTIVE, &wdt_regs->wdt_ctrl);
#if 1
	
	//val = readl(&wdt_regs->cur_val);
	st = readl(&wdt_regs->intr_st);
	ini = readl(&wdt_regs->ini_val);
	while(!st){
		st = readl(&wdt_regs->intr_st);
		if(st){
			if(config->feed == 1) {
				writel(WDT_FEED, &wdt_regs->wdt_rcc);
				printf("feed\n");
			}
			printf("The first feeding WDT DONE!\n");	
			while(!(readl(&wdt_regs->intr_st))){
				printf("current value: %x intr_status: %x \n", readl(&wdt_regs->cur_val), readl(&wdt_regs->intr_st));	
			}
			printf("The second feeding WDT DONE!\n");	
		}
		
		printf("wdt current value: %p-%x init value: %x intr_status: %x \n",
				&wdt_regs->cur_val, readl(&wdt_regs->cur_val), ini, st);

		if(config->feed == 1) {
			writel(WDT_FEED, &wdt_regs->wdt_rcc);
			printf("feed\n");
		}
		udelay(10 * 1000);
	}
#endif

	return 0;
}

static int gpt_timer_init(struct timer_config *config)
{
	u32 value;
	regs = (struct gpt_timer_regs *)(u64)(GPTXSOC_MMIO_BASE | APIU_GPTIMER1 | (config->id << 8));

	writel(TIMER_INTCLR, &regs->int_clr);
	writel(config->clkdiv, &regs->clkdiv);
	writel(config->timer_val, &regs->timer_val);
	writel(TIMER_INTEN, &regs->int_mask); //1:disable 0:enalbe

	value = TIMER_START;

	if(config->mode == CONTINUE_MODE){
		if(config->reload_val){
			writel(config->reload_val, &regs->reload_val);
			writel(0x1, &regs->pulse_mode); //1.auto 0.occurs on write
		}
		value |= TIMER_CONT;
	}

	if(config->clkout){
		if(config->pwm_val){
			value |= TIMER_PWM_EN;
			writel(config->pwm_val, &regs->pwm_val);
		}
		value |= TIMER_TOG_EN;
	}

	writel(value, &regs->control);

	return 0;
}

static void gpt_timer_uninit(void)
{
	writel(0, &regs->control);
}
#ifdef CONFIG_SYS_LONGHELP
static char timer_help_text[] =
"GPT TIMER Controller Test\n"
"<id>\n"
"\t<0,3>:GP Timer <4>:WD Timer\n"
"<mode>\n"
"\t1.continue mode 0.oneshot mode\n"
"<clkdiv>\n"
"\tfin/(1+clkdiv)\n"
"<clkout>\n"
"\t1.enable 0.disable\n"
"<tv>\n"
"\ttarget value of timer\n"
"<rv>\n"
"\treload counter value of timer\n"
"<pv>\n"
"\tPWM value\n"
"<iv>\n"
"\tinitial value of wdt\n"
"<feed>\n"
"\tt\n"
"exmple: mode=1 clkdiv=0 clkout=0 tv=ffff rv=0 pv=0\n"
"\n";
#endif


static int get_user_argument(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[], void *data)
{
	int i;
	char *delim = "=,:";
	char *token;
	char buf[1024];
	unsigned int tmp;
	struct timer_config *config = (struct timer_config *)data;

#ifdef CONFIG_SYS_LONGHELP
	if(!strcmp((argv[1]), "help")) {
		puts(timer_help_text);
		return -1;
	}
#endif

	for (i = 1; i < argc; i++) {
		strcpy(buf, argv[i]);
		token = strtok(buf, delim);
		if (!strcmp(token, "id")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (tmp > 5)
				printf("id[0,5] %dbad!!!\n", tmp);
			else
				config->id = tmp;
			continue;
		}
		if (!strcmp(token, "mode")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (tmp > 1)
				printf("mode[0,1] %dbad!!!\n", tmp);
			else
				config->mode = tmp;
			continue;
		}
		if (!strcmp(token, "clkdiv")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (tmp < 0)
				printf("clkdiv[0,1] %dbad!!!\n", tmp);
			else
				config->clkdiv = tmp;
			printf("clkdiv: %d \n", config->clkdiv);
			continue;
		}
		if (!strcmp(token, "clkout")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (tmp > 1)
				printf("clkout[0,1] %dbad!!!\n", tmp);
			else
				config->clkout = tmp;
			continue;
		}
		if (!strcmp(token, "tv")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (!tmp)
				printf("timer_val [0,0xffffffff] bad\n");
			else
				config->timer_val = tmp;
			continue;
		}
		if (!strcmp(token, "rv")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (!tmp)
				printf("reload_val [0,0xffffffff] bad\n");
			else
				config->reload_val = tmp;
			continue;
		}
		if (!strcmp(token, "pv")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (!tmp)
				printf("pwm_val [0,0xffffffff] bad\n");
			else
				config->pwm_val = tmp;
			continue;
		}
		if (!strcmp(token, "iv")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 16);
			if (!tmp)
				printf("ini_val [0,0xffffffff] bad\n");
			else
				config->ini_val = tmp;
			continue;
		}
		if (!strcmp(token, "feed")) {
			token = strtok(NULL, delim);
			tmp = simple_strtoul(token, NULL, 10);
			if (tmp > 64)
				printf("feed[0,1] bad\n");
			else
				config->feed = tmp;
			continue;
		}

		if (!strcmp(token, "dump")) {
			timer_regdump();
			continue;
		}

		if(!strcmp(token, "disable")) {
			config->wdt_disable = 1;
			continue;
		}

		printf("Bad argument: <%s>\n", token);
		return -1;
	}

	printf("Timer config:\nid[%d],mode[%d],clkdiv[%d],clkout[%d],timer_val[%x],reload[%x],pwm_val[%x]\n\n",
			config->id, config->mode, config->clkdiv, config->clkout, config->timer_val, config->reload_val, config->pwm_val);
	printf("WDT config:\nini_val[%x],feed[%d]\n\n",
			config->ini_val, config->feed);
	return 0;
}

static char watchdog_help_text [] =
	"watchdog id=4 clkdiv=10000 iv=1000 \n"
	"watchdog id=4 clkdiv=7f iv=fffffff \n"
	"watchdog id=4 disable \n";

int do_timer_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct timer_config config;

	if (argc <=1){
		printf("Arg error!!!\n \n%s", watchdog_help_text);
		//timer_regdump();
		//wdt_regdump();
		
		return 0;
	}

	config.id = 0;
	config.clkdiv = 0;
	config.clkout = 0;
	config.pwm_val = 0;
	config.timer_val = 0xffff;
	config.reload_val = 0;
	config.mode = ONESHOT_MODE;
	config.ini_val = 0xffffff;
	config.feed = 0;

	get_user_argument(cmdtp, flag, argc, argv, &config);
	if(config.id < 4){
		gpt_timer_init(&config);
		gpt_timer_get_count(40);
		timer_regdump();
		gpt_timer_uninit();
	}else if(config.id == 4){
		wdt_regs = (struct gpt_wdt_regs *)(GPTXSOC_MMIO_BASE | APIU_WDTIMER);
		gpt_wdt_test(&config);
	}else {
		printf("PSC =%016lX\n", sprget_gpt(PSC));
		printf("CPUC=%016lX\n", sprget_gpt(CPUC));
		printf("XEN =%016lX\n", sprget_gpt(XEN));
		printf("XPEN=%016lX\n", sprget_gpt(XPEN));
		printf("XLV=%016lX\n", sprget_gpt(XLV));
		printf("XMS =%016lX\n", sprget_gpt(XMS));
		printf("XHS =%016lX\n", sprget_gpt(XHS));
		sprset_gpt(XEN, sprget_gpt(XEN) | (1UL << config.feed));
		sprset_gpt(XPEN, sprget_gpt(XPEN) & ~(1UL << config.feed));
		udelay(10*1000);
		sprset_gpt(XEN, sprget_gpt(XEN) & ~(1UL << config.feed));
		sprset_gpt(XPEN, sprget_gpt(XPEN) | (1UL << config.feed));
		sprset_gpt(PSC, sprget_gpt(PSC) & ~(1UL << 3));
		printf("PSC =%016lX\n", sprget_gpt(PSC));
		printf("CPUC=%016lX\n", sprget_gpt(CPUC));
		printf("XEN =%016lX\n", sprget_gpt(XEN));
		printf("XPEN=%016lX\n", sprget_gpt(XPEN));
		printf("XLV=%016lX\n", sprget_gpt(XLV));
		printf("XMS =%016lX\n", sprget_gpt(XMS));
		printf("XHS =%016lX\n", sprget_gpt(XHS));
	}
	return 0;
}

U_BOOT_MINITEST_CMD(timer_test, CONFIG_SYS_MAXARGS, 0, do_timer_test,
		"APIU GP/WD timer test",
		"APIU GP/WD timer test"
		);

U_BOOT_CMD(
	watchdog, 6, 1, do_timer_test,
	"APIU GP/WD timer test",
	watchdog_help_text
);
