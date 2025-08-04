
#include <ocppr/init.hpp>
#include <ocppr/defs.hpp>


namespace init
{
	SECTION(.init) void kernel_init(const kernel_args_t&)
	{
	}
	

//	LD_SYMBOL(kernel_space_begin);
	LD_SYMBOL(init_section_begin);

	SECTION(.init) uptr kernel_remap(const kernel_args_t& args)
	{
//		const uptr virtual_base = kernel_space_begin;
		const uptr virtual_base = init_section_begin;
		const uptr rand_offset = 0;
		const uptr func_offset = (uptr)&kernel_init - args.base_phy_addr;
		return virtual_base + rand_offset + func_offset;
		//return virtual_base + rand_offset + (func_address - image_base)
	}
}
