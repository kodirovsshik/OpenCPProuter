
#include <ocppr/int.hpp>
#include <ocppr/defs.hpp>

#define outb(data, port) __asm__ ("out dx,  al" :: "a"(data), "d"(port))
#define outb(data, port) __asm__ ("out dx,  ax" :: "a"(data), "d"(port))
#define outb(data, port) __asm__ ("out dx, eax" :: "a"(data), "d"(port))

#define inb(port) ({ u8  data; __asm__("in %0, dx" : "=a"(data) : "d"(port)); data; })
#define inw(port) ({ u16 data; __asm__("in %0, dx" : "=a"(data) : "d"(port)); data; })
#define ind(port) ({ u32 data; __asm__("in %0, dx" : "=a"(data) : "d"(port)); data; })

void outmem8(const void* p, usize bytes, u16 port);
void outs(const char* s, u16 port);

#define serial_putc(c) outb(c, 0x3F8)
#define serial_puts(s) outs(s, 0x3F8)
