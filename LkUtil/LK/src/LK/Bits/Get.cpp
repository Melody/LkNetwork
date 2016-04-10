#include"stdafx.h"
#ifdef LK_BITS_GET_
namespace LK{
	namespace Bits {
		bool Get(const void* pSource, index_t bitPosition){
			bitPosition += ((size_t)pSource&(sizeof(size_t) - 1)) * 8;
			size_t*ps = (size_t*)((size_t)pSource & (~(sizeof(size_t) - 1)));

			if (bitPosition < 0) { ps += (bitPosition - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); bitPosition &= (sizeof(size_t) * 8) - 1; }
			else if (bitPosition >= (index_t)(sizeof(size_t) * 8)) { ps += bitPosition / ((index_t)sizeof(size_t) * 8); bitPosition &= (sizeof(size_t) * 8) - 1; }

			return (*ps >> (bitPosition & (sizeof(size_t) * 8 - 1))) & 1;
		}
		size_t GetX(const void* pSource, index_t bitPosition)
		{
			bitPosition += ((size_t)pSource&(sizeof(size_t) - 1)) * 8;
			size_t* ps = (size_t*)((size_t)pSource & (~(sizeof(size_t) - 1)));
			if (bitPosition < 0) { ps += (bitPosition - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); bitPosition &= (sizeof(size_t) * 8) - 1; }
			else if (bitPosition >= (index_t)(sizeof(size_t) * 8)) { ps += bitPosition / ((index_t)sizeof(size_t) * 8); bitPosition &= (sizeof(size_t) * 8) - 1; }
			return bitPosition == 0 ? *ps : ((*ps >> bitPosition) | (*(ps + 1) << (sizeof(size_t) * 8 - bitPosition)));
		}
	}
}
#endif