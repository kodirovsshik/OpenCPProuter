
#include "efi_helper.h"

#define stringify1(x) #x
#define stringify(x) stringify1(x)

#define countof(arr) (sizeof(arr)/sizeof(*(arr)))

alignas(4096) const unsigned char KERNEL_PAYLOAD[] =
{
#embed stringify(KERNEL_BIN)
};


static constexpr UINT32 kernel_memory_map_category_flags[] =
{
	//invalid category
	0,

	//useable after bootloader trasnfers control to kernel
	(1 << EfiConventionalMemory) |
	(1 << EFI_MEMORY_TYPE_CUSTOM::EfiPersistentMemory) | //Apparently this one is absent in my gnu-efi package
	(1 << EfiLoaderCode) |
	(1 << EfiLoaderData) |
	(1 << EfiBootServicesCode) |
	(1 << EfiBootServicesData),

	//useable after ACPI tables are parsed
	(1 << EfiACPIReclaimMemory),
};

static constexpr uint32_t kernel_memory_map_categories_count = countof(kernel_memory_map_category_flags);


struct kernel_data_t
{
	struct memory_map_entry_t
	{
		uint64_t begin;
		uint64_t pages;
	};

	struct preamble_t
	{
		struct { char value[6] = "OCPPR"; } str;
		uint8_t major = 1;
		uint8_t minor = 0;
	};

	preamble_t preamble;
	memory_map_entry_t* memory_map;
	uint32_t memory_map_category_sizes[kernel_memory_map_categories_count];
	
} kernel_data{};



UINTN memory_map_key = 0;
EFI_SYSTEM_TABLE* ST = nullptr;



template<class T>
T* alloc(UINTN n)
{
	UINTN bytes = n * sizeof(T);
	bytes -= 1;
	bytes |= 4095;
	bytes += 1;

	EFI_PHYSICAL_ADDRESS efi_ptr{};
	ST->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, bytes / 4096, &efi_ptr);
	return (T*)efi_ptr;
}

void fill_memory_map()
{
	UINT8 *efi_table = nullptr;
	UINTN bytes_total = 0;
	UINTN bytes_per_item = 0;
	UINT32 version = 0;

	auto try_get_memory_map = [&]()
	{
		return ST->BootServices->GetMemoryMap(
			&bytes_total,
			(EFI_MEMORY_DESCRIPTOR*)efi_table,
			&memory_map_key,
			&bytes_per_item,
			&version
			);
	};

	try_get_memory_map();
	
	bytes_total += 4 * bytes_per_item;
	const UINTN max_entries_total = bytes_total / bytes_per_item + 4;
	
	efi_table = alloc<UINT8>(bytes_total);
	auto* kernel_table = alloc<kernel_data_t::memory_map_entry_t>(max_entries_total);

	try_get_memory_map(); //TODO: check for success

	uint32_t kernel_table_index = 0;
	for (uint32_t category = 1; category < kernel_memory_map_categories_count; ++category)
	{
		const uint32_t kernel_table_index_begin = kernel_table_index;
		for (UINTN i = 0; i < bytes_total / bytes_per_item; ++i)
		{
			auto const efi_entry = (const EFI_MEMORY_DESCRIPTOR*)(efi_table + i * bytes_per_item);
			if ((1 << efi_entry->Type) & (kernel_memory_map_category_flags[category]))
			{
				auto* const kernel_table_ptr = kernel_table + kernel_table_index;
				kernel_table_ptr->begin = efi_entry->PhysicalStart;
				kernel_table_ptr->pages = efi_entry->NumberOfPages;
				++kernel_table_index;
			}
		}
		kernel_data.memory_map_category_sizes[category] = kernel_table_index - kernel_table_index_begin;
	}

	kernel_data.memory_map = kernel_table;
}

int efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* system_table)
{
	ST = system_table;
	println("OpenCPProuter loader");

	fill_memory_map();

	ST->BootServices->ExitBootServices(handle, memory_map_key);
	
	using kernel_entry_func_t = void (__attribute__((sysv_abi))*)(kernel_data_t*);
	((kernel_entry_func_t)&KERNEL_PAYLOAD)(&kernel_data);

	return -1;
}

