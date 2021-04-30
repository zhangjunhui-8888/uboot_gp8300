#include <common.h>
#include <console.h>

#include <linux/mtd/gpt_norflash.h>

#define LEGACY_ID_NORFLASH(id, size, sector_num) (size),(sector_num),(id)

/* info for FLASH chips */
flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS] = {
#ifdef NORFLASH_MX25L3206E_4MB 
	{LEGACY_ID_NORFLASH(0x1620c2, CONFIG_FLASH_SECT_SIZE, 1), {CONFIG_ENV_ADDR, (CONFIG_ENV_ADDR + CONFIG_FLASH_SECT_SIZE)}}
#endif

#ifdef NORFLASH_GD25Q18_16MB
	/*LEGACY_ID_NORFLASH(0x1131a1, 131072, 32),*/
#endif
};

void flash_info_dump(flash_info_t *info)
{
	int i;
	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; i++) {
		printf("ID: %lx, size: %ld MB, sector counts: %d\n",
			info[i].flash_id, (info[i].size)/MB, info[i].sector_count);
	}
}

void gpt_norflash_addr_mode(unsigned int mode)
{
	gpt_norflash_setconfig(GPT_SPI_CLK_DIV08 | GPT_SPI_MODE3 | mode | 
	GPT_SPI_RD_MODE_SIG | GPT_SPI_PREF_EN | GPT_SPI_WT_EXPR_MD |
	GPT_SPI_WT_MODE_SIG | GPT_SPI_WP_VAL | GPT_SPI_HD_VAL | GPT_CS_DEASSERT,
	GPT_WR_PT_ADDR_MASK | GPT_WR_PT_ADDR_MASK | GPT_PP_POL_DONE_MASK);
}


unsigned long flash_init(void)
{
	u16 i;
	int value;
	int sector_size = 0;
	int size = 0;

	/* Init: no FLASHes known */
	for (i=0; i<CONFIG_SYS_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}

	printf("Nor Controller init\n");
	gpt_norflash_init();
	gpt_norflash_reset();
	gpt_norflash_addr_mode(GPT_SPI_EN3BT_ADDR);

	value = gpt_norflash_getid();
	switch (value) {
	case 0x1620c2:
		sector_size = 0x1000;
		size =  4 * 1024 * 1024;
		break;

	case 0x1840c8:
		sector_size = 0x1000;
		size =  16 * 1024 * 1024;
		break;

	case 0x1860ef:         			/***** evk *****/
		sector_size = 0x1000;
		size =  16 * 1024 * 1024;
		break;

	case 0x21bb20:         			/***** dvb *****/
		sector_size = 0x10000;		/******64k******/
		size =  64 * 1024 * 1024;
		break;

	default:
		sector_size = 0x1000;
		size =  4 * 1024 * 1024;
		printf("unsupport flash device(0x%x)\n", value);
	}

	flash_info[0].sector_count = size / sector_size;
	flash_info[0].flash_id = value;
	flash_info[0].size = size;

	for (i = 0; (i < flash_info[0].sector_count); i++) {
		flash_info[0].start[i] = CONFIG_SYS_FLASH_BASE + sector_size * i;
	}

	flash_info_dump(flash_info);

	return flash_info[0].size;
}

#ifdef NORFLASH_MT25QU01GBBB_128MB
#define FLASH_SECTOR_SIZE		0x10000
#endif

int write_buff(flash_info_t *info, uchar *src, ulong addr, ulong len)
{
#if 0 
	s25fs_norflash_wren();
	mt25q_norflash_write_extend_addr(addr);

	writel(0x58032, &GPT_REGS(pp_cmd)); //bugfix
	s25fs_norflash_write(src, addr, len);
	writel(0x58002, &GPT_REGS(pp_cmd)); //bugfix

#endif

#if 1 
	gpt_norflash_addr_mode(GPT_SPI_EN4BT_ADDR);
	mt25q_norflash_enter_4byte();
	s25fs_norflash_write(src, addr, len);
	gpt_norflash_addr_mode(GPT_SPI_EN3BT_ADDR);
	mt25q_norflash_exit_4byte();
#endif

#if 0
	s25fs_norflash_sfo_write(src, addr, len);
#endif
	return 0;
}

int read_buff(flash_info_t *info, uchar *dst, ulong addr, ulong len)
{
#if 0 
	s25fs_norflash_wren();
	mt25q_norflash_write_extend_addr(addr);
	mt25q_norflash_read_extend_addr();

	writel(0x16b, &GPT_REGS(read_cmd)); //bugfix
	s25fs_norflash_read(dst, addr, len);
	writel(0x10b, &GPT_REGS(read_cmd)); //bugfix

#endif

#if 1 
	gpt_norflash_addr_mode(GPT_SPI_EN4BT_ADDR);
	mt25q_norflash_enter_4byte();
	s25fs_norflash_read(dst, addr, len);
	gpt_norflash_addr_mode(GPT_SPI_EN3BT_ADDR);
	mt25q_norflash_exit_4byte();
#endif

#if 0
	s25fs_norflash_sfo_read(dst, addr, len);
#endif

	return 0;
}

void flash_print_info(flash_info_t *info)
{
	if (info->flash_id == FLASH_UNKNOWN) {
		printf("missing or unknown FLASH type\n");
		return;
	}

	switch (info->flash_id & 0xff) {
	case 0xa1:
		printf("Fudan ");
		break;
	default:
		printf("%lx Unknown Vendor\n", info->flash_id & 0xff);
	}

	printf("Size %ld KB in %d Sectors\n",
			info->size, info->sector_count);
}


int flash_erase(flash_info_t *info, int s_first, int s_last)
{
	int i;
	ulong pcount, addr;
	ulong sector_addr = info->start[s_first];
	int sector_size = info->size / info->sector_count;

	pcount = s_last - s_first + 1;
	printf("\nerase  address: %lx, sector_count: 0x%lx, sector_size:0x%x\n", sector_addr, pcount, sector_size);

	s25fs_norflash_dev_rst();
	s25fs_norflash_wren();
	s25fs_norflash_unlock();

	if (pcount) {
		for (i = 0; i < pcount; i++) {
			addr = sector_addr + sector_size * i;
			s25fs_norflash_wren();
#ifdef NORFLASH_MT25QU01GBBB_128MB
#if 1 
			gpt_norflash_addr_mode(GPT_SPI_EN4BT_ADDR);
			mt25q_norflash_enter_4byte();
			s25fs_norflash_sfo_auto_se(addr);
			gpt_norflash_addr_mode(GPT_SPI_EN3BT_ADDR);
			mt25q_norflash_exit_4byte();
#endif

#if 0
			mt25q_norflash_write_extend_addr(addr);
			s25fs_norflash_sfo_auto_se(addr);
#endif
#endif
		}
	} else {
		printf("chip erase mode\n");
		s25fs_norflash_wren();
		s25fs_nor_flash_sfo_erase();
	}

	return 0;
}
