#include"stdafx.h"
#ifdef LK_BITS_MOVE_
namespace LK{
	namespace Bits {
		void Move(void* pDest, index_t destStartBitPositon, void const* pSource, index_t sourceStartBitPositon, size_t bitsCount)
		{
			destStartBitPositon += ((size_t)pDest&(sizeof(size_t) - 1)) * 8;
			size_t* pd = (size_t*)((size_t)pDest & (~(sizeof(size_t) - 1)));
			sourceStartBitPositon += ((size_t)pSource&(sizeof(size_t) - 1)) * 8;
			size_t* ps = (size_t*)((size_t)pSource & (~(sizeof(size_t) - 1)));
			if (destStartBitPositon < 0) { pd += (destStartBitPositon - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); destStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			else if (destStartBitPositon >= (index_t)(sizeof(size_t) * 8)) { pd += destStartBitPositon / ((index_t)sizeof(size_t) * 8); destStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			if (sourceStartBitPositon < 0) { ps += (sourceStartBitPositon - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); sourceStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			else if (sourceStartBitPositon >= (index_t)(sizeof(size_t) * 8)) { ps += sourceStartBitPositon / ((index_t)sizeof(size_t) * 8); sourceStartBitPositon &= (sizeof(size_t) * 8) - 1; }
			if (sizeof(size_t) == 8)Move64((u64*)pd, destStartBitPositon, (u64*)ps, sourceStartBitPositon, bitsCount);
			else Move32((u32*)pd, (long)destStartBitPositon, (u32*)ps, (long)sourceStartBitPositon, (u32)bitsCount);
		}
		void Move64(u64* pDest, s64 destStartBitPositon, cu64* pSource, s64 sourceStartBitPositon, u64 bitsCount)
		{
			if (bitsCount == 0)return;
			if (sourceStartBitPositon < 0) { pSource += (sourceStartBitPositon - 63) / 64; sourceStartBitPositon &= 63; }
			else if (sourceStartBitPositon >= 64) { pSource += sourceStartBitPositon / 64; sourceStartBitPositon &= 63; }
			if (destStartBitPositon < 0) { pDest += (destStartBitPositon - 63) / 64; destStartBitPositon &= 63; }
			else if (destStartBitPositon >= 64) { pDest += destStartBitPositon / 64; destStartBitPositon &= 63; }
			if (pDest <= pSource)
			{
				if (pDest < pSource || sourceStartBitPositon > destStartBitPositon) { Copy64(pDest, destStartBitPositon, pSource, sourceStartBitPositon, bitsCount); return; }
				if (sourceStartBitPositon == destStartBitPositon)return;
			}
			if (sourceStartBitPositon == 0)
			{
				if (destStartBitPositon == 0)
				{
					u64 bc = bitsCount & 63;
					u64 bm = bitsCount >> 6;
					u64* pEnd = bm + pDest;
					pSource += bm;
					if (bc != 0)*pEnd = (*pSource&(((u64)-1) >> (64 - bc))) | (*pEnd&(((u64)-1) << bc));
					while (pDest != pEnd)*--pEnd = *--pSource;
					return;
				}
				u64 bm = (destStartBitPositon + bitsCount) >> 6;
				u64* pEnd = bm + pDest;
				if (pEnd == pDest)
				{
					*pDest = (*pDest &(((((u64)-1) << (bitsCount + destStartBitPositon)) | (((u64)-1) >> (64 - destStartBitPositon)))))
						| ((*pSource&(((u64)-1) >> (64 - bitsCount))) << destStartBitPositon);
					return;
				}
				u64 bc = (destStartBitPositon + bitsCount) & 63;
				pSource += bm - 1;
				if (bc != 0)
				{
					u64 tmp = (*pSource >> (64 - destStartBitPositon));
					if (bc > (u64)destStartBitPositon)tmp |= pSource[1] << destStartBitPositon;
					*pEnd = (*pEnd&(((u64)-1) << bc)) | (tmp&(((u64)-1) >> (64 - bc)));
				}
				while (pDest != --pEnd) { --pSource; *pEnd = (*pSource >> (64 - destStartBitPositon)) | (*(pSource + 1) << destStartBitPositon); }
				*pDest = (*(pSource) << destStartBitPositon) | (*pDest&(((u64)-1) >> (64 - destStartBitPositon)));
				return;
			}
			if (destStartBitPositon == 0)
			{
				u64 bc = bitsCount & 63;
				u64 bm = bitsCount >> 6;
				u64* pEnd = bm + pDest;
				pSource += bm;
				if (bc != 0)
				{
					u64 tmp = *pSource >> sourceStartBitPositon;
					if (bc + sourceStartBitPositon > 64)tmp |= (pSource[1] << (64 - sourceStartBitPositon));
					*pEnd = (*pEnd & (((u64)-1) << bc)) | (tmp&(((u64)-1) >> (64 - bc)));
				}
				while (pEnd != pDest)
				{
					*--pEnd = (*(pSource - 1) >> sourceStartBitPositon) | (*pSource << (64 - sourceStartBitPositon));
					--pSource;
				}
				return;
			}
			u64 bm = (bitsCount + destStartBitPositon) >> 6;
			u64* pEnd = bm + pDest;
			if (pEnd == pDest)
			{
				u64 tmp = *pSource >> sourceStartBitPositon;
				if (bitsCount + sourceStartBitPositon > 64)tmp |= (pSource[1] << (64 - sourceStartBitPositon));
				*pDest = (*pDest &(((((u64)-1) << (bitsCount + destStartBitPositon)) | (((u64)-1) >> (64 - destStartBitPositon)))))
					| ((tmp&(((u64)-1) >> (64 - bitsCount))) << destStartBitPositon);
				return;
			}
			u64 bc = (bitsCount + destStartBitPositon) & 63;
			pSource += bm - 1;
			if (sourceStartBitPositon == destStartBitPositon)
			{
				if (bc != 0)
				{
					*pEnd = (*pEnd&(((u64)-1) << bc)) | ((*(pSource + 1))&(((u64)-1) >> (64 - bc)));
				}
				while (pDest != --pEnd) { *pEnd = *pSource--; }
			}
			else {
				s64 smov = sourceStartBitPositon - destStartBitPositon;
				if (smov < 0)
				{
					smov = -smov;
					if (bc != 0)
					{
						u64 tmp = *pSource >> (64 - smov);
						if (bc >(u64)smov)tmp |= pSource[1] << smov;
						*pEnd = (*pEnd&(((u64)-1) << bc)) | (tmp&(((u64)-1) >> (64 - bc)));
					}
					while (pDest != --pEnd)
					{
						--pSource;
						*pEnd = (*pSource >> (64 - smov)) | (pSource[1] << smov);
					}
				}
				else {
					if (bc != 0)
					{
						u64 tmp = pSource[1] >> smov;
						if (bc + smov > 64)tmp |= pSource[2] << (64 - smov);
						*pEnd = (*pEnd&(((u64)-1) << bc)) | (tmp&(((u64)-1) >> (64 - bc)));
					}
					while (pDest != --pEnd)
					{
						*pEnd = (*pSource >> smov) | (*(pSource + 1) << (64 - smov));
						--pSource;
					}
				}
			}
			*pDest = (((*pSource >> sourceStartBitPositon) | (pSource[1] << (64 - sourceStartBitPositon))) << destStartBitPositon) | (*pDest&(((u64)-1) >> (64 - destStartBitPositon)));
		}
		void Move32(u32* pDest, s32 destStartBitPositon, cu32* pSource, s32 sourceStartBitPositon, u32 bitsCount)
		{
			if (bitsCount == 0)return;
			if (sourceStartBitPositon < 0) { pSource += (sourceStartBitPositon - 31) / 32; sourceStartBitPositon &= 31; }
			else if (sourceStartBitPositon >= 32) { pSource += sourceStartBitPositon / 32; sourceStartBitPositon &= 31; }
			if (destStartBitPositon < 0) { pDest += (destStartBitPositon - 31) / 32; destStartBitPositon &= 31; }
			else if (destStartBitPositon >= 32) { pDest += destStartBitPositon / 32; destStartBitPositon &= 31; }
			if (pDest <= pSource)
			{
				if (pDest < pSource || sourceStartBitPositon > destStartBitPositon) { Copy32(pDest, destStartBitPositon, pSource, sourceStartBitPositon, bitsCount); return; }
				if (sourceStartBitPositon == destStartBitPositon)return;
			}
			if (sourceStartBitPositon == 0)
			{
				if (destStartBitPositon == 0)
				{
					u32 bc = bitsCount & 31;
					u32 bm = bitsCount >> 5;
					u32* pEnd = bm + pDest;
					pSource += bm;
					if (bc != 0)*pEnd = (*pSource&(((u32)-1) >> (32 - bc))) | (*pEnd&(((u32)-1) << bc));
					while (pDest != pEnd)*--pEnd = *--pSource;
					return;
				}
				u32 bm = (destStartBitPositon + bitsCount) >> 5;
				u32* pEnd = bm + pDest;
				if (pEnd == pDest)
				{
					*pDest = (*pDest &(((((u32)-1) << (bitsCount + destStartBitPositon)) | (((u32)-1) >> (32 - destStartBitPositon)))))
						| ((*pSource&(((u32)-1) >> (32 - bitsCount))) << destStartBitPositon);
					return;
				}
				u32 bc = (destStartBitPositon + bitsCount) & 31;
				pSource += bm - 1;
				if (bc != 0)
				{
					u32 tmp = (*pSource >> (32 - destStartBitPositon));
					if (bc > (u32)destStartBitPositon)tmp |= pSource[1] << destStartBitPositon;
					*pEnd = (*pEnd&(((u32)-1) << bc)) | (tmp&(((u32)-1) >> (32 - bc)));
				}
				while (pDest != --pEnd) { --pSource; *pEnd = (*pSource >> (32 - destStartBitPositon)) | (*(pSource + 1) << destStartBitPositon); }
				*pDest = (*(pSource) << destStartBitPositon) | (*pDest&(((u32)-1) >> (32 - destStartBitPositon)));
				return;
			}
			if (destStartBitPositon == 0)
			{
				u32 bc = bitsCount & 31;
				u32 bm = bitsCount >> 5;
				u32* pEnd = bm + pDest;
				pSource += bm;
				if (bc != 0)
				{
					u32 tmp = *pSource >> sourceStartBitPositon;
					if (bc + sourceStartBitPositon > 32)tmp |= (pSource[1] << (32 - sourceStartBitPositon));
					*pEnd = (*pEnd & (((u32)-1) << bc)) | (tmp&(((u32)-1) >> (32 - bc)));
				}
				while (pEnd != pDest)
				{
					*--pEnd = (*(pSource - 1) >> sourceStartBitPositon) | (*pSource << (32 - sourceStartBitPositon));
					--pSource;
				}
				return;
			}
			u32 bm = (bitsCount + destStartBitPositon) >> 5;
			u32* pEnd = bm + pDest;
			if (pEnd == pDest)
			{
				u32 tmp = *pSource >> sourceStartBitPositon;
				if (bitsCount + sourceStartBitPositon > 32)tmp |= (pSource[1] << (32 - sourceStartBitPositon));
				*pDest = (*pDest &(((((u32)-1) << (bitsCount + destStartBitPositon)) | (((u32)-1) >> (32 - destStartBitPositon)))))
					| ((tmp&(((u32)-1) >> (32 - bitsCount))) << destStartBitPositon);
				return;
			}
			u32 bc = (bitsCount + destStartBitPositon) & 31;
			pSource += bm - 1;
			if (sourceStartBitPositon == destStartBitPositon)
			{
				if (bc != 0)
				{
					*pEnd = (*pEnd&(((u32)-1) << bc)) | ((*(pSource + 1))&(((u32)-1) >> (32 - bc)));
				}
				while (pDest != --pEnd) { *pEnd = *pSource--; }
			}
			else {
				s32 smov = sourceStartBitPositon - destStartBitPositon;
				if (smov < 0)
				{
					smov = -smov;
					if (bc != 0)
					{
						u32 tmp = *pSource >> (32 - smov);
						if (bc >(u32)smov)tmp |= pSource[1] << smov;
						*pEnd = (*pEnd&(((u32)-1) << bc)) | (tmp&(((u32)-1) >> (32 - bc)));
					}
					while (pDest != --pEnd)
					{
						--pSource;
						*pEnd = (*pSource >> (32 - smov)) | (pSource[1] << smov);
					}
				}
				else {
					if (bc != 0)
					{
						u32 tmp = pSource[1] >> smov;
						if (bc + smov > 32)tmp |= pSource[2] << (32 - smov);
						*pEnd = (*pEnd&(((u32)-1) << bc)) | (tmp&(((u32)-1) >> (32 - bc)));
					}
					while (pDest != --pEnd)
					{
						*pEnd = (*pSource >> smov) | (*(pSource + 1) << (32 - smov));
						--pSource;
					}
				}
			}
			*pDest = (((*pSource >> sourceStartBitPositon) | (pSource[1] << (32 - sourceStartBitPositon))) << destStartBitPositon) | (*pDest&(((u32)-1) >> (32 - destStartBitPositon)));
		}
		void Move(void* p, u64 startBitPosition, u64 bitsCount, s64 mov)//将p指向的内存的startBitPosition比特位起bitsCount比特的数据进行mov比特位移，正为左移，负为右移
		{
			if (!p || !bitsCount || !mov)return;
			if (startBitPosition >= 8)return Bits::Move((unsigned char*)p + (startBitPosition >> 3), startBitPosition & 7, bitsCount, mov);
			if (mov > (s64)bitsCount || -mov > (s64)bitsCount)return Bits::Set(p, (index_t)startBitPosition, (size_t)bitsCount, false);//过度位移
			size_t bytes = (size_t)((startBitPosition + bitsCount + 7) >> 3);//影响字节数
			unsigned char c_start = *(unsigned char*)p;//起始字节
			unsigned char c_end = *((unsigned char*)p + bytes - 1);//终止字节
			if (mov & 7)
			{
				s64 Bmov = mov < 0 ? (-mov) >> 3 : mov >> 3;
				u64 tmp;//临时数据存放
				if (mov > 0)//左移
				{
					s64 i = bytes - Bmov;
					mov &= 7;
					while (i > 7)
					{
						memmove(&tmp, (unsigned char*)p + i - 8, 8);
						tmp <<= mov;
						memmove((unsigned char*)p + i + Bmov - 7, (unsigned char*)&tmp + 1, 7);
						i -= 7;
					}
					tmp = 0;
					memcpy(&tmp, (unsigned char*)p, (size_t)i);
					tmp <<= mov;
					memcpy((unsigned char*)p + Bmov, &tmp, (size_t)i);
					if (Bmov)memset(p, 0, (size_t)Bmov);
				}
				else//右移
				{
					s64 i = 0;
					mov = (-mov) & 7;
					while (i + Bmov + 8 <= (s64)bytes)
					{
						memcpy(&tmp, (unsigned char*)p + i + Bmov, 8);
						tmp >>= mov;
						memcpy((unsigned char*)p + i, &tmp, 7);
						i += 7;
					}
					tmp = 0;
					memcpy(&tmp, (unsigned char*)p + i + Bmov, (size_t)(bytes - i - Bmov));
					tmp >>= mov;
					memcpy((unsigned char*)p + i, &tmp, (size_t)(bytes - i - Bmov));
					if (Bmov)memset((unsigned char*)p + bytes - Bmov, 0, (size_t)Bmov);
				}
			}
			else
			{
				mov >>= 3;
				if (mov > 0)//左移
				{
					memmove((unsigned char*)p + mov, p, bytes - (size_t)mov);
					memset(p, 0, (size_t)mov);
				}
				else//右移
				{
					memmove(p, (unsigned char*)p - mov, bytes + (size_t)mov);
					memset((unsigned char*)p + bytes + mov, 0, (size_t)-mov);
				}
			}
			//首字节处理
			if (startBitPosition != 0)
				*(unsigned char*)p = (*(unsigned char*)p&(0xff << startBitPosition)) | (c_start & (0xff >> (8 - startBitPosition)));
			bitsCount = (bitsCount + startBitPosition) & 7;//尾字节处理
			if (bitsCount != 0)
				*((unsigned char*)p + bytes - 1) = (*((unsigned char*)p + bytes - 1) & (0xff >> (8 - bitsCount))) | (c_end & (0xff << bitsCount));
		}
		u64 Move64(u64* p, size_t len, s64 mov)
		{
			if (mov == 0)return 0;
			size_t m;
			u64 result = 0;
			if (mov > 0)
			{
				m = (size_t)(mov >> 6);
				if (m >= len)
				{
					if (m == len)result = *p << (mov & 63);
					while (len-- != 0)*p++ = 0;
					return result;
				}
				mov &= 63;
				p += len;
				len -= m;
				if (mov == 0)
				{
					result = *(p - m);
					while (len-- != 0) { --p; *p = *(p - m); }
					while (m-- != 0) { *--p = 0; }
					return result;
				}
				result = len == 1 ? (*(p - m) << mov) : (*(p - m) << mov) | (*(p - m - 1) >> (64 - mov));
				while (--len != 0) { --p; *p = (*(p - m) << mov) | (*(p - m - 1) >> (64 - mov)); }
				--p; *p = (*(p - m) << mov);
				while (m-- != 0) { *--p = 0; }
				return result;
			}
			mov = -mov;
			m = (size_t)((u64)mov >> 6);
			if (m >= len)
			{
				if (m == len)result = *(p + m - 1) >> (mov & 63);
				while (len-- != 0)*p++ = 0;
				return result;
			}
			mov &= 63;
			len -= m;
			if (mov == 0)
			{
				result = *(p + m - 1);
				while (len-- != 0) { *p = *(p + m); ++p; }
				while (m-- != 0) { *p = 0; ++p; }
				return result;
			}
			result = len == 1 ? (*(p + m - 1) >> mov) : (*(p + m - 1) >> mov) | (*(p + m) << (64 - mov));
			while (--len != 0) { *p = (*(p + m) >> mov) | (*(p + m + 1) << (64 - mov)); ++p; }
			*p = (*(p + m) >> mov);
			while (m-- != 0) { *++p = 0; }
			return result;
		}
	}
}
#endif