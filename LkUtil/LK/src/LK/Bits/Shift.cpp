#include"stdafx.h"
#ifdef LK_BITS_SET_
namespace LK{
	namespace Bits {
		void CopyShift64(cu64* pSource, size_t sLen, u64* pDest, size_t dLen, s64 mov)
		{
			size_t m;
			if (mov == 0)//不相对位移
			{
				m = sLen > dLen ? dLen : sLen;
				memmove(pDest, pSource, sizeof(u64)*m);
				if (m < dLen)
				{
					pDest += m;
					while (m++ < dLen)*pDest++ = 0;
				}
				return;
			}
			if (mov > 0)//左移
			{
				//获取移动块数
				m = (size_t)(mov >> 6);
				if (m >= dLen)//移动后导致全部移除
				{
					while (dLen-- != 0)*pDest++ = 0;
					return;
				}
				mov &= 63;
				u64*pd = pDest + m;//目标起始写入位置
				size_t dl = dLen - m;//目标起始写入位置到末尾的块数
				size_t len = sLen > dl ? dl : sLen;//实际写入块数
				if (mov == 0)//移动数量刚好为整数块
				{
					if (pd > pSource)
					{
						pd += len;
						pSource += len;
						while (len-- != 0)*--pd = *--pSource;//复制数据
						while (m-- != 0)*--pd = 0;//左移，右端置0
						return;
					}
					if (pd < pSource)
					{
						while (len-- != 0)*pd++ = *pSource++;
						while (m-- != 0)*pDest++ = 0;//左移，右端置0
						return;
					}
					while (m-- != 0)*--pd = 0;//右端置0
					return;
				}
				if (pd >= pSource)
				{
					pd += len;
					pSource += len;

					if (len > 1)while (--len != 0)
					{
						--pd;
						--pSource;
						*pd = (*pSource << mov) | (*(pSource - 1) >> (63 ^ mov));
					}
					if (len != 0)
					{
						*--pd = (*(pSource - 1) << mov);
					}
					while (m-- != 0)*--pd = 0;//右端置0
					return;
				}
				//pd < pSource
				while (pDest != pd)*pDest++ = 0;//右端置0
				if (len != 0)
				{
					*pd = *pSource << mov;
				}
				if (len > 1)while (--len != 0)
				{
					*++pd = (*(pSource + 1) << mov) | (*pSource >> (63 ^ mov));
					++pSource;
				}
				return;
			}
			//mov<0
			mov = -mov;
			m = (size_t)(mov >> 6);
			if (m >= sLen)//移动后导致全部移除，由于目标左端不做处理，将直接返回
			{
				return;
			}
			mov &= 63;
			pSource += m;
			sLen -= m;
			size_t len = sLen > dLen ? dLen : sLen;
			if (mov == 0)//刚好移动整数块
			{
				if (pDest < pSource)
				{
					while (len-- != 0)*pDest++ = *pSource++;
					return;
				}
				if (pDest > pSource)
				{
					pDest += len;
					pSource += len;
					while (len-- != 0)*--pDest = *--pSource;
					return;
				}
				return;
			}
			//mov!=0
			if (pDest <= pSource)
			{
				if (len > 1)while (--len != 1)
				{
					*pDest++ = (*(pSource + 1) << (63 ^ mov)) | (*pSource >> mov);
					++pSource;
				}
				if (len != 0)
				{
					*pDest = *pSource >> mov;
				}
				return;
			}
			pDest += len;
			pSource += len;
			if (len != 0)
			{
				*--pDest = *--pSource >> mov;
			}
			if (len > 1)while (--len != 0)
			{
				--pDest;
				--pSource;
				*pDest = (*pSource >> mov) | (*(pSource + 1) << (63 ^ mov));
			}
			return;
		}
	}
}
#endif