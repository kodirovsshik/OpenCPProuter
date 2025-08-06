
#include <efi.h>
#undef __attribute__ //fuck gnu-efi

#define efistr(x) (wchar_t*)(L ## x)
#define println(x)												\
	{															\
		ST->ConOut->OutputString(ST->ConOut, efistr(x));		\
		ST->ConOut->OutputString(ST->ConOut, efistr("\r\n"));	\
	}
#define wait_kb()                                                       \
    {                                                                   \
        EFI_INPUT_KEY Key;                                              \
        ST->ConIn->Reset(ST->ConIn, FALSE);                             \
        while (ST->ConIn->ReadKeyStroke(ST->ConIn, &Key) == EFI_NOT_READY); \
    }
#define stop(msg, code)                             \
    {                                               \
        println(msg);                               \
        println("\r\nPress any key to continue");   \
        wait_kb();                                  \
        return code;                                \
    }
#define eficheck(x, msg)                            \
    {                                               \
        EFI_STATUS Status = (x);                    \
        if (EFI_ERROR(Status)) stop(msg, Status);   \
    }

struct EFI_MEMORY_TYPE_CUSTOM
{
	enum
	{
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

