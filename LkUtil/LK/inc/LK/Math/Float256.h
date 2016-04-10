#pragma once
#define LK_MATH_FLOAT256_
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
		@class Float256*/
		class FloatData256
		{
		public:
			union
			{
				unsigned long long ull[4];
				unsigned int ui[8];
				unsigned short us[16];
				unsigned char uc[32];
			};
			/**
			@brief 自身左移操作(往高位移动mov位)
			@return 返回溢出最低的64bit数据*/
			unsigned long long operator<<=(long long mov);
			/**
			@brief 自身右移操作(往低位移动mov位)
			@return 返回溢出的最高64bit数据*/
			unsigned long long operator>>=(long long mov);
			/**
			@brief 自身加上一个尾数，返回进位*/
			bool operator+=(FloatData256 &data);
			/**
			@brief 自身减去一个尾数，返回借位*/
			bool operator-=(FloatData256 &data);
			/**
			@brief 自增运算，返回进位*/
			bool operator++();
			/**
			@brief 自减运算，返回借位*/
			bool operator--();
			/**
			@brief 返回前导0的数量*/
			long long BitsZeroCount();
			operator bool()const { return ull[3] != 0 || ull[2] != 0 || ull[1] != 0 || ull[0] != 0; }
			bool operator !()const { return ull[3] == 0 && ull[2] == 0 && ull[1] == 0 && ull[0] == 0; }
			bool operator>(const FloatData256 &data)const;
			bool operator<(const FloatData256 &data)const;
			bool operator==(const FloatData256 &data)const;
			bool operator!=(const FloatData256 &data)const;
			bool operator>=(const FloatData256 &data)const;
			bool operator<=(const FloatData256 &data)const;
			inline bool GetBit(long long position)const { return Bits::Get(this, (index_t)position); }
			void Clear() { ull[3] = ull[2] = ull[1] = ull[0] = 0; }
			index_t CompareWith(const FloatData256&fd);
		};

		/**
		@brief 表示一个256位浮点数*/
		struct Float256
		{
			static void BitsMoveLeft(unsigned long long * pull);
			static void BitsMoveRight(unsigned long long * pull);
			static unsigned long BitsMultiply(FloatData256 &dat, FloatData256 &desdat, unsigned long long& num);
		public:
			const static long long ExpMaxValue;
			const static long long ExpMinValue;
			union
			{
				long long Exp : 56;
				struct
				{
					char _exp[7];
					signed char Sign;
				};
			};
			FloatData256 Data;
			/**
			@brief 设置错误后是否要抛出异常,默认为false*/
			static bool ThrowException;
#ifdef LK_MATH_FLOAT128_
			/**
			@brief 从Float128构造*/
			Float256(Float128 const&f) :Exp(f.Exp)
			{
				Sign = f.Sign;
				Data.ull[3] = f.Data.ull[1];
				Data.ull[2] = f.Data.ull[0];
				Data.ull[1] = Data.ull[0] = 0;
			}
			/**
			@brief 转换到Float128*/
			operator Float128()const
			{
				Float128 f;
				if (Exp > Float128::ExpMaxValue)return f.MakeError(MathError::InputOverflow);
				if (Exp < Float128::ExpMinValue)return Float128::Zero();
				f.Sign = Sign;
				f.Exp = Exp;
				f.Data.ull[1] = Data.ull[3];
				f.Data.ull[0] = Data.ull[2];
				if ((s64)Data.ull[1] < 0 && ++f.Data)
				{
					if (Exp == Float128::ExpMaxValue)f.MakeError(MathError::InputOverflow);
					else { ++f.Exp; f.Data.ull[1] = MinValues::Int64; }
				}
				return f;
			}
#endif
			/**
			@brief 默认构造函数，构造未初始化的Float256*/
			Float256() {}
			/**
			@brief 析构函数*/
			~Float256() {}
			/**
			@brief 从其他数构造*/
			Float256(const Float256&f):Exp(f.Exp),Data(f.Data),Sign(f.Sign) {}
			Float256(double number);
			Float256(float number);
			Float256(long long number);
			Float256(unsigned long long number);
			Float256(long number);
			Float256(unsigned long number);
			Float256(int number);
			Float256(unsigned int number);
			Float256(short number);
			Float256(unsigned short number);
			Float256(signed char number);
			Float256(unsigned char number);
			Float256(char number);
			Float256(bool number);
			Float256(wchar_t const* number);
			Float256(char const* number);
			inline Float256(const std::string & number) { *this = Float256(number.c_str()); }
			inline Float256(const std::wstring& number) { *this = Float256(number.c_str()); }
			/**
			@brief 到其他数据的自动转换*/
			operator double()const;
			operator float()const;
			operator long long()const;
			operator unsigned long long()const;
			operator long()const;
			operator unsigned long()const;
			operator int()const;
			operator unsigned int()const;
			operator short()const;
			operator unsigned short()const;
			operator signed char()const;
			operator unsigned char()const;
			operator char()const;
			operator bool()const;
			/**
			@brief 判断是否出错*/
			inline bool IsError()const { return Sign < -1; }
			/**
			@brief 设置为错误状态*/
			inline Float256& MakeError(char errorcode) {
				Sign = errorcode;
				if (ThrowException)Throw<MathException >(Sign);
				return *this;
			}
			/**
			@brief 判断是否等于0*/
			inline bool Equals0()const { return !Sign; }
			/**
			@brief 判断是否是整数*/
			bool IsInteger()const;
			/**
			@brief 判断是否是奇数*/
			bool IsOdd()const;
			/**
			@brief 负号重载*/
			Float256 operator-()const;
			/**
			@brief 自身取相反值*/
			Float256& SelfNegative();
			/**
			@brief 在该数有效数字最低位进行自+Sign操作
			@return 当发生错误时返回true，否则返回false*/
			bool SelfPlusExp256();
			/**
			@brief 在该数有效数字最低位进行自-Sign操作
			@return 当发生错误时返回true，否则返回false*/
			bool SelfMinusExp256();
			/**
			@brief 正号重载*/
			Float256 operator+()const { return *this; }
			/**
			@brief 前置运算重载*/
			Float256 &operator++();
			/**
			@brief 前置运算重载*/
			Float256 &operator--();
			/**
			@brief 后置运算重载*/
			Float256 operator++(int);
			/**
			@brief 后置运算重载*/
			Float256 operator--(int);
			/**
			@brief +=重载*/
			Float256& operator+=(Float256 const&number);
			Float256& operator+=(unsigned long long num);
			Float256& operator+=(double num) { return operator+=(Float256(num)); }
			Float256& operator+=(float num) { return operator+=(Float256(num)); }
			Float256& operator+=(long long num) { if (num >= 0)return operator+=((u64)num); return operator-=((u64)-num); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float256&>::type operator+=(T num) { return operator+=((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float256&>::type operator+=(T num) { return operator+=((s64)num); }
			/**
			@brief +重载*/
			Float256 operator+(Float256 num)const;
			Float256 operator+(unsigned long long num)const;
			Float256 operator+(long long num)const { if (num >= 0)return operator+((u64)num); return operator-((u64)-num); }
			Float256 operator+(double num)const { return operator+(Float256(num)); }
			Float256 operator+(float num)const { return operator+(Float256(num)); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float256>::type operator+(T num)const { return operator+((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float256>::type operator+(T num)const { return operator+((s64)num); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float256>::type operator+(const T& t, const Float256& vi) { return vi + t; }
			/**
			@brief -=重载*/
			Float256& operator-=(Float256 const& number);
			Float256& operator-=(unsigned long long num);
			Float256& operator-=(double num) { return operator-=(Float256(num)); }
			Float256& operator-=(float num) { return operator-=(Float256(num)); }
			Float256& operator-=(long long num) { if (num >= 0)return operator-=((u64)num); return operator+=((u64)-num); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float256&>::type operator-=(T num) { return operator-=((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float256&>::type operator-=(T num) { return operator-=((s64)num); }

			/**
			@brief -重载*/
			Float256 operator-(Float256 num2)const;
			Float256 operator-(unsigned long long num)const;
			Float256 operator-(double num)const { return operator-(Float256(num)); }
			Float256 operator-(float num)const { return operator-(Float256(num)); }
			Float256 operator-(long long num)const { if (num >= 0)return operator-((u64)num); return operator+((u64)-num); }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Float256>::type operator-(T num)const { return operator-((u64)num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Float256>::type operator-(T num)const { return operator-((s64)num); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float256>::type operator-(const T& t, const Float256& vi) { return (vi - t).SelfNegative(); }
			/*乘法*/

			/**
			@brief 标准乘法*/
			Float256 Multiply(Float256 number)const;
			/**
			@brief 快速乘10*/
			Float256 Multiply10()const;
			/**
			@brief 自身快速自乘10*/
			Float256& SelfMultiply10();
			/**
			@brief *=重载*/
			Float256& operator*=(Float256 const& number);
			/**
			@brief *重载*/
			Float256 operator*(Float256 num2)const;
			Float256 operator*(double number)const;
			Float256 operator*(float number)const;
			Float256 operator*(long long number)const;
			Float256 operator*(unsigned long long number)const;
			Float256 operator*(long number)const;
			Float256 operator*(unsigned long number)const;
			Float256 operator*(int number)const;
			Float256 operator*(unsigned int number)const;
			Float256 operator*(short number)const;
			Float256 operator*(unsigned short number)const;
			Float256 operator*(signed char number)const;
			Float256 operator*(unsigned char number)const;
			Float256 operator*(char number)const;
			Float256 operator*(bool number)const;

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float256>::type operator*(const T& t, const Float256& vi) { return vi * t; }
			/*除法*/

			/**
			@brief 标准除法*/
			Float256 Divide(Float256 number)const;
			/**
			@brief 取一半*/
			Float256 Half()const;
			/**
			@brief 自身取一半*/
			Float256& SelfHalf();
			/**
			@brief /=重载*/
			Float256& operator/=(Float256 const& number);
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, Float256&>::type operator/=(T num) { return *this = *this / num;; }
			/**
			@brief /重载*/
			Float256 operator/(Float256 num2)const;
			Float256 operator/(double number)const;
			Float256 operator/(float number)const { return operator/((double)number); }
			Float256 operator/(long long number)const;
			Float256 operator/(unsigned long long number)const;
			Float256 operator/(long number)const;
			Float256 operator/(unsigned long number)const;
			Float256 operator/(int number)const;
			Float256 operator/(unsigned int number)const;
			Float256 operator/(short number)const;
			Float256 operator/(unsigned short number)const;
			Float256 operator/(signed char number)const;
			Float256 operator/(unsigned char number)const;
			Float256 operator/(char number)const;
			Float256 operator/(bool number)const;

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, Float256>::type operator/(const T& t, const Float256& vi) { return Float256(t) / vi; }
			/*求余*/

			/**
			@brief %重载*/
			friend Float256 operator%(const Float256& f1, const Float256& f2) { return f1.Mod(f2); }
			/**
			@brief %=重载*/
			Float256& operator%=(const Float256& f) { return *this = Mod(f); }

			/*求幂*/

			friend Float256 operator^(const Float256& f1, const Float256& f2) { return f1.Pow(f2); }
			Float256& operator^=(const Float256& f) { return *this = Pow(f); }

			/**
			@brief 逻辑运算符*/
			bool operator !()const;

			/*比较运算符*/

			bool operator==(Float256 const&num)const { return Sign == num.Sign&&Exp == num.Exp&&Data == num.Data; }
			bool operator>(Float256 const&num)const
			{
				if (Sign != num.Sign)return Sign > num.Sign;
				if (Sign == 0)return false;
				if (Exp != num.Exp)return Sign > 0 ? (Exp > num.Exp) : (Exp < num.Exp);
				return Sign > 0 ? (Data > num.Data) : (Data < num.Data);
			}
			bool operator<(Float256 const&num)const
			{
				if (Sign != num.Sign)return Sign < num.Sign;
				if (Sign == 0)return false;
				if (Exp != num.Exp)return Sign > 0 ? (Exp < num.Exp) : (Exp > num.Exp);
				return Sign > 0 ? (Data < num.Data) : (Data > num.Data);
			}
			bool operator!=(Float256 const&num)const { return !(*this == num); }
			bool operator>=(Float256 const&num)const { return !(*this < num); }
			bool operator<=(Float256 const&num)const { return !(*this > num); }

			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>(const T& t)const { return operator>(Float256(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<(const T& t)const { return operator<(Float256(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator==(const T& t)const { return operator==(Float256(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(const T& t)const { return operator!=(Float256(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(const T& t)const { return operator>=(Float256(t)); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(const T& t)const { return operator<=(Float256(t)); }

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>(const T& t, const Float256& vi) { return vi < t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<(const T& t, const Float256& vi) { return vi > t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator==(const T& t, const Float256& vi) { return vi == t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(const T& t, const Float256& vi) { return vi != t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(const T& t, const Float256& vi) { return vi <= t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(const T& t, const Float256& vi) { return vi >= t; }
			/**
			@brief 开平方*/
			Float256 Sqrt()const;
			/**
			@brief 开平方*/
			static Float256 Sqrt(Float256 const&f) { return f.Sqrt(); }
			/**
			@brief 取绝对值*/
			Float256 Abs()const;
			/**
			@brief 取绝对值*/
			static Float256 Abs(Float256 const& f) { return f.Abs(); }
			/**
			@brief 自身取绝对值*/
			Float256& SelfAbs();
			/**
			@brief 求余(>=0),商保存到quotient里，余数与number同号；number<0时向上取整，number>0时向下取整*/
			Float256 Mod(Float256 number, Float256 &quotient)const;
			/**
			@brief 求余*/
			Float256 Mod(Float256 number)const;
			/**
			@brief 返回该数以num为底的对数*/
			Float256 Log(Float256 const& num2)const;
			/**
			@brief 返回num2以num1为底的对数*/
			static inline Float256 Log(Float256 const& num1, Float256 const& num2) { return num2.Log(num1); }
			/**
			@brief 返回该数以10为底的对数*/
			Float256 Lg()const;
			/**
			@brief 返回num以10为底的对数*/
			static inline Float256 Lg(Float256 const& num) { return num.Lg(); }
			/**
			@brief 返回该数以e为底的对数*/
			Float256 Ln()const;
			/**
			@brief 返回num以e为底的对数*/
			static Float256 Ln(Float256 const& num) { return num.Ln(); }
			/**
			@brief 返回该数以2为底的对数*/
			Float256 Log2()const;
			/**
			@brief 返回num以2为底的对数*/
			static Float256 Log2(Float256 const& num) { return num.Log2(); }
			/**
			@brief 返回e的该数次幂*/
			Float256 Expe()const;
			/**
			@brief 返回e的num次幂*/
			static Float256 Expe(Float256 const& num) { return num.Expe(); }
			/**
			@brief 返回2的该数次幂*/
			Float256 Exp2()const;
			/**
			@brief 返回2的num次幂*/
			static Float256 Exp2(Float256 const& num) { return num.Exp2(); }
			/**
			@brief 返回该数的num次幂*/
			Float256 Pow(Float256 const&num)const;
			/**
			@brief 返回num1的num2次幂*/
			static Float256 Pow(Float256 const& num1, Float256 const&num2) { return num1.Pow(num2); }
			/**
			@brief 取正弦(弧度制)*/
			Float256 Sin()const;
			/**
			@brief 取正弦(弧度制)*/
			static Float256 Sin(Float256 const&num) { return num.Sin(); }
			/**
			@brief 取正弦(角度制)*/
			Float256 Dsin()const;
			/**
			@brief 取正弦(角度制)*/
			static Float256 Dsin(Float256 const&num) { return num.Dsin(); }
			/**
			@brief 取余弦(弧度制)*/
			Float256 Cos()const;
			/**
			@brief 取余弦(弧度制)*/
			static Float256 Cos(Float256 const&num) { return num.Cos(); }
			/**
			@brief 取余弦(角度制)*/
			Float256 Dcos()const;
			/**
			@brief 取余弦(角度制)*/
			static Float256 Dcos(Float256 const&num) { return num.Dcos(); }
			/**
			@brief 取正切(弧度制)*/
			Float256 Tan()const;
			/**
			@brief 取正切(弧度制)*/
			static Float256 Tan(Float256 const&num) { return num.Tan(); }
			/**
			@brief 取正切(角度制)*/
			Float256 Dtan()const;
			/**
			@brief 取正切(角度制)*/
			static Float256 Dtan(Float256 const&num) { return num.Dtan(); }
			/**
			@brief 取余切(弧度制)*/
			Float256 Cot()const;
			/**
			@brief 取余切(弧度制)*/
			static Float256 Cot(Float256 const&num) { return num.Cot(); }
			/**
			@brief 取余切(角度制)*/
			Float256 Dcot()const;
			/**
			@brief 取余切(角度制)*/
			static Float256 Dcot(Float256 const&num) { return num.Dcot(); }
			/**
			@brief 反正弦(弧度制)*/
			Float256 Arcsin()const;
			/**
			@brief 反正弦(弧度制)*/
			static Float256 Arcsin(Float256 const&num) { return num.Arcsin(); }
			/**
			@brief 反正弦(角度制)*/
			Float256 Darcsin()const;
			/**
			@brief 反正弦(角度制)*/
			static Float256 Darcsin(Float256 const&num) { return num.Darcsin(); }
			/**
			@brief 反余弦(弧度制)*/
			Float256 Arccos()const;
			/**
			@brief 反余弦(弧度制)*/
			static Float256 Arccos(Float256 const&num) { return num.Arccos(); }
			/**
			@brief 反余弦(角度制)*/
			Float256 Darccos()const;
			/**
			@brief 反余弦(角度制)*/
			static Float256 Darccos(Float256 const&num) { return num.Darccos(); }
			/**
			@brief 反正切(弧度制)*/
			Float256 Arctan()const;
			/**
			@brief 反正切(弧度制)*/
			static Float256 Arctan(Float256 const&num) { return num.Arctan(); }
			/**
			@brief 反正切(角度制)*/
			Float256 Darctan()const;
			/**
			@brief 反正切(角度制)*/
			static Float256 Darctan(Float256 const&num) { return num.Darctan(); }
			/**
			@brief 比较是否相等*/
			bool Equals(const Float256& num1)const;
			/**
			@brief 约等于,符号不同返回false.忽略位数末count位，比较是否等于*/
			bool AppEquals(const Float256& number, unsigned long long less = 4)const;
			/**
			@brief 基于LK::DefaultRand()在[0,1)范围内随机取值*/
			static Float256 Rand();
			/**
			@brief 获取整数部分*/
			Float256 GetIntegerPart()const;
			/**
			@brief 自身取整(截取)*/
			Float256& SelfGetIntegerPart();
			/**
			@brief 获取小数部分*/
			Float256 GetDecimalPart()const;
			/**
			@brief 自身截取小数部分*/
			Float256& SelfGetDecimalPart();
			/**
			@brief 转换为字符串描述*/
			std::string ToString(int length = 0)const;
			/**
			@brief 常量0*/
			const static Float256& Zero();
			/**
			@brief 常量1*/
			const static Float256& One();
			/**
			@brief 常量2*/
			const static Float256& Two();
			/**
			@brief 常量3*/
			const static Float256& Three();
			/**
			@brief 常量4*/
			const static Float256& Four();
			/**
			@brief 常量5*/
			const static Float256& Five();
			/**
			@brief 常量6*/
			const static Float256& Six();
			/**
			@brief 常量7*/
			const static Float256& Seven();
			/**
			@brief 常量8*/
			const static Float256& Eight();
			/**
			@brief 常量9*/
			const static Float256& Nine();
			/**
			@brief 常量10*/
			const static Float256& Ten();
			/**
			@brief 10^(2^(x))，其中x为0~53*/
			const static Float256*const _10e2();
			/**
			@brief 对2取自然数e为底的对数*/
			const static Float256& Ln2();
			/**
			@brief 对10取自然数e为底的对数*/
			const static Float256& Ln10();
			/**
			@brief 2开根号x次 : 2^(1/(2^x))*/
			const static Float256*const _2sqrt2();
			/**
			@brief 2开根号x次的倒数 : 1/(2^(1/(2^x)))*/
			const static Float256*const _12sqrt2();
			/**
			@brief ln(2^(1/(2^x)))=(ln2)/2^x*/
			const static Float256*const _ln2sqrt2();
			/**
			@brief 最大值*/
			const static Float256& MaxValue();
			/**
			@brief 最小值*/
			const static Float256& MinValue();
			/**
			@brief 自然数e*/
			const static Float256& E();
			/**
			@brief 圆周率π*/
			const static Float256& Pi();
			/**
			@brief 二分之π*/
			const static Float256& _pi_half();
			/**
			@brief 四分之π*/
			const static Float256& _pi_half2();
			/**
			@brief 根号2*/
			const static Float256& Sqrt2();
			/**
			@brief 二分之根号2*/
			const static Float256& _sqrt2_half();
			/**
			@brief 一百八十分之π*/
			const static Float256& _pi_180();
		};
	}
	namespace Conversion {
		template<>struct Convert <std::string , Math::Float256>{
			static inline std::string from(Math::Float256 const&l) { return l.ToString(DefParam[0]); }
			static int DefParam[1];
		};
	}
}
namespace std
{
	inline ::LK::Math::Float256 fmod(::LK::Math::Float256 const& f1, ::LK::Math::Float256 const& f2) { return f1.Mod(f2); }
	inline ::LK::Math::Float256 sin(::LK::Math::Float256 const& f) { return f.Sin(); }
	inline ::LK::Math::Float256 cos(::LK::Math::Float256 const& f) { return f.Cos(); }
	inline ::LK::Math::Float256 tan(::LK::Math::Float256 const& f) { return f.Tan(); }
	inline ::LK::Math::Float256 log(::LK::Math::Float256 const& f) { return f.Ln(); }
	inline ::LK::Math::Float256 log10(::LK::Math::Float256 const& f) { return f.Lg(); }
	inline ::LK::Math::Float256 pow(::LK::Math::Float256 const& f1, ::LK::Math::Float256 const& f2) { return f1.Pow(f2); }
	inline ::LK::Math::Float256 asin(::LK::Math::Float256 const& f) { return f.Arcsin(); }
	inline ::LK::Math::Float256 acos(::LK::Math::Float256 const& f) { return f.Arccos(); }
	inline ::LK::Math::Float256 atan(::LK::Math::Float256 const& f) { return f.Arctan(); }
	inline ::LK::Math::Float256 sqrt(::LK::Math::Float256 const& f) { return f.Sqrt(); }
	inline ::LK::Math::Float256 abs(::LK::Math::Float256 const& f) { return f.Abs(); }
	inline ::LK::Math::Float256 floor(::LK::Math::Float256 const& f) { ::LK::Math::Float256 ff = f.GetIntegerPart(); return f.Sign <= 0 ? ((ff == f) ? f : ff - ::LK::Math::Float256::One()) : ff; }
	inline ::LK::Math::Float256 ceil(::LK::Math::Float256 const& f) { ::LK::Math::Float256 ff = f.GetIntegerPart(); return f.Sign <= 0 ? ff : ((ff == f) ? f : ff + ::LK::Math::Float256::One()); }
}

#undef HAS_SIGNED_TYPES_EXCEPT64
#undef HAS_UNSIGNED_TYPES_EXCEPT64
#undef HAS_ALL_TYPES
