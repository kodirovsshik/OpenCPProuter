
#include <cstddef>

#define ATTR(attr) __attribute__((attr))
#define SECTION(name) ATTR(section(#name))
#define LINKER_SYMBOL extern "C" const std::byte
#define WEAK inline
#define HIDDEN static
