#include"stdafx.h"
#ifdef LK_BITS_COPY_
namespace LK{
	namespace Bits {
		void Copy64(u64* pDest, s64 destStartBitPositon, cu64* pSource, s64 sourceStartBitPositon, u64 bitsCount)
		{
			if (bitsCount == 0)return;
			if (sourceStartBitPositon < 0) { pSource += (sourceStartBitPositon - 63) / 64; sourceStartBitPositon &= 63; }
			else if (sourceStartBitPositon >= 64) { pSource += sourceStartBitPositon / 64; sourceStartBitPositon &= 63; }
			if (destStartBitPositon < 0) { pDest += (destStartBitPositon - 63) / 64; destStartBitPositon &= 63; }
			else if (destStartBitPositon >= 64) { pDest += destStartBitPositon / 64; destStartBitPositon &= 63; }
			if (sourceStartBitPositon == 0)
			{
				if (destStartBitPositon == 0)
				{
					u64* pEnd = (bitsCount >> 6) + pDest;
					while (pDest != pEnd)*pDest++ = *pSource++;
					if ((bitsCount &= 63) != 0)*pDest = (*pSource&(((u64)-1) >> (64 - bitsCount))) | (*pDest&(((u64)-1) << bitsCount));
					return;
				}
				u64* pEnd = ((bitsCount + destStartBitPositon) >> 6) + pDest;
				if (pEnd == pDest)
				{
					*pDest = (*pDest &(((((u64)-1) << (bitsCount + destStartBitPositon)) | (((u64)-1) >> (64 - destStartBitPositon)))))
						| ((*pSource&(((u64)-1) >> (64 - bitsCount))) << destStartBitPositon);
					return;
				}
				*pDest = (*pSource << destStartBitPositon) | (*pDest&(((u64)-1) >> (64 - destStartBitPositon)));
				while (++pDest != pEnd) { *pDest = (*pSource >> (64 - destStartBitPositon)) | (*(pSource + 1) << destStartBitPositon); ++pSource; }
				if ((bitsCount = (destStartBitPositon + bitsCount) & 63) != 0)
				{
					u64 tmp = (*pSource >> (64 - destStartBitPositon));
					if (bitsCount > (u64)destStartBitPositon)tmp |= *(pSource + 1) << destStartBitPositon;
					*pDest = (*pDest&(((u64)-1) << bitsCount)) | (tmp&(((u64)-1) >> (64 - bitsCount)));
				}
				return;
			}
			if (destStartBitPositon == 0)
			{
				u64* pEnd = (bitsCount >> 6) + pDest;
				while (pEnd != pDest)
				{
					*pDest++ = (*pSource >> sourceStartBitPositon) | (pSource[1] << (64 - sourceStartBitPositon));
					++pSource;
				}
				if ((bitsCount &= 63) != 0)
				{
					u64 tmp = *pSource >> sourceStartBitPositon;
					if (bitsCount + sourceStartBitPositon > 64)tmp |= (pSource[1] << (64 - sourceStartBitPositon));
					*pDest = (*pDest & (((u64)-1) << bitsCount)) | (tmp&(((u64)-1) >> (64 - bitsCount)));
				}
				return;
			}
			u64* pEnd = ((bitsCount + destStartBitPositon) >> 6) + pDest;
			if (pEnd == pDest)
			{
				u64 tmp = *pSource >> sourceStartBitPositon;
				if (bitsCount + sourceStartBitPositon > 64)tmp |= (pSource[1] << (64 - sourceStartBitPositon));
				*pDest = (*pDest &(((((u64)-1) << (bitsCount + destStartBitPositon)) | (((u64)-1) >> (64 - destStartBitPositon)))))
					| ((tmp&(((u64)-1) >> (64 - bitsCount))) << destStartBitPositon);
				return;
			}
			*pDest = (((*pSource >> sourceStartBitPositon) | (pSource[1] << (64 - sourceStartBitPositon))) << destStartBitPositon) | (*pDest&(((u64)-1) >> (64 - destStartBitPositon)));
			if (sourceStartBitPositon == destStartBitPositon)
			{
				while (++pDest != pEnd) { *pDest = *++pSource; }
				if ((bitsCount = (destStartBitPositon + bitsCount) & 63) != 0)
				{
					*pDest = (*pDest&(((u64)-1) << bitsCount)) | ((*(pSource + 1))&(((u64)-1) >> (64 - bitsCount)));
				}
				return;
			}
			sourceStartBitPositon -= destStartBitPositon;
			if (sourceStartBitPositon < 0)
			{
				sourceStartBitPositon = -sourceStartBitPositon;
				while (++pDest != pEnd)
				{
					*pDest = (*pSource >> (64 - sourceStartBitPositon)) | (*(pSource + 1) << sourceStartBitPositon);
					++pSource;
				}
				if ((bitsCount = (destStartBitPositon + bitsCount) & 63) != 0)
				{
					u64 tmp = *pSource >> (64 - sourceStartBitPositon);
					if (bitsCount > (u64)sourceStartBitPositon)tmp |= pSource[1] << sourceStartBitPositon;
					*pDest = (*pDest&(((u64)-1) << bitsCount)) | (tmp&(((u64)-1) >> (64 - bitsCount)));
				}
				return;
			}
			while (++pDest != pEnd)
			{
				++pSource;
				*pDest = (*pSource >> sourceStartBitPositon) | (*(pSource + 1) << (64 - sourceStartBitPositon));
			}
			if ((bitsCount = (destStartBitPositon + bitsCount) & 63) != 0)
			{
				u64 tmp = pSource[1] >> sourceStartBitPositon;
				if (bitsCount + sourceStartBitPositon > 64)tmp |= pSource[2] << (64 - sourceStartBitPositon);
				*pDest = (*pDest&(((u64)-1) << bitsCount)) | (tmp&(((u64)-1) >> (64 - bitsCount)));
			}
		}
		void Copy32(u32* pDest, s32 destStartBitPositon, cu32* pSource, s32 sourceStartBitPositon, u32 bitsCount)
		{
			if (bitsCount == 0)return;
			if (sourceStartBitPositon < 0) { pSource += (sourceStartBitPositon - 31) / 32; sourceStartBitPositon &= 31; }
			else if (sourceStartBitPositon >= 32) { pSource += sourceStartBitPositon / 32; sourceStartBitPositon &= 31; }
			if (destStartBitPositon < 0) { pDest += (destStartBitPositon - 31) / 32; destStartBitPositon &= 31; }
			else if (destStartBitPositon >= 32) { pDest += destStartBitPositon / 32; destStartBitPositon &= 31; }
			if (sourceStartBitPositon == 0)
			{
				if (destStartBitPositon == 0)
				{
					u32* pEnd = (bitsCount >> 5) + pDest;
					while (pDest != pEnd)*pDest++ = *pSource++;
					if ((bitsCount &= 31) != 0)*pDest = (*pSource&(((u32)-1) >> (32 - bitsCount))) | (*pDest&(((u32)-1) << bitsCount));
					return;
				}
				u32* pEnd = ((bitsCount + destStartBitPositon) >> 5) + pDest;
				if (pEnd == pDest)
				{
					*pDest = (*pDest &(((((u32)-1) << (bitsCount + destStartBitPositon)) | (((u32)-1) >> (32 - destStartBitPositon)))))
						| ((*pSource&(((u32)-1) >> (32 - bitsCount))) << destStartBitPositon);
					return;
				}
				*pDest = (*pSource << destStartBitPositon) | (*pDest&(((u32)-1) >> (32 - destStartBitPositon)));
				while (++pDest != pEnd) { *pDest = (*pSource >> (32 - destStartBitPositon)) | (*(pSource + 1) << destStartBitPositon); ++pSource; }
				if ((bitsCount = (destStartBitPositon + bitsCount) & 31) != 0)
				{
					u32 tmp = (*pSource >> (32 - destStartBitPositon));
					if (bitsCount > (u64)destStartBitPositon)tmp |= *(pSource + 1) << destStartBitPositon;
					*pDest = (*pDest&(((u32)-1) << bitsCount)) | (tmp&(((u32)-1) >> (32 - bitsCount)));
				}
				return;
			}
			if (destStartBitPositon == 0)
			{
				u32* pEnd = (bitsCount >> 5) + pDest;
				while (pEnd != pDest)
				{
					*pDest++ = (*pSource >> sourceStartBitPositon) | (pSource[1] << (32 - sourceStartBitPositon));
					++pSource;
				}
				if ((bitsCount &= 31) != 0)
				{
					u32 tmp = *pSource >> sourceStartBitPositon;
					if (bitsCount + sourceStartBitPositon > 32)tmp |= (pSource[1] << (32 - sourceStartBitPositon));
					*pDest = (*pDest & (((u32)-1) << bitsCount)) | (tmp&(((u32)-1) >> (32 - bitsCount)));
				}
				return;
			}
			u32* pEnd = ((bitsCount + destStartBitPositon) >> 5) + pDest;
			if (pEnd == pDest)
			{
				u32 tmp = *pSource >> sourceStartBitPositon;
				if (bitsCount + sourceStartBitPositon > 32)tmp |= (pSource[1] << (32 - sourceStartBitPositon));
				*pDest = (*pDest &(((((u32)-1) << (bitsCount + destStartBitPositon)) | (((u32)-1) >> (32 - destStartBitPositon)))))
					| ((tmp&(((u32)-1) >> (32 - bitsCount))) << destStartBitPositon);
				return;
			}
			*pDest = (((*pSource >> sourceStartBitPositon) | (pSource[1] << (32 - sourceStartBitPositon))) << destStartBitPositon) | (*pDest&(((u32)-1) >> (32 - destStartBitPositon)));
			if (sourceStartBitPositon == destStartBitPositon)
			{
				while (++pDest != pEnd) { *pDest = *++pSource; }
				if ((bitsCount = (destStartBitPositon + bitsCount) & 31) != 0)
				{
					*pDest = (*pDest&(((u32)-1) << bitsCount)) | ((*(pSource + 1))&(((u32)-1) >> (32 - bitsCount)));
				}
				return;
			}
			sourceStartBitPositon -= destStartBitPositon;
			if (sourceStartBitPositon < 0)
			{
				sourceStartBitPositon = -sourceStartBitPositon;
				while (++pDest != pEnd)
				{
					*pDest = (*pSource >> (32 - sourceStartBitPositon)) | (*(pSource + 1) << sourceStartBitPositon);
					++pSource;
				}
				if ((bitsCount = (destStartBitPositon + bitsCount) & 31) != 0)
				{
					u32 tmp = *pSource >> (32 - sourceStartBitPositon);
					if (bitsCount > (u64)sourceStartBitPositon)tmp |= pSource[1] << sourceStartBitPositon;
					*pDest = (*pDest&(((u32)-1) << bitsCount)) | (tmp&(((u32)-1) >> (32 - bitsCount)));
				}
				return;
			}
			while (++pDest != pEnd)
			{
				++pSource;
				*pDest = (*pSource >> sourceStartBitPositon) | (*(pSource + 1) << (32 - sourceStartBitPositon));
			}
			if ((bitsCount = (destStartBitPositon + bitsCount) & 31) != 0)
			{
				u32 tmp = pSource[1] >> sourceStartBitPositon;
				if (bitsCount + sourceStartBitPositon > 32)tmp |= pSource[2] << (32 - sourceStartBitPositon);
				*pDest = (*pDest&(((u32)-1) << bitsCount)) | (tmp&(((u32)-1) >> (32 - bitsCount)));
			}
		}
		void Copy(void* pDest, index_t destStartBitPositon, const void* pSource, index_t sourceStartBitPositon, size_t bitsCount) {
			destStartBitPositon += ((size_t)pDest&(sizeof(size_t) - 1)) * 8;
			size_t* pd = (size_t*)((size_t)pDest & (~(sizeof(size_t) - 1)));
			sourceStartBitPositon += ((size_t)pSource&(sizeof(size_t) - 1)) * 8;
			size_t* ps = (size_t*)((size_t)pSource & (~(sizeof(size_t) - 1)));
			if (destStartBitPositon < 0) { pd += (destStartBitPositon - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); destStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			else if (destStartBitPositon >= (index_t)(sizeof(size_t) * 8)) { pd += destStartBitPositon / ((index_t)sizeof(size_t) * 8); destStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			if (sourceStartBitPositon < 0) { ps += (sourceStartBitPositon - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); sourceStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			else if (sourceStartBitPositon >= (index_t)(sizeof(size_t) * 8)) { ps += sourceStartBitPositon / ((index_t)sizeof(size_t) * 8); sourceStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			if (sizeof(size_t) == 8)Copy64((u64*)pd, destStartBitPositon, (u64*)ps, sourceStartBitPositon, bitsCount);
			else Copy32((u32*)pd, (long)destStartBitPositon, (u32*)ps, (long)sourceStartBitPositon, (u32)bitsCount);
		}

	}
}
#endif