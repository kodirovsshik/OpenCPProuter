
#include <ocppr/int.hpp>

u64 fnv1a_64(const u8* p, usize sz)
{
	u64 result = 14695981039346656037u;
	while (sz --> 0)
		result = (result ^ *p++) * 1099511628211;
	return result;
}
