
PAGE_SIZE = 4096;
MEMSET_ALIGN = 4096;

OUTPUT_FORMAT(elf64-x86-64)
TARGET(elf64-x86-64)

ENTRY(kernel_entry)

SECTIONS
{
	PROVIDE_HIDDEN(_kernel_space_begin = .);


	/* Kernel's entry point should be the first byte of the image */
	/* This memory region is to be discarded and marked as free
	   as soon as the initialization is done */

	PROVIDE_HIDDEN(_init_segment_begin = .);

	.init :
	{
		*(.init_entry)
		*(.init)
	}

	. = ALIGN(PAGE_SIZE);
	PROVIDE_HIDDEN(_init_segment_size = . - _init_segment_begin);



	PROVIDE_HIDDEN(_executeable_segment_begin = .);

	.text :
	{
		*(.text .text.*)
	}

	. = ALIGN(PAGE_SIZE);
	PROVIDE_HIDDEN(_executeable_segment_size = . - _executeable_segment_begin);



	PROVIDE_HIDDEN(_rwdata_segment_begin = .);
	
	.data :
	{
		*(.data .data.*)
		PROVIDE_HIDDEN(_data_section_end = .);
	}

	. = ALIGN(MEMSET_ALIGN);
	PROVIDE_HIDDEN(_bss_begin = .);
	.bss :
	{
		*(.bss* .bss.*)
	}
	. = ALIGN(MEMSET_ALIGN);
	PROVIDE_HIDDEN(_bss_size = . - _bss_begin);

	. = ALIGN(PAGE_SIZE);
	PROVIDE_HIDDEN(_rwdata_segment_size = . - _rwdata_segment_begin);



	PROVIDE_HIDDEN(_rodata_segment_begin = .);
	
	.rodata :
	{
		*(.rodata .rodata.*)
	}

	. = ALIGN(MEMSET_ALIGN);
	PROVIDE_HIDDEN(_tls_template_begin = .);
	.tdata :
	{
		*(.tdata .tdata.*)
	}
	. = ALIGN(MEMSET_ALIGN);
	PROVIDE_HIDDEN(_tls_template_end = .);

	. = ALIGN(8);
	PROVIDE_HIDDEN(_build_id = .);
	.note.gnu.build-id :
	{
		*(.note.gnu.build-id)
	}
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

	. = ALIGN(PAGE_SIZE);
	PROVIDE_HIDDEN(_rodata_segment_size = . - _rodata_segment_begin);


	. += PAGE_SIZE;
	/* ^^^ This page intentionally left blank */	

	/* This area is to be used by the early init code
	   as a temporaty stack for the init thread */
	PROVIDE_HIDDEN(_main_thread_stack_bottom = .);
	. = . + PAGE_SIZE;
	PROVIDE_HIDDEN(_main_thread_stack_top = .);



	/* today I learned that symbol and string tables
	   are actually sections themselves */
	
	.comment 0 : { *(.comment) }
	.shstrtab 0 : { *(.shstrtab) }
	.symtab 0 : { *(.symtab) }
	.strtab 0 : { *(.strtab) }

	.debug           0 : { *(.debug) }
	.line            0 : { *(.line) }
	.debug_srcinfo   0 : { *(.debug_srcinfo) }
	.debug_sfnames   0 : { *(.debug_sfnames) }
	.debug_aranges   0 : { *(.debug_aranges) }
	.debug_pubnames  0 : { *(.debug_pubnames) }
	.debug_info      0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev    0 : { *(.debug_abbrev) }
	.debug_line      0 : { *(.debug_line .debug_line.* .debug_line_end) }
	.debug_frame     0 : { *(.debug_frame) }
	.debug_str       0 : { *(.debug_str) }
	.debug_loc       0 : { *(.debug_loc) }
	.debug_macinfo   0 : { *(.debug_macinfo) }
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	.debug_pubtypes  0 : { *(.debug_pubtypes) }
	.debug_ranges    0 : { *(.debug_ranges) }
	.debug_addr      0 : { *(.debug_addr) }
	.debug_line_str  0 : { *(.debug_line_str) }
	.debug_loclists  0 : { *(.debug_loclists) }
	.debug_macro     0 : { *(.debug_macro) }
	.debug_names     0 : { *(.debug_names) }
	.debug_rnglists  0 : { *(.debug_rnglists) }
	.debug_str_offsets 0 : { *(.debug_str_offsets) }
	.debug_sup       0 : { *(.debug_sup) }
	.gnu.attributes  0 : { KEEP (*(.gnu.attributes)) }
	
}
