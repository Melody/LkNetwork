#include "stdafx.h"
#ifdef LK_MATH_FLOAT128_
namespace LK
{
	namespace Math
	{
		Float128& Float128::MakeError(char errorcode) {
			Sign = errorcode;
			if (ThrowException)Throw<MathException >((-errorcode >= 24) ? "未知错误" : (errorcode>0 ? "未知异常" : MathError::description[-errorcode]));
			return *this;
		}
		Float128::Float128(double number)//计算100万次Float(1.1)用时0.018秒
		{//验证通过
			union {
				double dn;
				u64 num;
			};
			dn = number;
			if (number != 0)
			{
				Exp = ((0x7ff0000000000000 & num) >> 52) - 1023;
				Data.ull[1] = (num << 11) | 0x8000000000000000;
				if (number > 0)
					Sign = 1;
				else
					Sign = -1;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(float number)
		{
			union {
				u32 num;
				float dn;
			};
			dn = number;
			if (num != 0)
			{
				Exp = (s64)((0x7f800000 & num) >> 23) - 127;
				Data.ull[1] = ((u64)num << 41) | 0x8000000000000000;
				if (number > 0)
					Sign = 1;
				else
					Sign = -1;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0]= 0;
		}
		Float128::Float128(s64 number)
		{
			if (number != 0)
			{
				if (number > 0)
					Sign = 1;
				else
				{
					Sign = -1;
					number = -number;
				}
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 ^ count;
				Data.ull[1] = number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(u64 number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 - count;
				Data.ull[1] = number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(s32 number)
		{
			if (number != 0)
			{
				if (number > 0)
					Sign = 1;
				else
				{
					Sign = -1;
					number = -number;
				}
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 ^ count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(u32 number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 - count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(int number)
		{
			if (number != 0)
			{
				if (number > 0)
					Sign = 1;
				else
				{
					Sign = -1;
					number = -number;
				}
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 ^ count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(ui32 number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 - count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(s16 number)
		{
			if (number != 0)
			{
				if (number > 0)
					Sign = 1;
				else
				{
					Sign = -1;
					number = -number;
				}
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 ^ count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(u16 number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 - count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(s8 number)
		{
			if (number != 0)
			{
				if (number > 0)
					Sign = 1;
				else
				{
					Sign = -1;
					number = -number;
				}
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 ^ count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(u8 number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 - count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(char number)
		{
			if (number != 0)
			{
				if (number > 0)
					Sign = 1;
				else
				{
					Sign = -1;
					number = -number;
				}
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 ^ count;
				Data.ull[1] = (u64)number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[1] = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(bool number)
			:Exp(0)
		{
			if (number)
			{
				Data.ull[1] = MinValues::Int64;
				Sign = 1;
			}
			else
			{
				Data.ull[1] = 0;
				Sign = 0;
			}
			Data.ull[0] = 0;
		}
		Float128::Float128(char const* pString)
		{
			Sign = 0;
			Exp = 0;
			s64 i = 0, j = 0;//循环用数
			s64 num_exp = 0;//指数部分
			s64 position_point = 0;//小数点位置
			s64 position_e = 0;//e的位置
			Data.Clear();//清空内容
			char* numstr = 0;
			long long len = 0;
			index_t Length = 0;
			u8* arr=NULL;
			size_t mask=0;
			if (pString == NULL)goto End;
			Length = Strings::strlen(pString);
			for (len = 0; len < Length; ++len)
			{
				//如果首位是逗号或者遇到连续两个符号将退出循环
				if (len && (pString[len] == ',' || pString[len] == '.'))//点号和逗号前不能出现点号或逗号
				{
					if (pString[len - 1] == ',' || pString[len - 1] == '.')
						break;
					continue;
				}
				if (len && (pString[len] & 0xffdf) == 'E' && (pString[len - 1] & 0xffdf) == 'E')//两个e
					break;
				if (len && (pString[len] == '+' || pString[len] == '-'))//正负号只能出现在开头或e后面
				{
					if ((pString[len - 1] & 0xffdf) != 'E')
						break;
					continue;
				}
				if (pString[len] <= '9' && pString[len] >= '0')//数字字符
					continue;
				if ((pString[len] & 0xffdf) >= 'A' && (pString[len] & 0xffdf) <= 'Z')//字母
					continue;
				if (pString[len] == '.' || pString[len] == '+' || pString[len] == '-' || pString[len] == '_')//其他符号
					continue;
				break;//不支持符号，退出
			}
			if (!len)goto End;//空内容直接为0
            arr = new u8[(size_t)(len + 1)];
			numstr = (char*)(u8*)arr;
			memcpy(numstr, pString, size_t(len * 2));
			numstr[len] = 0;
			//通过首字初步判断是否属于特殊转换
			if (*numstr == '_' || (((*numstr) & 0xffdf) >= 'A' && ((*numstr) & 0xffdf) <= 'Z'))//以字母开头将进入特殊转换
			{
				if (Strings::strcmp(numstr, "min") == 0 || Strings::strcmp(numstr, "max") == 0)
				{
					Exp = ExpMaxValue;
					if (Strings::strcmp(numstr, "min") == 0)Sign = -1; else Sign = 1;
					Data.ull[1] |= MinValues::Int64;
					goto End;
				}
				if (Strings::strcmp(numstr, "ln2") == 0)
				{
					Exp = -1;	Sign = 1; Data.ull[0] = 0xc9e3b39803f2f6af; Data.ull[1] = 0xb17217f7d1cf79ab;
					goto End;
				}
				if (0 == Strings::strcmp(numstr, "ln10"))
				{
					Exp = 1;	Sign = 1; Data.ull[0] = 0xea56d62b82d30a29; Data.ull[1] = 0x935d8dddaaa8ac16;
					goto End;
				}
				//e = "2.7182818284590452353602874713526624977572470936999595749669676277240766303536";
				if (0 == Strings::strcmp(numstr, "e"))
				{
					Exp = 1; Sign = 1; Data.ull[0] = 0xafdc5620273d3cf2; Data.ull[1] = 0xadf85458a2bb4a9a;
					goto End;
				}
				//pi = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862";
				if (0 == Strings::strcmp(numstr, "pi"))
				{
					Exp = 1; Sign = 1;	Data.ull[0] = 0xc4c6628b80dc1cd1; Data.ull[1] = 0xc90fdaa22168c234;
					goto End;
				}
				//未能匹配特殊字符串，将进入常规字符串生成：
			}

			num_exp = 0;//指数部分
			//exp10 = 0;//e后面指数部分
			position_point = len;//小数点位置
			position_e = len;//e的位置
							 //1.寻找结束位置+移除逗号+移除小数点+寻找e的位置
			for (i = j = 0; i < len; ++i)
			{
				if (numstr[i] == ',')
					continue;
				if (numstr[i] == '.')//小数点位置
				{
					position_point = j;
					continue;
				}
				if (i != j)
					numstr[j] = numstr[i];
				if ((numstr[j] & 0xffdf) == 'E')
					position_e = j;
				++j;
			}
			len = j;
			if (position_e > len)//没找到e
				position_e = len;
			if (position_point > position_e)//小数点在e后面或没找到小数点
				position_point = position_e;
			while (j)//检测e前面是否有数值
			{
				--j;
				if (numstr[j] <= '9' && numstr[j] > '0')break;
			}
			if (numstr[j] > '9' || numstr[j] <= '0')goto End;
			//2.设置exp10mov(移除小数点带来的指数变动)
			//这里不用设置了，因为exp10mov 就是 position_e-position_point;
			//3.整数部分计算,将e前面的字符串的值计算出来，存到*this里面
			for (i = 0; i < position_e; ++i)
			{
				*this += numstr[i] - L'0';
				if (1 + i < position_e)
					SelfMultiply10();//快速乘10
			}
			//4.计算e后面的数值
			for (i = position_e + 1; i < len; ++i)
			{
				//如果遇到非数字字符将视为0
				if (numstr[i] >= '1' && numstr[i] <= '9')
					num_exp += numstr[i] - '0';
				if (1 + i < len)
				{
					if (num_exp > (s64)(MaxValue().Exp*0.03010299956639812))//数值已经溢出
					{
						MakeError(MathError::InputOverflow);
						goto End;
					}
					num_exp *= 10;
				}
			}
			//5.判断正负号
			if ('-' == numstr[0])
				Sign = -1;
			else
				Sign = 1;;
			//6.e后面的指数减去因移除小数点带来的指数变动
			if (position_e < len && '-' == numstr[1 + position_e])
				num_exp = -num_exp - (position_e - position_point);
			else
				num_exp -= position_e - position_point;
			//7.将指数的值乘到this中，此过程后要判断溢出
			if (num_exp > 0)
			{
				s64 texp = 1;
				int tmov = 0;
				while (texp <= num_exp)
				{
					if ((num_exp&texp) != 0)
					{
						*this = *this * _10e2()[tmov];
						num_exp ^= texp;
					}
					++tmov;
					texp <<= 1;
				}
				if (IsError())
				{
					*this = MaxValue();//溢出返回最大值：2^2147483647
					if (numstr[0] == '-')*this = -(*this);
				}
			}
			else if (num_exp < 0)
			{
				num_exp = -num_exp;
				u64 texp = 1;
				int tmov = 0;
				while (texp <= (u64)num_exp)
				{
					if ((num_exp&texp) != 0)
					{
						*this = *this / _10e2()[tmov];
						num_exp ^= texp;
					}
					++tmov;
					texp <<= 1;
				}
			}
			delete[] arr;
		End:;
		}
		Float128::operator double()const
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
				default:return 0.0 / (double)Zero();
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
				num.ull |= (Data.ull[1] & MaxValues::Int64) >> 11;//尾数
				if (Data.ull[1] & 0x400)num.ull++;//类似十进制的四舍五入
			}
			return num.number;
		}
		Float128::operator long long()const
		{
			if (Exp > 62)
			{
				FloatData128 d = Data;
				d <<= Exp - 63;
				if (Sign < 0)
					return -(s64)d.ull[1];
				return d.ull[1];
			}
			if (Exp < 0)
				return 0;
			unsigned long long dat = Data.ull[1];
			dat >>= 63 - Exp;
			if (Sign < 0)
				return -(long long)dat;
			return dat;
		}
		Float128::operator unsigned long long()const
		{
			if (Sign < 0)return (unsigned long long)operator long long();
			if (Exp > 63)
			{
				FloatData128 d = Data;
				d <<= Exp - 63;
				return d.ull[1];
			}
			if (Exp < 0)
				return 0;
			return Data.ull[1] >> (63 - Exp);
		}
		Float128::operator long()const
		{
			return (long)operator long long();
		}
		Float128::operator unsigned long()const
		{
			if (Sign < 0)return (unsigned long)operator long();
			if (Exp > 31)return (unsigned long)operator unsigned long long();
			if (Exp < 0)return 0;
			return u32(Data.ull[1] >> (63 - Exp));
		}
		bool Float128::SelfPlusExp128()
		{
			if (Sign == 0)return false;
			if (Sign > 0)
			{
				if (++Data)
				{
					if (Exp == ExpMaxValue)return true;
					Data.MoveRight();
					Data.ull[1] |= MinValues::Int64;
					++Exp;
				}
				return false;
			}
			--Data;
			if ((s64)Data.ull[1] > 0)
			{
				if (Exp == ExpMinValue)*this = Zero();
				else
				{
					Data.MoveLeft();
					--Exp;
				}
			}
			return false;
		}
		bool Float128::SelfMinusExp128()
		{
			if (Sign == 0)return false;
			if (Sign < 0)
			{
				if (++Data)
				{
					if (Exp == ExpMaxValue)return true;
					Data.MoveRight();
					Data.ull[1] |= MinValues::Int64;
					++Exp;
				}
				return false;
			}
			--Data;
			if ((s64)Data.ull[1]>0)
			{
				if (Exp == ExpMinValue)*this = Zero();
				else
				{
					Data.MoveLeft();
					--Exp;
				}
			}
			return false;
		}
		Float128& Float128::operator++()
		{
			if (IsError())return*this;
			if (Equals0())return *this = One();
			if (Exp >= 128)
			{
				if (Exp == 128) {
					if (Sign > 0) {
						if (++Data) { Data.MoveRight(); Exp++; Data.ull[1] |= MinValues::Int64; }
					}
					else { --Data; if ((s64)Data.ull[1] > 0) { Data.MoveLeft(); Exp--; } }
				}
				return *this;
			}
			if (Exp <= -128)
			{
				if (Exp == -128) {
					if (Sign > 0) {
						Data.ull[1] = MinValues::Int64;
						Data.ull[0] = 1;
						Exp = 0;
					}
					else
					{
						Data.ull[1] = Data.ull[0] = -1;
						Exp = -1;
					}
				}
				return *this;
			}
			if (Sign > 0)
			{
				if (Exp > 0)
				{
					if (Bits::SelfPlusOneRound64(Data.ull, 2, 127 - Exp))
					{
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					u64 flag = Data >>= -Exp;
					if ((s64)flag < 0)++Data;
					if ((s64)Data.ull[1] < 0)
					{
						Data.ull[1] ^= MinValues::Int64;
						Data.MoveRight();
						Exp = 1;
					}
					else Exp = 0;
					Data.ull[1] |= MinValues::Int64;
				}
				return *this;
			}
			if (Exp >= 0)
			{
				Bits::SelfMinusOneRound64(Data.ull, 2, 127 - Exp);
				s64 tmp = Data.BitsZeroCount();
				if (tmp == 128)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				s64 flag = -Exp;
				u64 low = Data >>= flag;
				Data.ull[1] = (1ull << 63) - Data.ull[1];
				if (Data.ull[0] != 0) { Data.ull[0] = -(long long)Data.ull[0]; --Data.ull[1]; }
				flag = Data.BitsZeroCount();
				if (flag == 128)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] <= MaxValues::Int64)
						{
							Data.MoveLeft();
							--Exp;
						}
					}
					else Data.ull[0] -= low;
				}
				Exp = -flag;
				Sign = 1;
			}
			return *this;
		}
		Float128& Float128::operator--()
		{
			if (IsError())return*this;
			if (Equals0())return*this = -1;
			if (Exp >= 128)
			{
				if (Exp == 128) {
					if (Sign < 0) {
						if (++Data) { Data.MoveRight(); Exp++; Data.ull[1] |= MinValues::Int64; }
					}
					else { --Data; if ((s64)Data.ull[1] > 0) { Data.MoveLeft(); Exp--; } }
				}
				return *this;
			}
			if (Exp <= -128)
			{
				if (Exp == -128) {
					if (Sign < 0) {
						Data.ull[1] = MinValues::Int64;
						Data.ull[0] = 1;
						Exp = 0;
					}
					else
					{
						Data.ull[1] = Data.ull[0] = -1;
						Exp = -1;
					}
				}
				return *this;
			}
			if (Sign < 0)
			{
				if (Exp > 0)
				{
					if (Bits::SelfPlusOneRound64(Data.ull, 2, 127 - Exp))
					{
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					u64 flag = Data >>= -Exp;
					if ((s64)flag < 0)++Data;
					if ((s64)Data.ull[1] < 0)
					{
						Data.ull[1] ^= MinValues::Int64;
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						Exp = 1;
					}
					else Exp = 0;
				}
				return *this;
			}
			if (Exp >= 0)
			{
				Bits::SelfMinusOneRound64(Data.ull, 2, 127 - Exp);
				s64 tmp = Data.BitsZeroCount();
				if (tmp == 128)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				s64 flag = -Exp;
				u64 low = Data >>= flag;
				Data.ull[1] = (1ull << 63) - Data.ull[1];
				if (Data.ull[0] != 0) { Data.ull[0] = -(s64)Data.ull[0]; --Data.ull[1]; }
				flag = Data.BitsZeroCount();
				if (flag == 128)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] <= MaxValues::Int64)
						{
							Data.MoveLeft();
							--Exp;
						}
					}
					else Data.ull[0] -= low;
				}
				Exp = -flag;
				Sign = -1;
			}
			return *this;
		}
		Float128& Float128::operator+=(u64 num)
		{
			if (IsError() || num == 0)return*this;
			s64 e = 63 ^ Bits::LeftZeroCount(num);
			if (Exp >= e + 128)
			{
				if (Exp == e + 128) {
					if (Sign > 0) {
						if (++Data) { Data.MoveRight(); Exp++; Data.ull[1] |= MinValues::Int64; }
					}
					else { --Data;	if ((s64)Data.ull[1] > 0) { Data.MoveLeft(); Exp--; } }
				}
				return *this;
			}
			if (Exp <= e - 128)
			{
				Data.ull[1] = num << (63 ^ e);
				Data.ull[0] = 0;
				if (Exp == e - 128) {
					Exp = e;
					if (Sign > 0)
						Data.ull[0] = 1;
					else {
						--Data; if ((s64)Data.ull[1] > 0) {
							Data.MoveLeft();
							Exp--;
						}
					}
				}
				else Exp = e;
				Sign = 1;
				return *this;
			}
			if (Sign > 0)
			{
				if (Exp > e)
				{
					if (Bits::SelfPlusIntRound64(Data.ull, 2, num, 127 - Exp))
					{
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					u64 flag = Data >>= e - Exp;
					if ((s64)flag < 0)++Data;
					if (Bits::SelfPlusIntRound64(Data.ull, 2, num, 127 - e))
					{
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						Exp = e + 1;;
					}
					else Exp = e;
				}
				return *this;
			}
			if (Exp > e || (Exp == e && Data.ull[1] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(Data.ull, 2, num, 127 - Exp);
				s64 tmp = Data.BitsZeroCount();
				if (tmp == 128)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				s64 flag = e - Exp;
				u64 low = Data >>= flag;
				Data.ull[1] = (num << (63 ^ e)) - Data.ull[1];
				if (Data.ull[0] != 0) { Data.ull[0] = -(s64)Data.ull[0]; --Data.ull[1]; }
				flag = Data.BitsZeroCount();
				if (flag == 128)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if ((s64)--Data.ull[1] >0)
						{
							Data.MoveLeft();
							--Exp;
						}
					}
					else Data.ull[0] -= low;
				}
				Exp = e - flag;
				Sign = 1;
			}
			return *this;
		}
		Float128 Float128::operator+(Float128 number)const
		{
			Float128 t = *this;
			if (t.Equals0() || number.IsError())return number;//进行错误传递
			if (number.Equals0() || t.IsError())return t;//进行错误传递
			u64 low = 0;
			if (number.Exp != t.Exp) {//指数位不同
				if (number.Exp < t.Exp)BigObject::Swap(t, number);//使得number的绝对值总是大于t
				if (number.Exp - t.Exp > 127) {
					if (number.Exp - t.Exp == 128) {
						if (number.Sign == t.Sign) {
							if (++number.Data) {
								if (number.Exp == Float128::ExpMaxValue)
									return number.MakeError(MathError::PlusExpOverflow);
								++number.Exp;
								number.Data.ull[1] = 0x8000000000000000;
							}
						}
						else {
							--number.Data;
							if (number.Data.ull[1] < 0x8000000000000000) {
								if (number.Exp == Float128::ExpMinValue)
									return Float128::Zero();
								--number.Exp;
								number.Data.MoveLeft();
							}
						}
					}
					return number;
				}
				low = t.Data >>= number.Exp - t.Exp;
			}

			if (number.Sign == t.Sign) {//符号位相同
				if ((number.Data += t.Data) || ((s64)low < 0 && ++number.Data)) {
					if (number.Exp == Float128::ExpMaxValue)
						return number.MakeError(MathError::PlusExpOverflow);
					number.Exp++;
					bool t = number.Data.ull[0] & 1;
					number.Data.MoveRight();
					if (t&&++number.Data&&number.Exp++ == Float128::ExpMaxValue)return number.MakeError(MathError::PlusExpOverflow);
					number.Data.ull[1] |= 0x8000000000000000;
				}
				return number;
			}
			if (number.Data < t.Data)BigObject::Swap(number, t);
			number.Data -= t.Data;
			s64 mov = number.Data.BitsZeroCount();
			if (mov == 128 || (number.Exp - mov) < Float128::ExpMinValue)return Float128::Zero();
			if (mov)
			{
				number.Data <<= mov;
				Bits::SelfMinusInt64(number.Data.ull, 2, low, mov - 64);
				if ((s64)number.Data.ull[1] > 0)
				{
					++mov;
					if (mov == 128 || (number.Exp - mov) < Float128::ExpMinValue)return Float128::Zero();
					number.Data.MoveLeft();
				}
				number.Exp -= mov;
			}
			return number;
		}
		Float128 Float128::operator+(u64 num)const
		{
			if (IsError() || num == 0)return*this;
			s64 e = 63 ^ Bits::LeftZeroCount(num);
			Float128 result(*this);
			if (result.Exp >= e + 128)
			{
				if (result.Exp == e + 128) {
					if (Sign > 0) { if (++result.Data) { result.Data.MoveRight(); result.Exp++; result.Data.ull[1] |= MinValues::Int64; } }
					else { --result.Data;	if ((s64)Data.ull[1] > 0) { result.Data.MoveLeft(); result.Exp--; } }
				}
				return result;
			}
			if (result.Exp <= e - 128)
			{
				result.Data.ull[1] = num << (63 ^ e);
				result.Data.ull[0] = 0;
				if (result.Exp == e - 128) {
					result.Exp = e;
					if (Sign > 0)
						result.Data.ull[0] = 1;
					else {
						--result.Data; if ((s64)result.Data.ull[1] > 0) {
							result.Data.MoveLeft();
							result.Exp--;
						}
					}
				}
				else result.Exp = e;
				result.Sign = 1;
				return result;
			}
			if (result.Sign > 0)
			{
				if (result.Exp > e)
				{
					if (Bits::SelfPlusIntRound64(result.Data.ull, 2, num, 127 - result.Exp))
					{
						result.Data.MoveRight();
						result.Data.ull[1] |= MinValues::Int64;
						++result.Exp;
					}
				}
				else
				{
					u64 flag = result.Data >>= e - Exp;
					if ((s64)flag < 0)++result.Data;
					if (Bits::SelfPlusIntRound64(result.Data.ull, 2, num, 127 - e))
					{
						result.Data.MoveRight();
						result.Data.ull[1] |= MinValues::Int64;
						result.Exp = e + 1;;
					}
					else result.Exp = e;
				}
				return result;
			}
			if (result.Exp > e || (result.Exp == e && result.Data.ull[1] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(result.Data.ull, 2, num, 127 - Exp);
				s64 tmp = result.Data.BitsZeroCount();
				if (tmp == 128)return Zero();
				result.Exp -= tmp;
				result.Data <<= tmp;
			}
			else
			{
				s64 flag = e - result.Exp;
				u64 low = result.Data >>= flag;
				result.Data.ull[1] = (num << (63 ^ e)) - result.Data.ull[1];
				if (result.Data.ull[0] != 0) { result.Data.ull[0] = -(long long)result.Data.ull[0]; flag = true; --result.Data.ull[1]; }
				flag = result.Data.BitsZeroCount();
				if (flag == 128)return Zero();
				if (flag)
				{
					result.Data <<= flag;
					low >>= 64 - flag;
					if (result.Data.ull[0] < low)
					{
						result.Data.ull[0] -= low;
						if ((s64)--result.Data.ull[1] >0)
						{
							result.Data.MoveLeft();
							--result.Exp;
						}
					}
					else result.Data.ull[0] -= low;
				}
				result.Exp = e - flag;
				result.Sign = 1;
			}
			return result;
		}
		Float128& Float128::operator-=(u64 num)
		{
			if (IsError() || num == 0)return*this;
			s64 e = 63 ^ Bits::LeftZeroCount(num);
			if (Exp >= e + 128)
			{
				if (Exp == e + 128) {
					if (Sign < 0) {
						if (++Data) { Data.MoveRight(); Exp++; Data.ull[1] |= MinValues::Int64; }
					}
					else { --Data; if ((s64)Data.ull[1] > 0) { Data.MoveLeft(); Exp--; } }
				}
				return *this;
			}
			if (Exp <= e - 128)
			{
				Data.ull[1] = num << (63 ^ e);
				Data.ull[0] = 0;
				if (Exp == e - 128) {
					Exp = e;
					if (Sign < 0)
						Data.ull[0] = 1;
					else
					{
						--Data;
						if ((s64)Data.ull[1]>0) {
							Data.MoveLeft();
							Exp--;
						}
					}
				}
				else Exp = e;
				Sign = 1;
				return *this;
			}
			if (Sign < 0)
			{
				if (Exp > e)
				{
					if (Bits::SelfPlusIntRound64(Data.ull, 2, num, 127 - Exp))
					{
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					u64 flag = Data >>= e - Exp;
					if ((s64)flag < 0)++Data;
					if (Bits::SelfPlusIntRound64(Data.ull, 2, num, 127 - e))
					{
						Data.MoveRight();
						Data.ull[1] |= MinValues::Int64;
						Exp = e + 1;
					}
					else Exp = e;
				}
				return *this;
			}
			if (Exp > e || (Exp == e && Data.ull[1] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(Data.ull, 2, num, 127 - Exp);
				s64 tmp = Data.BitsZeroCount();
				if (tmp == 128)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				s64 flag = e - Exp;
				u64 low = Data >>= flag;
				Data.ull[1] = (num << (63 ^ e)) - Data.ull[1];
				if (Data.ull[0] != 0) { Data.ull[0] = -(s64)Data.ull[0]; --Data.ull[1]; }
				flag = Data.BitsZeroCount();
				if (flag == 128)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] <= MaxValues::Int64)
						{
							Data.MoveLeft();
							--Exp;
						}
					}
					else Data.ull[0] -= low;
				}
				Exp = e - flag;
				Sign = -1;
			}
			return *this;
		}
		Float128 Float128::operator-(Float128 const& num2)const
		{
			if (IsError())return *this;
			if (num2.IsError())return num2;
			Float128 tmp(*this + (-num2));
			if (tmp.IsError())return tmp.MakeError(MathError::MinusExpOverflow);
			return tmp;
		}
		Float128 Float128::operator-(u64 num)const
		{
			if (IsError() || num == 0)return*this;
			s64 e = 63 ^ Bits::LeftZeroCount(num);
			Float128 result(*this);
			if (result.Exp >= e + 128)
			{
				if (result.Exp == e + 128) {
					if (result.Sign < 0) {
						if (++result.Data) { result.Data.MoveRight(); result.Exp++; result.Data.ull[1] |= MinValues::Int64; }
					}
					else { --result.Data; if ((s64)result.Data.ull[1] > 0) { result.Data.MoveLeft(); result.Exp--; } }
				}
				return result;
			}
			if (result.Exp <= e - 128)
			{
				result.Data.ull[1] = num << (63 ^ e);
				result.Data.ull[0] = 0;
				if (result.Exp == e - 128) {
					result.Exp = e;
					if (result.Sign < 0)
						result.Data.ull[0] = 1;
					else
					{
						--result.Data;
						if ((s64)result.Data.ull[1]>0) {
							result.Data.MoveLeft();
							result.Exp--;
						}
					}
				}
				else result.Exp = e;
				result.Sign = 1;
				return result;
			}
			if (result.Sign < 0)
			{
				if (result.Exp > e)
				{
					if (Bits::SelfPlusIntRound64(result.Data.ull, 2, num, 127 - result.Exp))
					{
						result.Data.MoveRight();
						result.Data.ull[1] |= MinValues::Int64;
						++result.Exp;
					}
				}
				else
				{
					u64 flag = result.Data >>= e - result.Exp;
					if ((s64)flag < 0)++result.Data;
					if (Bits::SelfPlusIntRound64(result.Data.ull, 2, num, 127 - e))
					{
						result.Data.MoveRight();
						result.Data.ull[1] |= MinValues::Int64;
						result.Exp = e + 1;;
					}
					else result.Exp = e;
				}
				return result;
			}
			if (result.Exp > e || (result.Exp == e && result.Data.ull[1] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(result.Data.ull, 2, num, 127 - result.Exp);
				s64 tmp = result.Data.BitsZeroCount();
				if (tmp == 128)return Zero();
				result.Exp -= tmp;
				result.Data <<= tmp;
			}
			else
			{
				s64 flag = e - result.Exp;
				u64 low = result.Data >>= flag;
				result.Data.ull[1] = (num << (63 ^ e)) - result.Data.ull[1];
				if (result.Data.ull[0] != 0) { result.Data.ull[0] = -(long long)result.Data.ull[0]; --result.Data.ull[1]; }
				flag = result.Data.BitsZeroCount();
				if (flag == 128)return Zero();
				if (flag)
				{
					result.Data <<= flag;
					low >>= 64 - flag;
					if (result.Data.ull[0] < low)
					{
						result.Data.ull[0] -= low;
						if (--result.Data.ull[1] <= MaxValues::Int64)
						{
							result.Data.MoveLeft();
							--result.Exp;
						}
					}
					else result.Data.ull[0] -= low;
				}
				result.Exp = e - flag;
				result.Sign = -1;
			}
			return result;
		}
		Float128& Float128::SelfMultiply10()
		{
			if (IsError() || Equals0())return *this;
			if (Exp + 3 <= ExpMaxValue)
				Exp += 3;//乘8
			else
				return MakeError(MathError::MultiplyExpOverflow);
			FloatData128 fd = Data;
			fd >>= 2;
			if (Data.ull[0] & 0x2)++fd;
			if (Data += fd)
			{
				if (Exp == ExpMaxValue)
					return MakeError(MathError::MinusExpOverflow);
				Exp++;
				Data.MoveRight();
				Data.ull[1] |= MinValues::Int64;
			}
			return *this;
		}
		Float128 Float128::Multiply10()const
		{
			if (IsError() || Equals0())return *this;
			s64 exp;
			if (Exp + 3 <= ExpMaxValue)
				exp = Exp + 3;//乘8
			else
				return Float128().MakeError(MathError::MultiplyExpOverflow);
			FloatData128 fd = Data;
			FloatData128 fd2 = Data;
			fd >>= 2;
			if (Data.ull[0] & 0x2)++fd;
			if (fd2 += fd)
			{
				if (Exp == ExpMaxValue)
					return Float128().MakeError(MathError::MinusExpOverflow);
				exp++;
				fd2.MoveRight();
				fd2.ull[1] |= MinValues::Int64;
			}
			return Float128(Sign, (s32)exp, fd2);
		}
		Float128 Float128::operator*(Float128 const& number)const
		{
			if (IsError())return *this;
			if (number.IsError())return number;
			if (Equals0() || number.Equals0())return Float128::Zero();
			u64 high[3], low[4];
			s64 exp = Exp + number.Exp;
			low[2] = Bits::Multiply(Data.ull[1], number.Data.ull[1], high + 2);
			low[1] = Bits::Multiply(Data.ull[1], number.Data.ull[0], high + 1);
			low[0] = Bits::Multiply(Data.ull[0], number.Data.ull[1], high);
			low[3] = (Data.ull[0] >> 32)*(number.Data.ull[0] >> 32);
			//叠加结果是high[2] , low[2], low[0]
			if (high[1] > ~low[2])high[2]++;
			low[2] += high[1];
			if (high[0] > ~low[2])high[2]++;
			low[2] += high[0];
			if (low[0] > ~low[1])if (++low[2] == 0)++high[2];
			low[0] += low[1];
			if (low[0] > ~low[3])if (++low[2] == 0)++high[2];
			low[0] += low[3];

			if ((s64)(high[2]) < 0)
				++exp;
			else
			{
				high[2] = (high[2] << 1) | ((s64)low[2] < 0);
				low[2] = (low[2] << 1) | ((s64)low[0] < 0);
				low[0] <<= 1;
			}
			if ((s64)low[0] < 0)if (++low[2] == 0)if (++high[2] == 0) { high[2] = MinValues::Int64; ++exp; }
			if (exp > ExpMaxValue)return Float128().MakeError(MathError::MultiplyExpOverflow);
			if (exp < ExpMinValue)return Zero();
			FloatData128 fd;
			fd.ull[1] = high[2];
			fd.ull[0] = low[2];
			return Float128(Sign == number.Sign ? 1 : -1, (s32)exp, fd);
		}
		Float128& Float128::operator*=(Float128 const& number)
		{
			if (IsError())return *this;
			if (number.IsError())return *this = number;
			if (Equals0() || number.Equals0())return *this = Float128::Zero();
			u64 high[3], low[3];
			s64 exp = Exp + number.Exp;
			low[2] = Bits::Multiply(Data.ull[1], number.Data.ull[1], high + 2);
			low[1] = Bits::Multiply(Data.ull[1], number.Data.ull[0], high + 1);
			low[0] = Bits::Multiply(Data.ull[0], number.Data.ull[1], high);
			//叠加结果是high[2] , low[2], low[0]
			if (high[1] > ~low[2])high[2]++;
			low[2] += high[1];
			if (high[0] > ~low[2])high[2]++;
			low[2] += high[0];
			if (low[0] > ~low[1])if (++low[2] == 0)++high[2];
			low[0] += low[1];

			if ((s64)(high[2]) < 0)
				++exp;
			else
			{
				high[2] = (high[2] << 1) | ((s64)low[2] < 0);
				low[2] = (low[2] << 1) | ((s64)low[0] < 0);
				low[0] <<= 1;
			}
			if ((s64)low[0] < 0)if (++low[2] == 0)if (++high[2] == 0) { high[2] = MinValues::Int64; ++exp; }
			if (exp > ExpMaxValue)return MakeError(MathError::MultiplyExpOverflow);
			if (exp < ExpMinValue)return *this = Zero();
			Exp = exp;
			Data.ull[1] = high[2];
			Data.ull[0] = low[2];
			Sign = Sign == number.Sign ? 1 : -1;
			return *this;
		}
		Float128 Float128::operator*(u64 number)const
		{
			if (IsError())return *this;
			if (Equals0() || number == 0)return Float128::Zero();
			u64 high[2], low[2];
			s64 exp = Bits::LeftZeroCount64(number);
			number <<= exp;
			exp = Exp + (63 ^ exp);
			low[1] = Bits::Multiply(Data.ull[1], number, high + 1);
			low[0] = Bits::Multiply(Data.ull[0], number, high);
			//叠加结果是high[1] , high[0], low[0]
			if (high[0] > ~low[1])++high[1];
			high[0] += low[1];

			if ((s64)(high[1]) < 0)
				++exp;
			else
			{
				high[1] = (high[1] << 1) | ((s64)high[0] < 0);
				high[0] = (high[0] << 1) | ((s64)low[0] < 0);
				low[0] <<= 1;
			}
			if ((s64)low[0] < 0)if (++high[0] == 0)if (++high[1] == 0) { high[1] = MinValues::Int64; ++exp; }
			if (exp > ExpMaxValue)return Float128().MakeError(MathError::MultiplyExpOverflow);
			if (exp < ExpMinValue)return Zero();
			FloatData128 fd;
			fd.ull[1] = high[1];
			fd.ull[0] = high[0];
			return Float128(Sign, (s32)exp, fd);
		}
		Float128 Float128::operator*(double num)const
		{
			if (IsError())return *this;
			if (Equals0() || num == 0)return Float128::Zero();
			union {
				u64 number;
				double dn;
			};
			dn = num;
			u64 high[2], low[2];
			s64 exp = Exp + ((0x7ff0000000000000 & number) >> 52) - 1023;
			number = (number << 11) | MinValues::Int64;
			low[1] = Bits::Multiply(Data.ull[1], number, high + 1);
			low[0] = Bits::Multiply(Data.ull[0], number, high);
			//叠加结果是high[1] , high[0], low[0]
			if (high[0] > ~low[1])++high[1];
			high[0] += low[1];

			if ((s64)(high[1]) < 0)
				++exp;
			else
			{
				high[1] = (high[1] << 1) | ((s64)high[0] < 0);
				high[0] = (high[0] << 1) | ((s64)low[0] < 0);
				low[0] <<= 1;
			}
			if ((s64)low[0] < 0)if (++high[0] == 0)if (++high[1] == 0) { high[1] = MinValues::Int64; ++exp; }
			if (exp > ExpMaxValue)return Float128().MakeError(MathError::MultiplyExpOverflow);
			if (exp < ExpMinValue)return Zero();
			FloatData128 fd;
			fd.ull[1] = high[1];
			fd.ull[0] = high[0];
			return Float128(Sign, (s32)exp, fd);
		}
		Float128 Float128::operator/(Float128 const& number)const
		{
			if (IsError())return *this;
			if (number.IsError())return number;
			if (number.Equals0())//若除数等于0则产生错误
				return Float128().MakeError(MathError::Divide0);
			if (Equals0())return *this;

			FloatData128 dat = Data;//获取一个数据副本
			u64 rd;
			FloatData128 datq;//存放试商
			FloatData128 datt;//存放试商与除数的乘积
			u64 d = number.Data.ull[1] + 1;

			bool flag = Data >= number.Data;
			if (flag)dat -= number.Data;//保证被除数小于除数
			//第一轮试商
			datq.ull[1] = d == 0 ? dat.ull[1] : Bits::Divide(dat.ull[1], dat.ull[0], d);
			u64 low = BitsMultiply(number.Data, datt, datq.ull[1]);
			dat -= datt;
			if (low != 0)--dat;
			rd = -(s64)low;
			//矫正，余数为dat,rd
			if (dat.ull[0] > number.Data.ull[1] || (dat.ull[0] == number.Data.ull[1] && rd >= number.Data.ull[0])) { if (rd < number.Data.ull[0])--dat.ull[0]; rd -= number.Data.ull[0]; dat.ull[0] -= number.Data.ull[1]; ++datq.ull[1]; }
			if (dat.ull[1] != 0) { if (rd < number.Data.ull[0])--dat; rd -= number.Data.ull[0]; dat.ull[0] -= number.Data.ull[1]; ++datq.ull[1]; }
			dat.ull[1] = dat.ull[0]; dat.ull[0] = rd;
			if (dat >= number.Data) { dat -= number.Data; ++datq.ull[1]; }
			//第二轮试商
			datq.ull[0] = d == 0 ? dat.ull[1] : Bits::Divide(dat.ull[1], dat.ull[0], d);
			low = BitsMultiply(number.Data, datt, datq.ull[0]);
			dat -= datt;
			if (low != 0)--dat;
			rd = -(s64)low;
			//矫正，余数为dat,rd
			if (dat.ull[0] > number.Data.ull[1] || (dat.ull[0] == number.Data.ull[1] && rd >= number.Data.ull[0])) { if (rd < number.Data.ull[0])--dat.ull[0]; rd -= number.Data.ull[0]; dat.ull[0] -= number.Data.ull[1]; ++datq.ull[0]; }
			if (dat.ull[1] != 0) { if (rd < number.Data.ull[0])--dat; rd -= number.Data.ull[0]; dat.ull[0] -= number.Data.ull[1]; ++datq.ull[0]; }
			dat.ull[1] = dat.ull[0]; dat.ull[0] = rd;
			if (dat >= number.Data) { dat -= number.Data; ++datq.ull[0]; }

			s64 exp = Exp - number.Exp;
			if (flag)
			{
				if ((datq.ull[0] & 1) && ++datq) { datq.ull[1] = MinValues::Int64; ++exp; }
				datq.MoveRight();
				datq.ull[1] |= MinValues::Int64;
			}
			else
			{
				flag = (s64)dat.ull[1] < 0;
				--exp;
				dat.MoveLeft();
				if ((flag || dat > number.Data) && ++datq) { datq.ull[1] = MinValues::Int64; ++exp; }
			}
			if (exp > Float128::ExpMaxValue)return Float128().MakeError(MathError::DivideExpOverflow);
			if (exp < Float128::ExpMinValue) { return Zero(); }
			return Float128(Sign == number.Sign ? 1 : -1, (s32)exp, datq);
		}
		Float128 Float128::operator/(double num)const
		{
			if (IsError())return *this;
			if (num == 0)return Float128().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			union {
				u64 number;
				double dn;
			};
			dn = num;
			s64 bCount = ((0x7ff0000000000000 & number) >> 52) - 1022;
			number = (number << 11) | MinValues::Int64;
			Float128 t(*this);
			bool te = t.Data.ull[1] >= number;
			u64 tmp;
			t.Data.ull[1] = Bits::Divide(t.Data.ull[1], t.Data.ull[0], number, &tmp);
			t.Data.ull[0] = Bits::Divide(t.Data.ull[0], tmp, number, &tmp);
			s64 exp = t.Exp - bCount + te;
			if (te)
			{
				te = t.Data.ull[0] & 1;
				if (te&& ++t.Data) ++exp;
				else  t.Data.MoveRight();
				t.Data.ull[1] |= MinValues::Int64;
			}
			else
			{
				if (tmp >= number / 2 && ++t.Data)
				{
					++exp;
					t.Data.ull[1] |= MinValues::Int64;
				}
			}
			if (exp > ExpMaxValue)return Float128().MakeError(MathError::DivideExpOverflow);
			if (exp < ExpMinValue)return Zero();
			t.Exp = exp;
			if (num < 0)t.Sign = -Sign;
			return t;
		}
		Float128 Float128::operator/(u64 number)const
		{
			if (IsError())return *this;
			if (number == 0)return Float128().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			s64 bCount = Bits::LeftZeroCount64(number);
			number <<= bCount;
			bCount = 64 - bCount;//bCount为number的exp
			Float128 t(*this);
			bool te = t.Data.ull[1] >= number;
			u64 tmp;
			t.Data.ull[1] = Bits::Divide(t.Data.ull[1], t.Data.ull[0], number, &tmp);
			t.Data.ull[0] = Bits::Divide(tmp, 0, number, &tmp);
			s64 exp = t.Exp - bCount + te;
			if (te)
			{
				te = t.Data.ull[0] & 1;
				if (te&& ++t.Data) ++exp;
				else  t.Data.MoveRight();
				t.Data.ull[1] |= MinValues::Int64;
			}
			else
			{
				if (tmp >= number / 2 && ++t.Data)
				{
					++exp;
					t.Data.ull[1] |= MinValues::Int64;
				}
			}
			if (exp > ExpMaxValue)return Float128().MakeError(MathError::DivideExpOverflow);
			if (exp < ExpMinValue)return Zero();
			t.Exp = exp;
			return t;
		}
		Float128 Float128::operator/(u32 number)const
		{
			if (IsError())return *this;
			if (number == 0)return Float128().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			s64 bCount = Bits::LeftZeroCount32(number);
			number <<= bCount;
			bCount = 32 - bCount;//bCount为number的exp
			Float128 t(*this);
			bool te = (t.Data.ull[1] >> 32) >= number;
			u64 tmp;
			tmp = t.Data.ull[1] % number;
			t.Data.ull[1] = (t.Data.ull[1] / number) << 32;
			tmp = (tmp << 32) | (u32)(t.Data.ull[0] >> 32);
			t.Data.ull[1] |= (u32)(tmp / number);
			tmp %= number;
			tmp = (tmp << 32) | (u32)(t.Data.ull[0]);
			t.Data.ull[0] = (tmp / number) << 32;
			tmp %= number;
			tmp <<= 32;
			t.Data.ull[0] |= (u32)(tmp / number);
			s64 exp = t.Exp - bCount + te;
			if (te)
			{
				te = t.Data.ull[0] & 1;
				if (te&& ++t.Data) ++exp;
				else  t.Data.MoveRight();
				t.Data.ull[1] |= MinValues::Int64;
			}
			else
			{
				if (tmp%number >= number / 2 && ++t.Data)
				{
					++exp;
					t.Data.ull[1] |= MinValues::Int64;
				}
			}
			if (exp > ExpMaxValue)return Float128().MakeError(MathError::DivideExpOverflow);
			if (exp < ExpMinValue)return Zero();
			t.Exp = exp;
			return t;
		}
		Float128 Float128::Sqrt()const
		{
			Float128 num(*this);
			if (num.Equals0() || num.IsError())return num;
			if (num.Sign < 0)return num.MakeError(MathError::SqrtLessThan0);
			if (num.Equals(One()))return num;
			Float128 num1 = num;
			s64 te = num1.Exp&((s64)MaxValues::LongLong ^ 1);
			num1.Exp ^= te;
			num1 = std::sqrt((double)num1);//近似值
			num1.Exp += te >> 1;
			Float128 num2 = num / num1;
			while (!num1.AppEquals(num2))//牛顿迭代
			{
				num1 = num1 + num2;
				num1.Exp--;
				num2 = num / num1;
			}
			return num2;
		}
		Float128 Float128::Mod(Float128 const&num, Float128 &quotient)const
		{
			if (IsError())return *this;
			if (num.IsError())return num;
			if (num.Equals0())return Float128().MakeError(MathError::ModDivide0);
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
				s64 exp = Exp - num.Exp;
				if (exp<ExpMinValue)quotient = Zero();
				else if (cmp>ExpMaxValue)quotient.MakeError(MathError::DivideExpOverflow);
				quotient.Exp = exp;
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
			Float128 tmp;
			if (Exp - num.Exp > 128) { quotient = *this / num; return Zero(); }
			if (cmp > 0)
			{
				Bits::ModHigh64(quotient.Data.ull, 2, Exp - num.Exp + 1, Data.ull, 2, num.Data.ull, 2, true, tmp.Data.ull, 2);
			}
			else
			{
				Bits::ModHigh64(quotient.Data.ull, 2, Exp - num.Exp, Data.ull, 2, num.Data.ull, 2, false, tmp.Data.ull, 2);
			}
			s64 mov = tmp.Data.BitsZeroCount();
			if (mov == 128)
			{
				quotient.Sign = Sign == num.Sign ? 1 : -1;
				s64 exp = Exp - num.Exp - (cmp < 0);
				if (exp<ExpMinValue)quotient = Zero();
				else if (cmp>ExpMaxValue)quotient.MakeError(MathError::DivideExpOverflow);
				quotient.Exp = exp;
				return Zero();
			}
			tmp.Data <<= mov;
			mov = num.Exp - mov;
			if (mov > ExpMaxValue)tmp.MakeError(MathError::DivideExpOverflow);
			else if (mov < ExpMinValue)tmp = Zero();
			else { tmp.Sign = num.Sign; tmp.Exp = mov; }
			quotient.Sign = 1;
			s64 exp = Exp - num.Exp - (cmp < 0);
			if (exp<ExpMinValue)quotient = Zero();
			else if (exp>ExpMaxValue)quotient.MakeError(MathError::DivideExpOverflow);
			else quotient.Exp = exp;

			//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
			if (Sign != num.Sign)
			{
				++quotient; quotient.Sign = -1;
				tmp = num - tmp;
			}
			return tmp;
		}
		Float128 Float128::Mod(Float128 const&num)const
		{
			if (IsError())return *this;
			if (num.IsError())return num;
			if (num.Equals0())return Float128().MakeError(MathError::ModDivide0);
			if (Exp < num.Exp)//0.1%2=0...0.1，0.1%-2=-1...-1.9，-0.1%2=-1...1.9，-0.1%-2=0...-0.1
			{
				if (Sign == num.Sign)return *this;
				return *this + num;
			}
			index_t cmp = Data.CompareWith(num.Data);
			if (cmp == 0)
			{
				return Zero();
			}
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
			Float128 tmp;
			if (Exp - num.Exp > 128) { return Zero(); }
			if (cmp > 0)
			{
				Bits::ModHigh64(NULL, 0, Exp - num.Exp + 1, Data.ull, 2, num.Data.ull, 2, true, tmp.Data.ull, 2);
			}
			else
			{
				Bits::ModHigh64(NULL, 0, Exp - num.Exp, Data.ull, 2, num.Data.ull, 2, false, tmp.Data.ull, 2);
			}
			s64 mov = tmp.Data.BitsZeroCount();
			if (mov == 128)
			{
				return Zero();
			}
			tmp.Data <<= mov;
			mov = num.Exp - mov;
			if (mov > ExpMaxValue)tmp.MakeError(MathError::DivideExpOverflow);
			else if (mov < ExpMinValue)tmp = Zero();
			else { tmp.Sign = num.Sign; tmp.Exp = mov; }

			//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
			if (Sign != num.Sign)
			{
				tmp = num - tmp;
			}
			return tmp;
		}
		Float128 Float128::Log(Float128 const& number)const
		{
			return Ln() / number.Ln();
		}
		Float128 Float128::Lg()const
		{
			return Ln() / Ln10();
		}
		Float128 Float128::Ln()const
		{
			if (IsError())return *this;
			Float128 num_x;//记录x
			Float128 num_xn;//记录x^n
			num_x = *this;
			s64 ln2 = Exp;//exp偏移
			Float128 ln_plus = Zero();

			if (num_x <= Zero())return num_x.MakeError(MathError::LnLessOrEqual0);
			num_x.Exp = 0;//使得num_x变为1.xxx
			for (int i = 1; i < 12; ++i)//取值为12时加速效果较好
				if (num_x >= _2sqrt2()[i])
				{
					num_x *= _12sqrt2()[i];
					ln_plus += _ln2sqrt2()[i];
				}


			//直接级数展开ln(1+x) = x-(x^2)/2+(x^3)/3-...
			//num_x-=1
			/*num_x.Data.ull[1]&=MaxValues::LongLong;
			long long mov = num_x.Data.BitsZeroCount();
			if(mov==128)return ln_plus+ Ln2()*ln2;
			num_x.Exp-=mov;
			num_x.Data<<=mov;

			num_xn = num_x;

			Float128 num_1 = num_x;//初始值
			Float128 num_2 = -One();//循环赋值
			for(int i=2;;i++)//泰勒展开叠加
			{
			num_xn = -num_xn*num_x;
			num_1 += num_xn/i;
			if(num_1.AppEquals(num_2))break;
			num_2 = num_1;
			}*/
			//另一种展开方式：ln(x) = ln((1+y)/(1-y)) = 2y*(1+(y^2)/3+(y^4)/5+(y^6)/7+...)
			//y=(x-1)/(x+1)
			Float128 num_y = (num_x - 1) / (1 + num_x);
			Float128 num_1 = num_y;
			Float128 num_y2 = num_y*num_y;
			Float128 num_yn = num_y*num_y2;
			for (s32 i = 3;; i += 2)
			{
				num_1 += num_yn / i;
				if (num_yn.Exp < -128)break;
				num_yn *= num_y2;
			}
			if (!num_1.Equals0())num_1.Exp++;
			num_1 += ln_plus;
			if (ln2)return num_1 + Ln2()*ln2;
			return num_1;
		}
		Float128 Float128::Log2()const
		{
			return Ln() / Ln2();
		}
		Float128 Float128::Expe()const
		{
			if (IsError())return *this;
			Float128 num_x, num_xn, num_factorial, num_1, num_2;
			if (Exp > 23)
			{
				if (Sign < 0)
					return Zero();
				num_x = *this;
				return num_x.MakeError(MathError::ExpEOverflow);
			}

			num_x = Mod(Ln2(), num_factorial);
			long long expmov = (long long)num_factorial;
			s32 i;
			s64 t_factorial = 1;
			num_xn = num_x;
			num_1 = One() + num_xn;
			num_2 = num_1;
			for (i = 2;; i++)
			{
				if (t_factorial != 0)
				{
					if (i > 20) { num_factorial = t_factorial; t_factorial = 0; num_factorial *= i; }
					else t_factorial *= i;
				}
				else {
					num_factorial = num_factorial*i;
				}
				num_xn = num_xn * num_x;
				num_1 += i < 13 ? num_xn / (s32)t_factorial : i < 21 ? num_xn / t_factorial : num_xn / num_factorial;
				if (num_1.AppEquals(num_2))break;
				num_2 = num_1;
			}
			if (expmov)num_1.Exp += expmov;
			return num_1;
		}
		Float128 Float128::Exp2()const
		{
			if (IsError())return *this;
			Float128 num_x, num_xn, num_factorial, num_1, num_2;
			if (Exp >= 31)
			{
				if (Sign < 0)
					return Zero();
				num_x = *this;
				return num_x.MakeError(MathError::Exp2Overflow);
			}

			num_x = GetDecimalPart();
			s64 expmov = (s64)GetIntegerPart();
			num_x = num_x*Ln2();
			s32 i;
			s64 t_factorial = 1;
			num_xn = num_x;
			num_1 = One() + num_x;
			num_2 = num_1;
			for (i = 2;; i++)
			{
				if (t_factorial != 0)
				{
					if (i > 20) { num_factorial = t_factorial; t_factorial = 0; num_factorial *= i; }
					else t_factorial *= i;
				}
				else {
					num_factorial = num_factorial*i;
				}
				num_xn *= num_x;
				num_1 += i < 13 ? num_xn / (s32)t_factorial : i < 21 ? num_xn / t_factorial : num_xn / num_factorial;
				if (num_1.AppEquals(num_2))break;
				num_2 = num_1;
			}
			if (expmov)num_1.Exp += expmov;
			return num_1;
		}
		Float128 Float128::Pow(Float128 const& number)const
		{
			if (IsError())return(*this);
			if (number.IsError())return number;
			if (*this <= Zero())
			{
				Float128 num = *this;
				if (Equals0() && number.Equals0())return num.MakeError(MathError::Pow00);
				if (number.GetDecimalPart().Equals0())//整数次方
				{
					Float128 tmp = (Abs().Log2()*number).Exp2();
					if (tmp.IsError())tmp.MakeError(MathError::PowExpOverflow);
					if (number.Half().GetDecimalPart())return -tmp;
					return tmp;
				}
				return num.MakeError(MathError::PowLessThan0);
			}
			Float128 tmp = (Log2()*number).Exp2();//使用Exp2来算
			if (tmp.IsError())tmp.MakeError(MathError::PowExpOverflow);
			return tmp;
			//return this->Abs().Ln()*(number).Exp();//使用Exp来算
		}
		Float128 Float128::Sin()const
		{
			if (IsError())return *this;
			Float128 num_x, num_xn, num_factorial, num_2, sum;
			num_x = Mod(Pi(), num_factorial);//除以π以判断正负
			bool negative = num_factorial.IsOdd();//商为奇数说明结果为负
			num_x = num_x.Mod(_pi_half2(), num_factorial);//除以π/4以判断操作方法
			switch ((int)num_factorial)
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
			s64 t_factorial = 1;//阶乘
			num_xn = num_x;
			sum = num_x;
			num_2 = num_x;
			num_x *= num_x;//x^2
			s64 i;
			num_factorial = 1;
			for (i = 3;; i += 2)//Sin x = x-x^3/3!+x^5/5!-
			{
				if (t_factorial != 0)
				{
					if (i > 20) { num_factorial = t_factorial; t_factorial = 0; num_factorial = num_factorial*(-i*(i - 1)); }
					else t_factorial *= -i*(i - 1);
				}
				else
					num_factorial = num_factorial*(-i*(i - 1));
				num_xn *= num_x;
				Float128 t = i < 13 ? (num_xn / (s32)t_factorial) : i < 21 ? (num_xn / t_factorial) : (num_xn / num_factorial);
				sum += t;
				if (t.Equals0() || sum.Exp >= t.Exp + 127)break;
			}
			if (negative)return -sum;
			return sum;
		}
		Float128 Float128::Dsin()const
		{
			return ((*this) * _pi_180()).Sin();
		}
		Float128 Float128::Cos()const
		{
			if (IsError())return *this;
			Float128 num_x, num_xn, num_factorial, num_2, sum;
			num_x = this->Mod(Pi(), num_factorial);//除以π以供判断正负
			bool negative = num_factorial.IsOdd();//商为奇时negative为true
			num_x = num_x.Mod(_pi_half2(), num_factorial);//除以π/4以判断操作方法
			switch (int(num_factorial))
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
			s64 t_factorial = 1;//阶乘
			num_xn = One();
			sum = One();
			num_2 = One();
			bool s_8 = num_x.Exp > ExpMinValue + 3;//1/8加速
			if (s_8)num_x.Exp -= 3;
			num_x *= num_x;//变x方
			s64 i;
			for (i = 2;; i += 2)//Cosx = 1-x^2/2!+x^4/4!-
			{
				if (t_factorial != 0)
				{
					if (i > 20) { num_factorial = t_factorial; t_factorial = 0; num_factorial *= (-i*(i - 1)); }
					else t_factorial *= (-i*(i - 1));
				}
				else {
					num_factorial = num_factorial*(-i*(i - 1));
				}
				num_xn = num_xn*num_x;
				Float128 t = i < 13 ? (num_xn / (s32)t_factorial) : i < 21 ? (num_xn / t_factorial) : num_xn / num_factorial;
				sum += t;
				if (t.Equals0() || sum.Exp >= t.Exp + 127)break;
			}
			if (s_8)
			{
				num_x = sum * sum;
				num_xn = -num_x;
				num_x *= num_x;
				num_xn.Exp += 5;
				sum = One() + num_xn;
				sum += num_x * 160;
				num_xn *= num_x;
				num_xn.Exp += 3;
				sum += num_xn;
				num_x *= num_x;
				num_x.Exp += 7;
				sum += num_x;
			}
			if (negative)return -sum;
			return sum;
		}
		void Float128::SinAndCos(Float128 & outsin, Float128 & outcos) const
		{
			if (IsError()) { outsin = outcos = *this; return; }
			Float128 num_x, num_xn, num_factorial, num_2, sum;
			num_x = Mod(_pi_half2(), num_factorial);//除以π/4以判断操作方法
			u64 flag = num_factorial;
			bool swp = false;//交换
			switch ((flag & 7))
			{
			case 1://45~90°
				if (num_x.Equals0()) { outsin = outcos = _sqrt2_half(); return; }//返回(根号2)/2
				num_x = _pi_half2() - num_x;//cos收敛速度快
				swp = true;
				break;
			case 2://90~135°
				if (num_x.Equals0()) { outsin = One(); outcos = Zero(); return; }
				swp = true;
				break;
			case 3://135~180°
				if (num_x.Equals0()) { outsin = outcos = _sqrt2_half(); outcos.SelfNegative(); return; }
				num_x = _pi_half2() - num_x;
			case 4://180~225°
				if (num_x.Equals0()) { outcos = -One(); outsin = Zero(); return; }
				break;
			case 5://225~270°
				if (num_x.Equals0()) { outsin = outcos = -_sqrt2_half(); return; }//返回(根号2)/2
				num_x = _pi_half2() - num_x;//cos收敛速度快
				swp = true;
				break;
			case 6://270~315°
				if (num_x.Equals0()) { outsin = -One(); outcos = Zero(); return; }
				swp = true;
				break;
			case 7://315~360°
				if (num_x.Equals0()) { outsin = outcos = _sqrt2_half(); outsin.SelfNegative(); return; }
				num_x = _pi_half2() - num_x;
			default:
				break;
			}
			s64 t_factorial = 1;//阶乘
			num_xn = num_x;
			sum = num_x;
			num_2 = num_x;
			num_x *= num_x;
			s64 i;
			for (i = 3;; i += 2)//Sin x = x-x^3/3!+x^5/5!-
			{
				if (t_factorial != 0)
				{
					if (i > 20) { num_factorial = t_factorial; t_factorial = 0; num_factorial *= (-i*(i - 1)); }
					else t_factorial *= (-i*(i - 1));
				}
				else {
					num_factorial = num_factorial*(-i*(i - 1));
				}
				num_xn *= num_x;
				Float128 t = i < 13 ? (num_xn / (s32)t_factorial) : i < 21 ? (num_xn / t_factorial) : num_xn / num_factorial;
				sum += t;
				if (t.Equals0() || sum.Exp >= t.Exp + 127)break;
			}
			if (swp)
			{
				outcos = (flag > 1 && flag < 6) ? -sum : sum;
				outsin = (One() - sum*sum).Sqrt();
				if (flag >= 4)outsin.SelfNegative();
			}
			else
			{
				outsin = flag >= 4 ? -sum : sum;
				outcos = (One() - sum*sum).Sqrt();
				if (flag > 1 && flag < 6)outcos.SelfNegative();
			}
		}
		Float128 Float128::Dcos()const
		{
			return ((*this)*_pi_180()).Cos();
		}
		Float128 Float128::Tan()const
		{
			if (IsError())return *this;
			Float128 num_cos, num_sin;
			SinAndCos(num_sin, num_cos);
			if (num_cos.Equals0())return num_cos.MakeError(MathError::Tan);
			return num_sin / num_cos;
		}
		Float128 Float128::Dtan()const
		{
			return Dsin() / Dcos();
		}
		Float128 Float128::Cot()const
		{
			if (IsError())return *this;
			Float128 num = Sin();
			if (num.Equals0())return num.MakeError(MathError::Cot);
			return Cos() / num;
		}
		Float128 Float128::Dcot()const
		{
			if (IsError())return *this;
			Float128 num = Dsin();
			if (num.Equals0())return num.MakeError(MathError::Cot);
			return Dcos() / num;
		}
		Float128 Float128::Arcsin()const
		{
			if (IsError())return *this;
			Float128 num_x, num_xn;//x、xn
			Float128 num1, num2;//分子、分母
			Float128 num_1, num_2;//前一项、后一项
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
				if (num_1.AppEquals(num_2))break;
				num_2 = num_1;
			}
			if (Sign < 0)return -num_1;
			return num_1;
		}
		Float128 Float128::Darcsin()const
		{
			return Arcsin() / _pi_180();
		}
		Float128 Float128::Arccos()const
		{
			if (IsError())return *this;
			if (Abs() > One())
			{
				Float128 num = *this;
				return num.MakeError(MathError::Arccos);//出错
			}
			if (*this < _sqrt2_half())return _pi_half() - Arcsin();//小于根号2/2，这样计算可以在接近1时获得较高的精度
			return (One() - *this**this).SelfAbs().Sqrt().Arcsin();//取正值
		}
		Float128 Float128::Darccos()const
		{
			return Arccos() / _pi_180();
		}
		Float128 Float128::Arctan()const
		{
			if (IsError())return *this;
			//配方法：tanx=sinx/cosx，设cosx>0，则sinx = this/sqrt(this平方+1)，转为求arcsin,但此方法在x趋于0时精度大减
			if (Abs() > _sqrt2_half())return (*this / ((*this**this + One()).SelfAbs().Sqrt())).Arcsin();
			//级数法：arctan x = x - x^3/3 + x^5/5 - ... (x≤1) ，x趋于1时收敛太慢
			Float128 num_x, num_xn;//x、xn
			Float128 num = Three();//分母
			Float128 num_1, num_2;//前一项、后一项
			num_x = -*this**this;
			num_xn = (*this)*num_x;
			num_2 = num_1 = *this;
			int i;
			for (i = 3;; i += 2)
			{
				num_1 += num_xn / i;
				if (num_1.AppEquals(num_2))break;
				num_2 = num_1;
				num_xn *= num_x;
			}
			return num_1;
		}
		Float128 Float128::Darctan()const
		{
			return Arctan() / _pi_180();
		}
		bool Float128::Equals(const Float128& number)const
		{
			return Exp == number.Exp && Data == number.Data && Sign == number.Sign;
		}
		bool Float128::AppEquals(const Float128& number, u64 less)const
		{
			if (number.Sign != Sign)
				return false;
			Float128 num1, num2;
			if (Exp > number.Exp)
			{
				if (Exp - 1 > number.Exp)return false;
				num1 = (*this);
				num2 = number;
				num2.Data.MoveRight();
			}
			else if (number.Exp > Exp)
			{
				if (number.Exp - 1 > Exp)return false;
				num2 = (*this);
				num1 = number;
				num2.Data.MoveRight();
			}
			else
			{
				if (Data > number.Data)
				{
					num1 = (*this);
					num2 = number;
				}
				else
				{
					num2 = (*this);
					num1 = number;
				}
			}
			num1.Data -= num2.Data;
			if (num1.Data.ull[1])
				return false;
			return num1.Data.ull[0] < less;
		}
		Float128 Float128::Rand()
		{
			Float128 f;
			f.Exp = -1;
			f.Sign = 1;
			f.Data.ull[0] = GetDefaultRand();
			f.Data.ull[1] =GetDefaultRand();
			s64 bitsCount = f.Data.BitsZeroCount();
			if (bitsCount == 128)
				return f = Zero();
			f.Data <<= bitsCount;
			f.Exp -= bitsCount;
			return f;
		}
		Float128 Float128::GetIntegerPart()const
		{
			if (IsError() || Exp >= 127)return *this;
			if (Exp < 0)
				return Zero();
			Float128 num = *this;
			num.Data >>= 127 - Exp;
			num.Data <<= 127 - Exp;
			return num;
		}
		Float128& Float128::SelfGetIntegerPart()
		{
			if (IsError() || Exp >= 127)return *this;
			if (Exp < 0)
				return *this = Zero();
			Data >>= 127 - Exp;
			Data <<= 127 - Exp;
			return *this;
		}
		Float128 Float128::GetDecimalPart()const
		{
			if (IsError() || Exp < 0)return *this;
			if (Exp >= 127)
				return Zero();
			Float128 num = *this;
			num.Data <<= 1 + Exp;
			long long mov = num.Data.BitsZeroCount();
			if (128 == mov)
				return Zero();
			num.Data <<= mov;
			num.Exp = -1 - mov;
			return num;
		}
		Float128& Float128::SelfGetDecimalPart()
		{
			if (IsError() || Exp < 0)return *this;
			if (Exp >= 127)
				return *this = Zero();
			Data <<= 1 + Exp;
			long long  mov = Data.BitsZeroCount();
			if (128 == mov) return *this = Zero();
			Data <<= mov;
			Exp = -1 - mov;
			return *this;
		}
		std::string Float128::ToString(int length)const
		{
			s64 i = 0;
			std::string tmpstr;//临时字符串
			if (Equals0())
				return "0";
			if (IsError())//错误信息输出
			{
				if (Sign > 1)return "未知异常";
				if (Sign < -23)return "未知错误";
				return MathError::description[-(int)Sign];
			}
			Float128 num = *this;
			unsigned char k;
			long long j, position, exp10;
			i = j = position = 0;
			exp10 = -1;
			unsigned char number[48];//最长精度为39,48位已经足够了
			if (length <= 0 || length > 47)length = 39;
			if (Exp >= 0)//加速处理
			{
				unsigned long long texp = 0x20000000;
				int exp_mov = 30;
				while (exp_mov-- > 0)
				{
					if (num.Exp >= _10e2()[exp_mov].Exp)
					{
						num /= _10e2()[exp_mov];
						exp10 += texp;
					}
					texp >>= 1;
				}
				num /= 10;
				exp10++;
			}
			if (Exp <= -332){
				unsigned long long texp = 0x20000000;
				int exp_mov = 30;
				while (exp_mov-- > 0)
				{
					if (num.Exp < -_10e2()[exp_mov].Exp)
					{
						num *= _10e2()[exp_mov];
						exp10 -= texp;
					}
					texp >>= 1;
				}
			}
			num.SelfAbs();
			for (;;)
			{
				k = (unsigned char)(num.SelfMultiply10()).GetIntegerPart();
				if (k)num.SelfGetDecimalPart();
				if (!k && !position)//确保首位非0
				{
					exp10--;//十进制指数减小
					continue;
				}
				else if (10 == k)//避免0.999999事件
				{
					k = 0;
					for (i = position - 1; i >= 0; --i)
					{
						if (9 > number[i])
						{
							number[i]++;
							break;
						}
						number[i] = 0;
					}
					if (i < 0)//进位到开头
					{
						number[position] = 0;
						position++;
						exp10++;
						number[0] = 1;
					}
				}
				number[position] = k;//字符串赋值
				if (position >= length)//进位校正(这里设定精度位数)
				{
					if (number[position]>4)//四舍五入，有进位操作
						number[position - 1]++;
					number[position] = 0;
					--position;
					while (position > 0)
					{
						if (number[position] > 9)//需要进位
						{
							number[position - 1]++;
							number[position] = 0;
						}
						else if (number[position])break;//作这个判断是为了多0情况下消除0
						--position;
					}
					if (number[0] > 9)//开头要进位
					{
						number[0] = 1;
						exp10++;
					}
					break;
				}
				++position;
			}
			if (exp10 >= 0 && position > exp10)//小数点夹在其中
			{
				tmpstr.resize(position + 4);
				for (i = 0; i <= position; ++i)
				{
					if (i == 1 + exp10)
					{
						tmpstr[i] = L'.';
						++j;
					}
					tmpstr[j] = number[i] + '0';
					++j;
				}
				tmpstr[j] = 0;
			}
			else if (position == exp10)//无需修改
			{
				tmpstr.resize(position + 3);
				for (i = 0; i <= position; ++i)
					tmpstr[i] = number[i] + '0';
				tmpstr[i] = 0;
			}
			else if (position<exp10 && position + 4>exp10)//末尾补0,+4代表末位最多出现3个0
			{
				tmpstr .resize(position + 3 + exp10 - position);
				for (i = 0; i <= position; ++i)
					tmpstr[i] = number[i] + '0';
				for (j = 0; j < exp10 - position; ++j)
					tmpstr[i + j] = '0';
				tmpstr[i + j] = 0;
			}
			else if (exp10<0 && exp10>-4)//小数，前面0较少，不用科学计数法
			{
				tmpstr.resize(position + 4 - exp10);
				for (i = 0; i < 1 - exp10; ++i)
					tmpstr[i] = '0';
				tmpstr[1] = '.';
				for (j = 0; j <= position; ++j)
					tmpstr[i + j] = number[j] + '0';
				tmpstr[i + j] = 0;
			}
			else//科学计数法
			{
				tmpstr .resize(position + 41);
				for (i = 0; i <= position; ++i)
				{
					if (i == 1)
					{
						tmpstr[j] = '.';
						++j;
					}
					tmpstr[j] = '0' + number[i];
					++j;
				}
				tmpstr[j] = 'e';
				++j;
				if (exp10 < 0)
				{
					tmpstr[j] = '-';
					++j;
					exp10 = -exp10;
				}
				for (i = 0; exp10; ++i)
				{
					tmpstr[j + i] = exp10 % 10 + '0';
					exp10 /= 10;
				}
				tmpstr[j + i] = 0;
				--i;
				while (i > 0)
				{
					k = (unsigned char)tmpstr[j];
					tmpstr[j] = tmpstr[j + i];
					tmpstr[j + i] = k;
					i -= 2;
					j++;
				}
			}
			i = Strings::strlen(tmpstr.c_str());
			if (Sign < 0)
			{
				j = ++i;
				while (j > 0) { tmpstr[j] = tmpstr[j - 1]; --j; }
				tmpstr[0 ]= '-';
			}
			tmpstr.resize(i);
			return tmpstr;
		}
		bool Float128::ThrowException = false;
		const Float128& Float128::Zero()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 val = Float128(0);
			p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::One()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(1);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Two()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(2);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Three()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(3);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Four()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(4);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Five()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(5);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Six()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(6);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Seven()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(7);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Eight()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(8);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Nine()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(9);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Ten()
		{
			static Float128* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(10);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128*const Float128::_10e2()
		{
			static Float128 *p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 f[30];
				f[0] = 10.0;
				for (int i = 1; i < 30; ++i)f[i] = f[i - 1] * f[i - 1];
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float128& Float128::Ln2()
		{
			static Float128* p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 val= Float128(L"ln2");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Ln10()
		{
			static Float128* p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(L"ln10");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128*const Float128::_2sqrt2()
		{
			static Float128* p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 f[16];
				f[0] = 2.0;
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Sqrt();
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float128*const Float128::_12sqrt2()
		{
			static Float128*p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 f[16];
				f[0] = 2.0;
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Sqrt();
				for (int i = 0; i < 16; ++i)f[i] = 1 / f[i];
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float128*const Float128::_ln2sqrt2()
		{
			static Float128*p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 f[16];
				f[0] = Float128(L"ln2");
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Half();
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float128& Float128::MaxValue()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 val = Float128(L"max");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::MinValue()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Float128 val = Float128(L"min");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::E()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(L"e");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Pi()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(L"pi");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::_pi_half()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(L"pi").Half();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::_pi_half2()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(L"pi").Half(2);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::Sqrt2()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(2).Sqrt();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::_sqrt2_half()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(2).Sqrt().Half();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float128& Float128::_pi_180()
		{
			static Float128 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float128 val = Float128(L"pi") / Float128(180.0);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
	}
	int Conversion::Convert <std::string , Math::Float128>::DefParam[1] = { 6 };
}
#endif
