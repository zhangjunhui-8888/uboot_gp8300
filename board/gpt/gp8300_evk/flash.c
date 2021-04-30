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
		printf("ID: %lx, size: %ld B, sectors: %d\n",
			info[i].flash_id, info[i].size, info[i].sector_count);
	}
}

unsigned long flash_init(void)
{
	u16 i;
	int value;
//	int sector_count = 0;
	int sector_size = 0;
	int size = 0;

	/* Init: no FLASHes known */
	for (i=0; i<CONFIG_SYS_MAX_FLASH_BANKS; ++i) {
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}

	printf("Nor Controller init\n");
	gpt_norflash_init();

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

	case 0x1860ef:
		sector_size = 0x1000;
		size =  16 * 1024 * 1024;
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

int write_buff(flash_info_t *info, uchar *src, ulong addr, ulong cnt)
{
	int j;
	int count = cnt / FLASH_PAGE_SIZE;
	int offset = cnt % FLASH_PAGE_SIZE;
	int data_len = FLASH_PAGE_SIZE;

	if (offset)
		count += 1;

	s25fs_norflash_wren();
	for (j = 0; j < count; j++) {
		if (offset && (j == offset - 1))
			data_len = offset;

		s25fs_norflash_write(src + j * FLASH_PAGE_SIZE, addr + j * FLASH_PAGE_SIZE, data_len);
	}

	return 0;
}

int read_buff(flash_info_t *info, uchar *dst, ulong addr, ulong cnt)
{
	s25fs_norflash_read(dst, addr, cnt);
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

//#define GPT_PAGE_SIZE 0x1000

int flash_erase(flash_info_t *info, int s_first, int s_last)
{
	int i;
	ulong pcount;
	ulong addr = info->start[s_first];
	int sector_size = info->size / info->sector_count;

#if 0
//	int s_len = info->size / info->sector_count * (s_last - s_first + 1);
//	pcount = s_len / GPT_PAGE_SIZE;
//	pcount += (s_len % GPT_PAGE_SIZE > 0 ? 1 : 0);
#else
	pcount = s_last - s_first + 1;
#endif
	printf("\nerase  address: %lx, page: %lx, sector_size:0x%x\n", addr, pcount, sector_size);

	s25fs_norflash_dev_rst();

	s25fs_norflash_wren();
	s25fs_norflash_unlock();

	if (pcount) {
		for (i = 0; i < pcount; i++) {
			s25fs_norflash_wren();
			s25fs_norflash_sfo_auto_se(addr + sector_size * i);
		}
	} else {
		printf("chip erase mode\n");
		s25fs_norflash_wren();
		s25fs_nor_flash_sfo_erase();
	}

	return 0;
}
