#include "stdafx.h"
#ifdef LK_MATH_INT_
namespace LK
{
	namespace Math
	{
		bool Int::BitsPlus(u64 *pdes, u64 * psource, u64 deslen, u64 sourcelen)
		{
			if (sourcelen > deslen)Throw<std::invalid_argument>("参数：sourcelen大于deslen");
			bool flag = false;
			u64 i = 0;
			while (i<sourcelen)
			{
				if (flag)
				{
					flag = pdes[i] >= ~psource[i];
					pdes[i] += psource[i] + 1;
				}
				else
				{
					flag = pdes[i]>~psource[i];
					pdes[i] += psource[i];
				}
				++i;
			}
			if (flag)while (i < deslen)if (++pdes[i] != 0)return false;
			return flag;
		}
		bool Int::BitsMinus(u64 *pdes, u64 * psource, u64 deslen, u64 sourcelen)
		{
			bool flag = false;
			u64 i = 0;
			while (i < sourcelen)
			{
				if (flag)
				{
					flag = pdes[i] <= psource[i];
					pdes[i] -= psource[i] + 1;
				}
				else
				{
					flag = pdes[i] < psource[i];
					pdes[i] += psource[i];
				}
				++i;
			}
			if (flag)while (i < deslen)if (pdes[i]-- != 0)return false;
			return flag;
		}
		void Int::BitsMinus(u64* pDat, u64* pNum, u64 numl64)
		{
			bool flag(false);
			for (u64 i(0); i < numl64; ++i)
			{
				if (flag)
				{
					flag = pNum[i] >= pDat[i];
					pDat[i] -= pNum[i] + 1;
				}
				else
				{
					flag = pNum[i] > pDat[i];
					pDat[i] -= pNum[i];
				}
			}
			if (flag)--pDat[numl64];
		}
		Int Int::AbsPlus(const Int&vi1, const Int&vi2)
		{
			if (vi1.IsError())return vi1;
			if (vi2.Equals0())return vi1.Abs();
			if (vi2.IsError())return vi2;
			if (vi1.Equals0())return vi2.Abs();
			size_t l64 = (size_t)(((vi1.BitsCount > vi2.BitsCount ? vi1.BitsCount : vi2.BitsCount) + 63) / 64 + 1);
			size_t maxs = Memory::GetResize(l64*sizeof(u64)) / sizeof(u64);
			u64* pdat = Malloc<u64>(maxs);
			u64 len = ((vi1.BitsCount > vi2.BitsCount ? vi2.BitsCount : vi1.BitsCount) + 63) / 64;
			bool flag = false;
			size_t i = 0;
			while (i<len)
			{
				if (flag)
				{
					flag = vi1.pData[i] >= ~vi2.pData[i];
					pdat[i] = vi1.pData[i] + vi2.pData[i] + 1;
				}
				else
				{
					flag = vi1.pData[i]>~vi2.pData[i];
					pdat[i] = vi1.pData[i] + vi2.pData[i];
				}
				i++;
			}
			while (i < l64 - 1)
			{
				flag = (pdat[i] = (vi1.BitsCount > vi2.BitsCount ? vi1.pData[i] : vi2.pData[i]) + flag) == 0;
				++i;
			}
			pdat[i] = flag;
			u64 bc = flag ? i * 64ull + 1 : i * 64ull - Bits::LeftZeroCount(pdat[i - 1]);
			if (bc > MaxBitsCount)return Int(pdat, l64, maxs, bc, MathError::PlusExpOverflow);
			return Int(pdat, l64, maxs, bc, 1);
		}
		Int Int::AbsMinus(const Int&vi1, const Int&vi2)
		{
			if (vi1.IsError())return vi1;
			if (vi2.IsError())return vi2;
			if (vi1.Equals0())return vi2.Abs();
			if (vi2.Equals0())return vi1.Abs();
			size_t l64 = (size_t)(((vi1.BitsCount > vi2.BitsCount ? vi1.BitsCount : vi2.BitsCount) + 63) / 64);
			size_t len = (size_t)(((vi1.BitsCount < vi2.BitsCount ? vi1.BitsCount : vi2.BitsCount) + 63) / 64);
			size_t maxs = Memory::GetResize(l64*sizeof(u64)) / sizeof(u64);
			u64* pdat = Malloc<u64>(maxs);
			u64* p1, *p2;
			if (AbsCompare(vi1, vi2) > 0) { p1 = vi1.pData; p2 = vi2.pData; }
			else { p1 = vi2.pData; p2 = vi1.pData; }
			size_t i = 0;
			bool flag = false;
			while (i < len)
			{
				if (flag)
				{
					flag = p1[i] <= p2[i];
					pdat[i] = p1[i] - p2[i] - 1;
				}
				else
				{
					flag = p1[i] < p2[i];
					pdat[i] = p1[i] - p2[i];
				}
				i++;
			}
			while (i < l64)
			{
				if (flag) {
					if (p1[i] == 0)pdat[i] = -1;
					else { pdat[i] = p1[i] - 1; flag = false; }
				}
				else pdat[i] = p1[i];
			}
			while (l64 != 0 && pdat[--l64] == 0);
			u64 bc;
			if (l64 != 0 || pdat[0] != 0) { bc = l64 * 64ull + 64 - Bits::LeftZeroCount(pdat[l64]); }
			else bc = 0;
			return Int(pdat, l64, maxs, bc, bc != 0);
		}
		u64 Int::MaxLen64 = ((u64)(1ull << (sizeof(size_t) * 8 - 1))) / 64;
		u64 Int::MaxBitsCount = ((u64)(MaxLen64 * 64)) - 1;

		u64 Int::FindBitsCount(size_t l64)
		{
			if (l64 != 0)while (--l64 != 0)if (pData[l64] != 0)break;
			len64 = l64 + 1;
			return (l64 << 6) + 64 - LK::Bits::LeftZeroCount(pData[l64]);
		}
		u32 Int::GetBits32(u64 mov)const
		{//mov为读取数据起始位，如当mov=64时将从pData[1]的0bit开始读取
			if ((mov & 0x3f) > 32)return (u32)((pData[(mov >> 6) + 1] << (64 - (mov & 0x3f))) | (pData[mov >> 6] >> (mov & 0x3f)));
			if (mov & 0x20)return pData[mov >> 6] >> 32;
			return (u32)(pData[mov >> 6] >> (mov & 0x1f));
		}
		u64 Int::GetBits64(u64* pDat, u64 mov)
		{//mov为读取数据起始位，如当mov=64时将从pData[1]的0bit开始读取
			if (mov & 0x3f)
				return (pDat[(mov >> 6) + 1] << (64 - (mov & 0x3f))) | (pDat[mov >> 6] >> (mov & 0x3f));
			return pDat[mov >> 6];
		}
		void Int::SetBits32(ui32 num, u64 mov)
		{//mov为设置起始位，如当mov=64时，将在pData[1]的0bit开始写入
			pData[mov >> 6] |= (u64)num << (mov & 0x3f);
			if ((mov & 0x3f) > 32 && (num >> (32 - (mov & 0x1f))))pData[1 + (mov >> 6)] |= num >> (32 - (mov & 0x1f));
		}
		bool Int::HasBits(u64 mov)const
		{//mov为检验bit总数，如当mov=64则刚好等于检验pData[0]是否有数据
			u64 l64((mov + 0x3f) >> 6);//涉及元素数量
			if (l64-- != 0)
			{
				if (mov &= 0x3f)//非整
				{
					if ((pData[l64] << (64 - mov)) != 0 || pData[--l64] != 0)return true;
				}
				while (l64 != 0)if (pData[l64--] != 0)return true;
				return pData[0] != 0;
			}
			return false;
		}
		void Int::BitsMultiply(u64 *pDes, u64 num)const
		{
			u64 t(0);
			u64 l64((BitsCount + 63) / 64);
			for (u64 i(0); i < l64; ++i)
			{
				t += (pData[i] & 0xffffffff) * num;
				pDes[i] = t & 0xffffffff;
				t >>= 32;
				t += (pData[i] >> 32) * num;
				pDes[i] |= t << 32;
				t >>= 32;
			}
			pDes[l64] = t;
		}
		void Int::BitsMovLeft32(u64* pDat, u64 l64)
		{
			while (--l64)
				pDat[l64] = (pDat[l64] << 32) | (pDat[l64 - 1] >> 32);
			pDat[0] <<= 32;
		}
		bool Int::BitsNotLessThan(u64* pDat1, u64*pDat2, u64 l64)
		{
			while (--l64 != 0)
			{
				if (pDat1[l64] != pDat2[l64])break;
			}
			return pDat1[l64] >= pDat2[l64];
		}
		int Int::AbsCompare(const Int& vi1, const Int& vi2)
		{
			if (vi1.BitsCount != vi2.BitsCount)return vi1.BitsCount > vi2.BitsCount ? 1 : -1;
			if (vi1.BitsCount == 0)return 0;
			for (s32 i = s32((vi1.BitsCount - 1) / 64); i >= 0; --i)
			{
				if (vi1.pData[i] != vi2.pData[i])return vi1.pData[i] > vi2.pData[i] ? 1 : -1;
			}
			return 0;
		}
		Int& Int::operator=(const Int& li)
		{
			if (this == &li)return *this;
			if (li.IsError())return MakeError(li.Sign);
			_Relen64((size_t)((li.BitsCount + 63) / 64));
			for (size_t i = 0; i < len64; ++i)pData[i] = li.pData[i];
			BitsCount = li.BitsCount;
			Sign = li.Sign;
			return *this;
		}
		//构造
		Int::Int(const Int&v) :BitsCount(0), pData(NULL), len64(0), maxSize(0), Sign(0)//仅复制有效数据部分
		{
			_Relen64(v.len64);
			for (size_t i = 0; i < len64; ++i)pData[i] = v.pData[i];
			BitsCount = v.BitsCount;
			Sign = v.Sign;
		}
#ifdef LK_MATH_FLOAT256_
		//从Float256构造
		Int::Int(const Float256&f)
			: BitsCount(0), pData(NULL), len64(0), maxSize(0), Sign(0)//创建长度为0的整数
		{
			if (f.Exp > (s32)MaxBitsCount)MakeError(-2);
			else
			{
				_Relen64(4);
				pData[0] = f.Data.ull[0];
				pData[1] = f.Data.ull[1];
				pData[2] = f.Data.ull[2];
				pData[3] = f.Data.ull[3];
				BitsCount = FindBitsCount(4);
				Sign = f.Sign;
				*this >>= 255 - f.Exp;
			}
		}
#endif
		Int::Int(u64 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0)
		{
			_Relen64(1);
			*pData = num;
		}
		Int::Int(u32 num)
			: BitsCount(32 - Bits::LeftZeroCount32(num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0)
		{
			_Relen64(1);
			*pData = num;
		}
		Int::Int(ui32 num)
			: BitsCount(32 - Bits::LeftZeroCount32(num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0)
		{
			_Relen64(1);
			*pData = num;
		}
		Int::Int(u16 num)
			: BitsCount(32 - Bits::LeftZeroCount32(num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0)
		{
			_Relen64(1);
			*pData = num;
		}
		Int::Int(u8 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0)
		{
			_Relen64(1);
			*pData = num;
		}
		Int::Int(s64 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num < 0 ? -num : num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0 ? 1 : num < 0 ? -1 : 0)
		{
			_Relen64(1);
			*pData = num < 0 ? -num : num;
		}
		Int::Int(s32 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num < 0 ? -num : num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0 ? 1 : num < 0 ? -1 : 0)
		{
			_Relen64(1);
			*pData = num < 0 ? -(s32)num : num;
		}
		Int::Int(i32 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num < 0 ? -num : num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0 ? 1 : num < 0 ? -1 : 0)
		{
			_Relen64(1);
			*pData = num < 0 ? -(s32)num : num;
		}
		Int::Int(s16 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num < 0 ? -num : num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0 ? 1 : num < 0 ? -1 : 0)
		{
			_Relen64(1);
			*pData = num < 0 ? -(s32)num : num;
		}
		Int::Int(s8 num)
			: BitsCount(64 - Bits::LeftZeroCount64(num < 0 ? -num : num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0 ? 1 : num < 0 ? -1 : 0)
		{
			_Relen64(1);
			*pData = num < 0 ? -(s32)num : num;
		}
		Int::Int(char num)
			: BitsCount(64 - Bits::LeftZeroCount64(num < 0 ? -num : num)), pData(NULL), len64(0), maxSize(0), Sign(num > 0 ? 1 : num < 0 ? -1 : 0)
		{
			_Relen64(1);
			*pData = num < 0 ? -(s32)num : num;
		}
		Int::Int(double num)
			: BitsCount(0), pData(0), maxSize(0), Sign(num >= 1 ? 1 : num <= -1 ? -1 : 0)
		{
			union
			{
				double d;
				u64 ull;
			};
			d = num;
			if (num < 0)Sign = -1;
			short exp = ((ull >> 52) & 0x7ff) - 1023;
			if (exp < 0)//小于1
				len64 = 0;
			else
			{
				BitsCount = exp + 1;
				len64 = (exp >> 6) + 1;
				if (len64)
				{
					_Relen64(len64);
					for (size_t i = 0; i < len64 - 1; ++i)pData[i] = 0;
					pData[len64 - 1] = (ull << 11) | MinValues::LongLong;
					if ((exp = 0x3f ^ (exp & 0x3f)) != 0)//需要右移位数
					{
						if (len64 > 1 && exp>11)pData[len64 - 2] = pData[len64 - 1] << (75 - exp);
						pData[len64 - 1] >>= exp;
					}
				}
			}
		}
		Int::Int(float num)
			: BitsCount(0), pData(0), maxSize(0), Sign(num >= 1 ? 1 : num <= -1 ? -1 : 0)
		{
			union
			{
				double d;
				u64 ull;
			};
			d = num;
			if (num < 0)Sign = -1;
			short exp = ((ull >> 52) & 0x7ff) - 1023;
			if (exp < 0)//小于1
				len64 = 0;
			else
			{
				BitsCount = exp + 1;
				len64 = (exp >> 6) + 1;
				if (len64)
				{
					_Relen64(len64);
					for (size_t i = 0; i < len64 - 1; ++i)pData[i] = 0;
					pData[len64 - 1] = (ull << 11) | MinValues::LongLong;
					if ((exp = 0x3f ^ (exp & 0x3f)) != 0)//需要右移位数
					{
						if (len64 > 1 && exp>11)pData[len64 - 2] = pData[len64 - 1] << (75 - exp);
						pData[len64 - 1] >>= exp;
					}
				}
			}
		}
		Int::Int(char const* str) : BitsCount(0), pData(NULL), len64(0), maxSize(0), Sign(0)
		{
			ui32 strlen(0);
			unsigned numlen(0);
			char flag(1);
			while ('-' == str[strlen] || '+' == str[strlen] || (str[strlen] >= '0' && str[strlen] <= '9'))
			{
				if (str[strlen] > '0' && str[strlen] <= '9')++numlen;
				else if (str[strlen] == '0' && numlen)++numlen;
				++strlen;
			}
			Relen64((size_t)(((u64)(numlen * 3.3219281) + 0x40) >> 6));//相当精确地调整内存
			if (strlen)
			{
				ui32 num(0);
				ui32 i(0);
				ui32 len(0);
				ui32 k(0);
				for (; i < strlen; ++i)
				{
					if ('-' == str[i])flag = -flag;
					else if ('+' == str[i])continue;
					else
					{
						num += str[i] - '0';
						if (k > 7)
						{
							operator*=(1000000000);
							operator+=(num);
							num = k = 0;
							len += 9;
						}
						else if (str[i] > '0' || BitsCount || num)
						{
							num *= 10;
							++k;
						}
					}
				}
				if (len < numlen)
				{
					i = 10; num /= 10;
					while (++len<numlen)i *= 10;
					operator*=(i);
					operator+=(num);
				}
			}
			if (BitsCount)Sign = flag;
		}

		//转换
		std::string Int::ToString()const
		{
			if (Sign > 1 || Sign < -1)
			{
				if (Sign<-1 && Sign>-20)return MathError::description[-Sign];
				return "未知错误。";
			}
			Int tmp(*this);
			u64 strlen((u64)(0.301029995664*tmp.BitsCount + 3));//相当精确地计算出字符串长度
			std::string pstr;
			pstr.resize(strlen);
			u64 result, current(0);
			if (Sign < 0)
			{
				pstr[0] = '-';
				current = 1;
			}
			result = tmp.SelfDivide(1000000000);//每9位为一个单位
			while (tmp.BitsCount != 0)
			{
				for (int i = 0; i < 9; ++i) { pstr[current] = '0' + result % 10; result /= 10; ++current; }
				result = tmp.SelfDivide(1000000000);//每9位为一个单位
			}
			while (result)
			{
				pstr[current] = '0' + result % 10; result /= 10; ++current;
			}
			if (!current)
			{
				pstr[0] = '0';
				++current;
			}
			pstr[ current] = 0;
			int vlen = int(current--);
			u64 i = (Sign < 0);
			while (i < current)
			{
				Swap(pstr[i], pstr[current]);
				++i;
				--current;
			}
			pstr.resize(vlen);
			return pstr;
		}
		Int & Int::operator=(s64 num)
		{
			_Relen64(1);
			if (num < 0)
			{
				*pData = -num;
				Sign = -1;
				BitsCount = 64 - Bits::LeftZeroCount64(-num);
			}
			else
			{
				*pData = num;
				BitsCount = 64 - Bits::LeftZeroCount64(num);
				Sign = num != 0;
			}
			return *this;
		}
		Int & Int::operator=(u64 num)
		{
			_Relen64(1);
			*pData = num;
			BitsCount = 64 - Bits::LeftZeroCount64(num);
			Sign = num != 0;
			return *this;
		}
		//加法
		Int& Int::operator+=(const Int& vi)
		{//需要获取两者该实例写权限和vi的读权限
			if (IsError() || vi.Equals0())return *this;
			if (vi.IsError())return MakeError(vi.Sign);
			if (Equals0())return *this = vi;
			if (Sign != vi.Sign)
			{
				Int tmp(vi.pData, vi.len64, vi.maxSize, vi.BitsCount, Sign);
				operator-=(tmp);
				tmp.pData = NULL;
				return *this;
			}
			if (len64 >= (vi.BitsCount + 63) / 64)
			{
				u64 tlen = ((BitsCount > vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64;
				if (BitsPlus(pData, vi.pData, tlen, (vi.BitsCount + 63) / 64))
				{
					Relen64((size_t)(tlen + 1));
					pData[tlen] = 1;
					BitsCount = tlen * 64 + 1;
					return *this;
				}
				BitsCount = FindBitsCount((size_t)tlen);
			}
			return *this;
		}
		Int& Int::operator+=(u64 num)
		{
			if (IsError())return *this;
			if (Equals0()) { return *this = num; }
			size_t l64((size_t)((BitsCount + 0x3f) >> 6));
			if (Sign > 0)
			{
				if (num > (~*pData))
				{
					*pData += num;
					for (size_t i(1); i < l64 && !(++pData[i]); ++i);//进位操作
					if (!pData[l64 - 1] || l64 == 1)//长度扩充
					{
						Relen64((size_t)(l64 + 1));
						pData[l64] = 1;
						++l64;
					}
				}
				else *pData += num;
				BitsCount = (l64 << 6) - Bits::LeftZeroCount64(pData[l64 - 1]);
			}
			else
			{
				if (BitsCount <= 64 && num >= *pData)
				{
					*pData = num - *pData;
					if (!(BitsCount = 64 - Bits::LeftZeroCount64(*pData)))Sign = 0;
					else Sign = 1;
					goto END;
				}
				if (num > *pData)
					for (size_t i(1); i < l64 && !pData[i]--; ++i);
				*pData -= num;
				BitsCount = FindBitsCount(len64);
			}
		END:;
			return *this;
		}
		Int& Int::operator+=(s64 num)
		{//不需要抢权限
			if (num > 0)
				return operator+=((u64)num);
			operator-=((u64)-num);
			return *this;
		}
		Int & Int::operator+=(double num)
		{
			if (IsError())return*this;
			if (Equals0())return *this = num;
			if ((Sign > 0 && num < 0) || (Sign < 0 && num>0))return operator-=(-num);
			if (num <= MaxValues::Int64&& num >= MinValues::Int64)
				return operator+=((s64)num);
			union { u64 ull; double d; };
			if (num < 0)d = -num;
			else d = num;
			index_t exp = (ull >> 52) - 1023;
			ull = (ull & 0x000fffffffffffff) | 0x0010000000000000;
			if (exp >= (index_t)BitsCount)Relen64(1 + (exp >> 6));
			if (Bits::SelfPlusInt64(pData, len64, ull, exp - 52))
			{
				Relen64(len64 + 1);
				pData[len64 - 1] = 1;
				BitsCount = len64 * 64 - 63;
				return *this;
			}
			BitsCount = FindBitsCount(len64);
			return *this;
		}
		//++运算符
		Int& Int::operator++()
		{
			if (IsError())return *this;
			if (!BitsCount) { _Relen64(1); pData[0] = 1; BitsCount = Sign = 1; return *this; }
			size_t l64((size_t)((BitsCount - 1) >> 6));
			size_t i(1);
			if (Sign > 0)//增运算
			{
				if (!++pData[0])//进位
				{
					while (i <= l64 && !++pData[i])
						++i;
					if (i > l64)//长度扩展
					{
						Relen64(i + 1);
						pData[i] = 1;
						BitsCount = 1 + (i << 6ull);
					}
				}
			}
			else//减运算
			{
				if (!pData[0]--)//借位
				{
					while (i <= l64 && !pData[i]++)
						++i;
				}
			}

			if (i == l64)BitsCount = FindBitsCount(l64 + 1);
			else BitsCount = FindBitsCount(l64);
			if (!BitsCount)Sign = 0;
			return *this;
		}
		Int Int::operator++(int)
		{
			Int tmp(*this);
			u64 *p = tmp.pData;
			tmp.pData = NULL;
			++*this;
			return Int(p, tmp.len64, tmp.maxSize, tmp.BitsCount, tmp.Sign);
		}
		//+运算符
		Int Int::operator+(const Int& vi)const
		{
			if (IsError())return *this;
			if (vi.IsError())return vi;
			if (Equals0())return vi;
			if (vi.Equals0())return *this;
			u64 bc = BitsCount > vi.BitsCount ? BitsCount : vi.BitsCount;
			size_t l64 = (size_t)((bc + 127) >> 6);
			size_t len = (size_t)(((BitsCount < vi.BitsCount ? BitsCount : vi.BitsCount) + 63) >> 6);
			size_t maxs;
			u64 *p1, *p2, *p;
			if (Sign == vi.Sign)
			{
				p = Malloc<u64>(l64, maxs);
				if (BitsCount >= vi.BitsCount) { p1 = pData; p2 = vi.pData; }
				else { p2 = pData; p1 = vi.pData; }
				size_t i = 0;
				bool flag = false;
				while (i < len)
				{
					if (flag) {
						flag = p1[i] >= ~p2[i];
						p[i] = p1[i] + p2[i] + 1;
					}
					else
					{
						flag = p1[i] > ~p2[i];
						p[i] = p1[i] + p2[i];
					}
					++i;
				}
				while (i < l64 - 1)
				{
					if (flag)
						flag = (p[i] = p1[i] + 1) == 0;
					else p[i] = p1[i];
					++i;
				}
				p[l64 - 1] = flag;
				if (!flag)bc = (l64 << 6) - 64 - Bits::LeftZeroCount(p[l64 - 2]);
				else bc = (l64 << 6) - 63;
				return Int(p, l64, maxs, bc, Sign);
			}
			int cmp = AbsCompare(*this, vi);
			if (cmp == 0)return Int();
			if (cmp > 0) { p1 = pData; p2 = vi.pData; }
			else { p2 = pData; p1 = vi.pData; }
			l64--;
			p = Malloc<u64>(l64, maxs);
			size_t i = 0;
			bool flag = false;
			while (i < len)
			{
				if (flag) {
					flag = p1[i] <= p2[i];
					p[i] = p1[i] - p2[i] - 1;
				}
				else
				{
					flag = p1[i] < p2[i];
					p[i] = p1[i] - p2[i];
				}
				++i;
			}
			while (i < l64)
			{
				if (flag)
					flag = (p[i] = p1[i] - 1) == (u64)-1;
				else p[i] = p1[i];
				++i;
			}
			while (l64 != 0 && p[--l64] == 0);
			bc = l64 * 64ull + 64 - Bits::LeftZeroCount(p[l64]);
			return Int(p, l64 + 1, maxs, bc, cmp>0 ? Sign : vi.Sign);
		}
		double Int::operator+(double vi)const
		{
			return (double)*this + vi;
		}
		Int Int::operator+(u64 num) const
		{
			if (IsError())return *this;
			if (Equals0()) { return num; }
			size_t l64;
			size_t maxs;
			u64* p;
			u64 bc;
			if (Sign > 0)
			{
				l64 = (size_t)(((BitsCount + 0x3f) >> 6) + 1);
				p = Malloc<u64>(l64, maxs);
				*p = *pData + num;
				size_t i = 0;
				if (num > (~*pData))
					while (++i < l64 - 1 && !(p[i] = pData[i] + 1));//进位操作
				if (i == l64 - 1) { p[i] = 1; bc = (i << 6ull) + 1; }
				else {
					while (++i < l64 - 1) { p[i] = pData[i]; }
					p[i] = 0;
					bc = (i << 6ull) - Bits::LeftZeroCount64(p[i - 1]);
				}
				return Int(p, l64, maxs, bc, 1);
			}
			if (BitsCount <= 64 && num >= *pData)
				return num - *pData;
			l64 = (size_t)(((BitsCount + 0x3f) >> 6));
			p = Malloc<u64>(l64, maxs);
			*p = *pData - num;
			size_t i = 0;
			if (num > *pData)
				while (++i < l64 && (p[i] = pData[i] - 1) == (u64)-1);
			while (++i < l64) p[i] = pData[i];
			while (--l64 != 0 && p[l64] == 0);
			bc = l64 * 64 + 64 - Bits::LeftZeroCount(p[l64]);
			return Int(p, l64 + 1, maxs, bc, -1);
		}
		Int Int::operator+(s64 num) const
		{
			if (num > 0)return operator+((u64)num);
			return operator-((u64)-num);
		}
		//-=重载
		Int& Int::operator-=(const Int& vi)
		{
			if (IsError() || vi.Equals0())return *this;
			if (vi.IsError())return MakeError(vi.Sign);
			if (pData == vi.pData)return *this = Int();
			if (Sign != vi.Sign)
			{
				Int tmp(vi.pData, vi.len64, vi.maxSize, vi.BitsCount, Sign);
				operator+=(tmp);
				tmp.pData = NULL;
				return *this;
			}
			if (AbsCompare(*this, vi) > 0)
			{
				BitsMinus(pData, vi.pData, (BitsCount + 63) / 64, (vi.BitsCount + 63) / 64);
				BitsCount = FindBitsCount((size_t)((BitsCount + 63) / 64));
			}
			SwapWith(AbsMinus(*this, vi).SelfNegative());
			return *this;
		}
		Int & Int::operator-=(u64 num)
		{
			if (IsError())return *this;
			if (Equals0()) { return (*this = num).SelfNegative(); }
			size_t l64((size_t)((BitsCount + 0x3f) >> 6));
			if (Sign < 0)
			{
				if (num > (~*pData))
				{
					*pData += num;
					for (size_t i(1); i < l64 && !(++pData[i]); ++i);//进位操作
					if (!pData[l64 - 1])//长度扩充
					{
						Relen64((size_t)(l64 + 1));
						pData[l64] = 1;
						++l64;
					}
				}
				else *pData += num;
				BitsCount = (l64 << 6) - Bits::LeftZeroCount64(pData[l64 - 1]);
			}
			else
			{
				if (BitsCount <= 64 && num >= *pData)
				{
					*pData = num - *pData;
					if (!(BitsCount = 64 - Bits::LeftZeroCount64(*pData)))Sign = 0;
					else Sign = -1;
					goto END;
				}
				if (num > *pData)
					for (size_t i(1); i < l64 && !pData[i]--; ++i);
				*pData -= num;
				BitsCount = FindBitsCount(len64);
			}
		END:;
			return *this;
		}
		Int& Int::operator-=(s64 num)
		{//不需要抢权限
			if (num > 0)
				return operator-=((u64)num);
			operator+=((u64)-num);
			return *this;
		}
		Int & Int::operator-=(double num)
		{
			if (IsError())return*this;
			if (Equals0())return *this = -num;
			if ((Sign > 0 && num < 0) || (Sign < 0 && num>0))return operator+=(-num);
			if (num <= MaxValues::Int64&& num >= MinValues::Int64)
				return operator+=((s64)num);
			union { u64 ull; double d; };
			if (num < 0)d = -num;
			else d = num;
			index_t exp = (ull >> 52) - 1023;
			ull = (ull & 0x000fffffffffffff) | 0x0010000000000000;
			u64 arr[2];
			arr[0] = ull << ((exp - 52) & 63);
			arr[1] = ((exp - 52) & 63) == 0 ? 0 : ull >> (64 - ((exp - 52) & 63));

			if (exp >= (index_t)BitsCount || (exp == (index_t)BitsCount - 1 && Bits::GetWindow64Low64(pData, len64, BitsCount - 53) <= ull))
			{
				Relen64(1 + (exp >> 6));
				bool flag = false;
				size_t i = 0;
				while ((index_t)i < ((exp - 52) >> 6))
				{
					if (flag)
						pData[i] = ~pData[i];
					else if (pData[i] != 0) { flag = true; pData[i] = -(s64)pData[i]; }
					++i;
				}
				if (flag)
				{
					flag = arr[0] <= pData[i];
					pData[i] = arr[0] - pData[i] - 1;
				}
				else
				{
					flag = arr[0] < pData[i];
					pData[i] = arr[0] - pData[i];
				}
				if (arr[1] != 0)
				{
					pData[i + 1] = arr[1] - pData[i + 1] - flag;
				}
				BitsCount = FindBitsCount(len64);
				Sign = -Sign;
				return *this;
			}
			size_t i = (exp - 52) >> 6;
			bool flag = pData[i] < arr[0];
			pData[i++] -= arr[0];
			flag = flag ? pData[i] <= arr[1] : pData[i] < arr[1];
			pData[i] -= arr[1] + flag;
			while (flag&& ++i < len64)flag = (pData[i]--) == 0;
			BitsCount = FindBitsCount(len64);
			return *this;
		}
		//--运算符
		Int& Int::operator--()
		{
			if (IsError())return *this;
			if (Equals0()) { _Relen64(1); pData[0] = 1; BitsCount = 1; Sign = -1; return *this; }
			size_t l64((size_t)((BitsCount - 1) >> 6));
			if (Sign < 0)//增运算
			{
				if (!++pData[0])//进位
				{
					size_t i(1);
					while (i <= l64 && !++pData[i])
						++i;
					if (i > l64)//长度扩展
					{
						Relen64(i + 1);
						pData[i] = 1;
						BitsCount = 1 + (l64 << 6);
					}
					else if (i == l64)
						BitsCount = (l64 << 6) + 64 - Bits::LeftZeroCount64(pData[l64]);
				}
			}
			else//减运算
			{
				if (!pData[0]--)//借位
				{
					size_t i(1);
					while (i <= l64 && !pData[i]++)
						++i;
					if (i == l64)
						BitsCount = FindBitsCount(l64 + 1);
				}
				if (!BitsCount)
					Sign = 0;
			}
			return *this;
		}
		Int Int::operator--(int)
		{
			Int tmp(*this);
			u64 *p = tmp.pData;
			tmp.pData = NULL;
			--*this;
			return Int(p, tmp.len64, tmp.maxSize, tmp.BitsCount, tmp.Sign);
		}
		Int Int::operator-(const Int& vi)const
		{
			char t[sizeof(Int)];
			return operator+(*placementnew((Int*)t, vi.pData, vi.len64, vi.maxSize, vi.BitsCount, -vi.Sign));
		}
		Int Int::operator-(u64 num) const
		{
			if (IsError())return *this;
			if (Equals0()) { return Int(num).SelfNegative(); }
			size_t l64;
			size_t maxs;
			u64* p;
			u64 bc;
			if (Sign < 0)
			{
				l64 = (size_t)(((BitsCount + 0x3f) >> 6) + 1);
				maxs = Memory::GetResize(l64*sizeof(u64)) / sizeof(u64);
				p = Malloc<u64>(maxs);
				*p = *pData + num;
				size_t i = 0;
				if (num > (~*pData))
					while (++i < l64 - 1 && !(p[i] = pData[i] + 1));//进位操作
				if (i == l64 - 1) { p[i] = 1; bc = (i << 6ull) + 1; }
				else {
					while (i < l64 - 1) { ++i; p[i] = pData[i]; }
					p[i] = 0;
					bc = (i << 6ull) - Bits::LeftZeroCount64(pData[i - 1]);
				}
				return Int(p, l64, maxs, bc, -1);
			}
			if (BitsCount <= 64 && num >= *pData)
				return (s64)(*pData - num);
			l64 = (size_t)(((BitsCount + 0x3f) >> 6));
			maxs = Memory::GetResize(l64*sizeof(u64)) / sizeof(u64);
			p = Malloc<u64>(maxs);
			*p = *pData - num;
			size_t i = 0;
			if (num > *pData)
				while (++i < l64 && (p[i] = pData[i] - 1) == (u64)-1);
			while (++i < l64) p[i] = pData[i];
			while (--l64 != 0 && p[l64] == 0);
			bc = l64 * 64 + 64 - Bits::LeftZeroCount(p[l64]);
			return Int(p, l64 + 1, maxs, bc, 1);
		}
		Int Int::operator-(s64 num) const
		{
			if (num > 0)return operator-((u64)num);
			return operator+((u64)-num);
		}
		double Int::operator-(double num) const
		{
			return (double)*this - num;
		}

		Int & Int::operator*=(const Int & vi)
		{
			if (IsError())return *this;
			if (vi.IsError())return *this = vi;
			if (vi.Equals0())return *this = 0;
			if (Equals0())return *this;
			size_t len = (size_t)((BitsCount + vi.BitsCount + 63) >> 6);
			if ((64 - Bits::LeftZeroCount64(len)) * len * 12 < (u64)(len64*vi.len64)){
				if (maxSize >= len){
					size_t l64 = len64*(sizeof(u64) / sizeof(u16));
					_Relen64(len);
					Bits::MultiplyFFTLow((u16*)pData, len64*(sizeof(u64) / sizeof(u16)), (u16*)pData, l64, (u16*)vi.pData, vi.len64*(sizeof(u64) / sizeof(u16)));
				}
				else
				{
					Int tmp;
					tmp.maxSize = Memory::GetResize(len*sizeof(u64)) / sizeof(u64);
					tmp.pData = Malloc<u64>(tmp.maxSize);
					tmp.len64 = len;
					tmp.Sign = Sign;
					Bits::MultiplyFFTLow((u16*)tmp.pData, len*(sizeof(u64) / sizeof(u16)), (u16*)pData, len64*(sizeof(u64) / sizeof(u16)), (u16*)vi.pData, vi.len64*(sizeof(u64) / sizeof(u16)));
					SwapWith(tmp);
				}
			}
			else
			{
				u8* buf=new u8[len*sizeof(u64)];
				u64* p = (u64*)(u8*)buf;
				Bits::MultiplyLow64(p, len, pData, len64, vi.pData, vi.len64);
				_Relen64(len);
				memcpy(pData, p, len*sizeof(u64));
				delete[] buf;
			}
			BitsCount = FindBitsCount(len64);
			Sign = Sign == vi.Sign ? 1 : -1;
			return *this;
		}
		Int& Int::operator*=(u64 i)
		{
			if (IsError())return *this;
			if (i == 0)return *this = 0;
			size_t len = (size_t)((127 - Bits::LeftZeroCount(i) + BitsCount) >> 6);
			if (len > len64)Relen64(len);
			Bits::SelfMultiplyInt64Low64(pData, len64, i);
			BitsCount = FindBitsCount(len64);
			return *this;
		}
		Int& Int::operator*=(u32 i)
		{
			if (IsError())return *this;
			if (i == 0)return *this = 0;
			size_t len = (size_t)((95 - Bits::LeftZeroCount(i) + BitsCount) >> 6);
			if (len > len64)Relen64(len);
			Bits::SelfMultiplyInt32Low64(pData, len64, i);
			BitsCount = FindBitsCount(len64);
			return *this;
		}
		Int & Int::operator*=(s64 i)
		{
			if (i < 0)
			{
				Sign = -Sign;
				return operator*=((u64)-i);
			}
			return operator*=((u64)i);
		}
		Int& Int::operator*=(s32 i)
		{
			if (i < 0)
			{
				Sign = -Sign;
				return operator*=((u32)-i);
			}
			return operator*=((u32)i);
		}
		Int& Int::operator*=(double num)
		{
			union
			{
				u64 ull;
				double d;
			};
			d = num;
			*this *= (ull | (((u64)(0x1) << 52))) & 0x001fffffffffffff;
			if (num < 0)SelfNegative();
			return *this <<= ((ull&MaxValues::LongLong) >> 52) - (1023 + 52);
		}
		Int Int::operator*(const Int& vi)const
		{
			if (IsError())return *this;
			if (vi.IsError())return vi;
			if (vi.Equals0())return 0;
			if (Equals0())return *this;
			Int tmp;
			tmp.len64 = (size_t)((BitsCount + vi.BitsCount + 63) >> 6);
			tmp.pData = Malloc<u64>(tmp.len64, tmp.maxSize);
			if ((64 - Bits::LeftZeroCount64(tmp.len64)) *tmp.len64* 12 < (u64)(len64*vi.len64)){
				Bits::MultiplyFFTLow((u16*)tmp.pData, tmp.len64*(sizeof(u64) / sizeof(u16)), (u16*)pData, len64*(sizeof(u64) / sizeof(u16)), (u16*)vi.pData, vi.len64*(sizeof(u64) / sizeof(u16)));
			}
			else
			{
				Bits::MultiplyLow64(tmp.pData, tmp.len64, pData, len64, vi.pData, vi.len64);
			}
			tmp.BitsCount = tmp.FindBitsCount(tmp.len64);
			u64*ptd = tmp.pData;
			tmp.Sign = Sign == vi.Sign ? 1 : -1;
			tmp.pData = NULL;
			return Int(ptd, tmp.len64, tmp.maxSize, tmp.BitsCount, tmp.Sign);
		}
		Int Int::operator*(u64 num) const
		{
			if (IsError() || Equals0())return *this;
			if (num == 0)return 0;
			size_t len = (size_t)((127 - Bits::LeftZeroCount(num) + BitsCount) >> 6);
			size_t maxs;
			u64*p = Malloc<u64>(len, maxs);
			Bits::MultiplyInt64Low64(p, len, pData, len64, num);
			while (!p[--len]);
			u64 bc = len * 64ull + 64 - Bits::LeftZeroCount(p[len]);
			return Int(p, len + 1, maxs, bc, Sign);
		}
		Int Int::operator*(s64 num) const
		{
			if (num < 0)return (*this*((u64)-num)).SelfNegative();
			return *this*((u64)num);
		}
		Int Int::operator*(u32 num) const
		{
			if (IsError() || Equals0())return *this;
			if (num == 0)return 0;
			size_t len = (size_t)((95 - Bits::LeftZeroCount(num) + BitsCount) >> 6);
			size_t maxs = Memory::GetResize(len*sizeof(u64)) / sizeof(u64);
			u64*p = Malloc<u64>(maxs);
			Bits::MultiplyInt32Low64(p, len, pData, len64, num);
			while (!p[--len]);
			u64 bc = len * 64ull + 64 - Bits::LeftZeroCount(p[len]);
			return Int(p, len + 1, maxs, bc, Sign);
		}
		Int Int::operator*(s32 num) const
		{
			if (num < 0)return (*this*((u32)-num)).SelfNegative();
			return *this*((u32)num);
		}
		double Int::operator*(double num) const
		{
			return ((double)*this)*num;
		}
		//除法
		Int& Int::operator/=(const Int& vi)
		{
			if (IsError())return *this;
			if (vi.IsError())return *this = vi;
			if (vi.Equals0())return MakeError(MathError::Divide0);
			if (Equals0())return *this;
			if (AbsCompare(*this, vi) < 0)return *this = 0;
			u64 bc = BitsCount - vi.BitsCount + 1;
			size_t len = (size_t)((bc + 63) >> 6);
			u8* arr = new u8[len*sizeof(u64)];
			u64* p = (u64*)(u8*)arr;
			Bits::DivideLow64(p, len, pData, BitsCount, vi.pData, vi.BitsCount);
			for (size_t i = 0; i < len; ++i)pData[i] = p[i];
			BitsCount = FindBitsCount(len);
			Sign = Sign == vi.Sign ? 1 : -1;
			delete [] arr;
			return *this;
		}
		Int& Int::operator/=(u64 num)
		{
			if (IsError())return *this;
			if (num == 0)return MakeError(MathError::Divide0);
			if (Equals0())return*this;
			Bits::SelfDivideInt64Low64(pData, len64, num);
			u64 lbc = BitsCount;
			if ((BitsCount = FindBitsCount(len64)) == 0)Sign = 0;
			if (lbc > BitsCount + 64 - Bits::LeftZeroCount64(num))
			{
				Sleep(1);
			}
			return *this;
		}
		Int& Int::operator/=(u32 num)
		{
			if (IsError())return *this;
			if (num == 0)return MakeError(MathError::Divide0);
			if (Equals0())return*this;
			Bits::SelfDivideInt32Low64(pData, len64, num);
			BitsCount = FindBitsCount(len64);
			return *this;
		}
		Int& Int::operator/=(double i)
		{
			if (IsError())return *this;
			if (i == 0)return MakeError(MathError::Divide0);
			return operator*=(1 / i);
		}
		Int Int::operator/(const Int& vi)const
		{
			if (IsError())return *this;
			if (vi.IsError())return vi;
			if (vi.Equals0())return Int().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			if (AbsCompare(*this, vi) < 0)return 0;
			Int tmp;
			tmp.len64 = (size_t)((BitsCount - vi.BitsCount + 64) >> 6);
			tmp.maxSize = Memory::GetResize(tmp.len64*sizeof(u64)) / sizeof(u64);
			tmp.pData = Malloc<u64>(tmp.maxSize);
			Bits::DivideLow64(tmp.pData, tmp.len64, pData, BitsCount, vi.pData, vi.BitsCount);
			tmp.BitsCount = tmp.FindBitsCount(tmp.len64);
			u64*ptd = tmp.pData;
			tmp.Sign = Sign == vi.Sign ? 1 : -1;
			tmp.pData = NULL;
			return Int(ptd, tmp.len64, tmp.maxSize, tmp.BitsCount, tmp.Sign);
		}
		Int Int::operator/(u64 num) const
		{
			if (IsError())return *this;
			if (num == 0)return Int().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			size_t maxs = Memory::GetResize(len64*sizeof(u64)) / sizeof(u64);
			u64*p = Malloc<u64>(maxs);
			Bits::DivideInt64Low64(p, len64, pData, len64, num);
			size_t len = len64;
			if (len != 0)while (!p[--len]);
			u64 bc = len * 64ull + 64 - Bits::LeftZeroCount(p[len]);
			return Int(p, len + 1, maxs, bc, Sign);
		}
		Int Int::operator/(s64 num) const
		{
			if (num < 0)return (*this / (u64)-num).SelfNegative();
			return (*this / (u64)num);
		}
		Int Int::operator/(u32 num) const
		{
			if (IsError())return *this;
			if (num == 0)return Int().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			size_t maxs = Memory::GetResize(len64*sizeof(u64)) / sizeof(u64);
			u64*p = Malloc<u64>(maxs);
			Bits::DivideInt32Low32((u32*)p, len64 * 2, (u32*)pData, len64 * 2, num);
			size_t len = len64;
			if (len != 0)while (!p[--len]);
			u64 bc = len * 64ull + 64 - Bits::LeftZeroCount(p[len]);
			return Int(p, len + 1, maxs, bc, Sign);
		}
		Int Int::operator/(s32 num) const
		{
			if (num < 0)return (*this / (u32)-num).SelfNegative();
			return (*this / (u32)num);
		}

		ui32 Int::SelfDivide(ui32 num)//返回余数
		{
			if (!num || !BitsCount)return 0;
			size_t l64((size_t)((BitsCount - 1) >> 6));
			union
			{
				u64 ull;
				ui32 ui[2];
			} tmp, t;
			tmp.ull = t.ull = 0;
			for (index_t i(l64); i >= 0; --i)
			{
				tmp.ull += pData[i] >> 32;
				t.ull = (tmp.ull % num) << 32;
				tmp.ui[1] = (ui32)(tmp.ull / num);
				t.ull += pData[i] & 0xffffffff;
				tmp.ui[0] = (ui32)(t.ull / num);
				pData[i] = tmp.ull;
				tmp.ull = (t.ull % num) << 32;
			}
			if (!pData[l64] && l64)--l64;
			BitsCount = (l64 << 6) + 64 - Bits::LeftZeroCount64(pData[l64]);
			if (!BitsCount)Sign = 0;
			return tmp.ui[1];
		}

		Int& Int::operator%=(const Int& vi)
		{
			if (IsError())return*this;
			if (vi.IsError())return *this = vi;
			if (vi.Equals0())return MakeError(MathError::ModDivide0);
			if (this == &vi)return *this = 0;
			if (AbsCompare(*this, vi) < 0)return *this;
			Bits::DivideLow64(NULL, 0, pData, BitsCount, vi.pData, vi.BitsCount, pData, (size_t)((vi.BitsCount + 63) >> 6));
			BitsCount = FindBitsCount((size_t)((vi.BitsCount + 63) >> 6));
			if (BitsCount == 0)Sign = 0;
			return *this;
		}
		Int& Int::operator%=(u64 num)
		{
			if (IsError())return *this;
			if (num == 0)return MakeError(MathError::Divide0);
			if (Equals0())return *this;
			*pData = *this% num;
			for (u64 i = (BitsCount - 1) / 64; i > 0; --i)pData[i] = 0;
			BitsCount = 64 - Bits::LeftZeroCount64(*pData);
			Sign = BitsCount > 0;
			return *this;
		}
		Int& Int::operator%=(u32 num)
		{
			if (IsError())return *this;
			if (num == 0)return MakeError(MathError::Divide0);
			if (Equals0())return *this;
			*pData = *this% num;
			for (u64 i = (BitsCount - 1) / 64; i > 0; --i)pData[i] = 0;
			BitsCount = 64 - Bits::LeftZeroCount64(*pData);
			Sign = BitsCount > 0;
			return *this;
		}

		Int Int::operator%(Int const & vi)const
		{
			if (IsError())return*this;
			if (vi.IsError())return vi;
			if (vi.Equals0())return Int().MakeError(MathError::ModDivide0);
			if (this == &vi)return 0;
			if (AbsCompare(*this, vi) < 0)return *this;
			size_t len = (size_t)((vi.BitsCount + 63) >> 6);
			size_t maxs = Memory::GetResize(len*sizeof(u64)) / sizeof(u64);
			u64* p = Malloc<u64>(maxs);
			Bits::DivideLow64(NULL, 0, pData, BitsCount, vi.pData, vi.BitsCount, p, len);
			while (len != 0 && !p[--len]);
			if (len == 0 && *p == 0)return Int();
			return Int(p, len + 1, maxs, len * 64ull + 64 - Bits::LeftZeroCount(p[len]), Sign);
		}
		u64 Int::operator%(u64 num)const
		{
			if (IsError() || Equals0())return 0;
			return Bits::DivideInt64Low64(NULL, 0, pData, (size_t)((BitsCount + 63) >> 6), num);
		}
		u32  Int::operator%(u32 num)const
		{
			if (IsError() || Equals0())return 0;
			size_t i = (size_t)((BitsCount - 1) / 32);
			u64 tmp = ((u32*)pData)[i] >= num ? ((u32*)pData)[i] % num : ((u32*)pData)[i];
			while (i-- != 0)
			{
				tmp = (tmp << 32) | ((u32*)pData)[i];
				tmp %= num;
			}
			return (u32)(tmp);
		}
		//位移
		Int Int::operator<<(s64 mov)//兼容负位移
		{
			if (IsError() || mov == 0)return *this;
			if (Equals0() || mov + BitsCount <= 0)return Int();
			if (mov < 0)return operator>>(-mov);
			s64 bcount = BitsCount + mov;
			if (bcount > (s64)MaxBitsCount)return Int().MakeError(MathError::MoveLeftOverflow);
			Int tmp;
			tmp.BitsCount = bcount;
			tmp._Relen64((size_t)((bcount + 63) / 64));
			tmp.pData[tmp.len64 - 1] = 0;
			for (index_t i = 0; i < (mov + 63) / 64; ++i)tmp.pData[i] = 0;
			Bits::Copy(tmp.pData, (index_t)mov, pData, 0, (size_t)BitsCount);
			u64 *ptd = tmp.pData;
			tmp.pData = NULL;
			return Int(ptd, tmp.len64, tmp.maxSize, tmp.BitsCount, Sign);
		}
		Int Int::operator>>(s64 mov)
		{
			if (IsError() || mov == 0)return *this;
			if (Equals0() || BitsCount - mov <= 0)return Int();
			if (mov < 0)return operator<<(-mov);
			s64 bcount = BitsCount - mov;
			Int tmp;
			tmp.BitsCount = bcount;
			tmp._Relen64((size_t)((bcount + 63) / 64));
			tmp.pData[tmp.len64 - 1] = 0;
			Bits::Copy(tmp.pData, 0, pData, (index_t)(BitsCount - tmp.BitsCount), (size_t)tmp.BitsCount);
			u64 *ptd = tmp.pData;
			tmp.pData = NULL;
			return Int(ptd, tmp.len64, tmp.maxSize, tmp.BitsCount, Sign);
		}
		Int& Int::operator<<=(s64 mov)
		{
			if (IsError() || mov == 0)return *this;
			if (Equals0() || mov + BitsCount <= 0)return *this = Int();
			if (mov < 0)return operator>>=(-mov);
			s64 bcount = BitsCount + mov;
			if (bcount > (s64)MaxBitsCount)return MakeError(MathError::MoveLeftOverflow);
			if ((s64)maxSize < (bcount + 63) / 64) { SwapWith(Conversion::Convert<Int>::reference(*this << mov)); return*this; }
			_Relen64((size_t)((bcount + 63) / 64));
			BitsCount = bcount;
			Bits::Move(pData, 0, bcount, mov);
			return *this;
		}
		Int& Int::operator>>=(s64 mov)
		{
			if (IsError() || mov == 0)return *this;
			if (Equals0() || BitsCount - mov <= 0)return *this = Int();
			if (mov<0)return operator<<=(-mov);
			s64 bcount = BitsCount - mov;
			Bits::Move(pData, 0, BitsCount, -mov);
			BitsCount = bcount;
			len64 = (size_t)((bcount + 63) >> 6);
			return *this;
		}

		Int& Int::operator|=(const Int& vi)
		{
			if (IsError() || vi.Equals0())return *this;
			if (vi.IsError())return MakeError(vi.Sign);
			size_t lenmax = (size_t)(((BitsCount>vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64);
			size_t lenmin = (size_t)(((BitsCount < vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64);
			Relen64((size_t)lenmax);
			for (size_t i = 0; i < lenmin; ++i)pData[i] |= vi.pData[i];
			if (BitsCount < vi.BitsCount)BitsCount = vi.BitsCount;
			Sign = Sign < 0 || vi.Sign<0 ? -1 : 1;
			return *this;
		}
		Int operator|(Int const& v1, Int const& v2)
		{
			if (v1.IsError() || v2.Equals0())return v1;
			if (v2.IsError() || v1.Equals0())return v2;
			size_t lenmax = (size_t)(((v1.BitsCount>v2.BitsCount ? v1.BitsCount : v2.BitsCount) + 63) / 64);
			size_t lenmin = (size_t)(((v1.BitsCount < v2.BitsCount ? v1.BitsCount : v2.BitsCount) + 63) / 64);
			size_t maxs;
			u64 *p = Int::Malloc<u64>(lenmax, maxs);
			for (u64 i = 0; i < lenmin; ++i)p[i] = v1.pData[i] | v2.pData[i];
			if (v1.BitsCount > v2.BitsCount)for (u64 i = lenmin; i < lenmax; ++i)p[i] = v1.pData[i];
			else for (u64 i = lenmin; i<lenmax; ++i)p[i] = v2.pData[i];
			return Int(p, lenmax, maxs, v1.BitsCount>v2.BitsCount ? v1.BitsCount : v2.BitsCount, v1.Sign < 0 || v2.Sign < 0 ? -1 : 1);
		}
		Int& Int::operator&=(const Int& vi)
		{
			if (IsError() || Equals0())return *this;
			if (vi.IsError())return MakeError(vi.Sign);
			size_t lenmax = (size_t)(((BitsCount > vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64);
			size_t lenmin = (size_t)(((BitsCount < vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64);
			for (size_t i = 0; i < lenmin; ++i)pData[i] &= vi.pData[i];
			if (BitsCount > vi.BitsCount)
			{
				for (size_t i = lenmin; i < lenmax; ++i)pData[i] = 0;
				BitsCount = vi.BitsCount;
			}
			Sign = BitsCount == 0 ? 0 : Sign < 0 && vi.Sign < 0 ? -1 : 1;
			return *this;
		}
		Int operator&(const Int& v1, const Int& v2)
		{
			if (v1.IsError() || v2.Equals0())return v1;
			if (v2.IsError() || v1.Equals0())return v2;
			size_t lenmin = (size_t)(((v1.BitsCount < v2.BitsCount ? v1.BitsCount : v2.BitsCount) + 63) / 64);
			size_t len64 = lenmin;
			while (len64 != 0 && (v1.pData[len64 - 1] & v2.pData[len64 - 1]) == 0)--len64;
			if (len64 == 0)return Int();
			size_t mins;
			u64 *p = Int::Malloc<u64>(len64, mins);
			for (size_t i = 0; i < lenmin; ++i)p[i] = v1.pData[i] & v2.pData[i];
			return Int(p, len64, mins, len64 * 64 - Bits::LeftZeroCount64(p[len64 - 1]), v1.Sign < 0 && v2.Sign < 0 ? -1 : 1);
		}
		Int& Int::operator^=(const Int& vi)
		{
			if (IsError() || vi.Equals0())return *this;
			if (vi.IsError())return MakeError(vi.Sign);
			size_t lenmax = (size_t)(((BitsCount > vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64);
			size_t lenmin = (size_t)(((BitsCount < vi.BitsCount ? BitsCount : vi.BitsCount) + 63) / 64);
			Relen64(lenmax);
			for (size_t i = 0; i<lenmin; ++i)pData[i] |= vi.pData[i];
			BitsCount = FindBitsCount(lenmax);
			Sign = BitsCount == 0 ? 0 : Sign != vi.Sign ? -1 : 1;
			return *this;
		}
		Int operator^(const Int& v1, const Int& v2)
		{
			if (v1.IsError() || v2.Equals0())return v1;
			if (v2.IsError() || v1.Equals0())return v2;
			size_t lenmax = (size_t)(((v1.BitsCount>v2.BitsCount ? v1.BitsCount : v2.BitsCount) + 63) / 64);
			size_t lenmin = (size_t)(((v1.BitsCount < v2.BitsCount ? v1.BitsCount : v2.BitsCount) + 63) / 64);
			size_t len64 = lenmax;
			if (lenmax == lenmin)while (len64 != 0 && v1.pData[len64 - 1] == v2.pData[len64 - 1])--len64;
			if (len64 == 0)return Int();
			size_t maxs;
			u64 *p = Int::Malloc<u64>(len64, maxs);
			for (size_t i = 0; i < len64; ++i)p[i] = v1.pData[i] ^ v2.pData[i];
			if (v1.BitsCount > v2.BitsCount)for (size_t i = lenmin; i < lenmax; ++i)p[i] = v1.pData[i];
			else for (u64 i = lenmin; i < lenmax; ++i)p[i] = v2.pData[i];
			return Int(p, len64, maxs, len64 * 64 - Bits::LeftZeroCount64(p[len64 - 1]), v1.Sign == v2.Sign ? 1 : -1);
		}

		bool Int::operator>(double  num)const
		{
			union { u64 ull; double d; };
			index_t exp;
			if (num > 0)
			{
				if (Sign <= 0)return false;
				d = num;
				exp = (ull >> 52) - 1022;
				if ((index_t)BitsCount > exp)return true;
				if ((index_t)BitsCount < exp)return false;
				return Bits::GetWindow64Low64(pData, len64, (s64)BitsCount - 53)>((ull & 0x000fffffffffffff) | 0x0010000000000000);
			}
			if (num == 0)return Sign > 0;
			if (Sign >= 0)return true;
			d = -num;
			exp = (ull >> 52) - 1022;
			if ((index_t)BitsCount > exp)return true;
			if ((index_t)BitsCount < exp)return false;
			return Bits::GetWindow64Low64(pData, len64, (s64)BitsCount - 53) < ((ull & 0x000fffffffffffff) | 0x0010000000000000);
		}
		bool Int::operator<(double  num)const
		{
			union { u64 ull; double d; };
			int exp;
			if (num > 0)
			{
				if (Sign <= 0)return true;
				d = num;
				exp = (ull >> 52) - 1022;
				if ((index_t)BitsCount > exp)return false;
				if ((index_t)BitsCount < exp)return true;
				return Bits::GetWindow64Low64(pData, len64, (s64)BitsCount - 53) < ((ull & 0x000fffffffffffff) | 0x0010000000000000);
			}
			if (num == 0)return Sign < 0;
			if (Sign >= 0)return false;
			d = -num;
			exp = (ull >> 52) - 1022;
			if ((index_t)BitsCount > exp)return false;
			if ((index_t)BitsCount < exp)return true;
			return Bits::GetWindow64Low64(pData, len64, (s64)BitsCount - 53)>((ull & 0x000fffffffffffff) | 0x0010000000000000);
		}


		Int::operator double()const
		{
			if (IsError() || Equals0())return 0;
			if (BitsCount > 1024)return Sign>0 ? MaxValues::Double : MinValues::Double;
			union {
				u64 ull;
				double dn;
			};
			if (BitsCount >= 64)
			{
				ull = (GetBits64(pData, BitsCount - 64)&MaxValues::Int64) >> 11;
			}
			else
			{
				ull = (pData[0] << (64 - BitsCount)) >> 11;
			}
			ull = (ull & 0x000fffffffffffff) | ((BitsCount + 1022) << 52);
			if (Sign<0)return -dn;
			return dn;
		}
#ifdef LK_MATH_FLOAT256_
		Float256 Int::ToFloat256()const
		{
			if (IsError() || Equals0())return 0;
			if ((s64)BitsCount>Float256::ExpMaxValue)return Sign > 0 ? Float256::MaxValue() : Float256::MinValue();
			Float256 f(0);
			if (BitsCount >= 256)
			{
				for (index_t i = 0; i < 4; ++i) f.Data.ull[i] = GetBits64(pData, BitsCount - 256 + i * 64);
			}
			else
			{
				for (index_t i = (index_t)((BitsCount - 1) / 64); i >= 0; --i)f.Data.ull[i] = pData[i];
				f.Data <<= 256 - BitsCount;
			}
			f.Exp = BitsCount - 1;
			f.Sign = Sign;
			return f;
		}
#endif
	}
}

namespace std
{
	::LK::ptr<LK::Math::Int> pow(::LK::ptr<LK::Math::Int> const& f1, ::LK::ptr<LK::Math::Int> const& f2)
	{
		if (f1.IsError())return f1;
		if (f2.IsError())return f2;
		if (f1.Equals0())
		{
			if (f2.Equals0())
			{
				::LK::ptr<LK::Math::Int> i = LK::Conversion::Convert<::LK::ptr<LK::Math::Int>>::reference(f1->Clone());
				i->MakeError(LK::Math::MathError::Pow00);
				return i;
			}
			if (f2.GetSign() < 0)
			{
				::LK::ptr<LK::Math::Int> i = LK::Conversion::Convert<::LK::ptr<LK::Math::Int>>::reference(f1->Clone());
				i->MakeError(LK::Math::MathError::PowLessThan0);
				return i;
			}
			return 0;
		}
		if (f2.Equals0())return 1;
		if (f1.GetBitsCount() == 1)
		{
			if (f1.GetSign() == 1)return f1;
			return ((LK::u64)f2) & 1 ? f1 : -f1;
		}
		if (f2.GetSign() < 0)return 0;
		if (LK::Math::Int::MaxBitsCount / (f2.GetBitsCount() + 1) < f1.GetBitsCount())
		{
			::LK::ptr<LK::Math::Int> i = LK::Conversion::Convert<::LK::ptr<LK::Math::Int>>::reference(f1->Clone());
			i->MakeError(LK::Math::MathError::PowExpOverflow);
			return i;
		}
		LK::ptr<LK::Math::Int> result = 1;
		LK::ptr<LK::Math::Int> tmp = f1;
		LK::Bits::BitPtr bp(f2.get ()->pData);
		LK::u64 i = 0;
		LK::u64 bc = f2.GetBitsCount();
		for (;; )
		{
			if (bp.Get())result *= tmp;
			if (i >= bc)break;
			++i;
			++bp;
			tmp *= tmp;
		}
		return result;
	}
}
#endif
