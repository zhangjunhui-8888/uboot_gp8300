#include <linux/mtd/gpt_norflash.h>

//#define GPT_NOR_DEBUG

#if 0
unsigned char data_w[0x100000];
unsigned char data_r[0x100000];
#else
unsigned char *data_w = (void*)0x210000000;
unsigned char *data_r = (void*)0x220000000;
#endif

struct norflash_dev *curdev = NULL;
struct gpt_norflash_reg *norflash_regs;

static int pn25f_quad_enable(void);
static int pn25f_quad_disable(void);
static int w25q32_quad_enable(void);
static int gd25_norflash_setst1(int flags);
static int w25q32_quad_disable(void);
static int mx25l32_quad_enable(void);
static int mx25l32_quad_disable(void);
static int at25sf_quad_enable(void);
static int at25sf_quad_disable(void);

struct norflash_dev nordev[10] = {
	{
		0x4126bf,
		sst26_quad_enable,
		s25fs_norflash_dev_rst,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		w25q80_norflash_quad_read,
		NULL,
		s25fs_nor_flash_sfo_erase,
	},
	{
		0x17701c,
		pn25f_quad_enable,
		pn25f_quad_disable,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		w25q80_norflash_quad_read,
		w25q80_norflash_quad_write,
		s25fs_nor_flash_sfo_erase,
	},
	{
		0x1640ef,
		w25q32_quad_enable,
		w25q32_quad_disable,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		w25q80_norflash_quad_read,
		w25q80_norflash_quad_write,
		s25fs_nor_flash_sfo_erase,
	},
	{
		0x1620c2,
		mx25l32_quad_enable,
		mx25l32_quad_disable,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		w25q80_norflash_quad_read,
		NULL,
		s25fs_nor_flash_sfo_erase,
	},
	{
		0x01841f,
		at25sf_quad_enable,
		at25sf_quad_disable,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		w25q80_norflash_quad_read,
		NULL,
		s25fs_nor_flash_sfo_erase,
	},
	{
		0x1131a1,
		NULL,
		NULL,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		NULL,
		NULL,
		s25fs_nor_flash_sfo_erase,
	},
	{
		0x1840c8,
		w25q32_quad_enable,
		w25q32_quad_disable,
		s25fs_norflash_dev_rst,
		s25fs_norflash_wren,
		s25fs_norflash_unlock,
		s25fs_norflash_sfo_auto_se,
		s25fs_norflash_sfo_read,
		s25fs_norflash_sfo_write,
		s25fs_norflash_read,
		s25fs_norflash_write,
		w25q80_norflash_sfo_dual_read,
		w25q80_norflash_dual_write,
		w25q80_norflash_quad_read,
		w25q80_norflash_quad_write,
		s25fs_nor_flash_sfo_erase,
	},

};

#ifdef GPT_NOR_DEBUG
void gpt_norflash_dumpregs(void)
{
	int i;
	unsigned int *reg_t = (unsigned int *)norflash_regs;
	if (reg_t == NULL) {
		printf("please init norflash host first");
		return;
	}
	printf("noflash regs:\n");
	for (i = 0; i < 24; i++)
		printf("%.8x%c", reg_t[i], i % 4 == 3 ? '\n' : ' ');
}
#else
void gpt_norflash_dumpregs(void)
{
}
#endif

void gpt_norflash_dumpdata(unsigned char *data, int length)
{
	int i;
	for (i = 0; i < length; i++)
		printf("%.2x%c", data[i], i % 16 == 15 ? '\n' : ' ');
	printf("\n");
}

int gpt_norflash_checkdata(unsigned char *src, unsigned char *dst, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		if (src[i] != dst[i]) {
			printf("#######compare error i:%d, %x-->%x\n",
					i, src[i], dst[i]);
			return -1;
		}
	}
	return 0;
}

int gpt_norflash_checkblank(unsigned char *data, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		if (data[i] != 0xff)
			return -1;
	}
	return 0;
}

int gpt_norflash_setconfig(unsigned int sfc_ctl, unsigned int intmask)
{
#ifdef GPT_NOR_DEBUG
	printf("sfc_ctl: %x, intmask: %x\n", sfc_ctl, intmask);
#endif
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	writel(1, &GPT_REGS(sw_rst));
	writel(0, &GPT_REGS(sw_rst));

	writel(sfc_ctl, &GPT_REGS(sfc_ctl));
	writel(intmask, &GPT_REGS(sfc_int));

	return 0;
}

void gpt_norflash_reset(void)
{
	writel(1, &GPT_REGS(sw_rst));
	writel(0, &GPT_REGS(sw_rst));
}

int gpt_norflash_init(void)
{
	norflash_regs = (struct gpt_norflash_reg *)GPT_SPI_NOR_APB_BASE;
	writel(0x10b, &GPT_REGS(read_cmd)); //bugfix
	return 0;
}

int gpt_wait_sfo_ready(unsigned int bits)
{
	unsigned int status;
	unsigned int timeout = 0x100000;
	do {
		status = readl(&GPT_REGS(sfc_int));
	#ifdef GPT_NOR_DEBUG
		printf("status: %x, bits:%x\n", status, bits);
	#endif
		if (status & bits)
			return 1;
	} while ((timeout--) > 0);

	return 0;
}

int sfo_read_transfer(struct gpt_norflash_msg *msg)
{
	int i, ret;
	int count;
	unsigned int reg;
	unsigned int *data = (unsigned int *)msg->buff;
	ret = (msg->instr << 8) | (msg->dumy_len << 20)
		| (msg->data_len << 24) | (msg->addr_len << 16) | (msg->opts);
	writel(ret, &GPT_REGS(sfo_cmd));
	count = (msg->len_buff + 3) / 4;
#ifdef GPT_NOR_DEBUG
	printf("%s:%d sfo_cmd : %x-->%x ,count:%x\n",
		__func__, __LINE__, ret, 0x04009f50, count);
#endif
	for (i = 0; i < count; i++) {
		switch(msg->instr){
			case S25FS_COMMAND_FST_RD:
				writel(msg->addr + i * 4, &GPT_REGS(sfo_addr));
				break;
			case S25FS_COMMAND_READ:
				writel(msg->addr + i * 4, &GPT_REGS(sfo_addr));
				break;
			default:
				break;
		}

		reg = readl(&(GPT_REGS(sfo_cmd)));
		writel(reg | 0x1, &GPT_REGS(sfo_cmd));
		ret = gpt_wait_sfo_ready(GPT_SFO_INT_ST | GPT_SFO_INT_RAW);
		if (!ret) {
			printf("util read timeout\n");
			return -1;
		}
		GPT_REGS(sfc_int)  |= GPT_SFO_INT_CLR;

		data[i] = readl(&GPT_REGS(sfo_rd));
	}

	return 0;
}

int sfo_instr_transfer(struct gpt_norflash_msg *msg)
{
	int ret;
	int reg;

	ret = (msg->instr << 8) | (msg->dumy_len << 20)
		| (msg->data_len << 24) | (msg->addr_len << 16) | (msg->opts);

#ifdef GPT_NOR_DEBUG
	printf("%s:%d sfo_cmd : %x-->%x\n", __func__, __LINE__,
				(msg->instr << 8), ret);
#endif

	writel(ret, &GPT_REGS(sfo_cmd));
	/*writel((msg->instr << 8) | flags, &GPT_REGS(sfo_cmd));*/
	reg = readl(&GPT_REGS(sfo_cmd));
	writel(reg | 0x1, &GPT_REGS(sfo_cmd));

	gpt_norflash_dumpregs();
	ret = gpt_wait_sfo_ready(GPT_SFO_INT_ST | GPT_SFO_INT_RAW);
	if (!ret) {
		printf("%s:%d: write data timeout\n", __func__, __LINE__);
		return -1;
	}
	return 0;
}

int sfo_write_transfer(struct gpt_norflash_msg *msg)
{
	int i = 0;
	int ret;
	int count;
	unsigned int reg;
	unsigned int *data = (unsigned int *)msg->buff;

	ret = (msg->instr << 8) | (msg->dumy_len << 20) | (msg->data_len << 24) |
              (msg->addr_len << 16) | (msg->opts) | (msg->pollflag);
	writel(ret, &GPT_REGS(sfo_cmd));
	count = (msg->len_buff + 3) / 4;

#ifdef GPT_NOR_DEBUG
	printf("%s:%d sfo_cmd : %x-->%x, count:%x ,data:%x\n", __func__,__LINE__, (msg->instr << 8),0x04009f50,count, data[0]);
#endif
	while (i < count) {
		switch(msg->instr){
			case MT25Q_COMMAND_WR_EXTEND_ADDR:
				writel(data[i], &GPT_REGS(sfo_wd));
				break;
			default:
				writel(data[i], &GPT_REGS(sfo_wd));
				writel(msg->addr + i * 4, &GPT_REGS(sfo_addr));
				break;
		}

		reg = readl(&GPT_REGS(sfo_cmd));
		writel(reg | 0x1, &GPT_REGS(sfo_cmd));
		ret = gpt_wait_sfo_ready(GPT_SFO_INT_ST | GPT_SFO_INT_RAW);
		if (!ret) {
			printf("%s:%d: write data timeout\n",
						__func__, __LINE__);
			return -1;
		}
		i++;
		GPT_REGS(sfc_int)  |= GPT_SFO_INT_CLR;
	}

	return 0;
}

int gpt_norflash_sfo_xfer(struct gpt_norflash_msg *msg)
{
	switch (msg->opts) {
	case GPT_SFO_CMD_TYPE_READ:
		sfo_read_transfer(msg);
		break;

	case GPT_SFO_CMD_TYPE_WRITE: //for write
		sfo_write_transfer(msg);
		break;

	case GPT_SFO_CMD_TYPE_INST: //for only instruct 
		sfo_instr_transfer(msg);
		break;
	default:
		printf("sfo xfer options is invalied\n");
		return -1;
	}
	return 0;
}

void gpt_norflash_hd_wp_test(int flag)
{
	unsigned int ctl_reg = readl(&GPT_REGS(sfc_ctl));

	if (flag)
		writel(ctl_reg | (3 << 18), &GPT_REGS(sfc_ctl));
	else
		writel(ctl_reg & (~(3 << 18)), &GPT_REGS(sfc_ctl));
}

int w25q80_norflash_getst1(void)
{
	int ret;
	unsigned int status;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.instr = W25Q80_COMMAND_RSR2;
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr_len = 0;
	msg.data_len = 1;
	msg.dumy_len = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	return status;

}

int s25fs_norflash_getst(void)
{
	int ret;
	unsigned int status;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.instr = S25FS_COMMAND_RDSR1;
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;

	msg.addr_len = 0;
	msg.data_len = 1;
	msg.dumy_len = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}

	return status;
}

int s25fs_norflash_getid(void)
{
	int ret;
	unsigned int jedecid;
	struct gpt_norflash_msg  msg;

	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.instr = S25FS_COMMAND_RDID;
	msg.buff = (unsigned char *)&jedecid;
	msg.len_buff = 4;
	msg.addr_len = 0;
	msg.data_len = 3;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}

	return jedecid;
}

int gpt_norflash_getid(void)
{
	return s25fs_norflash_getid();
}

int s25fs_norflash_4byte_en(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_4BAM;
	msg.addr_len = 0;
	msg.data_len = 4;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	printf("4 byte address mode , state: %x\n",
			s25fs_norflash_sfo_rdar(0x00000003));
	return 0;
}

int s25fs_norflash_rst_en(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_RSTEN;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	return 0;
}


int s25fs_norflash_rst(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_RST;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	return 0;
}

int s25fs_norflash_dev_rst(void)
{
	s25fs_norflash_rst_en();
	s25fs_norflash_rst();
	return 0;
}

int s25fs_norflash_unlock(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_UNLOCK;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
#ifdef GPT_NOR_DEBUG
	printf("command for write enable, state: %x\n", s25fs_norflash_getst());
#endif
	return 0;
}

int s25fs_norflash_wren(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_WREN;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
#ifdef GPT_NOR_DEBUG
	printf("command for write enable, state: %x\n", s25fs_norflash_getst());
#endif
	return 0;
}

int mt25q_norflash_enter_4byte(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = MT25Q_COMMAND_ENTER_4BYTE;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
#ifdef GPT_NOR_DEBUG
	printf("4 byte address mode , state: %x\n",
			s25fs_norflash_sfo_rdar(0x00000003));
#endif

	return 0;
}

int mt25q_norflash_exit_4byte(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = MT25Q_COMMAND_EXIT_4BYTE;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
#ifdef GPT_NOR_DEBUG
	printf("4 byte address mode , state: %x\n",
			s25fs_norflash_sfo_rdar(0x00000003));
#endif

	return 0;
}


int mt25q_norflash_read_extend_addr(void)
{
	int ret;
	unsigned int status = 0xaa;
	struct gpt_norflash_msg  msg;

	msg.opts = GPT_SFO_CMD_TYPE_READ;
#ifdef	NORFLASH_MT25QU01GBBB_128MB 
	msg.instr = MT25Q_COMMAND_RD_EXTEND_ADDR;
#endif
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr_len = 0;
	msg.data_len = 1;
	msg.dumy_len = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	printf("read extend address register: 0x%x\n", status);
	return status;
}


int mt25q_norflash_write_extend_addr(unsigned int addr)
{
	int ret;
	struct gpt_norflash_msg  msg;
	unsigned int status = (addr >> 24);
	//unsigned int status = addr;

	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
#ifdef	NORFLASH_MT25QU01GBBB_128MB 
	msg.instr = MT25Q_COMMAND_WR_EXTEND_ADDR;
#endif
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr_len = 0;
	msg.data_len = 1;
	msg.dumy_len = 0;
	msg.pollflag = (GPT_SFO_POLLING_EN |
		 0 << GPT_SFO_POLLING_BSEL) & (~GPT_SFO_POLLING_BPOL);

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}


int s25fs_norflash_st_wren(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = AT25SF_COMMAND_ST_WREN;
	msg.addr_len = 0;
	msg.data_len = 4;
	msg.dumy_len = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
#ifdef GPT_NOR_DEBUG
	printf("command for write enable, state: %x\n", s25fs_norflash_getst());
#endif
	return 0;
}

int s25fs_norflash_read(unsigned char *data, int from, int length)
{
	memcpy(data, (void *)GPT_SPI_NOR_AXI_BASE + from, length);
#ifdef GPT_NOR_DEBUG
	printf("read norflash with axi bus, address %x\n", from);
	gpt_norflash_dumpdata(data, length);
#endif
	return 0;
}

int s25fs_norflash_write32(unsigned char *data, int from, int length)
{
#ifdef GPT_NOR_DEBUG
	printf("axi write addr:%x , length:%d\n", from, length);
#endif
	writel(0x10b, &GPT_REGS(read_cmd));
	writel(length - 1, &GPT_REGS(pp_len));
	writel(0x8149, &GPT_REGS(pp_pol_init));

	gpt_norflash_dumpregs();
	s25fs_norflash_wren();
	memcpy((void *)GPT_SPI_NOR_AXI_BASE + from, data, length);
	if (!gpt_wait_sfo_ready(GPT_PP_POL_ADDR_ST | GPT_PP_POL_DONE_RAW)) {
		printf("axi write timeout\n");
		return -1;
	}
	writel(GPT_PP_POL_DONE_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int s25fs_norflash_write(unsigned char *data, int from, int length)
{
	#define GPT_PP_DATA_LEN	64 
	int i;
	int count;
	int offset;
	int cur_len = GPT_PP_DATA_LEN;
	count = length / GPT_PP_DATA_LEN;
	offset = length % GPT_PP_DATA_LEN;

	if(offset)
		count++;	

	for (i = 0; i < count; i++) {
		if (offset && (i == count - 1))
			cur_len = offset;
		s25fs_norflash_write32(data + i * GPT_PP_DATA_LEN,
			from + i * GPT_PP_DATA_LEN, cur_len);
	}
	return 0;
}

int s25fs_norflash_sfo_pp(unsigned int addr)
{
	int ret;
	unsigned char buff_in[10];
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = buff_in;
	msg.len_buff = 4;
	msg.addr = addr;
	msg.instr =  S25FS_COMMAND_PP;
	msg.pollflag = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	return 0;
}

int s25fs_norflash_sfo_wrar(int index, int data)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.instr = S25FS_COMMAND_WRAR;
	msg.buff = (unsigned char *)&data;
	msg.len_buff = 4;
	msg.addr = index;

	msg.addr_len = 3;
	msg.data_len = 4;
	msg.dumy_len = 0;
	msg.pollflag = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	return 0;
}

int s25fs_norflash_sfo_4rdar(int index)
{
	int ret;
	unsigned int status = 0;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.instr = S25FS_COMMAND_RDAR;
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr = index;

	msg.addr_len = 4;
	msg.data_len = 4;
	msg.dumy_len = 1;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	printf("command for read configure register , state: %x\n", status);
	return status;
}

int s25fs_norflash_sfo_rdar(int index)
{
	int ret;
	unsigned int status = 0;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_READ;
#ifdef	NORFLASH_MT25QU01GBBB_128MB 
	msg.instr = MT25Q_COMMAND_NONVOLATILE_CONF;
#else
	msg.instr = S25FS_COMMAND_RDAR;
#endif
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr = index;

	msg.addr_len = 3;
	msg.data_len = 4;
	msg.dumy_len = 1;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	printf("command for read configure register , state: %x\n", status);
	return status;
}

int s25fs_norflash_sfo_auto_4se(unsigned int address)
{
	unsigned int status = 0;
	int ret;
	struct gpt_norflash_msg  msg;

#ifdef GPT_NOR_DEBUG
	printf("erase flash ,offset:%x\n", address);
#endif
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.instr = S25FS_COMMAND_P4E;
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr = address;

	msg.addr_len = 4;
	msg.data_len = 0;
	msg.dumy_len = 0;
	msg.pollflag = (GPT_SFO_POLLING_EN |
		 0 << GPT_SFO_POLLING_BSEL) & (~GPT_SFO_POLLING_BPOL);

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}



int s25fs_norflash_sfo_auto_se(unsigned int address)
{
	unsigned int status = 0;
	int ret;
	struct gpt_norflash_msg  msg;

#ifdef GPT_NOR_DEBUG
	printf("erase flash ,offset:%x\n", address);
#endif
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
#ifdef NORFLASH_MT25QU01GBBB_128MB
	msg.instr = S25FS_COMMAND_SE;   //sector 64k
#else
	msg.instr = S25FS_COMMAND_P4E;  //sector 4k
#endif
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
#if 1 
	msg.addr = address;
	msg.addr_len = 4;    		//for 4byte mode
#else
	msg.addr = address & 0xffffff;
	msg.addr_len = 3;    		//for 3byte mode
#endif
	msg.data_len = 0;
	msg.dumy_len = 0;
	msg.pollflag = (GPT_SFO_POLLING_EN |
		 0 << GPT_SFO_POLLING_BSEL) & (~GPT_SFO_POLLING_BPOL);

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int s25fs_norflash_sfo_se(unsigned int address)
{
	unsigned int status = 0;
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.instr = S25FS_COMMAND_P4E;
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;
	msg.addr = address;

	msg.addr_len = 3;
	msg.data_len = 0;
	msg.dumy_len = 0;
	msg.pollflag = 0;
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("%s:%d write enable failed\n", __func__, __LINE__);
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int s25fs_norflash_sfo_4read(unsigned char *buff, unsigned int addr, unsigned int length)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.buff = buff;
	msg.len_buff = length;
	msg.addr = addr;
	msg.addr_len = 4;
	msg.data_len = 4;
	msg.dumy_len = 1;
	msg.instr = S25FS_COMMAND_FST_RD;
	printf("sfo read addr:%x , length:%d\n", addr, length);
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}

	return 0;
}

int w25q80_norflash_sfo_read_status2(void)
{
	int ret;
	unsigned int status;
	struct gpt_norflash_msg  msg;

	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.instr = S25FS_COMMAND_RDSR1;
	msg.buff = (unsigned char *)&status;
	msg.len_buff = 4;

	msg.addr_len = 0;
	msg.data_len = 4;
	msg.dumy_len = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}

	return status;
}

int w25q80_norflash_quad_write(unsigned char *buff,
		unsigned int addr, unsigned int length)
{
	writel(0x58032, &GPT_REGS(pp_cmd)); //bugfix
	s25fs_norflash_write(buff, addr, length);
	writel(0x58002, &GPT_REGS(pp_cmd)); //bugfix
	return 0;
}

int w25q80_norflash_quad_read(unsigned char *buff,
		unsigned int addr, unsigned int length)
{
	writel(0x16b, &GPT_REGS(read_cmd)); //bugfix
	s25fs_norflash_read(buff, addr, length);
	writel(0x10b, &GPT_REGS(read_cmd)); //bugfix
	return 0;
}

int w25q80_norflash_dual_write(unsigned char *buff,
		unsigned int addr, unsigned int length)
{
	return 0;
}
int w25q80_norflash_sfo_dual_read(unsigned char *buff,
		unsigned int addr, unsigned int length)
{
	writel(0x13b, &GPT_REGS(read_cmd)); //bugfix
	s25fs_norflash_read(buff, addr, length);
	writel(0x10b, &GPT_REGS(read_cmd)); //bugfix

	return 0;
}

int s25fs_norflash_sfo_read(unsigned char *buff,
		unsigned int addr, unsigned int length)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_READ;
	msg.buff = buff;
	msg.len_buff = length;
	msg.addr = addr;
	msg.addr_len = 3;
	msg.data_len = 4;
	msg.dumy_len = 0;
#if defined(CONFIG_GPT_NORFLASH_EXTEND_ADDR_MODE)
	msg.instr = S25FS_COMMAND_READ;
#elif defined(CONFIG_GPT_NORFLASH_4BYTE_ADDR_MODE)
	msg.instr = S25FS_COMMAND_FST_RD;
#endif

#ifdef GPT_NOR_DEBUG
	printf("sfo read addr:%x , length:%d\n", addr, length);
#endif
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}

	return 0;
}

int s25fs_norflash_sfo_auto_write(unsigned char *data,
		unsigned int addr, unsigned int length)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = data;
	msg.len_buff = length;
	msg.instr = S25FS_COMMAND_PP;
	msg.addr = addr;
	msg.addr_len = 3;
	msg.data_len = 4;
	msg.dumy_len = 0;
	msg.pollflag = GPT_SFO_POLLING_EN |
		GPT_SFO_POLLING_BPOL | 0 << GPT_SFO_POLLING_BSEL;

#ifdef GPT_NOR_DEBUG
	printf("sfo write addr:%x , length:%d\n", addr, length);
#endif
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	return 0;
}

int w25p80_norflash_setst(int flags)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = (unsigned char *)&flags;
	msg.len_buff = 4;
	msg.instr = W25Q80_COMMAND_WSR;
	msg.addr_len = 0;
	msg.data_len = 2;
	msg.dumy_len = 0;
	msg.pollflag = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}
int s25fs_norflash_sfo_write(unsigned char *data, unsigned int addr, unsigned int length)
{
	int i;
	int count;
	int result;
	count = (length + 3) / 4;
	for (i = 0; i < count; i++) {
		s25fs_norflash_wren();
		//writel(0x58002, &GPT_REGS(pp_cmd)); //bugfix
		s25fs_norflash_sfo_write_4byte(data + i * 4, addr + i * 4);
		do {
			result = s25fs_norflash_getst();
			udelay(10);
			//printf("-------result: %x\n", result);
		} while (result & 0x1);
	}
	return 0;
}

int s25fs_norflash_sfo_4write_4byte(unsigned char *data, unsigned int addr)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = data;
	msg.len_buff = 4;
	msg.instr = S25FS_COMMAND_PP;
	msg.addr = addr;
	msg.addr_len = 4;
	msg.data_len = 4;
	msg.dumy_len = 0;
	msg.pollflag = 0;
/*
	msg.pollflag = GPT_SFO_POLLING_EN |
		GPT_SFO_POLLING_BPOL | 0 << GPT_SFO_POLLING_BSEL;
*/
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int s25fs_norflash_sfo_4write(unsigned char *data,
		unsigned int addr, unsigned int length)
{
	int i;
	int count;
	int result;
	count = (length + 3) / 4;
	for (i = 0; i < count; i++) {
		s25fs_norflash_wren();
		s25fs_norflash_sfo_4write_4byte(data + i * 4, addr + i * 4);
		do {
			result = s25fs_norflash_getst();
			udelay(100*1000);
		} while (result & 0x1);
	}
	return 0;
}

int s25fs_norflash_sfo_write_4byte(unsigned char *data, unsigned int addr)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = data;
	msg.len_buff = 4;
	msg.instr = S25FS_COMMAND_PP;
	msg.addr = addr;
	msg.addr_len = 3;
	msg.data_len = 4;
	msg.dumy_len = 0;
	msg.pollflag = 0;
/*
	msg.pollflag = GPT_SFO_POLLING_EN |
		GPT_SFO_POLLING_BPOL | 0 << GPT_SFO_POLLING_BSEL;
*/
	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int s25fs_nor_flash_sfo_auto_erase(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_CHIPERASE;
	msg.addr_len = 0;
	msg.data_len = 0;
	msg.dumy_len = 0;

	msg.pollflag = (GPT_SFO_POLLING_EN |
		0 << GPT_SFO_POLLING_BSEL) & (~GPT_SFO_POLLING_BPOL);

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	printf("erase norflash complete, status is %x, host status: %x\n",
			s25fs_norflash_getst(), GPT_REGS(sfc_int));
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	return 0;
}

int s25fs_nor_flash_sfo_erase(void)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_INST;
	msg.instr = S25FS_COMMAND_CHIPERASE;
	msg.addr_len = 0;
	msg.data_len = 4;
	msg.dumy_len = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}

	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));
	return 0;
}

int s25fs_norflash_axi_host4byte_dev3byte_test(int flags)
{
	unsigned int regs;


#define GPT_AXI_H4_D3_TEST_LEN	0x100
#define GPT_AXI_H4_D3_TEST_ADDR	0x0a0

	memset(data_r, 0xff, 0x100);
	s25fs_norflash_read(data_r, GPT_AXI_H4_D3_TEST_ADDR , 0x20);

	gpt_norflash_dumpregs();
	regs = readl(&GPT_REGS(sfc_ctl));
	writel(regs | GPT_SPI_EN4BT_ADDR, &GPT_REGS(sfc_ctl)); //bugfix
	writel(0x1bb, &GPT_REGS(read_cmd)); //bugfix
	gpt_norflash_dumpregs();

	memset(data_r, 0xff, 0x100);
	s25fs_norflash_read(data_r, GPT_AXI_H4_D3_TEST_ADDR, 0x20);

	regs = readl(&GPT_REGS(sfc_ctl));
	writel(regs & (~GPT_SPI_EN4BT_ADDR), &GPT_REGS(sfc_ctl)); //bugfix
	writel(0x10b, &GPT_REGS(read_cmd)); //bugfix

	gpt_norflash_dumpregs();
	return 0;
}

int s25fs_norflash_axi_4byte_test(int flags)
{
	int ret;
	unsigned int regs;

	generate_random(data_w, 0x1000);

	regs = readl(&GPT_REGS(sfc_ctl));
	writel(regs | GPT_SPI_EN4BT_ADDR, &GPT_REGS(sfc_ctl)); //bugfix

	s25fs_norflash_4byte_en();
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_4se(GPT_AXI_TEST_ADDR);
	s25fs_norflash_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);

	s25fs_norflash_write(data_w, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	s25fs_norflash_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);

	ret = gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN);
	if (ret)
		printf("%s:%d, axi test check data failed\n", __func__, flags);
	else
		printf("%s:%d axi test success\n", __func__, flags);

	regs = readl(&GPT_REGS(sfc_ctl));
	writel(regs & (~GPT_SPI_EN4BT_ADDR), &GPT_REGS(sfc_ctl)); //bugfix
	s25fs_norflash_dev_rst();
	return 0;
}

int s25fs_norflash_axi_test(int flags)
{
	int ret;

	generate_random(data_w, 0x2000);
//	s25fs_norflash_wren();
//	s25fs_norflash_sfo_auto_se(GPT_AXI_TEST_ADDR);
//	s25fs_norflash_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	curdev->dev_wren();
	curdev->sec_erase(GPT_AXI_TEST_ADDR);
	curdev->fast_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	if (gpt_norflash_checkblank(data_r, GPT_AXI_TEST_LEN))
		printf("%s:%d erase norflash failed\n", __func__, __LINE__);

#ifdef GPT_NOR_DEBUG
	gpt_norflash_dumpdata(data_w, GPT_AXI_TEST_LEN);
#endif
//	s25fs_norflash_wren();
//	s25fs_norflash_write(data_w, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	curdev->dev_wren();
	curdev->fast_write(data_w, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);

	memset(data_r, 0x0, GPT_AXI_TEST_LEN);
//	s25fs_norflash_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	curdev->fast_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	ret = gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN);
	if (ret)
		printf("%d, axi test check data failed\n", flags);
	else
		printf("%d axi test success\n", flags);

	return 0;
}

int w25q80_norflash_protect_test(int flags)
{
	#define GPT_WR_PR_ADDR	0x3000
	printf("%d host write  protect test case\n", flags);
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(0);
	s25fs_norflash_read(data_r, 0, 0x20);
	if (gpt_norflash_checkblank(data_r, 0x20))
		printf("%s error>>> check black failed\n", __func__);

	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(GPT_WR_PR_ADDR);
	s25fs_norflash_read(data_r, GPT_WR_PR_ADDR, 0x20);
	if (gpt_norflash_checkblank(data_r, 0x20))
		printf("%s error>>> check black failed\n", __func__);

	writel(0x50000000, &GPT_REGS(legal_wr_sp0_base));
	writel(0x50002000, &GPT_REGS(legal_wr_sp0_end));
	writel(0x50005000, &GPT_REGS(legal_wr_sp1_base));
	writel(0x50007000, &GPT_REGS(legal_wr_sp1_end));

	printf("write protect normal ,opt address 0x0\n");
	s25fs_norflash_write(data_w, 0, 0x100);
	s25fs_norflash_read(data_r, 0, 0x100);
	if (gpt_norflash_checkdata(data_w, data_r, 0x100))
		printf("normal address test failed\n");
	else
		printf("normal address test success\n");

	memset(data_r, 0x00, 0x1000);

	printf("write protect ,opt address 0x%x\n", GPT_WR_PR_ADDR);
	s25fs_norflash_write(data_w, GPT_WR_PR_ADDR, 0x20);
	gpt_norflash_dumpregs();
	if (readl(&GPT_REGS(sfc_int)) & (GPT_WR_PT_ADDR_ST | GPT_WR_PT_ADDR_RAW))
		printf("norflash host protect check success\n");
	writel(GPT_WR_PT_ADDR_CLR, &GPT_REGS(sfc_int));
	s25fs_norflash_read(data_r, GPT_WR_PR_ADDR, 0x20);
	if (gpt_norflash_checkdata(data_w, data_r, 0x20))
		printf("protect address test success\n");
	else
		printf("protect address test failed\n");

#ifdef GPT_NOR_DEBUG
	printf("write protect restore envirenment\n");
#endif
	writel(0x0, &GPT_REGS(legal_wr_sp0_base));
	writel(0xffffffff, &GPT_REGS(legal_wr_sp0_end));
	writel(0x0, &GPT_REGS(legal_wr_sp1_base));
	writel(0xffffffff, &GPT_REGS(legal_wr_sp1_end));

#ifdef GPT_NOR_DEBUG
	printf("write protect test complete\n");
#endif
	return 0;
}

int gd25_norflash_setst1(int flags)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = (unsigned char *)&flags;
	msg.len_buff = 4;
	msg.instr = W25Q80_COMMAND_WSR1;
	msg.addr_len = 0;
	msg.data_len = 1;
	msg.dumy_len = 0;
	msg.pollflag = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int s25fs_norflash_dual_test(int flags)
{
	if (curdev->dual_read == NULL) {
		printf("current norflash not support dual mode\n");
		return -1;
	}
	gpt_norflash_dumpregs();
	memset(data_r, 0xff, 0x2000);
	curdev->dual_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	if (gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN))
		printf("%d: dual mode test failed\n", flags);
	else
		printf("%d: dual mode test success\n", flags);
	return 0;
}

int s25fs_norflash_quad_test(int flags)
{
	int ret = 0;

	if ((curdev->quad_write == NULL) && (curdev->quad_read == NULL)) {
		printf("current norflash not support quad mode\n");
		return -1;
	}

	if (curdev->enable_quad) {
		curdev->dev_wren();
		curdev->enable_quad();
	}
	udelay(1000*1000);

#ifdef GPT_NOR_DEBUG
	printf("pp_cmd: %x\n", readl(&GPT_REGS(pp_cmd))); //bugfix
#endif

	if (curdev->quad_write) {
		generate_random(data_w, 0x2000);
		printf("quad write data\n");
		curdev->dev_wren();
		curdev->quad_write(data_w, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	}

#ifdef GPT_NOR_DEBUG
	gpt_norflash_dumpdata(data_w, 0x50);
#endif
	memset(data_r, 0x0, GPT_AXI_TEST_LEN);
	curdev->quad_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	ret = gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN);
	if (ret)
		printf("%d, quad test check data failed\n", flags);
	else
		printf("%d quad test success\n", flags);

	if (curdev->disable_quad) {
		curdev->dev_wren();
		curdev->disable_quad();
	}
	printf("norflash state2 is %x\n", w25q80_norflash_getst1());
	return 0;
}

int w25q80_axi_order_test(int clk_div)
{
	printf("G. test for clock mod0 r/w single mode\n");
	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE0
				| GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_CS_DEASSERT
				| GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	printf(">> test for write protect\n");
	gpt_norflash_dumpregs();
	w25q80_norflash_protect_test(1);

	gpt_norflash_dumpregs();
	printf(">> test for axi read/write\n");
	s25fs_norflash_axi_test(1);
#ifdef GPT_OPTION_4BYTE_EN
	s25fs_norflash_axi_4byte_test(1);
#endif
	printf("G. ##################################################\n");

	printf("H. test for clock mode0 read dual mode\n");
	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE0 |
				GPT_SPI_RD_MODE_DUAL_OT | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
				GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	s25fs_norflash_dual_test(1);
	printf("H. ##################################################\n");

	printf("I. test for clock mode0 r/w quad mode\n");

	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
#if DEBUG_TEST1
	gpt_norflash_dumpregs();
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(0);
	s25fs_norflash_read(data_r, 0, 0x32);
	if (gpt_norflash_checkblank(data_r, 0x32))
		printf("%s error>>> check black failed\n", __func__);
#else
	if (curdev->quad_write) {
		printf("quad erase  flash\n");
		curdev->dev_wren();
		curdev->sec_erase(GPT_AXI_TEST_ADDR);
		curdev->fast_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
		if (gpt_norflash_checkblank(data_r, GPT_AXI_TEST_LEN))
			printf("%s error>>> check black failed\n", __func__);
	}
#endif

	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE0 |
				GPT_SPI_RD_MODE_QUAD_OT | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
				GPT_SPI_WT_MODE_QUAD_IT	| GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	gpt_norflash_dumpregs();
	s25fs_norflash_quad_test(2);
	printf("I. ##################################################\n");

#if 0
	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE0 |
				GPT_SPI_RD_MODE_QUAD_IO | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
				GPT_SPI_WT_MODE_QUAD_IO | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_axi_test(4);
#endif
	printf("J. test for clock mode0 r/w single mode pref disable\n");
	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE0  |
				GPT_SPI_RD_MODE_SIG  | GPT_CS_DEASSERT | GPT_SPI_WT_MODE_SIG
				| GPT_SPI_WP_VAL | GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	printf(">> test for write protect\n");
	gpt_norflash_dumpregs();
	w25q80_norflash_protect_test(8);

	gpt_norflash_dumpregs();
	printf(">> test for axi read/write\n");
	s25fs_norflash_axi_test(8);
#ifdef GPT_OPTION_4BYTE_EN
	s25fs_norflash_axi_4byte_test(8);
#endif
	printf("J. ##################################################\n");

	printf("K. test for clock mode0 read dual mode pref disable\n");
	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE0 | GPT_SPI_RD_MODE_DUAL_OT
				| GPT_CS_DEASSERT | GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL
				| GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN) ,
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_dual_test(1);
#if 0
	gpt_norflash_dumpregs();

	memset(data_r, 0xff, 0x2000);
	w25q80_norflash_sfo_dual_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	if (gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN))
		printf("dual mode test failed\n");
	else
		printf("dual mode test success\n");
#endif
	printf("K. ##################################################\n");

	printf("L. test for clock mode0 r/w quad mode pref disable\n");
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
#if DEBUG_TEST1
	gpt_norflash_dumpregs();
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(0);
	s25fs_norflash_read(data_r, 0, 0x32);
	if (gpt_norflash_checkblank(data_r, 0x32))
		printf("%s error>>> check black failed\n", __func__);
#else
	if (curdev->quad_write) {
		printf("quad erase  flash\n");
		curdev->dev_wren();
		curdev->sec_erase(GPT_AXI_TEST_ADDR);
		curdev->fast_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
		if(gpt_norflash_checkblank(data_r, GPT_AXI_TEST_LEN))
			printf("%s error>>> check black failed\n", __func__);
	}

#endif

	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE0 | GPT_SPI_RD_MODE_QUAD_OT |
				GPT_CS_DEASSERT | GPT_SPI_WT_MODE_QUAD_IT | GPT_SPI_WP_VAL |
				GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	gpt_norflash_dumpregs();
	s25fs_norflash_quad_test(9);
	printf("L. ##################################################\n");
#if 0
	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE0 |
				GPT_SPI_RD_MODE_QUAD_IO | GPT_CS_DEASSERT |
				GPT_SPI_WT_MODE_QUAD_IO | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_axi_test(11);
#endif
	printf("M. test for clock mode3 r/w single mode pref enable\n");
	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE3 |
				GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
				GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
				GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	printf(">> test for write protect\n");
	gpt_norflash_dumpregs();
	w25q80_norflash_protect_test(15);

	gpt_norflash_dumpregs();
	printf(">> test for axi read/write\n");
	s25fs_norflash_axi_test(15);
#ifdef GPT_OPTION_4BYTE_EN
	s25fs_norflash_axi_4byte_test(15);
#endif
	printf("M. ##################################################\n");

	printf("N. test for clock mode3 read dual mode pref enable\n");
	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_DUAL_OT | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_dual_test(1);
#if 0
	gpt_norflash_dumpregs();
	memset(data_r, 0xff, 0x2000);
	w25q80_norflash_sfo_dual_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	if (gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN))
		printf("dual mode test failed\n");
	else
		printf("dual mode test success\n");
#endif
	printf("N. ##################################################\n");

	printf("O. test for clock mode3 r/w quad mode pref enable\n");

	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
#if DEBUG_TEST1
	gpt_norflash_dumpregs();
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(0);
	s25fs_norflash_read(data_r, 0, 0x32);
	if(gpt_norflash_checkblank(data_r, 0x32))
		printf("%s error>>> check black failed \n", __func__);
#else
	if (curdev->quad_write) {
		printf("quad erase  flash\n");
		curdev->dev_wren();
		curdev->sec_erase(GPT_AXI_TEST_ADDR);
		curdev->fast_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
		if (gpt_norflash_checkblank(data_r, GPT_AXI_TEST_LEN))
			printf("%s error>>> check black failed\n", __func__);
	}
#endif

	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_QUAD_OT | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
			GPT_SPI_WT_MODE_QUAD_IT | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	gpt_norflash_dumpregs();
	s25fs_norflash_quad_test(16);
	printf("O. ##################################################\n");
#if 0
	gpt_norflash_setconfig(clk_div | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_QUAD_IO | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
			GPT_SPI_WT_MODE_QUAD_IO | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_axi_test(18);
#endif
	printf("P. test for clock mode3 r/w single mode pref disable\n");
	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE3 | GPT_SPI_RD_MODE_SIG |
			GPT_CS_DEASSERT | GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL |
			GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	printf(">> test for write protect\n");
	gpt_norflash_dumpregs();
	w25q80_norflash_protect_test(22);

	gpt_norflash_dumpregs();
	printf(">> test for axi read/write\n");
	s25fs_norflash_axi_test(22);
#ifdef GPT_OPTION_4BYTE_EN
	s25fs_norflash_axi_4byte_test(22);
#endif
	printf("P. ##################################################\n");

	printf("Q. test for clock mode3 read dual mode pref disable\n");
	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE3 | GPT_SPI_RD_MODE_DUAL_OT  |
			GPT_CS_DEASSERT | GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL |
			GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_dual_test(1);
#if 0
	gpt_norflash_dumpregs();
	printf(">> test for axi read/write\n");
	memset(data_r, 0xff, 0x2000);
	w25q80_norflash_sfo_dual_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
	if (gpt_norflash_checkdata(data_w, data_r, GPT_AXI_TEST_LEN))
		printf("dual mode test failed\n");
	else
		printf("dual mode test success\n");
#endif
	printf("Q. ##################################################\n");

	printf("R. test for clock mode3 r/w quad mode pref disable\n");

	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
#if DEBUG_TEST1
	gpt_norflash_dumpregs();
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(0);
	s25fs_norflash_read(data_r, 0, 0x32);
	if (gpt_norflash_checkblank(data_r, 0x32))
		printf("%s error>>> check black failed \n", __func__);
#else
	if (curdev->quad_write) {
		printf("quad erase  flash\n");
		curdev->dev_wren();
		curdev->sec_erase(GPT_AXI_TEST_ADDR);
		curdev->fast_read(data_r, GPT_AXI_TEST_ADDR, GPT_AXI_TEST_LEN);
		if(gpt_norflash_checkblank(data_r, GPT_AXI_TEST_LEN))
			printf("%s error>>> check black failed\n", __func__);
	}

#endif

	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE3 | GPT_SPI_RD_MODE_QUAD_OT |
			GPT_CS_DEASSERT | GPT_SPI_WT_MODE_QUAD_IT | GPT_SPI_WP_VAL |
			GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	gpt_norflash_dumpregs();
	s25fs_norflash_quad_test(23);
	printf("R. ##################################################\n");
#if 0
	gpt_norflash_setconfig((clk_div | GPT_SPI_MODE3 | GPT_SPI_RD_MODE_DUAL_IO |
			GPT_CS_DEASSERT | GPT_SPI_WT_MODE_QUAD_IO | GPT_SPI_WP_VAL |
			GPT_SPI_HD_VAL) & (~GPT_SPI_PREF_EN),
			GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_axi_test(26);
#endif
	return 0;
}

void generate_random(unsigned char *buff, int length)
{
	int i;
	unsigned int rand = get_timer(0);
#ifndef GPT_NOR_DEBUG
	printf("random data is %x\n", rand);
#endif
	for (i = 0; i < length; i++)
		buff[i] = (rand++) & 0xff;
}

int norflash_lookup_dev(void)
{
	int count;
	int devid = s25fs_norflash_getid();
	printf("norflash  device id %x\n", devid);
	for (count = 0; count < 10; count++) {
#ifdef GPT_NOR_DEBUG
		printf("table : %x\n", nordev[count].id);
#endif
		if ((devid & 0xffffff) == nordev[count].id) {
			printf("norflash %x device found\n", devid);
			curdev = &nordev[count];
			return count;
		}
	}
	return -1;
}

int sst26_quad_enable(void)
{
	w25p80_norflash_setst(0x020a);
	return 0;
}

int w25p80_quad_enable(void)
{
	w25p80_norflash_setst(0x0202);
	return 0;
}

int pn25f_quad_wrsr(int data)
{
	int ret;
	struct gpt_norflash_msg  msg;
	msg.opts = GPT_SFO_CMD_TYPE_WRITE;
	msg.buff = (unsigned char *)&data;
	msg.len_buff = 4;
	msg.instr = W25Q80_COMMAND_WSR;
	msg.addr_len = 0;
	msg.data_len = 1;
	msg.dumy_len = 0;
	msg.pollflag = 0;

	ret =  gpt_norflash_sfo_xfer(&msg);
	if (ret) {
		printf("msg transfer faild\n");
		return -1;
	}
	writel(GPT_SFO_INT_CLR, &GPT_REGS(sfc_int));

	return 0;
}

int pn25f_quad_enable(void)
{
	pn25f_quad_wrsr(0x40);
	return 0;
}

int pn25f_quad_disable(void)
{
	pn25f_quad_wrsr(0x00);
	return 0;
}

int w25q32_quad_enable(void)
{
	gd25_norflash_setst1(0x2);
	return 0;
}

int w25q32_quad_disable(void)
{
	gd25_norflash_setst1(0x0);
	return 0;
}

int mx25l32_quad_enable(void)
{
	pn25f_quad_wrsr(0x40);
	return 0;
}

int mx25l32_quad_disable(void)
{
	pn25f_quad_wrsr(0x0);
	return 0;
}

int at25sf_quad_enable(void)
{
	w25p80_norflash_setst(0x0202);
	return 0;
}

int at25sf_quad_disable(void)
{
	w25p80_norflash_setst(0x0200);
	return 0;
}

int gpt_dev_probe(void)
{
	gpt_norflash_reset();
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	if (norflash_lookup_dev() < 0) {
		printf("norflash device do not support\n");
		return -1;
	}
	return 0;
}
int s25fs_nor_flash_test(void)
{
	int result = 0;

	generate_random(data_w, 0x1000);
	printf("init norflash controler\n");
	gpt_norflash_init();

	writel(0x10b, &GPT_REGS(read_cmd)); //bugfix

	printf("..................FOR BASE TEST......................\n");
	printf("A. read data from axi without any configure\n");
	s25fs_norflash_read(data_r, 0x0, 0x100);
	gpt_norflash_dumpdata(data_r, 0x100);
	printf("A. ##################################################\n");

	gpt_norflash_reset();
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	if (norflash_lookup_dev() < 0) {
		printf("norflash device do not support\n");
		return -1;
	}

	if (curdev->dev_rst)
		curdev->dev_rst();

	printf("B. read data from norflash for poweron check\n");
	curdev->sfo_read(data_r, 0x0, 0x30);
	gpt_norflash_dumpdata(data_r, 0x30);
	gpt_norflash_dumpregs();
	printf("B. ##################################################\n");
	printf("\n\nC. norflash id is %x\n", s25fs_norflash_getid());
	printf("C. ##################################################\n");
	printf("norflash state1: %x , state2: %x\n",
		s25fs_norflash_getst(), w25q80_norflash_getst1());

	if (curdev->protect_unlock) {
		curdev->dev_wren();
		curdev->protect_unlock();
	}

#define GPT_NOR_HOLD_WP
#ifdef GPT_NOR_HOLD_WP
	printf("D. ...........please check wp and hold signal........\n");
	gpt_norflash_hd_wp_test(0);
	udelay(6000*1000);
	gpt_norflash_hd_wp_test(1);
	printf("D. ##########wp and hold signal test finish##########\n");
#endif

	printf("..................FOR SFO TEST......................\n");

#define GPT_NOR_NO_AUTOPOLL
#ifdef GPT_NOR_NO_AUTOPOLL
	printf("E. test sfo write without autopoll\n");
	curdev->fast_read(data_r, 0, 0x32);
	curdev->dev_wren();
	s25fs_norflash_sfo_se(0);
	printf("page erase norflash complete, before wait, status is %x\n",
				s25fs_norflash_getst());
	do {
		result = s25fs_norflash_getst();
		udelay(100*1000);
	} while (result & 0x1);
	printf("page erase norflash complete, status is %x\n",
				s25fs_norflash_getst());

	curdev->fast_read(data_r, 0, 0x32);
	if (gpt_norflash_checkblank(data_r, 0x32))
		printf("%s error>>> check black failed\n", __func__);

	curdev->sfo_write(data_w, 0, 0x32);
	gpt_norflash_dumpregs();

	curdev->fast_read(data_r, 0, 0x32);
	if (gpt_norflash_checkdata(data_w, data_r, 0x32))
		printf("%s disable autopoll test check data failed \n", __func__);
	else
		printf("%s disable autopoll test success\n", __func__);

	printf("E. ##################################################\n");
#endif

#if 0
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE0 |
		GPT_SPI_RD_MODE_QUAD_OT | GPT_SPI_PREF_EN | GPT_CS_DEASSERT |
		GPT_SPI_WT_MODE_QUAD_IT	| GPT_SPI_WP_VAL | GPT_SPI_HD_VAL,
		GPT_SFO_INT_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
	s25fs_norflash_quad_test(2);
	return 0;

	curdev->dev_wren();
	curdev->enable_quad();
	printf("norflash state1: %x , state2: %x \n", s25fs_norflash_getst(), w25q80_norflash_getst1());
	memset(data_r, 0x0, GPT_AXI_TEST_LEN);
	curdev->quad_read(data_r, 0, GPT_AXI_TEST_LEN);
	gpt_norflash_dumpdata(data_r, 0x50);
	curdev->dev_rst();
	printf("norflash state1: %x , state2: %x \n", s25fs_norflash_getst(), w25q80_norflash_getst1());

	return 0;
#endif

#define GPT_NOR_WITH_AUTOPOLL
#ifdef GPT_NOR_WITH_AUTOPOLL
	printf("F. test sfo write with autopoll\n");
	s25fs_norflash_read(data_r, 0, 0x32);
	s25fs_norflash_wren();
	s25fs_norflash_sfo_auto_se(0);
		printf("page erase norflash complete, status is %x\n",
			s25fs_norflash_getst());
	s25fs_norflash_read(data_r, 0, 0x32);
	if (gpt_norflash_checkblank(data_r, 0x32))
		printf("%s error>> autopoll check black failed\n", __func__);
	else
		printf("%s autopoll check black success\n", __func__);
	printf("F. ##################################################\n");
#endif

	printf("..................FOR AXI TEST......................\n");
	w25q80_axi_order_test(GPT_SPI_CLK_DIV100);
	w25q80_axi_order_test(GPT_SPI_CLK_DIV32);
	w25q80_axi_order_test(GPT_SPI_CLK_DIV16);
/*
	w25q80_axi_order_test(GPT_SPI_CLK_DIV08);
	w25q80_axi_order_test(GPT_SPI_CLK_DIV04);
	w25q80_axi_order_test(GPT_SPI_CLK_DIV02);
*/
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
		GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
		GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
		GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);

	return 0;

#ifdef GPT_OPTION_AUTO_ANYREGS
	s25fs_norflash_wren();
	s25fs_norflash_sfo_wrar(0x00000003, 0x88);
	do {
		result = s25fs_norflash_getst();
		udelay(100*1000);
	} while (result & 0x1);

	udelay(1000*1000);

	gpt_norflash_dumpregs();
	printf("read any status is %x , result:%x\n",
			s25fs_norflash_sfo_rdar(0x00000003), result);
#endif
	return 0;
}

void gpt_test(void)
{
	gpt_norflash_reset();
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV100 | GPT_SPI_MODE3 |
			GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
			GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
			GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);


	printf("\n\nC. norflash id is %x\n", s25fs_norflash_getid());
}
