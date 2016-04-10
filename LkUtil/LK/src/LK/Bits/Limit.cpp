#include"stdafx.h"
#ifdef LK_BITS_LIMIT_
namespace LK{
	namespace Bits {
		bool SelfLimitBitsRoundHigh64(u64* p, u64 len, s64 mov)
		{
			if (mov > 0) { for (size_t i = 0; i < len; ++i)p[i] = 0; return false; }
			mov = len * 64 + mov;
			if (mov <= 0)return false;
			size_t i = (size_t)(mov / 64);
			mov &= 63;
			u64 ull = mov == 0 ? ((s64)p[i - 1] < 0) : ((p[i] & (1ull << (mov - 1))) << 1);
			for (size_t k = 0; k < i; ++k)p[k] = 0;
			if (mov != 0)p[i] = (p[i] >> mov) << mov;
			while (i < len)
			{
				if (ull != 0)
					ull = (p[i] += ull) == 0;
				else return false;
				++i;
			}
			return ull != 0;
		}
	}
}
#endif