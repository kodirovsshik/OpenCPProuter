
#include <ocppr/int.hpp>

namespace init
{
	struct kernel_args_t
	{
		struct memory_map_categories
		{
			enum
			{
				//invalid category
				reserved = 0,

				//useable after bootloader's data is no longer needed
				available,

				//useable after ACPI tables are parsed
				acpi_reclaimable,

				count
			};
		};

		struct memory_map_entry_t
		{
			u64 begin;
			u64 pages;
		};

		struct { char value[6] = "OCPPR"; } preamble;
		u8 major = 1;
		u8 minor = 0;

		memory_map_entry_t* memory_map;
		uptr base_phy_addr;
		uptr efi_runtime_services;
		u32 memory_map_category_sizes[memory_map_categories::count];
	
	};
}
