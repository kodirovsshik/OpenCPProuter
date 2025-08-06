
#include <ocppr/init.hpp>
#include <ocppr/defs.hpp>
#include <ocppr/arch/defs.hpp>
#include <ocppr/hash.hpp>

#define DEFINE_SEGMENT(s) LD_SYMBOL(s##_segment_begin) LD_SYMBOL_ABSOLUTE(s##_segment_size)

DEFINE_SEGMENT(init)
DEFINE_SEGMENT(executeable)
DEFINE_SEGMENT(rodata)
DEFINE_SEGMENT(rwdata)



extern "C" void init_get_entropy16(u8*);
extern "C" void init_switch_to_virtual_addresses(ptrdiff);

void mmap_addr(uptr, uptr, usize, u8)
{
	
}

namespace init
{
	SECTION(.init) uptr kernel_space_begin();
	
	SECTION(.init) u64 rand_offset()
	{
		u8 entropy[16];
		init_get_entropy16(entropy);
		constexpr u64 rng_mask = ((u64)1 << KERNEL_LOAD_RANDOM_BITS) - 1;
		const u64 hash = fnv1a_64(entropy, sizeof(entropy)) & rng_mask; //TODO: use top bits instead?
		return hash << PAGE_SHIFT;
	}
	
	SECTION(.init) void kernel_remap(const kernel_args_t& args)
	{
		const uptr phys_base_addr = args.base_phy_addr;
		const uptr virt_base_addr = kernel_space_begin() + rand_offset();

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

		asm volatile ("hlt");
	}
}
