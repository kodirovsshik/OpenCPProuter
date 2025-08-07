
#ifndef _OCPPR_DEFS_HPP_
#define _OCPPR_DEFS_HPP_

#include <ocppr/int.hpp>
#include <ocppr/arch/defs.hpp>

struct byte;

#define NOMANGLE extern "C"
#define SECTION(s) __attribute__((section(#s)))
#define LD_SYMBOL(s) extern "C" byte _##s; static uptr s(){ return (uptr)&_##s; }
#define LD_SYMBOL_ABSOLUTE(s) extern "C" byte _##s; static uptr s(){ return (uptr)&_##s - (uptr)&_kernel_space_begin; }

LD_SYMBOL(kernel_space_begin)

#endif
