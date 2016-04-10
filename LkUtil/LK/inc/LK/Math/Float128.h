#pragma once
#define LK_MATH_FLOAT128_
#define HAS_SIGNED_TYPES_EXCEPT64 std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value
#define HAS_UNSIGNED_TYPES_EXCEPT64 std::is_same<T, u32>::value|| std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value
#define HAS_ALL_TYPES std::is_same<T, u32>::value || std::is_same<T, u64>::value || std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value || std::is_same<T, s64>::value || std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, double>::value || std::is_same<T, float>::value || std::is_same<T, char>::value

/**@author 由W意波编写*/
namespace LK
{
	namespace Math
	{
		/**
		@brief 表示的有效数字
		@class Float128*/
		struct FloatData128
		{
			unsigned long long ull[2];
			/**
			@brief 左移1bit*/
			void MoveLeft() { ull[1] = (ull[1] << 1) | ((s64)ull[0] < 0); ull[0] <<= 1; }
			/**
			@brief 右移1bit*/
			void MoveRight() { ull[0] = (ull[0] >> 1) | (ull[1] << 63); ull[1] >>= 1; }
			/**
			@brief 自身左移操作(往高位移动mov位)
			@return 返回溢出最低的64bit数据*/
			unsigned long long operator<<=(long long mov)
			{
				if (mov >= 192 || mov <= -192)return ull[1] = ull[0] = 0;
				if (mov <= 0) { if (mov == 0)return 0; return operator>>=(-mov); }
				unsigned long long tmp;
				if (mov > 128) { tmp = ull[0] << (mov - 128); ull[1] = ull[0] = 0; return tmp; }
				if (mov == 128) { tmp = ull[0]; ull[1] = ull[0] = 0; return tmp; }
				if (mov > 64) { tmp = (ull[1] << (mov - 64)) | (ull[0] >> (128 - mov)); ull[1] = ull[0] << (mov - 64); ull[0] = 0; return tmp; }
				if (mov == 64) { tmp = ull[1]; ull[1] = ull[0]; ull[0] = 0; return tmp; }
				tmp = (ull[1] >> (64 - mov)); ull[1] = (ull[1] << mov) | (ull[0] >> (64 - mov)); ull[0] <<= mov; return tmp;
			}
			/**
			@brief 自身右移操作(往低位移动mov位)
			@return 返回溢出的最高64bit数据*/
			unsigned long long operator>>=(long long mov)
			{
				if (mov >= 192 || mov <= -192)return ull[1] = ull[0] = 0;
				if (mov <= 0) { if (mov == 0)return 0; return operator<<=(-mov); }
				unsigned long long tmp;
				if (mov > 128) { tmp = ull[1] >> (mov - 128); ull[1] = ull[0] = 0; return tmp; }
				if (mov == 128) { tmp = ull[1]; ull[1] = ull[0] = 0; return tmp; }
				if (mov > 64) { tmp = (ull[0] >> (mov - 64)) | (ull[1] << (128 - mov)); ull[0] = ull[1] >> (mov - 64); ull[1] = 0; return tmp; }
				if (mov == 64) { tmp = ull[0]; ull[0] = ull[1]; ull[1] = 0; return tmp; }
				tmp = (ull[0] << (64 - mov)); ull[0] = (ull[0] >> mov) | (ull[1] << (64 - mov)); ull[1] >>= mov; return tmp;
			}
			/**
			@brief 自身加上一个尾数，返回进位*/
			bool operator+=(const FloatData128 &data)
			{
				if (ull[0] > ~data.ull[0])
				{
					ull[0] += data.ull[0];
					if (ull[1] >= ~data.ull[1])
					{
						ull[1] += data.ull[1] + 1;
						return true;
					}
					ull[1] += data.ull[1] + 1;
					return false;
				}
				ull[0] += data.ull[0];
				if (ull[1] > ~data.ull[1])
				{
					ull[1] += data.ull[1];
					return true;
				}
				ull[1] += data.ull[1];
				return false;
			}
			/**
			@brief 自身减去一个尾数，返回借位*/
			bool operator-=(const FloatData128 &data)
			{
				if (ull[0] < data.ull[0])//需要借位
				{
					ull[0] -= data.ull[0];
					if (ull[1] <= data.ull[1])
					{
						ull[1] -= data.ull[1] + 1;
						return true;
					}
					ull[1] -= data.ull[1] + 1;
					return false;
				}
				ull[0] -= data.ull[0];
				if (ull[1] < data.ull[1])
				{
					ull[1] -= data.ull[1];
					return true;
				}
				ull[1] -= data.ull[1];
				return false;
			}
			/**
			@brief 自增运算，返回进位*/
			bool operator++() { return ++ull[0] == 0 && ++ull[1] == 0; }
			/**
			@brief 自减运算，返回借位*/
			bool operator--() { return ull[0]-- == 0 && ull[1]-- == 0; }
			/**
			@brief 返回前导0的数量*/
			long long BitsZeroCount()
			{
				return ull[1] != 0 ? Bits::LeftZeroCount64(ull[1]) : (64 + Bits::LeftZeroCount64(ull[0]));
			}
			operator bool()const { return ull[1] != 0 || ull[0] != 0; }
			bool operator !()const { return ull[1] == 0 && ull[0] == 0; }
			bool operator>(const FloatData128 &data)const { return ull[1] > data.ull[1] || (ull[1] == data.ull[1] && ull[0] > data.ull[0]); }
			bool operator<(const FloatData128 &data)const { return ull[1] < data.ull[1] || (ull[1] == data.ull[1] && ull[0] < data.ull[0]); }
			bool operator==(const FloatData128 &data)const { return ull[1] == data.ull[1] && ull[0] == data.ull[0]; }
			bool operator!=(const FloatData128 &data)const { return ull[1] != data.ull[1] || ull[0] != data.ull[0]; }
			bool operator>=(const FloatData128 &data)const { return ull[1] > data.ull[1] || (ull[1] == data.ull[1] && ull[0] >= data.ull[0]); }
			bool operator<=(const FloatData128 &data)const { return ull[1] < data.ull[1] || (ull[1] == data.ull[1] && ull[0] <= data.ull[0]); }
			inline bool GetBit(long long position)const { if (position >= 128 || position < 0)return false; if (position >= 64)return (s64)(ull[1] << (127 ^ position)) < 0; return (s64)(ull[0] << (63 ^ position)) < 0; }
			void Clear() { ull[1] = ull[0] = 0; }
			index_t CompareWith(const FloatData128&fd)const { if (ull[1] > fd.ull[1])return 1; if (ull[1]<fd.ull[1])return -1; if (ull[0]>fd.ull[0])return 1; if (ull[0] < fd.ull[0])return -1; return 0; }
		};

		/**
		@brief 表示一个128位浮点数*/
		class Float128
		{
			static u64 BitsMultiply(FloatData128 const &dat, FloatData128 &desdat, u64& num)
			{
				desdat.ull[0] = Bits::Multiply(dat.ull[1], num, desdat.ull + 1);
				u64 low, high;
				low = Bits::Multiply(dat.ull[0], num, &high);
				if (desdat.ull[0] > ~high)++desdat.ull[1];
				desdat.ull[0] += high;
				return low;
			}
		public:
			static cs64 ExpMaxValue = MaxValues::Int32;
			static cs64 ExpMinValue = MinValues::Int32;
			union {
				s64 Exp : 32;
				struct {
					u8 _exp[4];
					int Sign;
				};
			};
			FloatData128 Data;
			/**
			@brief 设置错误后是否要抛出异常,默认为false*/
			static bool ThrowException;
			/**
			@brief 默认构造函数，构造未初始化的Float128*/
			Float128() {}
			/**
			@brief 析构函数*/
			~Float128() {}
#ifdef LK_MATH_FLOAT256_
			/**
			@brief 从Float256构造*/
			Float128(Float256 const&f)
			{
				if (f.Exp > ExpMaxValue)MakeError(MathError::InputOverflow);
				else if (f.Exp < ExpMinValue)*this = Zero();
				else
				{
					Sign = f.Sign;
					Exp = f.Exp;
					Data.ull[1] = f.Data.ull[3];
					Data.ull[0] = f.Data.ull[2];
					if ((s64)f.Data.ull[1] < 0 && ++Data)
					{
						if (Exp == ExpMaxValue)MakeError(MathError::InputOverflow);
						else { ++Exp; Data.ull[1] = MinValues::Int64; }
					}
				}
			}
			/**
			@brief 转换到Float256*/
			operator Float256()const
			{
				Float256 f;
				f.Sign = Sign;
				f.Exp = Exp;
				f.Data.ull[3] = Data.ull[1];
				f.Data.ull[2] = Data.ull[0];
				f.Data.ull[1] = f.Data.ull[0] = 0;
				return f;
			}
#endif
			/**
			@brief 传入成员进行构造*/
			Float128(int sign, s32 exp, FloatData128 const& dat) : Exp(exp), Data(dat) { Sign = sign; }
			/**
			@brief 从其他数构造*/
			Float128(double number);
			Float128(float number);
			Float128(s64 number);
			Float128(u64 number);
			Float128(s32 number);
			Float128(u32 number);
			Float128(int number);
			Float128(ui32 number);
			Float128(s16 number);
			Float128(u16 number);
			Float128(s8 number);
			Float128(u8 number);
			Float128(char number);
			Float128(bool number);
			Float128(char const* number);

			Float128& operator=(Float128 const& f) { Sign = f.Sign; Exp = f.Exp; Data = f.Data; return*this; }

			inline Float128(const std::string & number) { *this = Float128(number.c_str()); }
			inline Float128(wchar_t const* number) { *this = Float128(std::wstring(number)); }
			inline Float128(const std::wstring & number) { *this = Float128(Strings::ToString(number)); }
			/**
			@brief 到其他数据的自动转换*/
			operator double()const;
			operator float()const { return float(operator double()); }
			operator long long()const;
			operator unsigned long long()const;
			operator long()const;
			operator unsigned long()const;
			operator int()const { return operator long(); }
			operator unsigned int()const { return (unsigned int)(operator int()); }
			operator short()const { return (short)(operator int()); }
			operator unsigned short()const { return (unsigned short)(operator unsigned int()); }
			operator signed char()const { return (signed char)(operator int()); }
			operator unsigned char()const { return (unsigned char)(operator unsigned int()); }
			operator char()const { return (char)(operator int()); }
			operator bool()const { return !Equals0(); }
			/**
			@brief 判断是否出错*/
			inline bool IsError()const { return Sign < -1; }
			/**
			@brief 设置为错误状态*/
			Float128& MakeError(char errorcode);
			/**
			@brief 判断是否等于0*/
			inline bool Equals0()const { return !Sign; }
			/**
			@brief 判断是否是整数*/
			inline bool IsInteger()const {
				if (Equals0() || Exp >= 127)return true; if (Exp < 0)return false; if (Exp>63)return !(Data.ull[0] << (Exp - 63)); if (Data.ull[0] != 0)return false; return Exp == 63 || !(Data.ull[1] << (Exp + 1));
			}
			/**
			@brief 判断是否是奇数*/
			inline bool IsOdd()const {
				if (Equals0() || Exp > 127)return false; if (Exp < 0)return false; if (Exp == 127)return Data.ull[0] & 1; if (Exp>63)return !(Data.ull[0] << (Exp - 63)) && (s64)(Data.ull[0] << (Exp - 62)) < 0; if (Data.ull[0] != 0)return false; return Exp == 63 ? (Data.ull[1] & 1) : (!(Data.ull[1] << (Exp + 1)) && ((s64)(Data.ull[1] << Exp) < 0));
			}
			/**
			@brief 负号重载*/
			Float128 operator-()const { return Float128(*this).SelfNegative(); }
			/**
			@brief 自身取相反值*/
			Float128& SelfNegative() { Sign = -Sign; return *this; }
			/**
			@brief 在该数有效数字最低位进行自+Sign操作
			@return 当发生错误时返回true，否则返回false*/
			bool SelfPlusExp128();
			/**
			@brief 在该数有效数字最低位进行自-Sign操作
			@return 当发生错误时返回true，否则返回false*/
			bool SelfMinusExp128();
			/**
			@brief 正号重载*/
			Float128 operator+()const { return *this; }
			/**
			@brief 前置运算重载*/
			Float128 &operator++();
			/**
			@brief 前置运算重载*/
			Float128 &operator--();
			/**
			@brief 后置运算重载*/
			Float128 operator++(int) { Float128 tmp(*this); ++(*this); return tmp; }
			/**
			@brief 后置运算重载*/
			Float128 operator--(int) { Float128 tmp(*this); --(*this); return tmp; }
			/**
			@brief +=重载*/
			Float128& operator+=(Float128 const&number) { return *this = *this + number; }
			Float128& operator+=(u64 num);
			Float128& operator+=(double num) { return operator+=(Float128(num)); }
			Float128& operator+=(float num) { return operator+=(Float128(num)); }
			Float128& operator+=(long long num) { if (num >= 0)return operator+=((u64)num); return operator-=((u64)-num); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float128&>::type operator+=(T num) { return operator+=((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float128&>::type operator+=(T num) { return operator+=((s64)num); }
			/**
			@brief +重载*/
			Float128 operator+(Float128 num)const;
			Float128 operator+(u64 num)const;
			Float128 operator+(s64 num)const { if (num >= 0)return operator+((u64)num); return operator-((u64)-num); }
			Float128 operator+(double num)const { return operator+(Float128(num)); }
			Float128 operator+(float num)const { return operator+(Float128(num)); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float128>::type operator+(T num)const { return operator+((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float128>::type operator+(T num)const { return operator+((s64)num); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float128>::type operator+(const T& t, const Float128& vi) { return vi + t; }
			/**
			@brief -=重载*/
			Float128& operator-=(Float128 const& number) { return *this = *this - number; }
			Float128& operator-=(u64 num);
			Float128& operator-=(double num) { return operator-=(Float128(num)); }
			Float128& operator-=(float num) { return operator-=(Float128(num)); }
			Float128& operator-=(long long num) { if (num >= 0)return operator-=((u64)num); return operator+=((u64)-num); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float128&>::type operator-=(T num) { return operator-=((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float128&>::type operator-=(T num) { return operator-=((s64)num); }

			/**
			@brief -重载*/
			Float128 operator-(Float128 const& num2)const;
			Float128 operator-(u64 num)const;
			Float128 operator-(double num)const { return operator-(Float128(num)); }
			Float128 operator-(float num)const { return operator-(Float128(num)); }
			Float128 operator-(long long num)const { if (num >= 0)return operator-((u64)num); return operator+((u64)-num); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float128>::type operator-(T num)const { return operator-((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float128>::type operator-(T num)const { return operator-((s64)num); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float128>::type operator-(const T& t, const Float128& vi) { return (vi - t).SelfNegative(); }
			/*乘法*/

			/**
			@brief 快速乘10*/
			Float128 Multiply10()const;
			/**
			@brief 自身快速自乘10*/
			Float128& SelfMultiply10();
			/**
			@brief *=重载*/
			Float128& operator*=(Float128 const& number);
			/**
			@brief *重载*/
			Float128 operator*(Float128 const& num2)const;
			Float128 operator*(double number)const;
			Float128 operator*(float number)const { return operator*((double)number); }
			Float128 operator*(s64 number)const { return (number > 0) ? *this*((u64)number) : (*this*(u64)(-number)).SelfNegative(); }
			Float128 operator*(u64 number)const;
			Float128 operator*(s32 number)const { return operator*((s64)number); }
			Float128 operator*(u32 number)const { return operator*((u64)number); }
			Float128 operator*(int number)const { return operator*((s64)number); }
			Float128 operator*(ui32 number)const { return operator*((u64)number); }
			Float128 operator*(s16 number)const { return operator*((s64)number); }
			Float128 operator*(u16 number)const { return operator*((u64)number); }
			Float128 operator*(s8 number)const { return operator*((s64)number); }
			Float128 operator*(u8 number)const { return operator*((u64)number); }
			Float128 operator*(char number)const { return operator*((s64)number); }
			Float128 operator*(bool number)const { if (IsError() || number)return *this; return Zero(); }

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float128>::type operator*(const T& t, const Float128& vi) { return vi * t; }
			/*除法*/

			/**
			@brief 取一半*/
			Float128 Half()const { if (IsError())return *this; if (Exp == ExpMinValue)return Zero(); return Float128(Sign, s32(Exp - 1), Data); }
			/**
			@brief 乘以2^-x次方*/
			Float128 Half(s64 x)const
			{
				if (IsError())return *this;
				s64 exp = Exp - x;
				if (exp < ExpMinValue)return Zero();
				if (exp > ExpMaxValue)return Float128().MakeError(MathError::MultiplyExpOverflow);
				return Float128(Sign, (s32)exp, Data);
			}
			/**
			@brief 自身取一半*/
			Float128& SelfHalf() { if (IsError())return *this; if (Exp == ExpMinValue)return *this = Zero(); --Exp; return *this; }
			/**
			@brief /=重载*/
			Float128& operator/=(Float128 const& number) { return *this = *this / number; }

			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, Float128&>::type operator/=(T num) { return *this = *this / num;; }
			/**
			@brief /重载*/
			Float128 operator/(Float128 const& num)const;
			Float128 operator/(double number)const;
			Float128 operator/(float number)const { return operator/((double)number); }
			Float128 operator/(s64 number)const { if (number < 0)return operator/((u64)-number).SelfNegative(); return operator/((u64)number); }
			Float128 operator/(u64 number)const;
			Float128 operator/(s32 number)const { if (number < 0)return operator/((u32)-number).SelfNegative(); return operator/((u32)number); }
			Float128 operator/(u32 number)const;
			Float128 operator/(int number)const { return operator/((s32)number); }
			Float128 operator/(unsigned int number)const { return operator/((u32)number); }
			Float128 operator/(short number)const { return operator/((s32)number); }
			Float128 operator/(unsigned short number)const { return operator/((u32)number); }
			Float128 operator/(signed char number)const { return operator/((s32)number); }
			Float128 operator/(unsigned char number)const { return operator/((u32)number); }
			Float128 operator/(char number)const { return operator/((s32)number); }
			Float128 operator/(bool number)const { if (IsError() || number)return *this; return Float128().MakeError(MathError::Divide0); }

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float128>::type operator/(const T& t, const Float128& vi) { return Float128(t) / vi; }
			/*求余*/

			/**
			@brief %重载*/
			friend Float128 operator%(const Float128& f1, const Float128& f2) { return f1.Mod(f2); }
			/**
			@brief %=重载*/
			Float128& operator%=(const Float128& f) { return *this = Mod(f); }

			/*求幂*/

			friend Float128 operator^(const Float128& f1, const Float128& f2) { return f1.Pow(f2); }
			Float128& operator^=(const Float128& f) { return *this = Pow(f); }

			/**
			@brief 逻辑运算符*/
			bool operator !()const { return Equals0(); }

			/*比较运算符*/

			bool operator==(Float128 const&num)const { return Sign == num.Sign&&Exp == num.Exp&&Data == num.Data; }
			bool operator>(Float128 const&num)const
			{
				if (Sign != num.Sign)return Sign > num.Sign;
				if (Sign == 0)return false;
				if (Exp != num.Exp)return Sign > 0 ? (Exp > num.Exp) : (Exp < num.Exp);
				return Sign > 0 ? (Data > num.Data) : (Data < num.Data);
			}
			bool operator<(Float128 const&num)const
			{
				if (Sign != num.Sign)return Sign < num.Sign;
				if (Sign == 0)return false;
				if (Exp != num.Exp)return Sign > 0 ? (Exp < num.Exp) : (Exp > num.Exp);
				return Sign > 0 ? (Data < num.Data) : (Data > num.Data);
			}
			bool operator!=(Float128 const&num)const { return !(*this == num); }
			bool operator>=(Float128 const&num)const { return !(*this < num); }
			bool operator<=(Float128 const&num)const { return !(*this > num); }

			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>(const T& t)const { return operator>(Float128(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<(const T& t)const { return operator<(Float128(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator==(const T& t)const { return operator==(Float128(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(const T& t)const { return operator!=(Float128(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(const T& t)const { return operator>=(Float128(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(const T& t)const { return operator<=(Float128(t)); }

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>(const T& t, const Float128& vi) { return vi < t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<(const T& t, const Float128& vi) { return vi > t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator==(const T& t, const Float128& vi) { return vi == t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(const T& t, const Float128& vi) { return vi != t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(const T& t, const Float128& vi) { return vi <= t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(const T& t, const Float128& vi) { return vi >= t; }
			/**
			@brief 开平方*/
			Float128 Sqrt()const;
			/**
			@brief 开平方*/
			static Float128 Sqrt(Float128 const&f) { return f.Sqrt(); }
			/**
			@brief 取绝对值*/
			Float128 Abs()const { if (Sign != -1)return*this; return Float128(1, Exp, Data); }
			/**
			@brief 取绝对值*/
			static Float128 Abs(Float128 const& f) { return f.Abs(); }
			/**
			@brief 自身取绝对值*/
			Float128& SelfAbs() { if (Sign == -1)Sign = 1; return *this; }
			/**
			@brief 求余(>=0),商保存到quotient里，余数与number同号；number<0时向上取整，number>0时向下取整*/
			Float128 Mod(Float128 const&number, Float128 &quotient)const;
			/**
			@brief 求余*/
			Float128 Mod(Float128 const& number)const;
			/**
			@brief 返回该数以num为底的对数*/
			Float128 Log(Float128 const& num2)const;
			/**
			@brief 返回num1以num2为底的对数*/
			static inline Float128 Log(Float128 const& num1, Float128 const& num2) { return num1.Log(num2); }
			/**
			@brief 返回该数以10为底的对数*/
			Float128 Lg()const;
			/**
			@brief 返回num以10为底的对数*/
			static inline Float128 Lg(Float128 const& num) { return num.Lg(); }
			/**
			@brief 返回该数以e为底的对数*/
			Float128 Ln()const;
			/**
			@brief 返回num以e为底的对数*/
			static Float128 Ln(Float128 const& num) { return num.Ln(); }
			/**
			@brief 返回该数以2为底的对数*/
			Float128 Log2()const;
			/**
			@brief 返回num以2为底的对数*/
			static Float128 Log2(Float128 const& num) { return num.Log2(); }
			/**
			@brief 返回e的该数次幂*/
			Float128 Expe()const;
			/**
			@brief 返回e的num次幂*/
			static Float128 Expe(Float128 const& num) { return num.Expe(); }
			/**
			@brief 返回2的该数次幂*/
			Float128 Exp2()const;
			/**
			@brief 返回2的num次幂*/
			static Float128 Exp2(Float128 const& num) { return num.Exp2(); }
			/**
			@brief 返回该数的num次幂*/
			Float128 Pow(Float128 const&num)const;
			/**
			@brief 返回num1的num2次幂*/
			static Float128 Pow(Float128 const& num1, Float128 const&num2) { return num1.Pow(num2); }
			/**
			@brief 取正弦(弧度制)*/
			Float128 Sin()const;
			/**
			@brief 取正弦(弧度制)*/
			static Float128 Sin(Float128 const&num) { return num.Sin(); }
			/**
			@brief 取正弦(角度制)*/
			Float128 Dsin()const;
			/**
			@brief 取正弦(角度制)*/
			static Float128 Dsin(Float128 const&num) { return num.Dsin(); }
			/**
			@brief 取余弦(弧度制)*/
			Float128 Cos()const;
			/**
			@brief 同时计算正弦和余弦值*/
			void SinAndCos(Float128 & outsin, Float128 & outcos)const;
			/**
			@brief 取余弦(弧度制)*/
			static Float128 Cos(Float128 const&num) { return num.Cos(); }
			/**
			@brief 取余弦(角度制)*/
			Float128 Dcos()const;
			/**
			@brief 取余弦(角度制)*/
			static Float128 Dcos(Float128 const&num) { return num.Dcos(); }
			/**
			@brief 取正切(弧度制)*/
			Float128 Tan()const;
			/**
			@brief 取正切(弧度制)*/
			static Float128 Tan(Float128 const&num) { return num.Tan(); }
			/**
			@brief 取正切(角度制)*/
			Float128 Dtan()const;
			/**
			@brief 取正切(角度制)*/
			static Float128 Dtan(Float128 const&num) { return num.Dtan(); }
			/**
			@brief 取余切(弧度制)*/
			Float128 Cot()const;
			/**
			@brief 取余切(弧度制)*/
			static Float128 Cot(Float128 const&num) { return num.Cot(); }
			/**
			@brief 取余切(角度制)*/
			Float128 Dcot()const;
			/**
			@brief 取余切(角度制)*/
			static Float128 Dcot(Float128 const&num) { return num.Dcot(); }
			/**
			@brief 反正弦(弧度制)*/
			Float128 Arcsin()const;
			/**
			@brief 反正弦(弧度制)*/
			static Float128 Arcsin(Float128 const&num) { return num.Arcsin(); }
			/**
			@brief 反正弦(角度制)*/
			Float128 Darcsin()const;
			/**
			@brief 反正弦(角度制)*/
			static Float128 Darcsin(Float128 const&num) { return num.Darcsin(); }
			/**
			@brief 反余弦(弧度制)*/
			Float128 Arccos()const;
			/**
			@brief 反余弦(弧度制)*/
			static Float128 Arccos(Float128 const&num) { return num.Arccos(); }
			/**
			@brief 反余弦(角度制)*/
			Float128 Darccos()const;
			/**
			@brief 反余弦(角度制)*/
			static Float128 Darccos(Float128 const&num) { return num.Darccos(); }
			/**
			@brief 反正切(弧度制)*/
			Float128 Arctan()const;
			/**
			@brief 反正切(弧度制)*/
			static Float128 Arctan(Float128 const&num) { return num.Arctan(); }
			/**
			@brief 反正切(角度制)*/
			Float128 Darctan()const;
			/**
			@brief 反正切(角度制)*/
			static Float128 Darctan(Float128 const&num) { return num.Darctan(); }
			/**
			@brief 比较是否相等*/
			bool Equals(const Float128& num1)const;
			/**
			@brief 约等于,符号不同返回false.忽略位数末less偏差值，比较是否等于*/
			bool AppEquals(const Float128& number, u64 less = 4)const;
			/**
			@brief 基于LK::GetDefaultRand()在[0,1)范围内随机取值*/
			static Float128 Rand();
			/**
			@brief 获取整数部分*/
			Float128 GetIntegerPart()const;
			/**
			@brief 自身取整(截取)*/
			Float128& SelfGetIntegerPart();
			/**
			@brief 获取小数部分*/
			Float128 GetDecimalPart()const;
			/**
			@brief 自身截取小数部分*/
			Float128& SelfGetDecimalPart();
			/**
			@brief 转换为字符串描述*/
			std::string ToString(int length = 0)const;
			/**
			@brief 常量0*/
			const static Float128& Zero();
			/**
			@brief 常量1*/
			const static Float128& One();
			/**
			@brief 常量2*/
			const static Float128& Two();
			/**
			@brief 常量3*/
			const static Float128& Three();
			/**
			@brief 常量4*/
			const static Float128& Four();
			/**
			@brief 常量5*/
			const static Float128& Five();
			/**
			@brief 常量6*/
			const static Float128& Six();
			/**
			@brief 常量7*/
			const static Float128& Seven();
			/**
			@brief 常量8*/
			const static Float128& Eight();
			/**
			@brief 常量9*/
			const static Float128& Nine();
			/**
			@brief 常量10*/
			const static Float128& Ten();
			/**
			@brief 10^(2^(x))，其中x为0~53*/
			const static Float128*const _10e2();
			/**
			@brief 对2取自然数e为底的对数*/
			const static Float128& Ln2();
			/**
			@brief 对10取自然数e为底的对数*/
			const static Float128& Ln10();
			/**
			@brief 2开根号x次 : 2^(1/(2^x))*/
			const static Float128*const _2sqrt2();
			/**
			@brief 2开根号x次的倒数 : 1/(2^(1/(2^x)))*/
			const static Float128*const _12sqrt2();
			/**
			@brief ln(2^(1/(2^x)))=(ln2)/2^x*/
			const static Float128*const _ln2sqrt2();
			/**
			@brief 最大值*/
			const static Float128& MaxValue();
			/**
			@brief 最小值*/
			const static Float128& MinValue();
			/**
			@brief 自然数e*/
			const static Float128& E();
			/**
			@brief 圆周率π*/
			const static Float128& Pi();
			/**
			@brief 二分之π*/
			const static Float128& _pi_half();
			/**
			@brief 四分之π*/
			const static Float128& _pi_half2();
			/**
			@brief 根号2*/
			const static Float128& Sqrt2();
			/**
			@brief 二分之根号2*/
			const static Float128& _sqrt2_half();
			/**
			@brief 一百八十分之π*/
			const static Float128& _pi_180();
		};
	}
	namespace Conversion {
		template<>struct Convert <std::string, Math::Float128> {
			static inline std::string from(Math::Float128 const&l) { return l.ToString(DefParam[0]); }
			static int DefParam[1];
		};
	}
}
namespace std
{
	inline ::LK::Math::Float128 fmod(::LK::Math::Float128 const& f1, ::LK::Math::Float128 const& f2) { return f1.Mod(f2); }
	inline ::LK::Math::Float128 sin(::LK::Math::Float128 const& f) { return f.Sin(); }
	inline ::LK::Math::Float128 cos(::LK::Math::Float128 const& f) { return f.Cos(); }
	inline ::LK::Math::Float128 tan(::LK::Math::Float128 const& f) { return f.Tan(); }
	inline ::LK::Math::Float128 log(::LK::Math::Float128 const& f) { return f.Ln(); }
	inline ::LK::Math::Float128 log10(::LK::Math::Float128 const& f) { return f.Lg(); }
	inline ::LK::Math::Float128 pow(::LK::Math::Float128 const& f1, ::LK::Math::Float128 const& f2) { return f1.Pow(f2); }
	inline ::LK::Math::Float128 asin(::LK::Math::Float128 const& f) { return f.Arcsin(); }
	inline ::LK::Math::Float128 acos(::LK::Math::Float128 const& f) { return f.Arccos(); }
	inline ::LK::Math::Float128 atan(::LK::Math::Float128 const& f) { return f.Arctan(); }
	inline ::LK::Math::Float128 sqrt(::LK::Math::Float128 const& f) { return f.Sqrt(); }
	inline ::LK::Math::Float128 abs(::LK::Math::Float128 const& f) { return f.Abs(); }
	inline ::LK::Math::Float128 floor(::LK::Math::Float128 const& f) { ::LK::Math::Float128 ff = f.GetIntegerPart(); return f.Sign <= 0 ? ((ff == f) ? f : ff - ::LK::Math::Float128::One()) : ff; }
	inline ::LK::Math::Float128 ceil(::LK::Math::Float128 const& f) { ::LK::Math::Float128 ff = f.GetIntegerPart(); return f.Sign <= 0 ? ff : ((ff == f) ? f : ff + ::LK::Math::Float128::One()); }
}
#undef HAS_SIGNED_TYPES_EXCEPT64
#undef HAS_UNSIGNED_TYPES_EXCEPT64
#undef HAS_ALL_TYPES
