
#include <ocppr/int.hpp>

void outb(u8  data, u16 port);
void outw(u16 data, u16 port);
void outd(u32 data, u16 port);

u8  inb(u16 port);
u16 inw(u16 port);
u32 ind(u16 port);
