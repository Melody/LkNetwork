#include"stdafx.h"
#ifdef LK_BITS_SET_
namespace LK{
	namespace Bits {
		void Set(void* pp, index_t startBitPosition, size_t bitsCount, bool bitValue)//将p所指向的内存的startBitPosition比特起的bitsCount比特置为bit的值
		{
#ifdef _DEBUG
			if ((index_t)bitsCount < 0)Throw<::std::exception >("比特数异常");
#endif
			if (bitsCount == 0)return;
			startBitPosition += ((size_t)pp&(sizeof(size_t) - 1)) * 8;
			size_t* p = (size_t*)((size_t)pp&(~(sizeof(size_t) - 1)));
			if (startBitPosition < 0) { p += (startBitPosition - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); startBitPosition &= (sizeof(size_t) * 8) - 1; }
			else if (startBitPosition >= (index_t)(sizeof(size_t) * 8)) { p += startBitPosition / ((index_t)sizeof(size_t) * 8); startBitPosition &= (sizeof(size_t) * 8) - 1; }

			size_t allBits = bitsCount + startBitPosition;
			if (allBits <= sizeof(size_t) * 8)
			{
				if (bitValue) { *p |= (((size_t)-1) >> ((sizeof(size_t) * 8) - bitsCount)) << startBitPosition; return; }
				*p &= ~((((size_t)-1) >> ((sizeof(size_t) * 8) - bitsCount)) << startBitPosition); return;
			}
			if (bitValue) { *p |= ((size_t)-1) << startBitPosition; }
			else *p &= ~(((size_t)-1) << startBitPosition);
			while (allBits > (sizeof(size_t) * 16))
			{
				allBits -= (sizeof(size_t) * 8);
				*++p = bitValue ? -1 : 0;
			}
			if (allBits == 0)return;
			if (bitValue) { *(p + 1) |= ((size_t)-1) >> ((sizeof(size_t) * 8) - allBits); }
			else *(p + 1) &= ~(((size_t)-1) >> ((sizeof(size_t) * 8) - allBits));
		}
	}
}
#endif