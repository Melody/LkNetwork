#include"stdafx.h"
#ifdef LK_BITS_MINUS_
namespace LK{
	namespace Bits {
		bool SelfMinusOne64(u64*p, u64 len, s64 mov)
		{
			if (mov >= (s64)len * 64 || mov < 0)return false;//不在数组范围内
			p += mov / 64;
			len -= mov / 64;
			mov = 1ull << (mov & 63);
			if (*p >= (u64)mov) { *p -= mov; return false; }
			*p -= mov;
			for (u64 i = 1; i != len; ++i)if (p[i]-- != 0)return false;
			return true;
		}
		bool SelfMinusInt64(u64*p, u64 len, u64 num, s64 mov)
		{
			if (mov <= -64 || (mov / 64) >= (s64)len)return false;
			u64*pe = p + len;
			if (mov < 0)
			{
				mov &= 63;
				num >>= (64 - mov);
				if (*p < num)
				{
					*p -= num;
					while (++p != pe)if ((*p)-- != 0)return false;
					return true;
				}
				*p -= num;
				return false;
			}
			p += mov / 64;
			if ((mov &= 63) == 0)
			{
				if (*p < num)
				{
					*p -= num;
					while (++p != pe)if ((*p)-- != 0)return false;
					return true;
				}
				*p -= num;
				return false;
			}
			u64 lm = num << mov;
			num >>= 64 - mov;
			if (*p < lm) { num++; (*p++) -= lm; if (p == pe)return true; }
			else (*p++) -= lm;
			if (p == pe)return false;
			if (*p < num)
			{
				*p -= num;
				while (++p != pe)if ((*p)-- != 0)return false;
				return true;
			}
			*p -= num;
			return false;
		}
		bool SelfMinusIntRound64(u64*p, u64 len, u64 num, s64 mov)
		{
			if (mov >= 0 && (mov / 64) >= (s64)len)return false;
			if (mov < -64)return false;
			u64*pe = p + len;
			if (mov < 0)
			{
				mov &= 63;
				if (p == pe)return (-(s64)(num << (mov))>0);
				num = (mov == 0) ? (-(s64)num>0) : ((num >> (64 - mov)) + (-(s64)(num << (mov))>0));
				if (*p < num)
				{
					*p -= num;
					while (++p != pe)if ((*p)-- != 0)return false;
					return true;
				}
				*p -= num;
				return false;
			}
			p += mov / 64;
			if ((mov &= 63) == 0)
			{
				if (*p < num)
				{
					*p -= num;
					while (++p != pe)if ((*p)-- != 0)return false;
					return true;
				}
				*p -= num;
				return false;
			}
			u64 lm = num << mov;
			num >>= 64 - mov;
			if (*p < lm) { num++; (*p++) -= lm; if (p == pe)return true; }
			else (*p++) -= lm;
			if (p == pe)return false;
			if (*p < num)
			{
				*p -= num;
				while (++p != pe)if ((*p)-- != 0)return false;
				return true;
			}
			*p -= num;
			return false;
		}
		bool SelfMinus64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov)
		{
			size_t destStartIndex = (size_t)(mov < 64 ? 0 : (mov >> 6));
			mov = mov < 0 ? -mov : -(mov & 63);
			bool flag = false;
			while (destStartIndex < lenDest)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, mov);
				if (flag)
				{
					flag = pDest[destStartIndex] <= tmp;
					pDest[destStartIndex] -= tmp + 1;
				}
				else
				{
					flag = pDest[destStartIndex] <tmp;
					pDest[destStartIndex] -= tmp;
				}
				mov += 64;
				if (tmp == 0 && flag == false && mov > (s64)lenSource * 64)return false;
				destStartIndex++;
			}
			return flag;
		}
		bool SelfMinus64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource)
		{
			bool flag = false;
			for (size_t destStartIndex = 0; destStartIndex < lenDest; destStartIndex++)
			{
				u64 tmp = destStartIndex < lenSource ? pSource[destStartIndex] : 0;
				if (flag)
				{
					flag = pDest[destStartIndex] <= tmp;
					pDest[destStartIndex] -= tmp + 1;
				}
				else
				{
					flag = pDest[destStartIndex] < tmp;
					pDest[destStartIndex] -= tmp;
				}
				if (tmp == 0 && flag == false && destStartIndex >= lenSource)return false;
			}
			return flag;
		}
		bool SelfMinusRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov)
		{
			size_t destStartIndex = (size_t)(mov < 64 ? 0 : (mov >> 6));
			bool flag = mov < 0 ? (-(s64)GetWindow64Low64(pSource, (size_t)lenSource, -64 - mov))>0 : false;
			mov = mov < 0 ? -mov : -(mov & 63);
			while (destStartIndex < lenDest)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, mov);
				if (flag)
				{
					flag = pDest[destStartIndex] <= tmp;
					pDest[destStartIndex] -= tmp + 1;
				}
				else
				{
					flag = pDest[destStartIndex] <tmp;
					pDest[destStartIndex] -= tmp;
				}
				mov += 64;
				if (tmp == 0 && flag == false && mov > (s64)lenSource * 64)return false;
				destStartIndex++;
			}
			return flag;
		}
		bool MinusOne64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 movSource, s64 mov)
		{
			movSource = -movSource;
			u64 ull = 1ull << (mov & 63);
			if (mov > 0)mov = mov / 64;
			bool flag = false;
			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				if (flag) {
					flag = (pDest[i] = tmp)-- == 0;
				}
				else if ((s64)i == mov) {
					flag = tmp < ull;
					pDest[i] = tmp - ull;
				}
				else pDest[i] = tmp;
			}
			return flag;
		}
		index_t MinusOneRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 movSource, s64 mov)
		{
			movSource = -movSource;
			u64 ull = 1ull << (mov & 63);
			index_t flag = 0;
			if (mov >= 0) { mov = mov / 64; flag = (s64)GetWindow64Low64(pSource, (size_t)lenSource, movSource - 64) < 0; }
			else if (mov >= -64)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource - 64);
				if (tmp >= ull)flag = (s64)(tmp - ull) < 0;
				else flag = (s64)(tmp - ull) > 0 ? -1 : 0;
			}
			else flag = (s64)GetWindow64Low64(pSource, (size_t)lenSource, movSource - 64) < 0;
			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				u64 tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				if ((s64)i == mov)
				{
					if (flag > 0)ull -= 1;
					flag = tmp < ull ? -1 : 0;
					pDest[i] = tmp - ull;
				}
				else if (flag != 0)
				{
					if (flag > 0)flag = (pDest[i] = tmp + 1) == 0;
					else flag = (pDest[i] = tmp)-- == 0 ? -1 : 0;
				}
				else pDest[i] = tmp;
			}
			return flag;
		}
		bool MinusInt64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov)
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
					flag = tmp <= t;
					pDest[i] = tmp - t - 1;
				}
				else
				{
					flag = tmp < t;
					pDest[i] = tmp - t;
				}
			}
			return flag;
		}
		index_t MinusIntRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov)
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
				if (mov >= -64)
				{
					if (tmp >= ull)flag = (s64)(tmp - ull) < 0;
					else flag = (s64)(tmp - ull) > 0 ? -1 : 0;
				}
				else if (mov > -128)
				{
					if (tmp >= num)flag = (s64)(tmp - num) < 0;
					else flag = (s64)(tmp - num) > 0 ? -1 : 0;
				}
				else flag = (s64)tmp < 0;
				mov = (mov - 63) / 64;
			}

			for (u64 i = 0; i < lenDest; ++i, movSource += 64)
			{
				tmp = GetWindow64Low64(pSource, (size_t)lenSource, movSource);
				if ((s64)i == mov)
				{
					if (ull != 0)
					{
						if (flag > 0)ull -= 1;
						flag = tmp < ull ? -1 : 0;
						pDest[i] = tmp - ull;
					}
					else if (flag > 0) flag = (pDest[i] = tmp + 1) == 0;
					else pDest[i] = tmp;
				}
				else if ((s64)i == mov + 1)
				{
					if (num != 0)
					{
						if (flag != 0)num -= flag;
						flag = tmp < num ? -1 : 0;
						pDest[i] = tmp - num;
					}
					else if (flag != 0)
					{
						if (flag > 0)flag = (pDest[i] = tmp + 1) == 0;
						else flag = (s64)(pDest[i] = tmp - 1) == -1 ? -1 : 0;
					}
					else pDest[i] = tmp;
				}
				else if (flag != 0)
				{
					if (flag > 0)flag = (pDest[i] = tmp + 1) == 0;
					else flag = (s64)(pDest[i] = tmp - 1) == -1 ? -1 : 0;
				}
				else pDest[i] = tmp;
			}
			return flag;
		}
		bool Minus64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2)
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
					flag = tmp1 <= tmp2;
					pDest[i] = tmp1 - tmp2 - 1;
				}
				else
				{
					flag = tmp1 < tmp2;
					pDest[i] = tmp1 - tmp2;
				}
				mov1 += 64;
				mov2 += 64;
			}
			return flag;
		}
		index_t MinusRound64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2)
		{
			mov1 = -mov1;
			mov2 = -mov2;
			u64 tmp1 = GetWindow64Low64(p1, (size_t)len1, mov1 - 64);
			u64 tmp2 = GetWindow64Low64(p2, (size_t)len2, mov2 - 64);
			index_t flag = tmp1 >= tmp2 ? (s64)(tmp1 - tmp2) < 0 : ((s64)(tmp1 - tmp2) > 0 ? -1 : 0);
			for (size_t i = 0; i < lenDest; ++i)
			{
				tmp1 = GetWindow64Low64(p1, (size_t)len1, mov1);
				tmp2 = GetWindow64Low64(p2, (size_t)len2, mov2);
				if (flag != 0)
				{
					if (flag < 0)
					{
						flag = tmp1 <= tmp2 ? -1 : 0;
						pDest[i] = tmp1 - tmp2 - 1;
					}
					else
					{
						flag = tmp2 == 0 ? (s64)tmp1 == -1 : tmp1 < tmp2 - 1 ? -1 : 0;
						pDest[i] = tmp1 - tmp2 + 1;
					}
				}
				else
				{
					flag = tmp1 < tmp2 ? -1 : 0;
					pDest[i] = tmp1 - tmp2;
				}
				mov1 += 64;
				mov2 += 64;
			}
			return flag;
		}
	}
}
#endif