
#include <efi.h>

#define efistr(x) (unsigned short*)(L ## x)
#define println(x) ST->ConOut->OutputString(ST->ConOut, efistr(x L"\r\n"))
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
