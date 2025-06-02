
#include "efi.h"

#define stringify1(x) #x
#define stringify(x) stringify1(x)

#define countof(arr) (sizeof(arr)/sizeof(*(arr)))

alignas(4096) const unsigned char KERNEL_PAYLOAD[] =
{
#embed stringify(KERNEL_BIN)
};


struct EFI_MEMORY_TYPE_CUSTOM
{
	enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
		EfiUnacceptedMemoryType,
		EfiMaxMemoryType
	};
};


struct kernel_data
{
	struct memory_map_entry
	{
		uint64_t begin;
		uint64_t end;
	};

	struct memory_map_category_descriptor
	{
		uint32_t begin;
		uint32_t end;
	};

	static constexpr UINT32 memory_map_category_flags[] =
		{
			//invalid category
			0,

			//useable after bootloader trasnfers control to kernel
			(1 << EfiConventionalMemory) |
			(1 << EFI_MEMORY_TYPE_CUSTOM::EfiPersistentMemory) |
			(1 << EfiLoaderCode) |
			(1 << EfiLoaderData) |
			(1 << EfiBootServicesCode) |
			(1 << EfiBootServicesData),

			//useable after ACPI tables are parsed
			(1 << EfiACPIReclaimMemory),
		};

	static constexpr uint32_t memory_categories_count = countof(memory_map_category_flags);

	
	uint32_t magic = 0xC1A79A76;
	uint32_t memory_map_descriptors_count;
	memory_map_entry* memory_map_ranges;
	memory_map_category_descriptor memory_map_descriptors[memory_categories_count];
	void* acpi_ptr = nullptr;
	
} kernel_args{};



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

	ST->BootServices->GetMemoryMap(&bytes_total, (EFI_MEMORY_DESCRIPTOR*)efi_table, &memory_map_key, &bytes_per_item, &version);

	bytes_total += 4 * bytes_per_item;
	const UINTN max_entries_total = bytes_total / bytes_per_item + 4;
	
	efi_table = alloc<UINT8>(bytes_total);
	auto* kernel_table = alloc<kernel_data::memory_map_entry>(max_entries_total);

	ST->BootServices->GetMemoryMap(&bytes_total, (EFI_MEMORY_DESCRIPTOR*)efi_table, &memory_map_key, &bytes_per_item, &version);

	for (uint32_t category = 1; category < kernel_data::memory_categories_count; ++category)
		{
			for (UINTN bytes_left = bytes_total; bytes_left >= bytes_per_item; bytes_left -= bytes_per_item)
				{
					auto const entry = (const EFI_MEMORY_DESCRIPTOR*)&efi_table;
					
				}
			
		}

}

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* system_table)
{
	ST = system_table;
	println("OpenCPProuter loader");

	ST->BootServices->ExitBootServices(handle, memory_map_key);
	
	using kernel_entry_func_t = void(*)(kernel_data*);
	((kernel_entry_func_t)&KERNEL_PAYLOAD)(&kernel_args);

	return -1;
}

