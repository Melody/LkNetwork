#include "stdafx.h"
#ifdef LK_MATH_FLOAT256_
namespace LK
{
	namespace Math
	{

		unsigned long long FloatData256::operator<<=(long long mov)
		{//
			if (mov <= 0)
			{
				if (mov)return *this >>= -mov;
				return 0;
			}
			unsigned long long tmp = 0;
			if (!(mov & 0x3f))
			{
				switch (mov)
				{
				case 64:
					tmp = ull[3];
					ull[3] = ull[2]; ull[2] = ull[1]; ull[1] = ull[0];
					ull[0] = 0;
					return tmp;
				case 128:
					tmp = ull[2];
					ull[3] = ull[1]; ull[2] = ull[0];
					ull[1] = ull[0] = 0;
					return tmp;
				case 192:
					tmp = ull[1];
					ull[3] = ull[0];
					ull[2] = ull[1] = ull[0] = 0;
					return tmp;
				case 256:
					tmp = ull[0];
				default:
					ull[0] = ull[1] = ull[2] = ull[3] = 0;
				}
				return tmp;
			}
			switch (mov >> 6)
			{
			case 0:
				tmp = ull[3] >> (64 - mov);
				ull[3] = (ull[3] << mov) | (ull[2] >> (64 - mov));
				ull[2] = (ull[2] << mov) | (ull[1] >> (64 - mov));
				ull[1] = (ull[1] << mov) | (ull[0] >> (64 - mov));
				ull[0] <<= mov;
				return tmp;
			case 1:
				tmp = (ull[3] << (mov - 64)) | (ull[2] >> (128 - mov));
				ull[3] = (ull[2] << (mov - 64)) | (ull[1] >> (128 - mov));
				ull[2] = (ull[1] << (mov - 64)) | (ull[0] >> (128 - mov));
				ull[1] = ull[0] << (mov - 64);
				ull[0] = 0;
				return tmp;
			case 2:
				tmp = (ull[2] << (mov - 128)) | (ull[1] >> (192 - mov));
				ull[3] = (ull[1] << (mov - 128)) | (ull[0] >> (192 - mov));
				ull[2] = ull[0] << (mov - 128);
				ull[1] = ull[0] = 0;
				return tmp;
			case 3:
				tmp = (ull[1] << (mov - 192)) | (ull[0] >> (265 - mov));
				ull[3] = ull[0] << (mov - 192);
				ull[2] = ull[1] = ull[0] = 0;
				return tmp;
			case 4:tmp = ull[0] << (mov - 256);
			default:
				ull[0] = ull[1] = ull[2] = ull[3] = 0;
			}
			return tmp;
		}
		unsigned long long FloatData256::operator>>=(long long mov)
		{
			if (mov <= 0)
			{
				if (mov)return *this <<= -mov;
				return 0;
			}
			unsigned long long tmp = 0;
			if (!(mov & 0x3f))//
			{
				switch (mov)
				{
				case 64:
					tmp = ull[0];
					ull[0] = ull[1];
					ull[1] = ull[2];
					ull[2] = ull[3];
					ull[3] = 0;
					return tmp;
				case 128:
					tmp = ull[1];
					ull[0] = ull[2];
					ull[1] = ull[3];
					ull[2] = ull[3] = 0;
					return tmp;
				case 192:
					tmp = ull[2];
					ull[0] = ull[3];
					ull[1] = ull[2] = ull[3] = 0;
					return tmp;
				case 256:
					tmp = ull[3];
				default:
					ull[0] = ull[1] = ull[2] = ull[3] = 0;
				}
				return tmp;
			}
			switch (mov >> 6)
			{
			case 0:
				tmp = (ull[0] << (64 - mov));
				ull[0] = (ull[0] >> mov) | (ull[1] << (64 - mov));
				ull[1] = (ull[1] >> mov) | (ull[2] << (64 - mov));
				ull[2] = (ull[2] >> mov) | (ull[3] << (64 - mov));
				ull[3] >>= mov;
				return tmp;
			case 1:
				tmp = (ull[0] >> (mov - 64)) | (ull[1] << (128 - mov));
				ull[0] = (ull[1] >> (mov - 64)) | (ull[2] << (128 - mov));
				ull[1] = (ull[2] >> (mov - 64)) | (ull[3] << (128 - mov));
				ull[2] = ull[3] >> (mov - 64);
				ull[3] = 0;
				return tmp;
			case 2:
				tmp = (ull[1] >> (mov - 128)) | (ull[2] << (192 - mov));
				ull[0] = (ull[2] >> (mov - 128)) | (ull[3] << (192 - mov));
				ull[1] = ull[3] >> (mov - 128);
				ull[2] = ull[3] = 0;
				return tmp;
			case 3:
				tmp = (ull[2] >> (mov - 192)) | (ull[3] << (256 - mov));
				ull[0] = ull[3] >> (mov - 192);
				ull[1] = ull[2] = ull[3] = 0;
				return tmp;
			case 4:
				tmp = ull[3] >> (mov - 256);
			default:
				ull[0] = ull[1] = ull[2] = ull[3] = 0;
			}
			return tmp;
		}
		bool FloatData256::operator-=(FloatData256 &data)
		{
			bool sign1, sign2;
			sign1 = ull[0] < data.ull[0];
			ull[0] -= data.ull[0];
			if (sign1)sign2 = ull[1] <= data.ull[1];
			else sign2 = ull[1] < data.ull[1];
			ull[1] -= data.ull[1] + sign1;
			if (sign2)sign1 = ull[2] <= data.ull[2];
			else sign1 = ull[2] < data.ull[2];
			ull[2] -= data.ull[2] + sign2;
			if (sign1)sign2 = ull[3] <= data.ull[3];
			else sign2 = ull[3] < data.ull[3];
			ull[3] -= data.ull[3] + sign1;
			return sign2;
		}
		bool FloatData256::operator+=(FloatData256 &data)
		{
			bool sign1, sign2;
			sign1 = ull[0] > (~data.ull[0]);
			ull[0] += data.ull[0];
			if (sign1)sign2 = ull[1] >= (~data.ull[1]);
			else sign2 = ull[1] > (~data.ull[1]);
			ull[1] += data.ull[1] + sign1;
			if (sign2)sign1 = ull[2] >= (~data.ull[2]);
			else sign1 = ull[2] > (~data.ull[2]);
			ull[2] += data.ull[2] + sign2;
			if (sign1)sign2 = ull[3] >= (~data.ull[3]);
			else sign2 = ull[3] > (~data.ull[3]);
			ull[3] += data.ull[3] + sign1;
			return sign2;
		}
		bool FloatData256::operator++()
		{
			if (0 == (++ull[0]))if (0 == (++ull[1]))if (0 == (++ull[2]))if (0 == (++ull[3]))return true;
			return false;
		}
		bool FloatData256::operator--()
		{
			if (0 == (ull[0]--))if (0 == (ull[1]--))if (0 == (ull[2]--))if (0 == (ull[3]--))return true;
			return false;
		}
		long long FloatData256::BitsZeroCount()//返回值可能为0~256,已加速
		{
			if (ull[3] != 0) { return Bits::LeftZeroCount64(ull[3]); }
			if (ull[2] != 0) { return Bits::LeftZeroCount64(ull[2]) + 64; }
			if (ull[1] != 0) { return Bits::LeftZeroCount64(ull[1]) + 128; }
			return Bits::LeftZeroCount64(ull[0]) + 192;
		}
		bool FloatData256::operator>(const FloatData256 &data)const
		{
			if (ull[3] != data.ull[3])return ull[3] > data.ull[3];
			if (ull[2] != data.ull[2])return ull[2] > data.ull[2];
			if (ull[1] != data.ull[1])return ull[1] > data.ull[1];
			return ull[0] > data.ull[0];
		}
		bool FloatData256::operator<(const FloatData256 &data)const
		{
			if (ull[3] != data.ull[3])return ull[3] < data.ull[3];
			if (ull[2] != data.ull[2])return ull[2] < data.ull[2];
			if (ull[1] != data.ull[1])return ull[1] < data.ull[1];
			return ull[0] < data.ull[0];
		}
		bool FloatData256::operator==(const FloatData256 &data)const
		{
			return ull[3] == data.ull[3] && ull[2] == data.ull[2] && ull[1] == data.ull[1] && ull[0] == data.ull[0];
		}
		bool FloatData256::operator!=(const FloatData256 &data)const
		{
			return ull[3] != data.ull[3] || ull[2] != data.ull[2] || ull[1] != data.ull[1] || ull[0] != data.ull[0];
		}
		bool FloatData256::operator>=(const FloatData256 &data)const
		{
			if (ull[3] != data.ull[3])return ull[3] > data.ull[3];
			if (ull[2] != data.ull[2])return ull[2] > data.ull[2];
			if (ull[1] != data.ull[1])return ull[1] > data.ull[1];
			return ull[0] >= data.ull[0];
		}
		bool FloatData256::operator<=(const FloatData256 &data)const
		{
			if (ull[3] != data.ull[3])return ull[3] < data.ull[3];
			if (ull[2] != data.ull[2])return ull[2] < data.ull[2];
			if (ull[1] != data.ull[1])return ull[1] < data.ull[1];
			return ull[0] <= data.ull[0];
		}
		index_t FloatData256::CompareWith(const FloatData256 &data)
		{
			if (ull[3] != data.ull[3])return ull[3] > data.ull[3] ? 1 : -1;
			if (ull[2] != data.ull[2])return ull[2] > data.ull[2] ? 1 : -1;
			if (ull[1] != data.ull[1])return ull[1] > data.ull[1] ? 1 : -1;
			return ull[0] > data.ull[0] ? 1 : ull[0] < data.ull[0] ? -1 : 0;
		}

		bool Float256::IsInteger()const {
			if (Equals0() || Exp >= 255)return true;
			if (Exp < 0)return false;
			if (Exp > 191)return !(Data.ull[0] << (Exp - 191));
			if (Data.ull[0] != 0)return false;
			if (Exp == 191)return true;
			if (Exp > 127)return !(Data.ull[1] << (Exp - 127));
			if (Data.ull[1] != 0)return false;
			if (Exp == 127)return true;
			if (Exp > 63)return !(Data.ull[2] << (Exp - 63));
			if (Data.ull[2] != 0)return false;
			return Exp == 63 || !(Data.ull[3] << (Exp + 1));
		}
		bool Float256::IsOdd()const {
			if (Equals0() || Exp > 255)return false;
			if (Exp < 0)return false;
			if (Exp == 255)return Data.ull[0] & 1;
			if (Exp > 191)return !(Data.ull[0] << (Exp - 191)) && (s64)(Data.ull[0] << (Exp - 190)) < 0;
			if (Data.ull[0] != 0)return false;
			if (Exp == 191)return Data.ull[1] & 1;
			if (Exp > 127)return !(Data.ull[1] << (Exp - 127)) && (s64)(Data.ull[1] << (Exp - 126)) < 0;
			if (Data.ull[1] != 0)return false;
			if (Exp == 127)return Data.ull[2] & 1;
			if (Exp > 63)return !(Data.ull[2] << (Exp - 63)) && (s64)(Data.ull[2] << (Exp - 62)) < 0;
			if (Data.ull[2] != 0)return false;
			return Exp == 63 ? (Data.ull[3] & 1) : (!(Data.ull[3] << (Exp + 1)) && ((s64)(Data.ull[3] << Exp) < 0));
		}
		//Float256类的函数
		const long long Float256::ExpMaxValue = 0x007fffffffffffff;
		const long long Float256::ExpMinValue = 0xff80000000000000;
		void Float256::BitsMoveLeft(unsigned long long* pull)
		{//已验证
			pull[3] = (pull[3] << 1) | ((long long)pull[2] < 0);
			pull[2] = (pull[2] << 1) | ((long long)pull[1] < 0);
			pull[1] = (pull[1] << 1) | ((long long)pull[0] < 0);
			pull[0] <<= 1;
		}
		void Float256::BitsMoveRight(unsigned long long* pull)
		{//已验证
			if (*pull & 1)
			{
				pull[0] = (pull[0] >> 1) | (pull[1] << 63);
				pull[1] = (pull[1] >> 1) | (pull[2] << 63);
				pull[2] = (pull[2] >> 1) | (pull[3] << 63);
				pull[3] >>= 1;
				if (0 == ++pull[0])if (0 == ++pull[1])if (0 == ++pull[2])++pull[3];
				return;
			}
			pull[0] = (pull[0] >> 1) | (pull[1] << 63);
			pull[1] = (pull[1] >> 1) | (pull[2] << 63);
			pull[2] = (pull[2] >> 1) | (pull[3] << 63);
			pull[3] >>= 1;
		}
		unsigned long Float256::BitsMultiply(FloatData256 &dat, FloatData256 &desdat, unsigned long long& num)
		{
			union { unsigned long long ull; unsigned int ui[2]; }tmp;

			tmp.ull = dat.ui[0] * num;
			unsigned long t = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[1] * num;
			desdat.ui[0] = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[2] * num;
			desdat.ui[1] = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[3] * num;
			desdat.ui[2] = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[4] * num;
			desdat.ui[3] = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[5] * num;
			desdat.ui[4] = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[6] * num;
			desdat.ui[5] = tmp.ui[0];
			tmp.ull = tmp.ui[1] + dat.ui[7] * num;
			desdat.ull[3] = tmp.ull;
			return t;
		}
		Float256::Float256(double number)//计算100万次Float(1.1)用时0.018秒
		{//验证通过
			union {
				unsigned long long num;
				double dn;
			};
			dn = number;
			if (number != 0)
			{
				Exp = ((0x7ff0000000000000 & num) >> 52) - 1023;
				Data.ull[3] = num << 11 | 0x8000000000000000;
				if (number > 0)
					Sign = 1;
				else
					Sign = -1;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(float number)
		{
			union {
				u32 num;
				float dn;
			};
			dn = number;
			if (num != 0)
			{
				Exp = (s64)((0x7f800000 & num) >> 23)-127;
				Data.ull[3] = ((u64)num << 41) | 0x8000000000000000;
				if (number > 0)
					Sign = 1;
				else
					Sign = -1;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(long long number)
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
				Exp = 63 - count;
				Data.ull[3] = number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(unsigned long long number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount64(number);
				Exp = 63 - count;
				Data.ull[3] = number << count;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(long number)
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
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)(number)) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(unsigned long number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)(number)) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(int number)
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
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = number << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(unsigned int number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = number << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(short number)
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
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)number) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(unsigned short number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)(number)) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(signed char number)
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
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)(number)) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(unsigned char number)
		{
			if (number != 0)
			{
				Sign = 1;
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)(number)) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(char number)
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
				index_t count = Bits::LeftZeroCount32(number);
				Exp = 31 - count;
				Data.ui[7] = ((unsigned long)(number)) << count;
				Data.ui[6] = 0;
			}
			else
			{
				Exp = 0;
				Sign = 0;
				Data.ull[3] = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(bool number)
			:Exp(0)
		{
			if (number)
			{
				Data.ull[3] = MinValues::Int64;
				Sign = 1;
			}
			else
			{
				Data.ull[3] = 0;
				Sign = 0;
			}
			Data.ull[0] = Data.ull[1] = Data.ull[2] = 0;
		}
		Float256::Float256(char const* str)
		{
			Sign = 0;
			Exp = 0;
			long long i = 0, j = 0;//循环用数
			long long num_exp = 0;//指数部分
			long long position_point = 0;//小数点位置
			long long position_e = 0;//e的位置
			//清空内容
			Data.Clear();
			long long len = 0;
			index_t Length = 0;
			std::string numstr;
			if (str == NULL)goto End;
			Length = Strings::strlen(str);
			for (len = 0; len < Length; ++len)
			{
				//如果首位是逗号或者遇到连续两个符号将退出循环
				if (len && (str[len] == ',' || str[len] == '.'))//点号和逗号前不能出现点号或逗号
				{
					if (str[len - 1] == ',' || str[len - 1] == '.')
						break;
					continue;
				}
				if (len && (str[len] & 0xffdf) == 'E' && (str[len - 1] & 0xffdf) == 'E')//两个e
					break;
				if (len && (str[len] == '+' || str[len] == '-'))//正负号只能出现在开头或e后面
				{
					if ((str[len - 1] & 0xffdf) != 'E')
						break;
					continue;
				}
				if (str[len] <= '9' && str[len] >= '0')//数字字符
					continue;
				if ((str[len] & 0xffdf) >= 'A' && (str[len] & 0xffdf) <= 'Z')//字母
					continue;
				if (str[len] == '.' || str[len] == '+' || str[len] == '-' || str[len] == '_')//其他符号
					continue;
				break;//不支持符号，退出
			}
			if (!len)goto End;//空内容直接为0
			numstr.resize(len);
			memcpy(&numstr[0], str, size_t(len * sizeof(char)));
			numstr[len] = 0;
			//通过首字初步判断是否属于特殊转换
			if (numstr[0] == '_' || (((numstr[0]) & 0xffdf) >= 'A' && ((numstr[0]) & 0xffdf) <= 'Z'))//以字母开头将进入特殊转换
			{
				if (numstr=="min" || numstr=="max")
				{
					Exp = 0x007fffffffffffff;
					if (numstr=="min")Sign = -1; else Sign = 1;
					Data.ull[3] |= MinValues::Int64;
					goto End;
				}
				if (numstr=="ln2")
				{
					Exp = -1;	Sign = 1;	Data.ull[0] = 0x8a0d175b8baafa2c; Data.ull[1] = 0x40f343267298b62d; Data.ull[2] = 0xc9e3b39803f2f6af; Data.ull[3] = 0xb17217f7d1cf79ab;
					goto End;
				}
				if (numstr=="ln10")
				{
					Exp = 1;	Sign = 1;	Data.ull[0] = 0x83c61e8201f02d78; Data.ull[1] = 0xe28fecf9da5df90e; Data.ull[2] = 0xea56d62b82d30a28; Data.ull[3] = 0x935d8dddaaa8ac16;
					goto End;
				}
				//e = "2.7182818284590452353602874713526624977572470936999595749669676277240766303536";
				if (numstr=="e")
				{
					Exp = 1; Sign = 1;	Data.ull[0] = 0xa9e13641146433fc; Data.ull[1] = 0xd8b9c583ce2d3695; Data.ull[2] = 0xafdc5620273d3cf1; Data.ull[3] = 0xadf85458a2bb4a9a;
					goto End;
				}
				//pi = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862";
				if (numstr =="pi")
				{
					Exp = 1; Sign = 1;	Data.ull[0] = 0x20bbea63b139b23; Data.ull[1] = 0x29024e088a67cc74; Data.ull[2] = 0xc4c6628b80dc1cd1; Data.ull[3] = 0xc90fdaa22168c234;
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
				switch (numstr[i])
				{
				case '1': ++ *this; break;
				case '2': *this += Two(); break;
				case '3': *this += Three(); break;
				case '4': *this += Four(); break;
				case '5': *this += Five(); break;
				case '6': *this += Six(); break;
				case '7': *this += Seven(); break;
				case '8': *this += Eight(); break;
				case '9': *this += Nine();
				default:break;
				}
				if (1 + i < position_e)
					*this = Multiply10();//快速乘10
			}
			//4.计算e后面的数值
			for (i = position_e + 1; i < len; ++i)
			{
				//如果遇到非数字字符将视为0
				if (numstr[i] >= '1' && numstr[i] <= '9')
					num_exp += numstr[i] - '0';
				if (1 + i < len)
				{
					if (num_exp > (long long)(MaxValue().Exp*0.03010299956639812))//数值已经溢出
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
				long long texp = 1;
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
				unsigned long long texp = 1;
				int tmov = 0;
				while (texp <= (unsigned long long)num_exp)
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
		End:;
		}
		Float256::Float256(wchar_t const* str) { *this = Float256(Strings::ToString(std::wstring(str))); }
		Float256::operator double()const
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
				num.ull |= (Data.ull[3] & MaxValues::Int64) >> 11;//尾数
				if (Data.ui[6] & 0x400)num.ull++;//类似十进制的四舍五入
			}
			return num.number;
		}
		Float256::operator float()const { return float(operator double()); }
		Float256::operator long long()const
		{
			if (Exp > 62)
			{
				if (Sign < 0)
					return MinValues::Int64;
				return MaxValues::Int64;
			}
			if (Exp < 0)
				return 0;
			unsigned long long dat = Data.ull[3];
			dat >>= 63 - Exp;
			if (Sign < 0)
				return -(long long)dat;
			return dat;
		}
		Float256::operator unsigned long long()const
		{
			if (Sign < 0)return (unsigned long long)operator long long();
			if (Exp > 63)
			{
				return MaxValues::UInt64;
			}
			if (Exp < 0)
				return 0;
			return Data.ull[3] >> (63 - Exp);
		}
		Float256::operator long()const
		{
			if (Exp > 30)
			{
				if (Sign < 0)
					return MinValues::Int32;
				return MaxValues::Int32;
			}
			if (Exp < 0)
				return 0;
			unsigned long dat = Data.ui[7];
			dat >>= 31 - Exp;
			if (Sign < 0)return -(long)dat;
			return dat;
		}
		Float256::operator unsigned long()const
		{
			if (Sign < 0)return (unsigned long)operator long();
			if (Exp > 31)return MaxValues::UInt32;
			if (Exp < 0)return 0;
			return Data.ui[7] >> (31 - Exp);
		}
		Float256::operator int()const
		{
			if (Exp > 30)
			{
				if (Sign < 0)return MinValues::Int32;
				return MaxValues::Int32;
			}
			if (Exp < 0)return 0;
			unsigned long dat = Data.ui[7];
			dat >>= 31 - Exp;
			if (Sign < 0)return -(int)dat;
			return dat;
		}
		Float256::operator unsigned int()const
		{
			return (unsigned int)(operator int());
		}
		Float256::operator short()const
		{
			return (short)(operator int());
		}
		Float256::operator unsigned short()const
		{
			return (unsigned short)(operator unsigned int());
		}
		Float256::operator signed char()const
		{
			return (signed char)(operator int());
		}
		Float256::operator unsigned char()const
		{
			return (unsigned char)(operator unsigned int());
		}
		Float256::operator char()const
		{
			return (char)(operator int());
		}
		Float256::operator bool()const
		{
			return !Equals0();
		}
		Float256 Float256::operator-()const
		{
			return Float256(*this).SelfNegative();
		}
		Float256& Float256::SelfNegative()
		{
			Sign = -Sign;
			return *this;
		}
		bool Float256::SelfPlusExp256()
		{
			if (Sign == 0)return false;
			if (Sign > 0)
			{
				if (++Data)
				{
					if (Exp == ExpMaxValue)return true;
					BitsMoveRight(Data.ull);
					Data.ull[3] |= MinValues::Int64;
					++Exp;
				}
				return false;
			}
			--Data;
			if ((long long)Data.ull[3] > 0)
			{
				if (Exp == ExpMinValue)*this = Zero();
				else
				{
					BitsMoveLeft(Data.ull);
					--Exp;
				}
			}
			return false;
		}
		bool Float256::SelfMinusExp256()
		{
			if (Sign == 0)return false;
			if (Sign < 0)
			{
				if (++Data)
				{
					if (Exp == ExpMaxValue)return true;
					BitsMoveRight(Data.ull);
					Data.ull[3] |= MinValues::Int64;
					++Exp;
				}
				return false;
			}
			--Data;
			if ((long long)Data.ull[3]>0)
			{
				if (Exp == ExpMinValue)*this = Zero();
				else
				{
					BitsMoveLeft(Data.ull);
					--Exp;
				}
			}
			return false;
		}
		Float256& Float256::operator++()
		{
			if (IsError())return*this;
			if (Equals0())return *this = One();
			if (Exp >= 256)
			{
				if (Exp == 256) {
					if (Sign > 0) {
						if (++Data) { BitsMoveRight(Data.ull); Exp++; Data.ull[3] |= MinValues::Int64; }
					}
					else { --Data; if ((long long)Data.ull[3] > 0) { BitsMoveLeft(Data.ull); Exp--; } }
				}
				return *this;
			}
			if (Exp <= -256)
			{
				if (Exp == -256) {
					if (Sign > 0) {
						Data.ull[3] = MinValues::Int64;
						Data.ull[2] = Data.ull[1] = 0;
						Data.ull[0] = 1;
						Exp = 0;
					}
					else
					{
						Data.ull[3] = Data.ull[2] = Data.ull[1] = Data.ull[0] = -1;
						Exp = -1;
					}
				}
				return *this;
			}
			if (Sign > 0)
			{
				if (Exp > 0)
				{
					if (Bits::SelfPlusOneRound64(Data.ull, 4, 255 - Exp))
					{
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					unsigned long long flag = Data >>= -Exp;
					if ((long long)flag < 0)++Data;
					if ((long long)Data.ull[3] < 0)
					{
						Data.ull[3] ^= MinValues::Int64;
						BitsMoveRight(Data.ull);
						Exp = 1;
					}
					else Exp = 0;
					Data.ull[3] |= MinValues::Int64;
				}
				return *this;
			}
			if (Exp >= 0)
			{
				Bits::SelfMinusOneRound64(Data.ull, 4, 255 - Exp);
				long long tmp = Data.BitsZeroCount();
				if (tmp == 256)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				long long flag = -Exp;
				unsigned long long low = Data >>= flag;
				Data.ull[3] = (1ull << 63) - Data.ull[3];
				flag = 0;
				if (Data.ull[0] != 0) { Data.ull[0] = -(long long)Data.ull[0]; flag = true; }
				if (flag) { Data.ull[1] = ~Data.ull[1]; }
				else if (Data.ull[1] != 0) { Data.ull[1] = -(long long)Data.ull[1]; flag = true; }
				if (flag) { Data.ull[2] = ~Data.ull[2]; }
				else if (Data.ull[2] != 0) { Data.ull[2] = -(long long)Data.ull[2]; flag = true; }
				if (flag)--Data.ull[3];
				flag = Data.BitsZeroCount();
				if (flag == 256)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] == (u64)-1)
							if (--Data.ull[2] == (u64)-1)
							if (--Data.ull[3] <= MaxValues::Int64)
							{
								BitsMoveLeft(Data.ull);
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
		Float256& Float256::operator--()
		{
			if (IsError())return*this;
			if (Equals0())return (*this = One()).SelfNegative();
			if (Exp >= 256)
			{
				if (Exp == 256) {
					if (Sign < 0) {
						if (++Data) { BitsMoveRight(Data.ull); Exp++; Data.ull[3] |= MinValues::Int64; }
					}
					else { --Data; if ((long long)Data.ull[3] > 0) { BitsMoveLeft(Data.ull); Exp--; } }
				}
				return *this;
			}
			if (Exp <= -256)
			{
				if (Exp == -256) {
					if (Sign < 0) {
						Data.ull[3] = MinValues::Int64;
						Data.ull[2] = Data.ull[1] = 0;
						Data.ull[0] = 1;
						Exp = 0;
					}
					else
					{
						Data.ull[3] = Data.ull[2] = Data.ull[1] = Data.ull[0] = -1;
						Exp = -1;
					}
				}
				return *this;
			}
			if (Sign < 0)
			{
				if (Exp > 0)
				{
					if (Bits::SelfPlusOneRound64(Data.ull, 4, 255 - Exp))
					{
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					unsigned long long flag = Data >>= -Exp;
					if ((long long)flag < 0)++Data;
					if ((long long)Data.ull[3] < 0)
					{
						Data.ull[3] ^= MinValues::Int64;
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						Exp = 1;
					}
					else Exp = 0;
				}
				return *this;
			}
			if (Exp >= 0)
			{
				Bits::SelfMinusOneRound64(Data.ull, 4, 255 - Exp);
				long long tmp = Data.BitsZeroCount();
				if (tmp == 256)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				long long flag = -Exp;
				unsigned long long low = Data >>= flag;
				Data.ull[3] = (1ull << 63) - Data.ull[3];
				flag = 0;
				if (Data.ull[0] != 0) { Data.ull[0] = -(long long)Data.ull[0]; flag = true; }
				if (flag) { Data.ull[1] = ~Data.ull[1]; }
				else if (Data.ull[1] != 0) { Data.ull[1] = -(long long)Data.ull[1]; flag = true; }
				if (flag) { Data.ull[2] = ~Data.ull[2]; }
				else if (Data.ull[2] != 0) { Data.ull[2] = -(long long)Data.ull[2]; flag = true; }
				if (flag)--Data.ull[3];

				flag = Data.BitsZeroCount();
				if (flag == 256)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] == (u64)-1)
							if (--Data.ull[2] == (u64)-1)
							if (--Data.ull[3] <= MaxValues::Int64)
							{
								BitsMoveLeft(Data.ull);
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
		Float256 Float256::operator++(int) { Float256 tmp(*this); ++(*this); return tmp; }
		Float256 Float256::operator--(int) { Float256 tmp(*this); --(*this); return tmp; }
		Float256& Float256::operator+=(Float256 const& number)
		{
			return *this = *this + number;
		}
		Float256& Float256::operator+=(unsigned long long num)
		{
			if (IsError() || num == 0)return*this;
			if (Equals0())return *this = num;
			long long e = 63 ^ Bits::LeftZeroCount(num);
			if (Exp >= e + 256)
			{
				if (Exp == e + 256) {
					if (Sign > 0) {
						if (++Data) { BitsMoveRight(Data.ull); Exp++; Data.ull[3] |= MinValues::Int64; }
					}
					else { --Data;	if ((long long)Data.ull[3] > 0) { BitsMoveLeft(Data.ull); Exp--; } }
				}
				return *this;
			}
			if (Exp <= e - 256)
			{
				Data.ull[3] = num << (63 ^ e);
				Data.ull[2] = Data.ull[1] = Data.ull[0] = 0;
				if (Exp == e - 256) {
					Exp = e;
					if (Sign > 0)
						Data.ull[0] = 1;
					else {
						--Data; if ((long long)Data.ull[3] > 0) {
							BitsMoveLeft(Data.ull);
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
					if (Bits::SelfPlusIntRound64(Data.ull, 4, num, 255 - Exp))
					{
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					unsigned long long flag = Data >>= e - Exp;
					if ((long long)flag < 0)++Data;
					if (Bits::SelfPlusIntRound64(Data.ull, 4, num, 255 - e))
					{
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						Exp = e + 1;;
					}
					else Exp = e;
				}
				return *this;
			}
			if (Exp > e || (Exp == e && Data.ull[3] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(Data.ull, 4, num, 255 - Exp);
				long long tmp = Data.BitsZeroCount();
				if (tmp == 256)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				long long flag = e - Exp;
				unsigned long long low = Data >>= flag;
				Data.ull[3] = (num << (63 ^ e)) - Data.ull[3];
				flag = 0;
				if (Data.ull[0] != 0) { Data.ull[0] = -(long long)Data.ull[0]; flag = true; }
				if (flag) { Data.ull[1] = ~Data.ull[1]; }
				else if (Data.ull[1] != 0) { Data.ull[1] = -(long long)Data.ull[1]; flag = true; }
				if (flag) { Data.ull[2] = ~Data.ull[2]; }
				else if (Data.ull[2] != 0) { Data.ull[2] = -(long long)Data.ull[2]; flag = true; }
				if (flag)--Data.ull[3];
				flag = Data.BitsZeroCount();
				if (flag == 256)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] == (u64)-1)
							if (--Data.ull[2] == (u64)-1)
							if ((long long)--Data.ull[3] >0)
							{
								BitsMoveLeft(Data.ull);
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
		Float256 Float256::operator+(Float256 number)const
		{
			Float256 t = *this;
			if (t.Equals0() || number.IsError())return number;//进行错误传递
			if (number.Equals0() || t.IsError())return t;//进行错误传递
			unsigned long long low = 0;
			if (number.Exp != t.Exp) {//指数位不同
				if (number.Exp < t.Exp)BigObject::Swap(t, number);//使得number的绝对值总是大于t
				if (number.Exp - t.Exp > 255) {
					if (number.Exp - t.Exp == 256) {
						if (number.Sign == t.Sign) {
							if (++number.Data) {
								if (number.Exp == Float256::ExpMaxValue)
									return number.MakeError(MathError::PlusExpOverflow);
								++number.Exp;
								number.Data.ull[3] = 0x8000000000000000;
							}
						}
						else {
							--number.Data;
							if (number.Data.ull[3] < 0x8000000000000000) {
								if (number.Exp == Float256::ExpMinValue)
									return Float256::Zero();
								--number.Exp;
								Float256::BitsMoveLeft(number.Data.ull);
							}
						}
					}
					return number;
				}
				low = t.Data >>= number.Exp - t.Exp;
			}

			if (number.Sign == t.Sign) {//符号位相同
				if ((number.Data += t.Data) || ((long long)low < 0 && ++number.Data)) {
					if (number.Exp == Float256::ExpMaxValue)
						return number.MakeError(MathError::PlusExpOverflow);
					number.Exp++;
					bool t = number.Data.ull[0] & 1;
					Float256::BitsMoveRight(number.Data.ull);
					if (t&&++number.Data&&number.Exp++ == Float256::ExpMaxValue)return number.MakeError(MathError::PlusExpOverflow);
					number.Data.ull[3] |= 0x8000000000000000;
				}
				return number;
			}
			if (number.Data < t.Data)BigObject::Swap(number, t);
			number.Data -= t.Data;
			long long mov = number.Data.BitsZeroCount();
			if (mov == 256 || (number.Exp - mov) < Float256::ExpMinValue)return Float256::Zero();
			if (mov)
			{
				number.Data <<= mov;
				Bits::SelfMinusInt64(number.Data.ull, 4, low, mov - 64);
				if ((long long)number.Data.ull[3] > 0)
				{
					++mov;
					if (mov == 256 || (number.Exp - mov) < Float256::ExpMinValue)return Float256::Zero();
					Float256::BitsMoveLeft(number.Data.ull);
				}
				number.Exp -= mov;
			}
			return number;
		}
		Float256 Float256::operator+(unsigned long long num)const
		{
			if (IsError() || num == 0)return*this;
			long long e = 63 ^ Bits::LeftZeroCount(num);
			Float256 result(*this);
			if (result.Exp >= e + 256)
			{
				if (result.Exp == e + 256) {
					if (Sign > 0) { if (++result.Data) { BitsMoveRight(result.Data.ull); result.Exp++; result.Data.ull[3] |= MinValues::Int64; } }
					else { --result.Data;	if ((long long)Data.ull[3] > 0) { BitsMoveLeft(result.Data.ull); result.Exp--; } }
				}
				return result;
			}
			if (result.Exp <= e - 256)
			{
				result.Data.ull[3] = num << (63 ^ e);
				result.Data.ull[2] = result.Data.ull[1] = result.Data.ull[0] = 0;
				if (result.Exp == e - 256) {
					result.Exp = e;
					if (Sign > 0)
						result.Data.ull[0] = 1;
					else {
						--result.Data; if ((long long)result.Data.ull[3] > 0) {
							BitsMoveLeft(result.Data.ull);
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
					if (Bits::SelfPlusIntRound64(result.Data.ull, 4, num, 255 - result.Exp))
					{
						BitsMoveRight(result.Data.ull);
						result.Data.ull[3] |= MinValues::Int64;
						++result.Exp;
					}
				}
				else
				{
					unsigned long long flag = result.Data >>= e - Exp;
					if ((long long)flag < 0)++result.Data;
					if (Bits::SelfPlusIntRound64(result.Data.ull, 4, num, 255 - e))
					{
						BitsMoveRight(result.Data.ull);
						result.Data.ull[3] |= MinValues::Int64;
						result.Exp = e + 1;;
					}
					else result.Exp = e;
				}
				return result;
			}
			if (result.Exp > e || (result.Exp == e && result.Data.ull[3] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(result.Data.ull, 4, num, 255 - Exp);
				long long tmp = result.Data.BitsZeroCount();
				if (tmp == 256)return Zero();
				result.Exp -= tmp;
				result.Data <<= tmp;
			}
			else
			{
				long long flag = e - result.Exp;
				unsigned long long low = result.Data >>= flag;
				result.Data.ull[3] = (num << (63 ^ e)) - result.Data.ull[3];
				flag = 0;
				if (result.Data.ull[0] != 0) { result.Data.ull[0] = -(long long)result.Data.ull[0]; flag = true; }
				if (flag) { result.Data.ull[1] = ~result.Data.ull[1]; }
				else if (result.Data.ull[1] != 0) { result.Data.ull[1] = -(long long)result.Data.ull[1]; flag = true; }
				if (flag) { result.Data.ull[2] = ~result.Data.ull[2]; }
				else if (result.Data.ull[2] != 0) { result.Data.ull[2] = -(long long)result.Data.ull[2]; flag = true; }
				if (flag)--result.Data.ull[3];
				flag = result.Data.BitsZeroCount();
				if (flag == 256)return Zero();
				if (flag)
				{
					result.Data <<= flag;
					low >>= 64 - flag;
					if (result.Data.ull[0] < low)
					{
						result.Data.ull[0] -= low;
						if (--result.Data.ull[1] == (u64)-1)
							if (--result.Data.ull[2] == (u64)-1)
							if ((long long)--result.Data.ull[3] >0)
							{
								BitsMoveLeft(result.Data.ull);
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

		//减法重载
		Float256& Float256::operator-=(unsigned long long num)
		{
			if (IsError() || num == 0)return*this;
			if (Equals0())return (*this = num).SelfNegative();
			long long e = 63 ^ Bits::LeftZeroCount(num);
			if (Exp >= e + 256)
			{
				if (Exp == e + 256) {
					if (Sign < 0) {
						if (++Data) { BitsMoveRight(Data.ull); Exp++; Data.ull[3] |= MinValues::Int64; }
					}
					else { --Data; if ((long long)Data.ull[3] > 0) { BitsMoveLeft(Data.ull); Exp--; } }
				}
				return *this;
			}
			if (Exp <= e - 256)
			{
				Data.ull[3] = num << (63 ^ e);
				Data.ull[2] = Data.ull[1] = Data.ull[0] = 0;
				if (Exp == e - 256) {
					Exp = e;
					if (Sign < 0)
						Data.ull[0] = 1;
					else
					{
						--Data;
						if ((long long)Data.ull[3]>0) {
							BitsMoveLeft(Data.ull);
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
					if (Bits::SelfPlusIntRound64(Data.ull, 4, num, 255 - Exp))
					{
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						++Exp;
					}
				}
				else
				{
					unsigned long long flag = Data >>= e - Exp;
					if ((long long)flag < 0)++Data;
					if (Bits::SelfPlusIntRound64(Data.ull, 4, num, 255 - e))
					{
						BitsMoveRight(Data.ull);
						Data.ull[3] |= MinValues::Int64;
						Exp = e + 1;;
					}
					else Exp = e;
				}
				return *this;
			}
			if (Exp > e || (Exp == e && Data.ull[3] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(Data.ull, 4, num, 255 - Exp);
				long long tmp = Data.BitsZeroCount();
				if (tmp == 256)return *this = Zero();
				Exp -= tmp;
				Data <<= tmp;
			}
			else
			{
				long long flag = e - Exp;
				unsigned long long low = Data >>= flag;
				Data.ull[3] = (num << (63 ^ e)) - Data.ull[3];
				flag = 0;
				if (Data.ull[0] != 0) { Data.ull[0] = -(long long)Data.ull[0]; flag = true; }
				if (flag) { Data.ull[1] = ~Data.ull[1]; }
				else if (Data.ull[1] != 0) { Data.ull[1] = -(long long)Data.ull[1]; flag = true; }
				if (flag) { Data.ull[2] = ~Data.ull[2]; }
				else if (Data.ull[2] != 0) { Data.ull[2] = -(long long)Data.ull[2]; flag = true; }
				if (flag)--Data.ull[3];
				flag = Data.BitsZeroCount();
				if (flag == 256)return *this = Zero();
				if (flag)
				{
					Data <<= flag;
					low >>= 64 - flag;
					if (Data.ull[0] < low)
					{
						Data.ull[0] -= low;
						if (--Data.ull[1] == (u64)-1)
							if (--Data.ull[2] == (u64)-1)
							if (--Data.ull[3] <= MaxValues::Int64)
							{
								BitsMoveLeft(Data.ull);
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
		Float256& Float256::operator-=(Float256 const& number)
		{
			return *this = *this - number;
		}
		Float256 Float256::operator-(Float256 num2)const
		{
			Float256 num1 = *this;
			if (num1.IsError())return num1;
			if (num2.IsError())return num2;
			Float256 tmp(num1 + (-num2));
			if (tmp.IsError())return tmp.MakeError(MathError::MinusExpOverflow);
			return tmp;
		}
		Float256 Float256::operator-(unsigned long long num)const
		{
			if (IsError() || num == 0)return*this;
			long long e = 63 ^ Bits::LeftZeroCount(num);
			Float256 result(*this);
			if (result.Exp >= e + 256)
			{
				if (result.Exp == e + 256) {
					if (result.Sign < 0) {
						if (++result.Data) { BitsMoveRight(result.Data.ull); result.Exp++; result.Data.ull[3] |= MinValues::Int64; }
					}
					else { --result.Data; if ((long long)result.Data.ull[3] > 0) { BitsMoveLeft(result.Data.ull); result.Exp--; } }
				}
				return result;
			}
			if (result.Exp <= e - 256)
			{
				result.Data.ull[3] = num << (63 ^ e);
				result.Data.ull[2] = result.Data.ull[1] = result.Data.ull[0] = 0;
				if (result.Exp == e - 256) {
					result.Exp = e;
					if (result.Sign < 0)
						result.Data.ull[0] = 1;
					else
					{
						--result.Data;
						if ((long long)result.Data.ull[3]>0) {
							BitsMoveLeft(result.Data.ull);
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
					if (Bits::SelfPlusIntRound64(result.Data.ull, 4, num, 255 - result.Exp))
					{
						BitsMoveRight(result.Data.ull);
						result.Data.ull[3] |= MinValues::Int64;
						++result.Exp;
					}
				}
				else
				{
					unsigned long long flag = result.Data >>= e - result.Exp;
					if ((long long)flag < 0)++result.Data;
					if (Bits::SelfPlusIntRound64(result.Data.ull, 4, num, 255 - e))
					{
						BitsMoveRight(result.Data.ull);
						result.Data.ull[3] |= MinValues::Int64;
						result.Exp = e + 1;;
					}
					else result.Exp = e;
				}
				return result;
			}
			if (result.Exp > e || (result.Exp == e && result.Data.ull[3] >= (num << (63 ^ e))))
			{
				Bits::SelfMinusIntRound64(result.Data.ull, 4, num, 255 - result.Exp);
				long long tmp = result.Data.BitsZeroCount();
				if (tmp == 256)return Zero();
				result.Exp -= tmp;
				result.Data <<= tmp;
			}
			else
			{
				long long flag = e - result.Exp;
				unsigned long long low = result.Data >>= flag;
				result.Data.ull[3] = (num << (63 ^ e)) - result.Data.ull[3];
				flag = 0;
				if (result.Data.ull[0] != 0) { result.Data.ull[0] = -(long long)result.Data.ull[0]; flag = true; }
				if (flag) { result.Data.ull[1] = ~result.Data.ull[1]; }
				else if (Data.ull[1] != 0) { result.Data.ull[1] = -(long long)result.Data.ull[1]; flag = true; }
				if (flag) { result.Data.ull[2] = ~result.Data.ull[2]; }
				else if (result.Data.ull[2] != 0) { result.Data.ull[2] = -(long long)result.Data.ull[2]; flag = true; }
				if (flag)--result.Data.ull[3];
				flag = result.Data.BitsZeroCount();
				if (flag == 256)return Zero();
				if (flag)
				{
					result.Data <<= flag;
					low >>= 64 - flag;
					if (result.Data.ull[0] < low)
					{
						result.Data.ull[0] -= low;
						if (--result.Data.ull[1] == (u64)-1)
							if (--result.Data.ull[2] == (u64)-1)
							if (--result.Data.ull[3] <= MaxValues::Int64)
							{
								BitsMoveLeft(result.Data.ull);
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
		//乘法重载
		Float256 Float256::Multiply(Float256 number)const//标准乘法
		{//验证通过
			if (IsError())return *this;
			if (number.IsError())return number;
			if (Equals0() || number.Equals0())return Zero();
			Float256 num;
			long long mov, i, j, s, k;

			union { unsigned long long ull; unsigned int ui[2]; } tmp,
				sum[15],//乘积低32位
				usum[15];//乘积高32位

			for (i = 14; i >= 0; i--)//由14到0求和。
			{
				s = 14 - i;//下标和
				sum[i].ull = usum[i].ull = 0;//置零
				j = k = 7 > s ? s : 7;
				while (s <= k + j)//分段求和
				{
					tmp.ull = ((unsigned long long) Data.ui[j]) * number.Data.ui[s - j];
					sum[i].ull += (unsigned long long)tmp.ui[0];//低32位
					usum[i].ull += (unsigned long long)tmp.ui[1];//高32位
					--j;
				}
				usum[i].ull += sum[i].ui[1];
				if (i < 14)
				{
					usum[1 + i].ull += sum[i].ui[0];
					usum[i].ull += usum[1 + i].ui[1];
				}
			}
			num.Data.ui[0] = usum[7].ui[0];
			num.Data.ui[1] = usum[6].ui[0];
			num.Data.ui[2] = usum[5].ui[0];
			num.Data.ui[3] = usum[4].ui[0];
			num.Data.ui[4] = usum[3].ui[0];
			num.Data.ui[5] = usum[2].ui[0];
			num.Data.ui[6] = usum[1].ui[0];
			num.Data.ui[7] = usum[0].ui[0];

			mov = usum[0].ui[0] >> 31;//为0时须左移,为1时有进位
			if (mov + Exp + number.Exp < ExpMinValue)//乘积太小
				return Zero();
			if (mov + Exp + number.Exp > ExpMaxValue)//乘积溢出错误
			{
				return number.MakeError(MathError::MultiplyExpOverflow);
			}
			num.Exp = Exp + number.Exp + mov;
			if (!mov)
			{
				BitsMoveLeft(num.Data.ull);
				num.Data.ui[0] |= usum[8].ui[0] >> 31;//补上
				if (usum[8].ui[0] & 0x40000000)//精度校正
					if (++num.Data)
					{
						if (num.Exp == ExpMaxValue)return num.MakeError(MathError::MultiplyExpOverflow);
						++num.Exp;
						num.Data.ull[3] |= MinValues::LongLong;
					}
			}
			else if (usum[8].ui[0] >> 31)//精度校正
				if (++num.Data)
				{
					if (num.Exp == ExpMaxValue)return num.MakeError(MathError::MultiplyExpOverflow);
					++num.Exp;
					num.Data.ull[3] |= MinValues::LongLong;
				}
			num.Sign = Sign == number.Sign ? 1 : -1;
			return num;
		}
		Float256 Float256::Multiply10()const//快速乘10,计算100万次1.1.Multiply10()用时0.1010秒
		{
			return Float256(*this).SelfMultiply10();
		}
		Float256& Float256::SelfMultiply10()
		{
			if (IsError() || Equals0())return *this;
			if (Exp + 3 <= ExpMaxValue)
				Exp += 3;//乘8
			else
				return MakeError(MathError::MultiplyExpOverflow);
			FloatData256 fd = Data;
			fd >>= 2;
			if (Data.ull[0] & 0x2)++fd;
			if (Data += fd)
			{
				if (Exp == ExpMaxValue)
					return MakeError(MathError::MinusExpOverflow);
				Exp++;
				BitsMoveRight(Data.ull);
				Data.ull[3] |= MinValues::Int64;
			}
			return *this;
		}
		Float256& Float256::operator*=(Float256 const& number)
		{
			return *this = *this*number;
		}
		Float256 Float256::operator*(Float256 number)const
		{//试验通过
			Float256 num;
			Float256 t = *this;
			if (t.IsError())return t;
			if (number.IsError())return number;
			if (t.Equals0() || number.Equals0())return Float256::Zero();
			long long mov, i, j, s, k;

			union { unsigned long long ull; unsigned int ui[2]; }
			tmp,
				sum[9],//乘积低32位
				usum[9];//乘积高32位

			for (i = 8; i >= 0; i--)//由7到0求和
			{
				s = 14 - i;//下标和
				sum[i].ull = usum[i].ull = 0;//置零
				j = k = 7 > s ? s : 7;
				while (s <= k + j)//分段求和
				{
					tmp.ull = ((unsigned long long) t.Data.ui[j]) * number.Data.ui[s - j];
					sum[i].ull += (unsigned long long)tmp.ui[0];//低32位
					usum[i].ull += (unsigned long long)tmp.ui[1];//高32位
					--j;
				}
				usum[i].ull += sum[i].ui[1];
				if (i < 8)
				{
					usum[1 + i].ull += sum[i].ui[0];
					usum[i].ull += usum[1 + i].ui[1];
				}
			}
			num.Data.ui[0] = usum[7].ui[0];
			num.Data.ui[1] = usum[6].ui[0];
			num.Data.ui[2] = usum[5].ui[0];
			num.Data.ui[3] = usum[4].ui[0];
			num.Data.ui[4] = usum[3].ui[0];
			num.Data.ui[5] = usum[2].ui[0];
			num.Data.ui[6] = usum[1].ui[0];
			num.Data.ui[7] = usum[0].ui[0];

			mov = usum[0].ui[0] > (unsigned int)MaxValues::Int32;//为0时须左移,为1时有进位
			if (mov + t.Exp + number.Exp < Float256::ExpMinValue)//乘积太小
				return Float256::Zero();
			if (mov + t.Exp + number.Exp > Float256::ExpMaxValue)//乘积溢出错误
				return number.MakeError(MathError::MultiplyExpOverflow);
			num.Exp = t.Exp + number.Exp + mov;
			if (!mov) {
				Float256::BitsMoveLeft(num.Data.ull);
				if (usum[8].ui[0] > (unsigned long)MaxValues::Int32)num.Data.ui[0] |= 1;//补上
				if ((usum[8].ui[0] & 0x40000000) > 0)//精度校正
					if (++num.Data) {
						if (num.Exp == Float256::ExpMaxValue)return num.MakeError(MathError::MultiplyExpOverflow);
						++num.Exp;
						num.Data.ull[3] |= MinValues::Int64;
					}
			}
			else
				if (usum[8].ui[0] > (unsigned long)(MaxValues::Int32))//精度校正
				if (++num.Data) {
					if (num.Exp == Float256::ExpMaxValue)return num.MakeError(MathError::MultiplyExpOverflow);
					++num.Exp;
					num.Data.ull[3] |= MinValues::Int64;
				}
			num.Sign = (t.Sign == number.Sign) ? 1 : -1;
			return num;
		}
		Float256 Float256::operator*(double number)const
		{//试验通过
			Float256 t(*this);
			if (t.IsError())return t;
			if (t.Equals0() || number == 0)
				return Zero();
			Float256 num;
			Float256 n(number);
			index_t mov, i, j, s, k;

			union { unsigned long long ull; unsigned int ui[2]; }
			tmp,
				sum[9],//乘积低32位
				usum[9];//乘积高32位

			for (i = 8; i >= 0; i--)//由7到0求和
			{
				s = 14 - i;//下标和
				sum[i].ull = usum[i].ull = 0;//置零
				for (j = k = 7 > s ? s : 7; s <= k + j && j > 5; j--)//分段求和
				{
					tmp.ull = ((unsigned long long) t.Data.ui[s - j]) * n.Data.ui[j];
					sum[i].ull += (unsigned long long)tmp.ui[0];//低32位
					usum[i].ull += (unsigned long long)tmp.ui[1];//高32位
				}
				usum[i].ull += sum[i].ui[1];
				if (i != 8)
				{
					usum[1 + i].ull += sum[i].ui[0];
					usum[i].ull += usum[1 + i].ui[1];
				}
			}
			num.Data.ui[0] = usum[7].ui[0];
			num.Data.ui[1] = usum[6].ui[0];
			num.Data.ui[2] = usum[5].ui[0];
			num.Data.ui[3] = usum[4].ui[0];
			num.Data.ui[4] = usum[3].ui[0];
			num.Data.ui[5] = usum[2].ui[0];
			num.Data.ui[6] = usum[1].ui[0];
			num.Data.ui[7] = usum[0].ui[0];

			mov = usum[0].ui[0] >> 31;//为0时须左移,为1时有进位
			if (t.Exp + n.Exp + mov < ExpMinValue)//乘积太小
				return Zero();
			if (t.Exp + n.Exp + mov > ExpMaxValue)//乘积溢出,返回无穷大
			{
				if (t.Sign == n.Sign)return MaxValue();
				return MinValue();
			}
			if (t.Exp + n.Exp + mov > ExpMaxValue)
				return num.MakeError(MathError::MultiplyExpOverflow);
			num.Exp = t.Exp + n.Exp + mov;
			if (!mov)
			{
				Float256::BitsMoveLeft(num.Data.ull);
				num.Data.ui[0] |= usum[8].ui[0] >> 31;//补上
				if (usum[8].ui[0] & 0x40000000)//精度校正
					if (++num.Data)
					{
						if (num.Exp == ExpMaxValue)return num.MakeError(MathError::MultiplyExpOverflow);
						++num.Exp;
						num.Data.ull[3] |= MinValues::Int64;
					}
			}
			else if (usum[8].ui[0] > (unsigned long)(MaxValues::Int32))//精度校正
			{
				if (++num.Data)
				{
					if (num.Exp == ExpMaxValue)return num.MakeError(MathError::MultiplyExpOverflow);
					++num.Exp;
					num.Data.ull[3] |= MinValues::Int64;
				}
			}
			num.Sign = (t.Sign == n.Sign) ? 1 : -1;
			return num;
		}
		Float256 Float256::operator*(float number)const
		{
			return *this*double(number);
		}
		Float256 Float256::operator*(long long number)const
		{
			if (number == (long long)MinValues::Int64)return (*this*(unsigned long long)(MinValues::Int64)).SelfNegative();
			return (number > 0) ? *this*((unsigned long long)(number)) : (*this*(unsigned long long)(-number)).SelfNegative();
		}
		Float256 Float256::operator*(unsigned long long number)const
		{//实验通过
			Float256 t(*this);
			if (t.IsError() || t.Equals0())return t;
			if (number <= (unsigned long long)MaxValues::UnsignedLong)return operator*((unsigned long)(number));
			unsigned long long uh = number >> 32;
			unsigned long long ul = (unsigned long)(number);
			number = t.Data.ui[0] * ul;//2^-255
			unsigned long long th = 0, tl = number;
			union { unsigned long long ull; unsigned long ui[2]; } tt;

			for (int i = 0; i < 7; ++i)
			{
				tt.ull = t.Data.ui[i] * uh;
				tl = tt.ui[0] + (tl >> 32) + th;
				th = tt.ui[1];
				tt.ull = t.Data.ui[i + 1] * ul;
				tl += tt.ui[0]; th += tt.ui[1];
				t.Data.ui[i] = (unsigned long)tl;
			}
			th += t.Data.ui[7] * uh + (tl >> 32);
			t.Data.ui[7] = 0;
			u32 bCount = (u32)Bits::LeftZeroCount64(th);
			if (t.Exp + 64 - bCount > ExpMaxValue)return t.MakeError(MathError::MinusExpOverflow);
			t.Exp += 64 - bCount;
			if (t.Data.GetBit(31 - bCount))
			{
				t.Data >>= 32 - bCount;
				t.Data.ull[3] |= th << bCount;
				if (++t.Data)
				{
					if (t.Exp == ExpMaxValue)return t.MakeError(MathError::MultiplyExpOverflow);
					t.Exp++;
					t.Data.ui[7] = MinValues::Long;
				}
				return t;
			}
			t.Data >>= 32 - bCount;
			t.Data.ull[3] |= th << bCount;
			return t;
		}
		Float256 Float256::operator*(long number)const
		{
			if (number == (long)MinValues::Int32)return (*this*(unsigned long)(MinValues::Int32)).SelfNegative();
			return (number > 0) ? *this*((unsigned long)number) : (*this*(unsigned long)(-number)).SelfNegative();
		}
		Float256 Float256::operator*(unsigned long number)const
		{
			Float256 t(*this);
			if (t.IsError() || t.Equals0())return t;
			if (number == 0)return Zero();
			unsigned long long tmp;
			tmp = ((unsigned long long)(t.Data.ui[0]))*number;
			t.Data.ui[0] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[1]))*number;
			t.Data.ui[1] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[2]))*number;
			t.Data.ui[2] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[3]))*number;
			t.Data.ui[3] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[4]))*number;
			t.Data.ui[4] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[5]))*number;
			t.Data.ui[5] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[6]))*number;
			t.Data.ui[6] = ((unsigned long)(tmp));
			tmp >>= 32;
			tmp += ((unsigned long long)(t.Data.ui[7]))*number;
			t.Data.ui[7] = ((unsigned long)(tmp));
			long long mov = Bits::LeftZeroCount64(tmp);
			if (mov > 32)return Zero();
			if (t.Exp + 32 - mov > ExpMaxValue)return t.MakeError(MathError::MinusExpOverflow);
			t.Exp += 32 - mov;
			if (mov < 32 && (t.Data.ui[0] & (0x80000000 >> mov))>0)
			{
				t.Data >>= 32 - mov;
				t.Data.ull[3] |= (tmp & 0xffffffff00000000) << mov;
				++t.Data;
			}
			else
			{
				t.Data >>= 32 - mov;
				t.Data.ull[3] |= (tmp & 0xffffffff00000000) << mov;
			}
			return t;
		}
		Float256 Float256::operator*(int number)const
		{
			return *this*(long)(number);
		}
		Float256 Float256::operator*(unsigned int number)const
		{
			return *this*(unsigned long)(number);
		}
		Float256 Float256::operator*(short number)const
		{
			return *this*long(number);
		}
		Float256 Float256::operator*(unsigned short number)const
		{
			return *this*(unsigned long)(number);
		}
		Float256 Float256::operator*(signed char number)const
		{
			return *this*(long)(number);
		}
		Float256 Float256::operator*(unsigned char number)const
		{
			return *this*(unsigned long)(number);
		}
		Float256 Float256::operator*(char number)const
		{
			return *this*(long)(number);
		}
		Float256 Float256::operator*(bool number)const
		{
			if (IsError() || number)return *this;
			return Zero();
		}
		//除法重载
		Float256 Float256::Divide(Float256 number)const//标准除法，计算100万次1.1/10.1用时13.73秒
		{
			if (IsError())return *this;
			if (number.Equals0())
				return number.MakeError(MathError::Divide0);//若除数等于0则产生错误
			if (Equals0())return Zero();
			long long mov = 0;
			Float256 num;
			num.Data.Clear();
			FloatData256 dat = Data;
			unsigned long long flag = 0;
			if (dat < number.Data)
			{
				flag = dat.ull[3];
				mov = -1;
				BitsMoveLeft(dat.ull);
			}
			for (long long i = 0; i < 256; ++i)//逐位生商
			{
				if (flag > (unsigned long long)(MaxValues::LongLong) || dat >= number.Data)
				{
					dat -= number.Data;
					num.Data.ull[3 ^ (i >> 6)] |= (unsigned long long)1 << (63 ^ (0x3f & i));//12次试验试验发现略快于switch方法
					/*switch(i>>6)
					{
					case 0:num.data.ull[3] |= (unsigned long long)1<<(0x3f^i);break;
					case 1:num.data.ull[2] |= (unsigned long long)1<<(0x7f^i);break;
					case 2:num.data.ull[1] |= (unsigned long long)1<<(0xbf^i);break;
					default:num.data.ull[0] |= (unsigned long long)1<<(0xff^i);
					}*/
				}
				flag = dat.ull[3];
				BitsMoveLeft(dat.ull);
			}
			if (flag > (unsigned long long)(MaxValues::LongLong) || dat >= number.Data)//有进位
				++num.Data;
			num.Sign = Sign == number.Sign ? 1 : -1;
			mov += Exp - number.Exp;
			if (mov > ExpMaxValue)
			{//产生溢出错误
				return num.MakeError(MathError::DivideExpOverflow);
			}
			if (mov < ExpMinValue)
			{//太小，返回0
				return Zero();
			}
			num.Exp = mov;
			return num;
		}
		Float256 Float256::Half()const
		{
			Float256 t(*this);
			if (t.IsError())return t;
			if (t.Exp == ExpMinValue || t.Equals0())return Zero();
			t.Exp--;
			return t;
		}
		Float256& Float256::SelfHalf()
		{
			if (!IsError())
			{
				if (Exp == ExpMinValue || Equals0())*this = Zero();
				else Exp--;
			}
			return *this;
		}
		Float256& Float256::operator/=(Float256 const& number)
		{
			return *this = *this / number;
		}
		Float256 Float256::operator/(Float256 number)const
		{
			Float256 t = *this;
			if (t.IsError())return t;//错误传递
			if (number.IsError())return number;
			if (number.Equals0())//若除数等于0则产生错误
				return t.MakeError(MathError::Divide0);
			if (t.Equals0())return t;
			long long mov = -1;
			Float256 num(Float256::Zero());
			FloatData256 dat = t.Data;
			FloatData256 dat1;
			unsigned long long flag = (unsigned long long)(number.Data.ui[7]) + 1;
			unsigned long long q;
			unsigned long tl = 0;
			int tf;
			for (int i = 7; i >= 0; --i)
			{
				if (dat >= number.Data)
				{
					dat -= number.Data;
					for (int j = i + 1; j < 8; ++j)
						if (++num.Data.ui[j] != 0)break;
				}
				q = dat.ull[3] / flag;
				tl = Float256::BitsMultiply(number.Data, dat1, q);
				dat -= dat1;
				if (tl != 0)
				{
					--dat;
					tf = dat.ui[7];
					dat <<= 32;
					dat.ui[0] = -long(tl);
				}
				else
				{
					tf = dat.ui[7];
					dat <<= 32;
				}
				while (tf != 0)
				{
					q++;
					if (dat -= number.Data)--tf;
				}
				num.Data.ui[i] = (unsigned long)q;
			}
			if (dat >= number.Data)
			{
				dat -= number.Data;
				for (int j = 0; j < 4; ++j)
					if (++num.Data.ull[j] != 0)break;
			}
			if (t.Data >= number.Data)
			{
				mov = 0;
				Float256::BitsMoveRight(num.Data.ull);
				num.Data.ull[3] |= 0x8000000000000000;
			}
			else if (dat.ull[3] >(unsigned long long)(MaxValues::LongLong) || (dat.ull[3] << 1) >= number.Data.ull[3])++num.Data;

			num.Sign = t.Sign == number.Sign ? 1 : -1;
			mov += t.Exp - number.Exp;
			if (mov > Float256::ExpMaxValue)
			{//产生溢出错误
				return num.MakeError(MathError::DivideExpOverflow);
			}
			if (mov < Float256::ExpMinValue)
			{//太小，返回0
				return Float256::Zero();
			}
			num.Exp = mov;
			return num;
		}
		Float256 Float256::operator/(double number) const
		{
			if (number == 0)return Float256().MakeError(MathError::Divide0);
			if (Equals0())return *this;
			union { double d; u64 ull; };
			if (number > 0)
				d = number;
			else d = -number;
			Float256 tmp = operator/((ull & 0x000fffffffffffff) | 0x0010000000000000);
			tmp.Exp -= (index_t)(ull >> 52) - (1023 - 52);
			tmp.Sign = number < 0 ? Sign < 0 ? 1 : -1 : Sign < 0 ? -1 : 1;
			return tmp;
		}
		Float256 Float256::operator/(long long number)const
		{
			if (IsError())return *this;
			if (number == (long long)MinValues::LongLong)return -operator/((unsigned long long)(number));
			return -operator/((unsigned long long)(-number));
		}
		Float256 Float256::operator/(unsigned long long number)const
		{//试验通过，x64下计算100万次平均时间0.6秒
			Float256 t(*this);
			if (t.IsError())return t;
			if (number == 0)return t.MakeError(MathError::Divide0);
			if (t.Equals0())return t;

			Float256 num(Zero());
			FloatData256 dat = t.Data;
			FloatData256 dat1;
			long long mov = Bits::LeftZeroCount64(number);
			number <<= mov;
			mov -= 64;
			unsigned long long flag = (number >> 32) + ((number & 0xffffffff) > 0);
			unsigned long long q;
			for (int i = 7; i >= 0; --i)
			{
				if (dat.ull[3] >= number)
				{
					dat.ull[3] -= number;
					for (int j = i + 1; j < 8; ++j)
						if (++num.Data.ui[j] != 0)break;
				}
				q = dat.ull[3] / flag;
				union { unsigned long long ull; unsigned int ui[2]; }tmp;
				tmp.ull = (number & 0xffffffff) * q;
				dat1.ull[0] = dat1.ull[1] = dat1.ull[2] = 0;
				dat1.ui[5] = tmp.ui[0];
				tmp.ull = tmp.ui[1] + (number >> 32) * q;
				dat1.ull[3] = tmp.ull;
				dat -= dat1;
				unsigned int tf = dat.ui[7];
				dat <<= 32;
				while (tf != 0)
				{
					q++;
					if (dat.ull[3] < number)--tf;
					dat.ull[3] -= number;
				}
				num.Data.ui[i] = (unsigned long)q;
			}
			if (dat.ull[3] >= number)
			{
				dat.ull[3] -= number;
				for (int j = 0; j < 4; ++j)
					if (++num.Data.ull[j] != 0)break;
			}
			if (t.Data.ull[3] >= number)
			{
				mov++;
				BitsMoveRight(num.Data.ull);
				num.Data.ull[3] |= 0x8000000000000000;
			}
			else if (dat.ull[3] >(unsigned long long)(MaxValues::LongLong) || (dat.ull[3] << 1) >= number)++num.Data;
			num.Sign = t.Sign;
			mov += t.Exp;
			if (mov > ExpMaxValue)
			{//产生溢出错误
				return num.MakeError(MathError::DivideExpOverflow);
			}
			if (mov < ExpMinValue)
			{//太小，返回0
				return Zero();
			}
			num.Exp = mov;
			return num;
		}
		Float256 Float256::operator/(long number)const
		{
			if (IsError())return *this;
			if (number == (long)MinValues::Long)return -operator/((unsigned long)(number));
			if (number < 0)return -operator/((unsigned long)(-number));
			return operator/((unsigned long)(number));
		}
		Float256 Float256::operator/(unsigned long number)const
		{//x64下计算100万次平均时间0.6秒
			Float256 t(*this);
			if (t.IsError())return t;
			if (number == 0)return t.MakeError(MathError::Divide0);
			if (t.Equals0())return t;
			long long bCount = Bits::LeftZeroCount64(number) - 32;
			number <<= bCount;
			bCount = 32 - bCount;//bCount为number的exp
			bool te = t.Data.ui[7] >= number;
			unsigned long long tmp;
			tmp = t.Data.ull[3] % number;
			t.Data.ui[7] = (unsigned long)(t.Data.ull[3] / number);
			tmp = (tmp << 32) | (unsigned long)(t.Data.ui[5]);
			t.Data.ui[6] = (unsigned long)(tmp / number);
			tmp %= number;
			tmp = (tmp << 32) | (unsigned long)(t.Data.ui[4]);
			t.Data.ui[5] = (unsigned long)(tmp / number);
			tmp %= number;
			tmp = (tmp << 32) | (unsigned long)(t.Data.ui[3]);
			t.Data.ui[4] = (unsigned long)(tmp / number);
			tmp %= number;
			tmp = (tmp << 32) | (unsigned long)(t.Data.ui[2]);
			t.Data.ui[3] = (unsigned long)(tmp / number);
			tmp %= number;
			tmp = (tmp << 32) | (unsigned long)(t.Data.ui[1]);
			t.Data.ui[2] = (unsigned long)(tmp / number);
			tmp %= number;
			tmp = (tmp << 32) | (unsigned long)(t.Data.ui[0]);
			t.Data.ui[1] = (unsigned long)(tmp / number);
			tmp %= number;
			tmp <<= 32;
			t.Data.ui[0] = (unsigned long)(tmp / number);
			if (t.Exp - bCount + te < ExpMinValue)return Zero();
			t.Exp -= bCount - te;
			if (te)
			{
				BitsMoveRight(t.Data.ull);
				t.Data.ull[3] |= (unsigned long long)(MinValues::LongLong);
			}
			else
			{
				if (tmp%number >= number / 2)
				{
					++t.Data;
				}
			}
			return t;
		}
		Float256 Float256::operator/(int number)const
		{
			return operator/(long(number));
		}
		Float256 Float256::operator/(unsigned int number)const
		{
			return operator/((unsigned long)(number));
		}
		Float256 Float256::operator/(short number)const
		{
			return operator/(long(number));
		}
		Float256 Float256::operator/(unsigned short number)const
		{
			return operator/((unsigned long)(number));
		}
		Float256 Float256::operator/(signed char number)const
		{
			return operator/(long(number));
		}
		Float256 Float256::operator/(unsigned char number)const
		{
			return operator/((unsigned long)(number));
		}
		Float256 Float256::operator/(char number)const
		{
			return operator/(long(number));
		}
		Float256 Float256::operator/(bool number)const
		{
			Float256 tmp(*this);
			if (tmp.IsError() || number)return tmp;
			return tmp.MakeError(MathError::Divide0);
		}
		//逻辑运算符
		bool Float256::operator!()const { return Equals0(); }
		//开平方
		Float256 Float256::Sqrt()const
		{
			Float256 num(*this);
			if (num.Equals0() || num.IsError())return num;
			if (num.Sign < 0)return num.MakeError(MathError::SqrtLessThan0);
			if (num.Equals(One()))return num;
			Float256 num1 = num;
			long long te = num1.Exp&((long long)MaxValues::LongLong ^ 1);
			num1.Exp ^= te;
			num1 = std::sqrt((double)num1);//近似值
			num1.Exp += te >> 1;
			Float256 num2 = num / num1;
			while (!num1.AppEquals(num2))//牛顿迭代
			{
				num1 = num1 + num2;
				num1.Exp--;
				num2 = num / num1;
			}
			return num2;
		}
		//取绝对值
		Float256 Float256::Abs()const
		{
			if (Sign >= 0)return *this;
			return -*this;
		}
		//自身取绝对值
		Float256& Float256::SelfAbs()
		{
			if (IsError() || Sign >= 0)return *this;
			Sign = -Sign;
			return *this;
		}
		//求余(>=0),商保存到quotient里，余数与number同号；number<0时向上取整，number>0时向下取整
		Float256 Float256::Mod(Float256 number, Float256 &quotient)const
		{
			Float256 num(*this);
			if (num.IsError())return num;
			if (number.IsError())return number;
			if (number.Equals0())//若等于0则生成错误
			{
				return number.MakeError(MathError::ModDivide0);
			}
			if (num.Equals0())//若等于0则返回0
			{
				quotient = Zero();
				return Zero();
			}
			if (num.Exp < number.Exp || (num.Exp == number.Exp && num.Data < number.Data))//绝对值比除数的绝对值小
			{
				if (number.Sign == num.Sign)
				{
					quotient = Zero();
					return num;
				}
				quotient = -One();//-1
				return num + number;
			}
			if (num.Exp - number.Exp > 255)//远大于除数,余数将无法确定
			{
				quotient = num / number;
				return Zero();
			}
			long long mov = 0;//提前位移
			long long bmov = num.Exp - number.Exp;//求商位移
			long long flag = 0;
			quotient.Data.Clear();
			FloatData256 dat = num.Data;
			long long i;
			if (dat < number.Data) { flag = 1; mov = 1; BitsMoveLeft(dat.ull); }
			bmov -= mov;
			for (i = 0; i < bmov; ++i)
			{
				if (flag || dat >= number.Data)//生商
				{
					dat -= number.Data;
					switch (i >> 6)
					{
					case 0:quotient.Data.ull[3] |= (unsigned long long)1 << (0x3f ^ i); break;
					case 1:quotient.Data.ull[2] |= (unsigned long long)1 << (0x7f ^ i); break;
					case 2:quotient.Data.ull[1] |= (unsigned long long)1 << (0xbf ^ i); break;
					default:break;
					}
					if (!dat)break;//没得算了
				}
				flag = dat.ull[3] > (unsigned long long)(MaxValues::Int64);
				BitsMoveLeft(dat.ull);//挪一位
			}
			if (i == bmov && (flag || dat >= number.Data))
			{//商最后一位
				dat -= number.Data;
				quotient.Data.ui[7 ^ (i >> 5)] |= 1 << (31 ^ (0x1f & i));
			}
			quotient.Exp = num.Exp - number.Exp - mov;
			quotient.Sign = num.Sign == number.Sign ? 1 : -1;
			if (!dat)//整除
				return Zero();
			//得到的data为按同为正数算出的余数
			num.Data = dat;
			bmov = num.Data.BitsZeroCount();
			num.Exp = num.Exp - i - mov - bmov;
			num.Data <<= bmov;
			if (num.Abs().AppEquals(number.Abs()))
			{
				if (quotient.Sign > 0)++quotient;
				else --quotient;
				return Zero();
			}
			if (Sign != number.Sign)//异号
			{
				num = num + number;
				--quotient;//商为负值
			}
			return num;
		}
		//求余
		Float256 Float256::Mod(Float256 number)const
		{
			Float256 num(*this);
			if (num.IsError())return num;
			if (number.IsError())return number;
			if (number.Equals0())
				return number.MakeError(MathError::ModDivide0);//求余除以0错误
			if (Equals0())return Zero();//若等于0则返回0
			if (num.Exp < number.Exp || (num.Exp == number.Exp && num.Data < number.Data))//绝对值比除数的绝对值小
			{
				if (number.Sign == num.Sign)
					return num;
				return num + number;
			}
			if (num.Exp > number.Exp + 255)//远大于除数,余数将无法确定
				return Zero();
			long long mov = 0;//提前位移
			long long bmov = num.Exp - number.Exp;//求商位移
			long long flag = 0;
			FloatData256 dat = num.Data;
			long long i;
			if (dat < number.Data) { flag = 1; mov = 1; Float256::BitsMoveLeft(dat.ull); }
			bmov -= mov;
			for (i = 0; i < bmov; ++i)
			{
				if (flag || dat >= number.Data)//生商
				{
					dat -= number.Data;
					if (!dat)break;//没得算了
				}
				flag = dat.ull[3] >(unsigned long long)(MaxValues::Int64);
				BitsMoveLeft(dat.ull);//挪一位
			}
			if (i == bmov && (flag || dat >= number.Data))
			{//商最后一位
				dat -= number.Data;
			}
			if (!dat.ull[3])//整除
				return Zero();
			num.Data = dat;
			bmov = num.Data.BitsZeroCount();
			num.Exp = num.Exp - i - mov - bmov;
			num.Data <<= bmov;
			if (num.Abs().AppEquals(number.Abs()))
				return Zero();
			//if(num.Sign != number.Sign)
			//	num += number;
			if (number.Sign == num.Sign)
				return num;
			return num + number;
		}
		//返回该数以number为底的对数
		Float256 Float256::Log(Float256 const& number)const
		{
			return Ln() / number.Ln();
		}
		//返回该数以10为底的对数
		Float256 Float256::Lg()const
		{
			return Ln() / Ln10();
		}
		//返回该数以e为底的对数
		Float256 Float256::Ln()const
		{
			if (IsError())return *this;
			Float256 num_x;//记录x
			Float256 num_xn;//记录x^n
			num_x = *this;
			long long ln2 = Exp;//exp偏移
			Float256 ln_plus = Zero();

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
			/*num_x.Data.ull[3]&=MaxValues::LongLong;
			long long mov = num_x.Data.BitsZeroCount();
			if(mov==256)return ln_plus+ Ln2()*ln2;
			num_x.Exp-=mov;
			num_x.Data<<=mov;

			num_xn = num_x;

			Float256 num_1 = num_x;//初始值
			Float256 num_2 = -One();//循环赋值
			for(int i=2;;i++)//泰勒展开叠加
			{
			num_xn = -num_xn*num_x;
			num_1 += num_xn/i;
			if(num_1.AppEquals(num_2))break;
			num_2 = num_1;
			}*/
			//另一种展开方式：ln(x) = ln((1+y)/(1-y)) = 2y*(1+(y^2)/3+(y^4)/5+(y^6)/7+...)
			//y=(x-1)/(x+1)
			Float256 num_y = (num_x - One()) / (One() + num_x);
			Float256 num_1 = num_y;
			Float256 num_y2 = num_y*num_y;
			Float256 num_yn = num_y*num_y2;
			for (index_t i = 3;; i += 2)
			{
				num_1 += num_yn / i;
				if (num_yn.Equals0() || num_yn.Exp < -256)break;
				num_yn *= num_y2;
			}
			if (!num_1.Equals0())num_1.Exp++;
			num_1 += ln_plus;
			if (ln2)return num_1 + Ln2()*ln2;
			return num_1;
		}
		//返回该数以2为底的对数
		Float256 Float256::Log2()const
		{
			return Ln() / Ln2();
		}
		//返回e的该数次幂
		Float256 Float256::Expe()const
		{
			if (IsError())return *this;
			Float256 num_x, num_xn, num_factorial, num_1, num_2;
			if (Exp > 38)
			{
				if (Sign < 0)
					return Zero();
				num_x = *this;
				return num_x.MakeError(MathError::ExpEOverflow);
			}

			num_x = Mod(Ln2(), num_factorial);
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
		//返回2的该数次幂
		Float256 Float256::Exp2()const
		{
			if (IsError())return *this;
			Float256 num_x, num_xn, num_factorial, num_1, num_2;
			if (Exp >= 55)
			{
				if (Sign < 0)
					return Zero();
				num_x = *this;
				return num_x.MakeError(MathError::Exp2Overflow);
			}

			num_x = GetDecimalPart();
			long long expmov = (long long)GetIntegerPart();
			num_x = num_x*Ln2();
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
				if (num_1.AppEquals(num_2))break;
				num_2 = num_1;
			}
			if (expmov)num_1.Exp += expmov;
			return num_1;
		}
		//返回该数的number次幂
		Float256 Float256::Pow(Float256 const& number)const
		{
			if (IsError())return(*this);
			if (number.IsError())return number;
			if (*this <= Zero())
			{
				Float256 num = *this;
				if (Equals0() && number.Equals0())return num.MakeError(MathError::Pow00);
				if (number.GetDecimalPart().Equals0())//整数次方
				{
					Float256 tmp = (Abs().Log2()*number).Exp2();
					if (tmp.IsError())tmp.MakeError(MathError::PowExpOverflow);
					if (number.Half().GetDecimalPart())return -tmp;
					return tmp;
				}
				return num.MakeError(MathError::PowLessThan0);
			}
			Float256 tmp = (Log2()*number).Exp2();//使用Exp2来算
			if (tmp.IsError())tmp.MakeError(MathError::PowExpOverflow);
			return tmp;
			//return this->Abs().Ln()*(number).Exp();//使用Exp来算
		}
		//取正弦(弧度制)
		Float256 Float256::Sin()const
		{
			if (IsError())return *this;
			Float256 num_x, num_xn, num_factorial, num_1, num_2, sum;
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
			num_factorial = One();//阶乘
			num_xn = num_x;
			sum = num_x;
			num_1 = num_x;
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
					sum += num_xn / num_factorial;
					if (sum.AppEquals(num_1))break;
					num_1 = sum;
				}
			}
			if (negative)return -sum;
			return sum;
		}
		//取正弦(角度制)
		Float256 Float256::Dsin()const
		{
			return ((*this) * _pi_180()).Sin();
		}
		//取余弦(弧度制)
		Float256 Float256::Cos()const
		{
			if (IsError())return *this;
			Float256 num_x, num_xn, num_factorial, num_1, num_2, sum;
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
			num_factorial = One();//阶乘
			num_xn = One();
			sum = One();
			num_1 = One();
			num_2 = num_1;
			num_x *= num_x;//变x方
			int i;
			for (i = 2;; i++)//Cosx = 1-x^2/2!+x^4/4!-
			{
				num_factorial = num_factorial*i;
				if (i & 1)continue;
				num_xn = num_xn*num_x;
				num_factorial.SelfNegative();//变换符号
				sum += num_xn / num_factorial;
				if (num_1.AppEquals(sum))break;
				num_1 = sum;
			}
			if (negative)return -sum;
			return sum;
		}
		//取余弦(角度制)
		Float256 Float256::Dcos()const
		{
			return ((*this)*_pi_180()).Cos();
		}
		//取正切(弧度制)
		Float256 Float256::Tan()const
		{
			if (IsError())return *this;
			Float256 num = Cos();
			if (num.Equals0())return num.MakeError(MathError::Tan);
			return Sin() / num;
		}
		//取正切(角度制)
		Float256 Float256::Dtan()const
		{
			return Dsin() / Dcos();
		}
		//取余切(弧度制)
		Float256 Float256::Cot()const
		{
			if (IsError())return *this;
			Float256 num = Sin();
			if (num.Equals0())return num.MakeError(MathError::Cot);
			return Cos() / num;
		}
		//取余切(角度制)
		Float256 Float256::Dcot()const
		{
			if (IsError())return *this;
			Float256 num = Dsin();
			if (num.Equals0())return num.MakeError(MathError::Cot);
			return Dcos() / num;
		}
		//反正弦(弧度制)
		Float256 Float256::Arcsin()const
		{
			if (IsError())return *this;
			Float256 num_x, num_xn;//x、xn
			Float256 num1, num2;//分子、分母
			Float256 num_1, num_2;//前一项、后一项
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
		//反正弦(角度制)
		Float256 Float256::Darcsin()const
		{
			return Arcsin() / _pi_180();
		}
		//反余弦(弧度制)
		Float256 Float256::Arccos()const
		{
			if (IsError())return *this;
			if (Abs() > One())
			{
				Float256 num = *this;
				return num.MakeError(MathError::Arccos);//出错
			}
			if (*this < _sqrt2_half())return _pi_half() - Arcsin();//小于根号2/2，这样计算可以在接近1时获得较高的精度
			return (One() - *this**this).SelfAbs().Sqrt().Arcsin();//取正值
		}
		//反余弦(角度制)
		Float256 Float256::Darccos()const
		{
			return Arccos() / _pi_180();
		}
		//反正切(弧度制)
		Float256 Float256::Arctan()const
		{
			if (IsError())return *this;
			//配方法：tanx=sinx/cosx，设cosx>0，则sinx = this/sqrt(this平方+1)，转为求arcsin,但此方法在x趋于0时精度大减
			if (Abs() > _sqrt2_half())return (*this / ((*this**this + One()).SelfAbs().Sqrt())).Arcsin();
			//级数法：arctan x = x - x^3/3 + x^5/5 - ... (x≤1) ，x趋于1时收敛太慢
			Float256 num_x, num_xn;//x、xn
			Float256 num = Three();//分母
			Float256 num_1, num_2;//前一项、后一项
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
		//反正切(角度制)
		Float256 Float256::Darctan()const
		{
			return Arctan() / _pi_180();
		}
		//比较是否相等
		bool Float256::Equals(const Float256& number)const
		{
			return Exp == number.Exp && Data == number.Data && Sign == number.Sign;
		}
		//约等于,符号不同返回false.忽略位数末count位，比较是否等于
		bool Float256::AppEquals(const Float256& number, unsigned long long less)const
		{
			if (number.Sign != Sign)
				return false;
			Float256 num1, num2;
			if (Exp > number.Exp)
			{
				if (Exp - 1 > number.Exp)return false;
				num1 = (*this);
				num2 = number;
				BitsMoveRight(num2.Data.ull);
			}
			else if (number.Exp > Exp)
			{
				if (number.Exp - 1 > Exp)return false;
				num2 = (*this);
				num1 = number;
				BitsMoveRight(num2.Data.ull);
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
			for (size_t i = 3; i != 0; --i)if (num1.Data.ull[i] != 0)return false;
			return num1.Data.ull[0] < less;
		}
		//生成[0,1)区间的随机数
		Float256 Float256::Rand()
		{
			Float256 f;
			f.Exp = -1;
			f.Sign = 1;
			f.Data.ull[0] = GetDefaultRand();
			f.Data.ull[1] = GetDefaultRand();
			s64 bitsCount = f.Data.BitsZeroCount();
			if (bitsCount == 256)
				return f = Zero();
			f.Data <<= bitsCount;
			f.Exp -= bitsCount;
			return f;
		}
		//返回整数部分
		Float256 Float256::GetIntegerPart()const
		{
			if (IsError() || Exp >= 255)return *this;
			if (Exp < 0)
				return Zero();
			Float256 num = *this;
			num.Data >>= 255 - Exp;
			num.Data <<= 255 - Exp;
			return num;
		}
		//自身取整(小数部分全舍)
		Float256& Float256::SelfGetIntegerPart()
		{
			if (IsError() || Exp >= 255)return *this;
			if (Exp < 0)
				return *this = Zero();
			Data >>= 255 - Exp;
			Data <<= 255 - Exp;
			return *this;
		}
		//返回小数部分
		Float256 Float256::GetDecimalPart()const
		{
			if (IsError() || Exp < 0)return *this;
			if (Exp >= 255)
				return Zero();
			Float256 num = *this;
			num.Data <<= 1 + Exp;
			long long mov = num.Data.BitsZeroCount();
			if (256 == mov)
				return Zero();
			num.Data <<= mov;
			num.Exp = -1 - mov;
			return num;
		}
		//自身取小数部分
		Float256& Float256::SelfGetDecimalPart()
		{
			if (IsError() || Exp < 0)return *this;
			if (Exp >= 255)
				return *this = Zero();
			Data <<= 1 + Exp;
			long long  mov = Data.BitsZeroCount();
			if (256 == mov) return *this = Zero();
			Data <<= mov;
			Exp = -1 - mov;
			return *this;
		}
		std::string Float256::ToString(int length)const
		{
			long long i = 0;
			std::string tmpstr;//临时字符串
			if (Equals0())
				return "0";
			if (IsError())//错误信息输出
			{
				return MathError::description[-(int)Sign];
			}
			Float256 num = *this;
			num.Data.ull[3] |= MinValues::Int64;
			unsigned char k;
			long long j, position, exp10;
			i = j = position = 0;
			exp10 = -1;
			unsigned char number[80];//最长精度为77,80位已经足够了
			if (length <= 0 || length > 79)length = 76;
			if (Exp >= 0)//加速处理
			{
				unsigned long long texp = 0x20000000000000;
				int exp_mov = 54;
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
			if (Exp <= -332)
			{
				unsigned long long texp = 0x20000000000000;
				int exp_mov = 54;
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
				if (!k && !position){//确保首位非0
					exp10--;//十进制指数减小
					continue;
				}
				else if (10 == k){//避免0.999999事件
					k = 0;
					for (i = position - 1; i >= 0; --i){
						if (9 > number[i]){
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
				tmpstr.resize(size_t(position + 4));
				for (i = 0; i <= position; ++i)
				{
					if (i == 1 + exp10)
					{
						tmpstr[i] = '.';
						++j;
					}
					tmpstr[j] = number[i] + '0';
					++j;
				}
				tmpstr[j] = 0;
			}
			else if (position == exp10)//无需修改
			{
				tmpstr.resize(size_t(position + 3));
				for (i = 0; i <= position; ++i)
					tmpstr[i] = number[i] + '0';
				tmpstr[i] = 0;
			}
			else if (position<exp10 && position + 4>exp10)//末尾补0,+4代表末位最多出现3个0
			{
				tmpstr.resize(size_t(position + 3 + exp10 - position));
				for (i = 0; i <= position; ++i)
					tmpstr[i] = number[i] + '0';
				for (j = 0; j < exp10 - position; ++j)
					tmpstr[i + j] = '0';
				tmpstr[i + j] = 0;
			}
			else if (exp10<0 && exp10>-4)//小数，前面0较少，不用科学计数法
			{
				tmpstr.resize(size_t(position + 4 - exp10));
				for (i = 0; i < 1 - exp10; ++i)
					tmpstr[i] = '0';
				tmpstr[1] = '.';
				for (j = 0; j <= position; ++j)
					tmpstr[i + j] = number[j] + '0';
				tmpstr[i + j] = 0;
			}
			else//科学计数法
			{
				tmpstr.resize(size_t(position + 41));
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
				tmpstr[0] = '-';
			}
			tmpstr.resize(i);
			return tmpstr;
		}
		bool Float256::ThrowException = false;
		const Float256& Float256::Zero()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(0);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::One()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(1);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Two()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(2);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Three()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(3);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Four()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(4);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Five()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(5);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Six()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(6);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Seven()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(7);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Eight()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(8);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Nine()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(9);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Ten()
		{
			static Float256* p;
			if (p)return*p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(10);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256*const Float256::_10e2()
		{
			static Float256 *p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 f[54];
				f[0] = 10.0;
				for (int i = 1; i < 54; ++i)f[i] = f[i - 1] * f[i - 1];
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float256& Float256::Ln2()
		{
			static Float256* p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"ln2");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Ln10()
		{
			static Float256* p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"ln10");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256*const Float256::_2sqrt2()
		{
			static Float256* p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 f[16];
				f[0] = 2.0;
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Sqrt();
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float256*const Float256::_12sqrt2()
		{
			static Float256*p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 f[16];
				f[0] = 2.0;
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Sqrt();
				for (int i = 0; i < 16; ++i)f[i] = One() / f[i];
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float256*const Float256::_ln2sqrt2()
		{
			static Float256*p;
			if (p)return p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 f[16];
				f[0] = Float256(L"ln2");
				for (int i = 1; i < 16; ++i)f[i] = f[i - 1].Half();
				p = f;
			}
			GetDefaultMutexRef().unlock();
			return p;
		}
		const Float256& Float256::MaxValue()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"max");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::MinValue()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"min");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::E()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"e");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Pi()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"pi");
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::_pi_half()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"pi").Half();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::_pi_half2()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"pi").Half().Half();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::Sqrt2()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(2).Sqrt();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::_sqrt2_half()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(2).Sqrt().Half();
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Float256& Float256::_pi_180()
		{
			static Float256 *p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Float256 val = Float256(L"pi") / Float256(180.0);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
	}
	int ::LK::Conversion::Convert <std::string , Math::Float256>::DefParam[1] = { 6 };
}
#endif
