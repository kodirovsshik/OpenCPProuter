
#include <ocppr/int.hpp>
#include <ocppr/defs.hpp>

namespace init
{
	SECTION(.init) uptr kernel_space_begin()
	{
		return 0xFFFFFFFF80000000;
	}
}
