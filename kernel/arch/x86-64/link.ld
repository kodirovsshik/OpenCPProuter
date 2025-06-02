OUTPUT_FORMAT(elf64-x86-64)
TARGET(elf64-x86-64)

ENTRY(kernel_entry)

SECTIONS
{
	. = 0xFFFFFFFF80000000;

	.entry :
	{
		*(.entry_text)
	}



	. = ALIGN(4096);

	.text :
	{
		PROVIDE_HIDDEN(text_section_begin = .);
		*(.text)
		PROVIDE_HIDDEN(text_section_end = .);
	}



	. = ALIGN(4096);

	.data :
	{
		PROVIDE_HIDDEN(data_section_begin = .);
		*(.data)
		PROVIDE_HIDDEN(data_section_end = .);
	}



	. = ALIGN(4096);

	.rodata :
	{
		PROVIDE_HIDDEN(rodata_section_begin = .);
		*(.rodata)
		PROVIDE_HIDDEN(rodata_section_end = .);
	}



	. = ALIGN(4096);

	.note.gnu.build-id :
	{
		PROVIDE_HIDDEN(build_id = .);
		*(.note.gnu.build-id)
	}

	.comment 0 : { *(.comment) }
}
