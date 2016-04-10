#include"stdafx.h"
#ifdef LK_BITS_PLUS_
namespace LK{
	namespace Bits {
		bool SelfPlusOne64(u64*p, u64 len, s64 mov)
		{
			u64 bm = 1ull << (mov & 63);
			if (mov < 0 || (mov /= 64) >= (index_t)len)return false;
			if (p[mov] >(~bm))
			{
				u64*pe = p + len;
				*(p += mov) += bm;
				while (++p != pe)if (++*p != 0)break;
				return p == pe;
			}
			p[mov] += bm;
			return false;
		}
		bool SelfPlusOneRound64(u64*p, u64 len, s64 mov)
		{
			u64 bm = 1ull << (mov & 63);
			if (mov < 0 || (mov /= 64) >= (index_t)len)
			{
				if (mov == -1)
				{
					u64*pe = p + len;
					while (p != pe&&++*p == 0)++p;
					return p == pe;
				}
				return false;
			}
			if (p[mov] >(~bm))
			{
				u64*pe = p + len;
				*(p += mov) += bm;
				while (++p != pe)if (++*p != 0)return false;
				return true;
			}
			p[mov] += bm;
			return false;
		}
		bool SelfPlusInt64(u64*p, u64 len, u64 num, s64 mov)
		{
			if (mov <= -64 || (mov / 64) >= (index_t)len)return false;
			u64*pe = p + len;
			if (mov<0)
			{
				mov &= 63;
				num >>= (64 - mov);
				if (*p > ~num)
				{
					*p += num;
					while (++p != pe)if (++*p != 0)return false;
					return true;
				}
				*p += num;
				return false;
			}
			p += mov / 64;
			if ((mov &= 63) == 0)
			{
				if (*p > ~num)
				{
					*p += num;
					while (++p != pe)if (++*p != 0)return false;
					return true;
				}
				*p += num;
				return false;
			}
			u64 lm = num << mov;
			num >>= 64 - mov;
			if (*p > (~lm)) { num++; (*p++) += lm; if (p == pe)return true; }
			else (*p++) += lm;
			if (p == pe)return false;
			if (*p > (~num))
			{
				*p += num;
				while (++p != pe)if (++*p != 0)return false;
				return true;
			}
			*p += num;
			return false;
		}
		bool SelfPlusIntRound64(u64*p, u64 len, u64 num, s64 mov)
		{
			if (mov >= 0 && (mov / 64) >= (index_t)len)return false;
			if (mov < -64)return false;
			u64*pe = p + len;
			if (mov == -64)
			{
				if ((s64)num < 0)
				{
					while (p != pe)if (++*p != 0)return false;
					return true;
				}
				return false;
			}
			if (mov<0)
			{
				mov = -mov;
				if (len == 0)return ((num >> (mov - 1)) & 1) != 0;
				num = (num >> mov) + ((num >> (mov - 1)) & 1);
				if (*p > ~num)
				{
					*p += num;
					while (++p != pe)if (++*p != 0)return false;
					return true;
				}
				*p += num;
				return false;
			}
			p += mov / 64;
			if ((mov &= 63) == 0)
			{
				if (*p > ~num)
				{
					*p += num;
					while (++p != pe)if (++*p != 0)return false;
					return true;
				}
				*p += num;
				return false;
			}
			u64 lm = num << mov;
			num >>= 64 - mov;
			if (*p > (~lm)) { num++; (*p++) += lm; if (p == pe)return true; }
			else (*p++) += lm;
			if (p == pe)return false;
			if (*p > (~num))
			{
				*p += num;
				while (++p != pe)if (++*p != 0)return false;
				return true;
			}
			*p += num;
			return false;
		}
		bool SelfPlus64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov)
		{
			size_t destStartIndex = (size_t)(mov < 64 ? 0 : (mov >> 6));
			mov = mov<0 ? -mov : -(mov & 63);
			bool flag = false;
			while (destStartIndex < lenDest)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, mov);
				if (flag)
				{
					flag = pDest[destStartIndex] >= ~tmp;
					pDest[destStartIndex] += tmp + 1;
				}
				else
				{
					flag = pDest[destStartIndex] > ~tmp;
					pDest[destStartIndex] += tmp;
				}
				mov += 64;
				if (tmp == 0 && flag == false && mov > (s64)lenSource * 64)return false;
				destStartIndex++;
			}
			return flag;
		}
		bool SelfPlusRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov)
		{
			size_t destStartIndex = (size_t)(mov < 64 ? 0 : (mov >> 6));
			bool flag = mov < 0 ? (s64)GetWindow64Low64(pSource, (size_t)lenSource, -64 - mov) < 0 : false;
			mov = mov<0 ? -mov : -(mov & 63);
			while (destStartIndex < lenDest)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, mov);
				if (flag)
				{
					flag = pDest[destStartIndex] >= ~tmp;
					pDest[destStartIndex] += tmp + 1;
				}
				else
				{
					flag = pDest[destStartIndex] > ~tmp;
					pDest[destStartIndex] += tmp;
				}
				mov += 64;
				if (tmp == 0 && flag == false && mov > (s64)lenSource * 64)return false;
				destStartIndex++;
			}
			return flag;
		}
		bool PlusOne64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 movSource, s64 mov)
		{
			movSource = -movSource;
			u64 ull = 1ull << (mov & 63);
			if (mov > 0)mov = mov / 64;
			bool flag = false;
			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				if (flag) {
					flag = (pDest[i] = ++tmp) == 0;
				}
				else if ((s64)i == mov) {
					flag = tmp > ~ull;
					pDest[i] = tmp + ull;
				}
				else pDest[i] = tmp;
			}
			return flag;
		}
		bool PlusOneRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 movSource, s64 mov)
		{
			movSource = -movSource;
			u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource - 64);
			u64 ull = 1ull << (mov & 63);
			bool flag = (s64)tmp < 0;
			if (mov >= 0)
			{
				mov = mov / 64;
			}
			else if (mov >= -64)
			{
				flag = flag || (s64)(tmp + ull) < 0;
				mov = 0; ull = 0;
			}
			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				if ((s64)i == mov)
				{
					if (flag) {
						flag = tmp >= ~ull;
						pDest[i] = tmp + ull + 1;
					}
					else {
						flag = tmp > ~ull;
						pDest[i] = tmp + ull;
					}
				}
				else
				{
					if (flag)flag = (pDest[i] = tmp + 1) == 0;
					else pDest[i] = tmp;
				}
			}
			return flag;
		}
		bool PlusInt64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov)
		{
			movSource = -movSource;
			u64 ull = num << (mov & 63);
			num = (mov & 63) == 0 ? 0 : num >> (64 - (mov & 63));
			if (mov >= 0)mov = mov / 64;
			else { mov = (mov - 63) / 64; }
			bool flag = false;
			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				u64 t;
				if ((s64)i == mov)t = ull;
				else if ((s64)i == mov + 1)t = num;
				else if (!flag) { pDest[i] = tmp; continue; }
				else t = 0;

				if (flag)
				{
					flag = tmp >= ~t;
					pDest[i] = tmp + t + 1;
				}
				else
				{
					flag = tmp > ~t;
					pDest[i] = tmp + t;
				}
			}
			return flag;
		}
		index_t PlusIntRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov)
		{
			movSource = -movSource;
			u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource - 64);
			u64 ull = num << (mov & 63);
			num = (mov & 63) == 0 ? 0 : num >> (64 - (mov & 63));
			index_t flag = 0;
			if (mov >= 0)
			{
				mov = mov / 64;
				flag = (s64)tmp < 0;
			}
			else
			{
				if (mov >= -64)flag = (index_t)(tmp > ~ull) + ((s64)(tmp + ull) < 0);
				else if (mov > -128)flag = (index_t)(tmp > ~num) + ((s64)(tmp + num) < 0);
				else flag = (s64)tmp < 0;
				mov = (mov - 63) / 64;
			}

			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				u64 t = (s64)i == mov ? ull : (s64)i == mov + 1 ? num : 0;
				if (flag != 0) {
					if (flag == 2)
					{
						flag = (s64)tmp != -1ll ? tmp + 1 >= ~t : 1;
						pDest[i] = tmp + t + 2;
					}
					else
					{
						flag = tmp >= ~t;
						pDest[i] = tmp + t + 1;
					}
				}
				else
				{
					flag = tmp > ~t;
					pDest[i] = tmp + t;
				}
			}
			return flag;
		}
		bool Plus64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2)
		{
			mov1 = -mov1;
			mov2 = -mov2;
			bool flag = false;
			for (size_t i = 0; i < lenDest; ++i)
			{
				u64 tmp1 = GetWindow64Low64(p1, (size_t)len1, mov1);
				u64 tmp2 = GetWindow64Low64(p2, (size_t)len2, mov2);
				if (flag)
				{
					flag = tmp1 >= ~tmp2;
					pDest[i] = tmp1 + tmp2 + 1;
				}
				else
				{
					flag = tmp1 > ~tmp2;
					pDest[i] = tmp1 + tmp2;
				}
				mov1 += 64;
				mov2 += 64;
			}
			return flag;
		}
		index_t PlusRound64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2)
		{
			mov1 = -mov1;
			mov2 = -mov2;
			u64 tmp1 = GetWindow64Low64(p1, (size_t)len1, mov1 - 64);
			u64 tmp2 = GetWindow64Low64(p2, (size_t)len2, mov2 - 64);
			index_t flag = (index_t)(tmp1 > ~tmp2) + ((s64)(tmp1 + tmp2) < 0);

			for (size_t i = 0; i < lenDest; ++i)
			{
				tmp1 = GetWindow64Low64(p1, (size_t)len1, mov1);
				tmp2 = GetWindow64Low64(p2, (size_t)len2, mov2);
				if (flag != 0)
				{
					if (flag == 2)
					{
						flag = tmp1 == (u64)-1 && tmp2 == (u64)-1 ? 2 :
							(s64)tmp1 != -1ll ? tmp1 + 1 >= ~tmp2 : 1;
						pDest[i] = tmp1 + tmp2 + 2;
					}
					else
					{
						flag = tmp1 >= ~tmp2;
						pDest[i] = tmp1 + tmp2 + 1;
					}
				}
				else
				{
					flag = tmp1 > ~tmp2;
					pDest[i] = tmp1 + tmp2;
				}
				mov1 += 64;
				mov2 += 64;
			}
			return flag;
		}
		bool Plus64(u64 *pDes, cu64* pSource, size_t len)
		{
			size_t i = 0;
			bool flag = false;
			while (i < len)
			{
				if (flag)
				{
					flag = pDes[i] >= (MaxValues::UInt64 ^ pSource[i]);
					pDes[i] += pSource[i] + 1;
				}
				else
				{
					flag = pDes[i] >(MaxValues::UInt64 ^ pSource[i]);
					pDes[i] += pSource[i];
				}
				++i;
			}
			return flag;
		}
	}
}
#endif