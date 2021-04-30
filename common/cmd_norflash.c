#include <common.h>
#include <command.h>
#include <linux/mtd/gpt_norflash.h>

static int do_norflash(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *cmd;
	ulong addr;
	ulong start;
	ulong len;

	cmd = argv[1];

	if (strncmp(cmd, "erase", 5) == 0) {
		addr = (ulong)simple_strtoul(argv[2], NULL, 16);
		len = (ulong)simple_strtoul(argv[3], NULL, 16);

		flash_erase (NULL, addr, len);
	}

	if (strncmp(cmd, "read", 4) == 0) {
		addr = (ulong)simple_strtoul(argv[2], NULL, 16);
		start = (ulong)simple_strtoul(argv[3], NULL, 16);
		len = (ulong)simple_strtoul(argv[4], NULL, 16);

		printf("read addr: 0x%lx, start: 0x%lx, len: 0x%lx\n", addr, start, len);
		read_buff(NULL, (void *)addr, start, len);
	}

	if (strncmp(cmd, "write", 5) == 0) {
		addr = (ulong)simple_strtoul(argv[2], NULL, 16);
		start = (ulong)simple_strtoul(argv[3], NULL, 16);
		len = (ulong)simple_strtoul(argv[4], NULL, 16);

		printf("write addr: 0x%lx, start: 0x%lx, len: 0x%lx\n", addr, start, len);
		write_buff(NULL, (void *)addr, start, len);

	}

	return 0;
}

U_BOOT_CMD(
	norfl,    5,    1,    do_norflash,
        "print FLASH memory information",
        "erase start len\n       - erase norflash memory\n\tlen > 0 all chip erase, or sector erase\n"
        "read addr start len\n   - read data from flash to addr(mem)\n"
        "write addr start len\n   - write data from addr(mem) to flash\n"
);
