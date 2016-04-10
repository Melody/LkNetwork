#include"stdafx.h"
#ifdef LK_BITS_DIVIDE_
namespace LK{
	namespace Bits {
		u64 Divide(u64 high64, u64 low64, u64 num, u64*p){
			u64 nh;
			u64 nl;
			if (high64 == 0) { nh = low64 / num; if (p)*p = low64 - nh*num; return nh; }
			if (num < 0x100000000) {
				nh = ((u64)(high64 % num) << 32) | (low64 >> 32);
				high64 = nh / num;
				nl = ((u64)(nh - high64* num) << 32) | (u32)low64;
				nh = nl / num;
				if (p)*p = nl - nh* num;
				return ((high64) << 32) | (nh);
			}
			//前导0
			size_t lzc = LeftZeroCount(num);
			u64 nnum = num << lzc;
			nl = nnum & 0xffffffffull;
			nh = nnum >> 32;
			//试商
			u64 q1 = (u64)(high64 / ((double)nnum + 0x2000) * 0x100000000);
			//检验
			u64 tl = nl*(q1 & 0xffffffffull);
			u64 th = (q1 >= 0x100000000) ? (nl + q1*nh + (tl >> 32)) : (q1*nh + (tl >> 32));
			tl <<= 32;
			if (low64 < tl)high64--; low64 -= tl; high64 -= th;
			if (high64 >= nh) {
				th = nnum << 32;
				if (high64 == nh) {
					if (low64 >= th) {
						low64 -= th; high64 = 0; q1++;
					}
				}
				else {
					high64 -= (low64 >= th ? nh : nh + 1);
					low64 -= th;
					q1++;
				}
			}
			//第二轮试商
			u64 q2 = (u64)(((high64 << 32) | (low64 >> 32)) / ((double)nnum + 0x2000) * 0x100000000);
			//第二轮检验
			tl = (q2 & 0xffffffffull)*nl;
			th = q2*nh + (tl >> 32);
			tl = (tl & 0xffffffffull) | (th << 32);
			th >>= 32;
			if (low64 < tl)high64--;
			low64 -= tl;
			if (low64 >= nnum) {
				low64 -= nnum; ++q2;
			}
			if (high64 != th) {
				++q2;
				low64 -= nnum;
			}
			//输出结果
			if (lzc == 0) {
				if (p)*p = low64;
				return (q1 << 32) | q2;
			}
			tl = low64 / num;
			if (p)*p = low64 - tl*num;
			return (q1 << (32 + lzc)) | (q2 << lzc) | tl;
		}

		u32 SelfDivideInt32Low32(u32 *p, size_t len, u32 num)
		{
			if (num == 0)return 0;
			u64 tmp = 0;
			size_t i = len;
			while (i != 0)
			{
				tmp |= p[--i];
				p[i] = (u32)(tmp / num);
				tmp = (u64)(tmp%num) << 32;
			}
			return tmp >> 32;
		}
		u32 SelfDivideInt32Low64(u64 *p, size_t len, u32 num)
		{
			if (num == 0)return 0;
			u64 tmp = 0;
			size_t i = len;
			while (i != 0)
			{
				tmp |= p[--i] >> 32;
				u64 t = (tmp / num) << 32;
				tmp = ((u64)(tmp%num) << 32) | (u32)p[i];
				p[i] = (tmp / num) | t;
				tmp = ((u64)(tmp%num) << 32);
			}
			return tmp >> 32;
		}
		u64 SelfDivideInt64Low32(u32 *p, size_t len, u64 num)
		{
			if (num == 0)return 0;
			u64 rt;
			if (len == 1) { rt = *p; *p = 0; return rt; }
			rt = 0;
			size_t i = len;
			while (i > 1)
			{
				i -= 2;
				*(u64*)(p + i) = Divide(rt, *(u64*)(p + i), num, &rt);
			}
			if (i != 0)
			{
				*p = (u32)Divide(rt >> 32, (rt << 32) | *p, num, &rt);
			}
			return rt;
		}
		u64 SelfDivideInt64Low64(u64 *p, size_t len, u64 num)
		{
			if (num == 0)return 0;
			u64 tmp = 0;
			size_t i = len;
			while (i != 0)
			{
				--i;
				p[i] = Divide(tmp, p[i], num, &tmp);
			}
			return tmp;
		}
		u32 SelfDivideInt32High32(u32 *p, size_t len, u32 num, bool mov)
		{
			if (num == 0 || len == 0)return 0;
			size_t i = len - 1;
			u64 tmp = ((u64)p[i]) << 32;;
			u64 result = 0;
			while (i != 0)
			{
				tmp |= p[--i];
				result = (result << 32) | (tmp / num);
				tmp = (u64)(tmp%num) << 32;
				p[i + 1] = (u32)(mov ? (result >> 1) : result);
			}
			result = (result << 32) | (tmp / num);
			p[0] = (u32)(mov ? (result >> 1) : result);
			return (u32)(mov ? ((tmp % (((u64)num) << 1)) >> 1) : tmp%num);
		}
		u64 SelfDivideInt64High64(u64 *p, size_t len, u64 num, bool mov)
		{
			if (num == 0 || len == 0)return 0;
			size_t i = len - 1;
			u64 tmp = p[i];
			u64 result[2];
			result[0] = tmp / num;
			while (i != 0)
			{
				result[1] = result[0];
				result[0] = Divide(tmp, p[--i], num, &tmp);
				p[i + 1] = mov ? (result[0] >> 1) | (result[1] << 63) : result[0];
			}
			result[1] = result[0];
			result[0] = Divide(tmp, 0, num, &tmp);
			p[0] = mov ? (result[0] >> 1) | (result[1] << 63) : result[0];
			if (mov)
			{
				if ((result[0] & 1) == 0)tmp >>= 1;
				else
				{
					mov = tmp > ~num;
					tmp += num;
					if (mov)tmp = (tmp >> 1) | (1ull << 63);
					else tmp >>= 1;
				}
			}
			return tmp;
		}
		u32 DivideInt32Low32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num)
		{
			if (num == 0 || lenSource == 0) { for (size_t i = 0; i < lenDest; ++i)pDest[i] = 0; return 0; }
			u64 tmp = 0;
			size_t i = lenSource;
			if (lenDest > lenSource)
			{
				while (lenDest != lenSource)pDest[--lenDest] = 0;
			}
			while (i != 0)
			{
				tmp |= pSource[--i];
				if (i < lenDest)pDest[i] = (u32)(tmp / num);
				tmp = (u64)(tmp%num) << 32;
			}
			return tmp >> 32;
		}
		u64 DivideInt64Low64(u64 *pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num)
		{
			if (num == 0 || lenSource == 0) { for (size_t i = 0; i < lenDest; ++i)pDest[i] = 0; return 0; }
			u64 tmp = 0;
			if (lenDest > lenSource)
			{
				while (lenDest != lenSource)pDest[--lenDest] = 0;
			}
			size_t i = lenSource;
			while (i != 0)
			{
				--i;
				u64 t = Divide(tmp, pSource[i], num, &tmp);
				if (i < lenDest)pDest[i] = t;
			}
			return tmp;
		}
		u32 DivideInt32High32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num, bool mov)
		{
			if (num == 0 || lenSource == 0) { for (size_t i = 0; i < lenDest; ++i)pDest[i] = 0; return 0; }
			size_t i = lenSource - 1;
			if (lenDest == 0)
			{
				return (u32)(mov ? ((pSource[i] % (((u64)num) << 1)) | (pSource[i] & 1)) : pSource[i] % num);
			}
			u64 tmp = ((u64)pSource[i]) << 32;;
			size_t j = lenDest - 1;
			u64 result = 0;
			for (;;)
			{
				if (i != 0)tmp |= pSource[--i];
				result = (result << 32) | (tmp / num);
				pDest[j] = (u32)(mov ? (result >> 1) : result);
				if (i == 0 && tmp == 0 && result == 0)while (j != 0)pDest[--j] = 0;
				if (j == 0)break;
				tmp = (u64)(tmp%num) << 32;
				--j;
			}
			return (u32)(mov ? ((tmp >> 1) % num) : tmp%num);
		}
		u64 DivideInt64High64(u64 *pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num, bool mov)
		{
			if (num == 0 || lenSource == 0) { for (size_t i = 0; i < lenDest; ++i)pDest[i] = 0; return 0; }
			size_t i = lenSource - 1;
			u64 tmp = pSource[i];
			u64 result[2];
			if (lenDest == 0)
			{
				if (mov)
				{
					return (((tmp >> 1) % num) << 1) | (tmp & 1);
				}
				return tmp%num;
			}
			size_t j = lenDest;
			result[0] = tmp / num;
			while (j != 0)
			{
				result[1] = result[0];
				result[0] = i == 0 ? Divide(tmp, 0, num, &tmp) : Divide(tmp, pSource[--i], num, &tmp);
				pDest[--j] = mov ? (result[0] >> 1) | (result[1] << 63) : result[0];
				if (i == 0 && tmp == 0 && result[0] == 0) { while (j != 0)pDest[--j] = 0; }
			}
			if (mov)
			{
				if ((result[0] & 1) == 0)tmp >>= 1;
				else
				{
					mov = tmp > ~num;
					tmp += num;
					if (mov)tmp = (tmp >> 1) | (1ull << 63);
					else tmp >>= 1;
				}
			}
			return tmp;
		}
		void DivideLow64(u64*p, size_t len, cu64 *p1, u64 bitsCount1, cu64*p2, u64 bitsCount2, u64* pd, size_t lend, u64*ptmp)
		{
			if (bitsCount2 == 0)return;
			//计算长度
			size_t len1 = (size_t)((bitsCount1 + 63) >> 6);
			size_t len2 = (size_t)((bitsCount2 + 63) >> 6);
			//除数比被除数大
			if (bitsCount2 > bitsCount1)
			{
				for (size_t i = 0; i < len; ++i)p[i] = 0;
				if (pd)//输出余数
				{
					for (size_t i = 0; i < lend&&i < len1; ++i)pd[i] = p1[i];
					for (size_t i = len1; i < lend; ++i)pd[i] = 0;
				}
				return;
			}
			//除数比较小的时候采用快速除法
			if (len2 == 1)
			{
				bitsCount2 = DivideInt64Low64(p, len, p1, len1, *p2);//输出结果并获取余数
				if (pd&&lend != 0) { *pd = bitsCount2; for (size_t i = 1; i < lend; ++i)pd[i] = 0; }//输出余数
				return;
			}
			//获取计算所需空间
			u8*buf = ptmp?(u8*)ptmp:new u8[sizeof(u64)*(len2 + 1) * 2];
			u64* pbuf1 = (u64*)(u8*)buf;//被除数内存
			u64* pbuf2 = pbuf1 + len2 + 1;//试商乘积
										  //第一次试商（非标准）
			size_t destlen = len1 - len2;
			u64 dq = 0;
			u64 q = 0;
			for (size_t i = destlen; i < len; ++i)p[i] = 0;//清理商的高位数据
			if (CompareHigh64(p1 + destlen, p2, len2, len2) >= 0)
			{
				memcpy(pbuf1, p1 + destlen, len2*sizeof(u64));
				if (len2 == 1)q = p1[len1 - 1] / (p2[0] + 1);
				else {
					u8 lzc = (u8)LeftZeroCount(p2[len2 - 1]);
					u64 tmp1 = ShiftLeft(p2[len2 - 1], p2[len2 - 2], lzc);
					u64 tmp2 = ShiftLeft(0, p1[len1 - 1], lzc);
					if (++tmp1 != 0)
					{
						u64 tmp3 = ShiftRight(p1[len1 - 1], p1[len1 - 2], 64 - lzc);
						q = Divide(tmp2, tmp3, tmp1);
					}
					else q = tmp2;
				}
				MultiplyInt64Low64(pbuf2, len2, p2, len2, q);
				SelfMinus64(pbuf1, len2, pbuf2, len2);
				while (CompareHigh64(pbuf1, p2, len2, len2) >= 0)
				{
					SelfMinus64(pbuf1, len2, p2, len2);
					++q;
				}
				if (destlen < len)p[destlen] = q;
				if (destlen != 0)
				{
					for (size_t i = len2; i != 0; --i)pbuf1[i] = pbuf1[i - 1];//左移64bit
					dq = ((bitsCount2 & 63) == 0 ? p2[len2 - 1] : ShiftLeft(p2[len2 - 1], p2[len2 - 2], 64 - (bitsCount2 & 63))) + 1;
				}
			}
			else//除数比被除数大
			{
				if (destlen != 0)
				{
					memcpy(pbuf1 + 1, p1 + destlen, len2*sizeof(u64));
					dq = ((bitsCount2 & 63) == 0 ? p2[len2 - 1] : ShiftLeft(p2[len2 - 1], p2[len2 - 2], 64 - (bitsCount2 & 63))) + 1;
				}
				else
				{
					for (size_t i = 0; i < len; ++i)p[i] = 0;
					if (pd)
					{
						for (size_t i = 0; i < lend&&i < len1; ++i)pd[i] = p1[i];
						for (size_t i = len1; i < lend; ++i)pd[i] = 0;
					}
					if (!ptmp)delete []buf;
					return;
				}
			}
			while (destlen != 0)
			{
				destlen--;
				pbuf1[0] = p1[destlen];
				if (pbuf1[len2 - 1] != 0 || pbuf1[len2] != 0)
				{
					u64 h64 = (bitsCount2 & 63) == 0 ? pbuf1[len2] : ShiftLeft(pbuf1[len2], pbuf1[len2 - 1], 64 - (bitsCount2 & 63));
					q = dq == 0 ? h64 : Divide(h64, (bitsCount2 & 63) == 0 ? pbuf1[len2 - 1] : ShiftLeft(pbuf1[len2 - 1], pbuf1[len2 - 2], 64 - (bitsCount2 & 63)), dq);
					MultiplyInt64Low64(pbuf2, len2 + 1, p2, len2, q);//乘积
					SelfMinus64(pbuf1, len2 + 1, pbuf2, len2 + 1);//相减
					while (pbuf1[len2] != 0 || CompareHigh64(pbuf1, p2, len2, len2) >= 0)
					{
						SelfMinus64(pbuf1, len2 + 1, p2, len2);
						++q;
					}
				}
				else { q = 0; }
				if (destlen < len)p[destlen] = q;
				if (destlen != 0)for (size_t i = len2; i != 0; --i)pbuf1[i] = pbuf1[i - 1];//左移64bit
			}
			//结束部分，输出余数
			if (pd)
			{
				for (size_t i = 0; i < lend&&i < len2; ++i)pd[i] = pbuf1[i];
				for (size_t i = len2; i < lend; ++i)pd[i] = 0;
			}
			if (!ptmp)delete [] buf;
		}
		void DivideHigh64(u64*p, size_t len, cu64*p1, u64 len1, cu64*p2, u64 len2, bool mov, u64 *pd, size_t lend, u64*ptmp)
		{
			//除数为0直接返回
			if (len2 == 0)return;
			//被除数为0直接全0
			if (p1 == NULL || len1 == 0)
			{
				if (p)for (size_t i = 0; i != len; ++i)p[i] = 0;
				if (pd)for (size_t i = 0; i != lend; ++i)pd[i] = 0;
				return;
			}
			//除数长度为1
			if (len2 == 1)
			{
				u64 tmp = DivideInt64High64(p, (size_t)len, p1, (size_t)len1, *p2, mov);
				if (pd && lend != 0)
				{
					pd[lend - 1] = tmp;
					size_t i = lend - 1;
					if (len1 > len)
					{
						size_t j = (size_t)(len1 - len - 1);
						if (!mov || len == 0)while (j != 0 && i != 0)pd[--i] = p1[--j];
						else
						{
							mov = (p1[j] & 1) && ((*p2 & 1) == 0 || (*p & 1) == 0);
							size_t k = i;
							while (j != 0 && i != 0)
							{
								--i; --j;
								pd[i] = (p1[j] >> 1) | (p1[j + 1] << 63);
							}
							if (i != 0)pd[--i] = j != 0 ? (p1[j] << 63) : 0;
							if (k != 0)
							{
								if (mov)pd[k - 1] |= (1ull << 63);
								else pd[k - 1] &= 0x7fffffffffffffff;
							}
						}
					}
					while (i != 0)pd[--i] = 0;
				}
				return;
			}
			if (pd == NULL)lend = 0;
			size_t tmp = len + lend + 1;
			//除数长度计算
			if (len2 > tmp) { p2 += len2 - tmp; len2 = tmp; }
			//生商位置到此停止左移
			size_t stop = (size_t)(len2 > lend ? len2 - lend - 1 : 0);

			//获取试商乘积内存(自动释放)
			u8* buf = ptmp ?(u8*)ptmp:new u8[sizeof(u64)*(size_t)(len2 + 1) * 2];
			u64* pbuf1 = (u64*)(u8*)buf;
			u64* pbuf2 = (u64*)(u8*)buf + len2 + 1;
			//第一次试商
			u64 q[2];
			//复制被除数内容到pbuf1
			tmp = (size_t)(len > stop + 1 ? len2 : len2 + len - stop - 1);
			for (size_t i = (size_t)len1, j = tmp + 1; i != 0 && j != 0;)
			{
				--i; --j;
				pbuf1[j] = p1[i];
			}
			if (tmp >= len1)for (size_t i = (size_t)(tmp - len1);;) { pbuf1[i] = 0; if (i == 0)break; --i; }
			if (CompareHigh64(p1, p2, (size_t)len1, (size_t)len2) >= 0)
			{
				if (tmp < len2)SelfMinus64(pbuf1, tmp + 1, p2 + len2 - tmp - 1, tmp + 1);
				else SelfMinus64(pbuf1 + 1, tmp, p2 + len2 - tmp, tmp);
				q[1] = 1;
			}
			else
			{
				q[1] = 0;
			}
			size_t destlen = len;//生商位置
			tmp = (size_t)(len1 < len2 ? 0 : len1 - len2);//取值位置
			u64 dq = p2[len2 - 1] + 1;//试商除数
			while (destlen != 0)
			{
				--destlen;
				size_t k;
				if (destlen >= stop)
				{
					k = (size_t)len2;
					pbuf1[0] = tmp == 0 ? 0 : p1[--tmp];
				}
				else
					k = (size_t)(len2 + destlen - stop);
				if (pbuf1[k] == 0 && pbuf1[k - 1] < p2[len2 - 1])
				{
					q[0] = 0;
					size_t i = k - 1;
					while (i != 0 && pbuf1[i] == 0)i--;
					if (pbuf1[i] == 0)//整除
					{
						for (i = destlen;;) { p[i] = 0; if (i == 0)break; --i; }
						if (pd)for (i = 0; i != lend; ++i)pd[i] = 0;
					}
				}
				else {
					q[0] = dq != 0 ? Divide(pbuf1[k], pbuf1[k - 1], dq) : pbuf1[k];
					MultiplyInt64Low64(pbuf2, k + 1, p2 + len2 - k, k, q[0]);
					SelfMinus64(pbuf1, k + 1, pbuf2, k + 1);
					while (pbuf1[k] != 0 || CompareHigh64(pbuf1, p2 + len2 - k, k, k) >= 0)
					{
						if (SelfMinus64(pbuf1, k + 1, p2 + len2 - k, k))pbuf1[k]--;
						q[0]++;
					}
				}

				//商赋值
				if (mov)p[destlen] = (q[0] >> 1) | (q[1] << 63);
				else p[destlen] = q[0];
				//商左移
				q[1] = q[0];
				//被除数左移64bit
				if (destlen > stop)for (size_t i = (size_t)len2; i != 0; --i)pbuf1[i] = pbuf1[i - 1];
			}
			//输出余数
			if (pd != NULL&&lend != 0)
			{
				len2 -= stop;
				p2 += stop;
				if (mov)
				{
					if ((q[1] & 1) != 0)
					{
						SelfPlus64(pbuf1, len2 + 1, p2, len2, 0);
					}
					for (size_t i = (size_t)len2, j = (size_t)lend; j != 0 && i != 0; --i)
					{
						pd[--j] = len == 0 ? pbuf1[i - 1] : ShiftRight(pbuf1[i], pbuf1[i - 1], 1);
					}
					if (lend > len2)
					{
						if (tmp != 0)pd[lend - len2 - 1] = (pbuf1[0] << 63) | (p1[--tmp] >> 1);
						else pd[lend - len2 - 1] = (pbuf1[0] << 63);
						size_t i = (size_t)(lend - len2 - 1);
						if (tmp != 0)while (i != 0)
						{
							if (tmp != 0)
								pd[--i] = (p1[tmp] << 63) | (p1[tmp - 1] >> 1);
							else
							{
								pd[--i] = (p1[tmp] << 63);
								break;
							}
							--tmp;
						}
						while (i != 0)pd[--i] = 0;
					}
				}
				else
				{
					for (size_t i = (size_t)len2, j = (size_t)lend; j != 0 && i != 0;)
						pd[--j] = pbuf1[--i];
					if (lend > len2)for (size_t i = (size_t)(lend - len2); i != 0;)pd[--i] = tmp == 0 ? 0 : p1[--tmp];
				}
			}
			if (!ptmp)delete []buf;
		}
		void ModHigh64(u64*p, size_t len, u64 bits, cu64*p1, u64 len1, cu64*p2, u64 len2, bool mov, u64 *pd, size_t lend, u64*ptmp)
		{
			//除数为0直接返回
			if (len2 == 0)return;
			//被除数为0直接全0
			if (len1 == 0)
			{
				if (p)for (size_t i = 0; i != len; ++i)p[i] = 0;
				if (pd)for (size_t i = 0; i != lend; ++i)pd[i] = 0;
				return;
			}
			//除数长度计算
			size_t tmp = (size_t)(((bits + 63) >> 6) + lend + 1);
			if (len2 > tmp) { p2 += len2 - tmp; len2 = tmp; }
			//获取试商乘积内存(自动释放)
			u8* buf = ptmp ?(u8*)ptmp: new u8[sizeof(u64)*(size_t)(len2 + 1) * 2];
			u64* pbuf1 = (u64*)(u8*)buf;
			u64* pbuf2 = (u64*)(u8*)buf + len2 + 1;
			if (bits == 0 && mov &&lend != 0)//直接写入余数
			{
				/*//按余数总小于除数原则右移复制
				pd[lend - 1] = p1[len1 - 1] >> 1;
				for (size_t i = lend - 1, j = len1 - 1; j != 0 && i != 0;)
				{
				--j; --i;
				pd[i] = (p1[j+1] << 63) | (p1[j]>>1);
				}
				if (lend > len1)
				{
				pd[lend - len - 1] = *p1 << 63;
				for (size_t i = 0; i < lend - len - 1; ++i)pd[i]=0;
				}*/
				for (size_t i = (size_t)lend, j = (size_t)len1; j != 0 && i != 0;)
				{
					--j; --i;
					pd[i] = p1[j];
				}
				for (size_t i = 0; i + len1 < lend; ++i)pd[i] = 0;
				if (len != 0)memset(p, 0, sizeof(u64)*len);
				if (!ptmp)delete [] buf;
				return;
			}
			//开始试商
			u64 qarr[2] = { 0 };//商
			if (CompareHigh64(p1, p2, (size_t)len1, (size_t)len2) >= 0)
			{
				Minus64(pbuf1, len2 + 1, p1, len1, p2, len2, 64 * (s64)(len2 + 1 - len1), 64);
				if (bits == 0 || bits == (u64)mov)
				{
					if (len != 0)//商
					{
						if (mov)p[len - 1] = 1ull << 63;
						else p[len - 1] = 0;
						for (size_t i = len - 1; i != 0;)p[--i] = 0;
					}
					for (size_t i = (size_t)len2 + 1, j = (size_t)lend; i != 0 && j != 0;) { pd[--j] = pbuf1[--i]; }
					if (len1 > len2 && lend > len2)for (size_t i = (size_t)(len1 - len2 - 1), j = (size_t)(lend - len2 - 1); i != 0 && j != 0;) { pd[--j] = p1[--i]; }
					if (lend > len1)for (size_t i = 0; i < lend - len1; ++i)pd[i] = 0;
					if (!ptmp)delete [] buf;
					return;
				}
				qarr[1] = 1;
			}
			else
			{
				if (bits == 0 || bits == (u64)mov)//直接写入余数
				{
					for (size_t i = len; i != 0;)p[--i] = 0;//商0
					for (size_t i = (size_t)len1, j = (size_t)lend; i != 0 && j != 0;) { pd[--j] = p1[--i]; }
					if (lend > len1)for (size_t i = 0; i < lend - len1; ++i)pd[i] = 0;
					if (!ptmp)delete []buf;
					return;
				}
				size_t i = (size_t)len1, j = (size_t)len2 + 1;
				while (i != 0 && j != 0) { pbuf1[--j] = p1[--i]; }
				while (j != 0)pbuf1[--j] = 0;
			}
			u64 q = p2[len2 - 1] + 1;//试商除数
			size_t index1 = (size_t)(len1 > len2 ? len1 - len2 : 0);
			tmp = len;
			for (;;)
			{
				pbuf1[0] = index1 == 0 ? 0 : p1[--index1];
				qarr[0] = q == 0 ? pbuf1[len2] : Divide(pbuf1[len2], pbuf1[len2 - 1], q);//试商
				if ((s64)bits < 64 + mov)
				{
					bits = 64 + mov - bits;
					qarr[0] &= ((u64)-1) << bits;
					MultiplyInt64Low64(pbuf2, (size_t)len2 + 1, p2, (size_t)len2, qarr[0]);
					SelfMinus64(pbuf1, len2 + 1, pbuf2, len2 + 1);
					while ((pbuf1[len2] >> bits) != 0 || Compare(pbuf1, bits, len2 * 64, p2) >= 0)
					{
						SelfMinus64(pbuf1, len2 + 1, p2, len2, bits);
						qarr[0] += 1ull << bits;
					}
					if (tmp != 0)
					{
						if (mov)p[--tmp] = (qarr[0] >> 1) | (qarr[1] << 63);
						else p[--tmp] = qarr[0];
					}
					break;
				}
				MultiplyInt64Low64(pbuf2, (size_t)len2 + 1, p2, (size_t)len2, qarr[0]);
				SelfMinus64(pbuf1, len2 + 1, pbuf2, len2 + 1);
				while (pbuf1[len2] != 0 || CompareHigh64(pbuf1, p2, (size_t)len2, (size_t)len2) >= 0)
				{
					if (SelfMinus64(pbuf1, len2 + 1, p2, len2))pbuf1[len2]--;
					qarr[0]++;
				}
				if (tmp != 0) {
					if (mov)p[--tmp] = (qarr[0] >> 1) | (qarr[1] << 63);
					else p[--tmp] = qarr[0];
				}
				for (size_t i = (size_t)len2; i != 0; --i)pbuf1[i] = pbuf1[i - 1];//左移64bit
				if ((bits -= 64) == (u64)mov) { bits = 0; break; }
				qarr[1] = qarr[0];
			}
			//完整商剩余部位
			while (tmp != 0)p[--tmp] = 0;
			//输出余数
			size_t i = (size_t)lend, j = (size_t)len2 + 1;
			while (i != 0 && j != 1)
			{
				--i; --j;
				if (bits == 0)pd[i] = pbuf1[j];
				else pd[i] = (pbuf1[j] << (64 - bits)) | (pbuf1[j - 1] >> bits);
			}
			if (i != 0)
			{
				if (bits != 0)
				{
					pd[--i] = (pbuf1[0] << (64 - bits)) | (index1 == 0 ? 0 : (p1[--index1] >> bits));
					if (index1 != 0)while (i != 0)
					{
						--i;
						if (index1 != 0) {
							pd[i] = (p1[index1] << (64 - bits)) | (p1[index1 - 1] >> bits);
							--index1;
						}
						else { pd[i] = p1[0] << (64 - bits); break; }
					}
				}
				else
				{
					while (i != 0 && index1 != 0)pd[--i] = p1[--index1];
				}
				while (i != 0)pd[--i] = 0;
			}
			if (!ptmp)delete []buf;
		}
	}
}
#endif