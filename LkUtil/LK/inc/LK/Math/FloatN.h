#pragma once
#define LK_MATH_FLOATN_
#define HAS_SIGNED_TYPES_EXCEPT64 std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value
#define HAS_UNSIGNED_TYPES_EXCEPT64 std::is_same<T, u32>::value|| std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value
#define HAS_ALL_TYPES std::is_same<T, u32>::value || std::is_same<T, u64>::value || std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value || std::is_same<T, s64>::value || std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, double>::value || std::is_same<T, float>::value || std::is_same<T, char>::value
/**@author W意波编写*/
#include <math.h>
namespace LK
{
	namespace Math
	{
		/**
		@brief 表示一个(N*64)位浮点数*/
		template<size_t N>struct FloatNData
		{
			//数据
			unsigned long long arr[N];
			//默认构造，数据状态未知
			FloatNData() {}
#ifdef LK_MATH_FLOAT256_
			//从FloatData256构造
			FloatNData(FloatData256 const& d)
			{
				if (N > 4)for (size_t i = 0; i < N - 4; ++i)arr[i] = 0;
				for (size_t i = 0; N != i&&i != 4; ++i)arr[(N - 1) - i] = d.ull[3 - i];
			}
#endif
			/**
			@brief 全部数据清零*/
			void Clear() { for (size_t i = 0; i < N; ++i)arr[i] = 0; }
			/**
			@brief清零
			@param 参数high表示设置的最高元素值
			@return 当参数high非0时，返回自动进行左移以保证最高位为1 的移动位数（<0）*/
			long long Clear(unsigned long long high) { long long mov = high == 0 ? 0 : Bits::LeftZeroCount64(high); arr[N - 1] = high << mov; for (size_t i = 0; i < N - 1; ++i)arr[i] = 0; return -mov; }
			/**
			@brief 将偏移位数为mov及之后的比特全部置0*/
			void Remove(long long mov)
			{
				Bits::Set(arr, 0, (long long)N * 64ll - mov);
			}
			/**
			@brief 获取偏移最高位mov bit的64位值*/
			inline unsigned long long Get64(long long mov)const
			{
				return Bits::GetWindow64High64(arr, N, mov);
			}
			/**
			@brief 自身左移操作(往高位移动mov位)*/
			unsigned long long operator<<=(long long mov)
			{
				return Bits::Move64(arr, N, mov);
			}
			/**
			@brief 自身右移操作(往高位移动mov位)*/
			unsigned long long operator>>=(long long mov)
			{
				return Bits::Move64(arr, N, -mov);
			}
			/**
			@brief 自增运算，返回进位*/
			bool operator++() { size_t i = 0; while (i != N)if (++arr[i++] != 0)return false; return true; }
			/**
			@brief 自减运算，返回借位*/
			bool operator--() { size_t i = 0; while (i != N)if ((arr[i++]--) != 0)return false; return true; }
			/**
			@brief 返回前导0的数量*/
			long long BitsZeroCount()
			{
				for (size_t i = N; i != 0;) { --i; if (arr[i] != 0)return ((N - 1) * 64 - i * 64) + Bits::LeftZeroCount64(arr[i]); }return N * 64;
			}
			/**
			@brief 自身加上一个有效位数，mov为有效数相对偏移量，返回进位，
			@attention mov必须小于或等于0
			@note 相加时arr末端自动进位*/
			bool SelfPlus(FloatNData const &data, long long mov)
			{
				return Bits::SelfPlusRound64(arr, N, data.arr, N, mov);
			}
			/**
			@brief自身减去一个有效位数，mov为尾数相对偏移量，自动进行偏移使最高位为1，并返回偏移量（小于等于0）
			@attention mov必须小于或等于0，当mov为0时被减数必须不小于减数
			@note 相减时，arr末端自动借位*/
			long long SelfMinus(FloatNData const&data, long long mov)
			{
				Bits::SelfMinusRound64(arr, N, data.arr, N, mov);
				if ((long long)arr[N - 1] < 0)return 0;
				long long bitmov = BitsZeroCount();
				if (bitmov == N*sizeof(unsigned long long) * 8)return 0;
				Bits::Move64(arr, N, bitmov);
				return -bitmov;
			}

			bool operator-=(FloatNData const&data)
			{
				bool flag = false;
				for (size_t i = 0; i < N; ++i)
				{
					if (flag)
					{
						flag = arr[i] <= data.arr[i];
						arr[i] -= data.arr[i] + 1;
					}
					else
					{
						flag = arr[i] < data.arr[i];
						arr[i] -= data.arr[i];
					}
				}
				return flag;
			}

			/**
			@brief 如果有数据则返回true，否则返回false*/
			operator bool()const { for (size_t i = 0; i < N; ++i)if (arr[i] != 0)return true; return false; }
			/**
			@brief 如果有数据则返回false，否则返回true*/
			bool operator !()const { for (size_t i = 0; i < N; ++i)if (arr[i] != 0)return false; return true; }
			/**
			@brief 比较大小，该数较大时返回1，相等返回0，较小返回-1*/
			index_t CompareWith(const FloatNData&fd)const { size_t i = N; while (i != 0) { --i; if (arr[i] != fd.arr[i])return arr[i] > fd.arr[i] ? 1 : -1; }return 0; }
			/**
			@brief 前者较大时返回true*/
			bool operator>(const FloatNData&fd)const { return CompareWith(fd) > 0; }
			/**
			@brief 前者不小于后者返回true*/
			bool operator>=(const FloatNData&fd)const { return CompareWith(fd) >= 0; }
			/**
			@brief 前者较小时返回true*/
			bool operator<(const FloatNData&fd)const { return CompareWith(fd) < 0; }
			/**
			@brief 前者不大于后者时返回true*/
			bool operator<=(const FloatNData&fd)const { return CompareWith(fd) <= 0; }
			/**
			@brief 相等时返回true*/
			bool operator==(const FloatNData&fd)const { return CompareWith(fd) == 0; }
			/**
			@brief 不相等时返回true*/
			bool operator!=(const FloatNData&fd)const { return CompareWith(fd) != 0; }
		};
		/**
		@brief 表示一个N*64位浮点数*/
		template<size_t N>struct FloatN
		{
			typedef FloatNData<N> FloatnData;
			/**
			@brief 将num（64位）乘上dat的结果保存到destdat中，并返回被舍弃的尾数*/
			static unsigned long long BitsMultiply(FloatnData const &dat, FloatnData &desdat, unsigned long long num)
			{
				return Bits::MultiplyInt64Low64(desdat.arr, N, dat.arr, N, num);
			}
			/**
			@brief 符号和错误标记*/
			index_t Sign;
			/**
			@brief 指数*/
			long long Exp;
			/**
			@brief 有效数字*/
			FloatnData Data;

			/**
			@brief 设置错误后是否要抛出异常,默认为false*/
			static bool ThrowException;
			/**
			@brief 默认构造函数，构造一个未初始化的浮点数*/
			FloatN() {}
			/**
			@brief 通过传入符号、指数、有效数字 来构造*/
			FloatN(index_t sign, long long exp, FloatnData const& data) :Sign(sign), Exp(exp), Data(data) {}
			/**
			@brief 析构函数*/
			~FloatN() {}
#ifdef LK_MATH_FLOAT256_
			/**
			@brief 从Float256构造*/
			FloatN(Float256 const&f) : Sign(f.Sign), Exp(f.Exp), Data(f.Data) {}
#endif
			/**
			@brief 从double构造*/
			FloatN(double number)
			{
				unsigned long long num;
				num = reinterpret_cast<unsigned long long&>(number);
				if (number != 0)
				{
					Exp = ((0x7ff0000000000000 & num) >> 52) - 1023;
					Data.Clear();
					Data.arr[N - 1] = (num << 11 | 0x8000000000000000);
					if (number > 0)
						Sign = 1;
					else
						Sign = -1;
				}
				else
				{
					Exp = 0;
					Sign = 0;
					Data.Clear();
				}
			}
			FloatN(float number) { *this = FloatN((double)number); }
			FloatN(long long number)
			{
				if (number == 0)
				{
					Sign = 0;
					Exp = 0;
					Data.Clear();
				}
				else if (number < 0)
				{
					Sign = -1;
					Exp = 63 + Data.Clear(-number);
				}
				else
				{
					Sign = 1;
					Exp = 63 + Data.Clear(number);
				}
			}
			FloatN(unsigned long long number)
			{
				if (number == 0)
				{
					Sign = 0;
					Exp = 0;
					Data.Clear();
				}
				else
				{
					Sign = 1;
					Exp = 63 + Data.Clear(number);
				}
			}
			FloatN(long number) { *this = FloatN((long long)number); }
			FloatN(unsigned long number) { *this = FloatN((unsigned long long)number); }
			FloatN(int number) { *this = FloatN((long long)number); }
			FloatN(unsigned int number) { *this = FloatN((unsigned long long)number); }
			FloatN(short number) { *this = FloatN((long long)number); }
			FloatN(unsigned short number) { *this = FloatN((unsigned long long)number); }
			FloatN(signed char number) { *this = FloatN((long long)number); }
			FloatN(unsigned char number) { *this = FloatN((unsigned long long)number); }
			FloatN(char number) { *this = (char)-1 < 0 ? FloatN((long long)number) : FloatN((unsigned long long)number); }
			FloatN(bool number) :Sign(number), Exp(0) { Data.Clear(number ? MinValues::Int64 : 0); }
			FloatN(const std::string& number) :Sign(0), Exp(0), Data()
			{
				char const* pstr = number.c_str();
				index_t len = number.size();
				if (len != 0)
				{
					index_t expindex = 0;
					index_t point = number.size();
					while (expindex != len) { if (pstr[expindex] == '.')point = expindex; else if (pstr[expindex] == 'e' || pstr[expindex] == 'E')break; expindex++; }
					if (point > expindex)point = expindex;
					long long et = expindex < len - 1 ? Strings::ToInt64(number.substr(expindex + 1, len - (expindex + 1))) : 0;//指数
					index_t i = 0;
					unsigned long long ull;
					while (i < point - 19)
					{
						ull = 0;
						for (index_t j = 0; j < 19; ++j)ull = ull * 10 + (pstr[i + j] - '0');
						*this = *this * FloatN(10000000000000000000ull) + FloatN(ull);
						i += 19;
					}
					if (i < point)
					{
						ull = 0;
						unsigned long long tmp = 1;
						while (i < point) { ull = ull * 10 + (pstr[i] - '0'); tmp *= 10; ++i; }
						*this = *this * FloatN(tmp) + FloatN(ull);
					}
					if (point < expindex - 1)
					{
						i = point + 1;
						while (i < expindex - 19)
						{
							ull = 0;
							for (index_t j = 0; j < 19; ++j)ull = ull * 10 + (pstr[i + j] - '0');
							*this = *this * FloatN(10000000000000000000ull) + FloatN(ull);
							i += 19;
						}
						if (i < expindex)
						{
							ull = 0;
							unsigned long long tmp = 1;
							while (i < expindex) { ull = ull * 10 + (pstr[i] - '0'); tmp *= 10; ++i; }
							*this = *this * FloatN(tmp) + FloatN(ull);
						}
						*this *= FloatN(FloatN::Ten()).Pow(FloatN(et - (expindex - point - 1)));
					}
					else *this *= FloatN(FloatN::Ten()).Pow(FloatN(et));
				}
			}
			FloatN& operator=(const FloatN& f) { if (&f != this) { Sign = f.Sign; Exp = f.Exp; Data = f.Data; }return *this; }
			/**
			@brief 转换为double型数据*/
			double ToDouble()const
			{
				if (Equals0())return 0;
				if (IsError())
				{
					switch (Sign)
					{
					case MathError::Divide0:
					case MathError::Arccos:
					case MathError::Arcsin:
					case MathError::Cot:
					case MathError::Tan:
					case MathError::LnLessOrEqual0:
					case MathError::ModDivide0:
					case MathError::Pow00:
					case MathError::PowLessThan0:
					case MathError::SqrtLessThan0:
					default:return 0.0 / Zero().ToDouble();
					}
					return MaxValues::Double * 2;
				}
				union { unsigned long long ull; double number; }num;
				num.ull = 0;
				if (Exp >= -1023)
				{
					if (Exp > 1024)
					{
						if (Sign < 0)num.ull = 0xfff0000000000000;
						else num.ull = 0x7ff0000000000000;
						return num.number;
					}
					num.ull |= ((unsigned long long)(Exp + 1023)) << 52;//指数
					if (Sign < 0)num.ull |= MinValues::Int64;//负号
					num.ull |= (Data.arr[N - 1] & MaxValues::Int64) >> 11;//尾数
					if (Data.arr[N - 1] & 0x400)num.ull++;//类似十进制的四舍五入
				}
				return num.number;
			}
			unsigned long long ToUInt64()const
			{
				return Bits::GetWindow64High64(Data.arr, N, 63 - Exp);
			}
			float ToFloat()const { return (float)ToDouble(); }
			unsigned int ToUInt32()const { return (unsigned int)ToUInt64(); }
			unsigned short ToUInt16()const { return (unsigned short)ToUInt64(); }
			/**
			@brief 转换到64位整形，按截取的方式*/
			long long ToInt64()const
			{
				if (Sign < 0)
					return -(long long)ToUInt64();
				return ToUInt64();
			}
			int ToInt32()const { return (int)ToInt64(); }
			short ToInt16()const { return (short)ToInt64(); }
#ifdef LK_MATH_FLOAT256_
			Float256 ToFloat256()const
			{
				if (Exp < Float256::ExpMinValue)return Float256::Zero();
				Float256 f;
				if (Exp > Float256::ExpMaxValue)return f.MakeError(MathError::InputOverflow);
				f.Exp = Exp;
				f.Sign = Sign;
				for (size_t i = 0; i != N&&i != 4; ++i)
				{
					f.Data.ull[3 - i] = Data.arr[(N - 1) - i];
				}
				return f;
			}
#endif
			std::string ToString(size_t len = 6)const
			{
				if (Equals0())
					return "0";
				if (IsError())//错误信息输出
				{
					return Sign>1? "未知错异常":(Sign < -23 ? "未知错误" : MathError::description[-(int)Sign]);
				}
				if (len == 0)len = N * 64;
				std::string str;
				str.resize(len + 32);
				char* pstr = &str[0];
				index_t exp = (index_t)std::ceil((double)Exp / std::log(10.0)*std::log(2.0));
				FloatN tmp = Abs();
				tmp *= Ten().Pow(-exp);
				while (tmp >= One()) { tmp /= 10; exp++; }
				size_t i = 0;
				if (Sign < 0)*pstr++ = '-';
				while (i <= len)
				{
					if (tmp.IsError() || tmp.Equals0())break;
					unsigned long long ull = (tmp *= 10000000000000000000ull).ToUInt64();
					tmp.SelfGetDecimalPart();
					if (i == 0)
					{
						if (ull == 0) { exp -= 19; continue; }
						while (ull < 1000000000000000000ull) { i--; ull *= 10; exp--; }
					}
					i += 19;
					for (size_t j = 0; j != 19;) { pstr[i - (++j)] = L'0' + ull % 10; ull /= 10; }
				}
				if (i > len + 1) { pstr[(i = len + 1)] = 0; }
				if (i > len)
				{
					if (pstr[len] >= '5')//产生进位
					{
						i = len;
						while (i-- != 0)if (++pstr[i] <= '9')break;
						if (++i == 0) { exp++; pstr[0] = '1'; i = 1; }
						pstr[i] = 0;
					}
					else pstr[i = len] = 0;
				}
				while (i > 1 && pstr[i - 1] == '0')--i;
				pstr[i] = 0;
				//i在此时表示有数据部位长度
				if (exp<-2 || exp>(index_t)i + 3)//采用科学计数法
				{
					for (size_t j = i; j > 1; --j)pstr[j] = pstr[j - 1];
					if (i > 1) { pstr[1] = '.'; ++i; }
					pstr[i++] = 'e';
					std::string s = Strings::FromInt64(exp - 1);
					size_t j = 0;
					while (j < s.length())pstr[i++] = s[j++];
					pstr[i] = 0;
				}
				else if (exp >= -2 && exp <= 0)//前插0
				{
					i += 2 - exp;
					pstr[i] = 0;
					while ((index_t)i > 2 - exp) { --i; pstr[i] = pstr[i + exp - 2]; }
					while (--i != 0)pstr[i] = '0';
					pstr[0] = '0';
					pstr[1] = '.';
				}
				else if (exp >= (index_t)i && exp <= (index_t)i + 3)//后插0
				{
					while ((index_t)i < exp)pstr[i++] = '0';
					pstr[i] = 0;
				}
				else//内插小数点
				{
					pstr[i + 1] = 0;
					while ((index_t)i > exp) { pstr[i] = pstr[i - 1]; --i; }
					pstr[exp] = '.';
				}
				str.resize(Strings::strlen(str.c_str()));
				return str;
			}
			/**
			@brief 判断是否出错*/
			inline bool IsError()const { return Sign < -1; }
			/**
			@brief 设置为错误状态*/
			inline FloatN& MakeError(index_t errorcode) {
				Sign = errorcode;
				if (ThrowException)Throw<MathException >(Sign <= -24 ? "未知错误" : (Sign>1? "未知异常" :MathError::description[-Sign]));
				return *this;
			}
			/**
			@brief 相加溢出检查，上限溢出返回1，下限溢出返回-1，正常返回0*/
			inline static index_t PlusOverflowCheck(long long num1, long long num2, long long*presult)
			{
				long long num = num1 + num2;
				if (num1 > 0)
				{
					if (num2 > 0 && num < 0)return 1;
				}
				else if (num2 < 0 && num > 0)return -1;
				if (presult)*presult = num;
				return 0;
			}
			/**
			@brief 相减溢出检查，上限溢出返回1，下限溢出返回-1，正常返回0*/
			inline static index_t MinusOverflowCheck(long long num1, long long num2, long long*presult)
			{
				long long num = num1 - num2;
				if (num1 > 0)
				{
					if (num2 < 0 && num < 0)return 1;
				}
				else if (num2 > 0 && num > 0)return -1;
				if (presult)*presult = num;
				return 0;
			}
			/**
			@brief 判断是否等于0*/
			inline bool Equals0()const { return !Sign; }
			/**
			@brief 负号重载*/
			FloatN operator-()const
			{
				if (IsError() || Equals0())return*this;
				return FloatN(-Sign, Exp, Data);
			}
			/**
			@brief 自身取相反值*/
			FloatN& SelfNegative()
			{
				if (!IsError())Sign = -Sign;;
				return *this;
			}
			/**
			@brief 正号重载*/
			FloatN operator+()const { return *this; }
			/**
			@brief 前置运算重载*/
			FloatN &operator++()
			{
				if (IsError())return*this;
				if (Equals0())return *this = One();
				if (Exp >= (long long)(N*sizeof(unsigned long long) * 8))
				{
					if (Exp == (N*sizeof(unsigned long long) * 8)) {
						if (Sign > 0) {
							if (++Data)
							{
								for (size_t i = 0; i < N - 1; ++i) { Data.arr[i] = (Data.arr[i] >> 1) | (Data.arr[i + 1] << (sizeof(unsigned long long) * 8 - 1)); }
								Data.arr[N - 1] = (Data.arr[N - 1] >> 1) | MinValues::Int64;
								Exp++;
							}
						}
						else {
							--Data; if ((long long)Data.arr[N - 1]>0)
							{
								for (size_t i = 0; i < N - 1; ++i) { Data.arr[i] = (Data.arr[i] >> 1) | (Data.arr[i + 1] << (sizeof(unsigned long long) * 8 - 1)); }
								Data.arr[N - 1] = (Data.arr[N - 1] >> 1) | MinValues::Int64;
								Exp--;
							}
						}
					}
					return *this;
				}
				if (Exp <= -(long long)(N*sizeof(unsigned long long) * 8))
				{
					if (Exp == -(long long)(N*sizeof(unsigned long long) * 8)) {
						if (Sign > 0) {
							Data.Clear(MinValues::Int64);
							Data.arr[0] = 1;
							Exp = 0;
						}
						else
						{
							for (size_t i = 0; i < N; ++i)Data.arr[i] = -1;
							Exp = -1;
						}
					}
					return *this;
				}
				if (Sign > 0)
				{
					if (Exp > 0)
					{
						if (Bits::SelfPlusOneRound64(Data.arr, N, (long long)(N*sizeof(unsigned long long) * 8) - 1 - Exp))
						{
							for (size_t i = 0; i < N - 1; ++i) { Data.arr[i] = (Data.arr[i] >> 1) | (Data.arr[i + 1] << (sizeof(unsigned long long) * 8 - 1)); }
							Data.arr[N - 1] = (Data.arr[N - 1] >> 1) | MinValues::Int64;
							++Exp;
						}
					}
					else
					{
						unsigned long long flag = Data >>= -Exp;
						if ((long long)flag < 0)++Data;
						if ((long long)Data.arr[N - 1] < 0)
						{
							Data.arr[N - 1] ^= MinValues::Int64;
							for (size_t i = 0; i < N - 1; ++i) { Data.arr[i] = (Data.arr[i] >> 1) | (Data.arr[i + 1] << (sizeof(unsigned long long) * 8 - 1)); }
							Data.arr[N - 1] = (Data.arr[N - 1] >> 1) | MinValues::Int64;
							Exp = 1;
						}
						else { Data.arr[N - 1] |= MinValues::Int64; Exp = 0; }
					}
					return *this;
				}
				if (Exp >= 0)
				{
					Bits::SelfMinusOneRound64(Data.arr, N, (long long)(N*sizeof(unsigned long long) * 8) - 1 - Exp);
					long long tmp = Data.BitsZeroCount();
					if (tmp == (long long)(N*sizeof(unsigned long long) * 8))return *this = Zero();
					Exp -= tmp;
					Data <<= tmp;
				}
				else
				{
					long long flag = -Exp;
					unsigned long long low = Data >>= flag;
					Data.arr[N - 1] = (1ull << (sizeof(unsigned long long) * 8 - 1)) - Data.arr[N - 1];
					if (N > 1) {
						flag = 0;
						if (Data.arr[0] != 0) { Data.arr[0] = -(long long)Data.arr[0]; flag = true; }
						for (size_t i = 1; i < N - 1; ++i)
						{
							if (flag) { Data.arr[i] = ~Data.arr[i]; }
							else if (Data.arr[i] != 0) { Data.arr[i] = -(long long)Data.arr[i]; flag = true; }
						}
						if (flag)--Data.arr[N - 1];
					}
					flag = Data.BitsZeroCount();
					if (flag == (long long)(N*sizeof(unsigned long long) * 8))return *this = Zero();
					if (flag)
					{
						Data <<= flag;
						low >>= (long long)(N*sizeof(unsigned long long) * 8) - flag;
						if (Data.arr[0] < low)
						{
							Data.arr[0] -= low;
							flag = true;
							for (size_t i = 0; i < N - 1; ++i)
								if (--Data.arr[i] != -1) { flag = false; break; }
							if (flag&&--Data.arr[N - 1] <= MaxValues::Int64)
							{
								for (size_t i = N; i > 1;)
								{
									i--;
									Data.arr[i] = (Data.arr[i] << 1) | ((long long)Data.arr[i - 1] < 0);
								}
								Data.arr[0] <<= 1;
								--Exp;
							}
						}
						else Data.arr[0] -= low;
					}
					Exp = -flag;
					Sign = 1;
				}
				return *this;
			}
			/**
			@brief 前置运算重载*/
			FloatN &operator--() { return (++SelfNegative()).SelfNegative(); }
			/**
			@brief 后置运算重载*/
			FloatN operator++(int)
			{
				FloatN x = *this; ++*this; return x;
			}
			/**
			@brief 后置运算重载*/
			FloatN operator--(int) { FloatN x = *this; --*this; return x; }
			/**
			@brief 加法重载*/
			FloatN& operator+=(FloatN const& number)
			{
				if (Equals0() || number.IsError() || number.Exp > 0 && number.Exp - (s64)N * 64 > Exp || number.Exp - (s64)N * 64 < 0 && number.Exp - (s64)N * 64 > Exp)return *this = number;//进行错误传递
				if (number.Equals0() || IsError() || Exp > 0 && Exp - (s64)N * 64 > number.Exp || Exp - (s64)N * 64 < 0 && Exp - (s64)N * 64 > number.Exp)return *this;//进行错误传递
				if (&number == this)
				{
					if (Exp == MaxValues::Int64)return MakeError(MathError::PlusExpOverflow);
					++Exp;
					return *this;
				}
				if (Sign == number.Sign)
				{
					if (Exp > number.Exp)
					{
						if (Data.SelfPlus(number.Data, number.Exp - Exp))
						{
							if (Exp == MaxValues::Int64)return MakeError(MathError::PlusExpOverflow);
							if ((Data.arr[0] & 1) != 0)
							{
								Data >>= 1;
								++Data;
							}
							else Data >>= 1;
							Data.arr[N - 1] |= MinValues::Int64;
							++Exp;
						}
						return *this;
					}
					FloatnData d = number.Data;
					if (d.SelfPlus(Data, Exp - number.Exp))
					{
						if (number.Exp == MaxValues::Int64)return MakeError(MathError::PlusExpOverflow);
						if ((d.arr[0] & 1) != 0)
						{
							d >>= 1;
							++d;
						}
						else d >>= 1;
						d.arr[N - 1] |= MinValues::Int64;
						Exp = number.Exp + 1;
					}
					else Exp = number.Exp;
					Data = d;
					return *this;
				}
				if (Exp > number.Exp || Exp == number.Exp&&Data.CompareWith(number.Data) >= 0)
				{
					long long mov = Data.SelfMinus(number.Data, number.Exp - Exp);
					if (mov == ((s64)N*-64ll) || Exp < 0 && (Exp + mov)>0)return *this = 0;
					Exp += mov;
					return *this;
				}
				FloatnData d = number.Data;
				long long mov = d.SelfMinus(Data, Exp - number.Exp);
				if (number.Exp < 0 && (number.Exp + mov)>0)return *this = 0;
				Exp = number.Exp + mov;
				Data = d;
				Sign = number.Sign;
				return *this;
			}
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN&>::type operator+=(const T& t) { return operator+=(FloatN(t)); }
			/**
			@brief +重载*/
			FloatN operator+(FloatN number)const
			{
				FloatN t = *this;
				if (t.Equals0() || number.IsError())return number;//进行错误传递
				if (number.Equals0() || t.IsError())return t;//进行错误传递
				unsigned long long low = 0;
				if (number.Exp != t.Exp) {//指数位不同
					if (number.Exp < t.Exp)BigObject::Swap(t, number);//使得number的绝对值总是大于t
					if (number.Exp - t.Exp >(long long)(N*sizeof(unsigned long long) * 8)) {
						if (number.Exp - t.Exp == (long long)(N*sizeof(unsigned long long) * 8)) {
							if (number.Sign == t.Sign) {
								if (++number.Data) {
									if (number.Exp == MaxValues::Int64)
										return number.MakeError(MathError::PlusExpOverflow);
									++number.Exp;
									number.Data.arr[N - 1] = MinValues::Int64;
								}
							}
							else {
								--number.Data;
								if (number.Data.arr[N - 1] <= MaxValues::Int64) {
									if (number.Exp == MinValues::Int64)
										return FloatN::Zero();
									--number.Exp;
									for (size_t i = N; i > 1;)
									{
										i--;
										number.Data.arr[i] = (number.Data.arr[i] << 1) | ((long long)number.Data.arr[i - 1] < 0);
									}
									number.Data.arr[0] <<= 1;
								}
							}
						}
						return number;
					}
					low = t.Data >>= number.Exp - t.Exp;
				}

				if (number.Sign == t.Sign) {//符号位相同
					if (Bits::SelfPlus64(number.Data.arr, N, t.Data.arr, N, 0) || ((long long)low < 0 && ++number.Data)) {
						if (number.Exp == MaxValues::Int64)
							return number.MakeError(MathError::PlusExpOverflow);
						number.Exp++;
						for (size_t i = 0; i < N - 1; ++i) { number.Data.arr[i] = (number.Data.arr[i] >> 1) | (number.Data.arr[i + 1] << (sizeof(unsigned long long) * 8 - 1)); }
						number.Data.arr[N - 1] = (number.Data.arr[N - 1] >> 1) | MinValues::Int64;
					}
					return number;
				}
				if (number.Data < t.Data)BigObject::Swap(number, t);
				number.Data -= t.Data;
				long long mov = number.Data.BitsZeroCount();
				if (mov == (long long)(N*sizeof(unsigned long long) * 8) || FloatN::MinusOverflowCheck(number.Exp, mov, NULL) < 0)return Float256::Zero();
				if (mov)
				{
					number.Data <<= mov;
					Bits::SelfMinusInt64(number.Data.arr, N, low, mov - 64);
					if ((long long)number.Data.arr[N - 1] > 0)
					{
						++mov;
						if (mov == (long long)(N*sizeof(unsigned long long) * 8) || FloatN::MinusOverflowCheck(number.Exp, mov, NULL) < 0)return Float256::Zero();
						for (size_t i = N; i > 1;)
						{
							i--;
							number.Data.arr[i] = (number.Data.arr[i] << 1) | ((long long)number.Data.arr[i - 1] < 0);
						}
						number.Data.arr[0] <<= 1;
					}
					number.Exp -= mov;
				}
				return number;
			}
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator+(const T& t)const { return operator+(FloatN(t)); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator+(const T& t, const FloatN& vi) { return vi + t; }
			/**
			@brief 减法重载*/
			FloatN& operator-=(FloatN const& number) { return *this += (-number); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN&>::type operator-=(const T& t) { return operator-=(FloatN(t)); }
			FloatN operator-(FloatN const& number2)const { return *this + (-number2); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator-(const T& t)const { return operator-(FloatN(t)); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator-(const T& t, const FloatN& vi) { return (vi - t).SelfNegative(); }
			/**
			@brief 乘法重载*/
			FloatN& operator*=(FloatN const& number)
			{
				return *this = *this*number;
			}
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN&>::type operator*=(const T& t) { return operator*=(FloatN(t)); }
			friend FloatN operator*(FloatN const& n1, FloatN const& n2)
			{
				if (n1.IsError())return n1;
				if (n2.IsError())return n2;
				if (n1.Equals0() || n2.Equals0())return FloatN::Zero();
				u8 arr [sizeof(FloatN)];
				FloatN* p = (FloatN*)(unsigned char const*)arr;
				unsigned short result;
#ifdef LK_MATH_DOUBLECOMPLEX_
				if ((1ull << (sizeof(N) * 8 - Bits::LeftZeroCount(N))) < N / 40.0 *N)
				{
					result = Bits::MultiplyFFTHigh((unsigned short*)(p->Data.arr), N*sizeof(unsigned long long) / sizeof(unsigned short), (unsigned short*)(n1.Data.arr), N*sizeof(unsigned long long) / sizeof(unsigned short), (unsigned short*)(n2.Data.arr), N*sizeof(unsigned long long) / sizeof(unsigned short));
				}
				else
#endif
					result = Bits::MultiplyHigh64(p->Data.arr, N, n1.Data.arr, N, n2.Data.arr, N) >> ((sizeof(unsigned long long) - sizeof(unsigned short)) * 8);
 				index_t k;
				if ((s64)p->Data.arr[N - 1] > 0)
				{
					for (size_t i = N; i > 1;)
					{
						i--;
						p->Data.arr[i] = (p->Data.arr[i] << 1) | ((s64)p->Data.arr[i - 1] < 0);
					}
					p->Data.arr[0] = (p->Data.arr[0] << 1) | ((short)result < 0);
					result <<= 1;
					k = FloatN::PlusOverflowCheck(n1.Exp, n2.Exp, &(p->Exp));
				}
				else
				{
					if (n1.Exp != MaxValues::Int64)k = FloatN::PlusOverflowCheck(n1.Exp + 1, n2.Exp, &(p->Exp));
					else { k = n2.Exp >= 0 ? 1 : 0; p->Exp = n1.Exp + n2.Exp + 1; }
				}

				if (k > 0) {
					return p->MakeError(MathError::MultiplyExpOverflow);
				}
				if (k < 0) {
					return Zero(); 
				}

				if ((short)result < 0 && ++(p->Data))
				{
					p->Data.arr[N - 1] = MinValues::Int64;
					if (p->Exp == MaxValues::Int64) {
						return p->MakeError(MathError::MultiplyExpOverflow);
					}
					++(p->Exp);
				}
				p->Sign = n1.Sign == n2.Sign ? 1 : -1;
				return *p;
			}
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator*(const T& t)const { return *this*(FloatN(t)); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator*(const T& t, const FloatN& vi) { return vi * t; }

			/**
			@brief 快速乘10*/
			FloatN Multiply10()const { return FloatN(*this).SelfMultiply10(); }
			/**
			@brief 自身快速自乘10*/
			FloatN& SelfMultiply10()
			{
				if (IsError() || Equals0())return *this;
				if (Exp > MaxValues::Int64 ^ 3)return MakeError(MathError::MultiplyExpOverflow);
				else Exp += 3;//乘8

				FloatnData fd = Data;
				fd >>= 2;
				if (Data.arr[0] & 0x2)++fd;
				if (Data += fd)
				{
					if (Exp == MaxValues::Int64)
						return MakeError(MathError::MultiplyExpOverflow);
					Exp++;
					Data >>= 1;
					Data.arr[N - 1] |= MinValues::Int64;
				}
				return *this;
			}

			//除法重载

			/**
			@brief 返回该数字的一半*/
			FloatN Half()const { if (IsError() || Equals0())return *this; if (Exp == MinValues::Int64)return 0; return FloatN(Sign, Exp - 1, Data); }
			/**
			@brief 乘以2的-x次方*/
			FloatN Half(long long x)const
			{
				if (IsError() || Equals0() || x == 0)return *this;
				index_t k = MinusOverflowCheck(Exp, x, NULL);
				if (k < 0)return Zero();
				if (k > 0)return FloatN().MakeError(MathError::Exp2Overflow);
				return FloatN(Sign, Exp - x, Data);
			}
			/**
			@brief 该数字自身取自身的一半*/
			FloatN& SelfHalf() { if (IsError() || Equals0())return *this; if (Exp == MinValues::Int64)return *this = 0; Exp--; return *this; }
			/**
			@brief 自身乘以2的-x次方*/
			FloatN& SelfHalf(long long x)
			{
				if (IsError() || Equals0() || x == 0)return *this;
				index_t k = MinusOverflowCheck(Exp, x, &Exp);
				if (k > 0)return MakeError(MathError::Exp2Overflow);
				if (k < 0)*this = Zero();
				return *this;
			}

			FloatN& operator/=(FloatN const& number)
			{
				return *this = *this / number;
			}
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN&>::type operator/=(const T& t) { return operator/=(FloatN(t)); }
			FloatN operator/(FloatN const& n2)const
			{
				FloatN const& n1 = *this;
				if (n1.IsError())return n1;
				if (n2.IsError())return n2;
				if (n2.Equals0())return FloatN().MakeError(MathError::Divide0);
				if (n1.Equals0())return n1;
				u8 arr[sizeof(FloatN)];
				FloatN* p = (FloatN*)arr;
				unsigned long long result;
				index_t k;
				if (n1.Data >= n2.Data)
				{
					Bits::DivideHigh64(p->Data.arr, N, n1.Data.arr, N, n2.Data.arr, N, true, &result, 1);
					k = FloatN::MinusOverflowCheck(n1.Exp, n2.Exp, &(p->Exp));
				}
				else
				{
					Bits::DivideHigh64(p->Data.arr, N, n1.Data.arr, N, n2.Data.arr, N, false, &result, 1);
					if (n1.Exp != MinValues::Int64) k = FloatN::MinusOverflowCheck(n1.Exp - 1, n2.Exp, &(p->Exp));
					else { k = n2.Exp < 0 ? 0 : -1; p->Exp = n1.Exp - n2.Exp - 1; }
				}
				if (k > 0) {
					return p->MakeError(MathError::DivideExpOverflow);
				}
				if (k < 0){
			 		return Zero();
				}

				if ((long long)result < 0 && ++(p->Data))
				{
					p->Data.arr[N - 1] = MinValues::Int64;
					++(p->Exp);
				}
				p->Sign = n1.Sign == n2.Sign ? 1 : -1;
				return *p;
			}
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator/(const T& t)const { return *this/(FloatN(t)); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, FloatN>::type operator/(const T& t, const FloatN& vi) { return FloatN(t) / vi; }

			/**
			@brief 求余*/
			FloatN& operator%=(const FloatN& f) { return *this = Mod(f); }
			friend FloatN operator%(const FloatN& f1, const FloatN& f2) { return f1.Mod(f2); }
			/*求幂*/
			FloatN& operator^=(const FloatN& f) { return *this = Pow(f); }
			friend FloatN operator^(const FloatN& f1, const FloatN& f2) { return f1.Pow(f2); }

			/*逻辑运算符*/
			bool operator !()const { return !Equals0(); }
			/*比较运算符*/
			friend bool operator>(FloatN const& number1, FloatN const& number2)
			{
				if (number1.Equals0() && number2.Equals0())return true;
				if (number1.Sign != number2.Sign)return number1.Sign > number2.Sign;
				if (number1.Exp != number2.Exp)return number1.Sign < 0 ? number1.Exp<number2.Exp : number1.Exp>number2.Exp;
				return number1.Sign < 0 ? number1.Data.CompareWith(number2.Data) < 0 : number1.Data.CompareWith(number2.Data) > 0;
			}
			friend bool operator<(FloatN const& number1, FloatN const& number2)
			{
				if (number1.Equals0() && number2.Equals0())return true;
				if (number1.Sign != number2.Sign)return number1.Sign < number2.Sign;
				if (number1.Exp != number2.Exp)return number1.Sign < 0 ? number1.Exp>number2.Exp:number1.Exp < number2.Exp;
				return number1.Sign < 0 ? number1.Data.CompareWith(number2.Data)>0 : number1.Data.CompareWith(number2.Data) < 0;
			}
			friend bool operator>=(FloatN const& number1, FloatN const& number2)
			{
				if (number1.Equals0() && number2.Equals0())return true;
				if (number1.Sign != number2.Sign)return number1.Sign > number2.Sign;
				if (number1.Exp != number2.Exp)return number1.Sign < 0 ? number1.Exp<number2.Exp : number1.Exp>number2.Exp;
				return number1.Sign < 0 ? number1.Data.CompareWith(number2.Data) <= 0 : number1.Data.CompareWith(number2.Data) >= 0;
			}
			friend bool operator<=(FloatN const& number1, FloatN const& number2)
			{
				if (number1.Equals0() && number2.Equals0())return true;
				if (number1.Sign != number2.Sign)return number1.Sign < number2.Sign;
				if (number1.Exp != number2.Exp)return number1.Sign < 0 ? number1.Exp>number2.Exp:number1.Exp < number2.Exp;
				return number1.Sign < 0 ? number1.Data.CompareWith(number2.Data) >= 0 : number1.Data.CompareWith(number2.Data) <= 0;
			}
			friend bool operator==(FloatN const& number1, FloatN const& number2) { return number1.Data.CompareWith(number2.Data) == 0 && number1.Exp == number2.Exp&&number1.Sign == number2.Sign; }
			friend bool operator!=(FloatN const& number1, FloatN const& number2) { return number1.Data.CompareWith(number2.Data) != 0 || number1.Exp != number2.Exp || number1.Sign != number2.Sign; }
			/**
			@brief开平方*/
			FloatN Sqrt()const
			{
				if (Equals0() || IsError())return *this;
				if (Equals(One()))return *this;
				FloatN num(*this);
				if (num.Sign < 0 || num.Sign>1)return num.MakeError(MathError::SqrtLessThan0);
				FloatN num1 = num;
				long long te = num1.Exp&((long long)MaxValues::UInt64 ^ 1);
				num1.Exp ^= te;
				num1 = std::sqrt(num1.ToDouble());//近似值
				num1.Exp += te >> 1;
				FloatN num2 = num / num1;
				size_t mexp = N;
				while (mexp > 1 || !num1.AppEquals(num2))//牛顿迭代
				{
					if (num1.Exp == num2.Exp)
					{
						bool last = false;
						for (size_t i = 0; i < mexp || (last &&i < N && (mexp = i + 1)); ++i)
						{
							unsigned long long tmp;
							if (last)//进位
							{
								last = num1.Data.arr[i] >= ~num2.Data.arr[i];//本次进位
								tmp = num1.Data.arr[i] + num2.Data.arr[i] + 1;
							}
							else
							{
								last = num1.Data.arr[i] > ~num2.Data.arr[i];//本次进位
								tmp = num1.Data.arr[i] + num2.Data.arr[i];
							}
							if (i != 0 && (tmp & 1))num1.Data.arr[i - 1] |= MinValues::Int64;
							num1.Data.arr[i] = tmp >> 1;
						}
						if (last)num1.Data.arr[mexp - 1] |= MinValues::Int64;
						while (mexp > 1 && num1.Data.arr[mexp - 1] == num2.Data.arr[mexp - 1])mexp--;
					}
					else {
						num1 = num1 + num2;
						num1.Exp--;
					}
					num2 = num / num1;
				}
				return num2;
			}
			/**
			@brief开平方*/
			static inline FloatN Sqrt(FloatN const&f) { return f.Sqrt(); }
			/**
			@brief 取绝对值*/
			FloatN Abs()const { if (Sign != -1)return *this; return FloatN(-Sign, Exp, Data); }
			/**
			@brief 取绝对值*/
			static inline FloatN Abs(FloatN const& f) { return f.Abs(); }
			/**
			@brief 自身取绝对值*/
			FloatN& SelfAbs() { if (Sign != -1)return *this; Sign = 1; return *this; }
			/**
			@brief 求余(>=0),商保存到quotient里，余数与num同号；num<0时向上取整，num>0时向下取整
			@note 要求num与quotient不能是同一个对象*/
			FloatN Mod(FloatN const& num, FloatN &quotient)const
			{
				if (IsError())return *this;
				if (num.IsError())return num;
				if (num.Equals0())return FloatN().MakeError(MathError::ModDivide0);
				if (Exp < num.Exp)//0.1%2=0...0.1，0.1%-2=-1...-1.9，-0.1%2=-1...1.9，-0.1%-2=0...-0.1
				{
					if (Sign == num.Sign)quotient = Zero();
					else { quotient = One(); quotient.Sign = -1; }
					if (Sign == num.Sign)return *this;
					return *this + num;
				}
				index_t cmp = Data.CompareWith(num.Data);
				if (cmp == 0)
				{
					quotient = One();
					quotient.Sign = Sign == num.Sign ? 1 : -1;
					cmp = MinusOverflowCheck(Exp, num.Exp, &quotient.Exp);
					if (cmp < 0)quotient = Zero();
					else if (cmp>0)quotient.MakeError(MathError::DivideExpOverflow);
					return Zero();
				}
				if (Exp == num.Exp) {
					if (cmp > 0)//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
					{
						if (Sign == num.Sign)quotient = One();
						else { quotient = Two(); quotient.Sign = -1; }
						if (Sign == num.Sign)return *this - num;
						return *this + num.Half(-1);
					}
					else
					{
						if (Sign == num.Sign)quotient = Zero();
						else { quotient = One(); quotient.Sign = -1; }
						if (Sign == num.Sign)return *this;
						return *this + num;
					}
				}
				Collection::PArray<unsigned char> arr = new Collection::FixedArray<u8,sizeof(FloatN)>(sizeof(FloatN));
				FloatN* p = (FloatN*)(unsigned char*)arr;
				if ((double)Exp - (double)num.Exp > (double)N * 64) { quotient = *this / num; return Zero(); }
				if (cmp > 0)
				{
					Bits::ModHigh64(quotient.Data.arr, N, Exp - num.Exp + 1, Data.arr, N, num.Data.arr, N, true, p->Data.arr, N);
				}
				else
				{
					Bits::ModHigh64(quotient.Data.arr, N, Exp - num.Exp, Data.arr, N, num.Data.arr, N, false, p->Data.arr, N);
				}
				long long mov = p->Data.BitsZeroCount();
				if (mov == N * 64)
				{
					quotient.Sign = Sign == num.Sign ? 1 : -1;
					cmp = MinusOverflowCheck(Exp, num.Exp + (cmp < 0), &quotient.Exp);
					if (cmp < 0)quotient = Zero();
					else if (cmp>0)quotient.MakeError(MathError::DivideExpOverflow);
					return Zero();
				}
				p->Data <<= mov;
				if (MinusOverflowCheck(num.Exp, mov, &p->Exp) < 0)p->Sign = 0;
				else p->Sign = num.Sign;
				quotient.Sign = 1;
				cmp = MinusOverflowCheck(Exp, num.Exp + (cmp < 0), &quotient.Exp);
				if (cmp < 0)quotient = Zero();
				else if (cmp>0)quotient.MakeError(MathError::DivideExpOverflow);

				//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
				if (Sign != num.Sign)
				{
					++quotient; quotient.Sign = -1;
					*p = num - *p;
				}
				return *p;
			}
			/**
			@brief 求余*/
			FloatN Mod(FloatN const& num)const
			{
				if (IsError())return *this;
				if (num.IsError())return num;
				if (num.Equals0())return FloatN().MakeError(MathError::ModDivide0);
				if (Exp < num.Exp)//0.1%2=0...0.1，0.1%-2=-1...-1.9，-0.1%2=-1...1.9，-0.1%-2=0...-0.1
				{
					if (Sign == num.Sign)return *this;
					return *this + num;
				}
				index_t cmp = Data.CompareWith(num.Data);
				if (cmp == 0)
					return Zero();
				if (Exp == num.Exp) {
					if (cmp > 0)//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
					{
						if (Sign == num.Sign)return *this - num;
						return *this + num.Half(-1);
					}
					else
					{
						if (Sign == num.Sign)return *this;
						return *this + num;
					}
				}
				Collection::PArray<unsigned char> arr = new Collection::FixedArray<u8,sizeof(FloatN)>(sizeof(FloatN));
				FloatN* p = (FloatN*)(unsigned char*)arr;
				if ((double)Exp - (double)num.Exp > (double)N * 64) { return Zero(); }
				if (cmp > 0)
				{
					Bits::ModHigh64(NULL, 0, Exp - num.Exp + 1, Data.arr, N, num.Data.arr, N, true, p->Data.arr, N);
				}
				else
				{
					Bits::ModHigh64(NULL, 0, Exp - num.Exp, Data.arr, N, num.Data.arr, N, false, p->Data.arr, N);
				}
				long long mov = p->Data.BitsZeroCount();
				if (mov == N * 64)
					return Zero();
				p->Data <<= mov;
				if (MinusOverflowCheck(num.Exp, mov, &p->Exp) < 0)p->Sign = 0;
				p->Sign = num.Sign;
				//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
				if (Sign != num.Sign)
					*p = num - *p;
				return *p;
			}
			/**
			@brief 求该数以number为底的对数*/
			FloatN Log(FloatN const& number)const
			{
				return Ln() / number.Ln();
			}
			/**
			@brief 求f2以f1为底的对数*/
			static inline FloatN Log(FloatN const& f1, FloatN const& f2) { return f2.Log(f1); }
			/**
			@brief 求该数以10为底的对数*/
			FloatN Lg()const
			{
				return Ln()* Calculate1Ln10();
			}
			/**
			@brief 求 f 以10为底的对数*/
			static inline FloatN Lg(FloatN const& f)
			{
				return f.Lg();
			}
			/**
			@brief 求该数以e为底的对数*/
			FloatN Ln()const
			{
				if (IsError())return *this;
				FloatN num_x;//记录x
				FloatN num_xn;//记录x^n
				num_x = *this;
				long long ln2 = Exp;//exp偏移
				num_x.Exp = 0;//使得num_x变为1.xxx
				FloatN ln_plus = 0;

				if (num_x <= Zero())return num_x.MakeError(MathError::LnLessOrEqual0);
				for (int i = 1; i < 12; ++i)
					if (num_x >= Cal2sqrt2()[i])
					{
						num_x *= Cal12sqrt2()[i];
						ln_plus += Calln2sqrt2()[i];
					}

				//展开Ln(1+x)=x-x^2/2+x^3/3-x^4/4
				//另一种展开方式令y=(x-1)/(x+1)：ln(x) = ln((1+y)/(1-y)) = 2y*(1+(y^2)/3+(y^4)/5+(y^6)/7+...)
				/*num_x -= 1;
				FloatN s = num_x;
				FloatN xn = -num_x*num_x;
				for (index_t i = 2;; ++i)
				{
				s += xn / i;
				if (xn.Exp-(64-Bits::LeftZeroCount64(i))<(long long)N*-64ll)break;
				xn *= -num_x;
				}*/
				FloatN num_y = (num_x - 1) / (1 + num_x);
				FloatN s = num_y;
				FloatN num_y2 = num_y*num_y;
				FloatN num_yn = num_y*num_y2;
				for (index_t i = 3;; i += 2)
				{
					s += num_yn / i;
					if (num_yn.Exp < (long long)N*-64ll)break;
					num_yn *= num_y2;
				}
				if (!s.Equals0())s.Exp++;

				s += ln_plus;
				if (ln2)return s + CalculateLn2()*ln2;
				return s;
			}
			/**
			@brief 求 f 以e为底的对数*/
			static inline FloatN Ln(FloatN const& f) { return f.Ln(); }
			/**
			@brief 求该数以2为底的对数*/
			FloatN Log2()const
			{
				return Ln() * Calculate1Ln2();
			}
			/**
			@brief 求 f 以2为底的对数*/
			static inline FloatN Log2(FloatN const& f) { return f.Log2(); }
			/**
			@brief 求e的该数次幂*/
			FloatN Expe()const
			{
				if (IsError())return *this;
				FloatN num_x, num_xn, num_factorial, num_1, num_2;
				if (Exp > 42)
				{
					if (Sign < 0)
						return Zero();
					num_x = *this;
					return num_x.MakeError(MathError::ExpEOverflow);
				}

				num_x = Mod(CalculateLn2(), num_factorial);
				long long expmov = (long long)num_factorial;
				int i;
				num_factorial = One();
				num_xn = num_x;
				num_1 = One() + num_xn;
				num_2 = num_1;
				for (i = 2;; i++)
				{
					num_xn = num_xn * num_x;
					num_factorial = num_factorial*i;
					num_1 += num_xn / num_factorial;
					if (num_1.AppEquals(num_2))break;
					num_2 = num_1;
				}
				if (expmov)num_1.Exp += expmov;
				return num_1;
			}
			/**
			@brief 求e的f次幂*/
			static inline FloatN Expe(FloatN const&f) { return f.Expe(); }
			/**
			@brief 求2的该数次幂*/
			FloatN Exp2()const
			{
				if (IsError())return *this;
				FloatN num_x, num_xn, num_factorial, num_1, num_2;
				if (Exp >= 63)
				{
					if (Sign < 0)
						return Zero();
					num_x = *this;
					return num_x.MakeError(MathError::Exp2Overflow);
				}

				num_x = GetDecimalPart();
				long long expmov = ToInt64();
				num_x = num_x*CalculateLn2();
				int i;
				num_factorial = One();
				num_xn = num_x;
				num_1 = One() + num_x;
				num_2 = num_1;
				for (i = 2;; i++)
				{
					num_xn *= num_x;
					num_factorial = num_factorial*i;
					num_1 += num_xn / num_factorial;
					if (num_1.Exp + num_xn.Exp - num_factorial.Exp < (long long)N*-64ll)break;
					num_2 = num_1;
				}
				if (expmov)num_1.Exp += expmov;
				return num_1;
			}
			/**
			@brief 求2的该数次幂*/
			static inline FloatN Exp2(FloatN const& f) { return f.Exp2(); }
			/**
			@brief 求该数的number次幂*/
			FloatN Pow(FloatN const& number)const
			{
				if (IsError())return(*this);
				if (number.IsError())return number;
				if (*this <= Zero())
				{
					FloatN num = *this;
					if (Equals0() && number.Equals0())return num.MakeError(MathError::Pow00);
					if (number.GetDecimalPart().Equals0())//整数次方
					{
						FloatN tmp = (Abs().Log2()*number).Exp2();
						if (tmp.IsError())tmp.MakeError(MathError::PowExpOverflow);
						if (number.Half().GetDecimalPart() != 0)return -tmp;
						return tmp;
					}
					return num.MakeError(MathError::PowLessThan0);
				}
				FloatN tmp = (Log2()*number).Exp2();//使用Exp2来算
				if (tmp.IsError())tmp.MakeError(MathError::PowExpOverflow);
				return tmp;
				//return this->Abs().Ln()*(number).Exp();//使用Exp来算
			}
			/**
			@brief 求 f 的number次幂*/
			static inline FloatN Pow(FloatN const& f, FloatN const& number) { return f.Pow(number); }
			/**
			@brief 求正弦(弧度制)*/
			FloatN Sin()const
			{
				if (IsError())return *this;
				FloatN num_x, num_xn, num_factorial, num_1, num_2, sum;
				num_x = Mod(CalculatePi(), num_factorial);//除以π以判断正负
				bool negative = num_factorial.ToInt64() & 1;//商为奇数说明结果为负
				num_x = num_x.Mod(_pi_half2(), num_factorial);//除以π/4以判断操作方法
				switch (num_factorial.ToInt64())
				{
				case 1://45~90°
					if (num_x.Equals0())//返回(根号2)/2
					{
						if (negative)return -_sqrt2_half();
						return _sqrt2_half();
					}
					num_x = _pi_half2() - num_x;//cos收敛速度快
					if (negative)return -num_x.Cos();
					return num_x.Cos();
				case 2://90~135°
					if (num_x.Equals0())
					{
						if (negative)return -One();
						return One();
					}
					if (negative)
						return -num_x.Cos();
					return num_x.Cos();
				case 3://135~180°
					num_x = _pi_half2() - num_x;
				default:
					break;
				}
				num_factorial = One();//阶乘
				num_xn = num_x;
				sum = num_x;
				num_2 = num_x;
				num_x *= num_x;
				int i;
				for (i = 2;; i++)//Sin x = x-x^3/3!+x^5/5!-
				{
					num_factorial = num_factorial*i;
					if (i & 1)
					{
						num_xn *= num_x;
						num_factorial.SelfNegative();
						num_1 = num_xn / num_factorial;
						sum += num_1;
						if (sum.Exp - num_1.Exp >= (long long)N * 64 || num_1.Equals0())break;
					}
				}
				if (negative)
				{
					return -sum;
				}
				return sum;
			}
			/**
			@brief 求正弦(弧度制)*/
			static inline FloatN Sin(FloatN const& f) { return f.Sin(); }
			/**
			@brief 求余弦(弧度制)*/
			FloatN Cos()const
			{
				if (IsError())return *this;
				FloatN num_x, num_xn, num_factorial, num_1, num_2, sum;
				num_x = Mod(CalculatePi(), num_factorial);//除以π以供判断正负
				bool negative = num_factorial.ToInt64() & 1;//商为奇时negative为true
				num_x = num_x.Mod(_pi_half2(), num_factorial);//除以π/4以判断操作方法
				switch (num_factorial.ToInt64())
				{
				case 1://45~90°
					if (num_x.Equals0())
					{
						if (negative)return -_sqrt2_half();
						return _sqrt2_half();
					}
					num_x = _pi_half2() - num_x;
					if (negative)return -num_x.Sin();
					return num_x.Sin();
				case 2://90~135°
					if (num_x.Equals0())
					{
						return Zero();
					}
					if (negative)return num_x.Sin();
					return -num_x.Sin();
				case 3:
					negative = !negative;
					num_x = _pi_half2() - num_x;
				default:
					break;
				}
				num_factorial = One();//阶乘
				num_xn = One();
				sum = One();
				num_2 = One();
				num_x *= num_x;//变x方
				int i;
				for (i = 2;; i++)//Cosx = 1-x^2/2!+x^4/4!-
				{
					num_factorial = num_factorial*i;
					if (i & 1)continue;
					num_xn = num_xn*num_x;
					num_factorial.SelfNegative();//变换符号
					num_1 = num_xn / num_factorial;
					sum += num_1;
					if (sum.Exp - num_1.Exp >= (long long)N * 64 || num_1.Equals0())break;
				}
				if (negative)return -sum;
				return sum;
			}
			/**
			@brief 求余弦(弧度制)*/
			static inline FloatN Cos(FloatN const& f) { return f.Cos(); }
			/**
			@brief 求正切(弧度制)*/
			FloatN Tan()const
			{
				FloatN x = Mod(CalculatePi());
				if (x.AppEquals(_pi_half()))return x.MakeError(MathError::Tan);
				bool flag = x > _pi_half();
				if (flag) { x = CalculatePi() - x; return -x.Sin() / x.Cos(); }
				return x.Sin() / x.Cos();
			}
			/**
			@brief 求正切(弧度制)*/
			static inline FloatN Tan(FloatN const& f) { return f.Tan(); }
			/**
			@brief 求余切(弧度制)*/
			FloatN Cot()const
			{
				FloatN x = Mod(CalculatePi());
				FloatN y = x.Sin();
				if (y.Equals0())return x.MakeError(MathError::Cot);
				return x.Cos() / y;
			}
			/**
			@brief 求余切(弧度制)*/
			static inline FloatN Cot(FloatN const& f) { return f.Cot(); }
			/**
			@brief 求反正弦(弧度制*/
			FloatN Arcsin()const
			{
				if (IsError())return *this;
				FloatN num_x, num_xn;//x、xn
				FloatN num1, num2;//分子、分母
				FloatN num_1, num_2;//前一项、后一项
				num_x = Abs();//取绝对值
				if (Abs() > One())
					return num_x.MakeError(MathError::Arcsin);//出错
				if (num_x > _sqrt2_half())//大于(根号2)/2
				{//为了加速收敛，转换成π/2－。。。
					if (Sign < 0)//(π/2)-arcsin(sqrt(1-x^2))
						return(-_pi_half() + (One() - num_x*num_x).SelfAbs().Sqrt().Arcsin());
					return(_pi_half() - ((One() - num_x*num_x).SelfAbs().Sqrt().Arcsin()));
				}
				num1 = num2 = One();
				num_xn = num_2 = num_1 = num_x;
				num_x *= num_x;
				int i;
				for (i = 2;; i++)//arcsin x = x + 1/2*x^3/3 + 1*3/(2*4)*x^5/5 + ……(|x|<1)
				{
					if (i & 1)num1 = num1*(i - 2);//奇数
					else
					{
						num2 = num2*i;//偶数
						continue;
					}
					num_xn = num_xn*num_x;
					num_1 += (num_xn*num1 / num2) / i;
					if (num_1.Exp == num_2.Exp && ((num_1.Data.arr[0] >> 2) == (num_2.Data.arr[0] >> 2)))break;
					num_2 = num_1;
				}
				if (Sign < 0)return -num_1;
				return num_1;
			}
			/**
			@brief 求反正弦(弧度制*/
			static inline FloatN Arcsin(FloatN const& f) { return f.Arcsin(); }
			/**
			@brief 求反余弦(弧度制)*/
			FloatN Arccos()const
			{
				if (IsError())return *this;
				if (Exp > 0 || (Exp == 0) && Data > One().Data)
				{
					Float256 num = *this;
					return num.MakeError(MathError::Arccos);//出错
				}
				if (*this < _sqrt2_half())return _pi_half() - Arcsin();//小于根号2/2，这样计算可以在接近1时获得较高的精度
				return (One() - *this**this).SelfAbs().Sqrt().Arcsin();//取正值
			}
			/**
			@brief 求反余弦(弧度制)*/
			static inline FloatN Arccos(FloatN const& f) { return f.Arccos(); }
			/**
			@brief 求反正切(弧度制)*/
			FloatN Arctan()const
			{
				if (IsError())return *this;
				//配方法：tanx=sinx/cosx，设cosx>0，则sinx = this/sqrt(this平方+1)，转为求arcsin,但此方法在x趋于0时精度大减
				if (Exp > _sqrt2_half().Exp || (Exp == _sqrt2_half().Exp&&Data >= _sqrt2_half().Data))
					return (*this / ((++(*this**this)).SelfAbs().Sqrt())).Arcsin();
				//级数法：arctan x = x - x^3/3 + x^5/5 - ... (x≤1) ，x趋于1时收敛太慢
				FloatN num_x, num_xn;//x、xn
				FloatN num = Three();//分母
				FloatN num_1;//项
				num_x = -*this**this;
				num_xn = (*this)*num_x;
				num_1 = *this;
				int i;
				for (i = 3;; i += 2)
				{
					num_1 += num_xn / i;
					if (num_1.Exp - num_xn.Exp >= (long long)(N * 64))break;
					num_xn *= num_x;
				}
				return num_1;
			}
			/**
			@brief 求反正切(弧度制)*/
			static inline FloatN Arctan(FloatN const& f) { return f.Arctan(); }
			/**
			@brief 求正弦(角度制)*/
			FloatN Dsin()const { return (*this*_pi_180()).Sin(); }
			/**
			@brief 求正弦(角度制)*/
			static inline FloatN Dsin(FloatN const& f) { return f.Dsin(); }
			/**
			@brief 求余弦(角度制)*/
			FloatN Dcos()const { return (*this*_pi_180()).Cos(); }
			/**
			@brief 求余弦(角度制)*/
			static inline FloatN Dcos(FloatN const& f) { return f.Dcos(); }
			/**
			@brief 求正切(角度制)*/
			FloatN Dtan()const { return (*this*_pi_180()).Tan(); }
			/**
			@brief 求余弦(角度制)*/
			static inline FloatN Dtan(FloatN const& f) { return f.Dtan(); }
			/**
			@brief 求余切(角度制)*/
			FloatN Dcot()const { return (*this*_pi_180()).Cot(); }
			/**
			@brief 求余切(角度制)*/
			static inline FloatN Dcot(FloatN const& f) { return f.Dcot(); }
			/**
			@brief 求反正弦(角度制)*/
			FloatN Darcsin()const { return Arcsin() / _pi_180(); }
			/**
			@brief 求反正弦(角度制)*/
			static inline FloatN Darcsin(FloatN const& f) { return f.Darcsin(); }
			/**
			@brief 求反余弦(角度制)*/
			FloatN Darccos()const { return Arccos() / _pi_180(); }
			/**
			@brief 求反余弦(角度制)*/
			static inline FloatN Darccos(FloatN const& f) { return f.Darccos(); }
			/**
			@brief 求反正切(角度制)*/
			FloatN Darctan()const { return Arctan() / _pi_180(); }
			/**
			@brief 求反正切(角度制)*/
			static inline FloatN Darctan(FloatN const& f) { return f.Darctan(); }
			/**
			@brief 比较该数与number 是否相等*/
			bool Equals(const FloatN& number)const { return *this == number; }
			/**
			@brief 比较该数是否约等于number
			@note 比较规则：1.符号不同认为不等，2.忽略位数末count（默认为2）位数据*/
			bool AppEquals(const FloatN& number, index_t count = 2)const
			{
				if (Sign != number.Sign)return false;
				if (Equals0())return true;
				index_t dif;
				if (Exp > number.Exp)
				{
					if (Exp > number.Exp + 1)return false;
					dif = Bits::DifferenceIndexHigh(Data.arr, 0, N*sizeof(unsigned long long) * 8, number.Data.arr, 0, N*sizeof(unsigned long long) * 8, -1);
				}
				else if (number.Exp == Exp)dif = Bits::DifferenceIndexHigh(Data.arr, 0, N*sizeof(unsigned long long) * 8, number.Data.arr, 0, N*sizeof(unsigned long long) * 8, 0);
				else
				{
					if (number.Exp > Exp + 1)return false;
					dif = Bits::DifferenceIndexHigh(number.Data.arr, 0, N*sizeof(unsigned long long) * 8, Data.arr, 0, N*sizeof(unsigned long long) * 8, -1);
				}

				return dif == 0 || dif > (index_t)(N*sizeof(unsigned long long) * 8 - count) || dif < -(index_t)(N*sizeof(unsigned long long) * 8 - count);
			}
			/**
			@brief 生成一个在[0,1)范围内平均分布的随机数
			@return 返回该数字本身
			@note 虽然随机数范围是[0,1)，但实际上取到0的概率非常小*/
			FloatN& Rand()
			{
				if (IsError())return *this;
				static Random x[N];

				Exp = -1;
				Sign = 1;
				for (size_t i = 0; i < N; ++i)
					Data.arr[i] = ((Data.arr[i] << 11) | (Data.arr[i] >> 53)) + Data.arr[(i + 1) % N] + x[i].Rand();
				long long bitsCount = Data.BitsZeroCount();
				if (bitsCount == N * 64ull)
					return *this = Zero();
				Data <<= bitsCount;
				Exp -= bitsCount;
				return *this;
			}
			/**
			@brief 获取整数部分*/
			FloatN GetIntegerPart()const
			{
				if (IsError() || Exp >= (long long)N * 64ll - 1)return *this;
				if (Exp < 0)
					return Zero();
				FloatN num = *this;
				num.Data.Remove(Exp + 1);
				return num;
			}
			/**
			@brief 自身取整(小数部分全舍)*/
			FloatN& SelfGetIntegerPart()
			{
				if (IsError() || Exp >= N * 64 - 1)return *this;
				if (Exp < 0)
					return *this = Zero();
				Data.Remove(Exp + 1);
				return *this;
			}
			/**
			@brief 获取小数部分*/
			FloatN GetDecimalPart()const
			{
				if (IsError() || Exp < 0)return *this;
				if (Exp >= N * 64ull - 1)
					return Zero();
				FloatN num = *this;
				num.Data <<= 1 + Exp;
				long long mov = num.Data.BitsZeroCount();
				if (N * 64ull == mov)
					return Zero();
				num.Data <<= mov;
				num.Exp = -1 - mov;
				return num;
			}
			/**
			@brief 自身取小数部分*/
			FloatN& SelfGetDecimalPart()
			{
				if (IsError() || Exp < 0)return *this;
				if (Exp >= N * 64ll - 1)
					return *this = Zero();
				Data <<= 1 + Exp;
				long long mov = Data.BitsZeroCount();
				if (N * 64ll == mov)
					return *this = Zero();
				Data <<= mov;
				Exp = -1 - mov;
				return *this;
			}
			/**
			@brief 常量0*/
			const static FloatN& Zero();
			/**
			@brief 常量1*/
			const static FloatN& One();
			/**
			@brief 常量2*/
			const static FloatN& Two();
			/**
			@brief 常量3*/
			const static FloatN& Three();
			/**
			@brief 常量4*/
			const static FloatN& Four();
			/**
			@brief 常量5*/
			const static FloatN& Five();
			/**
			@brief 常量6*/
			const static FloatN& Six();
			/**
			@brief 常量7*/
			const static FloatN& Seven();
			/**
			@brief 常量8*/
			const static FloatN& Eight();
			/**
			@brief 常量9*/
			const static FloatN& Nine();
			/**
			@brief 常量10*/
			const static FloatN& Ten();
			/**
			@brief 最大值*/
			const static FloatN& MaxValue();
			/**
			@brief 最小值*/
			const static FloatN& MinValue();
			/**
			@brief 自然数e*/
			const static FloatN& E();
			/**
			@brief 圆周率π*/
			const static FloatN& Pi();
			/**
			@brief 计算Ln2*/
			static FloatN& CalculateLn2()
			{
				static FloatN ln2;
				if (ln2.Equals0())
				{
					FloatN y = 1 / FloatN(3);
					ln2 = y;
					FloatN y2 = y*y;
					FloatN yn = y2*y;
					for (unsigned long long i = 3;; i += 2)
					{
						ln2 += yn / i;
						if (yn.Exp < (long long)N*-64ll)break;
						yn *= y2;
					}
					ln2.Exp++;
				}
				return ln2;
			}
			/**
			@brief 计算1/ln2*/
			static FloatN& Calculate1Ln2()
			{
				static FloatN _ln2 = 1 / CalculateLn2();
				return _ln2;
			}
			/**
			@brief 计算1/Ln10*/
			static FloatN& Calculate1Ln10()
			{
				static FloatN ln10;
				if (ln10.Equals0())
				{
					FloatN y = 0.25 / FloatN(2.25);
					ln10 = y;
					FloatN y2 = y*y;
					FloatN yn = y2*y;
					for (unsigned long long i = 3;; i += 2)
					{
						ln10 += yn / i;
						if (yn.Exp < (long long)N*-64ll)break;
						yn *= y2;
					}
					ln10.Exp++;
					ln10 += CalculateLn2() * 3;
					ln10 = 1 / ln10;
				}
				return ln10;
			}
			/**
			@brief 2开根号x次 : 2^(1/(2^x))，x为0~15*/
			static FloatN* Cal2sqrt2();
			/**
			@brief 2开根号x次的倒数 : 1/(2^(1/(2^x))，x为0~15)*/
			static FloatN* Cal12sqrt2();
			/**
			@brief ln(2^(1/(2^x)))=(ln2)/2^x*/
			static FloatN* Calln2sqrt2();
			/**
			@brief 计算π*/
			static FloatN& CalculatePi()
			{
				static FloatN f;
				if (f.Equals0())
				{
					FloatN a(1), b(1), t(1);
					long long p = 0;
					b = Sqrt2().Half();
					t.Exp = -2;
					for (;;)
					{
						f = (a + b);
						f.Exp--;
						b = Sqrt(a*b);
						if ((f.Data.arr[0] >> 2) == (b.Data.arr[0] >> 2) && f.AppEquals(b))break;
						a -= f;
						a *= a;
						a.Exp += p;
						t = t - a;
						p++;
						a = f;
					}
					f += b;
					f *= f;
					f /= t;
					f.Exp -= 2;

					/*
					f = 48 * (FloatN(1)/ FloatN(49)).Arctan();
					f += 128 * (FloatN(1) / FloatN(57)).Arctan();
					f -= 20 * (FloatN(1) / FloatN(239)).Arctan();
					f += 48 * (FloatN(1) / FloatN(110443)).Arctan();*/
				}
				return f;
			}
			/**
			@brief 计算π平方*/
			static FloatN& CalculatePi2()
			{
				static FloatN f;
				if (f.Equals0())	f = CalculatePi()*CalculatePi();
				return f;
			}
			/**
			@brief 计算e*/
			static FloatN& CalculateE()
			{
				static FloatN f;
				if (f.Equals0())
				{
					FloatN xn = 0.5;
					f = 2.5;
					for (size_t i = 3;; ++i)
					{
						xn /= i;
						f += xn;
						if (f.Exp - xn.Exp > (long long)N * 64)break;
					}
				}
				return f;
			}
			/**
			@brief 根号2*/
			const static FloatN& Sqrt2()
			{
				static FloatN f;
				if (f.Equals0())	f = FloatN(2).Sqrt();
				return f;
			}
		private:
			/**
			@brief 二分之π*/
			const static FloatN& _pi_half()
			{
				static FloatN f;
				if (f.Equals0())	f = CalculatePi().Half();
				return f;
			}
			/**
			@brief 四分之π*/
			const static FloatN& _pi_half2()
			{
				static FloatN f;
				if (f.Equals0())	f = _pi_half().Half();
				return f;
			}
			/**
			@brief 二分之根号2*/
			const static FloatN& _sqrt2_half()
			{
				static FloatN f;
				if (f.Equals0())	f = Sqrt2().Half();
				return f;
			}
			/**
			@brief 一百八十分之π*/
			const static FloatN& _pi_180()
			{
				static FloatN f;
				if (f.Equals0())	f = CalculatePi() / 180;
				return f;
			}

		};

		template<size_t N>bool FloatN<N>::ThrowException = false;
		template<size_t N>const FloatN<N>& FloatN<N>::Zero() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(0);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::One() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(1);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Two() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(2);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Three() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(3);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Four() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(4);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Five() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(5);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Six() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(6);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Seven() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(7);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Eight() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(8);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Nine() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(9);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Ten() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(10);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N> FloatN<N>* FloatN<N>::Cal2sqrt2()
		{
			static FloatN<N> f[16];
			if (f[0].Equals0()) {
				f[0] = 2.0;
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Sqrt();
			}
			return f;
		}
		template<size_t N> FloatN<N>* FloatN<N>::Cal12sqrt2()
		{
			static FloatN<N> f[16];
			if (f->Equals0())
			{
				f[0] = 2.0;
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Sqrt();
				for (int i = 0; i < 16; ++i)f[i] = 1 / f[i];
			}
			return f;
		}
		template<size_t N> FloatN<N>*FloatN<N>::Calln2sqrt2()
		{
			static FloatN<N> f[32];
			if (f->Equals0())
			{
				f[0] = CalculateLn2();
				for (int i = 1; i < 32; ++i)f[i] = f[i - 1].Half();
			}
			return f;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::MaxValue() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(1, MaxValues::Int64, FloatN(1).Data);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::MinValue() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN(-1, MaxValues::Int64, FloatN(1).Data);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::E() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = FloatN::CalculateE();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		template<size_t N>const FloatN<N>& FloatN<N>::Pi() {
			static FloatN* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static FloatN val = CalculatePi();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
	}
	template<size_t T>struct Conversion::Convert <std::string, Math::FloatN<T> >
	{
		static inline std::string From(Math::FloatN<T> const&l) { return l.ToString(); }
	};
}
#undef HAS_SIGNED_TYPES_EXCEPT64
#undef HAS_UNSIGNED_TYPES_EXCEPT64
#undef HAS_ALL_TYPES
