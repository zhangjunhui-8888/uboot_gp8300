OUTPUT_FORMAT("elf64-gptx","elf64-gptx","elf64-gptx")
OUTPUT_ARCH(gpt)
ENTRY(_start)
SECTIONS
{
	. = CONFIG_SYS_TEXT_BASE;

	. = ALIGN(8);
	.text : {
                *(.__image_copy_start) 
		*(.vector_tables)
		CPUDIR/start.o (.text*)
		*(.text*)
	}
	. = ALIGN(8);
	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	}

	. = ALIGN(8);
	.data : {
		*(.data*)
	}
	. = ALIGN(8);

	. = .;

	. = ALIGN(8);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	}

	. = ALIGN(8);
	__image_copy_end = .;
	__init_end = .;

	. = ALIGN(8);
	.rela.dyn : {
		__rel_dyn_start = .;
		*(.rela*)
		__rel_dyn_end = .;
	}

	_end = .;

	. = ALIGN(8);

	.bss_start : {
		KEEP(*(.__bss_start));
	}

        .bss : {
                *(.sbss.*)
                *(.bss.*)
		*(.scommon)
                . = ALIGN(4);
        }

	.bss_end : {
		KEEP(*(.__bss_end));
	}

        /DISCARD/ : { *(.dynsym) }
        /DISCARD/ : { *(.dynstr*) }
        /DISCARD/ : { *(.dynamic*) }
        /DISCARD/ : { *(.plt*) }
        /DISCARD/ : { *(.interp*) }
        /DISCARD/ : { *(.gnu*) }

}
