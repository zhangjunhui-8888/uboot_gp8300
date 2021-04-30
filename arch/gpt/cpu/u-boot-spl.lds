
MEMORY { .rom : ORIGIN = CONFIG_SPL_TEXT_BASE,
	LENGTH = CONFIG_SPL_MAX_SIZE }
MEMORY { .sram : ORIGIN = CONFIG_SPL_BSS_START_ADDR,
	LENGTH = CONFIG_SPL_BSS_MAX_SIZE }

#define LOAD_BASE 0x8000

OUTPUT_FORMAT("elf64-gptx","elf64-gptx","elf64-gptx")
OUTPUT_ARCH(gptx)
ENTRY(_start)
SECTIONS
{
	. = LOAD_BASE;

	. = ALIGN(8);
	.text : {
                *(.__image_copy_start)
		*(.vector_tables)
		*(.text*)
	} >.rom

	. = ALIGN(8);
	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	} >.rom

	.data_load_start :{
		. = ALIGN(8);
		__data_load_start = ALIGN(8);
	}>.rom

	. = ALIGN(8);

	.data :{
		__data_start = ALIGN(8);
		*(.data .data.*)
		__data_end = ALIGN(8);
	}> .sram AT > .rom

	. = ALIGN(8);

	.data_load_end :{
		. = ALIGN(8);
		__data_load_end = ALIGN(8);
	}>.rom

	. = .;

	. = ALIGN(8);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	} >.rom

	. = ALIGN(8);
        .image_copy_end :
        {
                *(.__image_copy_end)
        } >.rom
	__init_end = .;

        .end : {
                *(.__end)
        } >.rom

	.bss_start : {
		. = ALIGN(8);
		KEEP(*(.__bss_start));
	} >.sram

	.bss : {
		*(.bss*)
		. = ALIGN(8);
	} >.sram

	.bss_end : {
		KEEP(*(.__bss_end));
	} >.sram

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
}
