
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

				//useable at all times after control transferred to kernel
				available,

				//useable after init is done, includes kernel image and data
				bootloader,

				//useable after ACPI tables are parsed
				acpi_reclaimable,

				//anything reported as not useable by firmware
				unuseable,

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

		uptr efi_runtime_services = nullptr;
		memory_map_entry_t* memory_map = nullptr;
		u32 memory_map_category_sizes[memory_map_categories::count]{};
	
	};
}
