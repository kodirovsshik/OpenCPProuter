
#include <ocppr/init.hpp>
#include <ocppr/defs.hpp>
#include <ocppr/hash.hpp>
#include <ocppr/memfunc.hpp>
#include <ocppr/log.hpp>

#define DEFINE_SEGMENT(s) LD_SYMBOL(s##_segment_begin) LD_SYMBOL_ABSOLUTE(s##_segment_size)

DEFINE_SEGMENT(init)
DEFINE_SEGMENT(executeable)
DEFINE_SEGMENT(rodata)
DEFINE_SEGMENT(rwdata)

#define debuglog serial_puts
void halt();

extern "C" void init_get_entropy16(u8*);
extern "C" void init_switch_to_virtual_addresses(ptrdiff);
extern "C" const u8 kernel_entry;

void mmap_addr(uptr, uptr, usize, u8);

namespace init
{
	SECTION(.init) u64 rand_offset()
	{
		u8 entropy[16];
		init_get_entropy16(entropy);

		constexpr u64 rng_mask = ((u64)1 << (KERNEL_LOAD_RANDOM_BITS)) - 1;

		const u64 hash = fnv1a_64(entropy, sizeof(entropy)) & rng_mask; //TODO: use top bits instead?
		return hash << PAGE_SHIFT;
	}

	SECTION(.init) void validate_loader_data(const kernel_args_t& args)
	{
		if (memcmp(args.preamble.value, "OCPPR", 6) != 0)
		{
			debuglog("kernel data invalig signature");
			halt();
		}
		
	}

	SECTION(.init) void num_puts(u64 x, const u8 base, u8 width = 1, const u8 pad_char = '0')
	{
		char buf[72];
		char* p = buf + sizeof(buf);
		while (x)
		{
			*--p = x % base;
			x /= base;
			if (width) --width;
		}
		while (width --> 0)
			outb(pad_char, 0x3F8);
		serial_puts(p);
	}
	
	SECTION(.init) void kernel_init(const kernel_args_t& args)
	{
		serial_puts("OpenCPProuter!\r\n");
		serial_puts("loaded at 0x");
		num_puts(kernel_space_begin(), 16, 16);
		serial_puts("\r\n");
		
		validate_loader_data(args);

		(void)args;
		const uptr phys_base_addr = (uptr)&kernel_entry;
		const uptr virt_base_addr = KERNEL_SPACE_BEGIN + rand_offset();

		auto map_segment = [&]
		(uptr addr, usize sz, u8 flags)
		{
			const usize off = addr - phys_base_addr;
			mmap_addr(virt_base_addr + off, addr, sz, flags);
		};
		
		map_segment(init_segment_begin(), init_segment_size(), 3);
		map_segment(executeable_segment_begin(), executeable_segment_size(), 1);
		map_segment(rwdata_segment_begin(), rwdata_segment_size(), 2);
		map_segment(rodata_segment_begin(), rodata_segment_size(), 0);

		init_switch_to_virtual_addresses(virt_base_addr - phys_base_addr);

		halt();
	}
}
