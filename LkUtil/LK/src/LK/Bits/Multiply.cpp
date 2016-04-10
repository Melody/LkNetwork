#include"stdafx.h"
#ifdef LK_BITS_MULTIPLY_
namespace LK{
	namespace Bits {
		u32 SelfMultiplyInt32Low32(u32*p, size_t len, u32 num)
		{
			u64 tmp = 0;
			for (size_t i = 0; i < len; ++i)
			{
				tmp += (u64)p[i] * (u64)num;
				p[i] = (u32)tmp;
				tmp >>= 32;
			}
			return (u32)tmp;
		}
		u64 SelfMultiplyInt64Low32(u32*p, size_t len, u64 num)
		{
			u64 tmp[2] = { 0 };
			u64 ull = num >> 32;
			num &= (u32)-1;
			for (size_t i = 0; i < len; ++i)
			{
				u64 t = (u64)p[i] * num;
				if (tmp[0] > ~t)tmp[1]++;
				tmp[0] += t;
				*(u64*)((u32*)tmp + 1) += (u64)p[i] * ull;
				p[i] = (u32)tmp[0];
				tmp[0] = (tmp[0] >> 32) | (tmp[1] << 32);
				tmp[1] >>= 32;
			}
			return tmp[0];
		}
		u64 SelfMultiplyInt64Low64(u64*p, size_t len, u64 num)
		{
			u64 tmp[2];
			tmp[0] = 0;
			for (size_t i = 0; i < len; ++i)
			{
				u64 t = Multiply(p[i], num, tmp + 1);
				if (tmp[0] > ~t)tmp[1]++;
				p[i] = tmp[0] + t;
				tmp[0] = tmp[1];
			}
			return tmp[0];
		}
		u32 SelfMultiplyInt32High32(u32*p, size_t len, u32 num)
		{
			if (len == 0)return 0;
			u64 t = num*(u64)p[0];
			u64 tmp = t >> 32;
			for (size_t i = 0; i < len - 1; ++i)
			{
				tmp += (u64)p[i + 1] * (u64)num;
				p[i] = (u32)tmp;
				tmp >>= 32;
			}
			p[len - 1] = (u32)tmp;
			return (u32)t;
		}
		u64 SelfMultiplyInt64High32(u32*p, size_t len, u64 num)
		{
			if (len == 0)return 0;
			u64 tmp[2] = { 0 };
			u64 ull = num >> 32;
			num &= (u32)-1;
			u64 result = 0;
			for (size_t i = 0; i < len; ++i)
			{
				u64 t = (u64)p[i] * num;
				if (tmp[0] > ~t)tmp[1]++;
				tmp[0] += t;
				*(u64*)((u32*)tmp + 1) += (u64)p[i] * ull;
				if (i < 2) ((u32*)&result)[i] = (u32)tmp[0];
				else p[i - 2] = (u32)tmp[0];
				tmp[0] = (tmp[0] >> 32) | (tmp[1] << 32);
				tmp[1] = 0;
			}
			if (len == 1)
			{
				p[0] = tmp[0] >> 32;
				result |= tmp[0] << 32;
			}
			else
			{
				*(u64*)(p + len - 2) = *tmp;
			}
			return result;
		}
		u64 SelfMultiplyInt64High64(u64*p, size_t len, u64 num)
		{
			if (len == 0)return 0;
			u64 tlow, thigh;
			u64 t = Multiply(p[0], num, &tlow);
			for (size_t i = 0; i < len - 1; ++i)
			{
				u64 ull = Multiply(p[i + 1], num, &thigh);
				if (tlow > ~ull)++thigh;
				p[i] = tlow + ull;
				tlow = thigh;
			}
			p[len - 1] = tlow;
			return t;
		}
		u32 MultiplyInt32Low32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num)
		{
			u64 tmp = 0;
			for (size_t i = 0; i <(lenSource>lenDest ? lenDest : lenSource); ++i)
			{
				tmp += (u64)pSource[i] * (u64)num;
				pDest[i] = (u32)tmp;
				tmp >>= 32;
			}
			if (lenSource<lenDest)
			{
				pDest[lenSource] = (u32)tmp;
				while (++lenSource != lenDest)pDest[lenSource] = 0;
				return 0;
			}
			return (u32)(lenSource == lenDest ? tmp : (pSource[lenDest] * num + tmp));
		}
		u64 MultiplyInt64Low32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u64 num)
		{
			u64 tmp[2] = { 0 };
			u64 thigh = num >> 32;
			u64 tlow = num & (u32)-1;
			for (size_t i = 0; i < (lenSource>lenDest ? lenDest : lenSource); ++i)
			{
				u64 t = pSource[i] * tlow;
				if (tmp[0]>~t)tmp[1]++;
				tmp[0] += t;
				*(u64*)((u32*)tmp + 1) += pSource[i] * thigh;
				pDest[i] = (u32)tmp[0];
				tmp[0] = (tmp[0] >> 32) | (tmp[1] << 32);
				tmp[1] >>= 32;
			}
			if (lenSource < lenDest)
			{
				pDest[lenSource] = (u32)tmp[0];
				if (++lenSource != lenDest) { pDest[lenSource] = (tmp[0] >> 32); tmp[0] = 0; }
				else return tmp[0] >> 32;
				while (++lenSource != lenDest)pDest[lenSource] = 0;
				return 0;
			}
			return lenSource == lenDest ? tmp[0] :
				(lenSource == lenDest + 1) ? (pSource[lenDest] * num + tmp[0]) :
				((*(u64*)(pSource + lenDest))*num + tmp[0]);
		}
		u64 MultiplyInt64Low64(u64 *pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num)
		{
			u64 tlow = 0, thigh;
			for (size_t i = 0; i <(lenSource>lenDest ? lenDest : lenSource); ++i)
			{
				u64 t = Multiply(pSource[i], num, &thigh);
				if (tlow > ~t)++thigh;
				pDest[i] = tlow + t;
				tlow = thigh;
			}
			if (lenSource < lenDest)
			{
				pDest[lenSource] = tlow;
				while (++lenSource != lenDest)pDest[lenSource] = 0;
				return 0;
			}
			return lenSource == lenDest ? tlow : (pSource[lenDest] * num + tlow);
		}
		u32 MultiplyInt32High32(u32*pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num)
		{
			u64 t;
			u64 tmp;
			if (lenDest > lenSource)
			{
				//当lenDest多于容纳结果所需大小时（lenDest>lenSource+1），进行数据置零
				while (lenDest > lenSource + 1)
				{
					*pDest++ = 0;
					--lenDest;
				}
				//若刚刚好能容纳所需大小，则什么都不用做lenDest==lenSource+1
				t = 0;
				tmp = 0;
			}
			else//lenDest<=lenSource
			{
				//若空间不足，则应该适当调整源数据大小lenDest+1<=lenSource
				if (lenDest < lenSource)
				{
					pSource += lenSource - lenDest - 1;
					lenSource = lenDest + 1;
					tmp = (((u64)*pSource)*num) >> 32;
					if (lenSource == 1)return (u32)tmp;
					t = (((u64)pSource[1])*num) + tmp;
					tmp = t >> 32;
					lenSource -= 2;
					pSource += 2;
				}
				else//lenSource==lenDest
				{
					if (lenSource == 0)return 0;
					t = (((u64)*pSource)*num);
					tmp = t >> 32;
					lenSource--;
					pSource++;
				}
			}
			for (size_t i = 0; i < lenSource; ++i)//需要lenDest==lenSource+1
			{
				tmp += (u64)pSource[i] * (u64)num;
				pDest[i] = (u32)tmp;
				tmp >>= 32;
			}
			if (lenDest != 0)pDest[lenSource] = (u32)tmp;
			return (u32)t;
		}
		u64 MultiplyInt64High64(u64*pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num)
		{
			u64 t;
			u64 tlow, thigh;
			if (lenDest > lenSource)
			{
				//当lenDest多于容纳结果所需大小时（lenDest>lenSource+1），进行数据置零
				while (lenDest > lenSource + 1)
				{
					*pDest++ = 0;
					--lenDest;
				}
				//若刚刚好能容纳所需大小，则什么都不用做lenDest==lenSource+1
				t = 0;
				tlow = 0;
			}
			else//lenDest<=lenSource
			{
				//若空间不足，则应该适当调整源数据大小lenDest+1<=lenSource
				if (lenDest < lenSource)
				{
					pSource += lenSource - lenDest - 1;
					lenSource = lenDest + 1;
					Multiply(*pSource, num, &tlow);
					if (lenSource == 1)return tlow;
					t = Multiply(pSource[1], num, &thigh);
					if (t > ~tlow)++thigh;
					t += tlow;
					tlow = thigh;
					lenSource -= 2;
					pSource += 2;
				}
				else//lenSource==lenDest
				{
					if (lenSource == 0)return 0;
					t = Multiply(*pSource, num, &tlow);
					lenSource--;
					pSource++;
				}
			}
			for (size_t i = 0; i < lenSource; ++i)//需要lenDest==lenSource+1
			{
				u64 tmp = Multiply(pSource[i], num, &thigh);
				if (tlow > ~tmp)++thigh;
				pDest[i] = tlow + tmp;
				tlow = thigh;
			}
			if (lenDest != 0)pDest[lenSource] = tlow;
			return t;
		}
		u32 MultiplyLow32(u32*pDest, size_t lenDest, cu32* pm1, size_t len1, cu32*pm2, size_t len2)
		{
			u64 tlow = 0, thigh = 0;
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u32)); return 0; }
			size_t len = len1 + len2;
			size_t i = 0;
			for (;; ++i)
			{
				//当i>=len1+len2时，乘积结果为0
				if (i >= len) { if (i == lenDest)break; pDest[i] = 0; continue; }
				//当i<len1+len2时，要收集所有x+y==i，x∈[0,len1-1]，y∈[0,len2-1]的x,y对应的pm1[x]*pm2[y]乘积求和
				for (size_t x = (i < len2 ? 0 : i - len2 + 1); x < len1; ++x)
				{
					size_t y = i - x;
					u64 tmp = (u64)pm1[x] * pm2[y];
					if (tlow > ~tmp)++thigh;
					tlow += tmp;
					if (y == 0)break;
				}
				if (i == lenDest)return (u32)tlow;
				pDest[i] = (u32)tlow;
				tlow = (thigh << 32) | (tlow >> 32);
				thigh >>= 32;
			}
			return 0;
		}
		u64 MultiplyLow64(u64*pDest, size_t lenDest, cu64* pm1, size_t len1, cu64*pm2, size_t len2)
		{
			u64 tl = 0, th = 0, thigh = 0;
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u64)); return 0; }
			size_t len = len1 + len2;
			size_t i = 0;
			for (;; ++i)
			{
				//当i>=len1+len2时，乘积结果为0
				if (i >= len) { if (i == lenDest)break; pDest[i] = 0; continue; }
				//当i<len1+len2时，要收集所有x+y==i，x∈[0,len1-1]，y∈[0,len2-1]的x,y对应的pm1[x]*pm2[y]乘积求和
				for (size_t x = (i < len2 ? 0 : i - len2 + 1); x < len1; ++x)
				{
					size_t y = i - x;
					u64 tmphigh;
					u64 tmplow = Multiply(pm1[x], pm2[y], &tmphigh);
					if (tl > ~tmplow)if (++th == 0)++thigh;
					tl += tmplow;
					if (th > ~tmphigh)++thigh;
					th += tmphigh;
					if (y == 0)break;
				}
				if (i == lenDest)return tl;
				pDest[i] = tl;
				tl = th;
				th = thigh;
				thigh = 0;
			}
			return 0;
		}
		u32 MultiplyHigh32(u32*pDest, size_t lenDest, cu32* pm1, size_t len1, cu32*pm2, size_t len2)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u32)); return 0; }
			u64 tlow = 0, thigh = 0;
			u32 ret = 0;
			//乘数乘积长度
			size_t sumlen = len1 + len2;
			//如果目标内存足够，则多余部分置0
			while (lenDest > sumlen)
			{
				*pDest++ = 0;
				lenDest--;
			}
			//sum起始值设定,pDest[lenDest-1]对应pm1[len1-1]*pm2[len2-1]右移32位的结果，因此其对应的系数和应该是len1+len2-1
			size_t sum = sumlen - lenDest;
			//返回值对应的sum,如果sum=0，那么sumrt将是-1，将永远取不到
			size_t sumrt = sum - 1;
			//开始循环求和
			for (size_t i = sum < 2 ? 0 : sum - 2; i < sumlen; ++i)
			{
				//x+y=i , x∈[0,len1-1] , y∈[0,len2-1]
				for (size_t x = (i < len2 ? 0 : i - len2 + 1); x < len1; ++x)
				{
					size_t y = i - x;
					u64 tmp = (u64)pm1[x] * pm2[y];
					if (tlow > ~tmp)++thigh;
					tlow += tmp;
					if (y == 0)break;
				}
				if (i >= sum)pDest[i - sum] = (u32)tlow;
				else if (i == sumrt) { ret = (u32)tlow; }
				tlow = (thigh << 32) | (tlow >> 32);
				thigh >>= 32;
			}
			return ret;
		}
		u64 MultiplyHigh64(u64*pDest, size_t lenDest, cu64* pm1, size_t len1, cu64*pm2, size_t len2)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u64)); return 0; }
			u64 tl = 0, th = 0, thigh = 0;
			u64 ret = 0;
			//乘数乘积长度
			size_t sumlen = len1 + len2;
			//如果目标内存足够，则多余部分置0
			while (lenDest > sumlen)
			{
				*pDest++ = 0;
				lenDest--;
			}
			//sum起始值设定,pDest[lenDest-1]对应pm1[len1-1]*pm2[len2-1]右移32位的结果，因此其对应的系数和应该是len1+len2-1
			size_t sum = sumlen - lenDest;
			//返回值对应的sum,如果sum=0，那么sumrt将是-1，将永远取不到
			size_t sumrt = sum - 1;
			//开始循环求和
			for (size_t i = sum < 2 ? 0 : sum - 2; i < sumlen; ++i)
			{
				//x+y=i , x∈[0,len1-1] , y∈[0,len2-1]
				for (size_t x = (i < len2 ? 0 : i - len2 + 1); x < len1; ++x)
				{
					size_t y = i - x;
					u64 tmphigh;
					u64 tmplow = Multiply(pm1[x], pm2[y], &tmphigh);
					if (tl > ~tmplow)if (++th == 0)++thigh;
					tl += tmplow;
					if (th > ~tmphigh)++thigh;
					th += tmphigh;
					if (y == 0)break;
				}
				if (i >= sum)pDest[i - sum] = tl;
				else if (i == sumrt) { ret = tl; }
				tl = th;
				th = thigh;
				thigh = 0;
			}
			return ret;
		}
#ifdef LK_MATH_DOUBLECOMPLEX_
		u8 MultiplyFFTLow8(u8*pDest, size_t lenDest, cu8* pm1, size_t len1, cu8*pm2, size_t len2, u8*ptmp)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u16)); return 0; }
			size_t len = ((size_t)1) << (sizeof(size_t) * 8 + 1 - LeftZeroCount((len1 > len2 ? len1 : len2) - 1));
			u8* arr = ptmp ?ptmp: new u8[(sizeof(Math::DoubleComplex)*len * 2)];
			Math::DoubleComplex* dc1 = (Math::DoubleComplex*)(u8*)arr;
			Math::DoubleComplex* dc2 = dc1 + len;
			Math::DoubleComplex::FFTLow8(pm1, dc1, len1, len);
			Math::DoubleComplex::FFTLow8(pm2, dc2, len2, len);
			for (size_t i = 0; i < len; ++i) { dc1[i] *= dc2[i]; }
			u8 result = Math::DoubleComplex::IFFTLow8(dc1, pDest, len, lenDest);
			if (!ptmp)delete [] arr;
			return result;
		}
		u8 MultiplyFFTHigh8(u8*pDest, size_t lenDest, cu8* pm1, size_t len1, cu8*pm2, size_t len2, u8*ptmp)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u16)); return 0; }
			size_t len = ((size_t)1) << (sizeof(size_t) * 8 + 1 - LeftZeroCount((len1>len2 ? len1 : len2) - 1));
			u8* arr = ptmp ?ptmp: new u8[(sizeof(Math::DoubleComplex)*len * 2)];
			Math::DoubleComplex* dc1 = (Math::DoubleComplex*)(u8*)arr;
			Math::DoubleComplex* dc2 = dc1 + len;
			Math::DoubleComplex::FFTHigh8(pm1, dc1, len1, len);
			Math::DoubleComplex::FFTHigh8(pm2, dc2, len2, len);
			for (size_t i = 0; i < len; ++i)dc1[i] *= dc2[i];
			u8 result = Math::DoubleComplex::IFFTHigh8(dc1, pDest, len, lenDest);
			if(!ptmp)delete[]arr;
			return result;
		}
		u16 MultiplyFFTLow(u16*pDest, size_t lenDest, cu16* pm1, size_t len1, cu16*pm2, size_t len2, u8*ptmp)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u16)); return 0; }
			size_t len = ((size_t)1) << (sizeof(size_t) * 8 + 1 - LeftZeroCount((len1>len2 ? len1 : len2) - 1));
			u8* arr = ptmp ? ptmp : new u8[(sizeof(Math::DoubleComplex)*len * 2)];
			Math::DoubleComplex* dc1 = (Math::DoubleComplex*)arr;
			Math::DoubleComplex* dc2 = dc1 + len;
			Math::DoubleComplex::FFTLow(pm1, dc1, len1, len);
			Math::DoubleComplex::FFTLow(pm2, dc2, len2, len);
			for (size_t i = 0; i < len; ++i) { dc1[i] *= dc2[i]; }
			len = Math::DoubleComplex::IFFTLow(dc1, pDest, len, lenDest);
			if (!ptmp)delete []arr;
			return (u16)len;
		}
		u16 MultiplyFFTHigh(u16*pDest, size_t lenDest, cu16* pm1, size_t len1, cu16*pm2, size_t len2, u8*ptmp)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u16)); return 0; }
			size_t len = ((size_t)1) << (sizeof(size_t) * 8 + 1 - LeftZeroCount((len1>len2 ? len1 : len2) - 1));
			u8* arr = ptmp ?ptmp: new u8[(sizeof(Math::DoubleComplex)*len * 2)];
			Math::DoubleComplex* dc1 = (Math::DoubleComplex*)(u8*)arr;
			Math::DoubleComplex* dc2 = dc1 + len;
			Math::DoubleComplex::FFTHigh(pm1, dc1, len1, len);
			Math::DoubleComplex::FFTHigh(pm2, dc2, len2, len);
			for (size_t i = 0; i < len; ++i)dc1[i] *= dc2[i];
			len = Math::DoubleComplex::IFFTHigh(dc1, pDest, len, lenDest);
			if (!ptmp)delete [] arr;
			return (u16)len;
		}
#endif
#ifdef LK_MATH_COMPLEX128_
		u32 MultiplyFFTHigh32(u32 * pDest, size_t lenDest, cu32 * pm1, size_t len1, cu32 * pm2, size_t len2, u8*ptmp)
		{
			if (len1 == 0 || len2 == 0) { memset(pDest, 0, lenDest*sizeof(u32)); return 0; }
			size_t len = ((size_t)1) << (sizeof(size_t) * 8 + 1 - LeftZeroCount((len1>len2 ? len1 : len2) - 1));
			u8* arr = ptmp ? ptmp: new u8[(sizeof(Math::Complex128)*len * 2)];
			Math::Complex128* dc1 = (Math::Complex128*)(u8*)arr;
			Math::Complex128* dc2 = dc1 + len;
			Math::Complex128::FFTHigh(pm1, dc1, len1, len);
			Math::Complex128::FFTHigh(pm2, dc2, len2, len);
			for (size_t i = 0; i < len; ++i)dc1[i] *= dc2[i];
			u32 result = Math::Complex128::IFFTHigh(dc1, pDest, len, lenDest);
			if (!ptmp)delete []arr;
			return result;
		}
#endif
	}
}
#endif