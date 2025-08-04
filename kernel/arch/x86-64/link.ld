OUTPUT_FORMAT(elf64-x86-64)
TARGET(elf64-x86-64)

PAGE_SIZE = 4096;
PROVIDE_HIDDEN(_kernel_space_begin = 0xFFFFFFFF80000000);


ENTRY(kernel_entry)

SECTIONS
{
	/* Early init sections go here */
	/* Kernel's entry point should be the first byte of the image */
	/* This memory region is to be discarded and marked as free(?)
	   as soon as the initialization is done */
	/* TODO: consider if there should instead be a gap in the
	   virtual address space for potential address overflow safety */

	PROVIDE_HIDDEN(_init_segment_begin = .)
	.init :
	{
		PROVIDE_HIDDEN(_init_section_begin = .);
		*(.init_entry)
		*(.init)
		PROVIDE_HIDDEN(_init_section_end = .);
	}
	. = ALIGN(PAGE_SIZE);



	/* Executeable sections go here */
	. = ALIGN(PAGE_SIZE);
	PROVIDE_HIDDEN(_executeable_segment_begin = .);

	.text :
	{
		PROVIDE_HIDDEN(_text_section_begin = .);
		*(.text .text.*)
		. = ALIGN(64);
		PROVIDE_HIDDEN(_text_section_end = .);
	}



	/* Read/Write sections go here */
	
	. = ALIGN(PAGE_SIZE);
	.data :
	{
		PROVIDE_HIDDEN(_data_section_begin = .);
		*(.data .data.*)
		. = ALIGN(64);
		PROVIDE_HIDDEN(_data_section_end = .);
	}

	.bss :
	{
		PROVIDE_HIDDEN(_bss_begin = .);
		*(.bss* .bss.*)
		. = ALIGN(64);
		PROVIDE_HIDDEN(_bss_end = .);
	}



	/* Read-only sections go here */
	
	. = ALIGN(PAGE_SIZE);
	PROVIDE_HIDDEN(_read_only_data_begin = .);
	.rodata :
	{
		PROVIDE_HIDDEN(_rodata_section_begin = .);
		*(.rodata .rodata.*)
		. = ALIGN(64);
		PROVIDE_HIDDEN(_rodata_section_end = .);
	}

	. = ALIGN(64);
	.tdata :
	{
		PROVIDE_HIDDEN(_tls_template_begin = .);
		*(.tdata .tdata.*)
		. = ALIGN(64);
		PROVIDE_HIDDEN(_tls_template_end = .);
	}

	. = ALIGN(8);
	.note.gnu.build-id :
	{
		PROVIDE_HIDDEN(_build_id = .);
		*(.note.gnu.build-id)
	}
	. = ALIGN(64);
	PROVIDE_HIDDEN(_read_only_data_end = .);



	. = ALIGN(8);
	.note.build-date :
	{
		*(.note.build-date)
	}
	. = ALIGN(8);
	.note.build-time :
	{
		*(.note.build-time)
	}



	/* This area is to be used by the early init code
	   as a temporaty stack for the init thread */

	. = ALIGN(PAGE_SIZE);

	. += PAGE_SIZE;
	/* ^^^ This page intentionally left blank */	

	PROVIDE_HIDDEN(_main_thread_stack_bottom = .);
	. = . + PAGE_SIZE;
	PROVIDE_HIDDEN(_main_thread_stack_top = .);



	/* today I learned that symbol and string tables
	   are actually sections themselves */
	
	.comment 0 : { *(.comment) }
	.shstrtab 0 : { *(.shstrtab) }
	.symtab 0 : { *(.symtab) }
	.strtab 0 : { *(.strtab) }
}
