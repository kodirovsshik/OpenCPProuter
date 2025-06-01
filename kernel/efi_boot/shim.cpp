
#include "efi.h"

#define stringify1(x) #x
#define stringify(x) stringify1(x)

alignas(4096) const unsigned char KERNEL_PAYLOAD[] =
{
#embed stringify(KERNEL_BIN)
};

static_assert(sizeof(KERNEL_PAYLOAD) >= 5);

struct memory_map_entry_t
{
	uint64_t base;
	uint64_t size;
};

struct kernel_args_t
{
	struct
	{
		memory_map_entry_t* memory_useable = nullptr;
		uint32_t memory_useable_count;		
	} memory_map;
	
} kernel_args;

EFI_STATUS efi_main(EFI_HANDLE, EFI_SYSTEM_TABLE* ST)
{
	println("OpenCPProuter loader");

	using kernel_entry_func_t = void(*)(kernel_args_t*);
	((kernel_entry_func_t)&KERNEL_PAYLOAD)(&kernel_args);
	
	return 0;
}

