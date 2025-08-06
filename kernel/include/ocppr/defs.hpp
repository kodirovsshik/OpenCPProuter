
#include <ocppr/int.hpp>

struct ld_symbol_type;

#define NOMANGLE extern "C"
#define SECTION(s) __attribute__((section(#s)))
#define LD_SYMBOL(s) extern "C" ld_symbol_type _##s; static uptr s(){ return (uptr)&_##s; }
#define LD_SYMBOL_ABSOLUTE(s) extern "C" ld_symbol_type _##s; static uptr s(){ return (uptr)&_##s - (uptr)&_kernel_space_begin; }

extern "C" ld_symbol_type _kernel_space_begin;
