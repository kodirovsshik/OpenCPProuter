
#include <ocppr/int.hpp>

struct ld_symbol_type;

#define NOMANGLE extern "C"
#define SECTION(s) __attribute__((section(#s)))
#define LD_SYMBOL(s) extern "C" ld_symbol_type _##s; static const uptr s = (uptr)&_##s;


