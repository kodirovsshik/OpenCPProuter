
#ifndef _GNU_EFI
#include <stdint.h>
#include <stddef.h>
#endif

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using iptr = intptr_t;
using uptr = uintptr_t;

static_assert(sizeof(size_t) == sizeof(uptr));
using isize = iptr;
using usize = uptr;
using ptrdiff = isize;
