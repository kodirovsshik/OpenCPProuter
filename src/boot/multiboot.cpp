
#include <int.hpp>
#include <defs.hpp>

struct alignas(8) multiboot_header
{
	u32 magic = 0xE85250D6;
	u32 architecture = 0;
	u32 header_length = sizeof(multiboot_header);
	u32 checksum = u32(0x100000000 - 0xE85250D6 - sizeof(multiboot_header));

	struct alignas(8) multiboot_tag_flags
	{
		u16 type = 4;
		u16 flags = 0;
		u32 size = sizeof(multiboot_tag_flags);
		u32 console_flags = 1;
	} tag_flags;

	struct alignas(8) multiboot_tag_relocatable
	{
		u16 type = 10;
		u16 flags = 1;
		u32 size = sizeof(multiboot_tag_relocatable);
		u32 min_addr = 0;
		u32 max_addr = -1;
		u32 align = 4096;
		u32 preference = 2;
	} tag_relocatable;

	struct alignas(8) multiboot_tag_stop
	{
		u16 type = 0;
		u16 flags = 0;
		u32 size = sizeof(multiboot_tag_stop);
	} tag_stop;

} HIDDEN const SECTION(.multiboot) ATTR(used) multiboot_header_instance{};
